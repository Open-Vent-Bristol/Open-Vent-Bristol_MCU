#include "ventilation_states.h"
#include "ventilation/ventilation.h"
#include "state/state_machine.h"
#include "misc/util.h"

/**
 * IMPLEMENTATION note:
 * Values of old_state must be in contiguous blocks in ascending order
 */
static const state_transition_t s_pcv_transitions_array[] =
{
  { VENTILATION_STATE_HOMING,         VENTILATION_STATE_IDLE,           NO_EVENT },
  { VENTILATION_STATE_HOMING,         VENTILATION_STATE_FAULT,          NO_EVENT },
  { VENTILATION_STATE_IDLE,           VENTILATION_STATE_EXHALE_PAUSE,   NO_EVENT },
  { VENTILATION_STATE_IDLE,           VENTILATION_STATE_FAULT,          NO_EVENT },
  { VENTILATION_STATE_IDLE,           VENTILATION_STATE_MAINTENANCE,    NO_EVENT },
  { VENTILATION_STATE_IDLE,           VENTILATION_STATE_UNRECOVERABLE,  NO_EVENT },
  { VENTILATION_STATE_IDLE,           VENTILATION_STATE_IDLE,           NO_EVENT },
  { VENTILATION_STATE_INHALE,         VENTILATION_STATE_INHALE_PAUSE,   NO_EVENT },
  { VENTILATION_STATE_INHALE,         VENTILATION_STATE_FAULT,          NO_EVENT },
  { VENTILATION_STATE_INHALE_PAUSE,   VENTILATION_STATE_EXHALE,         NO_EVENT },
  { VENTILATION_STATE_EXHALE,         VENTILATION_STATE_EXHALE_PAUSE,   NO_EVENT },
  { VENTILATION_STATE_EXHALE,         VENTILATION_STATE_FAULT,          NO_EVENT },
  { VENTILATION_STATE_EXHALE_PAUSE,   VENTILATION_STATE_INHALE,         NO_EVENT },
  { VENTILATION_STATE_EXHALE_PAUSE,   VENTILATION_STATE_IDLE,           NO_EVENT },
  { VENTILATION_STATE_FAULT,          VENTILATION_STATE_MAINTENANCE,    NO_EVENT },
  { VENTILATION_STATE_FAULT,          VENTILATION_STATE_UNRECOVERABLE,  NO_EVENT },
  { VENTILATION_STATE_MAINTENANCE,    VENTILATION_STATE_INHALE,         NO_EVENT },
  { VENTILATION_STATE_MAINTENANCE,    VENTILATION_STATE_IDLE,           NO_EVENT },
  { VENTILATION_STATE_MAINTENANCE,    VENTILATION_STATE_FAULT,          NO_EVENT },
  { VENTILATION_STATE_UNRECOVERABLE,  VENTILATION_STATE_UNRECOVERABLE,  NO_EVENT },
};

static const state_transition_t s_psv_transitions_array[] =
{
  { VENTILATION_STATE_HOMING,         VENTILATION_STATE_IDLE,           NO_EVENT },
  { VENTILATION_STATE_HOMING,         VENTILATION_STATE_FAULT,          NO_EVENT },
  { VENTILATION_STATE_IDLE,           VENTILATION_STATE_EXHALE_PAUSE,   NO_EVENT },
  { VENTILATION_STATE_IDLE,           VENTILATION_STATE_FAULT,          NO_EVENT },
  { VENTILATION_STATE_IDLE,           VENTILATION_STATE_MAINTENANCE,    NO_EVENT },
  { VENTILATION_STATE_IDLE,           VENTILATION_STATE_UNRECOVERABLE,  NO_EVENT },
  { VENTILATION_STATE_IDLE,           VENTILATION_STATE_IDLE,           NO_EVENT },
  { VENTILATION_STATE_INHALE,         VENTILATION_STATE_INHALE_PAUSE,   NO_EVENT },
  { VENTILATION_STATE_INHALE,         VENTILATION_STATE_FAULT,          NO_EVENT },
  { VENTILATION_STATE_INHALE_PAUSE,   VENTILATION_STATE_EXHALE,         NO_EVENT },
  { VENTILATION_STATE_EXHALE,         VENTILATION_STATE_EXHALE_PAUSE,   NO_EVENT },
  { VENTILATION_STATE_EXHALE,         VENTILATION_STATE_FAULT,          NO_EVENT },
  { VENTILATION_STATE_EXHALE_PAUSE,   VENTILATION_STATE_INHALE,         NO_EVENT },
  { VENTILATION_STATE_EXHALE_PAUSE,   VENTILATION_STATE_IDLE,           NO_EVENT },
  { VENTILATION_STATE_FAULT,          VENTILATION_STATE_MAINTENANCE,    NO_EVENT },
  { VENTILATION_STATE_FAULT,          VENTILATION_STATE_UNRECOVERABLE,  NO_EVENT },
  { VENTILATION_STATE_MAINTENANCE,    VENTILATION_STATE_INHALE,         NO_EVENT },
  { VENTILATION_STATE_MAINTENANCE,    VENTILATION_STATE_IDLE,           NO_EVENT },
  { VENTILATION_STATE_MAINTENANCE,    VENTILATION_STATE_FAULT,          NO_EVENT },
  { VENTILATION_STATE_UNRECOVERABLE,  VENTILATION_STATE_UNRECOVERABLE,  NO_EVENT },
};

