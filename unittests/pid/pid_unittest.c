// Copyright (c) 2021 <OpenVent-Bristol, Donald Robson>

#include "unity.h"
#include "unity_fixture.h"
#include "actuator/private/pid.h"

TEST_GROUP(pid_tests);

TEST_SETUP(pid_tests)
{}

TEST_TEAR_DOWN(pid_tests)
{}

TEST(pid_tests, elapsed_time_zero)
{
  pid_data_t pid =
  {
    .output_max = INT16_MAX,
    .output_min = INT16_MIN,
    .output = 1,
    .kp = 1,
    .ki = 1,
    .kd = 1,
  };

  pid_update(&pid, 0u, 99);

  // Check output is unchanged - dividing a float by zero will not halt the program!
  TEST_ASSERT_EQUAL_INT(1, pid.output);
}

TEST(pid_tests, p_output_only)
{
  pid_data_t pid =
  {
    .output_max = INT16_MAX,
    .output_min = INT16_MIN,
    .set_point = 2000,
    .kp = 1,
    .ki = 0,
    .kd = 0,
  };

  pid_update(&pid, 100u, 999);
  TEST_ASSERT_EQUAL_INT(1001, pid.output);

  pid.set_point = 1000;
  pid.kp = 5;

  pid_update(&pid, 100u, 500);
  TEST_ASSERT_EQUAL_INT(2500, pid.output);

  pid.kp = 5.5;

  pid_update(&pid, 100u, 1500);
  TEST_ASSERT_EQUAL_INT(-2750, pid.output);

  pid_update(&pid, 100u, 2500);
  TEST_ASSERT_EQUAL_INT(-8250, pid.output);
}

TEST(pid_tests, i_output_only)
{
  pid_data_t pid =
  {
    .output_max = INT16_MAX,
    .output_min = INT16_MIN,
    .set_point = 2000,
    .kp = 0,
    .ki = 1,
    .kd = 0,
  };

  pid_update(&pid, 50u, 1900);
  TEST_ASSERT_EQUAL_INT(5000, pid.output);

  float last_integral = pid.integral;
  pid.set_point = 1000;
  pid.ki = 5;

  pid_update(&pid, 10u, 950);
  TEST_ASSERT_EQUAL_INT(5 * (last_integral + 500), pid.output);

  last_integral = pid.integral;
  pid.ki = 5.5;

  pid_update(&pid, 1u, 1100);
  TEST_ASSERT_EQUAL_INT(5.5 * (last_integral + -100), pid.output);

  last_integral = pid.integral;

  pid.set_point = 10;
  pid_update(&pid, 100u, 11);
  TEST_ASSERT_EQUAL_INT(5.5 * (last_integral + -100), pid.output);
}

TEST(pid_tests, d_output_only)
{
  pid_data_t pid =
  {
    .output_max = INT16_MAX,
    .output_min = INT16_MIN,
    .set_point = 2000,
    .kp = 0,
    .ki = 0,
    .kd = 1,
  };

  pid_update(&pid, 100u, 999);
  TEST_ASSERT_EQUAL_INT(10, pid.output);

  int32_t last_error = pid.previous_error;
  pid.set_point = 1000;
  pid.kd = 5;

  pid_update(&pid, 10u, 500);
  TEST_ASSERT_EQUAL_INT(-251, pid.output);

  last_error = pid.previous_error;
  pid.kd = 5.5;

  pid_update(&pid, 1u, 1500);
  TEST_ASSERT_EQUAL_INT(-5500, pid.output);

  last_error = pid.previous_error;

  pid_update(&pid, 100u, 2500);
  TEST_ASSERT_EQUAL_INT(-55, pid.output);
}

TEST(pid_tests, pid_output)
{
  pid_data_t pid =
  {
    .output_max = INT16_MAX,
    .output_min = INT16_MIN,
    .set_point = 2000,
    .kp = 11,
    .ki = 19,
    .kd = 29,
  };

  pid_update(&pid, 10u, 1899);
  TEST_ASSERT_EQUAL_INT(1111 + 19190 + 293, pid.output);

  pid.integral = 0;
  pid.previous_error = 0;

  pid_update(&pid, 10u, 2101);
  TEST_ASSERT_EQUAL_INT(-1111 - 19190 - 293, pid.output);
}

TEST(pid_tests, returns_output)
{
  pid_data_t pid =
  {
    .output_max = INT16_MAX,
    .output_min = INT16_MIN,
    .set_point = 100,
    .kp = 1,
    .ki = 0,
    .kd = 0,
  };

  int16_t output = pid_update(&pid, 10u, 80);
  TEST_ASSERT_EQUAL_INT(20, pid.output);
  TEST_ASSERT_EQUAL_INT(20, output);
}

TEST(pid_tests, input_cannot_overflow_16bit)
{
  pid_data_t pid =
  {
    .output_max = INT16_MAX,
    .output_min = INT16_MIN,
    .set_point = INT16_MAX,
    .kp = 1,
    .ki = 0,
    .kd = 0,
  };

  pid_update(&pid, 1, INT16_MIN);
  TEST_ASSERT_EQUAL(UINT16_MAX, pid.previous_error);

  pid.integral = 0;
  pid.previous_error = 0;
  pid.set_point = INT16_MIN;

  pid_update(&pid, 1, INT16_MAX);
  TEST_ASSERT_EQUAL(-(int32_t)UINT16_MAX, pid.previous_error);
}

TEST(pid_tests, output_cannot_overflow_16bit)
{
  pid_data_t pid =
  {
    .output_max = INT16_MAX,
    .output_min = INT16_MIN,
    .set_point = INT16_MAX,
    .kp = 2,
    .ki = 0,
    .kd = 0,
  };

  pid_update(&pid, 1, 0);
  TEST_ASSERT_EQUAL(INT16_MAX, pid.output);

  pid.integral = 0;
  pid.previous_error = 0;
  pid.set_point = INT16_MIN;

  pid_update(&pid, 1, 0);
  TEST_ASSERT_EQUAL(INT16_MIN, pid.output);
}

TEST(pid_tests, output_cannot_overflow_8bit)
{
  pid_data_t pid =
  {
    .output_max = 255,
    .output_min = 0,
    .set_point = INT16_MAX,
    .kp = 2,
    .ki = 0,
    .kd = 0,
  };

  pid_update(&pid, 1, 0);
  TEST_ASSERT_EQUAL(255, pid.output);

  pid.integral = 0;
  pid.previous_error = 0;
  pid.set_point = INT16_MIN;

  pid_update(&pid, 1, 0);
 TEST_ASSERT_EQUAL(0, pid.output);
}
