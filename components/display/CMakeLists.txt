idf_component_register(
        SRCS "display.h" "display.c"
        INCLUDE_DIRS "."
        REQUIRES "driver" "ssd1306"
)