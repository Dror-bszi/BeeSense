// Acoustic channel: MS3625 MEMS microphone (I2S, INMP441-compatible) inside
// the hive behind a mesh that keeps propolis and wax off the port.
//
// This is the channel most directly comparable to the literature -- the
// pre-swarm PSD rise near 110 Hz was recorded with ordinary microphones -- so
// it is the cross-check on the piezo, not a redundant sensor.
#pragma once

#include <stdint.h>
#include <stddef.h>
#include "esp_err.h"

#define MIC_SAMPLE_RATE_HZ 16000  // decimated to ~2 kHz before the FFT

esp_err_t mic_i2s_init(void);
esp_err_t mic_i2s_record(int16_t *samples, size_t sample_count);
void mic_i2s_deinit(void);
