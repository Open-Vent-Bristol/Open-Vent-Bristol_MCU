#include "drivers/display.hpp"
#include "gpio/gpio.h"
#include "gpio/parallel_port.h"
#include "board/board.h"

static const uint8_t DISPLAY_0_EN = 0x04;
static const uint8_t DISPLAY_1_EN = 0x80;
static const uint8_t REGISTER_SELECT = 0x01;
static const uint8_t DATA_SHIFT = 3;

enum class RegisterSelect
{
  COMMAND = 0,
  DATA = 1
};

void sendMessage(DisplayIndex idx, RegisterSelect reg, uint8_t data)
{
  const uint8_t en = DisplayIndex::DISPLAY_0 == idx? DISPLAY_0_EN : DISPLAY_1_EN;
  const uint8_t sel = RegisterSelect::DATA == reg? REGISTER_SELECT : 0u;
  const uint8_t data_low = (data & 0x0F);
  const uint8_t data_high = ((data>>4) & 0x0F);
  CLOCK_DELAY(1);
  parallel_port_write_byte(sel | (data_high << DATA_SHIFT) | en);
  CLOCK_DELAY(1);
  parallel_port_write_byte(sel | (data_high << DATA_SHIFT));
  CLOCK_DELAY(1);
  parallel_port_write_byte(sel | (data_low << DATA_SHIFT) | en);
  CLOCK_DELAY(1);
  parallel_port_write_byte(sel | (data_low << DATA_SHIFT));
}

void Display::initialize()
{
  const uint8_t en = DisplayIndex::DISPLAY_0 == idx? DISPLAY_0_EN : DISPLAY_1_EN;
  // Initialisation sequence from the HD44780 datasheet
  // Wait more than 40 milliseconds after Vcc on
  CLOCK_DELAY(40*1000);
  parallel_port_write_byte(en | (0x03 << DATA_SHIFT));
  CLOCK_DELAY(1);
  parallel_port_write_byte(0x03 << DATA_SHIFT);
  // Wait more than 4.1ms
  CLOCK_DELAY(4500);
  parallel_port_write_byte(en | (0x03 << DATA_SHIFT));
  CLOCK_DELAY(1);
  parallel_port_write_byte(0x03 << DATA_SHIFT);
  // Wait more than 100us
  CLOCK_DELAY(120);
  parallel_port_write_byte(en | (0x03 << DATA_SHIFT));
  CLOCK_DELAY(1);
  parallel_port_write_byte(0x03 << DATA_SHIFT);

  // Function set for 4 bit mode
  CLOCK_DELAY(1);
  parallel_port_write_byte(en | (0x02 << DATA_SHIFT));
  CLOCK_DELAY(1);
  parallel_port_write_byte(0x02 << DATA_SHIFT);

  // We are now in 4 bit mode, so can use sendMessage from here on
  sendMessage(this->idx, RegisterSelect::COMMAND, 0x28); // Two line display and 5x8 font
  sendMessage(this->idx, RegisterSelect::COMMAND, 0x08); // Display off
  sendMessage(this->idx, RegisterSelect::COMMAND, 0x01); // Display clear
}

void Display::setCursor(unsigned column, unsigned row)
{
  // unsigned address = Display::cursorToAddress(column, row);

}

void Display::write(const char* data, unsigned len)
{

}