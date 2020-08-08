add_subdirectory(hal)

add_library(OpenVent_drivers STATIC)
target_include_directories(OpenVent_drivers
  PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
)
add_subdirectory(boards/common)

if(${BOARD_CONFIG} STREQUAL BOARD_SIMULATED)
  add_subdirectory(boards/simulated)
elseif(${BOARD_CONFIG} STREQUAL BOARD_MK2)
  add_subdirectory(boards/board_mk2)
else()
  message(FATAL_ERROR "Unknown board type ${BOARD_CONFIG}")
endif()

target_compile_definitions(OpenVent_drivers PUBLIC ${BOARD_CONFIG})
target_link_libraries(OpenVent_drivers PUBLIC OpenVent_hal)

if(${PACKAGE_TESTS})
  add_library(OpenVent_mock_drivers INTERFACE)
  target_include_directories(OpenVent_mock_drivers
    INTERFACE
      $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  )
  target_compile_definitions(OpenVent_mock_drivers INTERFACE BOARD_MOCK)
endif()