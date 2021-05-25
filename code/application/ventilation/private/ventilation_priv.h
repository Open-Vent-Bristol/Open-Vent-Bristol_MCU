#ifndef VENTILATION_PRIV_H
#define VENTILATION_PRIV_H

#ifdef __cplusplus
extern "C"
{
#endif

/**
 * @brief Change the ventilation mode to the target mode (set using ventilation_set_target_mode).
 */
void ventilation_set_to_target_mode(void);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* VENTILATION_PRIV_H */
