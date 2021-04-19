#include "unity.h"
#include "unity_fixture.h"

TEST_GROUP_RUNNER(state_machine_tests)
{

}

static void RunAllTests(void)
{
  RUN_TEST_GROUP(state_machine_tests);
}

int main(int argc, const char* argv[])
{
  return UnityMain(argc, argv, RunAllTests);
}
