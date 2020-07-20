// This file should not be included directly.  Include board/board.h instead and define a board ID, e.g. BOARD_MK1
#ifndef BOARD_MK2_H
#define BOARD_MK2_H

#define CPU_CLOCK_HZ        ((uint32_t)16000000U)
// TODO - using NUCLEO 476RG board at the moment which has no HSE oscillator
#define HSI_VALUE           CPU_CLOCK_HZ
// #define HSE_VALUE           CPU_CLOCK_HZ

#include "stm32l4/stm32l4xx_ll_adc.h"
#include "stm32l4/stm32l4xx_ll_bus.h"
#include "stm32l4/stm32l4xx_ll_exti.h"
#include "stm32l4/stm32l4xx_ll_gpio.h"
#include "stm32l4/stm32l4xx_ll_pwr.h"
#include "stm32l4/stm32l4xx_ll_rcc.h"
#include "stm32l4/stm32l4xx_ll_tim.h"
#include "stm32l4/stm32l4xx_ll_usart.h"
#include "stm32l4/stm32l4xx_ll_utils.h"

// TODO - USART2 is virtual COM port on NUCLEO boards, but USART1 will be used with STM32L412
#define UART                (2u)
#define UART_BAUD_RATE      (57600ul)

#if (UART == 1u)
  #define UART_PERIPH         USART1
  #define UART_IRQ            USART1_IRQn
  #define UART_IRQ_HANDLER()  void USART1_IRQHandler(void)
  #define UART_CLOCK_ENABLE() LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_USART1)
  #define UART_CLOCK_SOURCE   LL_RCC_USART1_CLKSOURCE_SYSCLK
#elif (UART == 2u)
  #define UART_PERIPH         USART2
  #define UART_IRQ            USART2_IRQn
  #define UART_IRQ_HANDLER()  void USART2_IRQHandler(void)
  #define UART_CLOCK_ENABLE() LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_USART2)
  #define UART_CLOCK_SOURCE   LL_RCC_USART2_CLKSOURCE_SYSCLK
#else
  #error UART not supported in board definitions file.
#endif

// This must be a power of two!
#define UART_BUFFER_SIZE    (32u)

// Scheduler timer will interrupt at 1 kHz
#define SCHEDULER_HZ        (1000u)
#define SCHEDULER_START()   SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk
#define SCHEDULER_STOP()    SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk
#define SCHEDULER_ISR()     void SysTick_Handler(void)

// Motor PWM 3.9 kHz, phase correct
#define MOTOR_PWM_START()   LL_TIM_DisableCounter(TIM2)
#define MOTOR_PWM_STOP()    LL_TIM_EnableCounter(TIM2)
#define MOTOR_PWM(value)    LL_TIM_SetAutoReload(TIM2, value)

#define ADC_RESOLUTION_BITS (12u)

// Thermistor
#define THERM_FIXED_RESIST  (10000u)

#define ADC_PIN_CFG         { 0u, LL_GPIO_MODE_ANALOG, LL_GPIO_SPEED_LOW, LL_GPIO_OUTPUT_OPENDRAIN, LL_GPIO_PULL_NO, LL_GPIO_AF_0 }
#define OUT_PIN_CFG         { 0u, LL_GPIO_MODE_OUTPUT, LL_GPIO_SPEED_LOW, LL_GPIO_OUTPUT_PUSHPULL, LL_GPIO_PULL_NO, LL_GPIO_AF_0 }
#define IN_PU_PIN_CFG       { 0u, LL_GPIO_MODE_INPUT, LL_GPIO_SPEED_LOW, LL_GPIO_OUTPUT_OPENDRAIN, LL_GPIO_PULL_UP, LL_GPIO_AF_0 }

#define ADC_A_PORT          GPIOA
#define ADC_A_PRESSURE_PIN  (0u)
#define ADC_A_OXYGEN_PIN    (1u)
#define ADC_A_TEMP_PIN      (2u)
#define ADC_A_SPARE_PIN     (3u)
#define ADC_A_MASK          ((1u << ADC_A_PRESSURE_PIN) | (1u << ADC_A_OXYGEN_PIN) | (1u << ADC_A_TEMP_PIN) | (1u << ADC_A_SPARE_PIN))

#define ADC_B_PORT          GPIOC
#define ADC_B_SOUNDER_PIN   (0u)
#define ADC_B_MOTOR_PIN     (1u)
#define ADC_B_VBATT_PIN     (2u)
#define ADC_B_FLOW_PIN      (3u)
#define ADC_B_MASK          ((1u << ADC_B_SOUNDER_PIN) | (1u << ADC_B_MOTOR_PIN) | (1u << ADC_B_VBATT_PIN) | (1u << ADC_B_FLOW_PIN))

#define UART_PORT           GPIOA
#define UART_TX_PIN         (9u)
#define UART_TX_PIN_CFG     { (1u << UART_TX_PIN), LL_GPIO_MODE_OUTPUT, LL_GPIO_SPEED_MEDIUM, LL_GPIO_OUTPUT_PUSHPULL, LL_GPIO_PULL_NO, LL_GPIO_AF_7 }
#define UART_RX_PIN         (10u)
#define UART_RX_PIN_CFG     { (1u << UART_RX_PIN), LL_GPIO_MODE_INPUT, LL_GPIO_SPEED_MEDIUM, LL_GPIO_OUTPUT_OPENDRAIN, LL_GPIO_PULL_NO, LL_GPIO_AF_7 }

