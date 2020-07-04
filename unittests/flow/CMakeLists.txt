project(flow_unittest)

include(../unittests_common.cmake)

enable_testing()

add_executable(flow_unittest
  ${UNITY_SRC}
  ../../code/application/flow/flow.c
  flow_unittest.c
  all_tests.c
)

target_compile_definitions(flow_unittest
  PRIVATE
    PROGMEM=
)

add_test(NAME flow_unittest COMMAND flow_unittest -v)
