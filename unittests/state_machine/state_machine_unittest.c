// Copyright (c) 2021 <OpenVent-Bristol, Donald Robson>

#include "unity.h"
#include "unity_fixture.h"
#include "state/state_machine.h"
#include "state/private/state_machine_priv.h"
#include "misc/util.h"
#include "fake_state_defs.h"
#include <string.h>
#include <stdbool.h>

extern state_machine_t s_machines[STATE_MACHINE_COUNT_MAX];

extern bool state_machine_valid(const state_machine_t* const state_machine);

static enum system_event test_event_val;
static void test_store_event_val(enum system_event* const event_mask)
{
  test_event_val = *event_mask;
}

TEST_GROUP(state_machine_test);

TEST_SETUP(state_machine_test)
{
  FAKE_STATE_FUNCTIONS(RESET_FAKE);
  memset(s_machines, 0, sizeof(s_machines));
  test_event_val = UINT32_MAX;
}

TEST_TEAR_DOWN(state_machine_test)
{}

TEST(state_machine_test, valid_true)
{
  for (size_t i = 0u; i < STATE_MACHINE_COUNT_MAX; i++)
  {
    bool valid = state_machine_valid(&s_machines[i]);
    TEST_ASSERT_TRUE(valid);
  }
}

TEST(state_machine_test, valid_false)
{
  bool valid = state_machine_valid(NULL);
  TEST_ASSERT_FALSE(valid);

  state_machine_t bad_machine;
  valid = state_machine_valid(&bad_machine);
  TEST_ASSERT_FALSE(valid);
}

TEST(state_machine_test, set_transition_list_does_nothing_if_bad_args)
{
  state_transition_t transitions[INT8_MAX + 1u] = {0};
  state_transition_list_t transition_list =
  {
    transitions,
    ARRAY_LENGTH(transitions)
  };

  // No state machine pointer
  state_machine_set_transition_list(NULL, &transition_list);
  TEST_ASSERT_NULL(s_machines[0].transitions);

  // Bad state machine pointer
  state_machine_t bad_machine;
  state_machine_set_transition_list(&bad_machine, &transition_list);
  TEST_ASSERT_NULL(s_machines[0].transitions);

  // Genuine state machine pointer, no transition_list
  state_machine_set_transition_list(&s_machines[0], NULL);
  TEST_ASSERT_NULL(s_machines[0].transitions);

  // Genuine pointers, bad length
  state_machine_set_transition_list(&s_machines[0], &transition_list);
  TEST_ASSERT_NULL(s_machines[0].transitions);
}

TEST(state_machine_test, set_transition_list_sets_transitions)
{
  state_transition_t transitions[] =
  {
    {0, 1, 1}, {1, 0, 2}
  };

  state_transition_list_t transition_list =
  {
    transitions,
    ARRAY_LENGTH(transitions)
  };

  state_machine_set_transition_list(&s_machines[0], &transition_list);
  TEST_ASSERT_POINTERS_EQUAL(&transition_list, s_machines[0].transitions);
}

TEST(state_machine_test, set_transition_list_clears_index)
{
  state_transition_t transitions[] =
  {
    {0, 1, 1}, {1, 0, 2}
  };

  state_transition_list_t transition_list =
  {
    transitions,
    ARRAY_LENGTH(transitions)
  };

  state_machine_set_transition_list(&s_machines[0], &transition_list);
  TEST_ASSERT_EQUAL(-1, s_machines[0].transition_index[ARRAY_LENGTH(transitions)]);
}

TEST(state_machine_test, set_transition_list_populates_index)
{
  state_transition_t transitions[] =
  {
    {2, 3, 1}, {2, 4, 2}, {3, 2, 8}, {4, 3, 16}
  };

  state_transition_list_t transition_list =
  {
    transitions,
    ARRAY_LENGTH(transitions)
  };

  state_machine_set_transition_list(&s_machines[0], &transition_list);
  TEST_ASSERT_EQUAL(-1, s_machines[0].transition_index[0]);
  TEST_ASSERT_EQUAL(-1, s_machines[0].transition_index[1]);
  TEST_ASSERT_EQUAL(0, s_machines[0].transition_index[2]);
  TEST_ASSERT_EQUAL(2, s_machines[0].transition_index[3]);
  TEST_ASSERT_EQUAL(3, s_machines[0].transition_index[4]);
}

