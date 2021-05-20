// Copyright (c) 2021 <OpenVent-Bristol, Donald Robson>

#ifndef ACTUATOR_MOCK_H
#define ACTUATOR_MOCK_H

#include "actuator/actuator.h"
#include "fff.h"

#define FFF_GCC_FUNCTION_ATTRIBUTES

FAKE_VOID_FUNC(actuator_init, float, float, float);

FAKE_VOID_FUNC(actuator_stop);

FAKE_VOID_FUNC(actuator_constant_speed_home, uint8_t);

FAKE_VOID_FUNC(actuator_constant_speed_push, uint8_t);

FAKE_VOID_FUNC(actuator_pid_control_push, int16_t);

#define ACTUATOR_MOCKS(x) \
  x(actuator_init) \
  x(actuator_stop) \
  x(actuator_constant_speed_home) \
  x(actuator_constant_speed_push) \
  x(actuator_pid_control_push)

#endif /* ACTUATOR_MOCK_H */
