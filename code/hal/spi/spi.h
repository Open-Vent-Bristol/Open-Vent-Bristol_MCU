// Copyright (c) 2021 <OpenVent-Bristol, Alex Luisi>

#ifndef SPI_H
#define SPI_H

#include <stddef.h>
#include <stdbool.h>
#include "types/types.h"

#define SPI_NO_COMMAND    (0xFF)

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Setup the SPI peripheral and buffers
 *
 */
void spi_init(void);

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
 * @param command     Command to send (SPI_NO_COMMAND to skip command phase)
 * @param data_out    Data to write
 * @param length      Length of write (normally sizeof(data_out))
 * @return true       Transaction started
 * @return false      SPI device is unavailable
 */
bool spi_write(uint8_t command, const uint8_t* const data_out, uint8_t length);

/**
 * @brief Start a SPI read transaction.
 * If the SPI device is busy, the transaction will not be started
 * @note Not implemented!
 *
 * @param command     Command to send
 * @param data_in     Data to read
 * @param max_length  Length of read (normally sizeof(data_in))
 * @return true       Transaction started
 * @return false      SPI device is unavailable
 */
bool spi_read(uint8_t command, uint8_t* const data_in, uint8_t max_length);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* SPI_H */
