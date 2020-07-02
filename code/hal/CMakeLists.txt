add_library(OpenVent_hal)

add_subdirectory(adc)
add_subdirectory(board)
add_subdirectory(clock)
add_subdirectory(gpio)
add_subdirectory(spi)
add_subdirectory(types)
add_subdirectory(uart)

if($(MCU_TARGET) STREQUAL stm32l4)
  add_subdirectory(stm32l4)
endif()
