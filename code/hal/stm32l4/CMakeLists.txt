add_definitions(
  -DUSE_FULL_LL_DRIVER
)

target_sources(OpenVent_hal
  PUBLIC
    src/stm32l4xx_ll_adc.c
    # src/stm32l4xx_ll_comp.c
    # src/stm32l4xx_ll_crc.c
    # src/stm32l4xx_ll_crs.c
    # src/stm32l4xx_ll_dac.c
    # src/stm32l4xx_ll_dma.c
    # src/stm32l4xx_ll_dma2d.c
    src/stm32l4xx_ll_exti.c
    # src/stm32l4xx_ll_fmc.c
    src/stm32l4xx_ll_gpio.c
    # src/stm32l4xx_ll_i2c.c
    # src/stm32l4xx_ll_lptim.c
    # src/stm32l4xx_ll_lpuart.c
    # src/stm32l4xx_ll_opamp.c
    src/stm32l4xx_ll_pwr.c
    src/stm32l4xx_ll_rcc.c
    # src/stm32l4xx_ll_rng.c
    # src/stm32l4xx_ll_rtc.c
    # src/stm32l4xx_ll_sdmmc.c
    # src/stm32l4xx_ll_spi.c
    # src/stm32l4xx_ll_swpmi.c
    src/stm32l4xx_ll_tim.c
    src/stm32l4xx_ll_usart.c
    # src/stm32l4xx_ll_usb.c
    src/stm32l4xx_ll_utils.c
)
