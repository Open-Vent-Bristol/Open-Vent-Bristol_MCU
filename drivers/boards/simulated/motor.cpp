#include "drivers/motor.hpp"
#include <cstdio>

void Motor::initialize()
{
  printf("motor init\n");
}

void Motor::setDirection(MotorState state)
{
  switch (state)
  {
  case MotorState::OFF:
    printf("motor direction off\n");
    break;
  case MotorState::FORWARD:
    printf("motor direction fwd\n");
    break;
  case MotorState::REVERSE:
    printf("motor direction rev\n");
    break;
  }
}

void Motor::setSpeed(uint8_t speed)
{
  printf("motor speed %hhu\n", speed);
}