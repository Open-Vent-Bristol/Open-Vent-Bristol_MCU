// Copyright (c) 2021 <OpenVent-Bristol, Donald Robson>

#include "unity.h"
#include "unity_fixture.h"
#include "scheduler/timer.h"
#include "scheduler/private/timer_priv.h"
#include "dispatcher_mock.h"
#include <string.h>

extern timer_t* s_timers[TIMER_COUNT];

timer_t test_timer;

TEST_GROUP(timer_tests);

TEST_SETUP(timer_tests)
{
  memset(s_timers, 0, sizeof(s_timers));
  memset(&test_timer, 0, sizeof(test_timer));

  RESET_FAKE(dispatcher_signal_event_mask);
}

TEST_TEAR_DOWN(timer_tests)
{}

TEST(timer_tests, tick_all_ignores_null_pointers)
{
  // Just check for no segfaults
  timer_tick_all();
  TEST_ASSERT_EQUAL(0, 0);
}

TEST(timer_tests, tick_all_processes_all_timers)
{
  timer_t timer0 =
  {
    .remaining_ticks = 2,
    .unique_id = TIMER_ACTUATOR_SERVICE
  };

  timer_t timer1 =
  {
    .remaining_ticks = 20,
    .unique_id = TIMER_1
  };

  timer_t timer2 =
  {
    .remaining_ticks = 300,
    .unique_id = TIMER_2
  };

  timer_t timer3 =
  {
    .remaining_ticks = 4000,
    .unique_id = TIMER_3
  };

  timer_t timer4 =
  {
    .remaining_ticks = 50000,
    .unique_id = TIMER_4
  };

  timer_t timer5 =
  {
    .remaining_ticks = 600000,
    .unique_id = TIMER_5
  };

  s_timers[TIMER_ACTUATOR_SERVICE - TIMER_START_INDEX] = &timer0;
  s_timers[TIMER_1 - TIMER_START_INDEX] = &timer1;
  s_timers[TIMER_2 - TIMER_START_INDEX] = &timer2;
  s_timers[TIMER_3 - TIMER_START_INDEX] = &timer3;
  s_timers[TIMER_4 - TIMER_START_INDEX] = &timer4;
  s_timers[TIMER_5 - TIMER_START_INDEX] = &timer5;

  timer_tick_all();

  TEST_ASSERT_EQUAL_INT32(1, timer0.remaining_ticks);
  TEST_ASSERT_EQUAL_INT32(19, timer1.remaining_ticks);
  TEST_ASSERT_EQUAL_INT32(299, timer2.remaining_ticks);
  TEST_ASSERT_EQUAL_INT32(3999, timer3.remaining_ticks);
  TEST_ASSERT_EQUAL_INT32(49999, timer4.remaining_ticks);
  TEST_ASSERT_EQUAL_INT32(599999, timer5.remaining_ticks);
}

TEST(timer_tests, attach_pointer)
{
  test_timer.unique_id = TIMER_1;

  timer_attach(&test_timer);
  TEST_ASSERT_NOT_NULL(s_timers[TIMER_1 - TIMER_START_INDEX]);

  timer_attach(NULL);
  TEST_ASSERT_EQUAL_PTR(&test_timer, s_timers[TIMER_1 - TIMER_START_INDEX]);
}

TEST(timer_tests, attach_only_valid_index)
{
  test_timer.unique_id = 0u;

  timer_attach(&test_timer);

  for (size_t i = 0u; i < TIMER_COUNT; i++)
  {
    TEST_ASSERT_NULL(s_timers[i]);
  }

  test_timer.unique_id = UINT32_MAX;

  timer_attach(&test_timer);

  for (size_t i = 0u; i < TIMER_COUNT; i++)
  {
    TEST_ASSERT_NULL(s_timers[i]);
  }
}

TEST(timer_tests, attach_zero_ticks_does_not_signal_events)
{
  test_timer.remaining_ticks = 0;

  timer_attach(&test_timer);
  timer_tick_all();

  TEST_ASSERT_EQUAL(0, dispatcher_signal_event_mask_fake.call_count);
}

