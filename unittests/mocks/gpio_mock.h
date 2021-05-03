// Copyright (c) 2021 <OpenVent-Bristol, Donald Robson>

#ifndef GPIO_MOCK_H
#define GPIO_MOCK_H

#include "gpio/gpio.h"
#include "fff.h"

#define FFF_GCC_FUNCTION_ATTRIBUTES

FAKE_VOID_FUNC(gpio_set_mask, gpio_register_t, register_size_t);

FAKE_VOID_FUNC(gpio_clear_mask, gpio_register_t, register_size_t);

FAKE_VOID_FUNC(gpio_write_mask, gpio_register_t, register_size_t);

FAKE_VALUE_FUNC(register_size_t, gpio_read_mask, gpio_register_t, register_size_t);

FAKE_VOID_FUNC(gpio_set_pin, gpio_register_t, uint8_t);

FAKE_VOID_FUNC(gpio_clear_pin, gpio_register_t, uint8_t);

FAKE_VALUE_FUNC(uint8_t, gpio_read_pin, gpio_register_t, uint8_t);

#define GPIO_MOCKS(x) \
  x(gpio_set_mask) \
  x(gpio_clear_mask) \
  x(gpio_write_mask) \
  x(gpio_read_mask) \
  x(gpio_set_pin) \
  x(gpio_clear_pin) \
  x(gpio_read_pin)

#endif /* GPIO_MOCK_H */
