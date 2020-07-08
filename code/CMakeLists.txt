cmake_minimum_required(VERSION 3.13)

project(OpenVent_firmware
    VERSION 1.0
    DESCRIPTION "OpenVent Firmware"
    LANGUAGES ASM C CXX
)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# if(${BOARD_CONFIG})
# else()
  set(BOARD_CONFIG BOARD_MK2)
# endif()

# This definition is used to select the correct source files in the hal
if(${BOARD_CONFIG} STREQUAL BOARD_MK2)
  set(MCU_TARGET stm32l4)

  add_definitions(
    -DSTM32L476xx
    -DUSE_FULL_LL_DRIVER
  )
elseif(${BOARD_CONFIG} STREQUAL BOARD_MK1)
  set(MCU_TARGET atmega328p)
endif()

add_definitions(
  -D${BOARD_CONFIG}
)

# add_executable(OpenVent_firmware.elf)
# add_executable(OpenVent_ftp.elf)
add_executable(OpenVent_minimal.elf)

include_directories(
  application
  drivers
  hal
)

if(${MCU_TARGET} STREQUAL stm32l4)
  include_directories(
    hal/stm32l4
    hal/stm32l4/CMSIS/Include
    hal/stm32l4/CMSIS/Device/ST/STM32L4xx/Include
  )
endif()

# add_subdirectory(application)
# add_subdirectory(drivers)
add_subdirectory(hal)
add_subdirectory(functional_test)

# target_link_libraries(OpenVent_firmware.elf
#   OpenVent_application
#   OpenVent_drivers
#   OpenVent_hal
# )

# target_link_libraries(OpenVent_ftp.elf
#   OpenVent_hal
# )

target_link_libraries(OpenVent_minimal.elf
  OpenVent_hal
)
