// Copyright (c) 2021 <OpenVent-Bristol, Donald Robson>

#include "clock/clock.h"
#include "board/board.h"

static void motor_pwm_init(void);
static void buzz_pwm_init(void);
static void fan_pwm_init(void);

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

  // Set voltage scaling range to allow higher clock speeds for USB
  LL_APB1_GRP1_EnableClock(RCC_APB1ENR1_PWREN);
  LL_PWR_SetRegulVoltageScaling(LL_PWR_REGU_VOLTAGE_SCALE1);
  while (!LL_PWR_IsActiveFlag_VOS());

  // Due to voltage scaling range, flash latency (wait states) are needed
  LL_FLASH_SetLatency(LL_FLASH_LATENCY_2);

  // Setup MSI at 48MHz and tell USB to use it
  LL_RCC_MSI_SetRange(LL_RCC_MSIRANGE_11);
  LL_RCC_MSI_EnableRangeSelection();
  LL_RCC_SetUSBClockSource(LL_RCC_SYS_CLKSOURCE_MSI);

  // SysTick config for scheduler
  // IMPORTANT: the AHBPrescaler setting above must be accounted for if non-zero
  SysTick_Config((CPU_CLOCK_HZ / SYSTICK_HZ) - 1u);

  motor_pwm_init();
  buzz_pwm_init();
  fan_pwm_init();
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

// Set up a PWM output to produce a continuous 880Hz tone
static void buzz_pwm_init(void)
{
  // Enable clock
  LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_TIM5);

  // Reset TIM5 peripheral
  LL_TIM_DeInit(TIM5);

  LL_TIM_InitTypeDef tim_init_struct =
  {
    .Prescaler = BUZZ_PWM_PRESCALER,
    .CounterMode = LL_TIM_COUNTERMODE_UP,
    .Autoreload = BUZZ_PWM_TOP,
    .ClockDivision = LL_TIM_CLOCKDIVISION_DIV1,
    .RepetitionCounter = 0u
  };

  LL_TIM_Init(TIM5, &tim_init_struct);

  // Enable continuous reload (i.e. not one-shot)
  LL_TIM_EnableARRPreload(TIM5);

  LL_TIM_OC_InitTypeDef oc_init_struct =
  {
    .OCMode = LL_TIM_OCMODE_PWM1,
    .OCState = LL_TIM_OCSTATE_DISABLE,
    .OCNState = LL_TIM_OCSTATE_DISABLE,
    .CompareValue = (LL_TIM_GetAutoReload(TIM5) / 2),
    .OCPolarity = LL_TIM_OCPOLARITY_HIGH,
    .OCNPolarity = LL_TIM_OCPOLARITY_HIGH,
    .OCIdleState = LL_TIM_OCIDLESTATE_LOW,
    .OCNIdleState = LL_TIM_OCIDLESTATE_LOW
  };

  LL_TIM_OC_Init(TIM5, LL_TIM_CHANNEL_CH3, &oc_init_struct);

  // Enable autoreload of the compare value (duty cycle)
  LL_TIM_OC_EnablePreload(TIM5, LL_TIM_CHANNEL_CH3);

  // Enable PWM output
  BUZZ_PWM_STOP();
  LL_TIM_CC_EnableChannel(TIM5, LL_TIM_CHANNEL_CH3);
}

static void fan_pwm_init(void)
{
  // Enable clock
  LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_TIM3);

  // Reset TIM3 peripheral
  LL_TIM_DeInit(TIM3);

  LL_TIM_InitTypeDef tim_init_struct =
  {
    .Prescaler = FAN_PWM_PRESCALER,
    .CounterMode = LL_TIM_COUNTERMODE_CENTER_UP_DOWN,
    .Autoreload = FAN_PWM_TOP,
    .ClockDivision = LL_TIM_CLOCKDIVISION_DIV1,
    .RepetitionCounter = 0u
  };

  LL_TIM_Init(TIM3, &tim_init_struct);

  // Enable continuous reload (i.e. not one-shot)
  LL_TIM_EnableARRPreload(TIM3);

  LL_TIM_OC_InitTypeDef oc_init_struct =
  {
    .OCMode = LL_TIM_OCMODE_PWM1,
    .OCState = LL_TIM_OCSTATE_DISABLE,
    .OCNState = LL_TIM_OCSTATE_DISABLE,
    .CompareValue = (LL_TIM_GetAutoReload(TIM3) / 2),
    .OCPolarity = LL_TIM_OCPOLARITY_HIGH,
    .OCNPolarity = LL_TIM_OCPOLARITY_HIGH,
    .OCIdleState = LL_TIM_OCIDLESTATE_LOW,
    .OCNIdleState = LL_TIM_OCIDLESTATE_LOW
  };

  LL_TIM_OC_Init(TIM3, LL_TIM_CHANNEL_CH3, &oc_init_struct);

  // Enable autoreload of the compare value (duty cycle)
  LL_TIM_OC_EnablePreload(TIM3, LL_TIM_CHANNEL_CH3);

  // Enable PWM output
  FAN_PWM_STOP();
  LL_TIM_CC_EnableChannel(TIM3, LL_TIM_CHANNEL_CH3);
}
