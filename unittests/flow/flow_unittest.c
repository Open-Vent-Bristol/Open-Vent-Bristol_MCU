#include <math.h>

#include "unity.h"
#include "unity_fixture.h"
#include "flow/flow.h"
#include "flow/flow_parameters.def"

#define TOLERANCE (1)

#define C1 (19.06998055)
#define E1 (0.517553019)
#define C2 (0.00151537)
#define C3 (-0.00000422946)
#define B1 (0.96956652)
#define B2 (0.999945636)

int16_t Calculate_Flow_Using_Floating_Point(
          int16_t pressure_delta,
          int16_t back_pressure,
          Flow_Temperature_t ambient_temperature,
          Flow_CorrectionFactor_t correction_factor)
{
  double flow = pow(pressure_delta, 2);
  flow = pow(flow, E1 / 2.0);
  flow *= C1;

  double bpp = C3 * back_pressure;
  bpp += B2;

  double tp = C2 * ambient_temperature / 128.0;
  tp += B1;

  double cf = correction_factor / 256;
  cf += 1.0;

  int16_t result = (int16_t)(flow * bpp * tp * cf);
  if (pressure_delta < 0)
  {
    result *= -1;
  }

  return result;
}

TEST_GROUP(flow_tests);

TEST_SETUP(flow_tests)
{

}

TEST_TEAR_DOWN(flow_tests)
{

}

TEST(flow_tests, output_within_tolerance_of_floating_point)
{
  for (uint16_t i = 0u; i < PRESSURE_LIMIT; i++)
  {
    int16_t fixed_point = Get_Flow_Rate(i, 100, 298, 0);
    int16_t floating_point =
      Calculate_Flow_Using_Floating_Point(i, 100, 298, 0);

    uint16_t difference;
    if (fixed_point > floating_point)
    {
      difference = fixed_point - floating_point;
    }
    else
    {
      difference = floating_point - fixed_point;
    }

    TEST_ASSERT_LESS_THAN(TOLERANCE + 1u, difference);
  }
}
