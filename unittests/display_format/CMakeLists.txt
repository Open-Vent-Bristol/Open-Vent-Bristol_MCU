cmake_minimum_required(VERSION 3.13)

project(display_format_unittest)

include(../unittests_common.cmake)

enable_testing()

add_executable(display_format_unittest
  ${UNITY_SRC}
  ${CODE}/drivers/fpga/private/display_format.c
  display_format_unittest.c
  all_tests.c
)

target_compile_definitions(display_format_unittest
  PRIVATE
)

add_test(NAME display_format_unittest COMMAND display_format_unittest -v)
