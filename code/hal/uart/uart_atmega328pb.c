#include "uart/uart.h"
#include "uart/uart_priv.h"
#include "board/board.h"
#include <avr/interrupt.h>

void uart_configure(void)
{
  UART_BAUD_SET();
  UART_CFG();
}

void uart_start_transmission(void)
{
  // Wait for the UDR0 register to become empty
  while (UCSR0A & (1u << UDRE0) == 0u) {};

  UDR0 = tx_buffer.data[tx_buffer.read_position];
  tx_buffer.length--;
}

ISR(USART_RX_vect)
{
  rx_buffer.data[rx_buffer.write_position] = UDR0;
  rx_buffer.length++;
}

ISR(USART_TX_vect)
{
  UDR0 = tx_buffer.data[tx_buffer.read_position];
  tx_buffer.length--;
}
