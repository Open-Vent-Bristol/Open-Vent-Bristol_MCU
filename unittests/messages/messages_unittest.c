// Copyright (c) 2021 <OpenVent-Bristol, Donald Robson>

#include "unity.h"
#include "unity_fixture.h"
#include "fpga/messages.h"
#include "crc_mock.h"
#include "dispatcher_mock.h"
#include "display_mock.h"
#include "sensor_mock.h"
#include "spi_mock.h"
#include "test_macros.h"
#include <string.h>

extern enum fpga_operating_mode s_current_fpga_op_mode;
extern uint32_t s_invalid_crc_count;
extern uint16_t s_previous_received_heartbeat;
extern message_mcu_to_fpga_t s_tx_message;

static uint32_t crc_calculate_12345678(const void *const data, size_t byte_length)
{
  (void)data;
  (void)byte_length;

  return 0x12345678;
}

TEST_GROUP(messages_tests);

TEST_SETUP(messages_tests)
{
  s_current_fpga_op_mode = FPGA_MODE_STANDBY;
  s_invalid_crc_count = 0;
  s_previous_received_heartbeat = 0;
  memset(&s_tx_message, 0, sizeof(s_tx_message));

  CRC_MOCKS(RESET_FAKE);
  DISPATCHER_MOCKS(RESET_FAKE);
  DISPLAY_MOCKS(RESET_FAKE);
  SENSOR_MOCKS(RESET_FAKE);
  SPI_MOCKS(RESET_FAKE);
}

TEST_TEAR_DOWN(messages_tests)
{}

TEST(messages_tests, process_fpga_to_mcu_aborts_for_invalid_crc)
{
  crc_calculate_fake.custom_fake = crc_calculate_12345678;

  message_fpga_to_mcu_t test_message =
  {
    .crc32 = 0x56781234,
  };

  uint32_t count = message_process_fpga_to_mcu(&test_message);
  TEST_ASSERT_EQUAL_INT(1u, count);

  count = message_process_fpga_to_mcu(&test_message);
  TEST_ASSERT_EQUAL_INT(2u, count);
}

TEST(messages_tests, process_fpga_to_mcu_resets_invalid_crc_count)
{
  crc_calculate_fake.custom_fake = crc_calculate_12345678;

  message_fpga_to_mcu_t test_message =
  {
    .crc32 = 0x56781234,
  };

  uint32_t count = message_process_fpga_to_mcu(&test_message);
  TEST_ASSERT_EQUAL_INT(1u, count);

  test_message.crc32 = 0x12345678;

  count = message_process_fpga_to_mcu(&test_message);
  TEST_ASSERT_EQUAL_INT(0u, count);
}

TEST(messages_tests, process_fpga_to_mcu_stores_heartbeat)
{
  message_fpga_to_mcu_t test_message =
  {
    .heartbeat = 65432,
  };

  message_process_fpga_to_mcu(&test_message);
  TEST_ASSERT_EQUAL_INT(65432, s_previous_received_heartbeat);
}

TEST(messages_tests, process_fpga_to_mcu_signals_mode_change_event)
{
  message_fpga_to_mcu_t test_message =
  {
    .event_mask = FPGA_MODE_PRESSURE_SUPPORT,
  };

  message_process_fpga_to_mcu(&test_message);
  TEST_ASSERT_VALUE_IN_ARRAY(
    1u << EV_OP_MODE_CHANGE, dispatcher_signal_event_mask_fake.arg0_history);
  TEST_ASSERT_VALUE_IN_ARRAY(
    FPGA_MODE_PRESSURE_SUPPORT, dispatcher_signal_event_mask_fake.arg1_history);

  RESET_FAKE(dispatcher_signal_event_mask);
  test_message.event_mask = FPGA_MODE_PRESSURE_CONTROL;

  message_process_fpga_to_mcu(&test_message);
  TEST_ASSERT_VALUE_IN_ARRAY(
    1u << EV_OP_MODE_CHANGE, dispatcher_signal_event_mask_fake.arg0_history);
  TEST_ASSERT_VALUE_IN_ARRAY(
    FPGA_MODE_PRESSURE_CONTROL, dispatcher_signal_event_mask_fake.arg1_history);
}

