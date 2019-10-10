cmake_minimum_required(VERSION 3.14)
project(pifan CXX)

add_library(libpifan STATIC PiFan.h)
set_target_properties(libpifan PROPERTIES PUBLIC_HEADER PiFan.h)