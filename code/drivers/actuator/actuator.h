// Copyright (c) 2021 <OpenVent-Bristol, Donald Robson>

#ifndef ACTUATOR_H
#define ACTUATOR_H

#include <stdint.h>

/**
 * @brief Update scalar parameters used for pid control
 * @note The actuator will *not* be usable until this function has been called, as it also sets the
 * max and min outputs to those defined in the board header
 * @note This will stop the actuator if called again during a stroke, and reset the fault state
 *
 * @param kp          Scalar for proportional parameter
 * @param ki          Scalar for integral parameter
 * @param kd          Scalar for differential parameter
 */
void actuator_init(float kp, float ki, float kd);

/**
 * @brief Immediately bring the actuator to a stop, cancelling PID control if active
 *
 * @param arg         Not used
 */
void actuator_stop(int32_t arg);

/**
 * @brief Return the actuator to its home position
 *
 * @param speed       0 (stopped) to 255 (full speed)
 */
void actuator_constant_speed_home(uint8_t speed);

/**
 * @brief Run the actuator forward
 * @note It is not possible to run the actuator at high speed in this mode.  For test and
 * calibration only.
 *
 * @param speed       0 (stopped) to 63 (one quarter speed - internally capped)
 */
void actuator_constant_speed_push(uint8_t speed);

/**
 * @brief Run the actuator forward using PID control and reset time
 * @note The actuator will not begin to move until a measurement is supplied using
 * actuator_run
 *
 * @param set_point   Target value for control
 */
void actuator_pid_control_push(int16_t set_point);

#endif /* ACTUATOR_H */
