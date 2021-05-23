// Copyright (c) 2021 <OpenVent-Bristol, Donald Robson>

#include "unity.h"
#include "unity_fixture.h"
#include "fpga/display_controller.h"
#include "fpga/private/display_priv.h"
#include "display_format_mock.h"
#include "dispatcher_mock.h"
#include "sensor_mock.h"
#include <string.h>

extern enum display_override_index s_display_override_index;
extern enum display_override_line_2_index s_display_override_line_2_index;

extern void display_controller_update(int32_t arg);

TEST_GROUP(display_controller_test);

TEST_SETUP(display_controller_test)
{
  s_display_override_index = DISPLAY_OVERRIDE_NONE;
  s_display_override_line_2_index = DISPLAY_OVERRIDE_LINE2_NONE;
  DISPATCHER_MOCKS(RESET_FAKE);
  DISPLAY_FORMAT_MOCKS(RESET_FAKE);
  SENSOR_MOCKS(RESET_FAKE);
}

TEST_TEAR_DOWN(display_controller_test)
{}

TEST(display_controller_test, init_binds_event)
{
  display_controller_init();

  TEST_ASSERT_EQUAL_INT(1, dispatcher_bind_fake.call_count);
  TEST_ASSERT_EQUAL_INT(1u << EV_DO_UPDATE_DISPLAY, dispatcher_bind_fake.arg0_val);
}

TEST(display_controller_test, set_override_invalid_arg)
{
  display_controller_set_override(UINT16_MAX);

  TEST_ASSERT_EQUAL_INT(DISPLAY_OVERRIDE_NONE, s_display_override_index);
}

TEST(display_controller_test, set_override_valid_arg)
{
  display_controller_set_override(DISPLAY_OVERRIDE_LINE1_PLEASE_WAIT);

  TEST_ASSERT_EQUAL_INT(DISPLAY_OVERRIDE_LINE1_PLEASE_WAIT, s_display_override_index);
}

TEST(display_controller_test, set_line_2_override)
{
  display_controller_set_line_2_override(DISPLAY_OVERRIDE_LINE2_SUCCESS);

  TEST_ASSERT_EQUAL_INT(DISPLAY_OVERRIDE_LINE2_SUCCESS, s_display_override_line_2_index);
}

TEST(display_controller_test, update_clears_event)
{
  display_controller_update(0);

  TEST_ASSERT_EQUAL_INT(1, dispatcher_clear_event_mask_fake.call_count);
  TEST_ASSERT_EQUAL_INT(1u << EV_DO_UPDATE_DISPLAY, dispatcher_clear_event_mask_fake.arg0_val);
}

TEST(display_controller_test, update_no_override)
{
  display_controller_update(0);

  TEST_ASSERT_EQUAL_INT(7, sensor_get_reading_fake.call_count);

  TEST_ASSERT_EQUAL_INT(1, display_format_tidal_volume_fake.call_count);
  TEST_ASSERT_EQUAL_INT(1, display_format_peak_flow_fake.call_count);
  TEST_ASSERT_EQUAL_INT(1, display_format_respiration_rate_fake.call_count);
  TEST_ASSERT_EQUAL_INT(1, display_format_percent_o2_fake.call_count);
  TEST_ASSERT_EQUAL_INT(1, display_format_pressure_bar_fake.call_count);
  TEST_ASSERT_EQUAL_INT(1, display_format_battery_gauge_fake.call_count);
}

TEST(display_controller_test, update_no_override_battery_fault)
{
  sensor_get_reading_fake.return_val = -1;

  display_controller_update(0);

  TEST_ASSERT_EQUAL_INT(7, sensor_get_reading_fake.call_count);
  TEST_ASSERT_EQUAL_INT(1, display_format_battery_fault_fake.call_count);
}

TEST(display_controller_test, update_override_line_1_only)
{
  s_display_override_index = DISPLAY_OVERRIDE_LINE1_PLEASE_WAIT;

  display_controller_update(0);

  TEST_ASSERT_EQUAL_INT(0, sensor_get_reading_fake.call_count);
  TEST_ASSERT_EQUAL_INT(1, display_format_string_fake.call_count);
}

TEST(display_controller_test, update_override_both_lines)
{
  s_display_override_index = DISPLAY_OVERRIDE_HOLD_MUTE_SEL;

  display_controller_update(0);

  TEST_ASSERT_EQUAL_INT(0, sensor_get_reading_fake.call_count);
  TEST_ASSERT_EQUAL_INT(1, display_format_string_fake.call_count);

  RESET_FAKE(display_format_string);

  s_display_override_index = DISPLAY_OVERRIDE_LINE1_FULL_CALIB;
  s_display_override_line_2_index = DISPLAY_OVERRIDE_LINE2_FAILED;

  display_controller_update(0);

  TEST_ASSERT_EQUAL_INT(0, sensor_get_reading_fake.call_count);
  TEST_ASSERT_EQUAL_INT(2, display_format_string_fake.call_count);
}

TEST(display_controller_test, update_override_line_1_and_progress_bar)
{
  s_display_override_index = DISPLAY_OVERRIDE_LINE1_FULL_CALIB;
  s_display_override_line_2_index = DISPLAY_OVERRIDE_LINE2_FULL;

  display_controller_update(0);

  TEST_ASSERT_EQUAL_INT(0, sensor_get_reading_fake.call_count);
  TEST_ASSERT_EQUAL_INT(1, display_format_string_fake.call_count);
  TEST_ASSERT_EQUAL_INT(1, display_format_progress_bar_fake.call_count);
}
