// Copyright (c) 2021 <OpenVent-Bristol, Donald Robson>

#ifndef MESSAGES_H
#define MESSAGES_H

#include "fpga_api.h"
#include <stdint.h>

/**
 * @brief Initialise message processing (also starts FPGA failure watchdog)
 */
void message_init(void);

/**
 * @brief Start the FPGA failure watchdog
 */
void message_watchdog_enable(void);

/**
 * @brief Suspend the FPGA failure watchdog (for use during off mode or firmware update)
 */
void message_watchdog_disable(void);

/**
 * @brief Set bits in the MCU event_mask that is sent in messages to the FPGA
 *
 * @param event_mask See enum mcu_event_bits - not the same as the system_event mask!
 */
void message_set_event_bits(mcu_event_mask_t event_mask);

/**
 * @brief Clear bits in the MCU event_mask that is sent in messages to the FPGA
 *
 * @param event_mask See enum mcu_event_bits - not the same as the system_event mask!
 */
void message_clear_event_bits(mcu_event_mask_t event_mask);

#endif /* MESSAGES_H */
