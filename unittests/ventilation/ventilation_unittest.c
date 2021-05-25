// Copyright (c) 2021 <OpenVent-Bristol, Donald Robson>

#include "unity.h"
#include "unity_fixture.h"
#include "ventilation/ventilation.h"
#include "ventilation/private/ventilation_priv.h"
#include "ventilation/private/ventilation_states.h"
#include "fpga/fpga_api.h"
#include "dispatcher_mock.h"
#include "state_machine_mock.h"

extern enum fpga_operating_mode s_current_mode;
extern enum fpga_operating_mode s_next_mode;
extern state_machine_t* s_ventilation_state_machine;

extern void ventilation_set_target_mode(int32_t mode);

TEST_GROUP(ventilation_tests);

TEST_SETUP(ventilation_tests)
{
  s_current_mode = FPGA_MODE_STANDBY;
  s_next_mode = FPGA_MODE_STANDBY;
  s_ventilation_state_machine = NULL;
  DISPATCHER_MOCKS(RESET_FAKE);
  STATE_MACHINE_MOCKS(RESET_FAKE);
}

TEST_TEAR_DOWN(ventilation_tests)
{}

TEST(ventilation_tests, init_binds_event)
{
  ventilation_init();
  TEST_ASSERT_EQUAL_INT(1, dispatcher_bind_fake.call_count);
  TEST_ASSERT_EQUAL_INT(1u << EV_OP_MODE_CHANGE, dispatcher_bind_fake.arg0_val);
}

TEST(ventilation_tests, init_calls_state_machine_init)
{
  ventilation_init();
  TEST_ASSERT_EQUAL_INT(1, state_machine_init_fake.call_count);
  TEST_ASSERT_EQUAL_PTR(&s_ventilation_state_machine, state_machine_init_fake.arg0_val);
  TEST_ASSERT_EQUAL_INT(VENTILATION_STATE_HOMING, state_machine_init_fake.arg1_val);
  TEST_ASSERT_EQUAL_PTR(&s_state_definitions, state_machine_init_fake.arg2_val);
  TEST_ASSERT_EQUAL_PTR(&s_wait_at_home_transitions, state_machine_init_fake.arg3_val);
}

TEST(ventilation_tests, set_target_mode_clears_event)
{
  ventilation_set_target_mode(FPGA_MODE_STANDBY);
  TEST_ASSERT_EQUAL_INT(1, dispatcher_clear_event_mask_fake.call_count);
  TEST_ASSERT_EQUAL_INT(1u << EV_OP_MODE_CHANGE, dispatcher_clear_event_mask_fake.arg0_val);
}

TEST(ventilation_tests, set_target_mode_sets_target_mode)
{
  ventilation_set_target_mode(FPGA_MODE_PRESSURE_SUPPORT);
  TEST_ASSERT_EQUAL_INT(FPGA_MODE_PRESSURE_SUPPORT, s_next_mode);
}

TEST(ventilation_tests, set_target_mode_doesnt_set_current_mode)
{
  ventilation_set_target_mode(FPGA_MODE_PRESSURE_SUPPORT);
  TEST_ASSERT_EQUAL_INT(FPGA_MODE_STANDBY, s_current_mode);
}

TEST(ventilation_tests, set_to_target_mode_does_nothing_if_no_change)
{
  s_next_mode = FPGA_MODE_STANDBY;
  ventilation_set_to_target_mode();
  TEST_ASSERT_EQUAL_INT(FPGA_MODE_STANDBY, s_next_mode);
  TEST_ASSERT_EQUAL_INT(FPGA_MODE_STANDBY, s_current_mode);
  TEST_ASSERT_EQUAL_INT(0, state_machine_set_transition_list_fake.call_count);
}

TEST(ventilation_tests, set_to_target_mode_sets_target_mode_if_valid)
{
  s_next_mode = FPGA_MODE_RUN;
  ventilation_set_to_target_mode();
  TEST_ASSERT_EQUAL_INT(FPGA_MODE_RUN, s_next_mode);
  TEST_ASSERT_EQUAL_INT(FPGA_MODE_RUN, s_current_mode);

  s_next_mode = FPGA_MODE_OFF;
  ventilation_set_to_target_mode();
  TEST_ASSERT_EQUAL_INT(FPGA_MODE_OFF, s_next_mode);
  TEST_ASSERT_EQUAL_INT(FPGA_MODE_OFF, s_current_mode);

  s_next_mode = FPGA_MODE_PRESSURE_CONTROL;
  ventilation_set_to_target_mode();
  TEST_ASSERT_EQUAL_INT(FPGA_MODE_PRESSURE_CONTROL, s_next_mode);
  TEST_ASSERT_EQUAL_INT(FPGA_MODE_PRESSURE_CONTROL, s_current_mode);

  s_next_mode = FPGA_MODE_STANDBY;
  ventilation_set_to_target_mode();
  TEST_ASSERT_EQUAL_INT(FPGA_MODE_STANDBY, s_next_mode);
  TEST_ASSERT_EQUAL_INT(FPGA_MODE_STANDBY, s_current_mode);

  s_next_mode = FPGA_MODE_PRESSURE_SUPPORT;
  ventilation_set_to_target_mode();
  TEST_ASSERT_EQUAL_INT(FPGA_MODE_PRESSURE_SUPPORT, s_next_mode);
  TEST_ASSERT_EQUAL_INT(FPGA_MODE_PRESSURE_SUPPORT, s_current_mode);

  s_next_mode = FPGA_MODE_STOP;
  ventilation_set_to_target_mode();
  TEST_ASSERT_EQUAL_INT(FPGA_MODE_STOP, s_next_mode);
  TEST_ASSERT_EQUAL_INT(FPGA_MODE_STOP, s_current_mode);
}

