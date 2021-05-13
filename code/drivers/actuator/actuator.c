// Copyright (c) 2021 <OpenVent-Bristol, Donald Robson>

#include "actuator.h"
#include "private/actuator_priv.h"
#include "gpio/gpio.h"
#include "misc/telemetry_buffer.h"
#include "private/pid.h"
#include "board/board.h"
#include "scheduler/dispatcher.h"
#include "scheduler/timer.h"
#include "misc/util.h"
#include <string.h>

pid_data_t s_pid = {0};

actuator_t s_actuator =
{
  .mode = FAULT
};

timer_t s_timer =
{
  .type = TIMER_TYPE_CONTINUOUS,
  .initial_ticks = UPDATE_INTERVAL_MS,
  .remaining_ticks = UPDATE_INTERVAL_MS,
  .events_signalled = EV_ACTUATOR_SERVICE,
  .unique_id = TIMER_ACTUATOR_SERVICE
};

// TODO - Mechanical fault monitoring #48
// Circular buffers to track the time period of push strokes
// Every 256 push strokes an average of the preceding eight strokes will be recorded.
// At 8 strokes per minute, long term tracking of one minute of sampling per hour for about 8 hours.
// At 8 strokes per minute, short term tracking of every stroke for about 1 minutes.
// By comparing the values, the driver can spot if the resistance to motion has changed decreased.
// telemetry_buffer_t s_short_term_stroke_tracker = {0u};
// telemetry_buffer_t s_long_term_stroke_tracker = {0u};

/**
 * @brief Cap a speed.  For use with constant speed modes.
 *
 * @param speed
 */
static uint8_t cap_constant_speed(uint8_t speed)
{
  if (speed > CONSTANT_SPEED_CAP)
  {
    speed = CONSTANT_SPEED_CAP;
  }

  return speed;
}

/**
 * @brief Update the speed of the motor.
 *
 * @param arg  Timer unique ID (unused)
 */
TESTABLE void actuator_run(int32_t arg)
{
  switch (s_actuator.mode)
  {
  case PUSHING:
  case HOMING:
    {
      int32_t error = (int32_t)s_actuator.target_speed - (int32_t)s_actuator.current_speed;
      int32_t ds = ACCELERATION_CAP * UPDATE_INTERVAL_MS;

      if (error != 0)
      {
        if (error > ds)
        {
          error = ds;
        }
        else if (error < -ds)
        {
          error = -ds;
        }

        s_actuator.current_speed += error;
      }

      MOTOR_PWM(s_actuator.current_speed);
    }
    break;

  case FAULT:
    // TODO - alarm?
  case STOP:
  case PUSHING_PID:
    break;
  }
}

/**
 * @brief Update the speed of the motor.  This function should be called often.
 *
 * @param measured_value  PID control parameter
 */
TESTABLE void actuator_run_pid(int32_t measured_value)
{
  switch (s_actuator.mode)
  {
  case PUSHING_PID:
    s_actuator.current_speed = pid_update(&s_pid, UPDATE_INTERVAL_MS, (int16_t)measured_value);
    MOTOR_PWM(s_actuator.current_speed);
    break;

  case FAULT:
    // TODO - alarm?
  case STOP:
  case PUSHING:
  case HOMING:
    break;
  }
}

void actuator_init(float kp, float ki, float kd)
{
  actuator_stop(0);

  // Reset the fault state
  s_actuator.mode = STOP;

  pid_data_t new_pid_cfg =
  {
    .output_max = MOTOR_PWM_TOP,
    .kp = kp,
    .ki = ki,
    .kd = kd,
  };

  memcpy(&s_pid, &new_pid_cfg, sizeof(s_pid));

  // Timer signals EV_ACTUATOR_SERVICE for constant speed modes
  timer_attach(&s_timer);

  dispatcher_bind(EV_ACTUATOR_SERVICE, actuator_run);
  dispatcher_bind(EV_PRESSURE_UPDATE, actuator_run_pid);
}

void actuator_stop(int32_t arg)
{
  (void)arg;

  timer_stop(&s_timer);

  MOTOR_PWM_STOP();
  gpio_clear_pin(MOTOR_IN_A_PORT, MOTOR_IN_A_PIN);
  gpio_clear_pin(MOTOR_IN_B_PORT, MOTOR_IN_B_PIN);

  s_actuator.current_speed = 0u;
  s_actuator.target_speed = 0u;

  if (s_actuator.mode != FAULT)
  {
    s_actuator.mode = STOP;
  }
}

void actuator_constant_speed_home(uint8_t speed)
{
  if (s_actuator.mode != FAULT)
  {
    if (s_actuator.mode != HOMING)
    {
      MOTOR_PWM_STOP();
      gpio_set_pin(MOTOR_IN_B_PORT, MOTOR_IN_B_PIN);
      gpio_clear_pin(MOTOR_IN_A_PORT, MOTOR_IN_A_PIN);
      MOTOR_PWM_START();

      s_actuator.current_speed = 0u;
      s_actuator.mode = HOMING;

      timer_reset(&s_timer);
    }

    s_actuator.target_speed = speed;
  }
}

void actuator_constant_speed_push(uint8_t speed)
{
  if (s_actuator.mode != FAULT)
  {
    if (s_actuator.mode != PUSHING)
    {
      MOTOR_PWM_STOP();
      gpio_set_pin(MOTOR_IN_A_PORT, MOTOR_IN_A_PIN);
      gpio_clear_pin(MOTOR_IN_B_PORT, MOTOR_IN_B_PIN);
      MOTOR_PWM_START();

      s_actuator.current_speed = 0u;
      s_actuator.mode = PUSHING;

      timer_reset(&s_timer);
    }

    s_actuator.target_speed = cap_constant_speed(speed);
  }
}

void actuator_pid_control_push(int16_t set_point)
{
  if (s_actuator.mode != FAULT)
  {
    if (s_actuator.mode != PUSHING_PID)
    {
      timer_stop(&s_timer);

      MOTOR_PWM_STOP();
      gpio_set_pin(MOTOR_IN_A_PORT, MOTOR_IN_A_PIN);
      gpio_clear_pin(MOTOR_IN_B_PORT, MOTOR_IN_B_PIN);
      MOTOR_PWM_START();

      s_actuator.current_speed = 0u;
      s_actuator.mode = PUSHING_PID;
    }

    s_pid.set_point = set_point;
  }
}
