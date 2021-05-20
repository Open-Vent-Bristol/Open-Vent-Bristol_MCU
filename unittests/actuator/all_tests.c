// Copyright (c) 2021 <OpenVent-Bristol, Donald Robson>

#include "unity.h"
#include "unity_fixture.h"

TEST_GROUP_RUNNER(actuator_init_tests)
{
  RUN_TEST_CASE(actuator_init_tests, init_stops_motor);
  RUN_TEST_CASE(actuator_init_tests, init_configures_pid_control);
  RUN_TEST_CASE(actuator_init_tests, init_starts_service_timer);
  RUN_TEST_CASE(actuator_init_tests, init_binds_events);
  RUN_TEST_CASE(actuator_init_tests, init_clears_fault);
}

TEST_GROUP_RUNNER(actuator_mode_tests)
{
  RUN_TEST_CASE(actuator_mode_tests, stop_stops_motor);
  RUN_TEST_CASE(actuator_mode_tests, stop_stops_service_timer);
  RUN_TEST_CASE(actuator_mode_tests, stop_doesnt_clear_fault);

  RUN_TEST_CASE(actuator_mode_tests, constant_speed_home_stops_motor);
  RUN_TEST_CASE(actuator_mode_tests, constant_speed_home_doesnt_stop_on_second_call);
  RUN_TEST_CASE(actuator_mode_tests, constant_speed_home_no_change_during_fault);
  RUN_TEST_CASE(actuator_mode_tests, constant_speed_home_sets_motor_direction);
  RUN_TEST_CASE(actuator_mode_tests, constant_speed_home_sets_target_speed);
  RUN_TEST_CASE(actuator_mode_tests, constant_speed_home_resets_service_timer);

  RUN_TEST_CASE(actuator_mode_tests, constant_speed_push_stops_motor);
  RUN_TEST_CASE(actuator_mode_tests, constant_speed_push_doesnt_stop_on_second_call);
  RUN_TEST_CASE(actuator_mode_tests, constant_speed_push_no_change_during_fault);
  RUN_TEST_CASE(actuator_mode_tests, constant_speed_push_sets_motor_direction);
  RUN_TEST_CASE(actuator_mode_tests, constant_speed_push_caps_target_speed);
  RUN_TEST_CASE(actuator_mode_tests, constant_speed_push_resets_service_timer);

  RUN_TEST_CASE(actuator_mode_tests, pid_control_push_stops_motor);
  RUN_TEST_CASE(actuator_mode_tests, pid_control_push_doesnt_stop_on_second_call);
  RUN_TEST_CASE(actuator_mode_tests, pid_control_push_no_change_during_fault);
  RUN_TEST_CASE(actuator_mode_tests, pid_control_push_sets_motor_direction);
  RUN_TEST_CASE(actuator_mode_tests, pid_control_push_stops_service_timer);
  RUN_TEST_CASE(actuator_mode_tests, pid_control_push_set_point);
}

TEST_GROUP_RUNNER(actuator_run_tests)
{
  RUN_TEST_CASE(actuator_run_tests, run_doesnt_do_anything_in_pid_mode);
  RUN_TEST_CASE(actuator_run_tests, run_doesnt_overshoot_target_speed);
  RUN_TEST_CASE(actuator_run_tests, run_sets_motor_speed);
  RUN_TEST_CASE(actuator_run_tests, run_clears_event);

  RUN_TEST_CASE(actuator_run_tests, run_pid_doesnt_do_anything_in_constant_speed_mode);
  RUN_TEST_CASE(actuator_run_tests, run_pid_calls_pid_update);
  RUN_TEST_CASE(actuator_run_tests, run_pid_sets_motor_speed);
  RUN_TEST_CASE(actuator_run_tests, run_pid_clears_event);
}

static void RunAllTests(void)
{
  RUN_TEST_GROUP(actuator_init_tests);
  RUN_TEST_GROUP(actuator_mode_tests);
  RUN_TEST_GROUP(actuator_run_tests);
}

int main(int argc, const char* argv[])
{
  return UnityMain(argc, argv, RunAllTests);
}
