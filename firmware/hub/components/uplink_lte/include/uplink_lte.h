// Hub -> cloud over a SIMCom A7608E-H (LTE Cat-4, EMEA bands) on a BK-A7608E-H
// breakout, with a full-band LTE antenna and a GNSS patch antenna. One modem
// and one SIM serve the whole group of hives, which is what keeps the
// recurring cost per hive near zero.
//
// Two consequences of this particular module:
//
//  * PEAK CURRENT. Cat-4 transmit bursts are measured in amps, not
//    milliamps, against a hub that averages ~0.5 W. The average is fine
//    (~45 Wh/day from the panel); the supply has to survive the peaks, so the
//    hub board needs bulk capacitance right at the module. Verify the real
//    figure against the A7608E-H datasheet before drawing that board.
//
//  * GNSS FOR FREE. The module has a GNSS receiver, which makes the hub a
//    proper time source -- and the hub is what every node sets its clock from
//    (bs_ack_t carries epoch_s). That means no RTC anywhere in the system and
//    timestamps that line up across hives, which matters when lead time is a
//    reported result.
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
