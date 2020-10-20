#include "spi/spi.h"
#include "board/board.h"
#include "gpio/gpio.h"
#include <stm32l4/stm32l4xx_ll_spi.h>
#include <string.h>

typedef struct
{
  gpio_register_t chip_select_port;
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
  // Configure SPI as per the reference manual, RM0351.

  LL_SPI_Init(/*SPI1*/, &stm32l4_spi);

}