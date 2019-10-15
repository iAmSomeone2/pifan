include(GNUInstallDirs)

find_package(Boost 1.29.0 REQUIRED COMPONENTS filesystem)

set(SOURCE_FILES Connector.cpp include/Connector.h)

add_library(pifan-connector SHARED ${SOURCE_FILES})

set_target_properties(pifan-connector PROPERTIES VERSION ${PROJECT_VERSION})
set_target_properties(pifan-connector PROPERTIES PUBLIC_HEADER include/Connector.h)

configure_file(include/pifan-connector.pc.in pifan-connector.pc @ONLY)

target_include_directories(pifan-connector PRIVATE .)

target_include_directories(pifan-connector PRIVATE ${Boost_INCLUDE_DIRS})

target_link_libraries(pifan-connector Boost::filesystem)

# Set up install instructions
install(TARGETS pifan-connector EXPORT ConnectorConfig
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

install(FILES ${CMAKE_CURRENT_BINARY_DIR}/pifan-connector.pc
        DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/pkgconfig)

install(EXPORT ConnectorConfig DESTINATION share/Connector/cmake)

export(TARGETS pifan-connector FILE ConnectorConfig.cmake)