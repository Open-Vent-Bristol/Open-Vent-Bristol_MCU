cmake_minimum_required(VERSION 3.13)

project(alarm_unittest)

include(../unittests_common.cmake)

enable_testing()

add_executable(alarm_unittest
  ${UNITY_SRC}
  ${CODE}/drivers/alarm/alarm.c
  alarm_unittest.c
  all_tests.c
)

target_compile_definitions(alarm_unittest
  PRIVATE
)

add_test(NAME alarm_unittest COMMAND alarm_unittest -v)
