cmake_minimum_required(VERSION 3.13)

project(hardware_event_unittest)

include(../unittests_common.cmake)

enable_testing()

add_executable(hardware_event_unittest
  ${UNITY_SRC}
  ${CODE}/drivers/hardware_event/interrupt_handlers.c
  hardware_event_unittest.c
  all_tests.c
)

target_compile_definitions(hardware_event_unittest
  PRIVATE
)

add_test(NAME hardware_event_unittest COMMAND hardware_event_unittest -v)
