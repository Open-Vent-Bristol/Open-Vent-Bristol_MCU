// Copyright (c) 2021 <OpenVent-Bristol, Alex Luisi>

#include "spi/spi.h"
#include "board/board.h"
#include "gpio/gpio.h"
#include <stm32l4/stm32l4xx_ll_spi.h>
#include <string.h>

#define SPI SPI1
#define SPI_BUFFER_SIZE 255u

static LL_SPI_InitTypeDef stm32l4_spi =
{
  .TransferDirection = LL_SPI_FULL_DUPLEX,
  .Mode = LL_SPI_MODE_MASTER,
  .DataWidth = LL_SPI_DATAWIDTH_8BIT,
  .ClockPolarity = LL_SPI_POLARITY_HIGH,
  .ClockPhase = LL_SPI_PHASE_1EDGE,
  .NSS = LL_SPI_NSS_HARD_INPUT,
  .BaudRate = 115200, // placeholder
  .BitOrder = LL_SPI_LSB_FIRST,
  .CRCCalculation = LL_SPI_CRCCALCULATION_DISABLE,
  .CRCPoly = 0u
};

void spi_init(void)
{
  // Configure SPI
  LL_SPI_Init(SPI, &stm32l4_spi);
  LL_SPI_ENABLE(SPI);
  spi.available = true;

}


bool spi_command(uint8_t command)
{
  bool return_val = false;

  return_val = spi_write(command, NULL, 0u);
  return return_val;
}

bool spi_write(uint8_t command, const uint8_t* const data_out, uint8_t length)
{
  // DMA eventually

  bool return_value = false;

  // Check for null data before sending it
  if(data_out != NULL)
  {
  // Check if the SPI is busy
    if(!(LL_SPI_IsActiveFlag_BSY(SPI)))
    {
      // tx the command
      LL_SPI_TransmitData8(SPI, command);

      // Check if there is any payload to tx
      if(length > 0u)
      {
        // Tx the payload
        for(uint8_t i = 0u; i < length; i++)
        {
          LL_SPI_TransmitData8(SPI, data_out[i]);
        }

        // check if the buffer is now empty
        if(LL_SPI_IsActiveFlag_TXE(SPI))
        {
          // successful tx
          return_value = true;

        }

      }

    }

  }

  return return_value;

}

bool spi_read(uint8_t command, uint8_t* const data_in, uint8_t max_length)
{
    // DMA eventually

  bool return_value = false;

  if(data_in != NULL)
  {
    // Check if the SPI is busy
    if(!(LL_SPI_IsActiveFlag_BSY(SPI)))
    {
      // tx the command
      LL_SPI_TransmitData8(SPI, command);
      // get the first byte back
      // (may have to wait for RX Buffer to be full)
      data_in[0] = LL_SPI_ReceiveData8(SPI);

      // read the subsequent expected number of bytes back
      for(uint8_t i = 1u; i < max_length; i++)
      {
        // tx the dummy command
        LL_SPI_TransmitData8(SPI, SPI_NO_COMMAND);
        // (may have to wait for RX Buffer to be full)
        data_in[i] = LL_SPI_ReceiveData8(SPI);
      }

      return_value = true;

    }

  }

  return return_value;

}