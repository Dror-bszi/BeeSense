// Turns detection events into something a beekeeper acts on.
//
// The acceptance target is <= 1 false alarm per hive-week, so suppression is
// as much a part of this component as sending: a hive that is preparing to
// swarm will trip the rule on report after report, and that must be ONE alert
// with updates, not forty messages.
//
// The useful message is not "anomaly on hive 3" -- it is which hive to open
// first and why, since the whole point is to direct a visit.
#pragma once

#include "esp_err.h"
#include "detect.h"

esp_err_t alerts_init(void);

// Dedupes and rate-limits, then hands off to the uplink.
esp_err_t alerts_dispatch(const detect_result_t *event);

// Per-hive counters, so the false-alarm rate is measured rather than guessed.
void alerts_log_stats(void);
