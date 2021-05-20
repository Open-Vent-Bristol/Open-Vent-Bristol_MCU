// Copyright (c) 2021 <OpenVent-Bristol, Donald Robson>

#include "unity.h"
#include "unity_fixture.h"
#include "scheduler/dispatcher.h"
#include "board_mock.h"
#include <string.h>

#include "fff.h"

#define FFF_GCC_FUNCTION_ATTRIBUTES
DEFINE_FFF_GLOBALS;

void test_function(int32_t arg);
FAKE_VOID_FUNC(test_function, int32_t);
void test_function_caller(int32_t arg)
{
  test_function(arg);
}

extern system_event_callback s_bindings[EV_NUMBER_EVENTS];
extern int32_t s_signalled_args[EV_NUMBER_EVENTS];
extern system_event_mask_t s_unhandled_events;
extern uint32_t s_dispatch_errors;

TEST_GROUP(dispatcher_tests);

TEST_SETUP(dispatcher_tests)
{
  memset(s_bindings, 0, sizeof(s_bindings));
  memset(s_signalled_args, 0, sizeof(s_signalled_args));
  s_unhandled_events = 0u;
  s_dispatch_errors = 0u;

  RESET_FAKE(test_function);
  RESET_FAKE(GLOBAL_INTERRUPTS_DISABLE);
  RESET_FAKE(GLOBAL_INTERRUPTS_ENABLE);
}

TEST_TEAR_DOWN(dispatcher_tests)
{}

TEST(dispatcher_tests, signal_event_mask_sets_all_bits)
{
  dispatcher_signal_event_mask(0xAAAAAAAA, 0);
  TEST_ASSERT_EQUAL_INT(0xAAAAAAAA, s_unhandled_events);
}

TEST(dispatcher_tests, signal_event_mask_records_arg_for_all_set_bits)
{
  dispatcher_signal_event_mask(0xAAAAAAAA, 999);

  for (size_t i = 0u; i < EV_NUMBER_EVENTS; i++)
  {
    if (s_unhandled_events & (1u << i))
    {
      TEST_ASSERT_EQUAL_INT(999, s_signalled_args[i]);
    }
    else
    {
      TEST_ASSERT_EQUAL_INT(0, s_signalled_args[i]);
    }
  }
}

TEST(dispatcher_tests, signal_event_mask_disables_interrupts)
{
  // Set events 1 and 3
  dispatcher_signal_event_mask((1u << 1u) | (1u << 3u), 0);

  TEST_ASSERT_EQUAL_INT(1, GLOBAL_INTERRUPTS_DISABLE_fake.call_count);
  TEST_ASSERT_EQUAL_INT(1, GLOBAL_INTERRUPTS_ENABLE_fake.call_count);
}

TEST(dispatcher_tests, clear_event_mask_clears_all_bits)
{
  // Set events 1, 2 and 31
  dispatcher_signal_event_mask((1u << 1u) | (1u << 2u), 0);

  // Clear event 31
  dispatcher_clear_event_mask(1u << 31u);

  TEST_ASSERT_EQUAL_INT((1u << 1u) | (1u << 2u), s_unhandled_events);

  // Clear events 1 and 2
  dispatcher_clear_event_mask((1u << 1u) | (1u << 2u));

  TEST_ASSERT_EQUAL_INT(0u, s_unhandled_events);
}

TEST(dispatcher_tests, clear_event_mask_disables_interrupts)
{
  // Clear event 31
  dispatcher_clear_event_mask(1u << 31u);

  TEST_ASSERT_EQUAL_INT(1, GLOBAL_INTERRUPTS_DISABLE_fake.call_count);
  TEST_ASSERT_EQUAL_INT(1, GLOBAL_INTERRUPTS_ENABLE_fake.call_count);
}

TEST(dispatcher_tests, service_null_pointer_calls_default_handler)
{
  // Set all valid event bits
  s_unhandled_events = (1u << EV_NUMBER_EVENTS) - 1u;
  dispatcher_service();
  TEST_ASSERT_EQUAL_INT(EV_NUMBER_EVENTS, s_dispatch_errors);
  TEST_ASSERT_EQUAL_INT(0u, s_unhandled_events);
}

TEST(dispatcher_tests, service_doesnt_process_invalid_events)
{
  // Set all invalid event bits
  s_unhandled_events = ~((1u << EV_NUMBER_EVENTS) - 1u);
  dispatcher_service();
  TEST_ASSERT_EQUAL_INT((32u - EV_NUMBER_EVENTS), s_dispatch_errors);
  TEST_ASSERT_EQUAL_INT(0u, s_unhandled_events);
}

