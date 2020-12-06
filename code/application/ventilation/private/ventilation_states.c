#include "ventilation_states.h"
#include "ventilation/ventilation.h"
#include "misc/utility.h"

/**
 * Implementation notes:
 * Values of current_state must be in contiguous blocks in order, as the array is inspected in order
 * starting from the index in s_transitions_index.  (event_t)0 will cause the search to stop,
 * so if present this should only occur at the end of a block.  Likewise the search will stop if the
 * value of .current_state exceeds the comparison state value.
 * You will need to pad each array to the same length.
 */
const ventilation_state_transition_t s_transitions[MODE_COUNT][] =
{
  [VENTILATION_MODE_AUTO] =
  {
    { STATE_HOMING,         STATE_IDLE,           EVENT_MOTOR_CURRENT_LIMIT | EVENT_ACTUATOR_LIMIT | EVENT_PRESSURE_LIMIT | EVENT_TIDAL_VOLUME_LIMIT | EVENT_PERIOD_LIMIT },
    { STATE_HOMING,         STATE_FAULT,          EVENT_PERIOD_LIMIT },
    { STATE_IDLE,           STATE_EXHALE_PAUSE,   EVENT_PERIOD_LIMIT },
    { STATE_IDLE,           STATE_FAULT,          (event_t)0 },
    { STATE_IDLE,           STATE_MAINTENANCE,    (event_t)0 },
    { STATE_IDLE,           STATE_UNRECOVERABLE,  (event_t)0 },
    { STATE_IDLE,           STATE_IDLE,           EVENT_STOP },
    { STATE_INHALE,         STATE_INHALE_PAUSE,   EVENT_MOTOR_CURRENT_LIMIT | EVENT_ACTUATOR_LIMIT | EVENT_PRESSURE_LIMIT | EVENT_TIDAL_VOLUME_LIMIT },
    { STATE_INHALE,         STATE_FAULT,          EVENT_PERIOD_LIMIT },
    { STATE_INHALE_PAUSE,   STATE_EXHALE,         EVENT_PERIOD_LIMIT },
    { STATE_EXHALE,         STATE_EXHALE_PAUSE,   EVENT_MOTOR_CURRENT_LIMIT | EVENT_ACTUATOR_LIMIT },
    { STATE_EXHALE,         STATE_FAULT,          EVENT_PERIOD_LIMIT },
    { STATE_EXHALE_PAUSE,   STATE_INHALE,         EVENT_PERIOD_LIMIT },
    { STATE_EXHALE_PAUSE,   STATE_IDLE,           EVENT_STOP },
    { STATE_FAULT,          STATE_MAINTENANCE,    (event_t)0 },
    { STATE_FAULT,          STATE_UNRECOVERABLE,  (event_t)0 },
    { STATE_MAINTENANCE,    STATE_INHALE,         (event_t)0 },
    { STATE_MAINTENANCE,    STATE_IDLE,           (event_t)0 },
    { STATE_MAINTENANCE,    STATE_FAULT,          (event_t)0 },
    { STATE_UNRECOVERABLE,  STATE_UNRECOVERABLE,  (event_t)0 },
  },
  [VENTILATION_MODE_SUPPORT] =
  {
    { STATE_HOMING,         STATE_IDLE,           EVENT_MOTOR_CURRENT_LIMIT | EVENT_ACTUATOR_LIMIT | EVENT_PRESSURE_LIMIT | EVENT_TIDAL_VOLUME_LIMIT | EVENT_PERIOD_LIMIT },
    { STATE_HOMING,         STATE_FAULT,          EVENT_PERIOD_LIMIT },
    { STATE_IDLE,           STATE_EXHALE_PAUSE,   EVENT_PERIOD_LIMIT },
    { STATE_IDLE,           STATE_FAULT,          (event_t)0 },
    { STATE_IDLE,           STATE_MAINTENANCE,    (event_t)0 },
    { STATE_IDLE,           STATE_UNRECOVERABLE,  (event_t)0 },
    { STATE_IDLE,           STATE_IDLE,           EVENT_STOP },
    { STATE_INHALE,         STATE_INHALE_PAUSE,   EVENT_MOTOR_CURRENT_LIMIT | EVENT_ACTUATOR_LIMIT | EVENT_PRESSURE_LIMIT | EVENT_TIDAL_VOLUME_LIMIT | EVENT_FLOW_25_PC_LIMIT },
    { STATE_INHALE,         STATE_FAULT,          EVENT_PERIOD_LIMIT },
    { STATE_INHALE_PAUSE,   STATE_EXHALE,         EVENT_PERIOD_LIMIT },
    { STATE_EXHALE,         STATE_EXHALE_PAUSE,   EVENT_MOTOR_CURRENT_LIMIT | EVENT_ACTUATOR_LIMIT },
    { STATE_EXHALE,         STATE_FAULT,          EVENT_PERIOD_LIMIT },
    { STATE_EXHALE_PAUSE,   STATE_INHALE,         EVENT_PRESSURE_DROP | EVENT_PERIOD_LIMIT },
    { STATE_EXHALE_PAUSE,   STATE_IDLE,           EVENT_STOP },
    { STATE_FAULT,          STATE_MAINTENANCE,    (event_t)0 },
    { STATE_FAULT,          STATE_UNRECOVERABLE,  (event_t)0 },
    { STATE_MAINTENANCE,    STATE_INHALE,         (event_t)0 },
    { STATE_MAINTENANCE,    STATE_IDLE,           (event_t)0 },
    { STATE_MAINTENANCE,    STATE_FAULT,          (event_t)0 },
    { STATE_UNRECOVERABLE,  STATE_UNRECOVERABLE,  (event_t)0 },
  }
};

