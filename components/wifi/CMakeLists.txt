idf_component_register(
        SRCS "wifi.h" "wifi.c"
        INCLUDE_DIRS "."
        REQUIRES wifi_provisioning nvs_flash
)