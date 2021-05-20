// Copyright (c) 2021 <OpenVent-Bristol, Donald Robson>

#include "unity.h"
#include "unity_fixture.h"
#include "test_macros.h"
#include "alarm/alarm.h"
#include "board_mock.h"
#include "dispatcher_mock.h"
#include "timer_mock.h"
#include <stdbool.h>

extern timer_t s_alarm_timer;
extern enum alarm_mode s_current_mode;

extern void alarm_intermittance_cb(int32_t arg);

/* ALARM INIT TESTS */

TEST_GROUP(alarm_init_tests);

TEST_SETUP(alarm_init_tests)
{
  s_current_mode = ALARM_OVERRIDE_OFF;
  DISPATCHER_MOCKS(RESET_FAKE);
  TIMER_MOCKS(RESET_FAKE);
}

TEST_TEAR_DOWN(alarm_init_tests)
{}

TEST(alarm_init_tests, init_attaches_service_timer)
{
  alarm_init();
  TEST_ASSERT_EQUAL_INT(1u, timer_attach_fake.call_count);
  TEST_ASSERT_EQUAL_PTR(&s_alarm_timer, timer_attach_fake.arg0_val);
}

TEST(alarm_init_tests, init_binds_event)
{
  alarm_init();
  TEST_ASSERT_EQUAL_INT(1u, dispatcher_bind_fake.call_count);
  TEST_ASSERT_EQUAL_INT(1u << EV_ALARM_SERVICE, dispatcher_bind_fake.arg0_val);
}

/* ALARM MODE TESTS */

TEST_GROUP(alarm_mode_tests);

TEST_SETUP(alarm_mode_tests)
{
  s_current_mode = ALARM_OVERRIDE_OFF;
  s_alarm_timer.initial_ticks = -1;
  BOARD_MOCKS(RESET_FAKE);
  TIMER_MOCKS(RESET_FAKE);
}

TEST_TEAR_DOWN(alarm_mode_tests)
{}

TEST(alarm_mode_tests, mode_ALARM_OVERRIDE_OFF_is_not_sticky)
{
  alarm_mode(ALARM_OVERRIDE_OFF);

  alarm_mode(ALARM_OVERRIDE_ON);
  TEST_ASSERT_EQUAL_INT(ALARM_OVERRIDE_ON, s_current_mode);

  alarm_mode(ALARM_OVERRIDE_OFF);
  TEST_ASSERT_EQUAL_INT(ALARM_OVERRIDE_OFF, s_current_mode);

  alarm_mode(ALARM_SYSTEM_FAILURE);
  TEST_ASSERT_EQUAL_INT(ALARM_SYSTEM_FAILURE, s_current_mode);
}

TEST(alarm_mode_tests, mode_ALARM_OVERRIDE_OFF_stops_service_timer)
{
  alarm_mode(ALARM_OVERRIDE_OFF);
  TEST_ASSERT_EQUAL_INT(1, timer_stop_fake.call_count);
  TEST_ASSERT_EQUAL_PTR(&s_alarm_timer, timer_stop_fake.arg0_val);
}

TEST(alarm_mode_tests, mode_ALARM_OVERRIDE_OFF_stops_PWM)
{
  alarm_mode(ALARM_OVERRIDE_OFF);
  TEST_ASSERT_EQUAL_INT(1, BUZZ_PWM_STOP_fake.call_count);
}

TEST(alarm_mode_tests, mode_ALARM_OVERRIDE_ON_is_not_sticky)
{
  alarm_mode(ALARM_OVERRIDE_ON);

  alarm_mode(ALARM_OVERRIDE_OFF);
  TEST_ASSERT_EQUAL_INT(ALARM_OVERRIDE_OFF, s_current_mode);

  alarm_mode(ALARM_OVERRIDE_ON);
  TEST_ASSERT_EQUAL_INT(ALARM_OVERRIDE_ON, s_current_mode);

  alarm_mode(ALARM_SYSTEM_FAILURE);
  TEST_ASSERT_EQUAL_INT(ALARM_SYSTEM_FAILURE, s_current_mode);
}

TEST(alarm_mode_tests, mode_ALARM_OVERRIDE_ON_resets_service_timer)
{
  alarm_mode(ALARM_OVERRIDE_ON);
  TEST_ASSERT_EQUAL_INT(1, timer_reset_fake.call_count);
  TEST_ASSERT_EQUAL_PTR(&s_alarm_timer, timer_reset_fake.arg0_val);
}

