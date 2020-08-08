#include "drivers/sensors.hpp"
#include "drivers/system.hpp"

#include <cassert>
#include <cstring>

void Sensors::initialize()
{

}

Error Sensors::beginReadings()
{
  if(isInProgress()) {
    return Error::SENSORS_NOT_READY;
  }
  return Error::NONE;
}

Error Sensors::getReadings(ADC_resolution_t* values)
{
  if(isInProgress()) {
    return Error::SENSORS_NOT_READY;
  }
  return Error::NONE;
}

bool Sensors::isInProgress()
{
  return false;
}

void updateSensorValues(ADC_resolution_t* readings)
{

}