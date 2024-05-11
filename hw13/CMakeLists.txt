cmake_minimum_required(VERSION 3.12)

# Pull in SDK (must be before project)
include(pico_sdk_import.cmake)

project(hw13 C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

set(PICO_EXAMPLES_PATH ${PROJECT_SOURCE_DIR})

# Initialize the SDK
pico_sdk_init()

set(FAMILY rp2040)
set(BOARD pico_sdk)
set(TINYUSB_FAMILY_PROJECT_NAME_PREFIX "tinyusb_dev_")

add_executable(hw13
        hw13.c
        usb_descriptors.c
        )

target_sources(hw13 PUBLIC
        ${CMAKE_CURRENT_LIST_DIR}/hw13.c
        ${CMAKE_CURRENT_LIST_DIR}/usb_descriptors.c
        )

target_include_directories(hw13 PUBLIC
        ${CMAKE_CURRENT_LIST_DIR})

# pull in common dependencies
target_link_libraries(hw13 PUBLIC hardware_i2c PUBLIC pico_stdlib pico_unique_id tinyusb_device tinyusb_board)

# create map/bin/hex file etc.
pico_add_extra_outputs(hw13)
