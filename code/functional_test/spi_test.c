#include "uart/uart.h"
#include "spi/spi.h"

static uint8_t s_next = 0u;

void spi_ftp_run(void)
{
  spi_command(s_next);
  s_next++;
}
