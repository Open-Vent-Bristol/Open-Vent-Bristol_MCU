#include "adc/adc.h"
#include "board/board.h"
#include <avr/interrupt.h>
#include <stddef.h>

static ADC_callback_t s_adc_callback = NULL;

void adc_init(void)
{
  // Use AREF pin
  ADMUX = (1u << REFS0);

  // Enable ADC
  ADCSRA = (1u << ADEN) | ADC_PRESCALER;
}

bool adc_read_interrupt(ADC_channel_t channel, ADC_callback_t callback)
{
  bool return_val = false;

  // Check for an existing measurement underway by inspecting the reading bit
  if ((ADCSRA & (1u << ADSC)) == 0u)
  {
    ADMUX |= channel;

    // Assign the callback
    s_adc_callback = callback;

    // Start reading and enable interrupt
    ADCSRA |= (1u << ADSC) | (1u << ADIE);
    return_val = true;
  }

  return return_val;
}

ADC_resolution_t adc_read_blocking(ADC_channel_t channel)
{
  ADC_resolution_t reading = -1;

  // Check for an existing measurement underway by inspecting the reading bit
  if ((ADCSRA & (1u << ADSC)) == 0u)
  {
    ADMUX |= channel;

    // Start reading
    ADCSRA |= (1u << ADSC);

    // Block until the reading bit is cleared by hardware
    while ((ADCSRA & (1u << ADSC)) != 0u) {};

    // Get the reading from the ADC registers
    reading = ADC;

    // Clear the channel
    ADMUX &= ~((1u << MUX3) | (1u << MUX2) | (1u << MUX1) | (1u << MUX0));
  }

  return reading;
}

// ADC complete interrupt will take the reading and push it out via the callback
ISR(ADC_vect)
{
  if (s_adc_callback != NULL)
  {
    // Get the reading from the ADC registers
    ADC_resolution_t reading = ADC;
    s_adc_callback(reading);
  }

  // Disable interrupt and clear the channel
  ADCSRA &= ~(1u << ADIE);
  ADMUX &= ~((1u << MUX3) | (1u << MUX2) | (1u << MUX1) | (1u << MUX0));
}
