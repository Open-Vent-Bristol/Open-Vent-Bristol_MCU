// Copyright (c) 2021 <OpenVent-Bristol, Donald Robson>

#include "unity.h"
#include "unity_fixture.h"

TEST_GROUP_RUNNER(alarm_init_tests)
{
  RUN_TEST_CASE(alarm_init_tests, init_attaches_service_timer);
  RUN_TEST_CASE(alarm_init_tests, init_binds_event);
}

TEST_GROUP_RUNNER(alarm_mode_tests)
{
  RUN_TEST_CASE(alarm_mode_tests, mode_ALARM_OVERRIDE_OFF_is_not_sticky);
  RUN_TEST_CASE(alarm_mode_tests, mode_ALARM_OVERRIDE_OFF_stops_service_timer);
  RUN_TEST_CASE(alarm_mode_tests, mode_ALARM_OVERRIDE_OFF_stops_PWM);

  RUN_TEST_CASE(alarm_mode_tests, mode_ALARM_OVERRIDE_ON_is_not_sticky);
  RUN_TEST_CASE(alarm_mode_tests, mode_ALARM_OVERRIDE_ON_resets_service_timer);
  RUN_TEST_CASE(alarm_mode_tests, mode_ALARM_OVERRIDE_ON_starts_pwm);

  RUN_TEST_CASE(alarm_mode_tests, mode_ALARM_SYSTEM_FAILURE_is_sticky);
  RUN_TEST_CASE(alarm_mode_tests, mode_ALARM_SYSTEM_FAILURE_stops_service_timer);
  RUN_TEST_CASE(alarm_mode_tests, mode_ALARM_SYSTEM_FAILURE_starts_pwm);
}

TEST_GROUP_RUNNER(alarm_run_tests)
{
  RUN_TEST_CASE(alarm_run_tests, intermittance_cb_only_runs_in_OVERRIDE_ON_mode);
  RUN_TEST_CASE(alarm_run_tests, intermittance_cb_toggles_pwm);
}

static void RunAllTests(void)
{
  RUN_TEST_GROUP(alarm_init_tests);
  RUN_TEST_GROUP(alarm_mode_tests);
  RUN_TEST_GROUP(alarm_run_tests);
}

int main(int argc, const char* argv[])
{
  return UnityMain(argc, argv, RunAllTests);
}