#define USB_PORT            GPIOA
#define USB_DM_PIN          (11u)
#define USB_DM_PIN_CFG      { (1u << USB_DM_PIN), LL_GPIO_MODE_OUTPUT, LL_GPIO_SPEED_VERY_HIGH, LL_GPIO_OUTPUT_OPENDRAIN, LL_GPIO_PULL_NO, LL_GPIO_AF_10 }
#define USB_DP_PIN          (12u)
#define USB_DP_PIN_CFG      { (1u << USB_DP_PIN), LL_GPIO_MODE_OUTPUT, LL_GPIO_SPEED_VERY_HIGH, LL_GPIO_OUTPUT_OPENDRAIN, LL_GPIO_PULL_NO, LL_GPIO_AF_10 }

// MOTOR_PWM on TIM2 CH1
#define MOTOR_PWM_PORT      GPIOA
#define MOTOR_PWM_PIN       (5u)
#define MOTOR_PWM_PIN_CFG   { (1u << MOTOR_PWM_PIN), LL_GPIO_MODE_OUTPUT, LL_GPIO_SPEED_MEDIUM, LL_GPIO_OUTPUT_PUSHPULL, LL_GPIO_PULL_NO, LL_GPIO_AF_1 }

#define LCD_PORT            GPIOB
#define LCD_RS_PIN          (0u)
#define LCD_RW_PIN          (1u)
#define LCD_D4_PIN          (3u)
#define LCD_D5_PIN          (4u)
#define LCD_D6_PIN          (5u)
#define LCD_D7_PIN          (6u)
#define LCD_ENABLE_1_PIN    (2u)
#define LCD_ENABLE_2_PIN    (7u)
#define LCD_MASK            ((1u << LCD_RS_PIN) | (1u << LCD_RW_PIN) | (1u << LCD_D4_PIN) | (1u << LCD_D5_PIN) | (1u << LCD_D6_PIN) | (1u << LCD_D7_PIN) | (1u << LCD_ENABLE_1_PIN) | (1u << LCD_ENABLE_2_PIN))

#define SWITCH_PORT         GPIOB
#define SWITCH_1_PIN        (8u)
#define SWITCH_2_PIN        (9u)
#define SWITCH_3_PIN        (10u)
#define SWITCH_4_PIN        (11u)
#define SWITCH_MASK         ((1u << SWITCH_1_PIN) | (1u << SWITCH_2_PIN) | (1u << SWITCH_3_PIN) | (1u << SWITCH_4_PIN))

#define MOTOR_IN_PORT       GPIOB
#define MOTOR_IN_A_PIN      (12u)
#define MOTOR_IN_B_PIN      (13u)
#define MOTOR_IN_MASK       ((1u << MOTOR_IN_A_PIN) | (1u << MOTOR_IN_B_PIN))

#define ALERT_PORT          GPIOC
#define ALERT_MOTOR_A_N_PIN (4u)
#define ALERT_MOTOR_B_N_PIN (5u)
#define ALERT_VBATT_N_PIN   (9u)
#define ALERT_PRESS_N_PIN   (11u)
#define ALERT_MASK          ((1u << ALERT_MOTOR_A_N_PIN) | (1u << ALERT_MOTOR_B_N_PIN) | (1u << ALERT_VBATT_N_PIN) | (1u << ALERT_PRESS_N_PIN))

#define CHARGE_CONTROL_PORT GPIOB
#define CHARGE_CONTROL_PIN  (14u)
#define CHARGE_CONTROL_MASK (1u << CHARGE_CONTROL_PIN)

#define CHARGE_PGOOD_PORT   GPIOB
#define CHARGE_PGOOD_PIN    (15u)
#define CHARGE_PGOOD_MASK   (1u << CHARGE_PGOOD_PIN)

#define CHARGE_ISNS_PORT    GPIOC
#define CHARGE_ISNS_PIN     (6u)
#define CHARGE_ISNS_MASK    (1u << CHARGE_ISNS_PIN)

#define CHARGE_STAT_PORT    GPIOC
#define CHARGE_STAT2_PIN    (10u)
#define CHARGE_STAT1_PIN    (12u)
#define CHARGE_STAT_MASK    ((1u << CHARGE_STAT2_PIN) | (1u << CHARGE_STAT1_PIN))

#define ALERT_ENABLE_N_PORT GPIOC
#define ALERT_ENABLE_N_PIN  (13u)
#define ALERT_ENABLE_N_MASK (1u << ALERT_ENABLE_N_PIN)

#define LED_PORT            GPIOC
#define LED_PIN             (15u)
#define LED_MASK            (1u << LED_PIN)

#define PARALLEL_PORT       LCD_PORT

typedef enum
{
  ADC_SOUNDER       = LL_ADC_CHANNEL_1, // ADC1 or ADC2
  ADC_MOTOR_CURRENT = LL_ADC_CHANNEL_2, // ADC1 or ADC2
  ADC_VBATT         = LL_ADC_CHANNEL_3, // ADC1 or ADC2
  ADC_FLOW          = LL_ADC_CHANNEL_4, // ADC1 or ADC2
  ADC_PRESSURE      = LL_ADC_CHANNEL_5, // ADC1 only
  ADC_OXYGEN        = LL_ADC_CHANNEL_6, // ADC1 only
  ADC_TEMP          = LL_ADC_CHANNEL_7, // ADC1 or ADC2
  ADC_SPARE         = LL_ADC_CHANNEL_8  // ADC1 or ADC2
} ADC_channel_t;

#endif /* BOARD_MK2_H */
