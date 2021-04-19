#include "ventilation.h"
#include "private/ventilation_states.h"
#include "state/state_machine.h"
#include <stddef.h>

extern const state_transition_list_t s_pcv_transitions;
extern const state_transition_list_t s_psv_transitions;

// static ventilation_mode_t s_current_mode = VENTILATION_MODE_CONTROL;
// static ventilation_mode_t s_next_mode = VENTILATION_MODE_CONTROL;

void ventilation_init(void)
{

}

void ventilation_set_mode(ventilation_mode_t mode)
{
  // s_next_mode = mode;
}

void ventilation_run(enum system_event* const event_mask)
{

}

void ventilation_stop(void)
{
  // enum system_event stop_event = EVENT_STOP;
  // state_machine_run(&stop_event);
}
