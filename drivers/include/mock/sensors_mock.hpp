#ifndef MOCK_SENSORS_MOCK_H
#define MOCK_SENSORS_MOCK_H


#include "drivers/sensors.hpp"
#include "gmock/gmock.h"

class SensorsMock: public Sensors
{
public:
  SensorsMock() = default;
  SensorsMock(SensorsMock const&) = delete;
  SensorsMock& operator=(SensorsMock const&) = delete;
  MOCK_METHOD(Error, beginReadings, (), (override));
  MOCK_METHOD(bool, isInProgress, (), (override));
  MOCK_METHOD(Error, getReadings, (ADC_resolution_t* values), (override));
  MOCK_METHOD(void, mockInitialize, (), ());
protected:
  void initialize() override { mockInitialize(); };
};


#endif /* MOCK_SENSORS_MOCK_H */