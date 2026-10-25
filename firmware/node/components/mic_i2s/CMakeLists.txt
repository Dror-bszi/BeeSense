idf_component_register(SRCS "mic_i2s.c"
                       INCLUDE_DIRS "include"
                       REQUIRES node_config beesense_proto esp_driver_i2s)
