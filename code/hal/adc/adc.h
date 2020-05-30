#ifndef ADC_H
#define ADC_H

#include "types/types.h"
#include "board/board.h"
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Setup the ADC peripheral
 *
 */
void adc_init(void);

/**
 * @brief Start an ADC read.  The callback will be called with the reading once completed
 *
 * @param channel   ADC_Channel_t
 * @param callback  Function pointer of signature void(ADC_resolution_t)
 * @return true     ADC read started
 * @return false    ADC multiplexer busy, could not start read
 */
bool adc_read_interrupt(ADC_channel_t channel, ADC_callback_t callback);

/**
 * @brief Start an ADC read.  The reading will be returned once the measurement is complete, but
 * execution will be blocked until then
 *
 * @param channel           ADC_Channel_t
 * @return ADC_resolution_t The reading
 */
ADC_resolution_t adc_read_blocking(ADC_channel_t channel);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* ADC_H */
