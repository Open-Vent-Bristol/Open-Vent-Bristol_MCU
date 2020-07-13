#ifndef MOCK_MOTOR_MOCK_H
#define MOCK_MOTOR_MOCK_H


#include "drivers/motor.hpp"
#include "gmock/gmock.h"

class MotorMock: public Motor
{
public:
  MotorMock() = default;
  MotorMock(MotorMock const&) = delete;
  MotorMock& operator=(MotorMock const&) = delete;
  MOCK_METHOD(void, setDirection, (MotorState state), (override));
  MOCK_METHOD(void, setSpeed, (uint8_t speed), (override));
  MOCK_METHOD(void, mockInitialize, (), ());
protected:
  void initialize() override { mockInitialize(); };
};


#endif /* MOCK_MOTOR_MOCK_H */