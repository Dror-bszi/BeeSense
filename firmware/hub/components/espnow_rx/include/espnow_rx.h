// Receive side of the node link. The hub is mains-free but always-on (it is
// the receiver), which is why it gets the 15 W panel and the nodes do not.
//
// Channel constraint: ESP-NOW and station-mode Wi-Fi share one radio and must
// use the same channel. Using a cellular modem for the uplink side-steps that
// entirely -- which is the configuration chosen here.
#pragma once

#include "esp_err.h"
#include "beesense_proto.h"
#include "hub_config.h"

// Called from the ESP-NOW receive callback context: keep it short, hand the
// report to a queue, do the work on a task.
typedef void (*espnow_rx_cb_t)(const uint8_t mac[6], const bs_report_t *report);

esp_err_t espnow_rx_start(const hub_config_t *cfg, espnow_rx_cb_t on_report);

// Reply with hub time and any period retune, so nodes stay aligned.
esp_err_t espnow_rx_send_ack(const uint8_t mac[6], const bs_ack_t *ack);

// Delivery ratio per hive from the report sequence numbers -- this is the
// measurement behind the >= 95% packet-delivery target.
float espnow_rx_delivery_ratio(uint8_t hive_id);
