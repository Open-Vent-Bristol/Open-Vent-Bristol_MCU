#include "drivers/system.hpp"
#include "measurement_display.hpp"

#include <cassert>

void application_main()
{
  System::initialize();
  MeasurementDisplay measurementDisplay(System::getDisplay(DisplayIndex::DISPLAY_0));
  measurementDisplay.updateTidalVolume(69);
}