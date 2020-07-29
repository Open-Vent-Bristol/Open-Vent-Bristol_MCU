#include "gpio/gpio.h"
#include "board/board.h"
#include <stdint.h>

void gpio_init(void)
{
  // Enable clocks
  LL_AHB2_GRP1_EnableClock(
    LL_AHB2_GRP1_PERIPH_GPIOA | LL_AHB2_GRP1_PERIPH_GPIOB | LL_AHB2_GRP1_PERIPH_GPIOC);

  // Clock for EXTI
  LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_SYSCFG);

  // ADC pins
  LL_GPIO_InitTypeDef adc_init_struct = ADC_PIN_CFG;
  adc_init_struct.Pin = ADC_A_MASK;
  LL_GPIO_Init(ADC_A_PORT, &adc_init_struct);
  LL_GPIO_EnablePinAnalogControl(ADC_A_PORT, ADC_A_MASK);

  adc_init_struct.Pin = ADC_B_MASK;
  LL_GPIO_Init(ADC_B_PORT, &adc_init_struct);
  LL_GPIO_EnablePinAnalogControl(ADC_B_PORT, ADC_B_MASK);

  // ALERT, SWITCH, PGOOD, STAT1, STAT2: input, pullup
  LL_GPIO_InitTypeDef in_init_struct = IN_PU_PIN_CFG;
  in_init_struct.Pin = ALERT_MASK;
  LL_GPIO_Init(ALERT_PORT, &in_init_struct);

  in_init_struct.Pin = SWITCH_MASK;
  LL_GPIO_Init(SWITCH_PORT, &in_init_struct);

  in_init_struct.Pin = CHARGE_PGOOD_MASK;
  LL_GPIO_Init(CHARGE_PGOOD_PORT, &in_init_struct);

  in_init_struct.Pin = CHARGE_ISNS_MASK;
  LL_GPIO_Init(CHARGE_ISNS_PORT, &in_init_struct);

  in_init_struct.Pin = CHARGE_STAT_MASK;
  LL_GPIO_Init(CHARGE_STAT_PORT, &in_init_struct);

  // GPIO falling interrupts for ALERT, SWITCH
  LL_SYSCFG_SetEXTISource(ALERT_INT_PORT, ALERT_MOTOR_A_N_INT);
  LL_SYSCFG_SetEXTISource(ALERT_INT_PORT, ALERT_MOTOR_B_N_INT);
  LL_SYSCFG_SetEXTISource(SWITCH_INT_PORT, SWITCH_1_INT);
  LL_SYSCFG_SetEXTISource(SWITCH_INT_PORT, SWITCH_2_INT);
  LL_SYSCFG_SetEXTISource(SWITCH_INT_PORT, SWITCH_3_INT);
  LL_SYSCFG_SetEXTISource(SWITCH_INT_PORT, SWITCH_4_INT);
  LL_EXTI_EnableIT_0_31(ALERT_INT_PIN_MASK | SWITCH_INT_PIN_MASK);
  LL_EXTI_EnableFallingTrig_0_31(ALERT_INT_PIN_MASK | SWITCH_INT_PIN_MASK);

  // IRQ for ALERT_MOTOR_A_N
  NVIC_SetPriority(EXTI4_IRQn, 1u);
  NVIC_EnableIRQ(EXTI4_IRQn);

  // IRQ for ALERT_MOTOR_B_N, SWITCH_1, SWITCH_2
  NVIC_SetPriority(EXTI9_5_IRQn, 1u);
  NVIC_EnableIRQ(EXTI9_5_IRQn);

  // IRQ for SWITCH_3, SWITCH_4
  NVIC_SetPriority(EXTI15_10_IRQn, 1u);
  NVIC_EnableIRQ(EXTI15_10_IRQn);

  // MOTOR_IN_A/B, LCD, CE_CONTROL, ALERT_ENABLE, LED: output
  LL_GPIO_InitTypeDef out_init_struct = OUT_PIN_CFG;

  out_init_struct.Pin = MOTOR_IN_MASK;
  LL_GPIO_Init(MOTOR_IN_PORT, &out_init_struct);

  out_init_struct.Pin = LCD_MASK;
  LL_GPIO_Init(LCD_PORT, &out_init_struct);

  out_init_struct.Pin = CHARGE_CONTROL_MASK;
  LL_GPIO_Init(CHARGE_CONTROL_PORT, &out_init_struct);

  out_init_struct.Pin = ALERT_ENABLE_N_MASK;
  LL_GPIO_Init(ALERT_ENABLE_N_PORT, &out_init_struct);

  out_init_struct.Pin = LED_MASK;
  LL_GPIO_Init(LED_PORT, &out_init_struct);

  // UART
  LL_GPIO_InitTypeDef uart_tx_init_struct = UART_TX_PIN_CFG;
  LL_GPIO_Init(UART_PORT, &uart_tx_init_struct);
  LL_GPIO_InitTypeDef uart_rx_init_struct = UART_RX_PIN_CFG;
  LL_GPIO_Init(UART_PORT, &uart_rx_init_struct);

  // USB - TODO

  // MOTOR_PWM
  LL_GPIO_InitTypeDef motor_pwm_init_struct = MOTOR_PWM_PIN_CFG;
  LL_GPIO_Init(MOTOR_PWM_PORT, &motor_pwm_init_struct);
}

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

// ISR for ALERT_MOTOR_A_N
void EXTI4_IRQHandler(void)
{
  if (LL_EXTI_IsActiveFlag_0_31(1u << ALERT_MOTOR_A_N_PIN))
  {
    LL_EXTI_ClearFlag_0_31(1u << ALERT_MOTOR_A_N_PIN);

    // TODO - callback
  }
}

// ISR for ALERT_MOTOR_B_N, SWITCH_1, SWITCH_2
void EXTI9_5_IRQHandler(void)
{
  if (LL_EXTI_IsActiveFlag_0_31(1u << ALERT_MOTOR_B_N_PIN))
  {
    LL_EXTI_ClearFlag_0_31(1u << ALERT_MOTOR_B_N_PIN);

    // TODO - callback
  }
  else if (LL_EXTI_IsActiveFlag_0_31(1u << SWITCH_1_PIN))
  {
    LL_EXTI_ClearFlag_0_31(1u << SWITCH_1_PIN);

    // TODO - callback
  }
  else if (LL_EXTI_IsActiveFlag_0_31(1u << SWITCH_2_PIN))
  {
    LL_EXTI_ClearFlag_0_31(1u << SWITCH_2_PIN);

    // TODO - callback
  }
}

// ISR for SWITCH_3, SWITCH_4
void EXTI15_10_IRQHandler(void)
{
  if (LL_EXTI_IsActiveFlag_0_31(1u << SWITCH_3_PIN))
  {
    LL_EXTI_ClearFlag_0_31(1u << SWITCH_3_PIN);

    // TODO - callback
  }
  else if (LL_EXTI_IsActiveFlag_0_31(1u << SWITCH_4_PIN))
  {
    LL_EXTI_ClearFlag_0_31(1u << SWITCH_4_PIN);

    // TODO - callback
  }
}
