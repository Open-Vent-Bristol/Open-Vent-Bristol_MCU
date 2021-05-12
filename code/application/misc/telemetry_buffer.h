// Copyright (c) 2021 <OpenVent-Bristol, Donald Robson>

#ifndef TELEMETRY_BUFFER_H
#define TELEMETRY_BUFFER_H

#include <stdint.h>

typedef struct
{
  uint8_t read_position;
  uint8_t write_position;
  uint8_t length;
  uint32_t data[8];
} telemetry_buffer_t;

#endif /* TELEMETRY_BUFFER_H */
