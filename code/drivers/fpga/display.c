#include "display.h"
#include "private/display_priv.h"
#include "misc/util.h"
#include <string.h>
#include <stdint.h>

TESTABLE uint64_t s_custom_chars[CUSTOM_CHAR_COUNT] = {0u};
TESTABLE char s_display[DISP_LEN] = {0u};
TESTABLE bool s_display_changed = false;

/**
 * Replace a cached custom_char
 *
 * @param char_to_create enum display_char
 */
TESTABLE void make_custom_char(enum display_char char_to_create)
{
  switch (char_to_create)
  {
    case DISPLAY_CHAR_PEAK_PRESSURE_0:
      char_to_create = DISPLAY_CHAR_PEAK_PRESSURE_3;
      /* fall-through */
    case DISPLAY_CHAR_PEAK_PRESSURE_1:
    case DISPLAY_CHAR_PEAK_PRESSURE_2:
    case DISPLAY_CHAR_PEAK_PRESSURE_3:
    {
      uint32_t shifts = (char_to_create - DISPLAY_CHAR_PEAK_PRESSURE_0 - 1u) * 2u;
      uint64_t return_char = (PRESSURE_BAR_EDGE >> shifts);

      if (s_custom_chars[CUSTOM_CHAR_PRESSURE_BAR_PEAK] != return_char)
      {
        s_display_changed = true;
        s_custom_chars[CUSTOM_CHAR_PRESSURE_BAR_PEAK] = return_char;
      }
      break;
    }

    case DISPLAY_CHAR_PRESSURE_0:
      break;

    case DISPLAY_CHAR_PRESSURE_1:
    case DISPLAY_CHAR_PRESSURE_2:
    case DISPLAY_CHAR_PRESSURE_3:
    {
      uint32_t shifts = (char_to_create - DISPLAY_CHAR_PRESSURE_1) * 2u;
      uint64_t return_char = PRESSURE_BAR_EDGE;

      while (shifts > 0)
      {
        return_char |= (PRESSURE_BAR_EDGE >> shifts);
        shifts--;
      }

      if (s_custom_chars[CUSTOM_CHAR_PRESSURE_BAR_EDGE] != return_char)
      {
        s_display_changed = true;
        s_custom_chars[CUSTOM_CHAR_PRESSURE_BAR_EDGE] = return_char;
      }
      break;
    }

    case DISPLAY_CHAR_BATTERY_0:
    case DISPLAY_CHAR_BATTERY_20:
    case DISPLAY_CHAR_BATTERY_40:
    case DISPLAY_CHAR_BATTERY_60:
    case DISPLAY_CHAR_BATTERY_80:
    case DISPLAY_CHAR_BATTERY_100:
    {
      uint32_t shifts = char_to_create - DISPLAY_CHAR_BATTERY_0;
      uint64_t return_char = BATTERY_INDICATOR_OUTLINE;
      uint64_t shift_char = BATTERY_INDICATOR_EDGE;

      while (shifts > 0)
      {
        shift_char <<= 8u;
        return_char |= shift_char;
        shifts--;
      }

      if (s_custom_chars[CUSTOM_CHAR_BATTERY_INDICATOR] != return_char)
      {
        s_display_changed = true;
        s_custom_chars[CUSTOM_CHAR_BATTERY_INDICATOR] = return_char;
      }
      break;
    }
  }
}

void display_format_tidal_volume(uint16_t tidal_volume_ml)
{
  // Format display resolution and units: ml
  static uint16_t last_val = 0u;

  if (tidal_volume_ml != last_val)
  {
    last_val = tidal_volume_ml;

    char digits[DISP_TIDAL_VOL_LEN] = {'>', '1', 'k'};

    if (tidal_volume_ml <= 999u)
    {
      uint16_t remainder = tidal_volume_ml;
      digits[0] = remainder / 100u;
      remainder -= (digits[0] * 100u);
      digits[1] = remainder / 10u;
      digits[2] = remainder - (digits[1] * 10u);

      for (size_t i = 0; i < DISP_TIDAL_VOL_LEN; i++)
      {
        digits[i] += '0';
      }
    }

    strncpy(&s_display[DISP_TIDAL_VOL], digits, DISP_TIDAL_VOL_LEN);
    s_display_changed = true;
  }
}

