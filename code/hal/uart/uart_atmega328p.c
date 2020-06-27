#include "uart/uart.h"
#include "uart/uart_priv.h"
#include "board/board.h"
#include <avr/interrupt.h>

void uart_configure(void)
{
  // Set baud rate
  UBRR0L = 8u;
  UBRR0H = 0u;
  // UBRR0L = UART_BAUD_SET & 0xFF;
  // UBRR0H = UART_BAUD_SET >> 8u;

  UCSR0A &= ~(1u << U2X0);
  UCSR0B = (1u << RXCIE0) | (1u << RXEN0) | (1u << TXEN0);
  UCSR0C = (1u << UCSZ00) | (1u << UCSZ01);
}

void uart_start_transmission(void)
{
  // Enable empty interrupt
  UCSR0B |= (1u << UDRIE0);
}

ISR(USART_RX_vect)
{
  rx_buffer.data[rx_buffer.write_position] = UDR0;
  rx_buffer.write_position++;
  rx_buffer.write_position &= (UART_BUFFER_SIZE - 1u);
  rx_buffer.length++;
}

ISR(USART_UDRE_vect)
{
  if (tx_buffer.length != 0u)
  {
    UDR0 = tx_buffer.data[tx_buffer.read_position];
    tx_buffer.read_position++;
    tx_buffer.read_position &= (UART_BUFFER_SIZE - 1u);
    tx_buffer.length--;
  }
  else
  {
    // No more data - disable the empty interrupt
    UCSR0B &= ~(1u << UDRIE0);
  }
}
