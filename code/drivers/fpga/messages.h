// Copyright (c) 2021 <OpenVent-Bristol, Donald Robson>

#ifndef MESSAGES_H
#define MESSAGES_H

#include "fpga_api.h"
#include <stdint.h>

/**
 * @brief Unpack a message and send the contents to the required places via the dispatcher
 *
 * @param message     Received message
 * @return uint32_t   Running total of messages received with invalid CRC
 *                    (resets on success, i.e. 0 is no error)
 */
uint32_t message_process_fpga_to_mcu(const message_fpga_to_mcu_t* const message);

/**
 * @brief Assemble a message and send it to the FPGA
 * @note The display should be formatted prior to calling this function, using the display API
 * functions.
 *
 * @param event_mask See enum mcu_event_bits - should be preassembled (it is not the same as the
 *                   system_event mask!)
 */
void message_send_mcu_to_fpga(mcu_event_mask_t event_mask);

#endif /* MESSAGES_H */
