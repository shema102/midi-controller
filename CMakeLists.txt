# Generated Cmake Pico project file

cmake_minimum_required(VERSION 3.13)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Initialise pico_sdk from installed location
# (note this can come from environment, CMake cache etc)

# == DO NOT EDIT THE FOLLOWING LINES for the Raspberry Pi Pico VS Code Extension to work ==
if(WIN32)
    set(USERHOME $ENV{USERPROFILE})
else()
    set(USERHOME $ENV{HOME})
endif()
set(sdkVersion 2.2.0)
set(toolchainVersion 14_2_Rel1)
set(picotoolVersion 2.2.0-a4)
set(picoVscode ${USERHOME}/.pico-sdk/cmake/pico-vscode.cmake)
if (EXISTS ${picoVscode})
    include(${picoVscode})
endif()
# ====================================================================================

set(PICO_BOARD pico2 CACHE STRING "Board type")

# Pull in Raspberry Pi Pico SDK (must be before project)
include(pico_sdk_import.cmake)

project(midi-controller C CXX ASM)

# Initialise the Raspberry Pi Pico SDK
pico_sdk_init()

# FreeRTOS
set(FREERTOS_CONFIG_FILE_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/config)
set(FREERTOS_KERNEL_PATH ${CMAKE_CURRENT_LIST_DIR}/lib/FreeRTOS-Kernel)

include(${CMAKE_CURRENT_LIST_DIR}/FreeRTOS_Kernel_import.cmake)

# Add executable. Default name is the project name, version 0.1

add_executable(midi-controller
        src/main.cpp
        config/definitions.h
        config/FreeRTOSConfig.h
        src/kb/handler/keyboard.h
        src/kb/handler/keyboard.cpp
        src/kb/event/events.h
        src/kb/event/events.cpp
        src/hooks.cpp
        config/tusb_config.h
        src/usb/usb_descriptors.cpp
)

pico_set_program_name(midi-controller "midi-controller")
pico_set_program_version(midi-controller "0.1")

# Modify the below lines to enable/disable output over UART/USB
pico_enable_stdio_uart(midi-controller 1)
pico_enable_stdio_usb(midi-controller 0)

# Add the standard library to the build
target_link_libraries(midi-controller
        pico_stdlib
        FreeRTOS-Kernel-Heap4
        tinyusb_device
)

# Add the standard include files to the build
target_include_directories(midi-controller PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}
)

pico_add_extra_outputs(midi-controller)

