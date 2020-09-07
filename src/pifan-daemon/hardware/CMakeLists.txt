add_library(pifan-hardware STATIC Fan.cpp Fan.hpp Gpio.cpp Gpio.hpp Pin.cpp Pin.hpp)

target_include_directories(pifan-hardware PUBLIC SYSTEM ${pigpio_INCLUDE_DIRS})

target_link_libraries(pifan-hardware
    ${pigpiod_if2_LIBRARY}
)