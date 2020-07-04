#include "spi/spi.h"
#include "board/board.h"
#include "gpio/gpio.h"
#include <avr/interrupt.h>
#include <string.h>

typedef struct
{
  MCU_register_t chip_select_port;
  register_size_t chip_select_pin;
  uint8_t out_buffer[SPI_BUFFER_SIZE];
  uint8_t out_data_length;
  uint8_t out_data_counter;
  bool available;
} spi_t;

static volatile spi_t spi =
{
  .chip_select_port = NULL,
  .chip_select_pin = 0u,
  .out_buffer = {0u},
  .out_data_length = 0u,
  .out_data_counter = 0u,
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
  return_val = spi_write(command, NULL, 0u);
  return return_val;
}

bool spi_write(uint8_t command, const uint8_t* const data_out, uint8_t length)
{
  bool return_val = false;

  if (spi.available)
  {
    spi.available = false;

    // Pull chip select low
    gpio_clear_pin(spi.chip_select_port, spi.chip_select_pin);

    // Copy the data into the SPI buffer
    if (data_out != NULL)
    {
      memcpy((void*)spi.out_buffer, (void*)data_out, length);
      spi.out_data_length = length;
    }

    // Put the first byte into the data register.  This will start the transaction
    if (command != SPI_NO_COMMAND)
    {
      spi.out_data_counter = 0u;
      SPDR = command;
    }
    else if (spi.out_data_length != 0u)
    {
      // Increment out_data_counter before, because the interrupt will happen quickly
      spi.out_data_counter = 1u;
      SPDR = spi.out_buffer[0u];
    }

    return_val = true;
  }

  return return_val;
}

// TODO: does not support receive, but not required for mk1 board
// bool spi_read(uint8_t* const data_in, uint8_t max_length)
// {

// }

ISR(SPI_STC_vect)
{
  if (spi.out_data_counter < spi.out_data_length)
  {
    // Put the next byte into the data register - TODO: does not support receive, but not required
    // for mk1 board
    SPDR = spi.out_buffer[spi.out_data_counter];
    spi.out_data_counter++;
  }
  else
  {
    // Transaction complete - pull chip select high and mark the device as available.  Also need to
    // zero the out_data_length because otherwise future command only transactions would also write
    // data from out_buffer
    gpio_set_pin(spi.chip_select_port, spi.chip_select_pin);
    spi.out_data_length = 0u;
    spi.available = true;
  }
}
