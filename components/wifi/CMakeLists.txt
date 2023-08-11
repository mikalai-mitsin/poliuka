idf_component_register(
        SRCS "wifi.c"
        INCLUDE_DIRS "include"
        REQUIRES wifi_provisioning nvs_flash
)