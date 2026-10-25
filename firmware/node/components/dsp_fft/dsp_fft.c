#include "dsp_fft.h"

#include "esp_log.h"

static const char *TAG = "dsp";

esp_err_t dsp_fft_band_energies(const int16_t *samples, size_t sample_count,
                                uint32_t sample_rate_hz,
                                uint16_t bands_out[BS_FFT_BANDS])
{
    ESP_LOGI(TAG, "bands: %u samples at %u Hz -> %d-point Q15 FFT, Hann window, 50%% overlap",
             (unsigned)sample_count, (unsigned)sample_rate_hz, DSP_FFT_SIZE);
    ESP_LOGI(TAG, "bands: accumulate |X(f)|^2 per window, average over the record -> PSD S(f)");
    ESP_LOGI(TAG, "bands: sum S(f) into K=%d bands over %d-%d Hz",
             BS_FFT_BANDS, BS_BAND_F_MIN_HZ, BS_BAND_F_MAX_HZ);
    ESP_LOGI(TAG, "bands: e_k = sum(B_k) / sum(B)  [eq. 1] -> Q0.16, cancels sensor coupling");
    // TODO: fixed-point FFT (esp-dsp dsps_fft2r_sc16, or own Q15 radix-2 if the
    //       esp-dsp C3 path proves slow) -- measure before committing.
    (void)samples;
    (void)bands_out;
    return ESP_OK;
}

uint32_t dsp_fft_rms(const int16_t *samples, size_t sample_count)
{
    ESP_LOGI(TAG, "rms: broadband level of the record, integer sqrt of the mean square");
    (void)samples;
    (void)sample_count;
    return 0;
}
