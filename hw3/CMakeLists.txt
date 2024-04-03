cmake_minimum_required(VERSION 3.12)

# Pull in SDK (must be before project)
include(pico_sdk_import.cmake)

project(hw3 C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

set(PICO_EXAMPLES_PATH ${PROJECT_SOURCE_DIR})

# Initialize the SDK
pico_sdk_init()

add_executable(hw3
        hw3.c
        )

# pull in common dependencies
target_link_libraries(hw3 pico_stdlib hardware_adc)

pico_enable_stdio_usb(hw3 1)
pico_enable_stdio_uart(hw3 0)

# create map/bin/hex file etc.
pico_add_extra_outputs(hw3)



