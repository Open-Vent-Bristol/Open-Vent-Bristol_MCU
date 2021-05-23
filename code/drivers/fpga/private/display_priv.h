// Copyright (c) 2021 <OpenVent-Bristol, Donald Robson>

#pragma once

#define DISPLAY_STR_HOLD_MUTE_SEL   "Hold Mute/Select\nTo turn off"

#define DISPLAY_STR_L1_QUICK_CALIB  "QuickCalibration"
#define DISPLAY_STR_L1_FULL_CALIB   "FullCalibration"
#define DISPLAY_STR_L1_TUBES        "Disconnect tubes"
#define DISPLAY_STR_L1_REDUCE_O2    "Reduce O2 to 0"
#define DISPLAY_STR_L1_INCREASE_O2  "incr. O2 to 100%"
#define DISPLAY_STR_L1_REMOVE_O2    "remove O2 supply"
#define DISPLAY_STR_L1_CONNECT_N2   "connect N2 100%"
#define DISPLAY_STR_L1_PLEASE_WAIT  "Please wait."

#define DISPLAY_STR_L2_STANDBY      "\nStandby"
#define DISPLAY_STR_L2_CONFIRM      "\nConfirm with Sel"
#define DISPLAY_STR_L2_SUCCESS      "\nSuccessful"
#define DISPLAY_STR_L2_FAILED       "\nFailed"

#define BATTERY_INDICATOR_OUTLINE \
  (uint64_t)( 0b01110ull << 56u | \
              0b11111ull << 48u | \
              0b10001ull << 40u | \
              0b10001ull << 32u | \
              0b10001ull << 24u | \
              0b10001ull << 16u | \
              0b10001ull << 8u  | \
              0b11111ull )

#define BATTERY_INDICATOR_EDGE (0b01110)

#define BATTERY_INDICATOR_FAULT \
  (uint64_t)( 0b01110ull << 56u | \
              0b11111ull << 48u | \
              0b10110ull << 40u | \
              0b11000ull << 32u | \
              0b00011ull << 24u | \
              0b01101ull << 16u | \
              0b10001ull << 8u  | \
              0b11111ull )

/**
 * This just creates a single vertical line down the left side.  To adjust the position,
 * right shift up to 4.  To create a solid block, OR the result in a loop
 */
#define PRESSURE_BAR_EDGE \
  (uint64_t)( 0b10000ull << 56u | \
              0b10000ull << 48u | \
              0b10000ull << 40u | \
              0b10000ull << 32u | \
              0b10000ull << 24u | \
              0b10000ull << 16u | \
              0b10000ull << 8u  | \
              0b10000ull )

#define FULL_BLOCK (219u)

enum display_index
{
  // TOP LINE
  DISP_TIDAL_VOL          = 0,
  DISP_TIDAL_VOL_LEN        = 3,
  DISP_PEAK_FLOW          = 4,
  DISP_PEAK_FLOW_LEN        = 4,
  DISP_RESP_RATE          = 9,
  DISP_RESP_RATE_LEN        = 2,
  DISP_PERCENT_O2         = 12,
  DISP_PERCENT_LEN          = 3,
  DISP_BATTERY            = 15,
  DISP_BATTERY_LEN          = 1,
  // BOTTOM LINE
  DISP_PRESSURE           = 16,
  DISP_PRESSURE_LEN         = 2,
  DISP_PRESSURE_GAUGE     = 18,
  DISP_PRESSURE_GAUGE_LEN = 14,
  DISP_PROGRESS_BAR       = DISP_PRESSURE,
  DISP_PROGRESS_BAR_LEN     = DISP_PRESSURE_LEN + DISP_PRESSURE_GAUGE_LEN,

  DISP_LEN                = 32
};

#define PRESSURE_BAR_MMH2O_MAX      (450u)
#define PRESSURE_BAR_SUB_INCREMENTS (3u) // line left, line centre, line right
#define PRESSURE_BAR_INCREMENTS     (DISP_PRESSURE_GAUGE_LEN * PRESSURE_BAR_SUB_INCREMENTS)
#define PRESSURE_BAR_MMH2O_INC      (PRESSURE_BAR_MMH2O_MAX / PRESSURE_BAR_INCREMENTS)

/**
 * These are the character addresses in the display for the custom characters.
 * Use these as the ascii code, e.g. (char)CUSTOM_CHAR_PRESSURE_BAR_EDGE.
 */
enum custom_char
{
  CUSTOM_CHAR_BATTERY_INDICATOR = 0,
  CUSTOM_CHAR_PRESSURE_BAR_EDGE = 1,
  CUSTOM_CHAR_PRESSURE_BAR_PEAK = 2,
  CUSTOM_CHAR_COUNT
};

/**
 * Createable custom character definitions
 *
 */
enum display_char
{
  DISPLAY_CHAR_PEAK_PRESSURE_0, // Single vertical line to right
  DISPLAY_CHAR_PEAK_PRESSURE_1, // Single vertical line to left
  DISPLAY_CHAR_PEAK_PRESSURE_2, // Single vertical line in centre
  DISPLAY_CHAR_PEAK_PRESSURE_3, // Single vertical line to right
  DISPLAY_CHAR_PRESSURE_0,      // INVALID
  DISPLAY_CHAR_PRESSURE_1,      // Single vertical line to left
  DISPLAY_CHAR_PRESSURE_2,      // Half-width block to left
  DISPLAY_CHAR_PRESSURE_3,      // Full block
  DISPLAY_CHAR_BATTERY_0,       // Empty battery outline
  DISPLAY_CHAR_BATTERY_20,      // Partly filled battery outline
  DISPLAY_CHAR_BATTERY_40,      // Partly filled battery outline
  DISPLAY_CHAR_BATTERY_60,      // Partly filled battery outline
  DISPLAY_CHAR_BATTERY_80,      // Partly filled battery outline
  DISPLAY_CHAR_BATTERY_100,     // Full battery outline
  DISPLAY_CHAR_BATTERY_FAULT,   // Broken battery outline
};