TEST(dispatcher_tests, service_processes_all_events)
{
  for (size_t i = 0u; i < EV_NUMBER_EVENTS; i++)
  {
    s_bindings[i] = test_function_caller;
    s_signalled_args[i] = i;
  }

  // Set all valid event bits
  s_unhandled_events = (1u << EV_NUMBER_EVENTS) - 1u;
  dispatcher_service();
  TEST_ASSERT_EQUAL_INT(0u, s_unhandled_events);

  // Check that all the events were processed as expected
  TEST_ASSERT_EQUAL_INT(EV_NUMBER_EVENTS, test_function_fake.call_count);
  for (size_t i = 0u; i < EV_NUMBER_EVENTS; i++)
  {
    TEST_ASSERT_EQUAL_INT(i, test_function_fake.arg0_history[i]);
  }
}

TEST(dispatcher_tests, service_disables_interrupts)
{
  // Set events 1 and 3
  s_bindings[1] = test_function_caller;
  s_bindings[3] = test_function_caller;
  s_signalled_args[1] = 100;
  s_signalled_args[3] = 1000;
  s_unhandled_events = (1u << 1u) | (1u << 3u);

  dispatcher_service();
  TEST_ASSERT_EQUAL_INT(100, test_function_fake.arg0_history[0]);
  TEST_ASSERT_EQUAL_INT(1000, test_function_fake.arg0_history[1]);
  TEST_ASSERT_EQUAL_INT(2, test_function_fake.call_count);

  TEST_ASSERT_EQUAL_INT(2, GLOBAL_INTERRUPTS_DISABLE_fake.call_count);
  TEST_ASSERT_EQUAL_INT(2, GLOBAL_INTERRUPTS_ENABLE_fake.call_count);
}

TEST(dispatcher_tests, bind_each_event_in_mask_to_same_handler)
{
  // Bind events 2 and 4
  dispatcher_bind((1u << 2u) | (1u << 4u), test_function_caller);

  TEST_ASSERT_EQUAL_PTR(test_function_caller, s_bindings[2]);
  TEST_ASSERT_EQUAL_PTR(test_function_caller, s_bindings[4]);
}

TEST(dispatcher_tests, bind_overwrites_handlers)
{
  // Bind events 2 and 4
  dispatcher_bind((1u << 2u) | (1u << 4u), test_function_caller);

  TEST_ASSERT_EQUAL_PTR(test_function_caller, s_bindings[2]);
  TEST_ASSERT_EQUAL_PTR(test_function_caller, s_bindings[4]);

  // Rebind event 4 to NULL
  dispatcher_bind((1u << 4u), NULL);

  TEST_ASSERT_EQUAL_PTR(test_function_caller, s_bindings[2]);
  TEST_ASSERT_NULL(s_bindings[4]);
}

TEST(dispatcher_tests, bind_disables_interrupts)
{
  // Bind events 2 and 4
  dispatcher_bind((1u << 2u) | (1u << 4u), test_function_caller);

  TEST_ASSERT_EQUAL_INT(1, GLOBAL_INTERRUPTS_DISABLE_fake.call_count);
  TEST_ASSERT_EQUAL_INT(1, GLOBAL_INTERRUPTS_ENABLE_fake.call_count);
}

TEST(dispatcher_tests, bind_all_overwrites_handlers)
{
  // Bind events 2 and 4
  dispatcher_bind((1u << 2u) | (1u << 4u), test_function_caller);

  TEST_ASSERT_EQUAL_PTR(test_function_caller, s_bindings[2]);
  TEST_ASSERT_EQUAL_PTR(test_function_caller, s_bindings[4]);

  system_event_callback binds[4] = {NULL};

  dispatcher_bind_all(binds, 4u);

  TEST_ASSERT_NULL(s_bindings[2]);
  TEST_ASSERT_EQUAL_PTR(test_function_caller, s_bindings[4]);
}

TEST(dispatcher_tests, bind_all_does_nothing_if_number_binds_exceeds_number_of_events)
{
  system_event_callback binds[EV_NUMBER_EVENTS + 1u] =
  {
    [0] = test_function_caller
  };

  dispatcher_bind_all(binds, EV_NUMBER_EVENTS + 1u);

  TEST_ASSERT_NULL(s_bindings[0]);
}

TEST(dispatcher_tests, bind_all_stops_at_number_binds)
{
  system_event_callback binds[2] =
  {
    [0] = test_function_caller,
    [1] = test_function_caller
  };

  dispatcher_bind_all(binds, 1u);

  TEST_ASSERT_EQUAL_PTR(test_function_caller, s_bindings[0]);
  TEST_ASSERT_NULL(s_bindings[1]);
}

TEST(dispatcher_tests, bind_all_disables_interrupts)
{
  system_event_callback binds[2] =
  {
    [0] = test_function_caller,
    [1] = test_function_caller
  };

  dispatcher_bind_all(binds, 2u);

  TEST_ASSERT_EQUAL_INT(1, GLOBAL_INTERRUPTS_DISABLE_fake.call_count);
  TEST_ASSERT_EQUAL_INT(1, GLOBAL_INTERRUPTS_ENABLE_fake.call_count);
}