void display_format_peak_flow(uint16_t peak_flow_dl_per_min)
{
  // Format display resolution and units: 0.1 l/min
  static uint16_t last_val = 0u;

  if (peak_flow_dl_per_min != last_val)
  {
    last_val = peak_flow_dl_per_min;

    char digits[DISP_PEAK_FLOW_LEN] = {'>', '1', '0', '0'};

    if (peak_flow_dl_per_min < 1000u)
    {
      uint16_t remainder = peak_flow_dl_per_min;
      digits[0] = remainder / 100u;
      remainder -= (digits[0] * 100u);
      digits[1] = remainder / 10u;
      digits[2] = remainder - (digits[1] * 10u);

      for (size_t i = 0; i < (DISP_PEAK_FLOW_LEN - 1u); i++)
      {
        digits[i] += '0';
      }

      // 'Divide' by 10 by inserting a decimal point before the final digit
      digits[3] = digits[2];
      digits[2] = '.';
    }

    strncpy(&s_display[DISP_PEAK_FLOW], digits, DISP_PEAK_FLOW_LEN);
    s_display_changed = true;
  }
}

void display_format_respiration_rate(uint8_t breaths_per_min)
{
  // Format display resolution and units: 1 breaths per minute
  static uint8_t last_val = 0u;

  if (breaths_per_min != last_val)
  {
    last_val = breaths_per_min;

    char digits[DISP_RESP_RATE_LEN] = {'-', '-'};

    if (breaths_per_min < 100u)
    {
      digits[0] = breaths_per_min / 10u;
      digits[1] = breaths_per_min - (digits[0] * 10u);

      for (size_t i = 0; i < DISP_RESP_RATE_LEN; i++)
      {
        digits[i] += '0';
      }
    }

    strncpy(&s_display[DISP_RESP_RATE], digits, DISP_RESP_RATE_LEN);
    s_display_changed = true;
  }
}

void display_format_percent_o2(uint8_t oxygen_percent)
{
  static uint8_t last_val = 0u;

  if (oxygen_percent != last_val)
  {
    last_val = oxygen_percent;

    char digits[DISP_PERCENT_LEN] = {'1', '0', '0'};

    if (oxygen_percent < 100u)
    {
      digits[0] = 0;
      digits[1] = oxygen_percent / 10u;
      digits[2] = oxygen_percent - (digits[0] * 10u);

      for (size_t i = 0; i < DISP_PERCENT_LEN; i++)
      {
        digits[i] += '0';
      }
    }

    strncpy(&s_display[DISP_PERCENT_O2], digits, DISP_PERCENT_LEN);
    s_display_changed = true;
  }
}

void display_format_battery_gauge(uint8_t charge_percent)
{
  if (charge_percent > 80u)
  {
    make_custom_char(DISPLAY_CHAR_BATTERY_100);
  }
  else if (charge_percent > 60u)
  {
    make_custom_char(DISPLAY_CHAR_BATTERY_80);
  }
  else if (charge_percent > 40u)
  {
    make_custom_char(DISPLAY_CHAR_BATTERY_60);
  }
  else if (charge_percent > 20u)
  {
    make_custom_char(DISPLAY_CHAR_BATTERY_40);
  }
  else if (charge_percent > 5u)
  {
    make_custom_char(DISPLAY_CHAR_BATTERY_20);
  }
  else
  {
    make_custom_char(DISPLAY_CHAR_BATTERY_0);
  }

  // Ensure the indicator shows on the display if overwritten
  s_display[DISP_BATTERY] = (char)CUSTOM_CHAR_BATTERY_INDICATOR;
}

