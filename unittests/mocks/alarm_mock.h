// Copyright (c) 2021 <OpenVent-Bristol, Donald Robson>

#ifndef ALARM_MOCK_H
#define ALARM_MOCK_H

#include "alarm/alarm.h"
#include "fff.h"

#define FFF_GCC_FUNCTION_ATTRIBUTES

FAKE_VOID_FUNC(alarm_init);

FAKE_VOID_FUNC(alarm_mode, int32_t);

#define ALARM_MOCKS(x) \
  x(alarm_init) \
  x(alarm_mode)

#endif /* ALARM_MOCK_H */
