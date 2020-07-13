#include "measurement_display.hpp"

#include <cstdio>
#include <cassert>

MeasurementDisplay::MeasurementDisplay(Display& display): display(display)
{

}

void MeasurementDisplay::updateTidalVolume(unsigned value)
{
  assert(value < 1000);
  char buffer[4];
  snprintf(buffer, 4, "%3u", value);
  this->display.setCursor(0, 0);
  this->display.write(buffer, 3);
}

void MeasurementDisplay::updateFlowRate(float value)
{
  assert(value < 1000);
  char buffer[7];
  snprintf(buffer, 7, "%3.2f", value);
  this->display.setCursor(5, 0);
  this->display.write(buffer, 6);
}

void MeasurementDisplay::updateFiO2(unsigned value)
{
  assert(value <= 100);
  char buffer[4];
  snprintf(buffer, 4, "%3u", value);
  this->display.setCursor(13, 0);
  this->display.write(buffer, 3);
}

void MeasurementDisplay::updatePressure(unsigned value)
{
  assert(value < 100);
  char buffer[3];
  snprintf(buffer, 3, "%2u", value);
  this->display.setCursor(0, 1);
  this->display.write(buffer, 2);
}