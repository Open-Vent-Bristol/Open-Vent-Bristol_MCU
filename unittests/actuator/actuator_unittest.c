// Copyright (c) 2021 <OpenVent-Bristol, Donald Robson>

#include "unity.h"
#include "unity_fixture.h"
#include "test_macros.h"
#include "board_mock.h"
#include "dispatcher_mock.h"
#include "gpio_mock.h"
#include "pid_mock.h"
#include "timer_mock.h"
#include "actuator/actuator.h"
#include "actuator/private/actuator_priv.h"
#include <stdbool.h>

extern pid_data_t s_pid;
extern actuator_t s_actuator;
extern timer_t s_timer;

extern void actuator_run(int32_t arg);
extern void actuator_run_pid(int32_t measured_value);

/* ACTUATOR INIT TESTS */

TEST_GROUP(actuator_init_tests);

TEST_SETUP(actuator_init_tests)
{
  memset(&s_pid, 0, sizeof(s_pid));

  BOARD_MOCKS(RESET_FAKE);
  DISPATCHER_MOCKS(RESET_FAKE);
  GPIO_MOCKS(RESET_FAKE);
  TIMER_MOCKS(RESET_FAKE);
  RESET_FAKE(pid_update);
}

TEST_TEAR_DOWN(actuator_init_tests)
{}

TEST(actuator_init_tests, init_stops_motor)
{
  actuator_init(1, 1, 1);
  TEST_ASSERT_EQUAL_INT(1u, MOTOR_PWM_STOP_fake.call_count);
}

TEST(actuator_init_tests, init_configures_pid_control)
{
  actuator_init(2, 4, 8);
  TEST_ASSERT_EQUAL_FLOAT(2, s_pid.kp);
  TEST_ASSERT_EQUAL_FLOAT(4, s_pid.ki);
  TEST_ASSERT_EQUAL_FLOAT(8, s_pid.kd);
}

TEST(actuator_init_tests, init_starts_service_timer)
{
  actuator_init(1, 1, 1);
  TEST_ASSERT_EQUAL_INT(1u, timer_attach_fake.call_count);
  TEST_ASSERT_EQUAL_PTR(&s_timer, timer_attach_fake.arg0_val);
}

TEST(actuator_init_tests, init_binds_events)
{
  actuator_init(1, 1, 1);
  TEST_ASSERT_EQUAL_INT(2u, dispatcher_bind_fake.call_count);
  TEST_ASSERT_VALUE_IN_ARRAY(EV_PRESSURE_UPDATE, dispatcher_bind_fake.arg0_history);
  TEST_ASSERT_VALUE_IN_ARRAY(EV_ACTUATOR_SERVICE, dispatcher_bind_fake.arg0_history);
}

TEST(actuator_init_tests, init_clears_fault)
{
  actuator_init(1, 1, 1);
  TEST_ASSERT_EQUAL_INT(STOP, s_actuator.mode);
}

/* ACTUATOR MODE TESTS */

TEST_GROUP(actuator_mode_tests);

TEST_SETUP(actuator_mode_tests)
{
  memset(&s_pid, 0, sizeof(s_pid));
  actuator_init(1, 1, 1);

  BOARD_MOCKS(RESET_FAKE);
  DISPATCHER_MOCKS(RESET_FAKE);
  GPIO_MOCKS(RESET_FAKE);
  TIMER_MOCKS(RESET_FAKE);
  RESET_FAKE(pid_update);
}

TEST_TEAR_DOWN(actuator_mode_tests)
{}

TEST(actuator_mode_tests, stop_stops_motor)
{
  actuator_stop(0);
  TEST_ASSERT_EQUAL_INT(1u, MOTOR_PWM_STOP_fake.call_count);
  TEST_ASSERT_EQUAL_INT(2u, gpio_clear_pin_fake.call_count);
  TEST_ASSERT_VALUE_IN_ARRAY(MOTOR_IN_A_PORT, gpio_clear_pin_fake.arg0_history);
  TEST_ASSERT_VALUE_IN_ARRAY(MOTOR_IN_A_PIN, gpio_clear_pin_fake.arg1_history);
  TEST_ASSERT_VALUE_IN_ARRAY(MOTOR_IN_B_PORT, gpio_clear_pin_fake.arg0_history);
  TEST_ASSERT_VALUE_IN_ARRAY(MOTOR_IN_B_PIN, gpio_clear_pin_fake.arg1_history);
}

