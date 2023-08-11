idf_component_register(
        SRCS "homekit.c"
        INCLUDE_DIRS "include"
        REQUIRES esp_hap_core esp_hap_platform esp_hap_apple_profiles esp_netif unit
)