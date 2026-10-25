// The component the 6-month battery target lives or dies by.
//
// Budget from PDR eq. (3):
//   I_avg  = I0 + A * Q / 86400,    T_life = eta * C / I_avg
//   Q  ~= 0.3 A*s per acquisition (mostly the CPU during the 10 s record)
//   I0 ~= 22 uA  (deep sleep + the nanopower wake circuit)
// At A = 96 periodic acquisitions/day: I_avg ~= 0.35 mA, so one 3000 mAh cell
// (eta = 0.8) lasts ~285 days. The target survives up to ~60 event wakes/day.
//
// The measured number will likely be worse than the datasheet one: on a
// SuperMini the power LED and the onboard regulator can dwarf the chip. That
// is a Stage 1 bench measurement, not an assumption.
#pragma once

#include <stdint.h>
#include "esp_err.h"
#include "node_config.h"

esp_err_t power_mgr_init(void);

uint16_t power_mgr_vbat_mv(void);

// Arms both wake paths and enters deep sleep. Does not return.
//   - timer: the remainder of the 15 min period, so the cycle does not drift
//   - GPIO:  NODE_PIN_PIEZO_WAKE, the analog wake circuit's output
void power_mgr_sleep(const node_config_t *cfg) __attribute__((noreturn));
