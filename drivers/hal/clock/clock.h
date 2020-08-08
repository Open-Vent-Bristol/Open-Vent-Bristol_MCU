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

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* CLOCK_H */
