cmake_minimum_required(VERSION 3.14)
project(pifan)

set(CMAKE_C_FLAGS "-Wall -Wextra -O2")

add_executable(target_temp pifancli.c)

include_directories("/usr/local/include")
include_directories(${CMAKE_CURRENT_BINARY_DIR}/../fan_daemon)
