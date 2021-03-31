#ifndef GPIO_H
#define GPIO_H

#include "types/types.h"

#ifdef __cplusplus
extern "C"
{
#endif

/**
 * @brief Set a pin mask on the GPIO port
 *
 * @param port
 * @param pin_mask
 */
void gpio_set_mask(gpio_register_t port, register_size_t pin_mask);

/**
 * @brief Clear a pin mask on the GPIO port
 *
 * @param port
 * @param pin_mask
 */
void gpio_clear_mask(gpio_register_t port, register_size_t pin_mask);

/**
 * @brief Overwrite the port to a pin_mask value
 * @note This will also clear any zeroed bits in the mask.  gpio_set_mask may
 * be more appropriate.
 * @note Improper use may cause pin change interrupts.
 *
 * @param port
 * @param pin_mask
 */
void gpio_write_mask(gpio_register_t port, register_size_t pin_mask);

/**
 * @brief Check a pin mask against the value of the GPIO port
 * @note Input and output registers may differ
 *
 * @param port
 * @param pin_mask
 * @return register_size_t  A mask of all pins that are high in pin_mask.
 *                          If all pins were high, the return value will be
 *                          the same as pin_mask.
 */
register_size_t gpio_read_mask(gpio_register_t port, register_size_t pin_mask);

/**
 * @brief Set a single pin (not left shifted)
 *
 * @param port
 * @param pin_number
 */
void gpio_set_pin(gpio_register_t port, uint8_t pin_number);

/**
 * @brief Clear a single pin (not left shifted)
 *
 * @param port
 * @param pin_number
 */
void gpio_clear_pin(gpio_register_t port, uint8_t pin_number);

/**
 * @brief Read a single pin (not left shifted)
 * @note Input and output registers may differ
 *
 * @param port
 * @param pin_mask
 * @return register_size_t  1 if the pin was high, 0 if the pin was low
 */
uint8_t gpio_read_pin(gpio_register_t port, uint8_t pin_number);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* GPIO_H */
