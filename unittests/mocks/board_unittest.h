// Copyright (c) 2021 <OpenVent-Bristol, Donald Robson>

#ifndef BOARD_UNITTEST_H
#define BOARD_UNITTEST_H

#include <stdint.h>

void GLOBAL_INTERRUPTS_DISABLE(void);
void GLOBAL_INTERRUPTS_ENABLE(void);

void MOTOR_PWM_START(void);
void MOTOR_PWM_STOP(void);
void MOTOR_PWM(uint8_t);

#define SYSTICK_ISR()     void systick(void)

#define MOTOR_PWM_TOP     (255u)

#define MOTOR_IN_A_PORT     'A'
#define MOTOR_IN_A_PIN      'a'
#define MOTOR_IN_B_PORT     'B'
#define MOTOR_IN_B_PIN      'b'

#define MOTOR_FAULT_A_PIN   (1)
#define MOTOR_FAULT_B_PIN   (2)
#define SW_END_PIN          (3)
#define SW_BACKUP_PIN       (4)
#define SW_HOME_PIN         (5)
#define PB_SEL_PIN          (6)
#define FPGA_READY_PIN      (7)
#define PRESSURE_ALERT_PIN  (8)

void BUZZ_PWM_START(void);
void BUZZ_PWM_STOP(void);

void FAN_PWM_START(void);
void FAN_PWM_STOP(void);
void FAN_PWM(uint8_t);

uint32_t LL_EXTI_ClearFlag_0_31(uint32_t mask);
uint32_t LL_EXTI_IsActiveFlag_0_31(uint32_t mask);

#endif /* BOARD_UNITTEST_H */
