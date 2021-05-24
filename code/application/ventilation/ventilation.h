#ifndef VENTILATION_H
#define VENTILATION_H

#ifdef __cplusplus
extern "C"
{
#endif

typedef enum
{
  VENTILATION_MODE_CONTROL, // PCV - triggers on timeout according to the I-E ratio and resp. rate
  VENTILATION_MODE_SUPPORT  // PSV - triggers on pressure drop and ends at the lower flow threshold
} ventilation_mode_t;

/**
 * @brief Initialise ventilation.  The actuator will home, then wait until further instructions are
 * received from the FPGA.
 */
void ventilation_init(void);

/**
 * @brief Change the ventilation mode
 *
 * @param mode  See ventilation_mode_t
 */
void ventilation_set_mode(ventilation_mode_t mode);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* VENTILATION_H */
