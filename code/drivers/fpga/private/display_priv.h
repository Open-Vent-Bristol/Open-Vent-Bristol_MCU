// Copyright (c) 2021 <OpenVent-Bristol, Donald Robson>

#pragma once

#define BATTERY_INDICATOR_OUTLINE \
  (uint64_t)( 0b01110ULL << 56u | \
              0b11111ULL << 48u | \
              0b10001ULL << 40u | \
              0b10001ULL << 32u | \
              0b10001ULL << 24u | \
              0b10001ULL << 16u | \
              0b10001ULL << 8u  | \
              0b11111ULL )

#define BATTERY_INDICATOR_EDGE (0b01110)

/**
 * This just creates a single vertical line down the left side.  To adjust the position,
 * right shift up to 4.  To create a solid block, OR the result in a loop
 */
#define PRESSURE_BAR_EDGE \
  (uint64_t)( 0b10000ULL << 56u | \
              0b10000ULL << 48u | \
              0b10000ULL << 40u | \
              0b10000ULL << 32u | \
              0b10000ULL << 24u | \
              0b10000ULL << 16u | \
              0b10000ULL << 8u  | \
              0b10000ULL )

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
  DISPLAY_CHAR_BATTERY_100      // Full battery outline
};
