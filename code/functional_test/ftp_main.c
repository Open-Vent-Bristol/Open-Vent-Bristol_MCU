#include "board/board.h"
#include "clock/clock.h"
#include "gpio/gpio.h"
// #include "spi/spi.h"
// #include "uart/uart.h"

// extern void uart_ftp_run(void);
// extern void spi_ftp_run(void);

volatile uint16_t g_timer_ms;

// Scheduler ISR interrupts at 1kHz
SCHEDULER_ISR()
{
  if (g_timer_ms != 0u)
  {
    g_timer_ms--;
  }
}

int main(void)
{
  g_timer_ms = 0u;

  clock_init();
  gpio_init();
  // spi_init();
  // uart_init();

  SCHEDULER_START();
  MOTOR_PWM(127u);
  MOTOR_PWM_START();

  sei();

  while (1u)
  {
    if (g_timer_ms == 0u)
    {
      // Make the spare GPIO high for the duration of processing - this enables the scheduler
      // frequency to be checked
      gpio_set_pin(&GPIO_SPARE_PORT, GPIO_SPARE_PIN);

      // uart_ftp_run();
      // spi_ftp_run();
      // Do some stuff!
      for (volatile uint16_t i = 0u; i < 500u; i++);

      // Clear the spare GPIO - for checking frequency
      gpio_clear_pin(&GPIO_SPARE_PORT, GPIO_SPARE_PIN);

      // Set timer so there's a delay before processing again
      g_timer_ms = 100u;
    }
  }
}
