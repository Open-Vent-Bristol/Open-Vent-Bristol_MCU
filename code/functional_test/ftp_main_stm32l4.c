#include "board/board.h"
#include "clock/clock.h"
#include "gpio/gpio.h"
#include "pinmux/pinmux.h"
#include "spi/spi.h"
#include "uart/uart.h"
#include "adc/adc.h"
#include "ftp_types.h"

void process_command(ftp_command_t* command);
extern void uart_ftp_get_command(ftp_command_t* command);
extern void spi_ftp_run(void);

static const char string_error_not_implemented[] = " - not implemented";
static const char string_high[] = "HIGH";
static const char string_low[] = "LOW";
static const char string_newline[] = "\r\n>";
static const char string_welcome[] = "\r\nOpenVent - Functional Test Program";

static volatile uint16_t g_timer_ms;

int main(void)
{
  g_timer_ms = 0u;

  clock_init();
  pinmux_init();
  uart_init();
  adc_init();
  spi_init();

  SYSTICK_START();

  uart_write(string_welcome, sizeof(string_welcome));
  uart_write(string_newline, sizeof(string_newline));

  ftp_command_t ftp_command = {0u};

  while (1u)
  {
    if (g_timer_ms == 0u)
    {
      // Make the spare GPIO high for the duration of processing - this enables the scheduler
      // frequency to be checked on a scope
      // gpio_set_pin(GPIO_SPARE_PORT, GPIO_SPARE_PIN);

      uart_ftp_get_command(&ftp_command);
      if (ftp_command.instruction != NONE)
      {
        process_command(&ftp_command);
      }

      // Clear the spare GPIO - for checking frequency
      // gpio_clear_pin(GPIO_SPARE_PORT, GPIO_SPARE_PIN);

      // Set timer so there's a delay before processing again
      g_timer_ms = 20u;
    }
  }
}

void process_command(ftp_command_t* command)
{
  ADC_resolution_t adc_reading = -1;
  uint8_t digital_reading = 0xFF;

  switch(command->instruction)
  {
    case READ_FPGA_READY:
      digital_reading = gpio_read_pin(FPGA_READY_PORT, FPGA_READY_PIN);
      break;

    case READ_MOTOR_FAULT_A:
      digital_reading = gpio_read_pin(MOTOR_FAULT_A_PORT, MOTOR_FAULT_A_PIN);
      break;

    case READ_MOTOR_FAULT_B:
      digital_reading = gpio_read_pin(MOTOR_FAULT_B_PORT, MOTOR_FAULT_B_PIN);
      break;

    case READ_PGOOD:
      digital_reading = gpio_read_pin(PGOOD_PORT, PGOOD_PIN);
      break;

    case READ_PRESSURE_ALERT:
      digital_reading = gpio_read_pin(PRESSURE_ALERT_PORT, PRESSURE_ALERT_PIN);
      break;

    case READ_PB_SEL:
      digital_reading = gpio_read_pin(PB_SEL_PORT, PB_SEL_PIN);
      break;

    case READ_PB_MUTE:
      digital_reading = gpio_read_pin(PB_MUTE_PORT, PB_MUTE_PIN);
      break;

    case READ_PB_MINUS:
      digital_reading = gpio_read_pin(PB_MINUS_PORT, PB_MINUS_PIN);
      break;

    case READ_PB_PLUS:
      digital_reading = gpio_read_pin(PB_PLUS_PORT, PB_PLUS_PIN);
      break;

    case READ_SW_BACKUP:
      digital_reading = gpio_read_pin(SW_BACKUP_PORT, SW_BACKUP_PIN);
      break;

    case READ_SW_END:
      digital_reading = gpio_read_pin(SW_END_PORT, SW_END_PIN);
      break;

    case READ_SW_HOME:
      digital_reading = gpio_read_pin(SW_HOME_PORT, SW_HOME_PIN);
      break;

      break;

    case READ_ANALOG_VBATT:
      adc_reading = adc_read_blocking(ADC_VBATT_SCALED);
      break;

    case READ_ANALOG_MOTOR_CURR:
      adc_reading = adc_read_blocking(ADC_MOTOR_CURR);
      break;

    case READ_ANALOG_ALARM_CURR:
      adc_reading = adc_read_blocking(ADC_SOUNDER_CURR);
      break;

    case READ_ANALOG_SPARE:
      adc_reading = adc_read_blocking(ADC_SPARE);
      break;

    case MOTOR_STOP:
      MOTOR_PWM_STOP();
      break;

    case MOTOR_RUN_IN:
      gpio_set_pin(MOTOR_IN_A_PORT, MOTOR_IN_A_PIN);
      gpio_clear_pin(MOTOR_IN_B_PORT, MOTOR_IN_B_PIN);
      MOTOR_PWM(command->arg);
      MOTOR_PWM_START();
      break;

    case MOTOR_RUN_OUT:
      gpio_clear_pin(MOTOR_IN_A_PORT, MOTOR_IN_A_PIN);
      gpio_set_pin(MOTOR_IN_B_PORT, MOTOR_IN_B_PIN);
      MOTOR_PWM(command->arg);
      MOTOR_PWM_START();
      break;

    case BUZZER_CONTROL:
      if (command->arg)
      {
        BUZZ_PWM_START();
      }
      else
      {
        BUZZ_PWM_STOP();
      }
      break;

    case BATT_LED_CONTROL:
      if (command->arg)
      {
        gpio_set_pin(BATTERY_LED_PORT, BATTERY_LED_PIN);
      }
      else
      {
        gpio_clear_pin(BATTERY_LED_PORT, BATTERY_LED_PIN);
      }
      break;

    case CE_CONTROL:
      if (command->arg)
      {
        gpio_set_pin(CE_CONTROL_PORT, CE_CONTROL_PIN);
      }
      else
      {
        gpio_clear_pin(CE_CONTROL_PORT, CE_CONTROL_PIN);
      }
      break;

    case LDO_EN_CONTROL:
      if (command->arg)
      {
        gpio_set_pin(LDO_EN_PORT, LDO_EN_PIN);
      }
      else
      {
        gpio_clear_pin(LDO_EN_PORT, LDO_EN_PIN);
      }
      break;

    case PWR_UP_CONTROL:
      if (command->arg)
      {
        gpio_set_pin(PWR_UP_PORT, PWR_UP_PIN);
      }
      else
      {
        gpio_clear_pin(PWR_UP_PORT, PWR_UP_PIN);
      }
      break;

    case PRINT_SPI:
      spi_ftp_run();
      uart_write("- check scope", 13u);
      break;

    case NONE:
      uart_write(string_error_not_implemented, sizeof(string_error_not_implemented));
      break;
  }

  if (digital_reading == 0u)
  {
    uart_write(string_newline, sizeof(string_newline));
    uart_write(string_low, sizeof(string_low));
  }
  else if (digital_reading == 1u)
  {
    uart_write(string_newline, sizeof(string_newline));
    uart_write(string_high, sizeof(string_high));
  }
  else if (adc_reading != -1)
  {
    // 5 spaces will work for anything up to a 16-bit ADC
    char mirror_str[5] = {'\0'};
    uint8_t i = 0u;
    while (adc_reading)
    {
      ADC_resolution_t tenth = adc_reading / 10u;
      mirror_str[i++] = adc_reading - (tenth * 10u) + '0';
      adc_reading = tenth;
    }

    uart_write(string_newline, sizeof(string_newline));

    // Characters are in reverse order!
    if (i == 0u)
    {
      char zero = '0';
      uart_write(&zero, 1u);
    }
    else
    {
      for (; i > 0u; i--)
      {
        uart_write(&mirror_str[i - 1u], 1u);
      }
    }
  }

  uart_write(string_newline, sizeof(string_newline));
}

