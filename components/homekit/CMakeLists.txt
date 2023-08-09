idf_component_register(
        SRCS "homekit.h" "homekit.c"
        INCLUDE_DIRS "."
        REQUIRES esp_hap_core esp_hap_platform esp_hap_apple_profiles esp_netif unit
)