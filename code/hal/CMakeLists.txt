add_library(OpenVent_hal)

add_subdirectory(adc)
add_subdirectory(board)
add_subdirectory(clock)
add_subdirectory(gpio)
add_subdirectory(spi)
add_subdirectory(types)
add_subdirectory(uart)

add_custom_target(crtatmega328pb
  COMMAND ${CMAKE_COMMAND} -E copy
  ${CMAKE_CURRENT_SOURCE_DIR}/avr/crtatmega328pb.o
  ${CMAKE_BINARY_DIR}
)

add_dependencies(OpenVent_firmware crtatmega328pb)