TEST(state_machine_test, init_does_nothing_if_bad_args)
{
  state_transition_t transitions[] =
  {
    {2, 3, 1}, {2, 4, 2}, {3, 2, 8}, {4, 3, 16}
  };

  state_transition_list_t transition_list =
  {
    transitions,
    ARRAY_LENGTH(transitions)
  };

  state_definition_t definitions[] =
  {
    [2] = {NULL, NULL, NULL},
    [3] = {NULL, NULL, NULL},
    [4] = {NULL, NULL, NULL},
  };

  state_definition_list_t definition_list =
  {
    definitions,
    ARRAY_LENGTH(definitions)
  };

  state_machine_t* null_machine = NULL;
  state_machine_t* bad_machine = (state_machine_t*)(1u);

  // No state machine pointer
  state_machine_init(NULL, 0, &definition_list, &transition_list);
  TEST_ASSERT_NULL(s_machines[0].definitions);
  TEST_ASSERT_NULL(s_machines[0].transitions);
  TEST_ASSERT_NULL(s_machines[1].definitions);
  TEST_ASSERT_NULL(s_machines[1].transitions);

  // Bad state machine pointer
  state_machine_init(&bad_machine, 0, &definition_list, &transition_list);
  TEST_ASSERT_NULL(s_machines[0].definitions);
  TEST_ASSERT_NULL(s_machines[0].transitions);
  TEST_ASSERT_NULL(s_machines[1].definitions);
  TEST_ASSERT_NULL(s_machines[1].transitions);

  // Genuine pointers, no transition_list
  state_machine_init(&null_machine, 0, &definition_list, NULL);
  TEST_ASSERT_NULL(s_machines[0].definitions);
  TEST_ASSERT_NULL(s_machines[0].transitions);
  TEST_ASSERT_NULL(s_machines[1].definitions);
  TEST_ASSERT_NULL(s_machines[1].transitions);

  // Genuine pointers, no definition_list
  null_machine = NULL;
  state_machine_init(&null_machine, 0, NULL, &transition_list);
  TEST_ASSERT_NULL(s_machines[0].definitions);
  TEST_ASSERT_NULL(s_machines[0].transitions);
  TEST_ASSERT_NULL(s_machines[1].definitions);
  TEST_ASSERT_NULL(s_machines[1].transitions);
}

TEST(state_machine_test, init_does_nothing_if_too_many_machines)
{
  state_transition_t transitions[] =
  {
    {2, 3, 1}, {2, 4, 2}, {3, 2, 8}, {4, 3, 16}
  };

  state_transition_list_t transition_list =
  {
    transitions,
    ARRAY_LENGTH(transitions)
  };

  state_definition_t definitions[] =
  {
    [2] = {NULL, NULL, NULL},
    [3] = {NULL, NULL, NULL},
    [4] = {NULL, NULL, NULL},
  };

  state_definition_list_t definition_list =
  {
    definitions,
    ARRAY_LENGTH(definitions)
  };

  state_machine_t* machine = NULL;
  state_machine_init(&machine, 0, &definition_list, &transition_list);
  TEST_ASSERT_POINTERS_EQUAL(&s_machines[0], machine);

  machine = NULL;
  state_machine_init(&machine, 0, &definition_list, &transition_list);
  TEST_ASSERT_POINTERS_EQUAL(&s_machines[1], machine);

  machine = NULL;
  state_machine_init(&machine, 0, &definition_list, &transition_list);
  TEST_ASSERT_NULL(machine);
}

