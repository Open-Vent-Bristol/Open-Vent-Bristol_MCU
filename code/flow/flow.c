#include "flow/flow.h"
#include "flow/flow_parameters.def"

#ifdef UTEST_MODE
// TODO - mock this properly and get rid of this pp block
uint16_t pgm_read_word_near(const void* position)
{
  return *(uint16_t*)position;
}
#else
#include <avr/pgmspace.h>
#endif

int16_t Get_Flow_Rate(int16_t pressure_delta,
                      int16_t back_pressure,
                      Flow_Temperature_t ambient_temperature,
                      Flow_CorrectionFactor_t correction_factor)
{
  // Result is the product of four parameters
  uint32_t flow_rate;
  uint32_t temp_param;
  uint32_t back_pressure_param;
  uint8_t correction_param;
  int8_t result_sign = 1;

  if (pressure_delta == 0)
  {
    return 0;
  }

  // The temperature argument is already shifted 7 bits, so we need to
  // recover those before adding the constant B1
  temp_param = C2_SHIFT16 * ambient_temperature;
  temp_param >>= 7u;
  temp_param += B1_SHIFT16;

  back_pressure_param = C3_SHIFT22 * back_pressure;
  back_pressure_param += B2_SHIFT22;

  // Extract table_param from the table in flash
  uint16_t index;
  if (pressure_delta < 0)
  {
    index = 0 - pressure_delta;

    // Keep track of the sign of the result
    result_sign = 0 - result_sign;
  }
  else
  {
    index = pressure_delta;
  }
  flow_rate = pgm_read_word_near(flow_lookup_table + index);

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
