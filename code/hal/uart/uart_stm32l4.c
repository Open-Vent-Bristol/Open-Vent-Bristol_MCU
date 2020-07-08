#include "uart/uart.h"
#include "uart/uart_priv.h"
#include "board/board.h"

void uart_configure(void)
{
  LL_RCC_SetUSARTClockSource(LL_RCC_USART1_CLKSOURCE_SYSCLK);

  // Enable clocks
  LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_USART1);

  LL_USART_DeInit(USART1);

  LL_USART_ClockInitTypeDef usart_clock_init_struct;
  LL_USART_ClockStructInit(&usart_clock_init_struct);
  LL_USART_ClockInit(&usart_clock_init_struct);

  LL_USART_InitTypeDef usart_init_struct;
  LL_USART_StructInit(&usart_init_struct);
  usart_init_struct.BaudRate = UART_BAUD_RATE;
  LL_USART_Init(USART1, &usart_init_struct);
}

void uart_start_transmission(void)
{
  // Enable empty interrupt
  LL_USART_EnableIT_TXE(USART1);
}

void USART1_IRQHandler(void)
{
  // Receive not empty
  if (LL_USART_IsActiveFlag_RXNE(USART1))
  {
    rx_buffer.data[rx_buffer.write_position] = LL_USART_ReceiveData8(USART1);
    rx_buffer.write_position++;
    rx_buffer.write_position &= (UART_BUFFER_SIZE - 1u);
    rx_buffer.length++;
  }

  // Transmit empty
  if (LL_USART_IsActiveFlag_TXE(USART1))
  {
    if (tx_buffer.length != 0u)
    {
      LL_USART_TransmitData8(USART1, tx_buffer.data[tx_buffer.read_position]);
      tx_buffer.read_position++;
      tx_buffer.read_position &= (UART_BUFFER_SIZE - 1u);
      tx_buffer.length--;
    }
    else
    {
      // No more data - disable the empty interrupt
      LL_USART_DisableIT_TXE(USART1);
    }
  }
}
