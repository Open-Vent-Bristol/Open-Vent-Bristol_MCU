cmake_minimum_required(VERSION 3.13)

project(pid_unittest)

include(../unittests_common.cmake)

enable_testing()

add_executable(pid_unittest
  ${UNITY_SRC}
  ${CODE}/drivers/actuator/private/pid.c
  pid_unittest.c
  all_tests.c
)

target_compile_definitions(pid_unittest
  PRIVATE
)

add_test(NAME pid_unittest COMMAND pid_unittest -v)