static void STATE_HOMING_on_entry(event_t* const event_mask)
{
  (void)event_mask;
}

static void STATE_HOMING_on_run(event_t* const event_mask)
{
  (void)event_mask;
}

static void STATE_HOMING_on_exit(event_t* const event_mask)
{
  (void)event_mask;
}

static void STATE_IDLE_on_entry(event_t* const event_mask)
{
  (void)event_mask;
}

static void STATE_IDLE_on_run(event_t* const event_mask)
{
  (void)event_mask;
}

static void STATE_IDLE_on_exit(event_t* const event_mask)
{
  (void)event_mask;
}

static void STATE_INHALE_on_entry(event_t* const event_mask)
{
  (void)event_mask;
}

static void STATE_INHALE_on_run(event_t* const event_mask)
{
  (void)event_mask;
}

static void STATE_INHALE_on_exit(event_t* const event_mask)
{
  (void)event_mask;
}

static void STATE_INHALE_PAUSE_on_entry(event_t* const event_mask)
{
  (void)event_mask;
}

static void STATE_INHALE_PAUSE_on_run(event_t* const event_mask)
{
  (void)event_mask;
}

static void STATE_INHALE_PAUSE_on_exit(event_t* const event_mask)
{
  (void)event_mask;
}

static void STATE_EXHALE_on_entry(event_t* const event_mask)
{
  (void)event_mask;
}

static void STATE_EXHALE_on_run(event_t* const event_mask)
{
  (void)event_mask;
}

static void STATE_EXHALE_on_exit(event_t* const event_mask)
{
  (void)event_mask;
}

static void STATE_EXHALE_PAUSE_on_entry(event_t* const event_mask)
{
  (void)event_mask;
}

static void STATE_EXHALE_PAUSE_on_run(event_t* const event_mask)
{
  (void)event_mask;
}

static void STATE_EXHALE_PAUSE_on_exit(event_t* const event_mask)
{
  (void)event_mask;
}

static void STATE_FAULT_on_entry(event_t* const event_mask)
{
  (void)event_mask;
}

static void STATE_FAULT_on_run(event_t* const event_mask)
{
  (void)event_mask;
}

static void STATE_FAULT_on_exit(event_t* const event_mask)
{
  (void)event_mask;
}

static void STATE_MAINTENANCE_on_entry(event_t* const event_mask)
{
  (void)event_mask;
}

static void STATE_MAINTENANCE_on_run(event_t* const event_mask)
{
  (void)event_mask;
}

static void STATE_MAINTENANCE_on_exit(event_t* const event_mask)
{
  (void)event_mask;
}

static void STATE_UNRECOVERABLE_on_entry(event_t* const event_mask)
{
  (void)event_mask;
}

static void STATE_UNRECOVERABLE_on_run(event_t* const event_mask)
{
  (void)event_mask;
}

static void STATE_UNRECOVERABLE_on_exit(event_t* const event_mask)
{
  (void)event_mask;
}

#define STATE_DEF_MEMBERS(name) \
  [name] = { CONCAT(name, _on_entry), CONCAT(name, _on_run), CONCAT(name, _on_exit) }

const ventilation_state_def_t s_state_definitions[STATE_COUNT] =
{
  STATE_DEF_MEMBERS(STATE_HOMING),
  STATE_DEF_MEMBERS(STATE_IDLE),
  STATE_DEF_MEMBERS(STATE_INHALE),
  STATE_DEF_MEMBERS(STATE_INHALE_PAUSE),
  STATE_DEF_MEMBERS(STATE_EXHALE),
  STATE_DEF_MEMBERS(STATE_EXHALE_PAUSE),
  STATE_DEF_MEMBERS(STATE_FAULT),
  STATE_DEF_MEMBERS(STATE_MAINTENANCE),
  STATE_DEF_MEMBERS(STATE_UNRECOVERABLE)
};
