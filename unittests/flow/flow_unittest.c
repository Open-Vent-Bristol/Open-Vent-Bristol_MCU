#include <stdio.h>
#include <math.h>

#include "flow/flow.h"
#include "flow/flow_parameters.def"

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

int main()
{
  printf("\nPressure full range, with T=298K and back pressure 100Pa:");
  for (uint16_t i = 0u; i < 25; i++)
  {
    printf("\n+%u: %d, %d",
      i,
      Get_Flow_Rate(i, 100, 298, 0),
      Calculate_Flow_Using_Floating_Point(i, 100, 298, 0));
    printf("\n-%u: %d, %d",
      i,
      Get_Flow_Rate(0-i, 100, 298, 0),
      Calculate_Flow_Using_Floating_Point(i, 100, 298, 0));
  }

  // printf("\nT=273-323K with pressure 15Pa and back pressure 100Pa:");
  // for (uint16_t i = 273u; i <= 323u; i++)
  // {
  //   printf("\n%u: %d", i, Get_Flow_Rate(15, 100, i, 0));
  // }

  puts("\n");
}