TEST(state_machine_test, init_sets_initial_state_transitions_and_definitions)
{
  state_transition_t transitions[] =
  {
    {2, 3, 1}, {2, 4, 2}, {3, 2, 8}, {4, 3, 16}
  };

  state_transition_list_t transition_list =
  {
    transitions,
    ARRAY_LENGTH(transitions)
  };

  state_definition_t definitions[] =
  {
    [2] = {NULL, NULL, NULL},
    [3] = {NULL, NULL, NULL},
    [4] = {NULL, NULL, NULL},
  };

  state_definition_list_t definition_list =
  {
    definitions,
    ARRAY_LENGTH(definitions)
  };

  state_machine_t* machine = NULL;

  state_machine_init(&machine, 2, &definition_list, &transition_list);
  TEST_ASSERT_POINTERS_EQUAL(&s_machines[0], machine);
  TEST_ASSERT_POINTERS_EQUAL(machine->definitions, &definition_list);
  TEST_ASSERT_POINTERS_EQUAL(machine->transitions, &transition_list);
  TEST_ASSERT_EQUAL(-1, machine->transition_index[0]);
  TEST_ASSERT_EQUAL(-1, machine->transition_index[1]);
  TEST_ASSERT_EQUAL(0, machine->transition_index[2]);
  TEST_ASSERT_EQUAL(2, machine->transition_index[3]);
  TEST_ASSERT_EQUAL(3, machine->transition_index[4]);
  TEST_ASSERT_EQUAL(2, machine->current_state);
}

TEST(state_machine_test, init_calls_initial_state_on_entry_function_with_NO_EVENT)
{
  state_transition_t transitions[] =
  {
    {2, 3, 1}, {2, 4, 2}, {3, 2, 8}, {4, 3, 16}
  };

  state_transition_list_t transition_list =
  {
    transitions,
    ARRAY_LENGTH(transitions)
  };

  state_definition_t definitions[] =
  {
    [2] = {test_state_member_2_entry, NULL, NULL},
    [3] = {NULL, NULL, NULL},
    [4] = {NULL, NULL, NULL},
  };

  state_definition_list_t definition_list =
  {
    definitions,
    ARRAY_LENGTH(definitions)
  };

  state_machine_t* machine = NULL;

  test_state_member_2_entry_fake.custom_fake = test_store_event_val;

  state_machine_init(&machine, 2, &definition_list, &transition_list);
  TEST_ASSERT_EQUAL(NO_EVENT, test_event_val);
}

/* Tests for state_machine_run */

state_transition_t run_transitions[] =
{
  {2, 3, 1},
  {2, 4, 2},
  {3, 2, 4},
  {4, 3, 8},
  {4, 5, 16},
  {5, 3, 32}
};

state_transition_list_t run_transition_list =
{
  run_transitions,
  ARRAY_LENGTH(run_transitions)
};

state_definition_t run_definitions[] =
{
  [2] = {test_state_member_2_entry, test_state_member_2_run, test_state_member_2_exit},
  [3] = {test_state_member_3_entry, test_state_member_3_run, test_state_member_3_exit},
  [4] = {test_state_member_4_entry, test_state_member_4_run, test_state_member_4_exit},
  [5] = {test_state_member_5_entry, test_state_member_5_run, test_state_member_5_exit},
};

state_definition_list_t run_definition_list =
{
  run_definitions,
  ARRAY_LENGTH(run_definitions)
};

state_machine_t* run_machine;

TEST_GROUP(state_machine_run_test);

TEST_SETUP(state_machine_run_test)
{
  FAKE_STATE_FUNCTIONS(RESET_FAKE);
  memset(s_machines, 0, sizeof(s_machines));
  test_event_val = UINT32_MAX;
  run_machine = NULL;
  state_machine_init(&run_machine, 2, &run_definition_list, &run_transition_list);
  RESET_FAKE(test_state_member_2_entry);
}

TEST_TEAR_DOWN(state_machine_run_test)
{}

TEST(state_machine_run_test, run_does_nothing_with_bad_args)
{
  // Check state has not been modified
  TEST_ASSERT_EQUAL(2, run_machine->current_state);

  test_state_member_2_entry_fake.custom_fake = test_store_event_val;
  test_state_member_2_run_fake.custom_fake = test_store_event_val;
  test_state_member_2_exit_fake.custom_fake = test_store_event_val;

  state_machine_run(run_machine, NULL);
  TEST_ASSERT_EQUAL(UINT32_MAX, test_event_val);

  enum system_event trans_2_to_4 = 2;

  state_machine_run(NULL, &trans_2_to_4);
  TEST_ASSERT_EQUAL(0, test_state_member_2_entry_fake.call_count);
  TEST_ASSERT_EQUAL(0, test_state_member_2_run_fake.call_count);
  TEST_ASSERT_EQUAL(0, test_state_member_2_exit_fake.call_count);
}

