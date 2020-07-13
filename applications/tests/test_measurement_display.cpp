#include "measurement_display.hpp"

#include "mock/display_mock.hpp"
#include "gtest/gtest.h"
#include "gmock/gmock.h"

using testing::Args;
using testing::ElementsAreArray;
using testing::Return;
using testing::_;


TEST(TestMeasurementDisplay, TestupdateTidalVolume) {
  DisplayMock display(DisplayIndex::DISPLAY_0);
  MeasurementDisplay measurementDisplay(display);

  const char expectedData[3] = {' ', '4', '5'};
  EXPECT_CALL(display, setCursor(0, 0));
  EXPECT_CALL(display, write(_, 3)).With(Args<0,1>(ElementsAreArray(expectedData)));
  measurementDisplay.updateTidalVolume(45);
}