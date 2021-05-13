// Copyright (c) 2021 <OpenVent-Bristol, Donald Robson>

// This file should not be included directly.  Include board/board.h instead and define a board ID,
// e.g. -DBOARD_V3
#ifndef BOARD_V3_H
#define BOARD_V3_H

#define GLOBAL_INTERRUPTS_DISABLE()
#define GLOBAL_INTERRUPTS_ENABLE()

#define CPU_CLOCK_HZ        ((uint32_t)16000000u)
// TODO - using NUCLEO 476RG board at the moment which has no HSE oscillator
#define HSI_VALUE           CPU_CLOCK_HZ
// #define HSE_VALUE           CPU_CLOCK_HZ

// CLOCK_DELAY only to be used during system initialisation (Note: divide by 2u compensates for two
// instructions per loop)
#define CLOCK_DELAY(usec)   {uint32_t index = (usec * (CPU_CLOCK_HZ / 1000000u)) / 2u; \
                            while (index-- != 0u);}

#include "stm32l4/stm32l4xx_ll_adc.h"
#include "stm32l4/stm32l4xx_ll_bus.h"
#include "stm32l4/stm32l4xx_ll_exti.h"
#include "stm32l4/stm32l4xx_ll_gpio.h"
#include "stm32l4/stm32l4xx_ll_pwr.h"
#include "stm32l4/stm32l4xx_ll_rcc.h"
#include "stm32l4/stm32l4xx_ll_system.h"
#include "stm32l4/stm32l4xx_ll_tim.h"
#include "stm32l4/stm32l4xx_ll_usart.h"
#include "stm32l4/stm32l4xx_ll_utils.h"

#include "pinmux/pinmux_stm32l4.h"

#define ADC_RESOLUTION_BITS (12u)

typedef enum
{
  ADC_SOUNDER_CURR  = LL_ADC_CHANNEL_1, // ADC1, ADC2 or ADC3
  ADC_MOTOR_CURR    = LL_ADC_CHANNEL_2, // ADC1, ADC2 or ADC3
  ADC_VBATT_SCALED  = LL_ADC_CHANNEL_3, // ADC1, ADC2 or ADC3
  ADC_SPARE         = LL_ADC_CHANNEL_8, // ADC1 or ADC2
} ADC_channel_t;

#define UART_BAUD_RATE      (57600ul)
#define UART_PERIPH         USART1
#define UART_IRQ            USART1_IRQn
#define UART_IRQ_HANDLER()  void USART1_IRQHandler(void)
#define UART_CLOCK_ENABLE() LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_USART1)
#define UART_CLOCK_SOURCE   LL_RCC_USART1_CLKSOURCE_SYSCLK
#define UART_BUFFER_SIZE    (32u) // This must be a power of two!

// Scheduler timer will interrupt at 1 kHz
#define SYSTICK_HZ          (1000u)
#define SYSTICK_START()     SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk
#define SYSTICK_STOP()      SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk
#define SYSTICK_ISR()       void SysTick_Handler(void)

// Motor PWM 15.6 kHz, when phase correct (centred), with PRESCALER == 1u and TOP == 255u
#define MOTOR_PWM_PRESCALER (1u)
#define MOTOR_PWM_TOP       (255u)
#define MOTOR_PWM(value)    LL_TIM_OC_SetCompareCH2(TIM2, value); LL_TIM_GenerateEvent_UPDATE(TIM2)
#define MOTOR_PWM_START()   LL_TIM_EnableCounter(TIM2); LL_TIM_GenerateEvent_UPDATE(TIM2)
#define MOTOR_PWM_STOP()    MOTOR_PWM(0u); LL_TIM_DisableCounter(TIM2); LL_TIM_GenerateEvent_UPDATE(TIM2)

// Buzzer PWM 880Hz, with PRESCALER == 1u and TOP == 255u
#define BUZZ_PWM_FREQ_HZ    (880u)
#define BUZZ_PWM_TOP        (2u)
#define BUZZ_PWM_PRESCALER  (CPU_CLOCK_HZ / (BUZZ_PWM_FREQ_HZ / BUZZ_PWM_TOP))
#define BUZZ_PWM(value)     LL_TIM_OC_SetCompareCH3(TIM5, value); LL_TIM_GenerateEvent_UPDATE(TIM5)
#define BUZZ_PWM_START()    LL_TIM_EnableCounter(TIM5); LL_TIM_GenerateEvent_UPDATE(TIM5)
#define BUZZ_PWM_STOP()     LL_TIM_DisableCounter(TIM5); LL_TIM_GenerateEvent_UPDATE(TIM5)

// GPIO inputs
#define FPGA_READY_PORT     GPIOB
#define FPGA_READY_PIN      (1u)

#define MOTOR_FAULT_B_PORT  GPIOC
#define MOTOR_FAULT_B_PIN   (5u)

#define MOTOR_FAULT_A_PORT  GPIOC
#define MOTOR_FAULT_A_PIN   (6u)

#define PB_SEL_PORT         GPIOA
#define PB_SEL_PIN          (0u)

#define PB_MUTE_PORT        GPIOB
#define PB_MUTE_PIN         (8u)

#define PB_PLUS_PORT        GPIOB
#define PB_PLUS_PIN         (9u)

#define PB_MINUS_PORT       GPIOB
#define PB_MINUS_PIN        (10u)

#define PGOOD_PORT          GPIOB
#define PGOOD_PIN           (15u)

#define PRESSURE_ALERT_PORT GPIOC
#define PRESSURE_ALERT_PIN  (11u)

#define SW_BACKUP_PORT      GPIOA
#define SW_BACKUP_PIN       (8u)

#define SW_END_PORT         GPIOC
#define SW_END_PIN          (8u)

#define SW_HOME_PORT        GPIOC
#define SW_HOME_PIN         (9u)

// GPIO outputs
#define BATTERY_LED_PORT    GPIOC
#define BATTERY_LED_PIN     (15u)

#define CE_CONTROL_PORT     GPIOB
#define CE_CONTROL_PIN      (14u)

#define LDO_EN_PORT         GPIOC
#define LDO_EN_PIN          (7u)

#define MOTOR_IN_A_PORT     GPIOB
#define MOTOR_IN_A_PIN      (12u)

#define MOTOR_IN_B_PORT     GPIOB
#define MOTOR_IN_B_PIN      (13u)

#define PWR_UP_PORT         GPIOC
#define PWR_UP_PIN          (10u)

#endif /* BOARD_V3_H */
