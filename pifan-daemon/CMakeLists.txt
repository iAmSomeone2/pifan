include(GNUInstallDirs)

# Copy the util dir into the build dir
# and figure out where pifan.json is.
if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    # For debugging, the config file will be kept in the debug build directory.
    message(STATUS "Build type set to: debug")
    file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/util DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/)
    set(CONFIG_FILE_PATH ${CMAKE_CURRENT_BINARY_DIR}/util/pifan.json)
else()
    # For all release types, This sets the config file path the appropriate system location.
    message(STATUS "Build type set to: release")
    set(CONFIG_FILE_PATH ${CMAKE_INSTALL_SYSCONFDIR}/pifan.json)
endif()

# This never changes regardless of config type.
set(THERMAL_FILE_PATH /sys/class/thermal/thermal_zone0/temp)

configure_file(PiFanConfig.h.in ${CMAKE_CURRENT_BINARY_DIR}/PiFanConfig.h)

set(CMAKE_CXX_FLAGS_DEBUG "-Wall -Wextra -O0 -g")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

find_package(pigpio REQUIRED)
find_package(Threads REQUIRED)
find_package(Boost 1.29.0 REQUIRED COMPONENTS
             filesystem)

# Build sub-modules
add_subdirectory(data_access)
add_subdirectory(hardware)

add_executable(pifand Main.cxx)
    
include_directories(${CMAKE_CURRENT_BINARY_DIR})
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/hardware)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/data_access)
include_directories(${Boost_INCLUDE_DIRS})

target_link_libraries(pifand ${pigpiod_if2_LIBRARY})
target_link_libraries(pifand dataAccess)
target_link_libraries(pifand hardware)
target_link_libraries(pifand ${CMAKE_THREAD_LIBS_INIT})
target_link_libraries(pifand Boost::filesystem)

# Installation settings
install(TARGETS pifand 
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})
        
install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/util/pifan.json
        DESTINATION ${CMAKE_INSTALL_SYSCONFDIR}
        CONFIGURATIONS Release)


install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/util/pifand.service
        DESTINATION /usr/lib/systemd/system/
        CONFIGURATIONS Release)
