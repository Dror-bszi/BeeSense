// Piezo vibration channel: a 27 mm disc pressed into the comb at the centre of
// the hive, where comb vibration is strongest.
//
// Signal chain: high-impedance piezo -> charge/buffer amp biased at VCC/2 ->
// protection diodes -> ESP32-C3 ADC. The amp is powered only while recording
// (NODE_PIN_AMP_ENABLE); between records only the nanopower wake comparator
// stays alive, which is the whole basis of the ~1 mW average figure.
//
// Night-time comb vibration is 1-5 um/s^2 per bin, so the circuit noise floor
// is the thing to beat -- that is the first Stage 1 bench test.
#pragma once

#include <stdint.h>
#include <stddef.h>
#include "esp_err.h"

#define PIEZO_SAMPLE_RATE_HZ 2000  // >= 2x the 800 Hz top band

esp_err_t piezo_adc_init(void);

// Records BS_RECORD_SECONDS into caller-provided storage. Blocking: the CPU
// being awake for these 10 s is the dominant term in Q ~= 0.3 A*s per
// acquisition, so nothing else should run here.
esp_err_t piezo_adc_record(int16_t *samples, size_t sample_count);

void piezo_adc_deinit(void);