// SysTick ISR interrupts at 1kHz
SYSTICK_ISR()
{
  if (g_timer_ms != 0u)
  {
    g_timer_ms--;
  }
}

// ISR for PB_SEL
void EXTI0_IRQHandler(void)
{
  LL_EXTI_ClearFlag_0_31(1u << PB_SEL_PIN);
}

// ISR for FPGA_READY
void EXTI1_IRQHandler(void)
{
  LL_EXTI_ClearFlag_0_31(1u << FPGA_READY_PIN);
}

// ISR for SW_BACKUP, SW_END, SW_HOME, nMOTOR_FAULT_A, nMOTOR_FAULT_B
void EXTI9_5_IRQHandler(void)
{
  if (LL_EXTI_IsActiveFlag_0_31(1u << SW_BACKUP_PIN))
  {
    // SW_BACKUP and SW_END
    MOTOR_PWM_STOP();
    LL_EXTI_ClearFlag_0_31(1u << SW_BACKUP_PIN);
  }
  else if (LL_EXTI_IsActiveFlag_0_31(1u << SW_HOME_PIN))
  {
    MOTOR_PWM_STOP();
    LL_EXTI_ClearFlag_0_31(1u << SW_HOME_PIN);
  }
  else if (LL_EXTI_IsActiveFlag_0_31(1u << MOTOR_FAULT_A_PIN))
  {
    LL_EXTI_ClearFlag_0_31(1u << MOTOR_FAULT_A_PIN);
  }
  else if (LL_EXTI_IsActiveFlag_0_31(1u << MOTOR_FAULT_B_PIN))
  {
    LL_EXTI_ClearFlag_0_31(1u << MOTOR_FAULT_B_PIN);
  }
}

// ISR for nPRESSURE_PATIENT_ALERT
void EXTI15_10_IRQHandler(void)
{
  LL_EXTI_ClearFlag_0_31(1u << PRESSURE_ALERT_PIN);
}
