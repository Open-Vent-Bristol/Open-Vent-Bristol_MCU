cmake_minimum_required(VERSION 3.13)

project(OpenVent_firmware
    VERSION 1.0
    DESCRIPTION "OpenVent Firmware"
    LANGUAGES CXX
)

option(PACKAGE_TESTS "Build the tests" ON)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
# specify the C++ standard
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# if(${BOARD_CONFIG})
# else()
  set(BOARD_CONFIG BOARD_SIMULATED)
# endif()

# This definition is used to select the correct source files in the hal
if(${BOARD_CONFIG} STREQUAL BOARD_MK1)
  set(MCU_TARGET atmega328p)
endif()

add_definitions(
  -D${BOARD_CONFIG}
)

if(PACKAGE_TESTS)
  enable_testing()
  include(GoogleTest)
  add_subdirectory("extern/googletest")
  # Keep cache clean
  mark_as_advanced(
    BUILD_GMOCK BUILD_GTEST BUILD_SHARED_LIBS
    gmock_build_tests gtest_build_samples gtest_build_tests
    gtest_disable_pthreads gtest_force_shared_crt gtest_hide_internal_symbols
  )
  # Keep IDE clean
  set_target_properties(gtest PROPERTIES FOLDER extern)
  set_target_properties(gtest_main PROPERTIES FOLDER extern)
  set_target_properties(gmock PROPERTIES FOLDER extern)
  set_target_properties(gmock_main PROPERTIES FOLDER extern)

  macro(package_add_test)
    cmake_parse_arguments(PACKAGE_TEST "" "TESTNAME" "FILES;LIBRARIES;INCLUDES" ${ARGN})
    add_executable(${PACKAGE_TEST_TESTNAME} ${PACKAGE_TEST_FILES})
    target_link_libraries(${PACKAGE_TEST_TESTNAME} gtest gmock gtest_main ${PACKAGE_TEST_LIBRARIES})
    if(DEFINED PACKAGE_TEST_INCLUDES)
      target_include_directories(${PACKAGE_TEST_TESTNAME} PRIVATE ${PACKAGE_TEST_INCLUDES})
    endif()
    gtest_discover_tests(${PACKAGE_TEST_TESTNAME})
    set_target_properties(${PACKAGE_TEST_TESTNAME} PROPERTIES FOLDER tests)
  endmacro()
endif()

add_subdirectory(drivers)
add_subdirectory(applications)