// Copyright (c) 2021 <OpenVent-Bristol, Donald Robson>

#ifndef FPGA_API_H
#define FPGA_API_H

#include <stdint.h>
#include <stdbool.h>

#define FPGA_UPDATE_INTERVAL_MS (5u)

typedef uint16_t fpga_event_mask_t;
typedef uint16_t mcu_event_mask_t;

/**
 * @brief Description of messages received by the MCU from the FPGA
 */
typedef struct
{
  fpga_event_mask_t event_mask;       // See enum fpga_event_bits
  uint16_t measured_flow_rate;        // decilitres per minute
  uint16_t measured_pressure;         // cmH2O
  uint16_t setting_drive_pressure;    // target pressure set by user, cmH2O
  uint16_t setting_breath_rate;       // target breaths per minute
  uint16_t setting_inhale_period_ds;  // target length of expiration in tenths of seconds
  uint16_t measured_percent_oxygen;
  uint16_t calculated_tidal_volume;   // millilitres
  uint16_t measured_temperature;      // degrees Celsius
  uint16_t heartbeat;                 // this must 2s complemented and sent back in the reply
  uint32_t crc32;                     // CRC-32 ETHERNET to verify message integrity
} message_fpga_to_mcu_t;

/**
 * @brief Description of messages sent by the MCU to the FPGA
 */
typedef struct
{
  char display_lines[32];       // Characters for display on measured value display. Not null terminated.
  uint64_t battery_indicator;   // Character representation of battery display (see display.c)
  uint64_t pressure_bar_edge;   // Character representation of pressure bar end (see display.c)
  uint64_t pressure_bar_peak;   // Character representation of pressure bar peak marker (see display.c)
  mcu_event_mask_t event_mask;  // See enum mcu_event_bits
  uint16_t heartbeat;           // 2s complement of the last heartbeat value received from the fpga
  uint32_t crc32;               // CRC-32 ETHERNET to verify message integrity
} message_mcu_to_fpga_t;

enum fpga_event_bits
{
  FPGA_EVENT_MODE_0                       = (1u << 0u),    // See enum fpga_operating_mode
  FPGA_EVENT_MODE_1                       = (1u << 1u),    // See enum fpga_operating_mode
  FPGA_EVENT_MODE_2                       = (1u << 2u),    // See enum fpga_operating_mode
  FPGA_EVENT_MODE_MASK                    =
    (FPGA_EVENT_MODE_2 | FPGA_EVENT_MODE_1 | FPGA_EVENT_MODE_0),

  FPGA_EVENT_MOTOR_DISABLED               = (1u << 3u),    // The FPGA has disabled the motor
  FPGA_EVENT_TIDAL_VOLUME_EXCEEDED        = (1u << 4u),    // The tidal volume limit is exceeded
  FPGA_EVENT_ALARM_FAULT                  = (1u << 5u),    // The MCU should sound its own alarm

  FPGA_EVENT_DISPLAY_OVERRIDE_0           = (1u << 6u),    // See enum display_override_index
  FPGA_EVENT_DISPLAY_OVERRIDE_1           = (1u << 7u),    // See enum display_override_index
  FPGA_EVENT_DISPLAY_OVERRIDE_2           = (1u << 8u),    // See enum display_override_index
  FPGA_EVENT_DISPLAY_OVERRIDE_3           = (1u << 9u),    // See enum display_override_index
  FPGA_EVENT_DISPLAY_OVERRIDE_MASK        =
    (FPGA_EVENT_DISPLAY_OVERRIDE_3 | FPGA_EVENT_DISPLAY_OVERRIDE_2 |
     FPGA_EVENT_DISPLAY_OVERRIDE_1 | FPGA_EVENT_DISPLAY_OVERRIDE_0),

