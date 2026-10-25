// Node -> hub radio link: ESP-NOW on 2.4 GHz, no router and no association, so
// the radio is up for milliseconds per report instead of seconds.
//
// The antenna sits OUTSIDE the hive: honey and wood attenuate 2.4 GHz badly.
// Range at the apiary is a Stage 1 field measurement (the SuperMini's chip
// antenna is small); the documented fallback is LoRa.
//
// Unsent reports are spooled to RTC memory and retried on the next wake, which
// is how a temporarily deaf hub still ends up with >= 95% delivery.
#pragma once

#include "esp_err.h"
#include "beesense_proto.h"
#include "node_config.h"

esp_err_t espnow_link_init(const node_config_t *cfg);

// Send one report and wait briefly for the hub's bs_ack_t. Returns an error if
// unacknowledged after the retry budget, so the caller can spool it.
esp_err_t espnow_link_send(const bs_report_t *report, bs_ack_t *ack_out);

// Retry anything held in RTC memory from previous wakes, oldest first,
// marked with BS_FLAG_SPOOL_REPLAY.
esp_err_t espnow_link_flush_backlog(void);

void espnow_link_deinit(void);
