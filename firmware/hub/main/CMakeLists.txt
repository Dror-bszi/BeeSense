idf_component_register(SRCS "main.c"
                       INCLUDE_DIRS ""
                       REQUIRES beesense_proto hub_config espnow_rx baseline
                                detect spool uplink_lte alerts nvs_flash)