TEST(ventilation_tests, set_to_target_mode_resets_target_mode_if_invalid)
{
  s_next_mode = INT8_MAX;
  ventilation_set_to_target_mode();
  TEST_ASSERT_EQUAL_INT(0, state_machine_set_transition_list_fake.call_count);
  TEST_ASSERT_EQUAL_INT(FPGA_MODE_STANDBY, s_next_mode);

  RESET_FAKE(state_machine_set_transition_list);

  s_next_mode = INT8_MIN;
  ventilation_set_to_target_mode();
  TEST_ASSERT_EQUAL_INT(0, state_machine_set_transition_list_fake.call_count);
  TEST_ASSERT_EQUAL_INT(FPGA_MODE_STANDBY, s_next_mode);
}

TEST(ventilation_tests, set_to_target_mode_changes_transition_list)
{
  s_next_mode = FPGA_MODE_RUN;
  ventilation_set_to_target_mode();
  TEST_ASSERT_EQUAL_INT(1, state_machine_set_transition_list_fake.call_count);
  TEST_ASSERT_EQUAL_PTR(
    s_ventilation_state_machine, state_machine_set_transition_list_fake.arg0_val);
  TEST_ASSERT_EQUAL_PTR(
    &s_continuous_run_transitions, state_machine_set_transition_list_fake.arg1_val);

  RESET_FAKE(state_machine_set_transition_list);

  s_next_mode = FPGA_MODE_OFF;
  ventilation_set_to_target_mode();
  TEST_ASSERT_EQUAL_INT(1, state_machine_set_transition_list_fake.call_count);
  TEST_ASSERT_EQUAL_PTR(
    s_ventilation_state_machine, state_machine_set_transition_list_fake.arg0_val);
  TEST_ASSERT_EQUAL_PTR(
    &s_wait_at_home_transitions, state_machine_set_transition_list_fake.arg1_val);

  RESET_FAKE(state_machine_set_transition_list);

  s_next_mode = FPGA_MODE_PRESSURE_CONTROL;
  ventilation_set_to_target_mode();
  TEST_ASSERT_EQUAL_INT(1, state_machine_set_transition_list_fake.call_count);
  TEST_ASSERT_EQUAL_PTR(
    s_ventilation_state_machine, state_machine_set_transition_list_fake.arg0_val);
  TEST_ASSERT_EQUAL_PTR(
    &s_pcv_transitions, state_machine_set_transition_list_fake.arg1_val);

  RESET_FAKE(state_machine_set_transition_list);

  s_next_mode = FPGA_MODE_STANDBY;
  ventilation_set_to_target_mode();
  TEST_ASSERT_EQUAL_INT(1, state_machine_set_transition_list_fake.call_count);
  TEST_ASSERT_EQUAL_PTR(
    s_ventilation_state_machine, state_machine_set_transition_list_fake.arg0_val);
  TEST_ASSERT_EQUAL_PTR(
    &s_wait_at_home_transitions, state_machine_set_transition_list_fake.arg1_val);

  RESET_FAKE(state_machine_set_transition_list);

  s_next_mode = FPGA_MODE_PRESSURE_SUPPORT;
  ventilation_set_to_target_mode();
  TEST_ASSERT_EQUAL_INT(1, state_machine_set_transition_list_fake.call_count);
  TEST_ASSERT_EQUAL_PTR(
    s_ventilation_state_machine, state_machine_set_transition_list_fake.arg0_val);
  TEST_ASSERT_EQUAL_PTR(
    &s_psv_transitions, state_machine_set_transition_list_fake.arg1_val);

  RESET_FAKE(state_machine_set_transition_list);

  s_next_mode = FPGA_MODE_STOP;
  ventilation_set_to_target_mode();
  TEST_ASSERT_EQUAL_INT(1, state_machine_set_transition_list_fake.call_count);
  TEST_ASSERT_EQUAL_PTR(
    s_ventilation_state_machine, state_machine_set_transition_list_fake.arg0_val);
  TEST_ASSERT_EQUAL_PTR(
    &s_wait_at_home_transitions, state_machine_set_transition_list_fake.arg1_val);
}
