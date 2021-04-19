target_sources(OpenVent_ftp.elf
  PUBLIC
    ftp_main_${MCU_TARGET}.c
    spi_test.c
    uart_test.c
)

target_sources(OpenVent_minimal.elf
  PUBLIC
    minimal_${MCU_TARGET}.c
)
