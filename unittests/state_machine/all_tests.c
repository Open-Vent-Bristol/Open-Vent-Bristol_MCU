#include "unity.h"
#include "unity_fixture.h"

TEST_GROUP_RUNNER(state_machine_tests)
{
  RUN_TEST_CASE(state_machine_test, valid_true);
  RUN_TEST_CASE(state_machine_test, valid_false);

  RUN_TEST_CASE(state_machine_test, set_transition_list_does_nothing_if_bad_args);
  RUN_TEST_CASE(state_machine_test, set_transition_list_sets_transitions);
  RUN_TEST_CASE(state_machine_test, set_transition_list_clears_index);
  RUN_TEST_CASE(state_machine_test, set_transition_list_populates_index);

  RUN_TEST_CASE(state_machine_test, init_does_nothing_if_bad_args);
  RUN_TEST_CASE(state_machine_test, init_does_nothing_if_too_many_machines);
  RUN_TEST_CASE(state_machine_test, init_sets_initial_state_transitions_and_definitions);
  RUN_TEST_CASE(state_machine_test, init_calls_initial_state_on_entry_function_with_NO_EVENT);
}

TEST_GROUP_RUNNER(state_machine_run_tests)
{
  RUN_TEST_CASE(state_machine_run_test, run_does_nothing_with_bad_args);
  RUN_TEST_CASE(state_machine_run_test, run_does_not_transition_with_NO_EVENT);
  RUN_TEST_CASE(state_machine_run_test, run_does_not_transition_with_unmatched_event);
  RUN_TEST_CASE(state_machine_run_test, run_calls_state_on_run_function_with_no_transition);
  RUN_TEST_CASE(state_machine_run_test, run_calls_old_state_on_exit_function_on_transition);
  RUN_TEST_CASE(state_machine_run_test, run_calls_new_state_on_entry_function_on_transition);
  RUN_TEST_CASE(state_machine_run_test, run_does_not_clear_event_bits_on_transition);
  RUN_TEST_CASE(state_machine_run_test, run_prioritises_low_bits_for_transitions);
}

TEST_GROUP_RUNNER(state_machine_run_all_tests)
{
  RUN_TEST_CASE(state_machine_run_all_test, run_all_sends_same_event_mask_to_all_machines);
  RUN_TEST_CASE(
    state_machine_run_all_test, run_all_clears_bits_in_event_mask_if_a_machine_clears_them);
}

static void RunAllTests(void)
{
  RUN_TEST_GROUP(state_machine_tests);
  RUN_TEST_GROUP(state_machine_run_tests);
  RUN_TEST_GROUP(state_machine_run_all_tests);
}

int main(int argc, const char* argv[])
{
  return UnityMain(argc, argv, RunAllTests);
}
