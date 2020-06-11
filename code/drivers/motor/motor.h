#ifndef MOTOR_H
#define MOTOR_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef enum
{
  MOTOR_OFF = 0u,
  MOTOR_FORWARD = 1u,
  MOTOR_REVERSE = 2u,
} motor_mode_t;

/**
 * @brief Setup the motor PWM timer and GPIOs
 *
 */
void motor_init(void);

/**
 * @brief Set the motor speed
 *
 * @param speed     Speed ranging from 0 to 255
 */
void motor_set_speed(uint8_t speed);

/**
 * @brief Set the motor mode
 *
 * @param mode     Motor mode
 */
void motor_set_mode(motor_mode_t mode);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* MOTOR_H */