TEST(actuator_mode_tests, stop_stops_service_timer)
{
  actuator_stop(0);
  TEST_ASSERT_EQUAL_INT(1u, timer_stop_fake.call_count);
  TEST_ASSERT_EQUAL_PTR(&s_timer, timer_stop_fake.arg0_val);
}

TEST(actuator_mode_tests, stop_doesnt_clear_fault)
{
  s_actuator.mode = FAULT;
  actuator_stop(0);
  TEST_ASSERT_EQUAL_INT(FAULT, s_actuator.mode);
}

TEST(actuator_mode_tests, constant_speed_home_stops_motor)
{
  actuator_constant_speed_home(255u);
  TEST_ASSERT_EQUAL_INT(1u, MOTOR_PWM_STOP_fake.call_count);
}

TEST(actuator_mode_tests, constant_speed_home_doesnt_stop_on_second_call)
{
  actuator_constant_speed_home(255u);
  actuator_constant_speed_home(255u);
  TEST_ASSERT_EQUAL_INT(1u, MOTOR_PWM_STOP_fake.call_count);
}

TEST(actuator_mode_tests, constant_speed_home_no_change_during_fault)
{
  s_actuator.mode = FAULT;
  actuator_constant_speed_home(255u);
  TEST_ASSERT_EQUAL_INT(FAULT, s_actuator.mode);
}

TEST(actuator_mode_tests, constant_speed_home_sets_motor_direction)
{
  actuator_constant_speed_home(255u);
  TEST_ASSERT_EQUAL_INT(1u, gpio_set_pin_fake.call_count);
  TEST_ASSERT_EQUAL_CHAR(MOTOR_IN_B_PORT, gpio_set_pin_fake.arg0_val);
  TEST_ASSERT_EQUAL_CHAR(MOTOR_IN_B_PIN, gpio_set_pin_fake.arg1_val);
  TEST_ASSERT_EQUAL_INT(1u, gpio_clear_pin_fake.call_count);
  TEST_ASSERT_EQUAL_CHAR(MOTOR_IN_A_PORT, gpio_clear_pin_fake.arg0_val);
  TEST_ASSERT_EQUAL_CHAR(MOTOR_IN_A_PIN, gpio_clear_pin_fake.arg1_val);
}

TEST(actuator_mode_tests, constant_speed_home_sets_target_speed)
{
  actuator_constant_speed_home(255u);
  TEST_ASSERT_EQUAL_INT(255u, s_actuator.target_speed);
  actuator_constant_speed_home(1u);
  TEST_ASSERT_EQUAL_INT(1u, s_actuator.target_speed);
}

TEST(actuator_mode_tests, constant_speed_home_resets_service_timer)
{
  actuator_constant_speed_home(255u);
  TEST_ASSERT_EQUAL_INT(1u, timer_reset_fake.call_count);
  TEST_ASSERT_EQUAL_PTR(&s_timer, timer_reset_fake.arg0_val);
}

TEST(actuator_mode_tests, constant_speed_push_stops_motor)
{
  actuator_constant_speed_push(255u);
  TEST_ASSERT_EQUAL_INT(1u, MOTOR_PWM_STOP_fake.call_count);
}

TEST(actuator_mode_tests, constant_speed_push_doesnt_stop_on_second_call)
{
  actuator_constant_speed_push(255u);
  actuator_constant_speed_push(255u);
  TEST_ASSERT_EQUAL_INT(1u, MOTOR_PWM_STOP_fake.call_count);
}

TEST(actuator_mode_tests, constant_speed_push_no_change_during_fault)
{
  s_actuator.mode = FAULT;
  actuator_constant_speed_push(255u);
  TEST_ASSERT_EQUAL_INT(FAULT, s_actuator.mode);
}

