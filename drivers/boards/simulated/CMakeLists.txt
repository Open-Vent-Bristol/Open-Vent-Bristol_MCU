FIND_PACKAGE ( Threads REQUIRED )

target_sources(OpenVent_drivers
  PUBLIC
    main.cpp
    motor.cpp
    display.cpp
    scheduler.cpp
    sensors.cpp
    system.cpp
)

target_link_libraries(OpenVent_drivers ${CMAKE_THREAD_LIBS_INIT})