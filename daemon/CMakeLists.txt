cmake_minimum_required(VERSION 3.14)
project(pifan)

# Copy the util dir into the build dir
# and figure out where pifan.json is
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/util DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/)
set(CONFIG_FILE_PATH ${CMAKE_CURRENT_BINARY_DIR}/util/pifan.json)

set(THERMAL_FILE_PATH /sys/class/thermal_zone0/temp)

configure_file(PiFanConfig.h.in ${CMAKE_CURRENT_BINARY_DIR}/PiFanConfig.h)

set(CMAKE_CXX_FLAGS "-Wall -Wextra")

find_package(PkgConfig REQUIRED)
find_package(pigpio REQUIRED)

pkg_check_modules(JSONCPP jsoncpp)

link_libraries(${JSONCPP_LIBRARIES})

add_executable( pifand
        Main.cxx
        Fan.cxx Fan.h
        DataAccess.cpp DataAccess.h)
    
include_directories(${CMAKE_CURRENT_BINARY_DIR})

target_link_libraries(pifand ${JSONCPP_LIBRARIES})
target_link_libraries(pifand ${pigpiod_if2_LIBRARY})
