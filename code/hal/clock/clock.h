#ifndef CLOCK_H
#define CLOCK_H

#include <stdint.h>

#ifdef __cplusplus
extern "C"
{
#endif

/**
 * @brief Bring up all required MCU clocks and setup timers
 *
 */
void clock_init(void);

/**
 * @brief Turn on the motor PWM clock
 *
 */
void clock_motor_pwm_enable(void);

/**
 * @brief Turn off the motor PWM clock
 *
 */
void clock_motor_pwm_disable(void);

/**
 * @brief Set the motor PWM clock ratio
 *
 * @param value    PWM ratio from 0 to 255. 0 is fully off and 255 is fully on.
 */
void clock_motor_pwm_set(uint8_t value);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* CLOCK_H */
