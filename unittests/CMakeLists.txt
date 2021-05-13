cmake_minimum_required(VERSION 3.13)

project(OpenVent_unittest
    VERSION 1.0
    DESCRIPTION "OpenVent Unit Tests"
    LANGUAGES C
)

enable_testing()

add_subdirectory(actuator)
add_subdirectory(alarm)
add_subdirectory(dispatcher)
add_subdirectory(display)
add_subdirectory(messages)
add_subdirectory(pid)
add_subdirectory(state_machine)
add_subdirectory(timer)
