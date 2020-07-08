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

  // Set peripheral clock sources - TODO: move to drivers
  LL_RCC_SetUSARTClockSource(LL_RCC_USART1_CLKSOURCE_SYSCLK);
  // SCHEDULER_TIM_CFG();
  // MOTOR_PWM_TIM_CFG();
}
