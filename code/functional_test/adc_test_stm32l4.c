#include "board/board.h"
#include "clock/clock.h"
#include "gpio/gpio.h"
#include "uart/uart.h"
#include "adc/adc.h"
#include <stdio.h>
#include <string.h>

// Note that BUFFER_LENGTH and SAMPLING_INTERVAL_MS must be powers of two!
#define BUFFER_LENGTH             (1024u)
#define BUFFER_LENGTH_PRE_TRIGGER (64u)
#define SAMPLE_INTERVAL_MS        (2u)

static volatile uint16_t g_timer_ms = 0u;
static volatile size_t g_ramp_start_index = 0u;
static volatile size_t g_ramp_end_index = 0u;
static volatile size_t g_index = 0u;
static volatile bool g_ramping = false;
static volatile bool g_do_sample = false;

static int16_t g_readings[BUFFER_LENGTH] = {0};

// Scheduler ISR interrupts at 1kHz
SCHEDULER_ISR()
{
  g_timer_ms++;

  if (g_timer_ms & (SAMPLE_INTERVAL_MS - 1u))
  {
    // When ramping, the condition below makes the end index act as a block
    // to incrementing the write index
    if (!g_ramping || (g_index != g_ramp_end_index))
    {
      g_index++;
      g_index &= (BUFFER_LENGTH - 1u);

      // Ensure the loop does a sample and writes it into the new index
      g_do_sample = true;
    }
  }
}

void start_ramp(void)
{
  // Start ramping the index will increment only up to the end index
  g_ramp_start_index = g_index;
  g_ramp_end_index = (g_ramp_start_index - BUFFER_LENGTH_PRE_TRIGGER - 1u) & (BUFFER_LENGTH - 1u);
  g_ramping = true;
}

void EXTI4_IRQHandler(void)
{
  LL_EXTI_ClearFlag_0_31(1u << ALERT_MOTOR_A_N_PIN);
  LL_EXTI_DisableIT_0_31(ALERT_INT_PIN_MASK);
  start_ramp();
}

void EXTI9_5_IRQHandler(void)
{
  LL_EXTI_ClearFlag_0_31(1u << ALERT_MOTOR_B_N_PIN);
  LL_EXTI_DisableIT_0_31(ALERT_INT_PIN_MASK);
  start_ramp();
}

int main(void)
{
  char output[13];

  clock_init();
  gpio_init();

  // Need interrupts on either edge, so can use either rising on ALERT_MOTOR_A_N_PIN or
  // falling on ALERT_MOTOR_B_N_PIN.  The former already setup for PULL_UP, so fix this first
  LL_GPIO_SetPinPull(ALERT_PORT, (1u << ALERT_MOTOR_A_N_PIN), LL_GPIO_PULL_DOWN);
  LL_EXTI_DisableFallingTrig_0_31(1u << ALERT_MOTOR_A_N_PIN);
  LL_EXTI_EnableRisingTrig_0_31(1u << ALERT_MOTOR_A_N_PIN);
  NVIC_SetPriority(EXTI4_IRQn, 1u);
  NVIC_SetPriority(EXTI9_5_IRQn, 1u);

  uart_init();
  adc_init();

  // Enable GPIO interrupts
  NVIC_EnableIRQ(EXTI4_IRQn);
  NVIC_EnableIRQ(EXTI9_5_IRQn);

  while (1u)
  {
    if (g_do_sample)
    {
      // Cache the adc reading
      g_readings[g_index] = adc_read_blocking(ADC_FLOW);
      g_do_sample = false;
    }

    // Once the buffer is full, print it out in CSV through the UART port
    if (g_ramping && (g_index == g_ramp_end_index))
    {
      LL_EXTI_ClearFlag_0_31(1u << ALERT_MOTOR_A_N_PIN);
      LL_EXTI_ClearFlag_0_31(1u << ALERT_MOTOR_B_N_PIN);

      // After the recording, send all data points out through the serial port,
      // with the pre-trigger buffer first
      size_t i = g_ramp_end_index;
      for (size_t print_count = 0u; print_count < BUFFER_LENGTH; print_count++)
      {
        i++;
        i &= (BUFFER_LENGTH - 1u);
        snprintf(&output[0], sizeof(output),
          "%4u, %4u\r\n", (print_count * SAMPLE_INTERVAL_MS), g_readings[i]);

        while (!uart_capacity(sizeof(output)));
        uart_write(output, sizeof(output));
      }

      // Start sampling again, ready for the next trigger
      g_ramping = false;

      // Re-enable GPIO interrupts
      LL_EXTI_EnableIT_0_31(ALERT_INT_PIN_MASK);
    }
  }
}