TEST(actuator_mode_tests, constant_speed_push_sets_motor_direction)
{
  actuator_constant_speed_push(255u);
  TEST_ASSERT_EQUAL_INT(1u, gpio_set_pin_fake.call_count);
  TEST_ASSERT_EQUAL_CHAR(MOTOR_IN_A_PORT, gpio_set_pin_fake.arg0_val);
  TEST_ASSERT_EQUAL_CHAR(MOTOR_IN_A_PIN, gpio_set_pin_fake.arg1_val);
  TEST_ASSERT_EQUAL_INT(1u, gpio_clear_pin_fake.call_count);
  TEST_ASSERT_EQUAL_CHAR(MOTOR_IN_B_PORT, gpio_clear_pin_fake.arg0_val);
  TEST_ASSERT_EQUAL_CHAR(MOTOR_IN_B_PIN, gpio_clear_pin_fake.arg1_val);
}

TEST(actuator_mode_tests, constant_speed_push_caps_target_speed)
{
  actuator_constant_speed_push(255u);
  TEST_ASSERT_EQUAL_INT(63u, s_actuator.target_speed);
  actuator_constant_speed_push(1u);
  TEST_ASSERT_EQUAL_INT(1u, s_actuator.target_speed);
}

TEST(actuator_mode_tests, constant_speed_push_resets_service_timer)
{
  actuator_constant_speed_push(255u);
  TEST_ASSERT_EQUAL_INT(1u, timer_reset_fake.call_count);
  TEST_ASSERT_EQUAL_PTR(&s_timer, timer_reset_fake.arg0_val);
}

TEST(actuator_mode_tests, pid_control_push_stops_motor)
{
  actuator_pid_control_push(10);
  TEST_ASSERT_EQUAL_INT(1u, MOTOR_PWM_STOP_fake.call_count);
}

TEST(actuator_mode_tests, pid_control_push_doesnt_stop_on_second_call)
{
  actuator_constant_speed_push(10);
  actuator_constant_speed_push(10);
  TEST_ASSERT_EQUAL_INT(1u, MOTOR_PWM_STOP_fake.call_count);
}

TEST(actuator_mode_tests, pid_control_push_no_change_during_fault)
{
  s_actuator.mode = FAULT;
  actuator_pid_control_push(10);
  TEST_ASSERT_EQUAL_INT(FAULT, s_actuator.mode);
}

TEST(actuator_mode_tests, pid_control_push_sets_motor_direction)
{
  actuator_pid_control_push(10);
  TEST_ASSERT_EQUAL_INT(1u, gpio_set_pin_fake.call_count);
  TEST_ASSERT_EQUAL_CHAR(MOTOR_IN_A_PORT, gpio_set_pin_fake.arg0_val);
  TEST_ASSERT_EQUAL_CHAR(MOTOR_IN_A_PIN, gpio_set_pin_fake.arg1_val);
  TEST_ASSERT_EQUAL_INT(1u, gpio_clear_pin_fake.call_count);
  TEST_ASSERT_EQUAL_CHAR(MOTOR_IN_B_PORT, gpio_clear_pin_fake.arg0_val);
  TEST_ASSERT_EQUAL_CHAR(MOTOR_IN_B_PIN, gpio_clear_pin_fake.arg1_val);
}

TEST(actuator_mode_tests, pid_control_push_stops_service_timer)
{
  actuator_pid_control_push(10);
  TEST_ASSERT_EQUAL_INT(1u, timer_stop_fake.call_count);
  TEST_ASSERT_EQUAL_PTR(&s_timer, timer_stop_fake.arg0_val);
}

TEST(actuator_mode_tests, pid_control_push_set_point)
{
  actuator_pid_control_push(10);
  TEST_ASSERT_EQUAL_INT(10, s_pid.set_point);
}

/* ACTUATOR RUN TESTS */

TEST_GROUP(actuator_run_tests);

