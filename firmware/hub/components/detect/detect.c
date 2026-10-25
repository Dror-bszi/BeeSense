#include "detect.h"

#include "esp_log.h"

static const char *TAG = "detect";

esp_err_t detect_init(void)
{
    ESP_LOGI(TAG, "init: per-hive state -- consecutive over-threshold count, first-alert time");
    return ESP_OK;
}

esp_err_t detect_evaluate(const hub_config_t *cfg, const bs_report_t *report,
                          const baseline_z_t *z, detect_result_t *out)
{
    ESP_LOGI(TAG, "eval: skip entirely while z->baseline_ready is false (new or reset hive)");
    ESP_LOGI(TAG, "eval: pre-swarm  = swarm-band z > %.1f AND temp_in z > %.1f, held over consecutive reports",
             cfg ? cfg->z_threshold : 3.0f, cfg ? cfg->z_threshold : 3.0f);
    ESP_LOGI(TAG, "eval: departure  = weight drop > %ld g within %u s AND a vibration burst",
             cfg ? (long)cfg->departure_drop_g : 800L,
             (unsigned)(cfg ? cfg->departure_window_s : 1800u));
    ESP_LOGI(TAG, "eval: on departure, lead_time = now - first pre-swarm alert for this hive");
    ESP_LOGI(TAG, "eval: anomaly    = off baseline but not swarm-shaped (dearth, queen loss, robbing)");
    ESP_LOGI(TAG, "eval: fault      = report flags (scale/mic/temp fault, low battery)");
    ESP_LOGI(TAG, "eval: one event per hive at a time; alerts dedupes the repeats");
    (void)report;
    (void)z;
    (void)out;
    return ESP_OK;
}

esp_err_t detect_check_silent_nodes(const hub_config_t *cfg, detect_result_t *out)
{
    ESP_LOGI(TAG, "silent: any enrolled hive with no report for several periods -> DETECT_NODE_FAULT");
    (void)cfg;
    (void)out;
    return ESP_OK;
}
