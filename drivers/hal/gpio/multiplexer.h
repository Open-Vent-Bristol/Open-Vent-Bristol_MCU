#ifndef MULTIPLEXER_H
#define MULTIPLEXER_H

#include "board/board.h"

#ifdef __cplusplus
extern "C"
{
#endif

/**
 * @brief Set the multiplexer to connect a channel to its GPIO pin.
 * @note This may cause a pin change interrupt.
 *
 * @param channel     See multiplexer_select_t
 */
void multiplexer_select_channel(multiplexer_select_t channel);

/**
 * @brief Read the currently selected channel.
 *
 * @param channel     See multiplexer_select_t
 * @return uint8_t    1 if the pin was high, 0 if the pin was low
 */
uint8_t multiplexer_digital_read_current_channel(void);

/**
 * @brief Set the multiplexer to connect a channel to its GPIO pin and
 * then read it.
 * @note The channel will remain selected afterwards.
 * @note This may cause a pin change interrupt.
 *
 * @param channel     See multiplexer_select_t
 * @return uint8_t    1 if the pin was high, 0 if the pin was low
 */
uint8_t multiplexer_digital_read_channel(multiplexer_select_t channel);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* MULTIPLEXER_H */