  FPGA_EVENT_DISPLAY_LINE_2_OVERRIDE_0    = (1u << 10u),   // See enum display_override_line_2_index
  FPGA_EVENT_DISPLAY_LINE_2_OVERRIDE_1    = (1u << 11u),   // See enum display_override_line_2_index
  FPGA_EVENT_DISPLAY_LINE_2_OVERRIDE_2    = (1u << 12u),   // See enum display_override_line_2_index
  FPGA_EVENT_DISPLAY_LINE_2_OVERRIDE_3    = (1u << 13u),   // See enum display_override_line_2_index
  FPGA_EVENT_DISPLAY_LINE_2_OVERRIDE_4    = (1u << 14u),   // See enum display_override_line_2_index
  FPGA_EVENT_DISPLAY_LINE_2_OVERRIDE_5    = (1u << 15u),   // See enum display_override_line_2_index
  FPGA_EVENT_DISPLAY_LINE_2_OVERRIDE_MASK =
    (FPGA_EVENT_DISPLAY_LINE_2_OVERRIDE_5 | FPGA_EVENT_DISPLAY_LINE_2_OVERRIDE_4 |
     FPGA_EVENT_DISPLAY_LINE_2_OVERRIDE_3 | FPGA_EVENT_DISPLAY_LINE_2_OVERRIDE_2 |
     FPGA_EVENT_DISPLAY_LINE_2_OVERRIDE_1 | FPGA_EVENT_DISPLAY_LINE_2_OVERRIDE_0),
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

/**
 * Bits 6-9 of enum fpga_even t_bits
 * String overrides for the display.
 * Strings might affect only line 1, as indicated.
 */
enum display_override_index
{
  DISPLAY_OVERRIDE_NONE               = 0u,
  DISPLAY_OVERRIDE_HOLD_MUTE_SEL      = 1u,
  DISPLAY_OVERRIDE_LINE1_QUICK_CALIB  = 2u,
  DISPLAY_OVERRIDE_LINE1_FULL_CALIB   = 3u,
  DISPLAY_OVERRIDE_LINE1_TUBES        = 4u,
  DISPLAY_OVERRIDE_LINE1_REDUCE_O2    = 5u,
  DISPLAY_OVERRIDE_LINE1_INCREASE_O2  = 6u,
  DISPLAY_OVERRIDE_LINE1_REMOVE_O2    = 7u,
  DISPLAY_OVERRIDE_LINE1_CONNECT_N2   = 8u,
  DISPLAY_OVERRIDE_LINE1_PLEASE_WAIT  = 9u,
};

/**
 * Bits 10-15 of enum fpga_even t_bits
 * String overrides for the display line 2.
 * These will have no effect if the main override value (see above) is DISPLAY_OVERRIDE_NONE
 * To display a progress bar, the value should be between 15 (zero length) and 31 (full length)
 */
enum display_override_line_2_index
{
  DISPLAY_OVERRIDE_LINE2_NONE         = 0u,
  DISPLAY_OVERRIDE_LINE2_STANDBY      = 1u,
  DISPLAY_OVERRIDE_LINE2_CONFIRM      = 2u,
  DISPLAY_OVERRIDE_LINE2_SUCCESS      = 3u,
  DISPLAY_OVERRIDE_LINE2_FAILED       = 4u,
  /* Values 15-31 reserved for progress bar length */
  DISPLAY_OVERRIDE_LINE2_EMPTY        = 15u,
  DISPLAY_OVERRIDE_LINE2_FULL         = 31u,
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
  // MCU_EVENT_RESERVED_13             = (1u << 13u),
  // MCU_EVENT_RESERVED_14             = (1u << 14u),
  // MCU_EVENT_RESERVED_15             = (1u << 15u)
};

/**
 * Calculate the heartbeat word required to prevent the FPGA resetting the MCU and insert it into
 * the message
 *
 * @param message_to_format Pointer to the message that will be set to the FPGA
 * @param received_heartbeat Heartbeat extracted from the last message received from the FPGA
 */
static inline void fpga_heartbeat_calculate(
  message_mcu_to_fpga_t* const message_to_format, uint16_t received_heartbeat)
{
  message_to_format->heartbeat = ~received_heartbeat + 1u;
}

#endif /* FPGA_API_H */
