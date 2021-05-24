cmake_minimum_required(VERSION 3.13)

project(state_machine_unittest)

include(../unittests_common.cmake)

enable_testing()

add_executable(state_machine_unittest
  ${UNITY_SRC}
  ${CODE}/application/scheduler/state_machine.c
  state_machine_unittest.c
  all_tests.c
)

target_compile_definitions(state_machine_unittest
  PRIVATE
)

add_test(NAME state_machine_unittest COMMAND state_machine_unittest -v)
