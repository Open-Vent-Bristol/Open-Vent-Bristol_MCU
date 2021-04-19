// Copyright (c) 2021 <OpenVent-Bristol, Donald Robson>

#include "gpio/gpio.h"
#include "board/board.h"
#include <stdint.h>

void gpio_set_mask(gpio_register_t port, register_size_t pin_mask)
{
  LL_GPIO_SetOutputPin(port, pin_mask);
}

void gpio_clear_mask(gpio_register_t port, register_size_t pin_mask)
{
  LL_GPIO_ResetOutputPin(port, pin_mask);
}

void gpio_write_mask(gpio_register_t port, register_size_t pin_mask)
{
  LL_GPIO_WriteOutputPort(port, pin_mask);
}

register_size_t gpio_read_mask(gpio_register_t port, register_size_t pin_mask)
{
  return LL_GPIO_IsInputPinSet(port, pin_mask);
}

void gpio_set_pin(gpio_register_t port, uint8_t pin_number)
{
  LL_GPIO_SetOutputPin(port, (1u << pin_number));
}

void gpio_clear_pin(gpio_register_t port, uint8_t pin_number)
{
  LL_GPIO_ResetOutputPin(port, (1u << pin_number));
}

uint8_t gpio_read_pin(gpio_register_t port, uint8_t pin_number)
{
  uint8_t return_val = 0u;

  if (LL_GPIO_IsInputPinSet(port, (1u << pin_number)))
  {
    return_val = 1u;
  }

  return return_val;
}
