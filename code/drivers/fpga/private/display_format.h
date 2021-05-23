// Copyright (c) 2021 <OpenVent-Bristol, Donald Robson>

#pragma once

#include "fpga/fpga_api.h"
#include <stdbool.h>
#include <stdint.h>

/**
 * Display formatting functions.  These will update the cached version of the display
 */
void display_format_tidal_volume(uint16_t tidal_volume_ml);
void display_format_peak_flow(uint16_t peak_flow_dl_per_min);
void display_format_respiration_rate(uint8_t breaths_per_min);
void display_format_percent_o2(uint8_t oxygen_percent);
void display_format_battery_gauge(uint8_t charge_percent);
void display_format_battery_fault(void);
void display_format_pressure_bar(uint16_t pressure_cmH2O, uint16_t peak_pressure_cmH2O);
void display_format_progress_bar(uint8_t progress_sixteenths);

/**
 * @brief Display a specific string up to two lines long
 * @note Autowraps unless a newline character \n occurs within the first 16 characters.
 * @note If the first character is a newline \n, the first line of the display will be retained and
 * the remainder of the string should be up to 16 characters.
 * @note If the string is otherwise <= 16 characters, the second line will be retained.
 * @note Trailing spaces will be inserted automatically after a newline \n or null termination \0.
 * @note Newline \n and null termination \0 do not count towards the 32 character limit.
 *
 * @param string_to_display   Null terminated string, or char array
 */
void display_format_string(const char* const string_to_display);

/**
 * @brief Check if the cached display has been changed (to avoid needing to call display_format_get)
 *
 * @return true Cached version of display has been updated
 * @return false Cached version of display is unchanged
 */
bool display_format_has_changed(void);

/**
 * @brief Format a message with a copy of the cached display (including custom characters)
 *
 * @param message_mcu_to_fpga_t* Pointer to the message
 */
void display_format_get(message_mcu_to_fpga_t* const message_to_format);