TEST(state_machine_run_test, run_does_not_transition_with_NO_EVENT)
{
  // Check state has not been modified
  TEST_ASSERT_EQUAL(2, run_machine->current_state);

  enum system_event no_event = NO_EVENT;

  state_machine_run(run_machine, &no_event);
  TEST_ASSERT_EQUAL(0, test_state_member_2_exit_fake.call_count);
}

TEST(state_machine_run_test, run_does_not_transition_with_unmatched_event)
{
  // Check state has not been modified
  TEST_ASSERT_EQUAL(2, run_machine->current_state);

  enum system_event trans_3_to_2 = 4;

  state_machine_run(run_machine, &trans_3_to_2);
  TEST_ASSERT_EQUAL(0, test_state_member_2_exit_fake.call_count);
}

TEST(state_machine_run_test, run_calls_state_on_run_function_with_no_transition)
{
  // Check state has not been modified
  TEST_ASSERT_EQUAL(2, run_machine->current_state);

  test_state_member_2_run_fake.custom_fake = test_store_event_val;

  enum system_event no_event = NO_EVENT;

  state_machine_run(run_machine, &no_event);
  TEST_ASSERT_EQUAL(1, test_state_member_2_run_fake.call_count);
  TEST_ASSERT_EQUAL(NO_EVENT, test_event_val);
  TEST_ASSERT_EQUAL(0, test_state_member_2_exit_fake.call_count);
}

TEST(state_machine_run_test, run_calls_old_state_on_exit_function_on_transition)
{
  // Check state has not been modified
  TEST_ASSERT_EQUAL(2, run_machine->current_state);

  test_state_member_2_exit_fake.custom_fake = test_store_event_val;

  enum system_event trans_2_to_4 = 2;

  state_machine_run(run_machine, &trans_2_to_4);
  TEST_ASSERT_EQUAL(0, test_state_member_2_run_fake.call_count);
  TEST_ASSERT_EQUAL(1, test_state_member_2_exit_fake.call_count);
  TEST_ASSERT_EQUAL(2, test_event_val);
}

TEST(state_machine_run_test, run_calls_new_state_on_entry_function_on_transition)
{
  // Check state has not been modified
  TEST_ASSERT_EQUAL(2, run_machine->current_state);

  test_state_member_4_entry_fake.custom_fake = test_store_event_val;

  enum system_event trans_2_to_4 = 2;

  state_machine_run(run_machine, &trans_2_to_4);
  TEST_ASSERT_EQUAL(0, test_state_member_2_run_fake.call_count);
  TEST_ASSERT_EQUAL(1, test_state_member_4_entry_fake.call_count);
  TEST_ASSERT_EQUAL(2, test_event_val);
  TEST_ASSERT_EQUAL(0, test_state_member_4_run_fake.call_count);
}

TEST(state_machine_run_test, run_does_not_clear_event_bits_on_transition)
{
  // Check state has not been modified
  TEST_ASSERT_EQUAL(2, run_machine->current_state);

  test_state_member_4_entry_fake.custom_fake = test_store_event_val;

  enum system_event trans_2_to_4 = 2;

  state_machine_run(run_machine, &trans_2_to_4);
  TEST_ASSERT_EQUAL(0, test_state_member_2_run_fake.call_count);
  TEST_ASSERT_EQUAL(1, test_state_member_4_entry_fake.call_count);
  TEST_ASSERT_EQUAL(trans_2_to_4, test_event_val);
}

TEST(state_machine_run_test, run_prioritises_low_bits_for_transitions)
{
  // TODO - SEGFAULT
  run_machine->current_state = 4;

  enum system_event trans_4_to_3 = 8;
  enum system_event trans_4_to_5 = 16;

  enum system_event trans = trans_4_to_3 | trans_4_to_5;

  state_machine_run(run_machine, &trans);
  TEST_ASSERT_EQUAL(0, test_state_member_4_run_fake.call_count);
  TEST_ASSERT_EQUAL(1, test_state_member_4_exit_fake.call_count);
  TEST_ASSERT_EQUAL(1, test_state_member_3_entry_fake.call_count);
  TEST_ASSERT_EQUAL(0, test_state_member_5_entry_fake.call_count);
}
