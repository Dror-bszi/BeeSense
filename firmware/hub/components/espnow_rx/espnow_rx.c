#include "espnow_rx.h"

#include "esp_log.h"

static const char *TAG = "espnow_rx";

esp_err_t espnow_rx_start(const hub_config_t *cfg, espnow_rx_cb_t on_report)
{
    ESP_LOGI(TAG, "start: Wi-Fi STA, lock to channel %u, esp_now_init(), register recv cb",
             (unsigned)(cfg ? cfg->wifi_channel : 0));
    ESP_LOGI(TAG, "start: add each enrolled node as a peer; drop frames from unknown MACs");
    ESP_LOGI(TAG, "start: recv cb validates with bs_report_validate() then queues -- no work in the cb");
    (void)on_report;
    return ESP_OK;
}

esp_err_t espnow_rx_send_ack(const uint8_t mac[6], const bs_ack_t *ack)
{
    ESP_LOGI(TAG, "ack: reply with hub epoch (node clock correction) and next_period_s");
    (void)mac;
    (void)ack;
    return ESP_OK;
}

float espnow_rx_delivery_ratio(uint8_t hive_id)
{
    ESP_LOGI(TAG, "delivery: gaps in the seq counter for hive %u -> received/expected",
             (unsigned)hive_id);
    ESP_LOGI(TAG, "delivery: replayed reports count as delivered (they arrived, just late)");
    return 0.0f;
}
