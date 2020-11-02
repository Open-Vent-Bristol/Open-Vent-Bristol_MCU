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
  uint16_t exhale_inhale_ratio;   // target length of expiration x10 (unitless)
  uint16_t percent_oxygen;
  uint16_t tidal_volume;          // ml
  uint16_t padding_bytes;         // value only incidental in CRC calculation
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
  FPGA_EVENT_0 = (1u << 0u),
  FPGA_EVENT_1 = (1u << 1u),
  FPGA_EVENT_2 = (1u << 2u),
  FPGA_EVENT_3 = (1u << 3u),
  FPGA_EVENT_4 = (1u << 4u),
  FPGA_EVENT_5 = (1u << 5u),
  FPGA_EVENT_6 = (1u << 6u),
  FPGA_EVENT_7 = (1u << 7u),
  FPGA_EVENT_8 = (1u << 8u),
  FPGA_EVENT_9 = (1u << 9u),
  FPGA_EVENT_10 = (1u << 10u),
  FPGA_EVENT_11 = (1u << 11u),
  FPGA_EVENT_12 = (1u << 12u),
  FPGA_EVENT_13 = (1u << 13u),
  FPGA_EVENT_14 = (1u << 14u),
  FPGA_EVENT_15 = (1u << 15u)
};

enum mcu_event_bits
{
  MCU_EVENT_0 = (1u << 0u),
  MCU_EVENT_1 = (1u << 1u),
  MCU_EVENT_2 = (1u << 2u),
  MCU_EVENT_3 = (1u << 3u),
  MCU_EVENT_4 = (1u << 4u),
  MCU_EVENT_5 = (1u << 5u),
  MCU_EVENT_6 = (1u << 6u),
  MCU_EVENT_7 = (1u << 7u),
  MCU_EVENT_8 = (1u << 8u),
  MCU_EVENT_9 = (1u << 9u),
  MCU_EVENT_10 = (1u << 10u),
  MCU_EVENT_11 = (1u << 11u),
  MCU_EVENT_12 = (1u << 12u),
  MCU_EVENT_13 = (1u << 13u),
  MCU_EVENT_14 = (1u << 14u),
  MCU_EVENT_15 = (1u << 15u)
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
  // TODO - implementation to be confirmed - probably bitwise NOT as below
  message_to_format->heartbeat = ~received_heartbeat;
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

/** Format a message with a copy of the cached display (including custom characters)
 *
 * @param message_mcu_to_fpga_t* Pointer to the message
 */
void fpga_display_get(message_mcu_to_fpga_t* const message_to_format);

#endif /* FPGA_API_H */
