#include "adc/adc.h"
#include "board/board.h"

static ADC_callback_t s_adc_callback = NULL;

void adc_init(void)
{
  // Enable ADC
  ADCSRA = (1u << ADEN) | ADC_PRESCALER;
}

bool adc_read_interrupt(ADC_channel_t channel, ADC_callback_t callback)
{
  bool return_val = false;

  // Check for an existing measurement underway by inspecting the reading bit and the
  // multiplexer channel
  if (((ADCSRA & (1u << ADSC)) == 0u) |
      (ADMUX & ((1u << MUX3) | (1u << MUX2) | (1u << MUX1) | (1u << MUX0))))
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

  // Check for an existing measurement underway by inspecting the reading bit and the
  // multiplexer channel
  if (((ADCSRA & (1u << ADSC)) == 0u) |
      (ADMUX & ((1u << MUX3) | (1u << MUX2) | (1u << MUX1) | (1u << MUX0))))
  {
    ADMUX |= channel;

    // Disable interrupt and start reading
    ADCSRA &= ~(1u << ADIE);
    ADCSRA |= (1u << ADSC);

    // Block until the reading bit is cleared by hardware
    while ((ADCSRA & (1u << ADSC)) != 0u) {};

    // Get the reading from the ADC registers (disable all interrupts during 16-bit read)
    sei();
    reading = ((ADCL << 8u) & 0xFF) | (ADCL & 0xFF);
    cli();

    // Clear the channel to release the ADC for a new reading
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
    ADC_resolution_t reading = ((ADCL << 8u) & 0xFF) | (ADCL & 0xFF);

    // Clear the channel to release the ADC for a new reading
    ADMUX &= ~((1u << MUX3) | (1u << MUX2) | (1u << MUX1) | (1u << MUX0));

    s_adc_callback(reading);
  }
}
