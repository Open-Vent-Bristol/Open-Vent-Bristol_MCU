#include "uart/uart.h"
#include "spi/spi.h"

uint8_t s_buffer[4] = {0u, 1u, 2u, 3u};

void spi_ftp_run(void)
{
  for (uint8_t i = 0u; i < sizeof(s_buffer); i++)
  {
    spi_command(s_buffer[i]);
    s_buffer[i]++;
  }
}
