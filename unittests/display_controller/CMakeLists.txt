cmake_minimum_required(VERSION 3.13)

project(display_controller_unittest)

include(../unittests_common.cmake)

enable_testing()

add_executable(display_controller_unittest
  ${UNITY_SRC}
  ${CODE}/drivers/fpga/display_controller.c
  display_controller_unittest.c
  all_tests.c
)

target_compile_definitions(display_controller_unittest
  PRIVATE
)

add_test(NAME display_controller_unittest COMMAND display_controller_unittest -v)
