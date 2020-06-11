#ifndef SCHEDULER_H
#define SCHEDULER_H


#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Set a callback to be executed every millisecond. Set to NULL to disable
 *
 * @param callback    The function pointer to call
 */
void scheduler_set_ms_callback(void(*callback)(void));


#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* SCHEDULER_H */