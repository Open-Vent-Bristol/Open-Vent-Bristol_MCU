// Copyright (c) 2021 <OpenVent-Bristol, Donald Robson>

#ifndef STATE_MACHINE_H
#define STATE_MACHINE_H

#include "state/system_event.h"
#include <stdint.h>
#include <stddef.h>

// The actual state machine objects are private and internal.  Memory will be allocated for this
// number
#define STATE_MACHINE_COUNT_MAX (2u)

// The maximum allowable number of states in a state machine
#define STATE_COUNT_MAX         (8u)

/**
 * @brief State machine handle type.  This must be instantiated as a pointer, and a pointer to that
 * pointer must be used when calling state_machine_init
 */
typedef struct state_machine_t state_machine_t;

typedef void (*state_member_fn)(enum system_event* const);

/**
 * A state machine is comprised of a series of states (state_definition_t) that each have functions
 * to describe what actions are performed on entry to the state, while the state is active, and on
 * exit from the state.
 */
typedef struct
{
  const state_member_fn on_entry;
  const state_member_fn on_run;
  const state_member_fn on_exit;
} state_definition_t;

/**
 * IMPORTANT NOTE ON IMPLEMENTATION:
 * In the state_definition_list_t type, the members must be array indexed by state number and there
 * must be no more than STATE_COUNT_MAX definitions
 */
typedef struct
{
  const state_definition_t* definitions_array_ptr;
  const size_t length;
} state_definition_list_t;

/**
 * Entry and exit of states is in accordance with transitions (state_transition_t) in response to
 * a system_event mask (which is in priority order with low bits serviced first).  When a system
 * event is flagged, the current state of the machine and the event will be checked to identify if
 * a transition should occur.
 */
typedef struct
{
  const int32_t old_state;
  const int32_t new_state;
  const enum system_event event_mask;
} state_transition_t;

/**
 * IMPORTANT NOTE ON IMPLEMENTATION:
 * In the state_transition_list_t type, the members must be arranged in ascending order of old_state
 * and there must be no more than INT8_MAX transitions
 */
typedef struct
{
  const state_transition_t* transtitions_array_ptr;
  const size_t length;
} state_transition_list_t;

/**
 *  @brief Initialise the state machine
 *
 * @param state_machine_ptr   Pointer to an externally instantiated pointer of this type.  If NULL,
 *                            the address of a private, internal object will be returned in this
 *                            pointer, otherwise no changes will occur.
 * @param initial_state       If state_machine_ptr NULL, the machine will be initialised with this
 *                            state, else this will be ignored.
 * @param definitions
 * @param transitions
 */
void state_machine_init(state_machine_t** state_machine_ptr, int8_t initial_state,
  const state_definition_list_t* definitions, const state_transition_list_t* transitions);

/**
 * @brief Assign a new transitions list to the state machine.  This might be used to change an
 * operating mode
 *
 * @param state_machine   State machine object pointer.
 * @param transitions
 */
void state_machine_set_transition_list(state_machine_t* state_machine,
  const state_transition_list_t* transitions);

/**
 * @brief Signal an event (or NO_EVENT) to the state_machine and call appropriate functions.
 * This function must be run frequently.
 *
 * @param state_machine State machine object pointer.  Event will be handled in the context of this
 *                      object.
 * @param event_mask    Event bits.  Bits that cause a transition will NOT be cleared automatically.
 */
void state_machine_run(state_machine_t* const state_machine, enum system_event* const event_mask);

#endif /* STATE_MACHINE_H */
