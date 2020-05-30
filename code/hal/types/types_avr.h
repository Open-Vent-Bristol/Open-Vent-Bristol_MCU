#ifndef TYPES_AVR_H
#define TYPES_AVR_H

#include <stdint.h>

typedef volatile uint8_t* MCU_register_t;
typedef uint8_t register_size_t;

typedef int16_t ADC_resolution_t;
typedef void (*ADC_callback_t)(ADC_resolution_t);

#endif /* TYPES_AVR_H */
