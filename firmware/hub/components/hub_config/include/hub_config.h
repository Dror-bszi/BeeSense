// Hub identity, the hive roster, and the detection thresholds.
//
// Thresholds are config, not constants: PDR 3.3 tunes them during the first
// weeks of the field trial and then FREEZES them before the evaluation
// window. Keeping them here (and in NVS) makes that freeze a recorded act
// rather than a code edit.
#pragma once

#include <stdint.h>
#include "esp_err.h"

#define HUB_MAX_HIVES 8   // one hub serves a group of adjacent hives

typedef struct {
    uint8_t hive_count;
    uint8_t hive_mac[HUB_MAX_HIVES][6];
    uint8_t hive_id[HUB_MAX_HIVES];

    // Pre-swarm rule (PDR 3.1): band energy in the swarming band AND in-hive
    // temperature both above the hive's own baseline.
    float   z_threshold;          // default 3.0
    uint8_t swarm_band_lo;        // first band index of the ~100-300 Hz range
    uint8_t swarm_band_hi;        // last band index

    // Departure rule: a prime swarm weighs ~1-2 kg and leaves within minutes.
    int32_t  departure_drop_g;    // default 800 g
    uint16_t departure_window_s;  // default 1800 s (tau = 30 min)

    uint16_t baseline_days;       // default 7
    uint8_t  wifi_channel;        // ESP-NOW and the uplink must share a channel
} hub_config_t;

esp_err_t hub_config_load(hub_config_t *out);
esp_err_t hub_config_save(const hub_config_t *cfg);

// Roster index for a sender MAC, or -1 if the node is not enrolled.
int hub_config_hive_index(const hub_config_t *cfg, const uint8_t mac[6]);
