#include "uplink_lte.h"

#include "esp_log.h"

static const char *TAG = "uplink";

esp_err_t uplink_lte_init(void)
{
    ESP_LOGI(TAG, "init: UART to the modem, probe with AT, read IMEI/SIM state");
    ESP_LOGI(TAG, "init: SIMCom A7608E-H (LTE Cat-4) -- AT command set per its datasheet");
    return ESP_OK;
}

esp_err_t uplink_lte_connect(void)
{
    ESP_LOGI(TAG, "connect: pulse PWRKEY, wait for the module, then for network attach");
    ESP_LOGI(TAG, "connect: bring up the PPP/data context on the carrier APN");
    ESP_LOGI(TAG, "connect: GNSS fix -> set hub time; the nodes all sync from this clock");
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
