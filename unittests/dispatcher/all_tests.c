// Copyright (c) 2021 <OpenVent-Bristol, Donald Robson>

#include "unity.h"
#include "unity_fixture.h"

TEST_GROUP_RUNNER(dispatcher_tests)
{
  // RUN_TEST_CASE(dispatcher_tests, default_handler_logs_error);

  RUN_TEST_CASE(dispatcher_tests, signal_event_mask_sets_all_bits);
  RUN_TEST_CASE(dispatcher_tests, signal_event_mask_records_arg_for_all_set_bits);
  RUN_TEST_CASE(dispatcher_tests, signal_event_mask_disables_interrupts);

  RUN_TEST_CASE(dispatcher_tests, service_null_pointer_calls_default_handler);
  RUN_TEST_CASE(dispatcher_tests, service_doesnt_process_invalid_events);
  RUN_TEST_CASE(dispatcher_tests, service_processes_all_events);
  RUN_TEST_CASE(dispatcher_tests, service_disables_interrupts);

  RUN_TEST_CASE(dispatcher_tests, bind_each_event_in_mask_to_same_handler);
  RUN_TEST_CASE(dispatcher_tests, bind_overwrites_handlers);
  RUN_TEST_CASE(dispatcher_tests, bind_disables_interrupts);

  RUN_TEST_CASE(dispatcher_tests, bind_all_overwrites_handlers);
  RUN_TEST_CASE(dispatcher_tests, bind_all_does_nothing_if_number_binds_exceeds_number_of_events);
  RUN_TEST_CASE(dispatcher_tests, bind_all_stops_at_number_binds);
  RUN_TEST_CASE(dispatcher_tests, bind_all_disables_interrupts);
}

static void RunAllTests(void)
{
  RUN_TEST_GROUP(dispatcher_tests);
}

int main(int argc, const char* argv[])
{
  return UnityMain(argc, argv, RunAllTests);
}
