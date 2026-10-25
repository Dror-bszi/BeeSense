// Per-hive, per-hour-of-day baseline -- PDR eq. (2).
//
//   z = (x - mu_h) / sigma_h
//
// The hour-of-day split is the important part: colony activity follows a
// strong daily cycle, so "band energy is high" means nothing until it is
// compared against the same hive at the same hour over the past week. Each
// hive is its own reference, which also absorbs differences in sensor
// coupling and hive strength between colonies.
//
// 8 hives x 24 hours x (24 bands + weight + 2 temps) of running mean and
// variance is small enough for RAM, but it must survive a reboot -- a hub
// that loses its baseline is blind for a week.
#pragma once

#include <stdbool.h>
#include <stdint.h>
#include "esp_err.h"
#include "beesense_proto.h"

typedef struct {
    float z_piezo_swarm_band;  // z of the summed ~100-300 Hz energy
    float z_mic_swarm_band;
    float z_temp_in;
    float z_weight;
    int32_t weight_delta_g;    // DW over the departure window (eq. 2)
    bool    baseline_ready;    // false until a hive has enough history
} baseline_z_t;

esp_err_t baseline_init(void);

// Score a report against its hive's history, THEN fold it in -- scoring
// against a baseline that already contains the sample dilutes the anomaly.
esp_err_t baseline_evaluate(const bs_report_t *report, baseline_z_t *z_out);

// Persist to flash/SD periodically so a reboot does not cost a week.
esp_err_t baseline_checkpoint(void);
