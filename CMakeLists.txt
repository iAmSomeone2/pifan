cmake_minimum_required(VERSION 3.10)
project(pifan CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/CMakeModules)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include/PiFan)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/libs)

#add_subdirectory(include)
#find_package(Python3 3.6 REQUIRED
#        COMPONENTS Interpreter Development
#        OPTIONAL_COMPONENTS Compiler)

add_subdirectory(src)



# Top-level install commands
include(GNUInstallDirs)

#install(TARGETS libpifan
#        PUBLIC_HEADER
#        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/pifan
#)
