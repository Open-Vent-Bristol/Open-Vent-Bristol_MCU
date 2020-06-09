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

if(${BOARD_CONFIG} STREQUAL BOARD_MK1)
# This definition is used to select the correct source files in the hal
set(MCU_TARGET atmega328pb)

add_definitions(
  -D${BOARD_CONFIG}
  )
  endif()

  add_executable(OpenVent_firmware)

  include_directories(
  application
  drivers
  hal
)

add_subdirectory(application)
add_subdirectory(drivers)
add_subdirectory(hal)

target_link_libraries(OpenVent_firmware
  OpenVent_application
  OpenVent_drivers
  OpenVent_hal
)
