cmake_minimum_required(VERSION 3.14)
project(pifan)

set(THERMAL_FILE_PATH /sys/class/thermal_zone0/temp)

configure_file(PiFanConfig.h.in ${CMAKE_CURRENT_BINARY_DIR}/pifanconfig.h)

set(CMAKE_CXX_FLAGS "-Wall -Wextra")

find_package(pigpio REQUIRED)

add_executable( pifand
        Main.cxx
        Fan.cxx Fan.h
        DataAccess.cpp DataAccess.h)
    
include_directories(${CMAKE_CURRENT_BINARY_DIR})

target_link_libraries(pifand ${pigpiod_if2_LIBRARY})
