# target_sources(OpenVent_ftp.elf
#   PUBLIC
#     ftp_main.c
#     gpio_test.c
#     spi_test.c
#     uart_test.c
# )

target_sources(OpenVent_minimal.elf
  PUBLIC
    minimal_${MCU_TARGET}.c
)
