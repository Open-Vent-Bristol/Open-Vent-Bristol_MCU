// Copyright (c) 2021 <OpenVent-Bristol, Donald Robson>

#include "unity.h"
#include "unity_fixture.h"
#include "test_macros.h"
#include "misc/util.h"
#include "cooling/fan.h"
#include "board_mock.h"
#include "dispatcher_mock.h"
#include "sensor_mock.h"
#include "timer_mock.h"
#include <stdbool.h>

extern timer_t s_fan_service_timer;
extern size_t s_pressure_lookup_index;
extern uint8_t s_current_duty;

extern void fan_switch_lookup(int32_t set_pressure);
extern void fan_service(int32_t arg);

uint16_t test_temperature = 0;
uint16_t get_test_temperature(enum sensor_index sensor)
{
  return test_temperature;
}

/* FAN INIT TESTS */

TEST_GROUP(fan_init_tests);

TEST_SETUP(fan_init_tests)
{
  DISPATCHER_MOCKS(RESET_FAKE);
  TIMER_MOCKS(RESET_FAKE);
}

TEST_TEAR_DOWN(fan_init_tests)
{}

TEST(fan_init_tests, init_attaches_service_timer)
{
  fan_init();
  TEST_ASSERT_EQUAL_INT(1u, timer_attach_fake.call_count);
  TEST_ASSERT_EQUAL_PTR(&s_fan_service_timer, timer_attach_fake.arg0_val);
}

TEST(fan_init_tests, init_binds_events)
{
  fan_init();
  TEST_ASSERT_EQUAL_INT(2u, dispatcher_bind_fake.call_count);
  TEST_ASSERT_VALUE_IN_ARRAY(1u << EV_FAN_SERVICE, dispatcher_bind_fake.arg0_history);
  TEST_ASSERT_VALUE_IN_ARRAY(1u << EV_FAN_PRESSURE_UPDATE, dispatcher_bind_fake.arg0_history);
}

/* FAN SWITCH LOOKUP TESTS */

TEST_GROUP(fan_switch_lookup_tests);

TEST_SETUP(fan_switch_lookup_tests)
{
  s_pressure_lookup_index = 0u;
  s_current_duty = 0u;
  BOARD_MOCKS(RESET_FAKE);
  DISPATCHER_MOCKS(RESET_FAKE);
}

TEST_TEAR_DOWN(fan_switch_lookup_tests)
{}

TEST(fan_switch_lookup_tests, beyond_limits)
{
  fan_switch_lookup(29);
  TEST_ASSERT_EQUAL_INT(0u, s_pressure_lookup_index);

  fan_switch_lookup(46);
  TEST_ASSERT_EQUAL_INT(4u, s_pressure_lookup_index);

  fan_switch_lookup(INT16_MIN);
  TEST_ASSERT_EQUAL_INT(0u, s_pressure_lookup_index);

  fan_switch_lookup(INT16_MAX);
  TEST_ASSERT_EQUAL_INT(4u, s_pressure_lookup_index);
}

TEST(fan_switch_lookup_tests, within_limits)
{
  fan_switch_lookup(30);
  TEST_ASSERT_EQUAL_INT(1u, s_pressure_lookup_index);

  fan_switch_lookup(34);
  TEST_ASSERT_EQUAL_INT(1u, s_pressure_lookup_index);

  fan_switch_lookup(35);
  TEST_ASSERT_EQUAL_INT(2u, s_pressure_lookup_index);

  fan_switch_lookup(39);
  TEST_ASSERT_EQUAL_INT(2u, s_pressure_lookup_index);

  fan_switch_lookup(40);
  TEST_ASSERT_EQUAL_INT(3u, s_pressure_lookup_index);

  fan_switch_lookup(44);
  TEST_ASSERT_EQUAL_INT(3u, s_pressure_lookup_index);

  fan_switch_lookup(45);
  TEST_ASSERT_EQUAL_INT(4u, s_pressure_lookup_index);
}

