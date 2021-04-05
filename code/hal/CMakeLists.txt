add_library(OpenVent_hal STATIC)

if(${MCU_TARGET} STREQUAL stm32l4)
  add_subdirectory(stm32l4)
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
add_subdirectory(watchdog)
