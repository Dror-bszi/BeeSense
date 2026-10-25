idf_component_register(SRCS "uplink_lte.c"
                       INCLUDE_DIRS "include"
                       REQUIRES detect spool esp_driver_uart esp_netif)
