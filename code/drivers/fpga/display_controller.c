// Copyright (c) 2021 <OpenVent-Bristol, Donald Robson>

#include "display_controller.h"
#include "private/display_format.h"
#include "scheduler/dispatcher.h"
#include "sensor/sensor.h"
#include "misc/util.h"
#include <stddef.h>
#include <stdint.h>

enum display_override_index s_display_override_index = DISPLAY_OVERRIDE_NONE;
enum display_override_line_2_index s_display_override_line_2_index = DISPLAY_OVERRIDE_LINE2_NONE;

static const char* const s_override_strings[] =
{
  [DISPLAY_OVERRIDE_NONE] =               NULL,
  [DISPLAY_OVERRIDE_HOLD_MUTE_SEL] =      "Hold Mute/Select\nTo turn off",
  [DISPLAY_OVERRIDE_LINE1_QUICK_CALIB] =  "QuickCalibration",
  [DISPLAY_OVERRIDE_LINE1_FULL_CALIB] =   "FullCalibration",
  [DISPLAY_OVERRIDE_LINE1_TUBES] =        "Disconnect tubes",
  [DISPLAY_OVERRIDE_LINE1_REDUCE_O2] =    "Reduce O2 to 0",
  [DISPLAY_OVERRIDE_LINE1_INCREASE_O2] =  "incr. O2 to 100%",
  [DISPLAY_OVERRIDE_LINE1_REMOVE_O2] =    "remove O2 supply",
  [DISPLAY_OVERRIDE_LINE1_CONNECT_N2] =   "connect N2 100%",
  [DISPLAY_OVERRIDE_LINE1_PLEASE_WAIT] =  "Please wait.",
};

static const char* const s_override_line_2_strings[] =
{
  [DISPLAY_OVERRIDE_LINE2_NONE] =         NULL,
  [DISPLAY_OVERRIDE_LINE2_STANDBY] =      "\nStandby",
  [DISPLAY_OVERRIDE_LINE2_CONFIRM] =      "\nConfirm with Sel",
  [DISPLAY_OVERRIDE_LINE2_SUCCESS] =      "\nSuccessful",
  [DISPLAY_OVERRIDE_LINE2_FAILED] =       "\nFailed",
};

/**
 * @brief Update the display
 *
 * @param arg Unused
 */
TESTABLE void display_controller_update(int32_t arg)
{
  dispatcher_clear_event_mask(1u << EV_DO_UPDATE_DISPLAY);

  // Most of the time the measurements display should be shown
  if (s_display_override_index == DISPLAY_OVERRIDE_NONE)
  {
    display_format_tidal_volume((uint16_t)sensor_get_reading(SENSOR_TIDAL_VOLUME));
    display_format_peak_flow((uint16_t)sensor_get_reading(SENSOR_PEAK_FLOW_RATE));
    display_format_respiration_rate((uint8_t)sensor_get_reading(SENSOR_RESPIRATION_RATE));
    display_format_percent_o2((uint8_t)sensor_get_reading(SENSOR_PERCENT_O2));
    display_format_pressure_bar(
      (uint16_t)sensor_get_reading(SENSOR_PRESSURE),
      (uint16_t)sensor_get_reading(SENSOR_PEAK_PRESSURE));

    int16_t battery_charge = sensor_get_reading(SENSOR_PERCENT_BATTERY_CHARGE);

    if (battery_charge < 0)
    {
      display_format_battery_fault();
    }
    else
    {
      display_format_battery_gauge((uint8_t)battery_charge);
    }
  }
  else
  {
    // Special displays
    display_format_string(s_override_strings[s_display_override_index]);

    // In most cases a second line will need to be displayed too
    if (s_display_override_line_2_index != DISPLAY_OVERRIDE_LINE2_NONE)
    {
      if (s_display_override_line_2_index < ARRAY_LENGTH(s_override_strings))
      {
        display_format_string(s_override_line_2_strings[s_display_override_line_2_index]);
      }
      else if (s_display_override_line_2_index >= DISPLAY_OVERRIDE_LINE2_EMPTY)
      {
        // Display the progress bar instead
        display_format_progress_bar(s_display_override_line_2_index - DISPLAY_OVERRIDE_LINE2_EMPTY);
      }
    }
  }
}

void display_controller_init(void)
{
  dispatcher_bind(1u << EV_DO_UPDATE_DISPLAY, display_controller_update);
}

void display_controller_set_override(enum display_override_index index)
{
  if (s_display_override_index < ARRAY_LENGTH(s_override_strings))
  {
    s_display_override_index = index;
  }
}

void display_controller_set_line_2_override(enum display_override_line_2_index index)
{
  s_display_override_line_2_index = index;
}
