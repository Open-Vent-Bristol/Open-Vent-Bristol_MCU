#ifndef SHIFT_REGISTER_H
#define SHIFT_REGISTER_H

#include <stddef.h>
#include "board/board.h"

#ifdef __cplusplus
extern "C"
{
#endif

/**
 * @brief Setup the SPI interface for use by the shift register
 */
void shift_register_init(void);

/**
 * @brief Clears the value currently held in the shift register and latches
 */
void shift_register_clear(void);

/**
 * @brief Write to a single shift register channel - useful for writing to a single register or one
 * of several series-connected registers in one operation
 *
 * @param offset    Shift register offset
 * @param data      Byte data
 */
bool shift_register_write_byte(shift_register_offset_t offset, shift_register_size_t data);

/**
 * @brief Write an arbitrary number of bytes - useful for writing to several series-connected
 * registers in one operation
 *
 * @param data      Byte data
 * @param length    Number of bytes to write, up to SHIFT_REGISTER_SIZE_BYTES
 */
bool shift_register_write_bytes(const void* const data, size_t length);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* SHIFT_REGISTER_H */
