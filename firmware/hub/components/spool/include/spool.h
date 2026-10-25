// Durable store for reports and events while the uplink is down.
//
// The apiary has no mains power and no Wi-Fi, and LTE coverage there is not
// guaranteed. The hub must hold a backlog and drain it when the modem comes
// back, otherwise "30 days unattended" means "up to 30 days of lost data".
//
// Raw records are also the research output: the field evaluation needs the
// full per-hive history, not just the alerts that fired.
#pragma once

#include <stdint.h>
#include "esp_err.h"
#include "beesense_proto.h"
#include "detect.h"

esp_err_t spool_init(void);

esp_err_t spool_append_report(const bs_report_t *report);
esp_err_t spool_append_event(const detect_result_t *event);

// Oldest unsent record, for the uplink to drain. ESP_ERR_NOT_FOUND when empty.
esp_err_t spool_peek(uint8_t *buf, uint32_t buf_len, uint32_t *out_len);
esp_err_t spool_commit(void);  // uplink confirmed -> advance the read cursor

uint32_t spool_pending_count(void);
