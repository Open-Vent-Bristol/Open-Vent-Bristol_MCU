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
void display_format_pressure_bar(uint16_t pressure_cmH2O, uint16_t peak_pressure_cmH2O);

/**
 * Check if the cached display has been changed (to avoid needing to call display_get)
 *
 * @return true Cached version of display has been updated
 * @return false Cached version of display is unchanged
 */
bool display_has_changed(void);

/**
 * Format a message with a copy of the cached display (including custom characters)
 *
 * @param message_mcu_to_fpga_t* Pointer to the message
 */
void display_get(message_mcu_to_fpga_t* const message_to_format);
