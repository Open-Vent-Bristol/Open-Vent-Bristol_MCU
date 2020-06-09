#include "gpio/gpio.h"
#include "board/board.h"
#include <stdint.h>
#include <avr/interrupt.h>

void gpio_init(void)
{
  // ALERTS: input, tri-state, interrupt
  ALERTS_MODE &= ~(1u << ALERTS_PIN);
  ALERTS_PULLUP &= ~(1u << ALERTS_PIN);
  ALERTS_INT_REG |= (1u << ALERTS_INT_CHAN);

  // MUX_A, MUX_B: output
  MUX_A_MODE |= (1u << MUX_A_PIN);
  MUX_B_MODE |= (1u << MUX_B_PIN);

  // MOTOR_IN_A: output
  MOTOR_IN_A_MODE |= (1u << MOTOR_IN_A_PIN);

  // ADC_SPARE: output
  ADC_SPARE_MODE |= (1u << ADC_SPARE_PIN);

  // MOTOR_IN_B, ALERT_ENABLE_n, GPIO_SPARE, SR_MR_n: output
  MOTOR_IN_B_MODE |= (1u << MOTOR_IN_B_PIN);
  ALERT_ENABLE_n_MODE |= (1u << ALERT_ENABLE_n_PIN);
  ALERT_ENABLE_n_PORT |= (1u << ALERT_ENABLE_n_PIN);
  GPIO_SPARE_MODE |= (1u << GPIO_SPARE_PIN);
  SR_MR_n_MODE |= (1u << SR_MR_n_PIN);
  SR_MR_n_PORT |= (1u << SR_MR_n_PIN);

  // SWITCHES: input, tri-state, interrupt
  SWITCHES_MODE &= ~(1u << SWITCHES_PIN);
  SWITCHES_PULLUP &= ~(1u << SWITCHES_PIN);
  SWITCHES_INT_REG |= (1u << SWITCHES_INT_CHAN);

  // LATCH: output
  LATCH_MODE |= (1u << LATCH_PIN);

  // Disable digital input to ADC pins
  ADC_DIGITAL_DISABLE |= (1u << ADC_FLOW) |
                         (1u << ADC_PRESSURE) |
                         (1u << ADC_VBATT) |
                         (1u << ADC_MOTOR_CURRENT) |
                         (1u << ADC_TEMP);

  // Enable pin change interrupts - TODO: move to multiplexer driver
  PIN_CHANGE_INT_CFG();
}

void gpio_set_mask(MCU_register_t port, register_size_t pin_mask)
{
  *port |= pin_mask;
}

void gpio_clear_mask(MCU_register_t port, register_size_t pin_mask)
{
  *port &= ~pin_mask;
}

register_size_t gpio_read_mask(MCU_register_t port, register_size_t pin_mask)
{
  return (*port & pin_mask);
}

void gpio_set_pin(MCU_register_t port, uint8_t pin_number)
{
  *port |= (1u << pin_number);
}

void gpio_clear_pin(MCU_register_t port, uint8_t pin_number)
{
  *port &= ~(1u << pin_number);
}

uint8_t gpio_read_pin(MCU_register_t port, uint8_t pin_number)
{
  uint8_t return_val = 0u;

  if (*port & (1u << pin_number))
  {
    return_val = 1u;
  }

  return return_val;
}

// TODO: move to multiplexer driver
ISR(ALERTS_INT_ISR)
{

}

// TODO: move to multiplexer driver
ISR(SWITCHES_INT_ISR)
{

}
