cmake_minimum_required(VERSION 3.10)
project(pifan VERSION 0.2 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/CMakeModules)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/libs/PiGPIO)

add_subdirectory(pifan-daemon)
add_subdirectory(pifan-connector)



# Top-level install commands
include(GNUInstallDirs)

#install(TARGETS libpifan
#        PUBLIC_HEADER
#        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/pifan
#)
