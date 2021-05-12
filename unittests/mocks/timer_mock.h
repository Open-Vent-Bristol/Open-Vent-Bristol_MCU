// Copyright (c) 2021 <OpenVent-Bristol, Donald Robson>

#ifndef TIMER_MOCK_H
#define TIMER_MOCK_H

#include "scheduler/timer.h"
#include "fff.h"

#define FFF_GCC_FUNCTION_ATTRIBUTES

FAKE_VOID_FUNC(timer_attach, timer_t*);
FAKE_VOID_FUNC(timer_reset, timer_t*);
FAKE_VOID_FUNC(timer_stop, timer_t*);

#define TIMER_MOCKS(x) \
  x(timer_attach) \
  x(timer_reset) \
  x(timer_stop)

#endif /* TIMER_MOCK_H */
