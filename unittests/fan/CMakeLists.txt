cmake_minimum_required(VERSION 3.13)

project(fan_unittest)

include(../unittests_common.cmake)

enable_testing()

add_executable(fan_unittest
  ${UNITY_SRC}
  ${CODE}/drivers/cooling/fan.c
  fan_unittest.c
  all_tests.c
)

target_compile_definitions(fan_unittest
  PRIVATE
)

add_test(NAME fan_unittest COMMAND fan_unittest -v)
