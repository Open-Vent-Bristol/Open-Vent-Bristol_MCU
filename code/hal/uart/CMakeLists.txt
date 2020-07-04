target_sources(OpenVent_hal
  PUBLIC
    uart.c
    uart_${MCU_TARGET}.c
)
