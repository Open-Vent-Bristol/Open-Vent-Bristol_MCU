// Copyright (c) 2021 <OpenVent-Bristol, Donald Robson>

#include "unity.h"
#include "unity_fixture.h"

TEST_GROUP_RUNNER(hardware_event_tests)
{
  RUN_TEST_CASE(hardware_event_tests, power_button_and_switches_default_on);

  RUN_TEST_CASE(hardware_event_tests, init_attaches_debounce_timers);
  RUN_TEST_CASE(hardware_event_tests, init_binds_events);

  RUN_TEST_CASE(hardware_event_tests, debounce_end_enables_power_button);
  RUN_TEST_CASE(hardware_event_tests, debounce_end_enables_limit_switches);

  RUN_TEST_CASE(hardware_event_tests, power_button_isr_debounced);

  RUN_TEST_CASE(hardware_event_tests, limit_switches_isr_debounced);
  RUN_TEST_CASE(hardware_event_tests, limit_switches_isr_stops_actuator);

  RUN_TEST_CASE(hardware_event_tests, fpga_ready_isr_signals_event);
}

static void RunAllTests(void)
{
  RUN_TEST_GROUP(hardware_event_tests);
}

int main(int argc, const char* argv[])
{
  return UnityMain(argc, argv, RunAllTests);
}
