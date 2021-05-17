#ifndef FAN_H
#define FAN_H

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Initialise the cooling fan function
 * @note The fan will *not* function until this has been called.
 */
void fan_init(void);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* FAN_H */
