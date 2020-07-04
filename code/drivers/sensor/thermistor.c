#include "sensor/thermistor.h"
#include "board/board.h"

resistance_t thermistor_get_resistance(ADC_resolution_t adc_reading)
{
  resistance_t resistance = -1;

  // Only perform calculation if analogue_voltage is valid.
  // Casting to unsigned so we don't also have to check for negative.
  if ((uint16_t)adc_reading < ((1u << ADC_RESOLUTION_BITS) - 1u))
  {
    resistance = THERM_FIXED_RESIST * adc_reading;
    resistance /= (((1u << ADC_RESOLUTION_BITS) - 1u) - adc_reading);
  }

  return resistance;
}
