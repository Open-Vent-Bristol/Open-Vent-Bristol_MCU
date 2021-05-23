// Copyright (c) 2021 <OpenVent-Bristol, Donald Robson>

#ifndef DISPLAY_CONTROLLER_MOCK_H
#define DISPLAY_CONTROLLER_MOCK_H

#include "fff.h"
#include "fpga/display_controller.h"

#define FFF_GCC_FUNCTION_ATTRIBUTES

FAKE_VOID_FUNC(display_controller_init);

FAKE_VOID_FUNC(display_controller_set_override, enum display_override_index);

FAKE_VOID_FUNC(display_controller_set_line_2_override, enum display_override_line_2_index);

#define DISPLAY_CONTROLLER_MOCKS(x) \
  x(display_controller_init) \
  x(display_controller_set_override) \
  x(display_controller_set_line_2_override)

#endif /* DISPLAY_CONTROLLER_MOCK_H */
