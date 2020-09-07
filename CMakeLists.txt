cmake_minimum_required(VERSION 3.10)
project(pifan VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/CMakeModules)

add_subdirectory(src)

# Top-level install commands
include(GNUInstallDirs)
