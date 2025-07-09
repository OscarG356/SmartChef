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
set(sdkVersion 2.1.1)
set(toolchainVersion 14_2_Rel1)
set(picotoolVersion 2.1.1)
set(picoVscode ${USERHOME}/.pico-sdk/cmake/pico-vscode.cmake)
if (EXISTS ${picoVscode})
    include(${picoVscode})
endif()
# ====================================================================================
set(PICO_BOARD pico CACHE STRING "Board type")

# Pull in Raspberry Pi Pico SDK (must be before project)
include(pico_sdk_import.cmake)

project(SmartChef C CXX ASM)

# Initialise the Raspberry Pi Pico SDK
pico_sdk_init()

# Add executable. Default name is the project name, version 0.1

add_executable(SmartChef 
    SmartChef.c 
    src/sensors/limit_switch.c
    src/sensors/encoder.c
    src/sensors/load_cell.c
    src/sensors/current_sensor.c
    src/sensors/ultrasonic.c
    src/utils/smart_timers.c
    src/smart_blender.c
    src/drivers/recetas.c
    src/drivers/batido_control.c
    )

pico_set_program_name(SmartChef "SmartChef")
pico_set_program_version(SmartChef "0.1")

# Modify the below lines to enable/disable output over UART/USB
pico_enable_stdio_uart(SmartChef 0)
pico_enable_stdio_usb(SmartChef 1)

# Add the standard library to the build
target_link_libraries(SmartChef
    pico_stdlib
    pico_stdlib
    pico_time
    hardware_timer
    hardware_spi
    hardware_rtc
    hardware_adc
    hardware_clocks
    hardware_xosc
    hardware_pll
    hardware_gpio
    hardware_irq
    hardware_pwm
    hardware_i2c
)

# Add the standard include files to the build
target_include_directories(SmartChef PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_SOURCE_DIR}/src
        ${CMAKE_CURRENT_SOURCE_DIR}/src/sensors
)

pico_add_extra_outputs(SmartChef)

