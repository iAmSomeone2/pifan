cmake_minimum_required(VERSION 3.14)
project(pifan CXX)

set(CMAKE_CXX_STANDARD 11)

set(CMAKE_CXX_FLAGS_DEBUG "-Wall -Wextra -O0 -g")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

find_package(PkgConfig REQUIRED)
pkg_check_modules(JSONCPP jsoncpp)
link_libraries(${JSONCPP_LIBRARIES})

add_library(configure STATIC Configure.cpp Configure.h)

include_directories(${CMAKE_CURRENT_BINARY_DIR})
target_link_libraries(configure ${JSONCPP_LIBRARIES})