TEST(alarm_mode_tests, mode_ALARM_OVERRIDE_ON_starts_pwm)
{
  alarm_mode(ALARM_OVERRIDE_ON);
  TEST_ASSERT_EQUAL_INT(1, BUZZ_PWM_START_fake.call_count);
}

TEST(alarm_mode_tests, mode_ALARM_SYSTEM_FAILURE_is_sticky)
{
  alarm_mode(ALARM_SYSTEM_FAILURE);

  alarm_mode(ALARM_OVERRIDE_ON);
  TEST_ASSERT_EQUAL_INT(ALARM_SYSTEM_FAILURE, s_current_mode);

  alarm_mode(ALARM_OVERRIDE_OFF);
  TEST_ASSERT_EQUAL_INT(ALARM_SYSTEM_FAILURE, s_current_mode);
}

TEST(alarm_mode_tests, mode_ALARM_SYSTEM_FAILURE_stops_service_timer)
{
  alarm_mode(ALARM_SYSTEM_FAILURE);
  TEST_ASSERT_EQUAL_INT(1, timer_stop_fake.call_count);
  TEST_ASSERT_EQUAL_PTR(&s_alarm_timer, timer_stop_fake.arg0_val);
}

TEST(alarm_mode_tests, mode_ALARM_SYSTEM_FAILURE_starts_pwm)
{
  alarm_mode(ALARM_SYSTEM_FAILURE);
  TEST_ASSERT_EQUAL_INT(1, BUZZ_PWM_START_fake.call_count);
}

/* ALARM RUN TESTS */

TEST_GROUP(alarm_run_tests);

TEST_SETUP(alarm_run_tests)
{
  BOARD_MOCKS(RESET_FAKE);
  DISPATCHER_MOCKS(RESET_FAKE);
  TIMER_MOCKS(RESET_FAKE);
}

TEST_TEAR_DOWN(alarm_run_tests)
{}

TEST(alarm_run_tests, intermittance_cb_only_runs_in_OVERRIDE_ON_mode)
{
  s_current_mode = ALARM_OVERRIDE_OFF;

  alarm_intermittance_cb(0);
  TEST_ASSERT_EQUAL_INT(0, BUZZ_PWM_START_fake.call_count);
  TEST_ASSERT_EQUAL_INT(0, BUZZ_PWM_STOP_fake.call_count);

  s_current_mode = ALARM_SYSTEM_FAILURE;

  alarm_intermittance_cb(0);
  TEST_ASSERT_EQUAL_INT(0, BUZZ_PWM_START_fake.call_count);
  TEST_ASSERT_EQUAL_INT(0, BUZZ_PWM_STOP_fake.call_count);

  s_current_mode = ALARM_OVERRIDE_ON;

  alarm_intermittance_cb(0);
  TEST_ASSERT_NOT_EQUAL_INT(BUZZ_PWM_START_fake.call_count, BUZZ_PWM_STOP_fake.call_count);
}

TEST(alarm_run_tests, intermittance_cb_toggles_pwm)
{
  s_current_mode = ALARM_OVERRIDE_ON;

  alarm_intermittance_cb(0);
  TEST_ASSERT_NOT_EQUAL_INT(BUZZ_PWM_START_fake.call_count, BUZZ_PWM_STOP_fake.call_count);

  alarm_intermittance_cb(0);
  TEST_ASSERT_EQUAL_INT(BUZZ_PWM_START_fake.call_count, BUZZ_PWM_STOP_fake.call_count);

  alarm_intermittance_cb(0);
  TEST_ASSERT_NOT_EQUAL_INT(BUZZ_PWM_START_fake.call_count, BUZZ_PWM_STOP_fake.call_count);

  TEST_ASSERT_NOT_EQUAL_INT(0, BUZZ_PWM_START_fake.call_count);
  TEST_ASSERT_NOT_EQUAL_INT(0, BUZZ_PWM_STOP_fake.call_count);
}

TEST(alarm_run_tests, intermittance_cb_clears_event)
{
  alarm_intermittance_cb(0);
  TEST_ASSERT_EQUAL_INT(1u, dispatcher_clear_event_mask_fake.call_count);
  TEST_ASSERT_EQUAL_INT(1u << EV_ALARM_SERVICE, dispatcher_clear_event_mask_fake.arg0_val);
}
