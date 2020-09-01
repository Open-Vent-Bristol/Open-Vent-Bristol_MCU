target_sources(OpenVent_ftp.elf
  PUBLIC
    ftp_main_${MCU_TARGET}.c
    uart_test.c
)

if(${MCU_TARGET} STREQUAL atmega328p)
  target_sources(OpenVent_ftp.elf
    PUBLIC
      spi_test.c
  )
endif()

target_sources(OpenVent_minimal.elf
  PUBLIC
    minimal_${MCU_TARGET}.c
)

if(${MCU_TARGET} STREQUAL stm32l4)
  target_sources(OpenVent_stm32l4_adc_test.elf
    PUBLIC
      adc_test_stm32l4.c
  )
endif()
