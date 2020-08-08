target_sources(OpenVent_hal
  PUBLIC
    shift_register_74HC595.c
    spi_${MCU_TARGET}.c
)
