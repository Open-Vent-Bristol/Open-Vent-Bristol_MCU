#include "spi/spi.h"
#include "board/board.h"
#include "gpio/gpio.h"
#include <avr/interrupt.h>

typedef struct
{
  MCU_register_t chip_select_port;
  register_size_t chip_select_pin;
  bool available;
} spi_t;

static volatile spi_t spi =
{
  .chip_select_port = NULL,
  .chip_select_pin = 0u,
  .available = true
};

void spi_init(void)
{
  // Interrupt, enable, LSB first, master, idle clock high, speed is fcpu/128
  SPCR = (1u << SPIE) | (1u << SPE) | (1u << DORD) |
         (1u << MSTR) | (1u << CPOL) |
         (1u << SPR1) | (1u << SPR0);
}

bool spi_available(void)
{
  return spi.available;
}

bool spi_setup_transaction(MCU_register_t chip_select_port, register_size_t chip_select_pin)
{
  if (spi.available)
  {
    spi.chip_select_port = chip_select_port;
    spi.chip_select_pin = chip_select_pin;
  }

  return spi.available;
}

bool spi_command(uint8_t command)
{
  bool return_val = false;

  if (spi.available)
  {
    spi.available = false;

    // Pull chip select low
    gpio_clear_pin(spi.chip_select_port, spi.chip_select_pin);

    // Put the command into the data register
    SPDR = command;

    return_val = true;
  }

  return return_val;
}

// bool spi_write(uint8_t command, const uint8_t* const data_out, uint8_t length)
// {

// }

// bool spi_read(uint8_t command, uint8_t* const data_in, uint8_t max_length)
// {

// }

ISR(SPI_STC_vect)
{
  // Transaction complete - pull chip select high and mark the device as available
  gpio_set_pin(spi.chip_select_port, spi.chip_select_pin);
  spi.available = true;
}
