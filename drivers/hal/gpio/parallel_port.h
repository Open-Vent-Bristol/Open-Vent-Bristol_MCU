#ifndef PARALLEL_PORT_H
#define PARALLEL_PORT_H

#include <stddef.h>
#include "types/types.h"

#ifdef __cplusplus
extern "C"
{
#endif

/**
 * @brief Write a single byte to the port
 * @note The GPIO port must be fully initialised prior to using this function
 *
 * @param value
 */
void parallel_port_write_byte(uint8_t value);

/**
 * @brief Write a series of bytes to the port
 * @note The GPIO port must be fully initialised prior to using this function
 * @note This will block execution until all bytes are sent
 *
 * @param values
 * @param length
 */
void parallel_port_write(const uint8_t* const values, size_t length);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* PARALLEL_PORT_H */
