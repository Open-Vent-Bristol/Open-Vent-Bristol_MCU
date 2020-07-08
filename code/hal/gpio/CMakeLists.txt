target_sources(OpenVent_hal
  PUBLIC
    gpio_${MCU_TARGET}.c
)

if(${MCU_TARGET} STREQUAL atmega328p)
  target_sources(OpenVent_hal
    PUBLIC
      multiplexer_74HC4052.c
  )
endif()

if(${MCU_TARGET} STREQUAL stm32l4)
  target_sources(OpenVent_hal
    PUBLIC
      parallel_port_${MCU_TARGET}.c
  )
endif()
