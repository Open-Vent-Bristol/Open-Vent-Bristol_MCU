// Copyright (c) 2021 <OpenVent-Bristol, Donald Robson>

#include "state_machine.h"
#include "private/state_machine_priv.h"
#include "misc/util.h"
#include <string.h>
#include <stdbool.h>


TESTABLE state_machine_t s_machines[STATE_MACHINE_COUNT_MAX];

TESTABLE bool state_machine_valid(const state_machine_t* const state_machine)
{
  for (size_t i = 0u; i < ARRAY_LENGTH(s_machines); i++)
  {
    if (state_machine == &s_machines[i])
    {
      return true;
    }
  }

  return false;
}

void state_machine_init(state_machine_t** state_machine_ptr, int8_t initial_state,
  const state_definition_list_t* definitions, const state_transition_list_t* transitions)
{
  if ((state_machine_ptr != NULL) && (definitions != NULL) && (transitions != NULL))
  {
    if (*state_machine_ptr == NULL)
    {
      if ((definitions->length > STATE_COUNT_MAX) || (transitions->length > INT8_MAX))
      {
        // Should never occur - too many state definitions/transitions
        // TODO: log error
        return;
      }

      for (size_t i = 0u; i < STATE_MACHINE_COUNT_MAX; i++)
      {
        if (s_machines[i].definitions == NULL)
        {
          *state_machine_ptr = &s_machines[i];
          break;
        }
      }

      if (*state_machine_ptr == NULL)
      {
        // Should never occur - too many state machines
        // TODO: log error
        return;
      }

      state_machine_set_transition_list(*state_machine_ptr, transitions);
      (*state_machine_ptr)->definitions = definitions;

      // Set initial state and call its on_entry function
      (*state_machine_ptr)->current_state = initial_state;

      state_member_fn on_entry =
        (*state_machine_ptr)->definitions->definitions_array_ptr[initial_state].on_entry;

      if (on_entry != NULL)
      {
        system_event_mask_t no_event = NO_EVENT;
        on_entry(&no_event);
      }
    }
  }
}

void state_machine_set_transition_list(state_machine_t* state_machine,
  const state_transition_list_t* transitions)
{
  if (transitions != NULL)
  {
    if (transitions->length > INT8_MAX)
    {
      // Should never occur - too many state transitions
      // TODO: log error
      return;
    }

    if (state_machine_valid(state_machine))
    {
      state_machine->transitions = transitions;
      memset(state_machine->transition_index, -1, STATE_COUNT_MAX);

      // Populate member array transition_index, which contains search start indices
      for (size_t state = 0u; state < STATE_COUNT_MAX; state++)
      {
        for (
          size_t i = 0u; i < transitions->length; i++)
        {
          if (transitions->transtitions_array_ptr[i].old_state == state)
          {
            state_machine->transition_index[state] = i;
            break;
          }
        }
      }
    }
  }
}

void state_machine_run(state_machine_t* const state_machine, system_event_mask_t* const event_mask)
{
  if (state_machine_valid(state_machine) && (event_mask != NULL))
  {
    int32_t new_state = state_machine->current_state;

    if (*event_mask != NO_EVENT)
    {
      // Search for a permitted transition
      for (size_t i = (size_t)(state_machine->transition_index[state_machine->current_state]);
          i < state_machine->transitions->length;
          i++)
      {
        // End search if appropriate
        if (state_machine->transitions->transtitions_array_ptr[i].old_state
          != state_machine->current_state)
        {
          // No match
          break;
        }

        if (state_machine->transitions->transtitions_array_ptr[i].event_mask & *event_mask)
        {
          // Transition permitted
          new_state = state_machine->transitions->transtitions_array_ptr[i].new_state;
          break;
        }
      }
    }

    // Check if a state transition is required
    if (new_state != state_machine->current_state)
    {
      // Run the on_exit function for the current state
      state_member_fn on_exit =
        state_machine->definitions->definitions_array_ptr[state_machine->current_state].on_exit;

      if (on_exit != NULL)
      {
        on_exit(event_mask);
      }

      // Change state
      state_machine->current_state = new_state;

      // Run the on_entry function for the new state
      state_member_fn on_entry =
        state_machine->definitions->definitions_array_ptr[state_machine->current_state].on_entry;

      if (on_entry != NULL)
      {
        on_entry(event_mask);
      }
    }
    else
    {
      state_member_fn on_run =
        state_machine->definitions->definitions_array_ptr[state_machine->current_state].on_run;

      if (on_run != NULL)
      {
        on_run(event_mask);
      }
    }
  }
}
