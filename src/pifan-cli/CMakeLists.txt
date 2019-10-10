set(CMAKE_CXX_STANDARD 11)

set(CMAKE_CXX_FLAGS_DEBUG "-Wall -Wextra -O0 -g")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

add_subdirectory(Configure)

add_executable(pifan Main.cxx)

include_directories(${CMAKE_CURRENT_BINARY_DIR})
include_directories(${CMAKE_CURRENT_BINARY_DIR}/../pifan-daemon/)

target_link_libraries(pifan ${JSONCPP_LIBRARIES})
target_link_libraries(pifan configure)
