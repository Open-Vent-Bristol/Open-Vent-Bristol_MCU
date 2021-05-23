// Copyright (c) 2021 <OpenVent-Bristol, Donald Robson>

#include "unity.h"
#include "unity_fixture.h"
#include "fpga/private/fpga_priv.h"
#include "fpga/messages.h"
#include "alarm_mock.h"
#include "crc_mock.h"
#include "dispatcher_mock.h"
#include "display_mock.h"
#include "sensor_mock.h"
#include "spi_mock.h"
#include "test_macros.h"
#include "timer_mock.h"
#include <string.h>
#include <stdbool.h>

extern enum fpga_operating_mode s_current_fpga_op_mode;
extern uint32_t s_invalid_crc_count;
extern uint16_t s_previous_received_heartbeat;
extern message_mcu_to_fpga_t s_tx_message;
extern timer_t s_fpga_watchdog_timer;

extern void message_fpga_watchdog_expiry(int32_t arg);

extern uint32_t message_process_fpga_to_mcu(const message_fpga_to_mcu_t* const message);
extern void message_fetch_from_fpga(int32_t arg);
extern void message_send_mcu_to_fpga(int32_t arg);

static uint32_t crc_calculate_12345678(const void *const data, size_t byte_length)
{
  (void)data;
  (void)byte_length;

  return 0x12345678;
}

static bool returns_true(uint8_t command, uint8_t* const data_in, uint8_t max_length)
{
  (void)command;
  (void)data_in;
  (void)max_length;

  return true;
}

TEST_GROUP(messages_tests);

TEST_SETUP(messages_tests)
{
  s_current_fpga_op_mode = FPGA_MODE_STANDBY;
  s_invalid_crc_count = 0;
  s_previous_received_heartbeat = 0;
  memset(&s_tx_message, 0, sizeof(s_tx_message));

  ALARM_MOCKS(RESET_FAKE);
  CRC_MOCKS(RESET_FAKE);
  DISPATCHER_MOCKS(RESET_FAKE);
  DISPLAY_FORMAT_MOCKS(RESET_FAKE);
  SENSOR_MOCKS(RESET_FAKE);
  SPI_MOCKS(RESET_FAKE);
  TIMER_MOCKS(RESET_FAKE);
}

TEST_TEAR_DOWN(messages_tests)
{}

TEST(messages_tests, fpga_watchdog_expiry_sets_system_failure_alarm)
{
  message_fpga_watchdog_expiry(0);
  TEST_ASSERT_EQUAL_INT(1u, alarm_mode_fake.call_count);
  TEST_ASSERT_EQUAL_INT(ALARM_SYSTEM_FAILURE, alarm_mode_fake.arg0_val);
}

TEST(messages_tests, fpga_watchdog_expiry_clears_event)
{
  message_fpga_watchdog_expiry(0);
  TEST_ASSERT_EQUAL_INT(1u, dispatcher_clear_event_mask_fake.call_count);
  TEST_ASSERT_EQUAL_INT(1u << EV_FPGA_WATCHDOG_EXPIRY, dispatcher_clear_event_mask_fake.arg0_val);
}

TEST(messages_tests, init_binds_events)
{
  message_init();
  TEST_ASSERT_EQUAL_INT(3, dispatcher_bind_fake.call_count);
  TEST_ASSERT_VALUE_IN_ARRAY(1u << EV_FPGA_READY, dispatcher_bind_fake.arg0_history);
  TEST_ASSERT_VALUE_IN_ARRAY(1u << EV_FPGA_SEND, dispatcher_bind_fake.arg0_history);
  TEST_ASSERT_VALUE_IN_ARRAY(1u << EV_FPGA_WATCHDOG_EXPIRY, dispatcher_bind_fake.arg0_history);
}

TEST(messages_tests, init_attaches_timer)
{
  message_init();
  TEST_ASSERT_EQUAL_INT(1, timer_attach_fake.call_count);
  TEST_ASSERT_EQUAL_PTR(&s_fpga_watchdog_timer, timer_attach_fake.arg0_val);
}

TEST(messages_tests, watchdog_enable_resets_fpga_watchdog_timer)
{
  message_watchdog_enable();
  TEST_ASSERT_EQUAL_INT(1, timer_reset_fake.call_count);
  TEST_ASSERT_EQUAL_PTR(&s_fpga_watchdog_timer, timer_reset_fake.arg0_val);
}

TEST(messages_tests, watchdog_disable_stops_fpga_watchdog_timer)
{
  message_watchdog_disable();
  TEST_ASSERT_EQUAL_INT(1, timer_stop_fake.call_count);
  TEST_ASSERT_EQUAL_PTR(&s_fpga_watchdog_timer, timer_stop_fake.arg0_val);
}

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
  TEST_ASSERT_EQUAL_INT(2u, s_invalid_crc_count);
}

