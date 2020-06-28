target_sources(OpenVent_ftp.elf
  PUBLIC
    ftp_main.c
    gpio_test.c
    spi_test.c
    uart_test.c
)

target_link_libraries(OpenVent_ftp.elf
  OpenVent_hal
)
