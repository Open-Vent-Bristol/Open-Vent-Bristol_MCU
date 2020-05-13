#include <math.h>

#include "unity.h"
#include "unity_fixture.h"
#include "flow/flow.h"
#include "flow/flow_parameters.def"

#define TOLERANCE (4)

extern int16_t Calculate_Back_Pressure(uint16_t back_pressure_raw);

int16_t Calculate_Flow_Using_Floating_Point(
          int16_t pressure_delta_raw,
          int16_t back_pressure_raw,
          Flow_Temperature_t ambient_temperature,
          Flow_CorrectionFactor_t correction_factor)
{
  double pressure_delta_mV =
    pressure_delta_raw * (double)ADC_REF_MV / (double)ADC_RESOLUTION;
  double pressure_delta = (pressure_delta_mV - MV_AT_ZERO_PA) / MV_PER_PA;
  double flow = pow(pressure_delta, 2.0);
  flow = pow(flow, E1 / 2.0);
  flow *= C1;

  double back_pressure_mV = back_pressure_raw * ADC_REF_MV / ADC_RESOLUTION;
  double back_pressure = (back_pressure_mV - MV_AT_ZERO_PA) / MV_PER_PA;
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

TEST(flow_tests, flow_lookup_table_correct)
{
  for (uint16_t i = 0u; i < ADC_RESOLUTION; i++)
  {
    int8_t sign = 1;

    // Calculate pressure
    double flow = i * (double)ADC_REF_MV / (double)ADC_RESOLUTION;
    flow = (flow - MV_AT_ZERO_PA) / MV_PER_PA;

    if (flow < 0)
    {
      sign = -1;
    }
    flow = pow(flow, 2.0);

    // Calculate flow parameter
    flow = pow(flow, E1 / 2.0);
    flow *= C1;
    flow *= sign;

    int16_t difference;
    if (flow_lookup_table[i] > (int16_t)flow)
    {
      difference = flow_lookup_table[i] - (int16_t)flow;
    }
    else
    {
      difference = (int16_t)flow - flow_lookup_table[i];
    }

    // More stringent tolerance here because it is the most significant
    // parameter in the calculation.  Results must be within +/- 1
    TEST_ASSERT_LESS_THAN(2u, difference);
  }
}

TEST(flow_tests, Calculate_Back_Pressure_within_tolerance)
{
  for (uint16_t i = 0u; i < ADC_RESOLUTION; i++)
  {
    int16_t fixed_point = Calculate_Back_Pressure(i);
    double floating_point = i * (double)ADC_REF_MV / (double)ADC_RESOLUTION;
    floating_point = (floating_point - MV_AT_ZERO_PA) / MV_PER_PA;

    int16_t difference;
    if (fixed_point > (int16_t)floating_point)
    {
      difference = fixed_point - (int16_t)floating_point;
    }
    else
    {
      difference = (int16_t)floating_point - fixed_point;
    }

    TEST_ASSERT_LESS_THAN(TOLERANCE + 1u, difference);
  }
}

TEST(flow_tests, output_within_tolerance_deltaPressure)
{
  const uint16_t backP = 10;
  const Flow_Temperature_t temp = 298u << 7u;
  const Flow_CorrectionFactor_t cf = 0;

  for (uint16_t i = 0u; i < ADC_RESOLUTION; i++)
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
  const uint16_t deltaP = 20;
  const Flow_Temperature_t temp = 310u << 7u;
  const Flow_CorrectionFactor_t cf = 0;

  for (uint16_t i = 0u; i < ADC_RESOLUTION; i++)
  {
    int16_t fixed_point = Get_Flow_Rate(deltaP, i, temp, cf);
    int16_t floating_point =
      Calculate_Flow_Using_Floating_Point(deltaP, i, temp, cf);

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
  const uint16_t deltaP = 20;
  const uint16_t backP = 5;
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
  const uint16_t deltaP = 20;
  const uint16_t backP = 5;
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

TEST(flow_tests, output_with_maximum_param_values)
{
  // Largest possible values
  uint16_t deltaP = INT16_MAX;
  uint16_t backP = INT16_MAX;
  Flow_Temperature_t temp = UINT16_MAX;
  Flow_CorrectionFactor_t cf = INT8_MAX;

  int16_t fixed_point = Get_Flow_Rate(deltaP, backP, temp, cf);
  int16_t floating_point =
    Calculate_Flow_Using_Floating_Point(deltaP, backP, temp, cf);

  uint16_t difference;
  if (fixed_point > floating_point)
  {
    difference = fixed_point - floating_point;
  }
  else
  {
    difference = floating_point - fixed_point;
  }

  printf("fixed: %d, float: %d", fixed_point, floating_point);
  TEST_ASSERT_LESS_THAN(TOLERANCE + 1u, difference);
}