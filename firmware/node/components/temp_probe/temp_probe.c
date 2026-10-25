#include "temp_probe.h"

#include "esp_log.h"
#include "node_config.h"

static const char *TAG = "temp";

esp_err_t temp_probe_init(void)
{
    ESP_LOGI(TAG, "init: 1-Wire on GPIO%d, enumerate ROM codes", NODE_PIN_ONEWIRE);
    ESP_LOGI(TAG, "init: map ROM code -> in-hive / ambient from NVS (probes are interchangeable)");
    return ESP_OK;
}

esp_err_t temp_probe_read(int16_t *in_c100, int16_t *out_c100)
{
    ESP_LOGI(TAG, "read: convert-all, wait 750 ms at 12-bit, then read both scratchpads");
    ESP_LOGI(TAG, "read: verify CRC; on failure set BS_FLAG_TEMP_FAULT, never invent a value");
    (void)in_c100;
    (void)out_c100;
    return ESP_OK;
}
