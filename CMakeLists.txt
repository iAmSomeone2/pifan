cmake_minimum_required(VERSION 3.14)
project(pifan CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/CMakeModules)

include_directories(${CMAKE_CURRENT_SOURCE_DIR})

add_subdirectory(pifan-daemon)
# Re-add the cli once it's been transitioned to C++
add_subdirectory(pifan-cli)
# Re-add the GUI once the libraries in Manjaro are fixed
# add_subdirectory(fan_gui)
