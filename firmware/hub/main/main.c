// BeeSense apiary hub -- ESP32-S3, solar powered, always on.
//
// Unlike the nodes this never sleeps: it is the receiver, so it has to be
// listening whenever a node decides to talk. That is exactly why the hub gets
// the 18 V / 15 W panel, a charger and a Li-ion pack -- it draws about 0.5 W
// against the panel's ~45 Wh/day, and the nodes get to stay on one cell.
//
// It is also where the thinking happens. The S3 has an FPU and vector
// instructions, a week of history for every hive, and no power budget to
// protect, so the baseline and the swarm rules run here rather than on the
// C3 nodes.
//
//   ESP-NOW rx (node report)
//     |
//     +-- validate, map sender MAC -> hive, ack with hub time
//     |
//     +-- queue -> report task  (the receive callback must stay short)
//            |
//            +-- baseline: z = (x - mu_h) / sigma_h  for this hive, this hour  [eq. 2]
//            |
//            +-- detect:   pre-swarm rule | departure rule | anomaly | fault
//            |
//            +-- spool:    append raw report (the research record) and any event
//            |
//            '-- alerts:   dedupe, rank, send now if urgent
//
//   uplink task (periodic)       -- raise LTE, drain the spool, drop the link
//   housekeeping task (periodic) -- silent-node check, baseline checkpoint, stats

#include <string.h>

#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"
#include "freertos/task.h"

#include "esp_log.h"
#include "nvs_flash.h"

#include "alerts.h"
#include "baseline.h"
#include "beesense_proto.h"
#include "detect.h"
#include "espnow_rx.h"
#include "hub_config.h"
#include "spool.h"
#include "uplink_lte.h"

static const char *TAG = "hub";

#define HUB_UPLINK_PERIOD_MS      (15 * 60 * 1000)
#define HUB_HOUSEKEEPING_PERIOD_MS (60 * 1000)

typedef struct {
    uint8_t     mac[6];
    bs_report_t report;
} hub_queued_report_t;

static hub_config_t s_cfg;
static QueueHandle_t s_report_queue;

// ESP-NOW callback context: copy and get out. Anything slow here stalls the
// radio and starts costing packets against the 95% delivery target.
static void on_report(const uint8_t mac[6], const bs_report_t *report)
{
    ESP_LOGI(TAG, "rx: report from hive %u -- queueing for the report task", (unsigned)report->hive_id);
    hub_queued_report_t item = {0};
    memcpy(item.mac, mac, 6);
    item.report = *report;
    if (xQueueSend(s_report_queue, &item, 0) != pdTRUE) {
        ESP_LOGW(TAG, "rx: queue full -- spool directly rather than dropping the report");
    }
}

static void report_task(void *arg)
{
    hub_queued_report_t item;
    ESP_LOGI(TAG, "report_task: started -- baseline -> detect -> spool -> alerts");

    while (1) {
        if (xQueueReceive(s_report_queue, &item, portMAX_DELAY) != pdTRUE) {
            continue;
        }
        ESP_LOGI(TAG, "--- processing report: hive %u, wake=%s ---",
                 (unsigned)item.report.hive_id,
                 bs_wake_reason_str((bs_wake_reason_t)item.report.wake_reason));

        // Acknowledge first: the node is burning battery waiting for this.
        bs_ack_t ack = { .version = BS_PROTO_VERSION, .ack_seq = item.report.seq };
        espnow_rx_send_ack(item.mac, &ack);

        baseline_z_t z = {0};
        ESP_ERROR_CHECK(baseline_evaluate(&item.report, &z));

        detect_result_t result = {0};
        ESP_ERROR_CHECK(detect_evaluate(&s_cfg, &item.report, &z, &result));

        // Always keep the raw report, alert or not -- the field evaluation
        // needs the full history, including the quiet weeks.
        ESP_ERROR_CHECK(spool_append_report(&item.report));

        if (result.event != DETECT_NONE) {
            ESP_LOGI(TAG, "detect: event %d on hive %u (score %.2f)",
                     (int)result.event, (unsigned)result.hive_id, (double)result.score);
            ESP_ERROR_CHECK(spool_append_event(&result));
            ESP_ERROR_CHECK(alerts_dispatch(&result));
        }
        ESP_LOGI(TAG, "--- report done ---");
    }
}

static void uplink_task(void *arg)
{
    ESP_LOGI(TAG, "uplink_task: started -- duty-cycled, the modem is the hub's biggest load");
    while (1) {
        if (spool_pending_count() > 0) {
            ESP_LOGI(TAG, "uplink: %u records pending -- raising the link",
                     (unsigned)spool_pending_count());
            if (uplink_lte_connect() == ESP_OK) {
                uplink_lte_flush_spool();
            }
            uplink_lte_disconnect();
        }
        vTaskDelay(pdMS_TO_TICKS(HUB_UPLINK_PERIOD_MS));
    }
}

static void housekeeping_task(void *arg)
{
    ESP_LOGI(TAG, "housekeeping_task: started");
    while (1) {
        // A node gone silent is itself an alert: with 30 days unattended,
        // nobody is at the apiary to notice a dead node.
        detect_result_t fault = {0};
        if (detect_check_silent_nodes(&s_cfg, &fault) == ESP_OK &&
            fault.event != DETECT_NONE) {
            alerts_dispatch(&fault);
        }
        baseline_checkpoint();
        alerts_log_stats();
        vTaskDelay(pdMS_TO_TICKS(HUB_HOUSEKEEPING_PERIOD_MS));
    }
}

void app_main(void)
{
    ESP_LOGI(TAG, "BeeSense apiary hub (proto v%d) -- scaffold, no real I/O yet",
             BS_PROTO_VERSION);

    ESP_ERROR_CHECK(nvs_flash_init());
    ESP_ERROR_CHECK(hub_config_load(&s_cfg));
    ESP_ERROR_CHECK(spool_init());
    ESP_ERROR_CHECK(baseline_init());
    ESP_ERROR_CHECK(detect_init());
    ESP_ERROR_CHECK(alerts_init());
    ESP_ERROR_CHECK(uplink_lte_init());

    s_report_queue = xQueueCreate(16, sizeof(hub_queued_report_t));
    ESP_ERROR_CHECK(s_report_queue ? ESP_OK : ESP_ERR_NO_MEM);

    ESP_ERROR_CHECK(espnow_rx_start(&s_cfg, on_report));

    xTaskCreate(report_task,       "report",  4096, NULL, 6, NULL);
    xTaskCreate(uplink_task,       "uplink",  4096, NULL, 4, NULL);
    xTaskCreate(housekeeping_task, "house",   3072, NULL, 3, NULL);

    ESP_LOGI(TAG, "hub up: listening for %u enrolled hives", (unsigned)s_cfg.hive_count);
}