TEST_SETUP(actuator_run_tests)
{
  memset(&s_pid, 0, sizeof(s_pid));
  actuator_init(1, 1, 1);

  BOARD_MOCKS(RESET_FAKE);
  DISPATCHER_MOCKS(RESET_FAKE);
  GPIO_MOCKS(RESET_FAKE);
  TIMER_MOCKS(RESET_FAKE);
  RESET_FAKE(pid_update);
}

TEST_TEAR_DOWN(actuator_run_tests)
{}

TEST(actuator_run_tests, run_doesnt_do_anything_in_pid_mode)
{
  actuator_pid_control_push(255);
  actuator_run(TIMER_ACTUATOR_SERVICE);
  TEST_ASSERT_EQUAL_INT(0u, MOTOR_PWM_fake.call_count);
  TEST_ASSERT_EQUAL_INT(0u, s_actuator.current_speed);
}

TEST(actuator_run_tests, run_doesnt_overshoot_target_speed)
{
  const uint8_t max_change_per_run = ACCELERATION_CAP * FPGA_UPDATE_INTERVAL_MS;

  // Test for target_speed is multiple
  actuator_constant_speed_push(3u * max_change_per_run);

  actuator_run(TIMER_ACTUATOR_SERVICE);
  TEST_ASSERT_EQUAL_INT(max_change_per_run, s_actuator.current_speed);
  actuator_run(TIMER_ACTUATOR_SERVICE);
  TEST_ASSERT_EQUAL_INT(2u * max_change_per_run, s_actuator.current_speed);
  actuator_run(TIMER_ACTUATOR_SERVICE);
  TEST_ASSERT_EQUAL_INT(3u * max_change_per_run, s_actuator.current_speed);

  // Test for target_speed is prime
  actuator_constant_speed_home(23u);

  actuator_run(TIMER_ACTUATOR_SERVICE);
  TEST_ASSERT_EQUAL_INT(max_change_per_run, s_actuator.current_speed);
  actuator_run(TIMER_ACTUATOR_SERVICE);
  TEST_ASSERT_EQUAL_INT(2 * max_change_per_run, s_actuator.current_speed);
  actuator_run(TIMER_ACTUATOR_SERVICE);
  TEST_ASSERT_EQUAL_INT(23u, s_actuator.current_speed);
}

TEST(actuator_run_tests, run_sets_motor_speed)
{
  actuator_constant_speed_push(255u);
  actuator_run(TIMER_ACTUATOR_SERVICE);
  TEST_ASSERT_EQUAL_INT(1u, MOTOR_PWM_fake.call_count);

  actuator_constant_speed_home(255u);
  actuator_run(TIMER_ACTUATOR_SERVICE);
  TEST_ASSERT_EQUAL_INT(2u, MOTOR_PWM_fake.call_count);
}

TEST(actuator_run_tests, run_pid_doesnt_do_anything_in_constant_speed_mode)
{
  actuator_constant_speed_home(255u);
  actuator_run_pid(10);
  TEST_ASSERT_EQUAL_INT(0u, pid_update_fake.call_count);
  TEST_ASSERT_EQUAL_INT(0u, MOTOR_PWM_fake.call_count);
  TEST_ASSERT_EQUAL_INT(0u, s_actuator.current_speed);

  actuator_constant_speed_push(255u);
  actuator_run_pid(10);
  TEST_ASSERT_EQUAL_INT(0u, pid_update_fake.call_count);
  TEST_ASSERT_EQUAL_INT(0u, MOTOR_PWM_fake.call_count);
  TEST_ASSERT_EQUAL_INT(0u, s_actuator.current_speed);
}

TEST(actuator_run_tests, run_pid_calls_pid_update)
{
  actuator_pid_control_push(10);
  actuator_run_pid(1);
  TEST_ASSERT_EQUAL_INT(1u, pid_update_fake.call_count);
}

TEST(actuator_run_tests, run_pid_sets_motor_speed)
{
  actuator_pid_control_push(10);
  actuator_run_pid(1);
  TEST_ASSERT_EQUAL_INT(1u, MOTOR_PWM_fake.call_count);
}
