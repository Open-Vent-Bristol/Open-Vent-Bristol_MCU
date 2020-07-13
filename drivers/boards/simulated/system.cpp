#include "drivers/system.hpp"

#include <cassert>

void System::initialize(void)
{
  getScheduler().initialize();
  getDisplay(DisplayIndex::DISPLAY_0).initialize();
  getDisplay(DisplayIndex::DISPLAY_1).initialize();
  getSensors().initialize();
  getMotor().initialize();
}

Display& System::getDisplay(DisplayIndex idx)
{
  static Display display0(DisplayIndex::DISPLAY_0);
  static Display display1(DisplayIndex::DISPLAY_1);

  switch (idx)
  {
  case DisplayIndex::DISPLAY_0:
    return display0;
  case DisplayIndex::DISPLAY_1:
    return display1;
  default:
    assert(0);
  }
}

Motor& System::getMotor(void)
{
  static Motor motor;
  return motor;
}

Scheduler& System::getScheduler(void)
{
  static Scheduler scheduler;
  return scheduler;
}

Sensors& System::getSensors(void)
{
  static Sensors sensors;
  return sensors;
}