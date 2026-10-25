idf_component_register(SRCS "power_mgr.c"
                       INCLUDE_DIRS "include"
                       REQUIRES node_config beesense_proto esp_adc esp_hw_support)
