idf_component_register(
        SRCS "sensor.h" "sensor.c"
        INCLUDE_DIRS "."
        REQUIRES "driver" "esp_adc"
)