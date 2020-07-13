#ifndef MOCK_DISPLAY_MOCK_H
#define MOCK_DISPLAY_MOCK_H

#include "drivers/display.hpp"
#include "gmock/gmock.h"


class DisplayMock: public Display
{
public:
  DisplayMock(DisplayIndex idx): Display(idx) {};
  DisplayMock(DisplayMock const&) = delete;
  DisplayMock& operator=(DisplayMock const&) = delete;
  MOCK_METHOD(void, setCursor, (unsigned column, unsigned row), (override));
  MOCK_METHOD(void, write, (const char* data, unsigned len), (override));
  MOCK_METHOD(void, mockInitialize, (), ());

protected:
  void initialize() override { mockInitialize(); };
};


#endif /* MOCK_DISPLAY_MOCK_H */