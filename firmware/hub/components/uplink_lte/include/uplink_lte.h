// Hub -> cloud over a cellular modem and a SIM. One modem and one SIM serve
// the whole group of hives, which is what keeps the recurring cost per hive
// near zero.
//
// Using LTE rather than the ESP32-S3's own Wi-Fi is also a deliberate radio
// decision: ESP-NOW and station-mode Wi-Fi must share a channel on one radio,
// and a separate modem removes that coupling completely.
//
// The modem is the hub's biggest consumer, so it is duty-cycled: batch the
// backlog, raise the link, drain, drop the link. Alerts are the exception --
// an approaching swarm is worth an immediate connection.
#pragma once

#include <stdbool.h>
#include "esp_err.h"
#include "detect.h"

esp_err_t uplink_lte_init(void);

// Power the modem, attach to the network, open the data context.
esp_err_t uplink_lte_connect(void);
void uplink_lte_disconnect(void);
bool uplink_lte_is_up(void);

// Drain the spool to the cloud, committing each record only once accepted.
esp_err_t uplink_lte_flush_spool(void);

// Out-of-band, does not wait for the next batch window.
esp_err_t uplink_lte_send_alert(const detect_result_t *event);
