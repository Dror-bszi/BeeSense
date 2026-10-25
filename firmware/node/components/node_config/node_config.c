#include "node_config.h"

#include "esp_log.h"

static const char *TAG = "node_cfg";

esp_err_t node_config_load(node_config_t *out)
{
    ESP_LOGI(TAG, "load: read hive_id, hub MAC, period, scale tare/calibration from NVS");
    ESP_LOGI(TAG, "load: fall back to compiled defaults on first boot, then save");
    // TODO: nvs_open("beesense") + nvs_get_blob; seed defaults if absent.
    (void)out;
    return ESP_OK;
}

esp_err_t node_config_save(const node_config_t *cfg)
{
    ESP_LOGI(TAG, "save: persist config to NVS (called after tare/calibration or a hub retune)");
    (void)cfg;
    return ESP_OK;
}
