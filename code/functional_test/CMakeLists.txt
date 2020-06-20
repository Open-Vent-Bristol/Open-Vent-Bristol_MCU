target_sources(OpenVent_ftp
  PUBLIC
    ftp_main.c
    adc_test.c
    gpio_test.c
    spi_test.c
    uart_test.c
)

target_link_libraries(OpenVent_ftp
  OpenVent_hal
)

if(${MCU_TARGET} STREQUAL atmega328pb)
  add_dependencies(OpenVent_ftp crtatmega328pb)
endif()
