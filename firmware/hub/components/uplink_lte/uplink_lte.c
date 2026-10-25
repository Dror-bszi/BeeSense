#include "uplink_lte.h"

#include "esp_log.h"

static const char *TAG = "uplink";

esp_err_t uplink_lte_init(void)
{
    ESP_LOGI(TAG, "init: UART to the modem, probe with AT, read IMEI/SIM state");
    ESP_LOGI(TAG, "init: TODO -- exact modem model is still unidentified (see docs TODO, HW-16)");
    return ESP_OK;
}

esp_err_t uplink_lte_connect(void)
{
    ESP_LOGI(TAG, "connect: power key, wait for network attach, bring up the PPP/data context");
    ESP_LOGI(TAG, "connect: log RSSI -- apiary coverage is a field unknown worth recording");
    return ESP_OK;
}

void uplink_lte_disconnect(void)
{
    ESP_LOGI(TAG, "disconnect: close the context and power the modem down (the hub's biggest load)");
}

bool uplink_lte_is_up(void)
{
    return false;
}

esp_err_t uplink_lte_flush_spool(void)
{
    ESP_LOGI(TAG, "flush: batch spooled records, POST to the cloud, spool_commit() per accepted record");
    ESP_LOGI(TAG, "flush: never commit on a timeout -- resending a report beats losing it");
    return ESP_OK;
}

esp_err_t uplink_lte_send_alert(const detect_result_t *event)
{
    ESP_LOGI(TAG, "alert: connect now if needed and push immediately -- do not wait for the batch window");
    (void)event;
    return ESP_OK;
}
