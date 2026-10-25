// BeeSense hive node -- ESP32-C3, one 18650 cell, one hive.
//
// There is no main loop. The node wakes, does one acquisition, sends it, and
// goes back to deep sleep; execution restarts here on every wake. Everything
// in the 6-month battery target follows from keeping this path short.
//
//   wake (timer @15 min  |  piezo event  |  boot)
//     |
//     +-- event wake inside the 20 min hold-off? -> back to sleep, no acquisition
//     |
//     +-- battery below cutoff? -> beacon only, back to sleep
//     |
//     +-- acquire ---- piezo 10 s  (amp on only here)
//     |            |-- mic   10 s
//     |            |-- weight (4 cells -> HX711, median)
//     |            '-- temperature in + ambient
//     |
//     +-- dsp ------- Q15 FFT -> PSD -> 24 bands over 50-800 Hz -> e_k  [eq. 1]
//     |
//     +-- send ------ ESP-NOW unicast to the hub, wait for ack (clock correction)
//     |            '-- no ack? spool to RTC memory, retry next wake
//     |
//     '-- sleep ----- radio off, sensors parked, timer + piezo GPIO armed
//
// Detection does NOT happen here. The node only ever ships features; the
// baseline and the swarm rules live on the hub, which has a week of history
// for every hive and the compute to use it.

#include <string.h>

#include "esp_log.h"
#include "nvs_flash.h"

#include "beesense_proto.h"
#include "dsp_fft.h"
#include "espnow_link.h"
#include "mic_i2s.h"
#include "node_config.h"
#include "piezo_adc.h"
#include "power_mgr.h"
#include "scale_hx711.h"
#include "temp_probe.h"
#include "wake_source.h"

static const char *TAG = "node";

// 10 s at 2 kHz. Too big for the stack -- it lives in .bss, and on the C3 this
// is one of the larger claims on RAM, so the two channels are recorded and
// transformed one at a time rather than both being held at once.
#define NODE_SAMPLE_COUNT (PIEZO_SAMPLE_RATE_HZ * BS_RECORD_SECONDS)
static int16_t s_samples[NODE_SAMPLE_COUNT];

static void acquire_and_report(const node_config_t *cfg, bs_wake_reason_t reason)
{
    // bs_report_t is packed for the wire, so nothing writes into its members
    // through a pointer -- that would hand the drivers an unaligned pointer.
    // Acquire into aligned locals, then copy in.
    uint16_t piezo_bands[BS_FFT_BANDS] = {0};
    uint16_t mic_bands[BS_FFT_BANDS] = {0};
    int32_t weight_g = 0;
    int16_t temp_in_c100 = 0;
    int16_t temp_out_c100 = 0;

    ESP_LOGI(TAG, "--- acquisition start (reason=%s) ---", bs_wake_reason_str(reason));

    // Vibration channel first: it is the primary sensor and the one the wake
    // circuit fired on, so it should be sampled as close to the event as possible.
    ESP_ERROR_CHECK(piezo_adc_init());
    ESP_ERROR_CHECK(piezo_adc_record(s_samples, NODE_SAMPLE_COUNT));
    piezo_adc_deinit();
    ESP_ERROR_CHECK(dsp_fft_band_energies(s_samples, NODE_SAMPLE_COUNT,
                                          PIEZO_SAMPLE_RATE_HZ, piezo_bands));

    // Acoustic channel reuses the same sample buffer.
    ESP_ERROR_CHECK(mic_i2s_init());
    ESP_ERROR_CHECK(mic_i2s_record(s_samples, NODE_SAMPLE_COUNT));
    mic_i2s_deinit();
    ESP_ERROR_CHECK(dsp_fft_band_energies(s_samples, NODE_SAMPLE_COUNT,
                                          2000, mic_bands));

    // Slow channels: no reason to hold the radio or the piezo amp up for these.
    ESP_ERROR_CHECK(scale_hx711_init());
    ESP_ERROR_CHECK(scale_hx711_read_grams(cfg, &weight_g));
    scale_hx711_power_down();

    ESP_ERROR_CHECK(temp_probe_init());
    ESP_ERROR_CHECK(temp_probe_read(&temp_in_c100, &temp_out_c100));

    bs_report_t report = {
        .version     = BS_PROTO_VERSION,
        .hive_id     = cfg->hive_id,
        .wake_reason = (uint8_t)reason,
        .weight_g    = weight_g,
        .temp_in_c100  = temp_in_c100,
        .temp_out_c100 = temp_out_c100,
        .vbat_mv     = power_mgr_vbat_mv(),
    };
    memcpy(report.piezo_bands, piezo_bands, sizeof(piezo_bands));
    memcpy(report.mic_bands, mic_bands, sizeof(mic_bands));

    ESP_LOGI(TAG, "report: hive=%u seq=%u weight=%ld g  t_in=%d t_out=%d (0.01 C)  vbat=%u mV",
             (unsigned)report.hive_id, (unsigned)report.seq, (long)weight_g,
             temp_in_c100, temp_out_c100, (unsigned)report.vbat_mv);

    bs_ack_t ack = {0};
    ESP_ERROR_CHECK(espnow_link_init(cfg));
    if (espnow_link_send(&report, &ack) == ESP_OK) {
        ESP_LOGI(TAG, "send: acknowledged; applying hub clock and any retune");
        espnow_link_flush_backlog();
    } else {
        ESP_LOGW(TAG, "send: no ack -- spooling to RTC memory for the next wake");
        // TODO: append to the RTC-memory ring buffer.
    }
    espnow_link_deinit();

    ESP_LOGI(TAG, "--- acquisition done ---");
}

void app_main(void)
{
    ESP_LOGI(TAG, "BeeSense hive node (proto v%d) -- scaffold, no real I/O yet",
             BS_PROTO_VERSION);

    ESP_ERROR_CHECK(nvs_flash_init());
    ESP_ERROR_CHECK(power_mgr_init());

    node_config_t cfg = {0};
    ESP_ERROR_CHECK(node_config_load(&cfg));

    bs_wake_reason_t reason = wake_source_get();
    ESP_LOGI(TAG, "woke: %s (%u event wakes today)",
             bs_wake_reason_str(reason), wake_source_event_count_today());

    if (reason == BS_WAKE_PIEZO_EVENT) {
        if (wake_source_in_holdoff(cfg.event_holdoff_s)) {
            ESP_LOGI(TAG, "holdoff: inside the %d s window -- skipping acquisition",
                     BS_EVENT_HOLDOFF_SEC);
            power_mgr_sleep(&cfg);
        }
        wake_source_mark_event();
    }

    if (power_mgr_vbat_mv() < cfg.vbat_cutoff_mv) {
        ESP_LOGW(TAG, "battery below cutoff -- skipping acquisition, beacon only");
        // TODO: send a minimal low-battery report so the hub can flag the hive.
        power_mgr_sleep(&cfg);
    }

    acquire_and_report(&cfg, reason);
    power_mgr_sleep(&cfg);
}
