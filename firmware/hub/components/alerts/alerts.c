#include "alerts.h"

#include "esp_log.h"

static const char *TAG = "alerts";

esp_err_t alerts_init(void)
{
    ESP_LOGI(TAG, "init: per-hive last-alert time and open-alert state");
    return ESP_OK;
}

esp_err_t alerts_dispatch(const detect_result_t *event)
{
    ESP_LOGI(TAG, "dispatch: suppress if this hive already has the same alert open");
    ESP_LOGI(TAG, "dispatch: rate-limit to protect the <= 1 false alarm / hive-week budget");
    ESP_LOGI(TAG, "dispatch: compose -- hive, what fired, how far off baseline, what to check");
    ESP_LOGI(TAG, "dispatch: rank hives so the beekeeper knows which to open FIRST");
    ESP_LOGI(TAG, "dispatch: uplink_lte_send_alert(), and always spool it whether or not it sent");
    (void)event;
    return ESP_OK;
}

void alerts_log_stats(void)
{
    ESP_LOGI(TAG, "stats: alerts per hive per week, and TP/FP once ground truth is entered");
}
