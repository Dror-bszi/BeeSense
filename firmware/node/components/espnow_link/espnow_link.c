#include "espnow_link.h"

#include "esp_log.h"

static const char *TAG = "espnow";

esp_err_t espnow_link_init(const node_config_t *cfg)
{
    ESP_LOGI(TAG, "init: Wi-Fi in STA mode, no connect; esp_now_init(); add hub as peer");
    ESP_LOGI(TAG, "init: pin the channel to the hub's -- ESP-NOW and the hub's uplink must agree");
    (void)cfg;
    return ESP_OK;
}

esp_err_t espnow_link_send(const bs_report_t *report, bs_ack_t *ack_out)
{
    ESP_LOGI(TAG, "send: one unicast report (~%u B, well inside the 250 B ESP-NOW payload)",
             (unsigned)sizeof(bs_report_t));
    ESP_LOGI(TAG, "send: wait for the send callback, then for the hub's ack (short timeout)");
    ESP_LOGI(TAG, "send: retry a few times with backoff; on failure the caller spools it");
    ESP_LOGI(TAG, "send: ack carries hub time -> correct the node clock (no RTC on the node)");
    (void)report;
    (void)ack_out;
    return ESP_OK;
}

esp_err_t espnow_link_flush_backlog(void)
{
    ESP_LOGI(TAG, "flush: resend reports held in RTC memory, oldest first, flagged as replay");
    return ESP_OK;
}

void espnow_link_deinit(void)
{
    ESP_LOGI(TAG, "deinit: esp_now_deinit() + esp_wifi_stop() -- the radio must be off before sleep");
}
