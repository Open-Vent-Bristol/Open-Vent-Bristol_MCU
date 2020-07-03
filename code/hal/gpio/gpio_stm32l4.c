#include "gpio/gpio.h"
#include "board/board.h"
#include <stdint.h>

void gpio_init(void)
{
  // Enable clocks
  LL_AHB2_GRP1_EnableClock(
    LL_AHB2_GRP1_PERIPH_GPIOA | LL_AHB2_GRP1_PERIPH_GPIOB | LL_AHB2_GRP1_PERIPH_GPIOC);

  // ADC pins
  LL_GPIO_InitTypeDef adc_init_struct = ADC_PIN_CFG;
  adc_init_struct.Pin = ADC_MASK;
  LL_GPIO_Init(ADC_PORT, &adc_init_struct);

  // ALERT, SWITCH, PGOOD, STAT1, STAT2: input, pullup, interrupt
  LL_GPIO_InitTypeDef in_init_struct = IN_PU_PIN_CFG;
  in_init_struct.Pin = ALERT_MASK;
  LL_GPIO_Init(ALERT_PORT, &in_init_struct);

  in_init_struct.Pin = SWITCH_MASK;
  LL_GPIO_Init(SWITCH_PORT, &in_init_struct);

  in_init_struct.Pin = PGOOD_PIN;
  LL_GPIO_Init(PGOOD_PORT, &in_init_struct);

  in_init_struct.Pin = STAT1_PIN;
  LL_GPIO_Init(STAT1_PORT, &in_init_struct);

  in_init_struct.Pin = STAT2_PIN;
  LL_GPIO_Init(STAT2_PORT, &in_init_struct);
  // TODO - EXTI

  // MOTOR_IN_A/B, LCD, CE_CONTROL, ALERT_ENABLE, LED: output
  LL_GPIO_InitTypeDef out_init_struct = OUT_PIN_CFG;

  out_init_struct.Pin = MOTOR_IN_MASK;
  LL_GPIO_Init(MOTOR_IN_PORT, &out_init_struct);

  out_init_struct.Pin = LCD_MASK;
  LL_GPIO_Init(LCD_PORT, &out_init_struct);

  out_init_struct.Pin = CE_CONTROL_PIN;
  LL_GPIO_Init(CE_CONTROL_PORT, &out_init_struct);

  out_init_struct.Pin = ALERT_ENABLE_PIN;
  LL_GPIO_Init(ALERT_ENABLE_PORT, &out_init_struct);

  out_init_struct.Pin = LED_PIN;
  LL_GPIO_Init(LED_PORT, &out_init_struct);

  // UART
  LL_GPIO_InitTypeDef uart_tx_init_struct = UART_TX_PIN_CFG;
  LL_GPIO_Init(UART_PORT, &uart_tx_init_struct);
  LL_GPIO_InitTypeDef uart_rx_init_struct = UART_RX_PIN_CFG;
  LL_GPIO_Init(UART_PORT, &uart_rx_init_struct);

  // MOTOR_PWM
  LL_GPIO_InitTypeDef motor_pwm_init_struct = MOTOR_PWM_PIN_CFG;
  LL_GPIO_Init(MOTOR_PWM_PORT, &motor_pwm_init_struct);
}

void gpio_set_mask(MCU_register_t port, register_size_t pin_mask)
{
  *port |= pin_mask;
}

void gpio_clear_mask(MCU_register_t port, register_size_t pin_mask)
{
  *port &= ~pin_mask;
}

void gpio_write_mask(MCU_register_t port, register_size_t pin_mask)
{
  *port = pin_mask;
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
