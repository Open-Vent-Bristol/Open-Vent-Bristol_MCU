target_sources(OpenVent_hal
  PRIVATE
    uart.c
    uart_${MCU_TARGET}.c
)
