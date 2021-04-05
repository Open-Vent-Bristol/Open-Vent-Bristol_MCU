// Copyright (c) 2021 <OpenVent-Bristol, Donald Robson>

#ifndef PINMUX_H
#define PINMUX_H

#include <stddef.h>
#include <stdint.h>

/**
 * @brief pinmux_pin_t constructed from the port number (A=0, B=1, C=2, etc) shifted
 * shifted four bits, ORd with the pin number
 */
typedef uint8_t pinmux_pin_t;

typedef struct
{
  pinmux_pin_t pin;
  uint16_t flags; // See chip specific pinmux header, e.g. pinmux_stm32l4.h
} pinmux_cfg_t;

/**
 * @brief Setup all GPIO pin signals, GPIO interrupts and analogue channels according to the passed
 * array.
 */
void pinmux_init(void);

#endif /* PINMUX_H */
