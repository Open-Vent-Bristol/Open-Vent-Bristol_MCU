#ifndef VENTILATION_STATES_H
#define VENTILATION_STATES_H

#include "state/event.h"

#define MODE_COUNT (2u)

typedef enum
{
  STATE_HOMING,
  STATE_IDLE,
  STATE_INHALE,
  STATE_INHALE_PAUSE,
  STATE_EXHALE,
  STATE_EXHALE_PAUSE,
  STATE_FAULT,
  STATE_MAINTENANCE,
  STATE_UNRECOVERABLE,

  STATE_COUNT // Not a valid state
} ventilation_state_t;

typedef struct
{
  ventilation_state_t current_state;
  ventilation_state_t new_state;
  event_t event_mask;
} ventilation_state_transition_t;

const ventilation_state_transition_t s_transitions[MODE_COUNT][];

typedef void (*state_member_fn)(event_t* const);

typedef struct
{
  state_member_fn on_entry;
  state_member_fn on_run;
  state_member_fn on_exit;
} ventilation_state_def_t;

const ventilation_state_def_t s_state_definitions[STATE_COUNT] =

#endif /* VENTILATION_STATES_H */
