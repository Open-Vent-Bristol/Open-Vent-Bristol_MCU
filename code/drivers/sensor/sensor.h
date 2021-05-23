// Copyright (c) 2021 <OpenVent-Bristol, Donald Robson>

#ifndef SENSOR_H
#define SENSOR_H

#include <stdint.h>

enum sensor_index
{
  SENSOR_FLOW_RATE,
  SENSOR_PEAK_FLOW_RATE,
  SENSOR_PERCENT_O2,
  SENSOR_PRESSURE,
  SENSOR_PEAK_PRESSURE,
  SENSOR_RESPIRATION_RATE,
  SENSOR_TEMPERATURE,
  SENSOR_TIDAL_VOLUME,

  SENSOR_COUNT_STORABLE,

  // These sensor readings will be fetched on demand
  SENSOR_PERCENT_BATTERY_CHARGE,
  SENSOR_MOTOR_CURRENT,

  SENSOR_COUNT_ALL
};

/**
 * @brief Store a sensor reading
 *
 * @param sensor    See enum sensor_index
 * @param reading   Value to store
 */
void sensor_store_reading(enum sensor_index sensor, int16_t reading);

/**
 * @brief Get a sensor reading
 *
 * @param sensor    See enum sensor_index
 */
int16_t sensor_get_reading(enum sensor_index sensor);

#endif /* SENSOR_H */
