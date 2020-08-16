#include "uart/uart.h"
#include "uart/uart_priv.h"
#include "board/board.h"
#include <stddef.h>
#include <string.h>

circular_buffer_t rx_buffer;
circular_buffer_t tx_buffer;

void uart_init(void)
{
  memset(&rx_buffer, 0u, sizeof(rx_buffer));
  memset(&tx_buffer, 0u, sizeof(tx_buffer));

  // Setup MCU UART peripheral
  uart_configure();

  uart_write("Hello!\r\n", 8u);
}

uint8_t uart_available(void)
{
  return rx_buffer.length;
}

uint8_t uart_capacity(uint8_t length)
{
  uint8_t capacity = UART_BUFFER_SIZE - tx_buffer.length;

  if (capacity >= length)
  {
    capacity = length;
  }
  else
  {
    capacity = 0u;
  }

  return capacity;
}

uint8_t uart_read(char* const data_in, uint8_t max_length)
{
  uint8_t read = 0u;

  uint8_t waiting = uart_available();
  if (waiting < max_length)
  {
    max_length = waiting;
  }

  if (data_in != NULL)
  {
    for (uint8_t i = 0u; i < max_length; i++)
    {
      // Copy the data
      data_in[i] = rx_buffer.data[rx_buffer.read_position];
      rx_buffer.read_position++;
      rx_buffer.read_position &= (UART_BUFFER_SIZE - 1u);
      rx_buffer.length--;

      read++;
    }
  }

  return read;
}

uint8_t uart_peek(char* const data_in, uint8_t max_length)
{
  uint8_t read = 0u;

  uint8_t waiting = uart_available();
  if (waiting < max_length)
  {
    max_length = waiting;
  }

  if (data_in != NULL)
  {
    uint8_t read_position = rx_buffer.read_position;

    for (uint8_t i = 0u; i < max_length; i++)
    {
      // Copy the data
      data_in[i] = rx_buffer.data[read_position];
      read_position++;
      read_position &= (UART_BUFFER_SIZE - 1u);

      read++;
    }
  }

  return read;
}

uint8_t uart_write(const char* const data_out, uint8_t length)
{
  uint8_t written = 0u;

  if ((uart_capacity(length) != 0u) && (data_out != NULL))
  {
    for (uint8_t i = 0u; i < length; i++)
    {
      // Copy the data
      tx_buffer.data[tx_buffer.write_position] = data_out[i];
      tx_buffer.write_position++;
      tx_buffer.write_position &= (UART_BUFFER_SIZE - 1u);
      tx_buffer.length++;

      written++;
    }
  }

  // Kick off the transmission
  uart_start_transmission();

  return written;
}
