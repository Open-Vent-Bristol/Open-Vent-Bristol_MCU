// Copyright (c) 2021 <OpenVent-Bristol, Donald Robson>

#include "unity.h"
#include "unity_fixture.h"
#include "fpga/display.h"
#include "fpga/private/display_priv.h"
#include <string.h>

extern uint64_t s_custom_chars[CUSTOM_CHAR_COUNT];
extern char s_display[DISP_LEN];
extern bool s_display_changed;

extern void make_custom_char(enum display_char char_to_create);

TEST_GROUP(display_tests);

TEST_SETUP(display_tests)
{
  memset(s_custom_chars, 0u, sizeof(s_custom_chars));
  memset(s_display, ' ', sizeof(s_display));
  s_display_changed = false;
}

TEST_TEAR_DOWN(display_tests)
{}

TEST(display_tests, has_changed_true)
{
  s_display_changed = true;
  TEST_ASSERT_TRUE(display_has_changed());
}

TEST(display_tests, has_changed_false)
{
  TEST_ASSERT_FALSE(display_has_changed());
}

TEST(display_tests, get_copies_custom_chars)
{
  message_mcu_to_fpga_t message;
  memset(&message, 0xFF, sizeof(message));

  s_display_changed = true;
  display_get(&message);

  TEST_ASSERT_EQUAL_UINT64(0u, message.battery_indicator);
  TEST_ASSERT_EQUAL_UINT64(0u, message.pressure_bar_edge);
  TEST_ASSERT_EQUAL_UINT64(0u, message.pressure_bar_peak);

  TEST_ASSERT_FALSE(s_display_changed);
}

TEST(display_tests, get_with_null_pointer)
{
  s_display_changed = true;
  display_get((message_mcu_to_fpga_t*)0u);

  TEST_ASSERT_TRUE(s_display_changed);
}

TEST(display_tests, make_custom_char_pressure_peak_marker)
{
  make_custom_char(DISPLAY_CHAR_PEAK_PRESSURE_0);
  TEST_ASSERT_EQUAL_UINT64(0x0101010101010101,
    s_custom_chars[CUSTOM_CHAR_PRESSURE_BAR_PEAK]);

  make_custom_char(DISPLAY_CHAR_PEAK_PRESSURE_1);
  TEST_ASSERT_EQUAL_UINT64(0x1010101010101010,
    s_custom_chars[CUSTOM_CHAR_PRESSURE_BAR_PEAK]);

  make_custom_char(DISPLAY_CHAR_PEAK_PRESSURE_2);
  TEST_ASSERT_EQUAL_UINT64(0x0404040404040404,
    s_custom_chars[CUSTOM_CHAR_PRESSURE_BAR_PEAK]);

  make_custom_char(DISPLAY_CHAR_PEAK_PRESSURE_3);
  TEST_ASSERT_EQUAL_UINT64(0x0101010101010101,
    s_custom_chars[CUSTOM_CHAR_PRESSURE_BAR_PEAK]);

  // Check others characters are unmodified
  TEST_ASSERT_EQUAL_UINT64(0u,
    s_custom_chars[CUSTOM_CHAR_BATTERY_INDICATOR]);
  TEST_ASSERT_EQUAL_UINT64(0u,
    s_custom_chars[CUSTOM_CHAR_PRESSURE_BAR_EDGE]);
}

TEST(display_tests, make_custom_char_pressure_bar_edge)
{
  // Invalid
  make_custom_char(DISPLAY_CHAR_PRESSURE_0);
  TEST_ASSERT_EQUAL_UINT64(0u,
    s_custom_chars[CUSTOM_CHAR_PRESSURE_BAR_EDGE]);

  make_custom_char(DISPLAY_CHAR_PRESSURE_1);
  TEST_ASSERT_EQUAL_UINT64(0x1010101010101010,
    s_custom_chars[CUSTOM_CHAR_PRESSURE_BAR_EDGE]);

  make_custom_char(DISPLAY_CHAR_PRESSURE_2);
  TEST_ASSERT_EQUAL_UINT64(0x1C1C1C1C1C1C1C1C,
    s_custom_chars[CUSTOM_CHAR_PRESSURE_BAR_EDGE]);

  make_custom_char(DISPLAY_CHAR_PRESSURE_3);
  TEST_ASSERT_EQUAL_UINT64(0x1F1F1F1F1F1F1F1F,
    s_custom_chars[CUSTOM_CHAR_PRESSURE_BAR_EDGE]);

  // Check others characters are unmodified
  TEST_ASSERT_EQUAL_UINT64(0u,
    s_custom_chars[CUSTOM_CHAR_BATTERY_INDICATOR]);
  TEST_ASSERT_EQUAL_UINT64(0u,
    s_custom_chars[CUSTOM_CHAR_PRESSURE_BAR_PEAK]);
}

