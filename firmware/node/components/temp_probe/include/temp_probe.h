// DS18B20 probes on one 1-Wire bus: one inside the hive, one ambient.
//
// In-hive temperature is half of the pre-swarm rule (PDR 3.1) -- the alert
// needs band energy AND temperature above baseline. Ambient is the reference
// that separates "the colony is warming" from "it is a hot afternoon", and
// also feeds load-cell drift compensation.
#pragma once

#include <stdint.h>
#include "esp_err.h"

esp_err_t temp_probe_init(void);

// Both probes in one bus conversion. Either may come back as a fault; the
// report carries BS_FLAG_TEMP_FAULT rather than a fabricated value.
esp_err_t temp_probe_read(int16_t *in_c100, int16_t *out_c100);
