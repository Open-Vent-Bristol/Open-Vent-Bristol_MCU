cmake_minimum_required(VERSION 3.13)

project(display_unittest)

include(../unittests_common.cmake)

enable_testing()

add_executable(display_unittest
  ${UNITY_SRC}
  ${CODE}/drivers/fpga/display.c
  display_unittest.c
  all_tests.c
)

target_compile_definitions(display_unittest
  PRIVATE
)

add_test(NAME display_unittest COMMAND display_unittest -v)
