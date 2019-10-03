cmake_minimum_required(VERSION 3.14)
project(pifan)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/CMakeModules)

include_directories(${CMAKE_CURRENT_SOURCE_DIR})

add_subdirectory(daemon)
add_subdirectory(fan_daemon)
add_subdirectory(fan_cli)
add_subdirectory(fan_gui)
