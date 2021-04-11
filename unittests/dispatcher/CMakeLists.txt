cmake_minimum_required(VERSION 3.13)

project(dispatcher_unittest)

include(../unittests_common.cmake)

enable_testing()

add_executable(dispatcher_unittest
  ${UNITY_SRC}
  ${CODE}/application/scheduler/dispatcher.c
  dispatcher_unittest.c
  all_tests.c
)

target_compile_definitions(dispatcher_unittest
  PRIVATE
)

add_test(NAME dispatcher_unittest COMMAND dispatcher_unittest -v)
