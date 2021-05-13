// Copyright (c) 2021 <OpenVent-Bristol, Donald Robson>

#include "unity.h"
#include "unity_fixture.h"

TEST_GROUP_RUNNER(messages_tests)
{
  RUN_TEST_CASE(messages_tests, fpga_watchdog_expiry_sets_system_failure_alarm);

  RUN_TEST_CASE(messages_tests, init_binds_timer_event);
  RUN_TEST_CASE(messages_tests, init_attaches_timer);

  RUN_TEST_CASE(messages_tests, watchdog_enable_resets_fpga_watchdog_timer);

  RUN_TEST_CASE(messages_tests, watchdog_disable_stops_fpga_watchdog_timer);

  RUN_TEST_CASE(messages_tests, process_fpga_to_mcu_aborts_for_invalid_crc);
  RUN_TEST_CASE(messages_tests, process_fpga_to_mcu_several_invalid_crc_sets_system_failure_alarm);
  RUN_TEST_CASE(messages_tests, process_fpga_to_mcu_resets_invalid_crc_count);
  RUN_TEST_CASE(messages_tests, process_fpga_to_mcu_resets_fpga_watchdog_timer);
  RUN_TEST_CASE(messages_tests, process_fpga_to_mcu_stores_heartbeat);
  RUN_TEST_CASE(messages_tests, process_fpga_to_mcu_signals_mode_change_event);
  RUN_TEST_CASE(messages_tests, process_fpga_to_mcu_signals_set_point_change);
  RUN_TEST_CASE(messages_tests, process_fpga_to_mcu_signals_control_variable_change);
  RUN_TEST_CASE(messages_tests, process_fpga_to_mcu_stores_sensor_readings);

  RUN_TEST_CASE(messages_tests, send_mcu_to_fpga_sets_event_mask);
  RUN_TEST_CASE(messages_tests, send_mcu_to_fpga_sets_heartbeat);
  RUN_TEST_CASE(messages_tests, send_mcu_to_fpga_copies_cached_display);
  RUN_TEST_CASE(messages_tests, send_mcu_to_fpga_sets_crc);
  RUN_TEST_CASE(messages_tests, send_mcu_to_fpga_sends_via_spi);
}

static void RunAllTests(void)
{
  RUN_TEST_GROUP(messages_tests);
}

int main(int argc, const char* argv[])
{
  return UnityMain(argc, argv, RunAllTests);
}
