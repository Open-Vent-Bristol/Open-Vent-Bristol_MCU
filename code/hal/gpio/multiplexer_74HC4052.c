#include "gpio/multiplexer.h"
#include "gpio/gpio.h"
#include "board/board.h"
#include <avr/interrupt.h>

multiplexer_select_t s_current_channel;

void multiplexer_select_channel(multiplexer_select_t channel)
{
  // Must disable global interrupts while doing read-modify-write
  cli();

  uint8_t pin_mask = gpio_read_mask(MUX_PORT, REGISTER_SIZE_MAX);
  pin_mask &= MUX_SELECT_MASK;
  pin_mask |= (channel & MUX_SELECT_MASK);
  gpio_write_mask(MUX_PORT, pin_mask);

  // Reenable global interrupts
  sei();

  s_current_channel = channel;
}

uint8_t multiplexer_digital_read_current_channel(void)
{
  uint8_t return_val = 0u;
  MCU_register_t port = s_current_channel & MUX_SELECT_BANK_MASK;
  uint8_t pin_mask = s_current_channel & MUX_SELECT_MASK;
  register_size_t pin_val = gpio_read_mask(port, pin_mask);

  if (pin_val != 0u)
  {
    return_val = 1u;
  }

  return return_val;
}

uint8_t multiplexer_digital_read_channel(multiplexer_select_t channel)
{
  uint8_t return_val = 0u;
  multiplexer_select_channel(channel);
  return_val = multiplexer_digital_read_current_channel();
  return return_val;
}
