cmake_minimum_required(VERSION 3.13)

project(OpenVent_unittest
    VERSION 1.0
    DESCRIPTION "OpenVent Unit Tests"
    LANGUAGES C
)

add_executable(flow_unittest)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g3 -O0 -Wall -Werror")

target_compile_definitions(flow_unittest
  PRIVATE
    DEBUG=
    UTEST_MODE=
    PROGMEM=
)

target_include_directories(flow_unittest
  PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ../mocks
    ../../code
)

target_sources(flow_unittest
  PRIVATE
    ../../code/flow/flow.c
    flow_unittest.c
)
