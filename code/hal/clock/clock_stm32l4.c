#include "clock/clock.h"
#include "board/board.h"

static void motor_pwm_init(void);

void clock_init(void)
{
  // TODO - swap to HSE
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
  SysTick_Config((CPU_CLOCK_HZ / SYSTICK_HZ) - 1u);

  motor_pwm_init();
}

static void motor_pwm_init(void)
{
  // Enable clock
  LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_TIM2);

  // Reset TIM2 peripheral
  LL_TIM_DeInit(TIM2);

  LL_TIM_InitTypeDef tim_init_struct =
  {
    .Prescaler = MOTOR_PWM_PRESCALER,
    .CounterMode = LL_TIM_COUNTERMODE_CENTER_UP_DOWN,
    .Autoreload = MOTOR_PWM_TOP,
    .ClockDivision = LL_TIM_CLOCKDIVISION_DIV1,
    .RepetitionCounter = 0u
  };

  LL_TIM_Init(TIM2, &tim_init_struct);

  // Enable continuous reload (i.e. not one-shot)
  LL_TIM_EnableARRPreload(TIM2);

  LL_TIM_OC_InitTypeDef oc_init_struct =
  {
    .OCMode = LL_TIM_OCMODE_PWM1,
    .OCState = LL_TIM_OCSTATE_DISABLE,
    .OCNState = LL_TIM_OCSTATE_DISABLE,
    .CompareValue = (LL_TIM_GetAutoReload(TIM2) / 2),
    .OCPolarity = LL_TIM_OCPOLARITY_HIGH,
    .OCNPolarity = LL_TIM_OCPOLARITY_HIGH,
    .OCIdleState = LL_TIM_OCIDLESTATE_LOW,
    .OCNIdleState = LL_TIM_OCIDLESTATE_LOW
  };

  LL_TIM_OC_Init(TIM2, LL_TIM_CHANNEL_CH1, &oc_init_struct);

  // Enable autoreload of the compare value (duty cycle)
  LL_TIM_OC_EnablePreload(TIM2, LL_TIM_CHANNEL_CH1);

  // Enable PWM output
  MOTOR_PWM_STOP();
  LL_TIM_CC_EnableChannel(TIM2, LL_TIM_CHANNEL_CH1);
}
