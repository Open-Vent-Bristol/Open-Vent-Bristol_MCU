#ifndef DRIVERS_MOTOR_H
#define DRIVERS_MOTOR_H

#include "types/types.h"
#include <cstdint>

enum class MotorState {
  OFF,
  FORWARD,
  REVERSE
};

class Motor
{
public:
  Motor(Motor const&) = delete;
  Motor& operator=(Motor const&) = delete;
  TESTABLE_DESTRUCTOR(Motor);
  TESTABLE_PREFIX void setDirection(MotorState state) TESTABLE_SUFFIX;
  TESTABLE_PREFIX void setSpeed(uint8_t speed) TESTABLE_SUFFIX;
protected:
  Motor() = default;
  TESTABLE_PREFIX void initialize() TESTABLE_SUFFIX;
  friend class System;
};

#endif /* DRIVERS_MOTOR_H */