cmake_minimum_required(VERSION 3.13)

project(actuator_unittest)

include(../unittests_common.cmake)

enable_testing()

add_executable(actuator_unittest
  ${UNITY_SRC}
  ${CODE}/drivers/actuator/actuator.c
  actuator_unittest.c
  all_tests.c
)

target_compile_definitions(actuator_unittest
  PRIVATE
)

add_test(NAME actuator_unittest COMMAND actuator_unittest -v)
