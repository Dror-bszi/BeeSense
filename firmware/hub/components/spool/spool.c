#include "spool.h"

#include "esp_log.h"

static const char *TAG = "spool";

esp_err_t spool_init(void)
{
    ESP_LOGI(TAG, "init: mount SD card; fall back to an internal-flash ring if absent");
    ESP_LOGI(TAG, "init: append-only log + a persisted read cursor (survives power loss)");
    return ESP_OK;
}

esp_err_t spool_append_report(const bs_report_t *report)
{
    ESP_LOGI(TAG, "append: raw report -> daily file; this is the research record, keep everything");
    (void)report;
    return ESP_OK;
}

esp_err_t spool_append_event(const detect_result_t *event)
{
    ESP_LOGI(TAG, "append: detection event -> event log, queued ahead of raw reports");
    (void)event;
    return ESP_OK;
}

esp_err_t spool_peek(uint8_t *buf, uint32_t buf_len, uint32_t *out_len)
{
    ESP_LOGI(TAG, "peek: oldest unsent record at the read cursor (events before reports)");
    (void)buf;
    (void)buf_len;
    (void)out_len;
    return ESP_OK;
}

esp_err_t spool_commit(void)
{
    ESP_LOGI(TAG, "commit: uplink confirmed -- advance and persist the read cursor");
    return ESP_OK;
}

uint32_t spool_pending_count(void)
{
    ESP_LOGI(TAG, "pending: unsent record count (reported upstream as hub health)");
    return 0;
}
