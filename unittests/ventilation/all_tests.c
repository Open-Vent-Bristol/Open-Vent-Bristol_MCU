// Copyright (c) 2021 <OpenVent-Bristol, Donald Robson>

#include "unity.h"
#include "unity_fixture.h"

TEST_GROUP_RUNNER(ventilation_tests)
{
  RUN_TEST_CASE(ventilation_tests, init_binds_event);
  RUN_TEST_CASE(ventilation_tests, init_calls_state_machine_init);

  RUN_TEST_CASE(ventilation_tests, set_target_mode_clears_event);
  RUN_TEST_CASE(ventilation_tests, set_target_mode_sets_target_mode);
  RUN_TEST_CASE(ventilation_tests, set_target_mode_doesnt_set_current_mode);

  RUN_TEST_CASE(ventilation_tests, set_to_target_mode_does_nothing_if_no_change);
  RUN_TEST_CASE(ventilation_tests, set_to_target_mode_sets_target_mode_if_valid);
  RUN_TEST_CASE(ventilation_tests, set_to_target_mode_resets_target_mode_if_invalid);
  RUN_TEST_CASE(ventilation_tests, set_to_target_mode_changes_transition_list);
}

static void RunAllTests(void)
{
  RUN_TEST_GROUP(ventilation_tests);
}

int main(int argc, const char* argv[])
{
  return UnityMain(argc, argv, RunAllTests);
}