TEST(display_tests, make_custom_char_battery_empty)
{
  make_custom_char(DISPLAY_CHAR_BATTERY_0);
  TEST_ASSERT_EQUAL_UINT64(BATTERY_INDICATOR_OUTLINE,
    s_custom_chars[CUSTOM_CHAR_BATTERY_INDICATOR]);

  // Check others characters are unmodified
  TEST_ASSERT_EQUAL_UINT64(0u,
    s_custom_chars[CUSTOM_CHAR_PRESSURE_BAR_EDGE]);
  TEST_ASSERT_EQUAL_UINT64(0u,
    s_custom_chars[CUSTOM_CHAR_PRESSURE_BAR_PEAK]);
}

TEST(display_tests, make_custom_char_battery)
{
  make_custom_char(DISPLAY_CHAR_BATTERY_20);
  TEST_ASSERT_EQUAL_UINT64((BATTERY_INDICATOR_OUTLINE | 0xE00),
    s_custom_chars[CUSTOM_CHAR_BATTERY_INDICATOR]);

  make_custom_char(DISPLAY_CHAR_BATTERY_40);
  TEST_ASSERT_EQUAL_UINT64((BATTERY_INDICATOR_OUTLINE | 0xE0E00),
    s_custom_chars[CUSTOM_CHAR_BATTERY_INDICATOR]);

  make_custom_char(DISPLAY_CHAR_BATTERY_60);
  TEST_ASSERT_EQUAL_UINT64((BATTERY_INDICATOR_OUTLINE | 0xE0E0E00),
    s_custom_chars[CUSTOM_CHAR_BATTERY_INDICATOR]);

  make_custom_char(DISPLAY_CHAR_BATTERY_80);
  TEST_ASSERT_EQUAL_UINT64((BATTERY_INDICATOR_OUTLINE | 0xE0E0E0E00),
    s_custom_chars[CUSTOM_CHAR_BATTERY_INDICATOR]);

  make_custom_char(DISPLAY_CHAR_BATTERY_100);
  TEST_ASSERT_EQUAL_UINT64((BATTERY_INDICATOR_OUTLINE | 0xE0E0E0E0E00),
    s_custom_chars[CUSTOM_CHAR_BATTERY_INDICATOR]);

  // Check others characters are unmodified
  TEST_ASSERT_EQUAL_UINT64(0u,
    s_custom_chars[CUSTOM_CHAR_PRESSURE_BAR_EDGE]);
  TEST_ASSERT_EQUAL_UINT64(0u,
    s_custom_chars[CUSTOM_CHAR_PRESSURE_BAR_PEAK]);
}

TEST(display_tests, format_tidal_volume_text)
{
  char expected1[DISP_TIDAL_VOL_LEN] = {'0', '0', '9'};
  display_format_tidal_volume(9u);
  TEST_ASSERT_EQUAL_CHAR_ARRAY(expected1, &s_display[DISP_TIDAL_VOL], DISP_TIDAL_VOL_LEN);
  TEST_ASSERT_TRUE(display_has_changed());

  char expected2[DISP_TIDAL_VOL_LEN] = {'1', '2', '3'};
  display_format_tidal_volume(123u);
  TEST_ASSERT_EQUAL_CHAR_ARRAY(expected2, &s_display[DISP_TIDAL_VOL], DISP_TIDAL_VOL_LEN);
}

