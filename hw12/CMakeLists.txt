cmake_minimum_required(VERSION 3.12)

# Pull in SDK (must be before project)
include(pico_sdk_import.cmake)

project(hw12 C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

set(PICO_EXAMPLES_PATH ${PROJECT_SOURCE_DIR})

# Initialize the SDK
pico_sdk_init()

add_executable(hw12
        hw12.c
        )

# pull in common dependencies
target_link_libraries(hw12 pico_stdlib hardware_pwm)

# create map/bin/hex file etc.
pico_add_extra_outputs(hw12)



