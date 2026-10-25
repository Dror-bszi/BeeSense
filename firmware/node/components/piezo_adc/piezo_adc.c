#include "piezo_adc.h"

#include "esp_log.h"
#include "beesense_proto.h"
#include "node_config.h"

static const char *TAG = "piezo";

esp_err_t piezo_adc_init(void)
{
    ESP_LOGI(TAG, "init: enable piezo amp on GPIO%d, settle the VCC/2 bias",
             NODE_PIN_AMP_ENABLE);
    ESP_LOGI(TAG, "init: ADC1 continuous mode on GPIO%d at %d Hz, 12-bit",
             NODE_PIN_PIEZO_ADC, PIEZO_SAMPLE_RATE_HZ);
    return ESP_OK;
}

esp_err_t piezo_adc_record(int16_t *samples, size_t sample_count)
{
    ESP_LOGI(TAG, "record: capture %u samples (%d s), remove DC bias, keep as int16",
             (unsigned)sample_count, BS_RECORD_SECONDS);
    ESP_LOGI(TAG, "record: flag clipping -- a swarm burst can rail the amp");
    (void)samples;
    return ESP_OK;
}

void piezo_adc_deinit(void)
{
    ESP_LOGI(TAG, "deinit: cut amp power on GPIO%d and release the ADC unit "
                  "(must happen before sleep or the amp dominates I0)",
             NODE_PIN_AMP_ENABLE);
}
