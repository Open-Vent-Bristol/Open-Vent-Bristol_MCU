#ifndef DRIVERS_SENSORS_H
#define DRIVERS_SENSORS_H

#include "types/types.h"
#include "drivers/error.hpp"
#include <atomic>

enum class SensorType
{
  TEMPERATURE=0,
  PRESSURE_OUT=1,
  FLOW_OUT=2,
  VBATT=3,
  NUM_SENSORS=4
};

class Sensors
{
public:
  Sensors(Sensors const&) = delete;
  Sensors& operator=(Sensors const&) = delete;
  TESTABLE_DESTRUCTOR(Sensors);
  TESTABLE_PREFIX Error beginReadings() TESTABLE_SUFFIX;
  TESTABLE_PREFIX bool isInProgress() TESTABLE_SUFFIX;
  TESTABLE_PREFIX Error getReadings(ADC_resolution_t* values) TESTABLE_SUFFIX;
protected:
  Sensors() = default;
  TESTABLE_PREFIX void initialize() TESTABLE_SUFFIX;
  friend class System;
};

#endif /* DRIVERS_SENSORS_H */