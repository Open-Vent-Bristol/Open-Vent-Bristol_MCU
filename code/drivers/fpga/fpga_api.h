#ifndef FPGA_API_H
#define FPGA_API_H

#include <stdint.h>
#include <stdbool.h>

typedef uint16_t fpga_event_mask_t;
typedef uint16_t mcu_event_mask_t;

typedef struct
{
  fpga_event_mask_t event_mask;
  uint16_t measured_flow_rate;    // ml per second
  uint16_t measured_pressure;     // mmH2O
  uint16_t plateau_pressure;      // target pressure set by user, mmH2O
  uint16_t breath_rate;           // target breaths per minute
  uint16_t inhale_period_cs;      // target length of expiration in centiseconds
  uint16_t percent_oxygen;
  uint16_t tidal_volume;          // ml
  uint16_t padding_bytes;         // reserved bytes (note: value affects CRC calculation result)
  uint16_t heartbeat;
  uint32_t crc32;
} message_fpga_to_mcu_t;

typedef struct
{
  char display_lines[32];
  uint64_t battery_indicator;
  uint64_t pressure_bar_edge;
  uint64_t pressure_bar_peak;
  mcu_event_mask_t event_mask;
  uint16_t heartbeat;
  uint32_t crc32;
} message_mcu_to_fpga_t;

enum fpga_event_bits
{
  FPGA_EVENT_MODE_0                 = (1u << 0u),
  FPGA_EVENT_MODE_1                 = (1u << 1u),
  FPGA_EVENT_MODE_2                 = (1u << 2u),
  FPGA_EVENT_MODE_MASK              = (FPGA_EVENT_MODE_2 | FPGA_EVENT_MODE_1 | FPGA_EVENT_MODE_0),
  FPGA_EVENT_MOTOR_DISABLED         = (1u << 3u),    // The FPGA has disabled the motor
  FPGA_EVENT_TIDAL_VOLUME_EXCEEDED  = (1u << 4u),    // The tidal volume limit is exceeded
  FPGA_EVENT_ALARM_FAULT            = (1u << 5u),    // The FPGA requires the MCU to sound the alarm
  // FPGA_EVENT_RESERVED_6             = (1u << 6u),
  // FPGA_EVENT_RESERVED_7             = (1u << 7u),
  // FPGA_EVENT_RESERVED_8             = (1u << 8u),
  // FPGA_EVENT_RESERVED_9             = (1u << 9u),
  // FPGA_EVENT_RESERVED_10            = (1u << 10u),
  // FPGA_EVENT_RESERVED_11            = (1u << 11u),
  // FPGA_EVENT_RESERVED_12            = (1u << 12u),
  // FPGA_EVENT_RESERVED_13            = (1u << 13u),
  // FPGA_EVENT_RESERVED_14            = (1u << 14u),
  // FPGA_EVENT_RESERVED_15            = (1u << 15u)
};

/**
 * Bits 0-2 of enum fpga_event_bits
 */
enum fpga_operating_mode
{
  FPGA_MODE_OFF                     = 0u,                                       // Power off request approved
  FPGA_MODE_STANDBY                 = FPGA_EVENT_MODE_0,                        // Idle, power on
  FPGA_MODE_PRESSURE_CONTROL        = FPGA_EVENT_MODE_1,                        // Use set points to control ventilation
  FPGA_MODE_PRESSURE_SUPPORT        = (FPGA_EVENT_MODE_1 | FPGA_EVENT_MODE_0),  // Ventilation in response to patient demand
  FPGA_MODE_STOP                    = FPGA_EVENT_MODE_2,                        // Used only during calibration
  FPGA_MODE_RUN                     = (FPGA_EVENT_MODE_2 | FPGA_EVENT_MODE_0),  // Used only during calibration
};

enum mcu_event_bits
{
  MCU_EVENT_POWER_REQUEST_OFF       = (1u << 0u), // Request permission to remove power
  MCU_EVENT_POWER_REQUEST_ON        = (1u << 1u), // Default - should be set except to request off
  // MCU_EVENT_RESERVED_2              = (1u << 2u),
  MCU_EVENT_RESET_MOTOR             = (1u << 3u), // Reenables the motor when disabled by the FPGA
  MCU_EVENT_ACTUATOR_FAULT          = (1u << 4u), // The actuator is stuck
  MCU_EVENT_BAG_FAULT               = (1u << 5u), // The bag is damaged
  MCU_EVENT_BLOCKAGE_FAULT          = (1u << 6u), // There is a blockage in the airway
  MCU_EVENT_LEAK_FAULT              = (1u << 7u), // There is a leak in the airway
  MCU_EVENT_LIMIT_SWITCH_FAULT      = (1u << 8u), // The primary limit switch is not responding
  // MCU_EVENT_RESERVED_9              = (1u << 9u),
  // MCU_EVENT_RESERVED_10             = (1u << 10u),
  // MCU_EVENT_RESERVED_11             = (1u << 11u),
  // MCU_EVENT_RESERVED_12             = (1u << 12u),
  MCU_EVENT_BATTERY_STATE_0         = (1u << 13u),
  MCU_EVENT_BATTERY_STATE_1         = (1u << 14u),
  MCU_EVENT_BATTERY_STATE_MASK      = (MCU_EVENT_BATTERY_STATE_1 | MCU_EVENT_BATTERY_STATE_0),
  // MCU_EVENT_RESERVED_15             = (1u << 15u)
};

/**
 * Bits 13-14 of enum mcu_event_bits
 */
enum battery_status
{
  BATTERY_STATUS_MAINS        = 0u,                                                       // The battery is charging (or charged)
  BATTERY_STATUS_DISCHARGING  = MCU_EVENT_BATTERY_STATE_0,                                // The battery is discharging
  BATTERY_STATUS_LOW          = MCU_EVENT_BATTERY_STATE_1,                                // The battery requires charging urgently
  BATTERY_STATUS_CRITICAL     = (MCU_EVENT_BATTERY_STATE_1 | MCU_EVENT_BATTERY_STATE_0),  // Battery power will be imminently withdrawn
};

/**
 * Calculate the heartbeat word required to prevent the FPGA resetting the MCU and insert it into
 * the message
 *
 * @param message_to_format Pointer to the message that will be set to the FPGA
 * @param received_heartbeat Heartbeat extracted from the last message received from the FPGA
 */
inline void fpga_heartbeat_calculate(
  message_mcu_to_fpga_t* const message_to_format, uint16_t received_heartbeat)
{
  message_to_format->heartbeat = ~received_heartbeat + 1u;
}

/**
 * Display formatting functions.  These will update the cached version of the display
 */
void fpga_display_format_tidal_volume(uint16_t tidal_volume_ml);
void fpga_display_format_peak_flow(uint16_t peak_flow_ml_per_sec);
void fpga_display_format_respiration_rate(uint8_t breaths_per_min);
void fpga_display_format_percent_o2(uint8_t oxygen_ppthou);
void fpga_display_format_battery_gauge(uint8_t charge_percent);
void fpga_display_format_pressure_bar(int16_t pressure_mmH2O, uint16_t peak_pressure_mmH2O);

/**
 * Check if the cached display has been changed (to avoid needing to call display_get)
 *
 * @return true Cached version of display has been updated
 * @return false Cached version of display is unchanged
 */
bool fpga_display_has_changed(void);

/**
 * Format a message with a copy of the cached display (including custom characters)
 *
 * @param message_mcu_to_fpga_t* Pointer to the message
 */
void fpga_display_get(message_mcu_to_fpga_t* const message_to_format);

#endif /* FPGA_API_H */