TEST(display_tests, format_tidal_volume_out_of_bounds)
{
  char expected[DISP_TIDAL_VOL_LEN] = {'>', '1', 'k'};
  display_format_tidal_volume(9999u);
  TEST_ASSERT_EQUAL_CHAR_ARRAY(expected, &s_display[DISP_TIDAL_VOL], DISP_TIDAL_VOL_LEN);
  TEST_ASSERT_TRUE(display_has_changed());
}

TEST(display_tests, format_tidal_volume_text_no_change)
{
  message_mcu_to_fpga_t message;

  display_format_tidal_volume(50u);
  TEST_ASSERT_TRUE(display_has_changed());

  display_get(&message);
  TEST_ASSERT_FALSE(display_has_changed());

  display_format_tidal_volume(50u);
  TEST_ASSERT_FALSE(display_has_changed());
}

TEST(display_tests, format_peak_flow_text)
{
  // Argument is in dl/min and output is in l/min.  9.6 l/min == 96 dl/min
  char expected1[DISP_PEAK_FLOW_LEN] = {'0', '9', '.', '6'};
  display_format_peak_flow(96u);
  TEST_ASSERT_EQUAL_CHAR_ARRAY(expected1, &s_display[DISP_PEAK_FLOW],
    DISP_PEAK_FLOW_LEN);
  TEST_ASSERT_TRUE(display_has_changed());

  // Argument is in dl/min and output is in l/min.  34.2 l/min == 342 dl/min
  char expected2[DISP_PEAK_FLOW_LEN] = {'3', '4', '.', '2'};
  display_format_peak_flow(342u);
  TEST_ASSERT_EQUAL_CHAR_ARRAY(expected2, &s_display[DISP_PEAK_FLOW],
    DISP_PEAK_FLOW_LEN);
}

TEST(display_tests, format_peak_flow_out_of_bounds)
{
  // Argument is in ml/s and output is in l/min.  100 l/min == 1667 ml/s
  char expected[DISP_PEAK_FLOW_LEN] = {'>', '1', '0', '0'};
  display_format_peak_flow(1667u);
  TEST_ASSERT_EQUAL_CHAR_ARRAY(expected, &s_display[DISP_PEAK_FLOW],
    DISP_PEAK_FLOW_LEN);
  TEST_ASSERT_TRUE(display_has_changed());
}

TEST(display_tests, format_peak_flow_text_no_change)
{
  message_mcu_to_fpga_t message;

  display_format_peak_flow(50u);
  TEST_ASSERT_TRUE(display_has_changed());

  display_get(&message);
  TEST_ASSERT_FALSE(display_has_changed());

  display_format_peak_flow(50u);
  TEST_ASSERT_FALSE(display_has_changed());
}

TEST(display_tests, format_respiration_rate_text)
{
  char resp_rate_expected1[DISP_RESP_RATE_LEN] = {'0', '9'};
  display_format_respiration_rate(9u);
  TEST_ASSERT_EQUAL_CHAR_ARRAY(resp_rate_expected1, &s_display[DISP_RESP_RATE], DISP_RESP_RATE_LEN);
  TEST_ASSERT_TRUE(display_has_changed());

  char resp_rate_expected2[DISP_RESP_RATE_LEN] = {'1', '2'};
  display_format_respiration_rate(12u);
  TEST_ASSERT_EQUAL_CHAR_ARRAY(resp_rate_expected2, &s_display[DISP_RESP_RATE], DISP_RESP_RATE_LEN);
}

TEST(display_tests, format_respiration_rate_out_of_bounds)
{
  char resp_rate_expected[DISP_RESP_RATE_LEN] = {'-', '-'};
  display_format_respiration_rate(UINT8_MAX);
  TEST_ASSERT_EQUAL_CHAR_ARRAY(resp_rate_expected, &s_display[DISP_RESP_RATE], DISP_RESP_RATE_LEN);
  TEST_ASSERT_TRUE(display_has_changed());
}

TEST(display_tests, format_respiration_rate_text_no_change)
{
  message_mcu_to_fpga_t message;

  display_format_respiration_rate(50u);
  TEST_ASSERT_TRUE(display_has_changed());

  display_get(&message);
  TEST_ASSERT_FALSE(display_has_changed());

  display_format_respiration_rate(50u);
  TEST_ASSERT_FALSE(display_has_changed());
}

