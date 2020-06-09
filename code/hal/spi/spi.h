#ifndef SPI_H
#define SPI_H

#include <stddef.h>
#include "types/types.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Setup the SPI peripheral and buffers
 *
 */
void spi_init(void);

/**
 * @brief Check that the SPI device is ready to start a new transaction
 *
 * @return true       SPI device is available
 * @return false      SPI device is unavailable
 */
bool spi_available(void);

/**
 * @brief Prepare to start a new transaction.
 * If the SPI device is busy, the transaction will not be started
 *
 * @param chip_select_port  GPIO port of the chip select pin
 * @param chip_select_pin   Chip select pin number (not left shifted)
 * @return true             SPI device is available
 * @return false            SPI device is unavailable
 */
bool spi_setup_transaction(MCU_register_t chip_select_port, register_size_t chip_select_pin);

/**
 * @brief Start a command only SPI transaction.
 * If the SPI device is busy, the transaction will not be started
 *
 * @param command     Command to send
 * @return true       Transaction started
 * @return false      SPI device is unavailable
 */
bool spi_command(uint8_t command);

/**
 * @brief Start a SPI write transaction.
 * If the SPI device is busy, the transaction will not be started
 * @note Not implemented!
 *
 * @param command     Command to send (0xFF to skip command phase)
 * @param data_out    Data to write
 * @param length      Length of write (normally sizeof(data_out))
 * @return true       Transaction started
 * @return false      SPI device is unavailable
 */
// bool spi_write(uint8_t command, const uint8_t* const data_out, uint8_t length);

/**
 * @brief Start a SPI read transaction.
 * If the SPI device is busy, the transaction will not be started
 * @note Not implemented!
 *
 * @param command     Command to send
 * @param data_in     Data to write
 * @param max_length  Length of write (normally sizeof(data_in))
 * @return true       Transaction started
 * @return false      SPI device is unavailable
 */
// bool spi_read(uint8_t command, uint8_t* const data_in, uint8_t max_length);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif SPI_H
