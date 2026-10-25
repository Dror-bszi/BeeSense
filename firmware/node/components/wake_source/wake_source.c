#include "wake_source.h"

#include "esp_log.h"

static const char *TAG = "wake";

bs_wake_reason_t wake_source_get(void)
{
    ESP_LOGI(TAG, "get: map esp_sleep_get_wakeup_cause() -> boot / timer / piezo-event");
    // TODO: ESP_SLEEP_WAKEUP_TIMER -> BS_WAKE_TIMER,
    //       ESP_SLEEP_WAKEUP_GPIO  -> BS_WAKE_PIEZO_EVENT, else BS_WAKE_BOOT.
    return BS_WAKE_BOOT;
}

bool wake_source_in_holdoff(uint16_t holdoff_s)
{
    ESP_LOGI(TAG, "holdoff: compare now against last event time in RTC memory (%u s window)",
             (unsigned)holdoff_s);
    return false;
}

void wake_source_mark_event(void)
{
    ESP_LOGI(TAG, "mark: stamp this event in RTC memory and bump today's event counter");
}

uint16_t wake_source_event_count_today(void)
{
    ESP_LOGI(TAG, "count: event wakes so far today (drives average current, see eq. 3)");
    return 0;
}