TEST(fan_switch_lookup_tests, sets_pwm_when_switch_lookup)
{
  fan_switch_lookup(50);
  TEST_ASSERT_EQUAL_INT(1, FAN_PWM_fake.call_count);
  TEST_ASSERT_NOT_EQUAL_INT(0u, FAN_PWM_fake.arg0_val);
  TEST_ASSERT_EQUAL_INT(1u, FAN_PWM_START_fake.call_count);

  fan_switch_lookup(10);
  TEST_ASSERT_EQUAL_INT(1, FAN_PWM_STOP_fake.call_count);
}

TEST(fan_switch_lookup_tests, clears_event)
{
  fan_switch_lookup(0);
  TEST_ASSERT_EQUAL_INT(1u, dispatcher_clear_event_mask_fake.call_count);
  TEST_ASSERT_EQUAL_INT(1u << EV_FAN_PRESSURE_UPDATE, dispatcher_clear_event_mask_fake.arg0_val);
}

/* FAN SERVICE TESTS */

TEST_GROUP(fan_service_tests);

TEST_SETUP(fan_service_tests)
{
  s_pressure_lookup_index = 0u;
  s_current_duty = 0u;
  BOARD_MOCKS(RESET_FAKE);
  DISPATCHER_MOCKS(RESET_FAKE);
  SENSOR_MOCKS(RESET_FAKE);
  TIMER_MOCKS(RESET_FAKE);

  test_temperature = 0;
  sensor_get_reading_fake.custom_fake = get_test_temperature;
}

TEST_TEAR_DOWN(fan_service_tests)
{}

