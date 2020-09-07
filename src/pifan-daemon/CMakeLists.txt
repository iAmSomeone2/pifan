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

configure_file(PiFanConfig.hpp.in PiFanConfig.hpp)
include_directories(${CMAKE_CURRENT_BINARY_DIR})

find_package(pigpio REQUIRED)

# Build sub-modules
add_subdirectory(hardware)

add_executable(pifand main.cpp)

target_link_libraries(pifand
        ${pigpiod_if2_LIBRARY}
        pifan-hardware
)

target_include_directories(pifand PUBLIC SYSTEM
        ${pigpio_INCLUDE_DIRS}
)
target_include_directories(pifand PRIVATE
        ./hardware
)

# Installation settings
install(TARGETS pifand 
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})
        
# install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/util/pifan.json
#         DESTINATION ${CMAKE_INSTALL_SYSCONFDIR}
#         CONFIGURATIONS Release)


install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/util/pifand.service
        DESTINATION /lib/systemd/system/
        CONFIGURATIONS Release)
