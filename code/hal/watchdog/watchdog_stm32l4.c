// Copyright (c) 2021 <OpenVent-Bristol, Donald Robson>

#include "watchdog.h"
#include "stm32l4/stm32l4xx_ll_iwdg.h"

void watchdog_init(void)
{
  LL_IWDG_Enable(IWDG);

  // Unlock the registers to allow configuration
  LL_IWDG_EnableWriteAccess(IWDG);

  // The counter is fed by an independent 32kHz oscillator,
  // so the prescaler setting results in a clock of 8192 Hz.
  LL_IWDG_SetPrescaler(IWDG, LL_IWDG_PRESCALER_4);

  // Set the reset interval.  The watchdog counter is 12-bit,
  // so the value must be at most 0xFFF.  With the above clock
  // this maximum value will result in a reset time of 0.5 sec.
  LL_IWDG_SetReloadCounter(IWDG, 0xFFF);

  // Wait for the register writes to complete
  while (LL_IWDG_IsReady(IWDG) != 1) {}

  // Reset the counter
  LL_IWDG_ReloadCounter(IWDG);
}

void watchdog_reset(void)
{
  LL_IWDG_ReloadCounter(IWDG);
}
