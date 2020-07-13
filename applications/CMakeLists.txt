
add_executable(OpenVent_firmware
    vent/application_main.cpp
    vent/measurement_display.cpp
)
target_include_directories(OpenVent_firmware
  PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/vent>
)
target_link_libraries(OpenVent_firmware PUBLIC OpenVent_drivers)

if(PACKAGE_TESTS)
  add_subdirectory(tests)
endif()