TEST(messages_tests, process_fpga_to_mcu_several_invalid_crc_sets_system_failure_alarm)
{
  crc_calculate_fake.custom_fake = crc_calculate_12345678;

  message_fpga_to_mcu_t test_message =
  {
    .crc32 = 0x56781234,
  };

  uint32_t count = 0u;

  for (size_t i = 0u; i < FPGA_ALLOWED_INVALID_MESSAGES; i++)
  {
    count = message_process_fpga_to_mcu(&test_message);
  }

  TEST_ASSERT_EQUAL_INT(5u, count);
  TEST_ASSERT_EQUAL_INT(5u, s_invalid_crc_count);

  TEST_ASSERT_EQUAL_INT(1u, alarm_mode_fake.call_count);
  TEST_ASSERT_EQUAL_INT(ALARM_SYSTEM_FAILURE, alarm_mode_fake.arg0_val);
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
  TEST_ASSERT_EQUAL_INT(0u, s_invalid_crc_count);
}

TEST(messages_tests, process_fpga_to_mcu_resets_fpga_watchdog_timer)
{
  message_fpga_to_mcu_t test_message = {0};

  message_process_fpga_to_mcu(&test_message);
  TEST_ASSERT_EQUAL_INT(0u, s_invalid_crc_count);
  TEST_ASSERT_EQUAL_INT(1, timer_reset_fake.call_count);
  TEST_ASSERT_EQUAL_PTR(&s_fpga_watchdog_timer, timer_reset_fake.arg0_val);
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
    .measured_temperature = 29,
  };

  message_process_fpga_to_mcu(&test_message);

  TEST_ASSERT_VALUE_IN_ARRAY(
    ((1u << EV_PRESSURE_UPDATE) | (1u << EV_FAN_PRESSURE_UPDATE)),
    dispatcher_signal_event_mask_fake.arg0_history);
  TEST_ASSERT_VALUE_IN_ARRAY(
    71, dispatcher_signal_event_mask_fake.arg1_history);

  TEST_ASSERT_VALUE_IN_ARRAY(
    1u << EV_FLOW_RATE_UPDATE, dispatcher_signal_event_mask_fake.arg0_history);
  TEST_ASSERT_VALUE_IN_ARRAY(
    47, dispatcher_signal_event_mask_fake.arg1_history);
}

TEST(messages_tests, process_fpga_to_mcu_signals_display_update)
{
  message_fpga_to_mcu_t test_message = {0};

  message_process_fpga_to_mcu(&test_message);

  TEST_ASSERT_VALUE_IN_ARRAY(
    (1u << EV_DO_UPDATE_DISPLAY), dispatcher_signal_event_mask_fake.arg0_history);
}

TEST(messages_tests, process_fpga_to_mcu_stores_sensor_readings)
{
  message_fpga_to_mcu_t test_message = {0};

  message_process_fpga_to_mcu(&test_message);
  TEST_ASSERT_VALUE_IN_ARRAY(SENSOR_TIDAL_VOLUME, sensor_store_reading_fake.arg0_history);
  TEST_ASSERT_VALUE_IN_ARRAY(SENSOR_FLOW_RATE, sensor_store_reading_fake.arg0_history);
  TEST_ASSERT_VALUE_IN_ARRAY(SENSOR_PERCENT_O2, sensor_store_reading_fake.arg0_history);
  TEST_ASSERT_VALUE_IN_ARRAY(SENSOR_PRESSURE, sensor_store_reading_fake.arg0_history);
  TEST_ASSERT_VALUE_IN_ARRAY(SENSOR_TEMPERATURE, sensor_store_reading_fake.arg0_history);
}

TEST(messages_tests, fetch_from_fpga_clears_event_only_when_spi_receives)
{
  message_fetch_from_fpga(0);
  TEST_ASSERT_EQUAL(0u, dispatcher_clear_event_mask_fake.call_count);

  spi_read_fake.custom_fake = returns_true;
  message_fetch_from_fpga(0);
  TEST_ASSERT_EQUAL(1u, dispatcher_clear_event_mask_fake.call_count);
  TEST_ASSERT_EQUAL(1u << EV_FPGA_READY, dispatcher_clear_event_mask_fake.arg0_val);
}

TEST(messages_tests, fetch_from_fpga_processes_message_when_spi_receives)
{
  message_fetch_from_fpga(0);
  TEST_ASSERT_EQUAL(0u, dispatcher_signal_event_mask_fake.call_count);

  spi_read_fake.custom_fake = returns_true;
  message_fetch_from_fpga(0);
  TEST_ASSERT_GREATER_THAN(0u, dispatcher_signal_event_mask_fake.call_count);
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
  TEST_ASSERT_EQUAL_INT(1, display_format_get_fake.call_count);
  TEST_ASSERT_EQUAL_PTR(&s_tx_message, display_format_get_fake.arg0_val);
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
