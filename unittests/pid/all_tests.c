// Copyright (c) 2021 <OpenVent-Bristol, Donald Robson>

#include "unity.h"
#include "unity_fixture.h"

TEST_GROUP_RUNNER(pid_tests)
{
  RUN_TEST_CASE(pid_tests, elapsed_time_zero);

  RUN_TEST_CASE(pid_tests, p_output_only);
  RUN_TEST_CASE(pid_tests, i_output_only);
  RUN_TEST_CASE(pid_tests, d_output_only);
  RUN_TEST_CASE(pid_tests, pid_output);

  RUN_TEST_CASE(pid_tests, input_cannot_overflow_16bit);
  RUN_TEST_CASE(pid_tests, output_cannot_overflow_16bit);
  RUN_TEST_CASE(pid_tests, output_cannot_overflow_8bit);
}

static void RunAllTests(void)
{
  RUN_TEST_GROUP(pid_tests);
}

int main(int argc, const char* argv[])
{
  return UnityMain(argc, argv, RunAllTests);
}
