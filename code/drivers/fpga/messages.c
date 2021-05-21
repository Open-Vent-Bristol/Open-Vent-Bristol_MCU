// Copyright (c) 2021 <OpenVent-Bristol, Donald Robson>

#include "messages.h"
#include "display.h"
#include "private/fpga_priv.h"
#include "alarm/alarm.h"
#include "crc/crc.h"
#include "scheduler/dispatcher.h"
#include "sensor/sensor.h"
#include "spi/spi.h"
#include "state/system_event.h"
#include "scheduler/timer.h"
#include "misc/util.h"

TESTABLE enum fpga_operating_mode s_current_fpga_op_mode = FPGA_MODE_STANDBY;
TESTABLE uint32_t s_invalid_crc_count;
TESTABLE uint16_t s_previous_received_heartbeat;
TESTABLE message_mcu_to_fpga_t s_tx_message;

TESTABLE void message_fetch_from_fpga(int32_t arg);

/**
 * @brief Assemble a message and send it to the FPGA
 * @note The display should be formatted externally.
 *
 * @param arg   Unused
 */
TESTABLE void message_send_mcu_to_fpga(int32_t arg);

TESTABLE timer_t s_fpga_watchdog_timer =
{
  .type = TIMER_TYPE_ONE_SHOT,
  .initial_ticks = FPGA_WATCHDOG_INTERVAL_MS,
  .remaining_ticks = FPGA_WATCHDOG_INTERVAL_MS,
  .events_signalled = EV_FPGA_WATCHDOG_EXPIRY,
  .unique_id = TIMER_FPGA_WATCHDOG_EXPIRY
};

TESTABLE void message_fpga_watchdog_expiry(int32_t arg)
{
  dispatcher_clear_event_mask(1u << EV_FPGA_WATCHDOG_EXPIRY);

  // When the timer expires, the alarm will be set to the system failure mode.
  // Direct call to prevent signal being overwritten in a message race condition.
  alarm_mode(ALARM_SYSTEM_FAILURE);
}

void message_init(void)
{
  dispatcher_bind(1u << EV_FPGA_READY, message_fetch_from_fpga);
  dispatcher_bind(1u << EV_FPGA_SEND, message_send_mcu_to_fpga);
  dispatcher_bind(1u << EV_FPGA_WATCHDOG_EXPIRY, message_fpga_watchdog_expiry);
  timer_attach(&s_fpga_watchdog_timer);
}

void message_watchdog_enable(void)
{
  timer_reset(&s_fpga_watchdog_timer);
}

void message_watchdog_disable(void)
{
  timer_stop(&s_fpga_watchdog_timer);
}

void message_set_event_bits(mcu_event_mask_t event_mask)
{
  s_tx_message.event_mask |= event_mask;
}

void message_clear_event_bits(mcu_event_mask_t event_mask)
{
  s_tx_message.event_mask &= ~event_mask;
}

/**
 * @brief Unpack a message and send the contents to the required places via the dispatcher
 *
 * @param message     Received message
 * @return uint32_t   Running total of messages received with invalid CRC
 *                    (resets on success, i.e. 0 is no error)
 */
TESTABLE uint32_t message_process_fpga_to_mcu(const message_fpga_to_mcu_t* const message)
{
  // Reset the watchdog timer
  timer_reset(&s_fpga_watchdog_timer);

  // Check CRC-32
  uint32_t expected_crc32 = crc_calculate(&message, sizeof(message) - sizeof(message->crc32));

  if (message->crc32 != expected_crc32)
  {
    if (++s_invalid_crc_count >= FPGA_ALLOWED_INVALID_MESSAGES)
    {
      // Set alarm to the system failure mode.
      // Direct call to prevent signal being overwritten in a message race condition.
      alarm_mode(ALARM_SYSTEM_FAILURE);
    }

    return s_invalid_crc_count;
  }

  s_invalid_crc_count = 0u;

  s_previous_received_heartbeat = message->heartbeat;

  // Process FPGA events
  enum fpga_operating_mode new_mode = message->event_mask & FPGA_EVENT_MODE_MASK;
  if (new_mode != s_current_fpga_op_mode)
  {
    dispatcher_signal_event_mask(1u << EV_OP_MODE_CHANGE, new_mode);
    s_current_fpga_op_mode = new_mode;
  }

  // if ((message->event_mask & FPGA_EVENT_MOTOR_DISABLED) ||
  //     (message->event_mask & FPGA_EVENT_TIDAL_VOLUME_EXCEEDED))
  // {
  //   dispatcher_signal_event_mask(1u << EV_MOTOR_DISABLED, 1);
  // }
  // else
  // {
  //   dispatcher_signal_event_mask(1u << EV_MOTOR_DISABLED, 0);
  // }

  if (message->event_mask & FPGA_EVENT_ALARM_FAULT)
  {
    dispatcher_signal_event_mask(1u << EV_DO_SOUND_ALARM, ALARM_OVERRIDE_ON);
  }
  else
  {
    dispatcher_signal_event_mask(1u << EV_DO_SOUND_ALARM, ALARM_OVERRIDE_OFF);
  }

  // Process settings
  if (s_current_fpga_op_mode == FPGA_MODE_PRESSURE_CONTROL)
  {
    dispatcher_signal_event_mask(1u << EV_SET_POINT_UPDATE,
      ((uint32_t)message->setting_breath_rate << 16u) | message->setting_inhale_period_ds);
  }
  else if (s_current_fpga_op_mode == FPGA_MODE_PRESSURE_SUPPORT)
  {
    dispatcher_signal_event_mask(1u << EV_SET_POINT_UPDATE, message->setting_drive_pressure);
  }

  // Process sensor readings used by MCU
  dispatcher_signal_event_mask(1u << EV_FLOW_RATE_UPDATE, message->measured_flow_rate);
  dispatcher_signal_event_mask((1u << EV_PRESSURE_UPDATE) | (1u << EV_FAN_PRESSURE_UPDATE),
                                message->measured_pressure);

  // Store measurements displayed or logged
  sensor_store_reading(SENSOR_TIDAL_VOLUME, message->calculated_tidal_volume);
  sensor_store_reading(SENSOR_FLOW_RATE, message->measured_flow_rate);
  sensor_store_reading(SENSOR_PERCENT_O2, message->measured_percent_oxygen);
  sensor_store_reading(SENSOR_PRESSURE, message->measured_pressure);
  sensor_store_reading(SENSOR_TEMPERATURE, message->measured_temperature);

  dispatcher_signal_event_mask(1u << EV_DO_UPDATE_DISPLAY, 0);

  return 0u;
}

TESTABLE void message_fetch_from_fpga(int32_t arg)
{
  message_fpga_to_mcu_t message;
  if (spi_read(SPI_NO_COMMAND, (uint8_t* const)&message, sizeof(message)))
  {
    dispatcher_clear_event_mask(1u << EV_FPGA_READY);

    message_process_fpga_to_mcu(&message);
  }
}

TESTABLE void message_send_mcu_to_fpga(int32_t arg)
{
  fpga_heartbeat_calculate(&s_tx_message, s_previous_received_heartbeat);
  display_get(&s_tx_message);

  // Calculate the CRC-32
  s_tx_message.crc32 =
    crc_calculate(&s_tx_message, sizeof(s_tx_message) - sizeof(s_tx_message.crc32));

  // Send out via SPI
  if(spi_write(SPI_NO_COMMAND, (const uint8_t* const)&s_tx_message, sizeof(s_tx_message)))
  {
    dispatcher_clear_event_mask(1u << EV_FPGA_SEND);
  }
}