TEST(messages_tests, process_fpga_to_mcu_signals_set_point_change)
{
  message_fpga_to_mcu_t test_message =
  {
    .setting_drive_pressure = 101,
    .setting_breath_rate = 17,
    .setting_inhale_period_ds = 29,
    .event_mask = FPGA_MODE_PRESSURE_SUPPORT,
  };

  message_process_fpga_to_mcu(&test_message);
  TEST_ASSERT_VALUE_IN_ARRAY(
    1u << EV_SET_POINT_UPDATE, dispatcher_signal_event_mask_fake.arg0_history);
  TEST_ASSERT_VALUE_IN_ARRAY(
    101, dispatcher_signal_event_mask_fake.arg1_history);

  RESET_FAKE(dispatcher_signal_event_mask);
  test_message.event_mask = FPGA_MODE_PRESSURE_CONTROL;

  message_process_fpga_to_mcu(&test_message);
  TEST_ASSERT_VALUE_IN_ARRAY(
    1u << EV_SET_POINT_UPDATE, dispatcher_signal_event_mask_fake.arg0_history);
  TEST_ASSERT_VALUE_IN_ARRAY(
    ((17 << 16) | 29), dispatcher_signal_event_mask_fake.arg1_history);
}

TEST(messages_tests, process_fpga_to_mcu_signals_control_variable_change)
{
  message_fpga_to_mcu_t test_message =
  {
    .measured_pressure = 71,
    .measured_flow_rate = 47,
  };

  message_process_fpga_to_mcu(&test_message);

  TEST_ASSERT_VALUE_IN_ARRAY(
    1u << EV_PRESSURE_UPDATE, dispatcher_signal_event_mask_fake.arg0_history);
  TEST_ASSERT_VALUE_IN_ARRAY(
    71, dispatcher_signal_event_mask_fake.arg1_history);

  TEST_ASSERT_VALUE_IN_ARRAY(
    1u << EV_FLOW_RATE_UPDATE, dispatcher_signal_event_mask_fake.arg0_history);
  TEST_ASSERT_VALUE_IN_ARRAY(
    47, dispatcher_signal_event_mask_fake.arg1_history);
}

TEST(messages_tests, process_fpga_to_mcu_stores_sensor_readings)
{
  message_fpga_to_mcu_t test_message = {0};

  message_process_fpga_to_mcu(&test_message);
  TEST_ASSERT_VALUE_IN_ARRAY(SENSOR_TIDAL_VOLUME, sensor_store_reading_fake.arg0_history);
  TEST_ASSERT_VALUE_IN_ARRAY(SENSOR_FLOW_RATE, sensor_store_reading_fake.arg0_history);
  TEST_ASSERT_VALUE_IN_ARRAY(SENSOR_PERCENT_O2, sensor_store_reading_fake.arg0_history);
  TEST_ASSERT_VALUE_IN_ARRAY(SENSOR_PRESSURE, sensor_store_reading_fake.arg0_history);
}

TEST(messages_tests, send_mcu_to_fpga_sets_event_mask)
{
  message_send_mcu_to_fpga(1234);
  TEST_ASSERT_EQUAL_INT(1234, s_tx_message.event_mask);
}

TEST(messages_tests, send_mcu_to_fpga_sets_heartbeat)
{
  s_previous_received_heartbeat = 5678;

  message_send_mcu_to_fpga(0);
  TEST_ASSERT_EQUAL_INT((uint16_t)-5678, s_tx_message.heartbeat);
}

TEST(messages_tests, send_mcu_to_fpga_copies_cached_display)
{
  message_send_mcu_to_fpga(0);
  TEST_ASSERT_EQUAL_INT(1, display_get_fake.call_count);
  TEST_ASSERT_EQUAL_PTR(&s_tx_message, display_get_fake.arg0_val);
}

TEST(messages_tests, send_mcu_to_fpga_sets_crc)
{
  crc_calculate_fake.custom_fake = crc_calculate_12345678;

  message_send_mcu_to_fpga(0);
  TEST_ASSERT_EQUAL_INT(1, crc_calculate_fake.call_count);
  TEST_ASSERT_EQUAL_PTR(0x12345678, s_tx_message.crc32);
}

TEST(messages_tests, send_mcu_to_fpga_sends_via_spi)
{
  message_send_mcu_to_fpga(0);
  TEST_ASSERT_EQUAL_INT(1, spi_write_fake.call_count);
  TEST_ASSERT_EQUAL_PTR(&s_tx_message, spi_write_fake.arg1_val);
}
