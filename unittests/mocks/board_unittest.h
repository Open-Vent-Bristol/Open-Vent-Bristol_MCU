// Copyright (c) 2021 <OpenVent-Bristol, Donald Robson>

#ifndef BOARD_UNITTEST_H
#define BOARD_UNITTEST_H

#include <stdint.h>

void GLOBAL_INTERRUPTS_DISABLE(void);
void GLOBAL_INTERRUPTS_ENABLE(void);

void MOTOR_PWM_START(void);
void MOTOR_PWM_STOP(void);
void MOTOR_PWM(uint8_t);

#define MOTOR_PWM_TOP     (255u)

#define MOTOR_IN_A_PORT   'A'
#define MOTOR_IN_A_PIN    'a'
#define MOTOR_IN_B_PORT   'B'
#define MOTOR_IN_B_PIN    'b'

void BUZZ_PWM_START(void);
void BUZZ_PWM_STOP(void);

void FAN_PWM_START(void);
void FAN_PWM_STOP(void);
void FAN_PWM(uint8_t);

#endif /* BOARD_UNITTEST_H */
