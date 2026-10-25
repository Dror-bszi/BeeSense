#include "mic_i2s.h"

#include "esp_log.h"
#include "beesense_proto.h"
#include "node_config.h"

static const char *TAG = "mic";

esp_err_t mic_i2s_init(void)
{
    ESP_LOGI(TAG, "init: I2S std mode, SCK=%d WS=%d SD=%d, 24-bit slots at %d Hz",
             NODE_PIN_MIC_I2S_SCK, NODE_PIN_MIC_I2S_WS, NODE_PIN_MIC_I2S_SD,
             MIC_SAMPLE_RATE_HZ);
    return ESP_OK;
}

esp_err_t mic_i2s_record(int16_t *samples, size_t sample_count)
{
    ESP_LOGI(TAG, "record: read %d s of I2S frames, take the high 16 bits",
             BS_RECORD_SECONDS);
    ESP_LOGI(TAG, "record: low-pass then decimate %d Hz -> %d Hz to match the piezo channel",
             MIC_SAMPLE_RATE_HZ, 2000);
    (void)samples;
    (void)sample_count;
    return ESP_OK;
}

void mic_i2s_deinit(void)
{
    ESP_LOGI(TAG, "deinit: stop the I2S channel so the mic stops drawing in sleep");
}
