#include <stdint.h>
#include <avr/io.h>

int main(void)
{
  DDRD = (1u << 4u) | (1u << 5u);
  PORTD = 0u;

  // PWM output on D5
  TCCR0A |= (1u << WGM01) | (1u << WGM00) | (1u << COM0B1);
  TCCR0B |= (1u << CS01);
  OCR0B = 63u;

  while(1u)
  {
    for (uint16_t i = 0u; i <= 10000u; i++)
    {}

    PORTD ^= (1u << 4u);
  }
}
