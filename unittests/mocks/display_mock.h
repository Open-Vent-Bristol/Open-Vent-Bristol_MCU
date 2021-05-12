// Copyright (c) 2021 <OpenVent-Bristol, Donald Robson>

#ifndef DISPLAY_MOCK_H
#define DISPLAY_MOCK_H

#include "fff.h"
#include "fpga/display.h"

#define FFF_GCC_FUNCTION_ATTRIBUTES

FAKE_VOID_FUNC(display_format_tidal_volume, uint16_t);

FAKE_VOID_FUNC(display_format_peak_flow, uint16_t);

FAKE_VOID_FUNC(display_format_respiration_rate, uint8_t);

FAKE_VOID_FUNC(display_format_percent_o2, uint8_t);

FAKE_VOID_FUNC(display_format_battery_gauge, uint8_t);

FAKE_VOID_FUNC(display_format_battery_fault);

FAKE_VOID_FUNC(display_format_pressure_bar, uint16_t, uint16_t);

FAKE_VOID_FUNC(display_format_progress_bar, uint8_t);

FAKE_VOID_FUNC(display_string, const char* const);

FAKE_VALUE_FUNC(bool, display_has_changed);

FAKE_VOID_FUNC(display_get, message_mcu_to_fpga_t* const);

#define DISPLAY_MOCKS(x) \
  x(display_format_tidal_volume) \
  x(display_format_peak_flow) \
  x(display_format_respiration_rate) \
  x(display_format_percent_o2) \
  x(display_format_battery_gauge) \
  x(display_format_battery_fault) \
  x(display_format_pressure_bar) \
  x(display_format_progress_bar) \
  x(display_string) \
  x(display_has_changed) \
  x(display_get)

#endif /* DISPLAY_MOCK_H */
