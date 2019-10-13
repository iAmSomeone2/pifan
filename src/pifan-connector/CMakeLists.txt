include(GNUInstallDirs)

find_package(Boost 1.29.0 REQUIRED COMPONENTS filesystem)

set(SOURCE_FILES Connector.cpp Connector.h)

add_library(pifan-connector Connector.cpp Connector.h)

target_include_directories(pifan-connector PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
        PRIVATE pifan-connector)

include_directories(${Boost_INCLUDE_DIRS})

target_link_libraries(pifan-connector Boost::filesystem)

# Set up install instructions
install(TARGETS pifan-connector EXPORT ConnectorConfig
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})

install(DIRECTORY include/
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME})

install(EXPORT ConnectorConfig DESTINATION share/Connector/cmake)

export(TARGETS pifan-connector FILE ConnectorConfig.cmake)