TEST(fan_service_tests, duty_calculation)
{
  s_pressure_lookup_index = 0u;
  test_temperature = 0u;
  fan_service(0);
  TEST_ASSERT_EQUAL_INT(0u, FAN_PWM_fake.call_count);

  test_temperature = 29u;
  fan_service(0);
  TEST_ASSERT_EQUAL_INT(0u, FAN_PWM_fake.call_count);

  test_temperature = 30u;
  fan_service(0);
  TEST_ASSERT_EQUAL_INT(0u, FAN_PWM_fake.call_count);

  test_temperature = 39u;
  fan_service(0);
  TEST_ASSERT_EQUAL_INT(0u, FAN_PWM_fake.call_count);

  test_temperature = 40u;
  fan_service(0);
  TEST_ASSERT_EQUAL_INT(0u, FAN_PWM_fake.call_count);

  test_temperature = UINT16_MAX;
  fan_service(0);
  TEST_ASSERT_EQUAL_INT(0u, FAN_PWM_fake.call_count);

  s_pressure_lookup_index = 1u;
  RESET_FAKE(FAN_PWM);

  test_temperature = 0u;
  fan_service(0);
  TEST_ASSERT_EQUAL_INT(0u, FAN_PWM_fake.call_count);

  test_temperature = 29u;
  fan_service(0);
  TEST_ASSERT_EQUAL_INT(0u, FAN_PWM_fake.call_count);

  test_temperature = 30u;
  fan_service(0);
  TEST_ASSERT_EQUAL_INT(0u, FAN_PWM_fake.call_count);

  test_temperature = 39u;
  fan_service(0);
  TEST_ASSERT_EQUAL_INT(0u, FAN_PWM_fake.call_count);

  test_temperature = 40u;
  fan_service(0);
  TEST_ASSERT_EQUAL_INT(PERCENT_TO_UINT8(25u), FAN_PWM_fake.arg0_val);

  test_temperature = UINT16_MAX;
  fan_service(0);
  TEST_ASSERT_EQUAL_INT(PERCENT_TO_UINT8(25u), FAN_PWM_fake.arg0_val);

  s_pressure_lookup_index = 2u;
  RESET_FAKE(FAN_PWM);

  test_temperature = 0u;
  fan_service(0);
  TEST_ASSERT_EQUAL_INT(0u, FAN_PWM_fake.call_count);

  test_temperature = 29u;
  fan_service(0);
  TEST_ASSERT_EQUAL_INT(0u, FAN_PWM_fake.call_count);

  test_temperature = 30u;
  fan_service(0);
  TEST_ASSERT_EQUAL_INT(0u, FAN_PWM_fake.call_count);

  test_temperature = 39u;
  fan_service(0);
  TEST_ASSERT_EQUAL_INT(0u, FAN_PWM_fake.call_count);

  test_temperature = 40u;
  fan_service(0);
  TEST_ASSERT_EQUAL_INT(PERCENT_TO_UINT8(50u), FAN_PWM_fake.arg0_val);

  test_temperature = UINT16_MAX;
  fan_service(0);
  TEST_ASSERT_EQUAL_INT(PERCENT_TO_UINT8(50u), FAN_PWM_fake.arg0_val);

  s_pressure_lookup_index = 3u;
  RESET_FAKE(FAN_PWM);

  test_temperature = 0u;
  fan_service(0);
  TEST_ASSERT_EQUAL_INT(PERCENT_TO_UINT8(10u), FAN_PWM_fake.arg0_val);

  test_temperature = 29u;
  fan_service(0);
  TEST_ASSERT_EQUAL_INT(PERCENT_TO_UINT8(10u), FAN_PWM_fake.arg0_val);

  test_temperature = 30u;
  fan_service(0);
  TEST_ASSERT_EQUAL_INT(PERCENT_TO_UINT8(25u), FAN_PWM_fake.arg0_val);

  test_temperature = 39u;
  fan_service(0);
  TEST_ASSERT_EQUAL_INT(PERCENT_TO_UINT8(25u), FAN_PWM_fake.arg0_val);

  test_temperature = 40u;
  fan_service(0);
  TEST_ASSERT_EQUAL_INT(PERCENT_TO_UINT8(75u), FAN_PWM_fake.arg0_val);

  test_temperature = UINT16_MAX;
  fan_service(0);
  TEST_ASSERT_EQUAL_INT(PERCENT_TO_UINT8(75u), FAN_PWM_fake.arg0_val);

  s_pressure_lookup_index = 4u;
  RESET_FAKE(FAN_PWM);

  test_temperature = 0u;
  fan_service(0);
  TEST_ASSERT_EQUAL_INT(PERCENT_TO_UINT8(30u), FAN_PWM_fake.arg0_val);

  test_temperature = 29u;
  fan_service(0);
  TEST_ASSERT_EQUAL_INT(PERCENT_TO_UINT8(30u), FAN_PWM_fake.arg0_val);

  test_temperature = 30u;
  fan_service(0);
  TEST_ASSERT_EQUAL_INT(PERCENT_TO_UINT8(75u), FAN_PWM_fake.arg0_val);

  test_temperature = 39u;
  fan_service(0);
  TEST_ASSERT_EQUAL_INT(PERCENT_TO_UINT8(75u), FAN_PWM_fake.arg0_val);

  test_temperature = 40u;
  fan_service(0);
  TEST_ASSERT_EQUAL_INT(PERCENT_TO_UINT8(100u), FAN_PWM_fake.arg0_val);

  test_temperature = UINT16_MAX;
  fan_service(0);
  TEST_ASSERT_EQUAL_INT(PERCENT_TO_UINT8(100u), FAN_PWM_fake.arg0_val);
}

TEST(fan_service_tests, sets_pwm_on_off)
{
  s_pressure_lookup_index = 4u;
  test_temperature = 40u;
  fan_service(0);
  TEST_ASSERT_EQUAL_INT(1, FAN_PWM_fake.call_count);
  TEST_ASSERT_EQUAL_INT(1, FAN_PWM_START_fake.call_count);

  s_pressure_lookup_index = 0u;
  test_temperature = 0u;
  fan_service(0);
  TEST_ASSERT_EQUAL_INT(1, FAN_PWM_STOP_fake.call_count);
}

TEST(fan_service_tests, clears_event)
{
  fan_service(0);
  TEST_ASSERT_EQUAL_INT(1u, dispatcher_clear_event_mask_fake.call_count);
  TEST_ASSERT_EQUAL_INT(1u << EV_FAN_SERVICE, dispatcher_clear_event_mask_fake.arg0_val);
}
