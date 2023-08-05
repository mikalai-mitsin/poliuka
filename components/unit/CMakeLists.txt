idf_component_register(
        SRCS "unit.h" "unit.c"
        INCLUDE_DIRS "."
        REQUIRES "driver" "sensor" "valve" "pump"
)
