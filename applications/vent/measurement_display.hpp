#ifndef MEASUREMENT_DISPLAY_H
#define MEASUREMENT_DISPLAY_H

#include "drivers/display.hpp"

class MeasurementDisplay
{
public:
  MeasurementDisplay(Display& display);
  MeasurementDisplay(MeasurementDisplay const&) = delete;
  MeasurementDisplay& operator=(MeasurementDisplay const&) = delete;
  void updateTidalVolume(unsigned value);
  void updateFlowRate(float value);
  void updateFiO2(unsigned value);
  void updatePressure(unsigned value);
private:
  Display& display;
};

#endif /* MEASUREMENT_DISPLAY_H */