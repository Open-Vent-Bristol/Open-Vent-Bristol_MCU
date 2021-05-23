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
