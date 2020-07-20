#include "clock/clock.h"
#include "board/board.h"

void clock_init(void)
{
  // Enable the external crystal oscillator and wait for it to stabilise
  // LL_RCC_HSE_Enable();
  // while (!LL_RCC_HSE_IsReady());
  LL_RCC_HSI_Enable();
  while (!LL_RCC_HSI_IsReady());

  // Set system clock to use HSE
  // LL_RCC_SetSysClkSource(LL_RCC_SYS_CLKSOURCE_HSE);
  LL_RCC_SetSysClkSource(LL_RCC_SYS_CLKSOURCE_HSI);

  // Set bus prescalers
  LL_RCC_SetAHBPrescaler(LL_RCC_SYSCLK_DIV_1);
  LL_RCC_SetAPB1Prescaler(LL_RCC_APB1_DIV_1);
  LL_RCC_SetAPB2Prescaler(LL_RCC_APB2_DIV_1);

  // SysTick config for scheduler
  // IMPORTANT: the AHBPrescaler setting above must be accounted for if non-zero
  SysTick_Config((CPU_CLOCK_HZ / SCHEDULER_HZ) - 1u);

  // Motor PWM timer - TODO
}
