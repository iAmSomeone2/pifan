cmake_minimum_required(VERSION 3.14)
project(pifan CXX)

# Set up the GUI portion of the project
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS "-Wall -Wextra")

find_package(PkgConfig REQUIRED)
# set(PKG_CONFIG_PATH /opt/vc/lib/pkgconfig)
pkg_check_modules(GTKMM REQUIRED gtkmm-3.0>=3.20.0)

add_executable(pifan main.cpp pifan_window.cpp pifan_window.hpp)

include_directories("/usr/local/include")
link_directories(${GTKMM_LIBRARY_DIRS})
include_directories(SYSTEM ${GTKMM_INCLUDE_DIRS})
include_directories(${CMAKE_CURRENT_BINARY_DIR}/../fan_daemon)

target_link_libraries(pifan ${GTKMM_LIBRARIES})
