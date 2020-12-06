#include "ventilation.h"
#include "private/ventilation_states.h"
#include <stddef.h>

static size_t s_transitions_index[MODE_COUNT][STATE_COUNT] = {0};

static ventilation_mode_t s_current_mode = VENTILATION_MODE_AUTO;
static ventilation_mode_t s_next_mode = VENTILATION_MODE_AUTO;
static ventilation_state_t s_state = STATE_HOMING;

void ventilation_init(void)
{
  // Populate array of s_transitions search start indices
  // TODO: could this be done statically?
  for (size_t mode = 0u; mode < MODE_COUNT; i++)
  {
    for (size_t state = 0u; state < STATE_COUNT; state++)
    {
      for (
        size_t i = 0u; i < (sizeof(s_transitions) / sizeof(s_transitions[0][0]) / MODE_COUNT); i++)
      {
        if (s_transitions[mode][i].current_state == (ventilation_state_t)state)
        {
          s_transitions_index[mode][state] = i;
          break;
        }
      }
    }
  }
}

void ventilation_set_mode(ventilation_mode_t mode)
{
  s_next_mode = mode;
}

void ventilation_run(event_t* const event_mask)
{
  ventilation_state_t new_state = s_state;

  // Search for a permitted transition
  for (size_t i = s_transitions_index[s_state];
       i < (sizeof(s_transitions[0]) / sizeof(s_transitions[0][0]));
       i++)
  {
    // End search if appropriate
    if (s_transitions[i].current_state > s_state)
    {
      // No match
      break;
    }

    if (s_transitions[i].event_mask == (ventilation_mode_t)0)
    {
      // Explicit stop
      break;
    }

    if (s_transitions[i].event_mask & *event_mask)
    {
      // Transition permitted
      new_state = s_transitions[i].new_state;
      break;
    }
  }

  // Check if a state transition is required
  if (new_state != s_state)
  {
    // Run the on_exit function for the current state
    s_state_definitions[s_state].on_exit(event_mask);

    // Change state
    s_state = new_state;

    // Run the on_entry function for the new state
    s_state_definitions[s_state].on_entry(event_mask);
  }
  else
  {
    s_state_definitions[s_state].on_run(event_mask);
  }
}

void ventilation_stop(void)
{
  event_t stop_event = EVENT_STOP;
  ventilation_run(&stop_event);
}
