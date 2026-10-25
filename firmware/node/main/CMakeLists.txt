idf_component_register(SRCS "main.c"
                       INCLUDE_DIRS ""
                       REQUIRES beesense_proto node_config wake_source piezo_adc
                                mic_i2s dsp_fft scale_hx711 temp_probe
                                espnow_link power_mgr nvs_flash)
