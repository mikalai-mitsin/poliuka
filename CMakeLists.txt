cmake_minimum_required(VERSION 3.16.0)

if(DEFINED ENV{HOMEKIT_PATH})
    set(HOMEKIT_PATH $ENV{HOMEKIT_PATH})
endif(DEFINED ENV{HOMEKIT_PATH})

set(EXTRA_COMPONENT_DIRS ${HOMEKIT_PATH}/components ${HOMEKIT_PATH}/components/homekit)

include($ENV{IDF_PATH}/tools/cmake/project.cmake)
project(poliuka-idf)
