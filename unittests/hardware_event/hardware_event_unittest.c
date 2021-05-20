// Copyright (c) 2021 <OpenVent-Bristol, Donald Robson>

#include "unity.h"
#include "unity_fixture.h"
#include "test_macros.h"
#include "hardware_event/interrupt_handlers.h"
#include "actuator_mock.h"
#include "board_mock.h"
#include "dispatcher_mock.h"
#include "timer_mock.h"
#include <stdbool.h>

extern timer_t s_power_debounce_timer;
extern timer_t s_limit_debounce_timer;
extern bool s_power_button_enabled;
extern bool s_limit_switches_enabled;

extern void debounce_end(int32_t timer_id);

extern void EXTI0_IRQHandler(void);
extern void EXTI1_IRQHandler(void);
extern void EXTI9_5_IRQHandler(void);

uint32_t return_arg_val(uint32_t arg)
{
  return arg;
}

TEST_GROUP(hardware_event_tests);

TEST_SETUP(hardware_event_tests)
{
  ACTUATOR_MOCKS(RESET_FAKE);
  BOARD_MOCKS(RESET_FAKE);
  DISPATCHER_MOCKS(RESET_FAKE);
  TIMER_MOCKS(RESET_FAKE);
}

TEST_TEAR_DOWN(hardware_event_tests)
{}

TEST(hardware_event_tests, power_button_and_switches_default_on)
{
  TEST_ASSERT(s_power_button_enabled);
  TEST_ASSERT(s_limit_switches_enabled);
}

TEST(hardware_event_tests, init_attaches_debounce_timers)
{
  interrupt_init();
  TEST_ASSERT_EQUAL_INT(2u, timer_attach_fake.call_count);
  TEST_ASSERT_VALUE_IN_ARRAY(&s_power_debounce_timer, timer_attach_fake.arg0_history);
  TEST_ASSERT_VALUE_IN_ARRAY(&s_limit_debounce_timer, timer_attach_fake.arg0_history);
}

TEST(hardware_event_tests, init_binds_events)
{
  interrupt_init();
  TEST_ASSERT_EQUAL_INT(1u, dispatcher_bind_fake.call_count);
  TEST_ASSERT_EQUAL_INT((1u << EV_LIMIT_DEBOUNCE_END) | (1u << EV_POWER_DEBOUNCE_END),
    dispatcher_bind_fake.arg0_val);
}

TEST(hardware_event_tests, debounce_end_enables_power_button)
{
  s_power_button_enabled = false;

  debounce_end(TIMER_POWER_DEBOUNCE);
  TEST_ASSERT(s_power_button_enabled);
}

TEST(hardware_event_tests, debounce_end_enables_limit_switches)
{
  s_limit_switches_enabled = false;

  debounce_end(TIMER_LIMIT_DEBOUNCE);
  TEST_ASSERT(s_limit_switches_enabled);
}

TEST(hardware_event_tests, power_button_isr_debounced)
{
  EXTI0_IRQHandler();
  TEST_ASSERT_FALSE(s_power_button_enabled);
  TEST_ASSERT_EQUAL_INT(1u, timer_reset_fake.call_count);
  TEST_ASSERT_EQUAL_PTR(&s_power_debounce_timer, timer_reset_fake.arg0_val);
}

TEST(hardware_event_tests, limit_switches_isr_debounced)
{
  LL_EXTI_IsActiveFlag_0_31_fake.custom_fake = return_arg_val;

  EXTI9_5_IRQHandler();
  TEST_ASSERT_FALSE(s_limit_switches_enabled);
  TEST_ASSERT_EQUAL_INT(1u, timer_reset_fake.call_count);
  TEST_ASSERT_EQUAL_PTR(&s_limit_debounce_timer, timer_reset_fake.arg0_val);
}

TEST(hardware_event_tests, limit_switches_isr_stops_actuator)
{
  LL_EXTI_IsActiveFlag_0_31_fake.custom_fake = return_arg_val;

  EXTI9_5_IRQHandler();
  TEST_ASSERT_EQUAL_INT(1u, actuator_stop_fake.call_count);
}

TEST(hardware_event_tests, fpga_ready_isr_signals_event)
{
  EXTI1_IRQHandler();
  TEST_ASSERT_EQUAL_INT(1u, dispatcher_signal_event_mask_fake.call_count);
  TEST_ASSERT_EQUAL_INT(1u << EV_FPGA_READY, dispatcher_signal_event_mask_fake.arg0_val);
}
