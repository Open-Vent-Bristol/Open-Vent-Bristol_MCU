#ifndef DRIVERS_SYSTEM_H
#define DRIVERS_SYSTEM_H


#include "drivers/display.hpp"
#include "drivers/motor.hpp"
#include "drivers/scheduler.hpp"
#include "drivers/sensors.hpp"

class System
{
public:
    System() = delete;
    System(System const&) = delete;
    System& operator=(System const&) = delete;

    static void initialize(void);
    static Display& getDisplay(DisplayIndex idx);
    static Motor& getMotor(void);
    static Scheduler& getScheduler(void);
    static Sensors& getSensors(void);
};


#endif /* DRIVERS_SYSTEM_H */