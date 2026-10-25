#include "hub_config.h"

#include "esp_log.h"

static const char *TAG = "hub_cfg";

esp_err_t hub_config_load(hub_config_t *out)
{
    ESP_LOGI(TAG, "load: hive roster (MAC -> hive_id) and thresholds from NVS");
    ESP_LOGI(TAG, "load: defaults -> z>3, drop 800 g / 30 min, 7-day baseline");
    (void)out;
    return ESP_OK;
}

esp_err_t hub_config_save(const hub_config_t *cfg)
{
    ESP_LOGI(TAG, "save: persist roster and thresholds (record WHEN thresholds were frozen)");
    (void)cfg;
    return ESP_OK;
}

int hub_config_hive_index(const hub_config_t *cfg, const uint8_t mac[6])
{
    ESP_LOGI(TAG, "lookup: sender MAC -> roster index; unknown MACs are ignored, not enrolled");
    (void)cfg;
    (void)mac;
    return -1;
}
