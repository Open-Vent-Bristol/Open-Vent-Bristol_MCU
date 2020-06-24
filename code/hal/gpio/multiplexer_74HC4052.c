#include "gpio/multiplexer.h"
#include "gpio/gpio.h"
#include "board/board.h"
#include <avr/interrupt.h>

multiplexer_select_t s_current_channel;

void multiplexer_select_channel(multiplexer_select_t channel)
{
  // Must disable global interrupts while doing read-modify-write
  cli();

  uint8_t pin_mask = gpio_read_mask(&MUX_PORT, REGISTER_SIZE_MAX);
  pin_mask &= MUX_SELECT_MASK;
  pin_mask |= (channel & MUX_SELECT_MASK);
  gpio_set_mask(&MUX_PORT, pin_mask);

  // Reenable global interrupts
  sei();

  s_current_channel = channel;
}

uint8_t multiplexer_digital_read_current_channel(void)
{
  uint8_t return_val = 0u;

  if ((s_current_channel & MUX_SELECT_BANK_MASK) == 0u)
  {
    return_val = gpio_read_pin(&MUX_BANK_0_PORT, MUX_BANK_0_PIN);
  }
  else
  {
    return_val = gpio_read_pin(&MUX_BANK_1_PORT, MUX_BANK_1_PIN);
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
