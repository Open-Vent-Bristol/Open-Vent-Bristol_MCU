// Copyright (c) 2021 <OpenVent-Bristol, Donald Robson>

#include "sensor.h"
#include "adc/adc.h"

uint32_t s_readings[SENSOR_COUNT_STORABLE];

void sensor_store_reading(enum sensor_index sensor, uint16_t reading)
{
  switch (sensor)
  {
  case SENSOR_FLOW_RATE:
  case SENSOR_PEAK_FLOW_RATE:
  case SENSOR_PERCENT_O2:
  case SENSOR_PRESSURE:
  case SENSOR_TIDAL_VOLUME:
    s_readings[sensor] = reading;

  case SENSOR_BATTERY_VOLTAGE:
  case SENSOR_MOTOR_CURRENT:
  default:
    return;
  }
}

uint16_t sensor_get_reading(enum sensor_index sensor)
{
  switch (sensor)
  {
  case SENSOR_FLOW_RATE:
  case SENSOR_PEAK_FLOW_RATE:
  case SENSOR_PERCENT_O2:
  case SENSOR_PRESSURE:
  case SENSOR_TIDAL_VOLUME:
    return s_readings[sensor];

  case SENSOR_BATTERY_VOLTAGE:
    // TODO - conversion
    return adc_read_blocking(ADC_VBATT_SCALED);

  case SENSOR_MOTOR_CURRENT:
    // TODO - conversion
    return adc_read_blocking(ADC_MOTOR_CURR);

  default:
    return 0u;
  }
}
