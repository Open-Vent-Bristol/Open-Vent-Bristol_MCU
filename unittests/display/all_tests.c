// Copyright (c) 2021 <OpenVent-Bristol, Donald Robson>

#include "unity.h"
#include "unity_fixture.h"

TEST_GROUP_RUNNER(display_tests)
{
  RUN_TEST_CASE(display_tests, has_changed_true);
  RUN_TEST_CASE(display_tests, has_changed_false);

  RUN_TEST_CASE(display_tests, get_copies_custom_chars);
  RUN_TEST_CASE(display_tests, get_with_null_pointer);

  RUN_TEST_CASE(display_tests, make_custom_char_pressure_peak_marker);
  RUN_TEST_CASE(display_tests, make_custom_char_pressure_bar_edge);
  RUN_TEST_CASE(display_tests, make_custom_char_battery_empty);
  RUN_TEST_CASE(display_tests, make_custom_char_battery);
  RUN_TEST_CASE(display_tests, make_custom_char_battery_fault);

  RUN_TEST_CASE(display_tests, format_tidal_volume_text);
  RUN_TEST_CASE(display_tests, format_tidal_volume_out_of_bounds);
  RUN_TEST_CASE(display_tests, format_tidal_volume_text_no_change);

  RUN_TEST_CASE(display_tests, format_peak_flow_text);
  RUN_TEST_CASE(display_tests, format_peak_flow_out_of_bounds);
  RUN_TEST_CASE(display_tests, format_peak_flow_text_no_change);

  RUN_TEST_CASE(display_tests, format_respiration_rate_text);
  RUN_TEST_CASE(display_tests, format_respiration_rate_out_of_bounds);
  RUN_TEST_CASE(display_tests, format_respiration_rate_text_no_change);

  RUN_TEST_CASE(display_tests, format_percent_o2_text);
  RUN_TEST_CASE(display_tests, format_percent_o2_out_of_bounds);
  RUN_TEST_CASE(display_tests, format_percent_o2_text_no_change);

  RUN_TEST_CASE(display_tests, format_pressure_text);
  RUN_TEST_CASE(display_tests, format_pressure_text_out_of_bounds);
  RUN_TEST_CASE(display_tests, format_pressure_text_no_change);

  RUN_TEST_CASE(display_tests, format_pressure_bar_rising);
  RUN_TEST_CASE(display_tests, format_pressure_bar_falling);
  RUN_TEST_CASE(display_tests, format_pressure_bar_edge_chars);
  RUN_TEST_CASE(display_tests, format_pressure_bar_overlapping_edge_chars);
  RUN_TEST_CASE(display_tests, format_pressure_bar_out_of_bounds);

  RUN_TEST_CASE(display_tests, format_battery_gauge_char);
  RUN_TEST_CASE(display_tests, format_battery_gauge_char_no_change);

  RUN_TEST_CASE(display_tests, format_battery_fault_char);
  RUN_TEST_CASE(display_tests, format_battery_fault_char_no_change);

  RUN_TEST_CASE(display_tests, format_progress_bar_steps);
  RUN_TEST_CASE(display_tests, format_progress_bar_out_of_bounds);

  RUN_TEST_CASE(display_tests, string_null_pointer);
  RUN_TEST_CASE(display_tests, string_terminates_at_length_32);
  RUN_TEST_CASE(display_tests, string_null_terminated_first_line);
  RUN_TEST_CASE(display_tests, string_opening_new_line_second_line_only);
  RUN_TEST_CASE(display_tests, string_ignores_new_line_at_index_16);
  RUN_TEST_CASE(display_tests, string_treats_new_line_on_second_line_as_termination);
}

static void RunAllTests(void)
{
  RUN_TEST_GROUP(display_tests);
}

int main(int argc, const char* argv[])
{
  return UnityMain(argc, argv, RunAllTests);
}
