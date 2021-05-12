cmake_minimum_required(VERSION 3.13)

project(messages_unittest)

include(../unittests_common.cmake)

enable_testing()

add_executable(messages_unittest
  ${UNITY_SRC}
  ${CODE}/drivers/fpga/messages.c
  messages_unittest.c
  all_tests.c
)

target_compile_definitions(messages_unittest
  PRIVATE
)

add_test(NAME messages_unittest COMMAND messages_unittest -v)
