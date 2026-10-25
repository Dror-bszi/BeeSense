// Spectral front end -- PDR eq. (1).
//
// The ESP32-C3 is a single-core RISC-V with NO floating-point unit, so this is
// deliberately fixed point: a Q15 radix-2 FFT, magnitude-squared accumulated
// into K = 24 bands between 50 and 800 Hz, then normalized by the total.
//
// Normalizing is what makes the band vector comparable over weeks: it divides
// out how well the piezo happens to be coupled to the comb, which changes as
// the bees propolize around the sensor.
//
// Runtime on the C3 is an open question and an explicit Stage 1 bench test:
// start at 1024 points / 2 kHz and measure (see tools/bench_fft_timing.py).
#pragma once

#include <stdint.h>
#include <stddef.h>
#include "esp_err.h"
#include "beesense_proto.h"

#define DSP_FFT_SIZE 1024  // starting point; raise only if the C3 has headroom

// samples -> normalized band energies e_k as Q0.16 (65535 == 1.0).
// Averages the PSD over all windows in the record before banding.
esp_err_t dsp_fft_band_energies(const int16_t *samples, size_t sample_count,
                                uint32_t sample_rate_hz,
                                uint16_t bands_out[BS_FFT_BANDS]);

// Broadband RMS of the record, kept alongside the normalized bands: the shape
// vector alone cannot tell a loud hive from a quiet one.
uint32_t dsp_fft_rms(const int16_t *samples, size_t sample_count);
