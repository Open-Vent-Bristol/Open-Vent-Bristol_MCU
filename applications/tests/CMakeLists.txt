package_add_test(TESTNAME test_measurement_display
  FILES
    test_measurement_display.cpp
    ../vent/measurement_display.cpp
  LIBRARIES
    OpenVent_mock_drivers
  INCLUDES
    ../vent
)