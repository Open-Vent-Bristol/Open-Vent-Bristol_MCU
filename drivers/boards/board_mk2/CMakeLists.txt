target_link_libraries(OpenVent_drivers PRIVATE OpenVent_hal)

target_sources(OpenVent_drivers
  PUBLIC
    main.cpp
    motor.cpp
    display.cpp
    scheduler.cpp
    sensors.cpp
    system.cpp
)