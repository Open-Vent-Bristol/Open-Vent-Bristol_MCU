#include "ventilation.h"
#include "private/ventilation_priv.h"
#include "private/ventilation_states.h"
#include "actuator/actuator.h"
#include "fpga/fpga_api.h"
#include "scheduler/dispatcher.h"
#include "scheduler/state_machine.h"
#include <misc/util.h>
#include <stddef.h>

TESTABLE enum fpga_operating_mode s_current_mode = FPGA_MODE_STANDBY;
TESTABLE enum fpga_operating_mode s_next_mode = FPGA_MODE_STANDBY;
TESTABLE state_machine_t* s_ventilation_state_machine;

/**
 * @brief Change the ventilation mode.  The mode will take effect the next time the actuator reaches
 * the home position.
 *
 * @param mode  See enum fpga_operating_mode
 */
TESTABLE void ventilation_set_target_mode(int32_t mode)
{
  if (mode != s_next_mode)
  {
    s_next_mode = mode;
  }
}

void ventilation_init(void)
{
  dispatcher_bind(1u << EV_OP_MODE_CHANGE, ventilation_set_target_mode);

  state_machine_init(
    &s_ventilation_state_machine, VENTILATION_STATE_HOMING,
    &s_state_definitions, &s_wait_at_home_transitions);
}

void ventilation_set_to_target_mode(void)
{
  if (s_next_mode != s_current_mode)
  {
    switch (s_next_mode)
    {
    case FPGA_MODE_OFF:
    case FPGA_MODE_STANDBY:
    case FPGA_MODE_STOP:
      // Return actuator to home and wait there
      state_machine_set_transition_list(s_ventilation_state_machine, &s_wait_at_home_transitions);
      break;

    case FPGA_MODE_PRESSURE_CONTROL:
      // PCV - triggers on timeout according to the I-E ratio and resp. rate
      state_machine_set_transition_list(s_ventilation_state_machine, &s_pcv_transitions);
      break;

    case FPGA_MODE_PRESSURE_SUPPORT:
      // PSV - triggers on pressure drop and ends at the lower flow threshold
      state_machine_set_transition_list(s_ventilation_state_machine, &s_psv_transitions);
      break;

    case FPGA_MODE_RUN:
      // Cycling actuator during calibration
      state_machine_set_transition_list(s_ventilation_state_machine, &s_continuous_run_transitions);
      break;

    default:
      // Invalid mode
      s_next_mode = s_current_mode;
      return;
    }

    s_current_mode = s_next_mode;
  }
}
