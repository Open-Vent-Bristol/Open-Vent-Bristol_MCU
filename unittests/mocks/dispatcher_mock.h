// Copyright (c) 2021 <OpenVent-Bristol, Donald Robson>

#ifndef DISPATCHER_MOCK_H
#define DISPATCHER_MOCK_H

#include "scheduler/dispatcher.h"
#include "fff.h"

#define FFF_GCC_FUNCTION_ATTRIBUTES

DEFINE_FFF_GLOBALS;

FAKE_VOID_FUNC(dispatcher_signal_event_mask, enum system_event, int32_t);

FAKE_VOID_FUNC(dispatcher_bind, enum system_event, system_event_callback);

FAKE_VOID_FUNC(dispatcher_bind_all, const system_event_callback*, size_t);

FAKE_VOID_FUNC(dispatcher_service);

#endif /* DISPATCHER_MOCK_H */
