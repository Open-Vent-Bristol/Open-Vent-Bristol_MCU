#ifndef VENTILATION_STATES_H
#define VENTILATION_STATES_H

#include "scheduler/state_machine.h"

enum ventilation_state
{
  VENTILATION_STATE_HOMING,
  VENTILATION_STATE_IDLE,
  VENTILATION_STATE_INHALE,
  VENTILATION_STATE_INHALE_PAUSE,
  VENTILATION_STATE_EXHALE,
  VENTILATION_STATE_EXHALE_PAUSE,
  VENTILATION_STATE_FAULT,
  VENTILATION_STATE_MAINTENANCE,
  VENTILATION_STATE_UNRECOVERABLE,

  VENTILATION_STATE_COUNT // Not a valid state
};

extern const state_definition_list_t s_state_definitions;

extern const state_transition_list_t s_pcv_transitions;
extern const state_transition_list_t s_psv_transitions;
extern const state_transition_list_t s_continuous_run_transitions;
extern const state_transition_list_t s_wait_at_home_transitions;

#endif /* VENTILATION_STATES_H */
