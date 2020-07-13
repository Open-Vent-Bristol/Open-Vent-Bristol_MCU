#include "drivers/display.hpp"
#include <cstdio>

void Display::initialize()
{
  printf("display init %u\n", static_cast<unsigned>(this->idx));
}


void Display::setCursor(unsigned column, unsigned row)
{
  printf("display setcursor %u %u %u\n", static_cast<unsigned>(this->idx), column, row);
}

void Display::write(const char* data, unsigned len)
{
  char buffer[256+1] = ""; // Extra +1 is for null terminator
  for(unsigned i=0; (i<len) && (i*2<256); i++) {
    sprintf(&buffer[i*2], "%02x", data[i]);
  }
  printf("display write %u %s\n", static_cast<unsigned>(this->idx), buffer);
}