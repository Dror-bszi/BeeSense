// Per-node identity, pin map and tuning, persisted in NVS so a node can be
// re-tuned in the field (or via a hub ack) without reflashing.
#pragma once

#include <stdint.h>
#include "esp_err.h"

// Pin map -- ESP32-C3 SuperMini. ALL TBD: fix these once the node board is
// wired, and keep the piezo wake pin on an RTC-capable GPIO (see power_mgr).
#define NODE_PIN_PIEZO_ADC    0   // TBD: ADC1 channel, piezo via buffer amp
#define NODE_PIN_PIEZO_WAKE   2   // TBD: must be RTC/deep-sleep wake capable
#define NODE_PIN_AMP_ENABLE   3   // TBD: powers the piezo amp only while recording
#define NODE_PIN_MIC_I2S_SCK  4   // TBD
#define NODE_PIN_MIC_I2S_WS   5   // TBD
#define NODE_PIN_MIC_I2S_SD   6   // TBD
#define NODE_PIN_HX711_DOUT   7   // TBD
#define NODE_PIN_HX711_SCK    8   // TBD: hold high to park HX711 in power-down
#define NODE_PIN_ONEWIRE      9   // TBD: DS18B20 bus, in-hive + ambient
#define NODE_PIN_VBAT_ADC     1   // TBD: divider off the 18650

typedef struct {
    uint8_t  hive_id;
    uint8_t  hub_mac[6];
    uint16_t period_s;          // default BS_PERIOD_SECONDS
    uint16_t event_holdoff_s;   // default BS_EVENT_HOLDOFF_SEC
    int32_t  scale_offset;      // HX711 raw counts at zero load (tare)
    float    scale_counts_per_g;// from reference-mass calibration
    uint16_t vbat_cutoff_mv;    // below this, stop acquiring and only beacon
} node_config_t;

esp_err_t node_config_load(node_config_t *out);
esp_err_t node_config_save(const node_config_t *cfg);
