// Copyright (c) 2021 <OpenVent-Bristol, Donald Robson>

#ifndef DISPLAY_CONTROLLER_H
#define DISPLAY_CONTROLLER_H

#include "fpga/fpga_api.h"

/**
 * @brief Initialise display control.
 * @note The display will not function until this is called.
 */
void display_controller_init(void);

/**
 * @brief Set a string override.  These are used to display help and calibration messages.
 * @note To unset an override, call with argument DISPLAY_OVERRIDE_NONE.
 *
 * @param index   See enum display_override_index
 */
void display_controller_set_override(enum display_override_index index);

/**
 * @brief Set a string override for line 2.  These are used to display help and calibration messages
 * @note To unset an override, call with argument DISPLAY_LINE_2_OVERRIDE_NONE.
 * @note Can also be used to set the progress bar length
 * @note No effect if main override function above is not first called with an argument other than
 * DISPLAY_OVERRIDE_NONE.
 *
 * @param index   See enum display_override_index
 */
void display_controller_set_line_2_override(enum display_override_line_2_index index);

#endif /* DISPLAY_CONTROLLER_H */
