#ifndef CLOCK_H
#define CLOCK_H

#ifdef __cplusplus
extern "C"
{
#endif

/**
 * @brief Bring up all required MCU clocks and setup timers
 *
 */
void clock_init(void);

/**
 * @brief Start the scheduler clock
 *
 * @param callback The function pointer to call
 */
void clock_scheduler_start(void);

/**
 * @brief Stop the scheduler clock
 *
 * @param callback The function pointer to call
 */
void clock_scheduler_stop(void);

/**
 * @brief Set a callback to be executed every millisecond
 *
 * @param callback The function pointer to call
 */
void clock_set_scheduler_ms_callback(void(*callback)(void));

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* CLOCK_H */
