cmake_minimum_required(VERSION 3.14)
project(pifan)

set(CMAKE_CXX_FLAGS "-Wall -Wextra")

find_package(pigpio REQUIRED)

add_library(hardware STATIC Fan.cxx Fan.h)

include_directories(${CMAKE_CURRENT_BINARY_DIR}/../)
include_directories(${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(hardware ${pigpiod_if2_LIBRARY})