#include "uart/uart.h"

void uart_ftp_run(void)
{
  if (uart_available())
  {
    char buffer[3] = {'R', '\0', '\n'};
    uart_read(&buffer[1], 1u);

    uart_write(buffer, sizeof(buffer));
  }
}
