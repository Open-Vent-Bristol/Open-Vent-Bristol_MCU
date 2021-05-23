// Copyright (c) 2021 <OpenVent-Bristol, Donald Robson>

#include "unity.h"
#include "unity_fixture.h"

TEST_GROUP_RUNNER(display_controller_test)
{
  RUN_TEST_CASE(display_controller_test, init_binds_event);

  RUN_TEST_CASE(display_controller_test, set_override_invalid_arg);
  RUN_TEST_CASE(display_controller_test, set_override_valid_arg);

  RUN_TEST_CASE(display_controller_test, set_line_2_override);
}

static void RunAllTests(void)
{
  RUN_TEST_GROUP(display_controller_test);
}

int main(int argc, const char* argv[])
{
  return UnityMain(argc, argv, RunAllTests);
}
