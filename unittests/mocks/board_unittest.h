// Copyright (c) 2021 <OpenVent-Bristol, Donald Robson>

#ifndef BOARD_UNITTEST_H
#define BOARD_UNITTEST_H

#define FPGA_UPDATE_INTERVAL_MS (5u)

void GLOBAL_INTERRUPTS_DISABLE(void);
void GLOBAL_INTERRUPTS_ENABLE(void);

void MOTOR_PWM_START(void);
void MOTOR_PWM_STOP(void);
void MOTOR_PWM(int32_t);

#define MOTOR_PWM_TOP     (255u)

#define MOTOR_IN_A_PORT   'A'
#define MOTOR_IN_A_PIN    'a'
#define MOTOR_IN_B_PORT   'B'
#define MOTOR_IN_B_PIN    'b'


#endif /* BOARD_UNITTEST_H */
