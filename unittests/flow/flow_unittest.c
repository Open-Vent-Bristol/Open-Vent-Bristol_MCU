#include <math.h>

#include "unity.h"
#include "unity_fixture.h"
#include "flow/flow.h"
#include "flow/flow_parameters.def"

#define TOLERANCE (4)

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

  double cf = correction_factor / 128.0;
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

TEST(flow_tests, output_within_tolerance_deltaPressure)
{
  const int16_t backP = 10;
  const Flow_Temperature_t temp = 298u << 7u;
  const Flow_CorrectionFactor_t cf = 0;

  for (int16_t i = (1 - PRESSURE_LIMIT); i < PRESSURE_LIMIT; i++)
  {
    int16_t fixed_point = Get_Flow_Rate(i, backP, temp, cf);
    int16_t floating_point =
      Calculate_Flow_Using_Floating_Point(i, backP, temp, cf);

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

TEST(flow_tests, output_within_tolerance_backPressure)
{
  const int16_t deltaP = 20;
  const Flow_Temperature_t temp = 310u << 7u;
  const Flow_CorrectionFactor_t cf = 0;

  for (int16_t i = (1 - PRESSURE_LIMIT); i < PRESSURE_LIMIT; i++)
  {
    int16_t fixed_point = Get_Flow_Rate(deltaP, i, temp, cf);
    int16_t floating_point =
      Calculate_Flow_Using_Floating_Point(deltaP, i, temp, cf);

    // TODO - negative back pressures don't work!
    // printf("\n%d: %d, %d", i, fixed_point, floating_point);
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

TEST(flow_tests, output_within_tolerance_ambientTemp)
{
  const int16_t deltaP = 20;
  const int16_t backP = 5;
  const Flow_CorrectionFactor_t cf = 0;
  const Flow_Temperature_t lowTemp = 273u << 7u;
  const Flow_Temperature_t highTemp = 323u << 7u;

  for (uint16_t i = lowTemp; i <= highTemp; i += (1u << 7u))
  {
    int16_t fixed_point = Get_Flow_Rate(deltaP, backP, i, cf);
    int16_t floating_point =
      Calculate_Flow_Using_Floating_Point(deltaP, backP, i, cf);

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

TEST(flow_tests, output_within_tolerance_correctionFactor)
{
  const int16_t deltaP = 20;
  const int16_t backP = 5;
  const Flow_Temperature_t temp = 303u << 7u;

  for (int16_t i = INT8_MIN; i <= INT8_MAX; i++)
  {
    int16_t fixed_point = Get_Flow_Rate(deltaP, backP, temp, i);
    int16_t floating_point =
      Calculate_Flow_Using_Floating_Point(deltaP, backP, temp, i);

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
