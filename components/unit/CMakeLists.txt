idf_component_register(
        SRCS "unit.c"
        INCLUDE_DIRS "include"
        REQUIRES "driver" "sensor" "valve" "pump"
)
