cmake_minimum_required(VERSION 3.14)
project(pifan)

set(CMAKE_C_FLAGS "-Wall -Wextra")

add_library(data_access STATIC data_access.c data_access.h)

include_directories("/usr/local/include")
include_directories(${CMAKE_CURRENT_BINARY_DIR}/../)
include_directories(${CMAKE_CURRENT_SOURCE_DIR})


