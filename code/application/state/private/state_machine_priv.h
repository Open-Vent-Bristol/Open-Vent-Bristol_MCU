// Copyright (c) 2021 <OpenVent-Bristol, Donald Robson>

#ifndef STATE_MACHINE_PRIV_H
#define STATE_MACHINE_PRIV_H

#include "state/state_machine.h"

typedef struct state_machine_t
{
  const state_definition_list_t* definitions;
  const state_transition_list_t* transitions;
  int8_t current_state;
  int8_t transition_index[STATE_COUNT_MAX];
} state_machine_t;

#endif /* STATE_MACHINE_PRIV_H */
