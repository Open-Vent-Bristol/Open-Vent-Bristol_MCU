// Copyright (c) 2021 <OpenVent-Bristol, Donald Robson>

#include "unity.h"
#include "unity_fixture.h"

TEST_GROUP_RUNNER(timer_tests)
{
  RUN_TEST_CASE(timer_tests, tick_all_ignores_null_pointers);
  RUN_TEST_CASE(timer_tests, tick_all_processes_all_timers);

  RUN_TEST_CASE(timer_tests, attach_pointer);
  RUN_TEST_CASE(timer_tests, attach_zero_ticks_does_not_signal_events);
  RUN_TEST_CASE(timer_tests, attach_negative_ticks_does_not_signal_events);

  RUN_TEST_CASE(timer_tests, reset_null_pointer);
  RUN_TEST_CASE(timer_tests, reset_sets_remaining_ticks);
  RUN_TEST_CASE(timer_tests, reset_zero_ticks_does_not_signal_events);
  RUN_TEST_CASE(timer_tests, reset_negative_ticks_does_not_signal_events);

  RUN_TEST_CASE(timer_tests, expired_timer_sets_all_events);
  RUN_TEST_CASE(timer_tests, expired_timer_sends_unique_id_arg);

  RUN_TEST_CASE(timer_tests, expired_one_shot_timer_does_not_restart);
  RUN_TEST_CASE(timer_tests, expired_one_shot_timer_does_not_overflow_back_to_int32_max);
  RUN_TEST_CASE(timer_tests, expired_continuous_timer_restarts_automatically);
}

static void RunAllTests(void)
{
  RUN_TEST_GROUP(timer_tests);
}

int main(int argc, const char* argv[])
{
  return UnityMain(argc, argv, RunAllTests);
}
