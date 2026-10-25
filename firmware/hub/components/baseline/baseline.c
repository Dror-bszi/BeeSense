#include "baseline.h"

#include "esp_log.h"

static const char *TAG = "baseline";

esp_err_t baseline_init(void)
{
    ESP_LOGI(TAG, "init: allocate [hive][hour-of-day] running mean/variance, restore checkpoint");
    return ESP_OK;
}

esp_err_t baseline_evaluate(const bs_report_t *report, baseline_z_t *z_out)
{
    ESP_LOGI(TAG, "eval: bucket by hive_id and hour-of-day from report epoch");
    ESP_LOGI(TAG, "eval: z = (x - mu_h) / sigma_h over the trailing 7 days  [eq. 2]");
    ESP_LOGI(TAG, "eval: score the swarm-band energy, in-hive temperature and weight");
    ESP_LOGI(TAG, "eval: DW(t) = W(t) - W(t - tau) from the stored weight history");
    ESP_LOGI(TAG, "eval: THEN update the running stats with this sample (score first, fold after)");
    ESP_LOGI(TAG, "eval: baseline_ready stays false until the hive has enough samples in the bucket");
    (void)report;
    (void)z_out;
    return ESP_OK;
}

esp_err_t baseline_checkpoint(void)
{
    ESP_LOGI(TAG, "checkpoint: write stats to flash/SD -- a lost baseline blinds the hub for a week");
    return ESP_OK;
}