TEST(display_tests, format_percent_o2_text)
{
  char percent_expected1[DISP_PERCENT_LEN] = {'0', '0', '9'};
  display_format_percent_o2(9u);
  TEST_ASSERT_EQUAL_CHAR_ARRAY(percent_expected1, &s_display[DISP_PERCENT_O2], DISP_PERCENT_LEN);
  TEST_ASSERT_TRUE(display_has_changed());

  char percent_expected2[DISP_PERCENT_LEN] = {'1', '0', '0'};
  display_format_percent_o2(100u);
  TEST_ASSERT_EQUAL_CHAR_ARRAY(percent_expected2, &s_display[DISP_PERCENT_O2], DISP_PERCENT_LEN);
}

TEST(display_tests, format_percent_o2_out_of_bounds)
{
  // Display should cap at 100%
  char percent_expected[DISP_PERCENT_LEN] = {'1', '0', '0'};
  display_format_percent_o2(UINT8_MAX);
  TEST_ASSERT_EQUAL_CHAR_ARRAY(percent_expected, &s_display[DISP_PERCENT_O2], DISP_PERCENT_LEN);
  TEST_ASSERT_TRUE(display_has_changed());
}

TEST(display_tests, format_percent_o2_text_no_change)
{
  message_mcu_to_fpga_t message;

  display_format_percent_o2(50u);
  TEST_ASSERT_TRUE(display_has_changed());

  display_get(&message);
  TEST_ASSERT_FALSE(display_has_changed());

  display_format_percent_o2(50u);
  TEST_ASSERT_FALSE(display_has_changed());
}

TEST(display_tests, format_pressure_text)
{
  char pressure_expected1[DISP_PRESSURE_LEN] = {'0', '9'};
  display_format_pressure_bar(9u, 0u);
  TEST_ASSERT_EQUAL_CHAR_ARRAY(pressure_expected1, &s_display[DISP_PRESSURE], DISP_PRESSURE_LEN);
  TEST_ASSERT_TRUE(display_has_changed());

  char pressure_expected2[DISP_PRESSURE_LEN] = {'1', '2'};
  display_format_pressure_bar(12u, 0u);
  TEST_ASSERT_EQUAL_CHAR_ARRAY(pressure_expected2, &s_display[DISP_PRESSURE], DISP_PRESSURE_LEN);
}

TEST(display_tests, format_pressure_text_out_of_bounds)
{
  char pressure_expected[DISP_PRESSURE_LEN] = {'-', '-'};
  display_format_pressure_bar(100u, 0u);
  TEST_ASSERT_EQUAL_CHAR_ARRAY(pressure_expected, &s_display[DISP_PRESSURE], DISP_PRESSURE_LEN);
  TEST_ASSERT_TRUE(display_has_changed());
}

TEST(display_tests, format_pressure_text_no_change)
{
  message_mcu_to_fpga_t message;

  display_format_pressure_bar(50u, 50u);
  TEST_ASSERT_TRUE(display_has_changed());

  display_get(&message);
  TEST_ASSERT_FALSE(display_has_changed());

  display_format_pressure_bar(50u, 50u);
  TEST_ASSERT_FALSE(display_has_changed());
}

