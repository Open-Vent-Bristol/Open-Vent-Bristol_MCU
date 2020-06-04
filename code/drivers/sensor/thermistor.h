#ifndef THERMISTOR_H
#define THERMISTOR_H

#include "types/types.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Convert an analogue voltage reading to thermistor resistance for use in
 * temperature calculation
 *
 * @param adc_reading
 * @return resistance_t     Thermistor resistance in ohms (-1 if error)
 */
resistance_t thermistor_get_resistance(ADC_resolution_t adc_reading);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* THERMISTOR_H */
