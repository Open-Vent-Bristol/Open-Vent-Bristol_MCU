// Copyright (c) 2021 <OpenVent-Bristol, Donald Robson>

#ifndef UART_PRIV_H
#define UART_PRIV_H

#include "board/board.h"
#include <stdint.h>

typedef struct
{
  uint8_t read_position;
  uint8_t write_position;
  uint8_t length;
  uint8_t data[UART_BUFFER_SIZE];
} circular_buffer_t;

extern circular_buffer_t rx_buffer;
extern circular_buffer_t tx_buffer;

/** @brief Setup the UART device on the MCU (mode, baud rate, etc)
 *
 */
void uart_configure(void);

/**
 * @brief It may be necessary to kick off the first byte of a transmission, so that interrupts can then take over.
 * Must be stubbed if unused.
 *
 */
void uart_start_transmission(void);

#endif /* UART_PRIV_H */
