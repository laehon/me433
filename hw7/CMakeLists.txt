cmake_minimum_required(VERSION 3.12)

# Pull in SDK (must be before project)
include(pico_sdk_import.cmake)

project(hw7 C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

set(PICO_EXAMPLES_PATH ${PROJECT_SOURCE_DIR})

# Initialize the SDK
pico_sdk_init()

add_executable(hw7
        hw7.c
        ssd1306.c
        )

# pull in common dependencies
target_link_libraries(hw7 pico_stdlib hardware_uart hardware_i2c)

# create map/bin/hex file etc.
pico_add_extra_outputs(hw7)



