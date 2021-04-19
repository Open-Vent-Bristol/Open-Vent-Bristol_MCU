// Copyright (c) 2021 <OpenVent-Bristol, Donald Robson>

#ifndef WATCHDOG_H
#define WATCHDOG_H

#ifdef __cplusplus
extern "C"
{
#endif

/**
 * @brief Setup the watchdog timer peripheral and start the timer running
 *
 */
void watchdog_init(void);

/**
 * @brief Reset the watchdog timer
 * @note This must be called at least every 0.5 seconds after watchdog_init or the MCU will reset,
 * including during low power mode for some microcontrollers
 * @note This function must *not* be called from an interrupt service request
 *
 */
void watchdog_reset(void);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* WATCHDOG_H */
