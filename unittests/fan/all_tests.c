// Copyright (c) 2021 <OpenVent-Bristol, Donald Robson>

#include "unity.h"
#include "unity_fixture.h"

TEST_GROUP_RUNNER(fan_init_tests)
{
  RUN_TEST_CASE(fan_init_tests, init_attaches_service_timer);
  RUN_TEST_CASE(fan_init_tests, init_binds_events);
}

TEST_GROUP_RUNNER(fan_switch_lookup_tests)
{
  RUN_TEST_CASE(fan_switch_lookup_tests, beyond_limits);
  RUN_TEST_CASE(fan_switch_lookup_tests, within_limits);
  RUN_TEST_CASE(fan_switch_lookup_tests, sets_pwm_when_switch_lookup);
  RUN_TEST_CASE(fan_switch_lookup_tests, clears_event);
}

TEST_GROUP_RUNNER(fan_service_tests)
{
  RUN_TEST_CASE(fan_service_tests, duty_calculation);
  RUN_TEST_CASE(fan_service_tests, sets_pwm_on_off);
  RUN_TEST_CASE(fan_service_tests, clears_event);
}

static void RunAllTests(void)
{
  RUN_TEST_GROUP(fan_init_tests);
  RUN_TEST_GROUP(fan_switch_lookup_tests);
  RUN_TEST_GROUP(fan_service_tests);
}

int main(int argc, const char* argv[])
{
  return UnityMain(argc, argv, RunAllTests);
}