TEST(timer_tests, attach_negative_ticks_does_not_signal_events)
{
  test_timer.remaining_ticks = INT32_MIN;

  timer_attach(&test_timer);
  timer_tick_all();

  TEST_ASSERT_EQUAL(0, dispatcher_signal_event_mask_fake.call_count);
}

TEST(timer_tests, reset_null_pointer)
{
  // Just check for no segfault
  timer_reset(NULL);
  TEST_ASSERT_EQUAL_INT(0, 0);
}

TEST(timer_tests, reset_sets_remaining_ticks)
{
  test_timer.initial_ticks = 100;

  timer_reset(&test_timer);
  TEST_ASSERT_EQUAL_INT(100, test_timer.remaining_ticks);
}

TEST(timer_tests, reset_zero_ticks_does_not_signal_events)
{
  test_timer.remaining_ticks = 100;
  test_timer.initial_ticks = 0;

  timer_reset(&test_timer);
  timer_tick_all();
  TEST_ASSERT_EQUAL(0, dispatcher_signal_event_mask_fake.call_count);
}

TEST(timer_tests, reset_negative_ticks_does_not_signal_events)
{
  test_timer.remaining_ticks = 100;
  test_timer.initial_ticks = INT32_MIN;

  timer_reset(&test_timer);
  timer_tick_all();
  TEST_ASSERT_EQUAL(0, dispatcher_signal_event_mask_fake.call_count);
}

TEST(timer_tests, stop_null_pointer)
{
  // Just check for no segfault
  timer_stop(NULL);
  TEST_ASSERT_EQUAL_INT(0, 0);
}

TEST(timer_tests, stop_sets_remaining_ticks)
{
  test_timer.initial_ticks = 100;

  timer_stop(&test_timer);
  TEST_ASSERT_EQUAL_INT(-1, test_timer.remaining_ticks);
}

TEST(timer_tests, expired_timer_sets_all_events)
{
  test_timer.events_signalled = 0xAAAAAAAA;
  test_timer.remaining_ticks = 1;
  test_timer.unique_id = TIMER_5;

  timer_attach(&test_timer);
  timer_tick_all();

  TEST_ASSERT_EQUAL_INT(1, dispatcher_signal_event_mask_fake.call_count);
  TEST_ASSERT_EQUAL_HEX32(0xAAAAAAAA, dispatcher_signal_event_mask_fake.arg0_val);
}

TEST(timer_tests, expired_timer_sends_unique_id_arg)
{
  test_timer.remaining_ticks = 1;
  test_timer.unique_id = TIMER_5;

  timer_attach(&test_timer);
  timer_tick_all();

  TEST_ASSERT_EQUAL_INT(1, dispatcher_signal_event_mask_fake.call_count);
  TEST_ASSERT_EQUAL_INT(TIMER_5, dispatcher_signal_event_mask_fake.arg1_val);
}

TEST(timer_tests, expired_one_shot_timer_does_not_restart)
{
  test_timer.remaining_ticks = 1;
  test_timer.unique_id = TIMER_5;
  test_timer.type = TIMER_TYPE_ONE_SHOT;

  timer_attach(&test_timer);
  timer_tick_all();

  TEST_ASSERT_EQUAL_INT(-1, test_timer.remaining_ticks);
}

TEST(timer_tests, expired_one_shot_timer_does_not_overflow_back_to_int32_max)
{
  test_timer.remaining_ticks = INT32_MIN;
  test_timer.unique_id = TIMER_5;
  test_timer.type = TIMER_TYPE_ONE_SHOT;

  timer_attach(&test_timer);
  timer_tick_all();

  TEST_ASSERT_EQUAL_INT(INT32_MIN, test_timer.remaining_ticks);
}

TEST(timer_tests, expired_continuous_timer_restarts_automatically)
{
  test_timer.initial_ticks = 100;
  test_timer.remaining_ticks = 1;
  test_timer.unique_id = TIMER_5;
  test_timer.type = TIMER_TYPE_CONTINUOUS;

  timer_attach(&test_timer);
  timer_tick_all();

  TEST_ASSERT_EQUAL_INT(100, test_timer.remaining_ticks);
}
