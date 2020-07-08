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

// This must be a power of two!
#define UART_BUFFER_SIZE    (32u)

#define UART_BAUD_RATE      (57600ul)

// Scheduler timer will interrupt at 1 kHz
// #define SCHEDULER_TIM_CFG()
#define SCHEDULER_START()   LL_TIM_EnableCounter(TIM6_BASE)
#define SCHEDULER_STOP()    LL_TIM_DisableCounter(TIM6_BASE)
#define SCHEDULER_ISR()     void TIM6_IRQHandler(void)

// Motor PWM 3.9 kHz, phase correct
// #define MOTOR_PWM_TIM_CFG()
#define MOTOR_PWM_START()   LL_TIM_DisableCounter(TIM2_BASE)
#define MOTOR_PWM_STOP()    LL_TIM_DisableCounter(TIM2_BASE)
// #define MOTOR_PWM(value)

// ADC prescalar 64 chosen for <200 kHz ADC clock (needed for 10-bit resolution)
// #define ADC_PRESCALER
// #define ADC_DIGITAL_DISABLE
// #define ADC_RESOLUTION_BITS (10u)

// Thermistor
#define THERM_FIXED_RESIST  (10000u)

#define ADC_PIN_CFG         { 0u, LL_GPIO_MODE_ANALOG, LL_GPIO_SPEED_LOW, LL_GPIO_OUTPUT_OPENDRAIN, LL_GPIO_PULL_NO, LL_GPIO_AF_0 }
#define OUT_PIN_CFG         { 0u, LL_GPIO_MODE_OUTPUT, LL_GPIO_SPEED_LOW, LL_GPIO_OUTPUT_PUSHPULL, LL_GPIO_PULL_NO, LL_GPIO_AF_0 }
#define IN_PU_PIN_CFG       { 0u, LL_GPIO_MODE_INPUT, LL_GPIO_SPEED_LOW, LL_GPIO_OUTPUT_OPENDRAIN, LL_GPIO_PULL_UP, LL_GPIO_AF_0 }

#define ADC_PORT            GPIOA
#define ADC_SOUNDER_PIN     LL_GPIO_PIN_0
#define ADC_MOTOR_PIN       LL_GPIO_PIN_1
#define ADC_VBATT_PIN       LL_GPIO_PIN_2
#define ADC_PRESSURE_PIN    LL_GPIO_PIN_3
#define ADC_FLOW_PIN        LL_GPIO_PIN_4
#define ADC_TEMP_PIN        LL_GPIO_PIN_5
#define ADC_SPARE_PIN       LL_GPIO_PIN_6
#define ADC_MASK            (ADC_SOUNDER_PIN | ADC_MOTOR_PIN | ADC_VBATT_PIN | ADC_PRESSURE_PIN | ADC_FLOW_PIN | ADC_TEMP_PIN | ADC_SPARE_PIN)

#define ALERT_PORT          GPIOA
#define ALERT_FLOW_PIN      LL_GPIO_PIN_7
#define ALERT_VBATT_PIN     LL_GPIO_PIN_8
#define ALERT_PRESSURE_PIN  LL_GPIO_PIN_12
#define ALERT_MASK          (ALERT_FLOW_PIN | ALERT_VBATT_PIN | ALERT_PRESSURE_PIN)

#define UART_PORT           GPIOA
#define UART_TX_PIN         LL_GPIO_PIN_9
#define UART_TX_PIN_CFG     { UART_TX_PIN, LL_GPIO_MODE_OUTPUT, LL_GPIO_SPEED_MEDIUM, LL_GPIO_OUTPUT_PUSHPULL, LL_GPIO_PULL_NO, LL_GPIO_AF_7 }
#define UART_RX_PIN         LL_GPIO_PIN_10
#define UART_RX_PIN_CFG     { UART_RX_PIN, LL_GPIO_MODE_INPUT, LL_GPIO_SPEED_MEDIUM, LL_GPIO_OUTPUT_OPENDRAIN, LL_GPIO_PULL_NO, LL_GPIO_AF_7 }

#define STAT2_PORT          GPIOA
#define STAT2_PIN           LL_GPIO_PIN_11

// MOTOR_PWM on TIM2 CH1
#define MOTOR_PWM_PORT      GPIOA
#define MOTOR_PWM_PIN       LL_GPIO_PIN_15
#define MOTOR_PWM_PIN_CFG   { MOTOR_PWM_PIN, LL_GPIO_MODE_OUTPUT, LL_GPIO_SPEED_MEDIUM, LL_GPIO_OUTPUT_PUSHPULL, LL_GPIO_PULL_NO, LL_GPIO_AF_1 }

#define LCD_PORT            GPIOB
#define LCD_RS_PIN          LL_GPIO_PIN_0
#define LCD_RW_PIN          LL_GPIO_PIN_1
#define LCD_D4_PIN          LL_GPIO_PIN_3
#define LCD_D5_PIN          LL_GPIO_PIN_4
#define LCD_D6_PIN          LL_GPIO_PIN_5
#define LCD_D7_PIN          LL_GPIO_PIN_6
#define LCD_ENABLE_1_PIN    LL_GPIO_PIN_2
#define LCD_ENABLE_2_PIN    LL_GPIO_PIN_7
#define LCD_MASK            (LCD_RS_PIN | LCD_RW_PIN | LCD_D4_PIN | LCD_D5_PIN | LCD_D6_PIN | LCD_D7_PIN | LCD_ENABLE_1_PIN | LCD_ENABLE_2_PIN)

#define SWITCH_PORT         GPIOB
#define SWITCH_1_PIN        LL_GPIO_PIN_8
#define SWITCH_2_PIN        LL_GPIO_PIN_9
#define SWITCH_3_PIN        LL_GPIO_PIN_10
#define SWITCH_4_PIN        LL_GPIO_PIN_11
#define SWITCH_MASK         (SWITCH_1_PIN | SWITCH_2_PIN | SWITCH_3_PIN | SWITCH_4_PIN)

#define MOTOR_IN_PORT       GPIOB
#define MOTOR_IN_A_PIN      LL_GPIO_PIN_12
#define MOTOR_IN_B_PIN      LL_GPIO_PIN_13
#define MOTOR_IN_MASK       (MOTOR_IN_A_PIN | MOTOR_IN_B_PIN)

#define CE_CONTROL_PORT     GPIOB
#define CE_CONTROL_PIN      LL_GPIO_PIN_14

#define PGOOD_PORT          GPIOB
#define PGOOD_PIN           LL_GPIO_PIN_15

#define STAT1_PORT          GPIOC
#define STAT1_PIN           LL_GPIO_PIN_13

#define ALERT_ENABLE_PORT   GPIOC
#define ALERT_ENABLE_PIN    LL_GPIO_PIN_14

#define LED_PORT            GPIOC
#define LED_PIN             LL_GPIO_PIN_15

#define PARALLEL_PORT       LCD_PORT

// typedef enum
// {
//   ADC_FLOW          = 0u,
//   ADC_PRESSURE      = 1u,
//   ADC_VBATT         = 2u,
//   ADC_MOTOR_CURRENT = 3u,
//   ADC_TEMP          = 7u
// } ADC_channel_t;

#endif /* BOARD_MK2_H */
