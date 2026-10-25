#include "power_mgr.h"

#include "esp_log.h"
#include "beesense_proto.h"

static const char *TAG = "power";

esp_err_t power_mgr_init(void)
{
    ESP_LOGI(TAG, "init: unused pins to known states; hold peripheral enables low");
    return ESP_OK;
}

uint16_t power_mgr_vbat_mv(void)
{
    ESP_LOGI(TAG, "vbat: one ADC read of the divider on GPIO%d, then disable the divider",
             NODE_PIN_VBAT_ADC);
    ESP_LOGI(TAG, "vbat: below cfg->vbat_cutoff_mv, skip acquisition and only beacon");
    return 0;
}

void power_mgr_sleep(const node_config_t *cfg)
{
    ESP_LOGI(TAG, "sleep: radio off, HX711 parked, piezo amp off, I2S stopped");
    ESP_LOGI(TAG, "sleep: arm timer for the rest of the %d s period (no drift accumulation)",
             BS_PERIOD_SECONDS);
    ESP_LOGI(TAG, "sleep: arm GPIO%d wake from the ~2 uA piezo wake circuit",
             NODE_PIN_PIEZO_WAKE);
    ESP_LOGI(TAG, "sleep: target I0 ~= 22 uA -- MEASURE the board, the LED/regulator may dominate");
    (void)cfg;
    // TODO: esp_sleep_enable_timer_wakeup + esp_deep_sleep_enable_gpio_wakeup,
    //       then esp_deep_sleep_start(). Never returns.
    ESP_LOGI(TAG, "sleep: esp_deep_sleep_start() -- execution restarts at app_main on wake");
    while (1) { }
}
