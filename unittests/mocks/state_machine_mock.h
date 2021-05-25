// Copyright (c) 2021 <OpenVent-Bristol, Donald Robson>

#ifndef STATE_MACHINE_MOCK_H
#define STATE_MACHINE_MOCK_H

#include "fff.h"
#include "scheduler/state_machine.h"
#include "scheduler/private/state_machine_priv.h"

#define FFF_GCC_FUNCTION_ATTRIBUTES

FAKE_VOID_FUNC(state_machine_init, state_machine_t**, int8_t,
  const state_definition_list_t*, const state_transition_list_t*);

FAKE_VOID_FUNC(state_machine_set_transition_list, state_machine_t*,
  const state_transition_list_t*);

FAKE_VOID_FUNC(state_machine_run_all, system_event_mask_t* const);

#define STATE_MACHINE_MOCKS(x) \
  x(state_machine_init) \
  x(state_machine_set_transition_list) \
  x(state_machine_run_all)

#endif /* STATE_MACHINE_MOCK_H */
