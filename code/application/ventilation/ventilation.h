#ifndef VENTILATION_H
#define VENTILATION_H

#ifdef __cplusplus
extern "C"
{
#endif

/**
 * @brief Initialise ventilation.  The actuator will home, then wait until further instructions are
 * received from the FPGA.
 */
void ventilation_init(void);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* VENTILATION_H */
