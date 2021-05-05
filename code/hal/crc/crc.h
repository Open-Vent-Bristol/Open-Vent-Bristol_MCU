// Copyright (c) 2021 <OpenVent-Bristol, Donald Robson>

#ifndef CRC_H
#define CRC_H

#include <stdint.h>
#include <stddef.h>

/**
 * @brief Initialise the hardware CRC peripheral
 */
void crc_init(void);

/**
 * @brief Calculate the CRC-32 ETHERNET for arbitrary data
 *
 * @param data
 * @param byte_length
 * @return uint32_t   Calculated CRC-32
 */
uint32_t crc_calculate(const void* const data, size_t byte_length);

#endif /* CRC_H */
