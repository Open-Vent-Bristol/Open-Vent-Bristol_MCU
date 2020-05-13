#include "flow/flow.h"
#include "flow/flow_parameters.def"

#include <stdio.h>

#ifdef UTEST_MODE
// TODO - mock this properly and get rid of this pp block
uint16_t pgm_read_word_near(const void* position)
{
  return *(uint16_t*)position;
}
#else
#include <avr/pgmspace.h>
#endif

int16_t Calculate_Back_Pressure(uint16_t back_pressure_raw)
{
  int32_t back_pressure;
  int8_t result_sign = 1;

  back_pressure = (back_pressure_raw * ADC_REF_MV) - (MV_ZERO_PA_SHIFT10);
  if (back_pressure < 0)
  {
    back_pressure = 0 - back_pressure;
    result_sign = -1;
  }

  back_pressure >>= 10;
  back_pressure *= PA_PER_MV_SHIFT10;
  back_pressure >>= 10;

  if (result_sign == -1)
  {
    back_pressure = 0 - back_pressure;
  }

  return back_pressure;
}

int16_t Get_Flow_Rate(int16_t pressure_delta_raw,
                      int16_t back_pressure_raw,
                      Flow_Temperature_t ambient_temperature,
                      Flow_CorrectionFactor_t correction_factor)
{
  // Result is the product of four parameters
  uint64_t flow_rate;
  uint32_t temp_param;
  uint32_t back_pressure_param;
  uint8_t correction_param;

  int16_t flow_rate_from_table;
  int16_t back_pressure;
  int8_t result_sign = 1;

  // Cap raw inputs at ADC_RESOLUTION - 1 to prevent array overflow
  if (pressure_delta_raw >= ADC_RESOLUTION)
  {
    pressure_delta_raw = ADC_RESOLUTION - 1;
  }

  if (back_pressure_raw >= ADC_RESOLUTION)
  {
    back_pressure_raw = ADC_RESOLUTION - 1;
  }

  // Convert back_pressure_raw into a pressure in Pa
  back_pressure = Calculate_Back_Pressure(back_pressure_raw);

  // Use the back pressure to calculate the parameter
  back_pressure_param = C3_SHIFT22 * back_pressure;
  back_pressure_param += B2_SHIFT22;

  // The temperature argument is already shifted 7 bits, so we need to
  // recover those before adding the constant B1
  temp_param = C2_SHIFT16 * ambient_temperature;
  temp_param >>= 7u;
  temp_param += B1_SHIFT16;

  // Extract flow_rate from the table in flash
  flow_rate_from_table = pgm_read_word_near(
                          flow_lookup_table + pressure_delta_raw);
  if (flow_rate_from_table < 0)
  {
    flow_rate = 0 - flow_rate_from_table;

    // Keep track of the sign of the result
    result_sign = 0 - result_sign;
  }
  else
  {
    flow_rate = flow_rate_from_table;
  }

  // Multiply by the back_pressure_param then recover bits
  flow_rate *= back_pressure_param;
  flow_rate >>= 22u;

  // Multiply by the temperature_param then recover bits
  flow_rate *= temp_param;
  flow_rate >>= 16u;

  // Apply the correction_factor.  First, offset it by 128 to
  // account for the signedness, then multiply, then recover bits
  correction_param = correction_factor + 128u;
  flow_rate *= correction_param;
  flow_rate >>= 7u;

  // Turn it back into a signed number
  return (int16_t)flow_rate * result_sign;
}
