// Copyright (c) 2021 <OpenVent-Bristol, Donald Robson>

#ifndef SENSOR_MOCK_H
#define SENSOR_MOCK_H

#include "fff.h"
#include "sensor/sensor.h"

#define FFF_GCC_FUNCTION_ATTRIBUTES

FAKE_VOID_FUNC(sensor_store_reading, enum sensor_index, int16_t);

FAKE_VALUE_FUNC(int16_t, sensor_get_reading, enum sensor_index);

#define SENSOR_MOCKS(x) \
  x(sensor_store_reading) \
  x(sensor_get_reading)

#endif /* SENSOR_MOCK_H */
