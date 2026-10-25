// Hive scale: four 50 kg half-bridge load cells at the corners of a base
// board, wired together into one full Wheatstone bridge and read by a single
// HX711 (gain 128, channel A). 4 x 50 kg = 200 kg, enough for a full hive with
// honey and supers.
//
// The HX711's own noise is worth about 3 g, well inside the +-20 g target, so
// the accuracy limit in the field is mechanical and thermal: cells must be
// mounted so only their centre carries load, and drift with temperature is the
// error that actually matters. Hence the ambient probe and the temperature
// sweep in the Stage 1 bench tests.
#pragma once

#include <stdint.h>
#include "esp_err.h"
#include "node_config.h"

esp_err_t scale_hx711_init(void);

// Median of N readings, converted with the stored tare and counts/g.
esp_err_t scale_hx711_read_grams(const node_config_t *cfg, int32_t *grams_out);

// Capture the current raw counts as zero load (empty platform).
esp_err_t scale_hx711_tare(node_config_t *cfg);

// Derive counts_per_g from a known reference mass.
esp_err_t scale_hx711_calibrate(node_config_t *cfg, int32_t known_grams);

void scale_hx711_power_down(void);
