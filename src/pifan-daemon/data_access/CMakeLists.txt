cmake_minimum_required(VERSION 3.14)
project(pifan)

set(CMAKE_CXX_FLAGS "-Wall -Wextra")

find_package(PkgConfig REQUIRED)
pkg_check_modules(JSONCPP jsoncpp)
link_libraries(${JSONCPP_LIBRARIES})

add_library(dataAccess STATIC DataAccess.cxx DataAccess.h)

include_directories(${CMAKE_CURRENT_BINARY_DIR}/../)
include_directories(${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(dataAccess ${JSONCPP_LIBRARIES})