set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g3 -O0 -Wall -Werror")

set(UNITY_SRC
  ../frameworks/Unity/src/unity.c
  ../frameworks/Unity/extras/fixture/src/unity_fixture.c
  ../frameworks/Unity/extras/memory/src/unity_memory.c
)

set(CODE ../../code)

add_definitions(
    -DDEBUG
    -DUTEST_MODE
)

include_directories(
  ../frameworks/fff
  ../frameworks/Unity/src
  ../frameworks/Unity/extras/fixture/src
  ../frameworks/Unity/extras/memory/src
  ../mocks
  ${CODE}/application
  ${CODE}/drivers
  ${CODE}/hal
)
