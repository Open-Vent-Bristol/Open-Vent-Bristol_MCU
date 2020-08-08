add_library(OpenVent_hal STATIC)

target_include_directories(OpenVent_hal
  PUBLIC .
)

if(${MCU_TARGET} STREQUAL stm32l4)
  add_subdirectory(stm32l4)
  target_include_directories(OpenVent_hal
    PUBLIC
      stm32l4
      stm32l4/CMSIS/Include
      stm32l4/CMSIS/Device/ST/STM32L4xx/Include
  )
  target_compile_definitions(OpenVent_hal
    PUBLIC
      -DSTM32L476xx
      -DUSE_FULL_LL_DRIVER
  )
endif()

if(${MCU_TARGET} STREQUAL atmega328p)
  add_subdirectory(spi)
endif()

add_subdirectory(adc)
add_subdirectory(board)
add_subdirectory(clock)
add_subdirectory(gpio)
add_subdirectory(types)
add_subdirectory(uart)