const state_transition_list_t s_pcv_transitions =
{
  .transtitions_array_ptr = s_pcv_transitions_array,
  .length = sizeof(s_pcv_transitions_array)
};

const state_transition_list_t s_psv_transitions =
{
  .transtitions_array_ptr = s_psv_transitions_array,
  .length = sizeof(s_psv_transitions_array)
};


static void VENTILATION_STATE_HOMING_on_entry(enum system_event* const event_mask)
{
  (void)event_mask;
}

static void VENTILATION_STATE_HOMING_on_run(enum system_event* const event_mask)
{
  (void)event_mask;
}

static void VENTILATION_STATE_HOMING_on_exit(enum system_event* const event_mask)
{
  (void)event_mask;
}

static void VENTILATION_STATE_IDLE_on_entry(enum system_event* const event_mask)
{
  (void)event_mask;
}

static void VENTILATION_STATE_IDLE_on_run(enum system_event* const event_mask)
{
  (void)event_mask;
}

static void VENTILATION_STATE_IDLE_on_exit(enum system_event* const event_mask)
{
  (void)event_mask;
}

static void VENTILATION_STATE_INHALE_on_entry(enum system_event* const event_mask)
{
  (void)event_mask;
}

static void VENTILATION_STATE_INHALE_on_run(enum system_event* const event_mask)
{
  (void)event_mask;
}

static void VENTILATION_STATE_INHALE_on_exit(enum system_event* const event_mask)
{
  (void)event_mask;
}

static void VENTILATION_STATE_INHALE_PAUSE_on_entry(enum system_event* const event_mask)
{
  (void)event_mask;
}

static void VENTILATION_STATE_INHALE_PAUSE_on_run(enum system_event* const event_mask)
{
  (void)event_mask;
}

static void VENTILATION_STATE_INHALE_PAUSE_on_exit(enum system_event* const event_mask)
{
  (void)event_mask;
}

static void VENTILATION_STATE_EXHALE_on_entry(enum system_event* const event_mask)
{
  (void)event_mask;
}

static void VENTILATION_STATE_EXHALE_on_run(enum system_event* const event_mask)
{
  (void)event_mask;
}

static void VENTILATION_STATE_EXHALE_on_exit(enum system_event* const event_mask)
{
  (void)event_mask;
}

static void VENTILATION_STATE_EXHALE_PAUSE_on_entry(enum system_event* const event_mask)
{
  (void)event_mask;
}

static void VENTILATION_STATE_EXHALE_PAUSE_on_run(enum system_event* const event_mask)
{
  (void)event_mask;
}

static void VENTILATION_STATE_EXHALE_PAUSE_on_exit(enum system_event* const event_mask)
{
  (void)event_mask;
}

static void VENTILATION_STATE_FAULT_on_entry(enum system_event* const event_mask)
{
  (void)event_mask;
}

static void VENTILATION_STATE_FAULT_on_run(enum system_event* const event_mask)
{
  (void)event_mask;
}

static void VENTILATION_STATE_FAULT_on_exit(enum system_event* const event_mask)
{
  (void)event_mask;
}

static void VENTILATION_STATE_MAINTENANCE_on_entry(enum system_event* const event_mask)
{
  (void)event_mask;
}

static void VENTILATION_STATE_MAINTENANCE_on_run(enum system_event* const event_mask)
{
  (void)event_mask;
}

static void VENTILATION_STATE_MAINTENANCE_on_exit(enum system_event* const event_mask)
{
  (void)event_mask;
}

static void VENTILATION_STATE_UNRECOVERABLE_on_entry(enum system_event* const event_mask)
{
  (void)event_mask;
}

static void VENTILATION_STATE_UNRECOVERABLE_on_run(enum system_event* const event_mask)
{
  (void)event_mask;
}

static void VENTILATION_STATE_UNRECOVERABLE_on_exit(enum system_event* const event_mask)
{
  (void)event_mask;
}

#define STATE_DEF_MEMBERS(name) \
  [name] = { name##_on_entry, name##_on_run, name##_on_exit }

const state_definition_t s_state_definitions[VENTILATION_STATE_COUNT] =
{
  STATE_DEF_MEMBERS(VENTILATION_STATE_HOMING),
  STATE_DEF_MEMBERS(VENTILATION_STATE_IDLE),
  STATE_DEF_MEMBERS(VENTILATION_STATE_INHALE),
  STATE_DEF_MEMBERS(VENTILATION_STATE_INHALE_PAUSE),
  STATE_DEF_MEMBERS(VENTILATION_STATE_EXHALE),
  STATE_DEF_MEMBERS(VENTILATION_STATE_EXHALE_PAUSE),
  STATE_DEF_MEMBERS(VENTILATION_STATE_FAULT),
  STATE_DEF_MEMBERS(VENTILATION_STATE_MAINTENANCE),
  STATE_DEF_MEMBERS(VENTILATION_STATE_UNRECOVERABLE)
};
