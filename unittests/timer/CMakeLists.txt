cmake_minimum_required(VERSION 3.13)

project(timer_unittest)

include(../unittests_common.cmake)

enable_testing()

add_executable(timer_unittest
  ${UNITY_SRC}
  ${CODE}/application/scheduler/timer.c
  timer_unittest.c
  all_tests.c
)

target_compile_definitions(timer_unittest
  PRIVATE
)

add_test(NAME timer_unittest COMMAND timer_unittest -v)
