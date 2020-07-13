#ifndef DRIVERS_DISPLAY_H
#define DRIVERS_DISPLAY_H

#include "types/types.h"
#include <cstdint>

const unsigned DISPLAY_ROWS = 2;
const unsigned DISPLAY_COLS = 16;
const unsigned DISPLAY_ADDRESSABLE_COLS = 40;
const unsigned DISPLAY_ROW_ADDRESS_INCREMENT = 0x40;

enum class DisplayIndex
{
  DISPLAY_0 = 0,
  DISPLAY_1 = 1
};

class Display
{
public:
  Display(Display const&) = delete;
  Display& operator=(Display const&) = delete;
  TESTABLE_DESTRUCTOR(Display);
  TESTABLE_PREFIX void setCursor(unsigned column, unsigned row) TESTABLE_SUFFIX;
  TESTABLE_PREFIX void write(const char* data, unsigned len) TESTABLE_SUFFIX;
  static unsigned cursorToAddress(unsigned column, unsigned row);
protected:
  Display(DisplayIndex idx): idx(idx) {};
  TESTABLE_PREFIX void initialize() TESTABLE_SUFFIX;
  DisplayIndex idx;
  friend class System;
};

#endif /* DRIVERS_DISPLAY_H */