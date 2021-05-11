// Copyright (c) 2021 <OpenVent-Bristol, Donald Robson>

#ifndef DISPATCHER_MOCK_H
#define DISPATCHER_MOCK_H

#include "scheduler/dispatcher.h"
#include "fff.h"

#define FFF_GCC_FUNCTION_ATTRIBUTES

DEFINE_FFF_GLOBALS;

FAKE_VOID_FUNC(dispatcher_signal_event_mask, system_event_mask_t, int32_t);

FAKE_VOID_FUNC(dispatcher_bind, system_event_mask_t, system_event_callback);

FAKE_VOID_FUNC(dispatcher_bind_all, const system_event_callback*, size_t);

FAKE_VOID_FUNC(dispatcher_service);

#define DISPATCHER_MOCKS(x) \
  x(dispatcher_signal_event_mask) \
  x(dispatcher_bind) \
  x(dispatcher_bind_all) \
  x(dispatcher_service)

#endif /* DISPATCHER_MOCK_H */
