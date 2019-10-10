cmake_minimum_required(VERSION 3.14)
project(pifan)

# Copy the util dir into the build dir
# and figure out where pifan.json is
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/util DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/)
set(CONFIG_FILE_PATH ${CMAKE_CURRENT_BINARY_DIR}/util/pifan.conf)

set(THERMAL_FILE_PATH /sys/class/thermal/thermal_zone0/temp)

configure_file(pifanconfig.h.in ${CMAKE_CURRENT_BINARY_DIR}/pifanconfig.h)

set(CMAKE_C_FLAGS "-Wall -Wextra")

set(THREADS_PREFER_PTHREAD_FLAG ON)

find_package(pigpio REQUIRED)
find_package(Threads REQUIRED)

add_subdirectory(data_access)

add_executable(pifanx
    pifand.c
    hardware.c
    hardware.h)

include_directories("/usr/local/include")
include_directories(${CMAKE_CURRENT_BINARY_DIR})
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/data_access)

target_link_libraries(pifanx ${pigpiod_if2_LIBRARY})
target_link_libraries(pifanx data_access)
target_link_libraries(pifanx Threads::Threads)
