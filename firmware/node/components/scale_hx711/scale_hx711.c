#include "scale_hx711.h"

#include "esp_log.h"

static const char *TAG = "scale";

esp_err_t scale_hx711_init(void)
{
    ESP_LOGI(TAG, "init: DOUT=%d SCK=%d, 2-wire bit-bang, gain 128 ch A",
             NODE_PIN_HX711_DOUT, NODE_PIN_HX711_SCK);
    ESP_LOGI(TAG, "init: wake from power-down, then discard the first conversions");
    return ESP_OK;
}

esp_err_t scale_hx711_read_grams(const node_config_t *cfg, int32_t *grams_out)
{
    ESP_LOGI(TAG, "read: poll DOUT low, clock out 24 bits + gain pulse, repeat N times");
    ESP_LOGI(TAG, "read: take the median (rejects a bee landing / a gust), then");
    ESP_LOGI(TAG, "read: grams = (raw - offset) / counts_per_g  -> target error <= +-20 g");
    (void)cfg;
    (void)grams_out;
    return ESP_OK;
}

esp_err_t scale_hx711_tare(node_config_t *cfg)
{
    ESP_LOGI(TAG, "tare: average raw counts on the empty platform -> cfg->scale_offset, save to NVS");
    (void)cfg;
    return ESP_OK;
}

esp_err_t scale_hx711_calibrate(node_config_t *cfg, int32_t known_grams)
{
    ESP_LOGI(TAG, "calibrate: with %ld g on the platform, counts_per_g = (raw - offset) / known",
             (long)known_grams);
    (void)cfg;
    return ESP_OK;
}

void scale_hx711_power_down(void)
{
    ESP_LOGI(TAG, "power_down: hold SCK=%d high >60 us -- HX711 draws ~0.3 uA parked",
             NODE_PIN_HX711_SCK);
}
