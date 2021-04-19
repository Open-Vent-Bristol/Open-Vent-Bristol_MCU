add_library(OpenVent_hal STATIC)

if(${MCU_TARGET} STREQUAL stm32l4)
  add_subdirectory(stm32l4)
endif()

add_subdirectory(adc)
add_subdirectory(clock)
add_subdirectory(gpio)
add_subdirectory(pinmux)
# add_subdirectory(spi)
add_subdirectory(uart)
add_subdirectory(watchdog)
