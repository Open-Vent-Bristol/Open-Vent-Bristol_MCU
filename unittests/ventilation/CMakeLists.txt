cmake_minimum_required(VERSION 3.13)

project(ventilation_unittest)

include(../unittests_common.cmake)

enable_testing()

add_executable(ventilation_unittest
  ${UNITY_SRC}
  ${CODE}/application/ventilation/ventilation.c
  ${CODE}/application/ventilation/private/ventilation_states.c
  ventilation_unittest.c
  all_tests.c
)

target_compile_definitions(ventilation_unittest
  PRIVATE
)

add_test(NAME ventilation_unittest COMMAND ventilation_unittest -v)
