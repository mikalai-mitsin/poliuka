idf_component_register(
        SRCS "display.c"
        INCLUDE_DIRS "include"
        REQUIRES "driver" "ssd1306" "unit"
)