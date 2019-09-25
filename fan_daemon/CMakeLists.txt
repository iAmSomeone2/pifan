cmake_minimum_required(VERSION 3.14)
project(pifan C)

# Copy the util dir into the build dir
# and figure out where pifan.conf is
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/util DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/)
set(CONFIG_FILE_PATH ${CMAKE_CURRENT_BINARY_DIR}/util/pifan.conf)

set(THERMAL_FILE_PATH /sys/class/thermal/thermal_zone0/temp)

configure_file(pifanconfig.h.in ${CMAKE_CURRENT_BINARY_DIR}/pifanconfig.h)

set(CMAKE_C_FLAGS "-Wall -Wextra -lpthread -lrt -lpigpiod_if2")

find_package(pigpio REQUIRED)

add_library(data_access STATIC data_access.c data_access.h)

add_executable(pifand
	pifand.c
	hardware.c
	hardware.h)

include_directories("/usr/local/include")
include_directories(${CMAKE_CURRENT_BINARY_DIR})

target_link_libraries(pifand ${pigpiod_if2_LIBRARY})
target_link_libraries(pifand data_access)
