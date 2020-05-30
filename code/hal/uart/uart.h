#ifndef UART_H
#define UART_H

#include <stdint.h>

#ifdef __cplusplus
extern "C"
{
#endif

/**
 * @brief Setup the UART peripheral and buffers
 *
 */
void uart_init(void);

/**
 * @brief Read the number of bytes waiting in the UART RX buffer
 *
 * @return uint8_t    Number of bytes available
 */
uint8_t uart_available(void);

/**
 * @brief Check that the UART TX buffer has sufficient space for a write
 *
 * @param length      Number of bytes to write
 * @return uint8_t    0 or length
 */
uint8_t uart_capacity(uint8_t length);

/**
 * @brief Read bytes from the UART RX buffer.  If the buffer contains insufficient data, less data will be returned
 *
 * @param data_in     Pointer to a receiving object.  Size of object >= max_length
 * @param max_length  Maximum number of bytes to read.  Normally sizeof(data_in)
 * @return uint8_t    Number of bytes read
 */
uint8_t uart_read(uint8_t* const data_in, uint8_t max_length);

/**
 * @brief Read bytes from the UART RX buffer without removing them from the buffer.  If the buffer contains insufficient
 * data, less data will be returned
 *
 * @param data_in     Pointer to a receiving object.  Size of object >= max_length
 * @param max_length  Maximum number of bytes to read.  Normally sizeof(data_in)
 * @return uint8_t    Number of bytes read
 */
uint8_t uart_peek(uint8_t* const data_in, uint8_t max_length);

/**
 * @brief Write bytes to the UART TX buffer.  If the buffer has insufficient space, no bytes will be written
 *
 * @param data_in     Pointer to a receiving object.  Size of object >= max_length
 * @param length      Number of bytes to write.  Normally sizeof(data_out)
 * @return uint8_t    Number of bytes written (length or 0)
 */
uint8_t uart_write(const uint8_t* const data_out, uint8_t length);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* UART_H */
