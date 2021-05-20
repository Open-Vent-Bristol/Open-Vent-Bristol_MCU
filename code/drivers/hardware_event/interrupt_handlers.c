// Copyright (c) 2021 <OpenVent-Bristol, Donald Robson>

#include "actuator/actuator.h"
#include "board/board.h"
#include "interrupt_handlers.h"
#include "scheduler/dispatcher.h"
#include "scheduler/timer.h"
#include "misc/util.h"
#include <stdbool.h>
#include <stdint.h>

#define FIFTY_MILLISECONDS (50)

TESTABLE timer_t s_power_debounce_timer =
{
  .type = TIMER_TYPE_ONE_SHOT,
  .initial_ticks = FIFTY_MILLISECONDS,
  .remaining_ticks = -1,
  .events_signalled = EV_POWER_DEBOUNCE_END,
  .unique_id = TIMER_POWER_DEBOUNCE
};

TESTABLE timer_t s_limit_debounce_timer =
{
  .type = TIMER_TYPE_ONE_SHOT,
  .initial_ticks = FIFTY_MILLISECONDS,
  .remaining_ticks = -1,
  .events_signalled = EV_LIMIT_DEBOUNCE_END,
  .unique_id = TIMER_LIMIT_DEBOUNCE
};

TESTABLE bool s_power_button_enabled = true;
TESTABLE bool s_limit_switches_enabled = true;

TESTABLE void debounce_end(int32_t timer_id)
{
  if (timer_id == TIMER_POWER_DEBOUNCE)
  {
    s_power_button_enabled = true;
  }
  else if (timer_id == TIMER_LIMIT_DEBOUNCE)
  {
    s_limit_switches_enabled = true;
  }
}

void interrupt_init(void)
{
  dispatcher_bind((1u << EV_LIMIT_DEBOUNCE_END) | (1u << EV_POWER_DEBOUNCE_END), debounce_end);
  timer_attach(&s_power_debounce_timer);
  timer_attach(&s_limit_debounce_timer);
}

/**
 * @brief ISR for PB_SEL, triggered when the power button is pushed
 */
void EXTI0_IRQHandler(void)
{
  LL_EXTI_ClearFlag_0_31(1u << PB_SEL_PIN);
  s_power_button_enabled = false;
  timer_reset(&s_power_debounce_timer);

  // TODO - power management
}

/**
 * @brief ISR for FPGA_READY, triggered when the FPGA is ready to communicate with the MCU
 */
void EXTI1_IRQHandler(void)
{
  LL_EXTI_ClearFlag_0_31(1u << FPGA_READY_PIN);
  dispatcher_signal_event_mask(1u << EV_FPGA_READY, 0);
}

/**
 * ISR for SW_END, triggered when the actuator hits the bottom switch.  This should not
 * happen in normal use.  Indicates that the bag has failed (or is not fitted).  Also indicates the
 * motor should be stopped (handled immediately).
 * ISR for SW_HOME, triggered when the actuator hits the home switch.  Indicates the motor should be
 * stopped (handled immediately).
 * ISR for SW_BACKUP, triggered when the actuator hits the backup home switch.  Indicates the
 * motor should be stopped (handled immediately) and that the home switch has failed.
 * ISR for nMOTOR_FAULT_A and nMOTOR_FAULT_B, riggered when the motor controller IC develops or
 * senses a fault.
 */
void EXTI9_5_IRQHandler(void)
{
  if ((LL_EXTI_IsActiveFlag_0_31(1u << SW_BACKUP_PIN)) |
      (LL_EXTI_IsActiveFlag_0_31(1u << SW_HOME_PIN)) |
      (LL_EXTI_IsActiveFlag_0_31(1u << SW_END_PIN)))
  {
    actuator_stop();
    LL_EXTI_ClearFlag_0_31((1u << SW_BACKUP_PIN) | (1u << SW_HOME_PIN) | (1u << SW_END_PIN));
    s_limit_switches_enabled = false;
    timer_reset(&s_limit_debounce_timer);

    // TODO - mechanical fault monitoring
  }
  else if (LL_EXTI_IsActiveFlag_0_31(1u << MOTOR_FAULT_A_PIN))
  {
    LL_EXTI_ClearFlag_0_31(1u << MOTOR_FAULT_A_PIN);

    // TODO - what is this for?
  }
  else if (LL_EXTI_IsActiveFlag_0_31(1u << MOTOR_FAULT_B_PIN))
  {
    LL_EXTI_ClearFlag_0_31(1u << MOTOR_FAULT_B_PIN);

    // TODO - what is this for?
  }
}

/**
 * @brief ISR for nPRESSURE_PATIENT_ALERT, triggered when... something.
 */
void EXTI15_10_IRQHandler(void)
{
  LL_EXTI_ClearFlag_0_31(1u << PRESSURE_ALERT_PIN);

  // TODO - what is this for?
}
