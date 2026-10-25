idf_component_register(SRCS "piezo_adc.c"
                       INCLUDE_DIRS "include"
                       REQUIRES node_config beesense_proto esp_adc)
