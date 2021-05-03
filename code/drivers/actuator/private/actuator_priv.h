// Copyright (c) 2021 <OpenVent-Bristol, Donald Robson>

#ifndef ACTUATOR_PRIV_H
#define ACTUATOR_PRIV_H

#include <stdint.h>

#define CONSTANT_SPEED_CAP  (63u)
#define ACCELERATION_CAP    (2) // PWM duty change per millisecond

enum actuator_mode
{
  STOP,
  PUSHING,
  PUSHING_PID,
  HOMING,
  FAULT
};

typedef struct
{
  enum actuator_mode mode;
  uint8_t current_speed;
  uint8_t target_speed;
} actuator_t;

#endif /* ACTUATOR_PRIV_H */