TEST(display_tests, format_pressure_bar_rising)
{
  char expected[DISP_PRESSURE_GAUGE_LEN] =
    {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',  ' '};

  display_format_pressure_bar(0u, 0u);
  TEST_ASSERT_EQUAL_CHAR_ARRAY(expected, &s_display[DISP_PRESSURE_GAUGE], DISP_PRESSURE_GAUGE_LEN);

  expected[0] = FULL_BLOCK;
  display_format_pressure_bar((PRESSURE_BAR_MMH2O_INC * 3u) / 10u, 0u);
  TEST_ASSERT_EQUAL_CHAR_ARRAY(expected, &s_display[DISP_PRESSURE_GAUGE], DISP_PRESSURE_GAUGE_LEN);

  expected[1] = FULL_BLOCK;
  display_format_pressure_bar((PRESSURE_BAR_MMH2O_INC * 6u) / 10u, 0u);
  TEST_ASSERT_EQUAL_CHAR_ARRAY(expected, &s_display[DISP_PRESSURE_GAUGE], DISP_PRESSURE_GAUGE_LEN);

  expected[2] = FULL_BLOCK;
  display_format_pressure_bar((PRESSURE_BAR_MMH2O_INC * 9u) / 10u, 0u);
  TEST_ASSERT_EQUAL_CHAR_ARRAY(expected, &s_display[DISP_PRESSURE_GAUGE], DISP_PRESSURE_GAUGE_LEN);
}

TEST(display_tests, format_pressure_bar_falling)
{
  char expected[DISP_PRESSURE_GAUGE_LEN] =
  {
    FULL_BLOCK, FULL_BLOCK, FULL_BLOCK,
    ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',
    CUSTOM_CHAR_PRESSURE_BAR_PEAK
  };

  display_format_pressure_bar((PRESSURE_BAR_MMH2O_INC * 9u) / 10u, PRESSURE_BAR_MMH2O_MAX);
  TEST_ASSERT_EQUAL_CHAR_ARRAY(expected, &s_display[DISP_PRESSURE_GAUGE], DISP_PRESSURE_GAUGE_LEN);

  expected[2] = ' ';
  display_format_pressure_bar((PRESSURE_BAR_MMH2O_INC * 6u) / 10u, PRESSURE_BAR_MMH2O_MAX);
  TEST_ASSERT_EQUAL_CHAR_ARRAY(expected, &s_display[DISP_PRESSURE_GAUGE], DISP_PRESSURE_GAUGE_LEN);

  expected[1] = ' ';
  display_format_pressure_bar((PRESSURE_BAR_MMH2O_INC * 3u) / 10u, PRESSURE_BAR_MMH2O_MAX);
  TEST_ASSERT_EQUAL_CHAR_ARRAY(expected, &s_display[DISP_PRESSURE_GAUGE], DISP_PRESSURE_GAUGE_LEN);

  expected[0] = ' ';
  display_format_pressure_bar(0u, PRESSURE_BAR_MMH2O_MAX);
  TEST_ASSERT_EQUAL_CHAR_ARRAY(expected, &s_display[DISP_PRESSURE_GAUGE], DISP_PRESSURE_GAUGE_LEN);
}

TEST(display_tests, format_pressure_bar_edge_chars)
{
  char expected[DISP_PRESSURE_GAUGE_LEN] =
  {
    FULL_BLOCK,
    CUSTOM_CHAR_PRESSURE_BAR_EDGE,
    CUSTOM_CHAR_PRESSURE_BAR_PEAK,
    ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '
  };

  make_custom_char(DISPLAY_CHAR_PRESSURE_1);
  make_custom_char(DISPLAY_CHAR_PEAK_PRESSURE_2);

  display_format_pressure_bar((PRESSURE_BAR_MMH2O_INC * 4u) / 10u,
    (PRESSURE_BAR_MMH2O_INC * 8u) / 10u);
  TEST_ASSERT_EQUAL_CHAR_ARRAY(expected, &s_display[DISP_PRESSURE_GAUGE], DISP_PRESSURE_GAUGE_LEN);

  // Check the characters have *not* been combined
  TEST_ASSERT_EQUAL_UINT64(0x1010101010101010, s_custom_chars[CUSTOM_CHAR_PRESSURE_BAR_EDGE]);
  TEST_ASSERT_EQUAL_UINT64(0x0404040404040404, s_custom_chars[CUSTOM_CHAR_PRESSURE_BAR_PEAK]);
}

TEST(display_tests, format_pressure_bar_overlapping_edge_chars)
{
  char expected[DISP_PRESSURE_GAUGE_LEN] =
  {
    FULL_BLOCK, FULL_BLOCK,
    CUSTOM_CHAR_PRESSURE_BAR_EDGE,
    ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '
  };

  make_custom_char(DISPLAY_CHAR_PRESSURE_1);
  make_custom_char(DISPLAY_CHAR_PEAK_PRESSURE_2);

  display_format_pressure_bar((PRESSURE_BAR_MMH2O_INC * 7u) / 10u,
    (PRESSURE_BAR_MMH2O_INC * 8u) / 10u);
  TEST_ASSERT_EQUAL_CHAR_ARRAY(expected, &s_display[DISP_PRESSURE_GAUGE], DISP_PRESSURE_GAUGE_LEN);

  // Check the characters have been combined
  TEST_ASSERT_EQUAL_UINT64(0x1414141414141414, s_custom_chars[CUSTOM_CHAR_PRESSURE_BAR_EDGE]);
}

TEST(display_tests, format_pressure_bar_out_of_bounds)
{
  char expected[DISP_PRESSURE_GAUGE_LEN] =
  {
    FULL_BLOCK, FULL_BLOCK, FULL_BLOCK, FULL_BLOCK,
    FULL_BLOCK, FULL_BLOCK, FULL_BLOCK, FULL_BLOCK,
    FULL_BLOCK, FULL_BLOCK, FULL_BLOCK, FULL_BLOCK,
    FULL_BLOCK, FULL_BLOCK
  };

  display_format_pressure_bar(UINT16_MAX, 0u);
  TEST_ASSERT_EQUAL_CHAR_ARRAY(expected, &s_display[DISP_PRESSURE_GAUGE], DISP_PRESSURE_GAUGE_LEN);
}

TEST(display_tests, format_battery_gauge_char)
{
  display_format_battery_gauge(0u);
  TEST_ASSERT_EQUAL_UINT64(BATTERY_INDICATOR_OUTLINE,
    s_custom_chars[CUSTOM_CHAR_BATTERY_INDICATOR]);

  display_format_battery_gauge(255u);
  TEST_ASSERT_EQUAL_UINT64((BATTERY_INDICATOR_OUTLINE | 0xE0E0E0E0E00),
    s_custom_chars[CUSTOM_CHAR_BATTERY_INDICATOR]);

  display_format_battery_gauge(5u);
  TEST_ASSERT_EQUAL_UINT64(BATTERY_INDICATOR_OUTLINE,
    s_custom_chars[CUSTOM_CHAR_BATTERY_INDICATOR]);

  display_format_battery_gauge(20u);
  TEST_ASSERT_EQUAL_UINT64((BATTERY_INDICATOR_OUTLINE | 0xE00),
    s_custom_chars[CUSTOM_CHAR_BATTERY_INDICATOR]);

  display_format_battery_gauge(40u);
  TEST_ASSERT_EQUAL_UINT64((BATTERY_INDICATOR_OUTLINE | 0xE0E00),
    s_custom_chars[CUSTOM_CHAR_BATTERY_INDICATOR]);

  display_format_battery_gauge(60u);
  TEST_ASSERT_EQUAL_UINT64((BATTERY_INDICATOR_OUTLINE | 0xE0E0E00),
    s_custom_chars[CUSTOM_CHAR_BATTERY_INDICATOR]);

  display_format_battery_gauge(80u);
  TEST_ASSERT_EQUAL_UINT64((BATTERY_INDICATOR_OUTLINE | 0xE0E0E0E00),
    s_custom_chars[CUSTOM_CHAR_BATTERY_INDICATOR]);

  display_format_battery_gauge(100u);
  TEST_ASSERT_EQUAL_UINT64((BATTERY_INDICATOR_OUTLINE | 0xE0E0E0E0E00),
    s_custom_chars[CUSTOM_CHAR_BATTERY_INDICATOR]);

  // Ensure the battery character is inserted into the cached display
  TEST_ASSERT_EQUAL_CHAR(CUSTOM_CHAR_BATTERY_INDICATOR, s_display[DISP_BATTERY]);
}

TEST(display_tests, format_battery_gauge_char_no_change)
{
  message_mcu_to_fpga_t message;

  display_format_battery_gauge(1u);
  TEST_ASSERT_TRUE(display_has_changed());

  display_get(&message);
  TEST_ASSERT_FALSE(display_has_changed());

  display_format_battery_gauge(1u);
  TEST_ASSERT_FALSE(display_has_changed());

  // Ensure the battery character is inserted into the cached display
  TEST_ASSERT_EQUAL_CHAR(CUSTOM_CHAR_BATTERY_INDICATOR, s_display[DISP_BATTERY]);
}
