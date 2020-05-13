#include "unity.h"
#include "unity_fixture.h"

TEST_GROUP_RUNNER(flow_tests)
{
  RUN_TEST_CASE(flow_tests, flow_lookup_table_correct);
  RUN_TEST_CASE(flow_tests, Calculate_Back_Pressure_within_tolerance);

  RUN_TEST_CASE(flow_tests, output_within_tolerance_deltaPressure);
  RUN_TEST_CASE(flow_tests, output_within_tolerance_backPressure);
  RUN_TEST_CASE(flow_tests, output_within_tolerance_ambientTemp);
  RUN_TEST_CASE(flow_tests, output_within_tolerance_correctionFactor);

  RUN_TEST_CASE(flow_tests, output_with_maximum_param_values);
}

static void RunAllTests(void)
{
  RUN_TEST_GROUP(flow_tests);
}

int main(int argc, const char* argv[])
{
  return UnityMain(argc, argv, RunAllTests);
}