void display_format_pressure_bar(uint16_t pressure_cmH2O, uint16_t peak_pressure_cmH2O)
{
  static uint16_t last_pressure = 0;
  static uint16_t last_peak = 0u;

  // Format pressure value
  if (pressure_cmH2O != last_pressure)
  {
    char digits[DISP_PRESSURE_LEN] = {'-', '-'};

    if (pressure_cmH2O < 100u)
    {
      digits[0] = pressure_cmH2O / 10u;
      digits[1] = pressure_cmH2O - (digits[0] * 10u);

      for (size_t i = 0; i < DISP_RESP_RATE_LEN; i++)
      {
        digits[i] += '0';
      }
    }

    strncpy(&s_display[DISP_PRESSURE], digits, DISP_PRESSURE_LEN);
  }

  // TODO: The peak value won't change very often, so this function could be speed optimised
  // by copying the cached bar into memory and editing it instead of recreating the bar
  // every time (GitHub issue #30)
  // if (peak_pressure_mmH2O != last_peak)
  // {

  // }

  // Format pressure bar
  if ((pressure_cmH2O != last_pressure) || (peak_pressure_cmH2O != last_peak))
  {
    char pressure_bar[DISP_PRESSURE_GAUGE_LEN];

    // Start with an empty gauge
    memset(pressure_bar, ' ', sizeof(pressure_bar));

    // Calculate the incremental positions of the peak and instantaneous pressures
    uint16_t peak_increments = peak_pressure_cmH2O * 10u / PRESSURE_BAR_MMH2O_INC;
    if (peak_increments > PRESSURE_BAR_INCREMENTS)
    {
      peak_increments = PRESSURE_BAR_INCREMENTS;
    }

    uint16_t pressure_increments = pressure_cmH2O * 10u / PRESSURE_BAR_MMH2O_INC;
    if (pressure_increments > PRESSURE_BAR_INCREMENTS)
    {
      pressure_increments = PRESSURE_BAR_INCREMENTS;
    }

    // Build the display block by block
    size_t index = 0u;
    if (pressure_increments >= 0u)
    {
      while ((pressure_increments >= PRESSURE_BAR_SUB_INCREMENTS) &&
        (index < PRESSURE_BAR_INCREMENTS))
      {
        pressure_bar[index] = (char)FULL_BLOCK;
        pressure_increments -= PRESSURE_BAR_SUB_INCREMENTS;
        peak_increments -= PRESSURE_BAR_SUB_INCREMENTS;
        index++;
      }

      if (pressure_increments > 0u)
      {
        // Cap off the gauge with a partial block
        pressure_bar[index] = (char)CUSTOM_CHAR_PRESSURE_BAR_EDGE;
      }
    }

    // Add peak pressure mark if still required
    bool chars_overlap = false;
    if (peak_increments > 0u)
    {
      if (peak_increments <= PRESSURE_BAR_SUB_INCREMENTS)
      {
        chars_overlap = true;
      }
      else
      {
        while ((peak_increments > PRESSURE_BAR_SUB_INCREMENTS) &&
          (index < PRESSURE_BAR_INCREMENTS))
        {
          peak_increments -= PRESSURE_BAR_SUB_INCREMENTS;
          index++;
        }

        // Cap off the gauge with the peak line
        pressure_bar[index] = (char)CUSTOM_CHAR_PRESSURE_BAR_PEAK;
      }
    }

    // Make the appropriate characters based on the remainders
    make_custom_char(DISPLAY_CHAR_PEAK_PRESSURE_0 + peak_increments);
    if (pressure_increments > 0u)
    {
      make_custom_char(DISPLAY_CHAR_PRESSURE_0 + pressure_increments);

      if (chars_overlap)
      {
        // Characters overlap, so we need to OR them and store in the instantaneous character
        // position (because the peak character doesn't change as much)
        s_custom_chars[CUSTOM_CHAR_PRESSURE_BAR_EDGE] |=
          s_custom_chars[CUSTOM_CHAR_PRESSURE_BAR_PEAK];
      }
    }

    last_pressure = pressure_cmH2O;
    last_peak = peak_pressure_cmH2O;

    strncpy(&s_display[DISP_PRESSURE_GAUGE], pressure_bar, DISP_PRESSURE_GAUGE_LEN);
    s_display_changed = true;
  }
}

bool display_has_changed(void)
{
  return s_display_changed;
}

void display_get(message_mcu_to_fpga_t* const message_to_format)
{
  if (!message_to_format)
  {
    return;
  }

  memcpy(message_to_format->display_lines, s_display, sizeof(s_display));
  message_to_format->battery_indicator = s_custom_chars[CUSTOM_CHAR_BATTERY_INDICATOR];
  message_to_format->pressure_bar_edge = s_custom_chars[CUSTOM_CHAR_PRESSURE_BAR_EDGE];
  message_to_format->pressure_bar_peak = s_custom_chars[CUSTOM_CHAR_PRESSURE_BAR_PEAK];
  s_display_changed = false;
}
