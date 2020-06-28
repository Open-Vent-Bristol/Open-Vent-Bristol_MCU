cmake_minimum_required(VERSION 3.13)

project(OpenVent_firmware
    VERSION 1.0
    DESCRIPTION "OpenVent Firmware"
    LANGUAGES C
)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# if(${BOARD_CONFIG})
# else()
  set(BOARD_CONFIG BOARD_MK1)
# endif()

# This definition is used to select the correct source files in the hal
if(${BOARD_CONFIG} STREQUAL BOARD_MK1)
  set(MCU_TARGET atmega328p)
endif()

add_definitions(
  -D${BOARD_CONFIG}
)

add_executable(OpenVent_firmware.elf)
add_executable(OpenVent_ftp.elf)
add_executable(OpenVent_minimal.elf
  functional_test/minimal.c
)

include_directories(
  application
  drivers
  hal
)

add_subdirectory(application)
add_subdirectory(drivers)
add_subdirectory(hal)
add_subdirectory(functional_test)

target_link_libraries(OpenVent_firmware.elf
  OpenVent_application
  OpenVent_drivers
  OpenVent_hal
)
