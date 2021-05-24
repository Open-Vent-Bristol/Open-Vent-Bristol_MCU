// Copyright (c) 2021 <OpenVent-Bristol, Donald Robson>

#ifndef STATE_MACHINE_PRIV_H
#define STATE_MACHINE_PRIV_H

#include "scheduler/state_machine.h"

typedef struct state_machine_t
{
  const state_definition_list_t* definitions;
  const state_transition_list_t* transitions;
  int8_t current_state;
  int8_t transition_index[STATE_COUNT_MAX];
} state_machine_t;

/**
 * @brief Signal an event (or NO_EVENT) to all state_machine objects and call appropriate functions.
 * @note This function must be run frequently by the dispatcher.
 * @note All state machines will receive the same event_mask value.  The state functions can clear
 * bits without affecting this behaviour.  The cleared bits will be mirrored in the pointer
 * after all state machines have processed.
 *
 * @param event_mask    Event bits.  Bits that cause a transition will NOT be cleared automatically.
 */
void state_machine_run_all(system_event_mask_t* const event_mask);

#endif /* STATE_MACHINE_PRIV_H */
