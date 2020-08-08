#include "uart/uart.h"
#include "uart/uart_priv.h"
#include "board/board.h"

void uart_configure(void)
{
  LL_RCC_SetUSARTClockSource(UART_CLOCK_SOURCE);

  // Enable clocks
  UART_CLOCK_ENABLE();

  LL_USART_DeInit(UART_PERIPH);

  LL_USART_ClockInitTypeDef usart_clock_init_struct;
  LL_USART_ClockStructInit(&usart_clock_init_struct);
  LL_USART_ClockInit(UART_PERIPH, &usart_clock_init_struct);

  LL_USART_InitTypeDef usart_init_struct;
  LL_USART_StructInit(&usart_init_struct);
  usart_init_struct.BaudRate = UART_BAUD_RATE;
  LL_USART_Init(UART_PERIPH, &usart_init_struct);

  NVIC_SetPriority(UART_IRQ, 0);
  NVIC_EnableIRQ(UART_IRQ);

  LL_USART_Enable(UART_PERIPH);

  // Enable receive interrupt
  LL_USART_EnableIT_RXNE(UART_PERIPH);
}

void uart_start_transmission(void)
{
  // Enable empty interrupt
  LL_USART_EnableIT_TXE(UART_PERIPH);
}

UART_IRQ_HANDLER()
{
  // Receive not empty
  if (LL_USART_IsActiveFlag_RXNE(UART_PERIPH))
  {
    rx_buffer.data[rx_buffer.write_position] = LL_USART_ReceiveData8(UART_PERIPH);
    rx_buffer.write_position++;
    rx_buffer.write_position &= (UART_BUFFER_SIZE - 1u);
    rx_buffer.length++;
  }

  // Transmit empty
  if (LL_USART_IsActiveFlag_TXE(UART_PERIPH))
  {
    if (tx_buffer.length != 0u)
    {
      LL_USART_TransmitData8(UART_PERIPH, tx_buffer.data[tx_buffer.read_position]);
      tx_buffer.read_position++;
      tx_buffer.read_position &= (UART_BUFFER_SIZE - 1u);
      tx_buffer.length--;
    }
    else
    {
      // No more data - disable the empty interrupt
      LL_USART_DisableIT_TXE(UART_PERIPH);
    }
  }
}
