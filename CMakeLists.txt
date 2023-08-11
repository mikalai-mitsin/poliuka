cmake_minimum_required(VERSION 3.16.0)

set(EXTRA_COMPONENT_DIRS $ENV{HOMEKIT_PATH}/components/homekit)

include($ENV{IDF_PATH}/tools/cmake/project.cmake)
project(poliuka-idf)
