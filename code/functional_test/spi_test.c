// Copyright (c) 2021 <OpenVent-Bristol, Donald Robson>

#include "uart/uart.h"
#include "spi/spi.h"

static uint8_t s_buffer[2] = {0u, 1u};

void spi_ftp_run(void)
{
  spi_write(SPI_NO_COMMAND, s_buffer, 2u);
  s_buffer[0]++;
  s_buffer[1]++;
}
