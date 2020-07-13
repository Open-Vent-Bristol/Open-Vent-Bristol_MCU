#include "sensors_priv.hpp"
#include "drivers/sensors.hpp"
#include "drivers/system.hpp"

#include <cassert>
#include <cstring>
#include <cstdio>

static ADC_resolution_t staticReadings[static_cast<unsigned>(SensorType::NUM_SENSORS)];
static std::atomic_bool inProgress;

void Sensors::initialize()
{
  printf("sensors init\n");
}

Error Sensors::beginReadings()
{
  if(isInProgress()) {
    return Error::SENSORS_NOT_READY;
  }
  inProgress = true;
  printf("sensors begin\n");
  return Error::NONE;
}

Error Sensors::getReadings(ADC_resolution_t* values)
{
  if(isInProgress()) {
    return Error::SENSORS_NOT_READY;
  }
  memcpy(values, staticReadings, static_cast<unsigned>(SensorType::NUM_SENSORS)*sizeof(ADC_resolution_t));
  return Error::NONE;
}

bool Sensors::isInProgress()
{
  return inProgress.load();
}

void updateSensorValues(ADC_resolution_t* readings)
{
  assert(nullptr != readings);
  memcpy(staticReadings, readings, static_cast<unsigned>(SensorType::NUM_SENSORS)*sizeof(ADC_resolution_t));
  inProgress = false;
}