#include "drivers/display.hpp"
#include <cassert>

unsigned Display::cursorToAddress(unsigned column, unsigned row)
{
  assert(column < DISPLAY_ADDRESSABLE_COLS);
  assert(row < DISPLAY_ROWS);
  return (row * DISPLAY_ROW_ADDRESS_INCREMENT) + column;
}