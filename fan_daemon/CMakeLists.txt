cmake_minimum_required(VERSION 3.14)
project(pifan C)

add_definitions(-Wall -lpthread -ltr -lpigpiod_if2)
set(CMAKE_C_STANDARD 11)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/CMakeModules)

add_executable(pifand 
	pifand.c
	hardware.c
	hardware.h)

include_directories("/usr/local/include")

find_package(pigpio REQUIRED)
target_link_libraries(pifand ${pigpiod_if2_LIBRARY})
