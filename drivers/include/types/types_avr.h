#ifndef TYPES_AVR_H
#define TYPES_AVR_H

#define __STDC_FORMAT_MACROS
#include <inttypes.h>
#include <limits.h>

typedef volatile uint8_t* MCU_register_t;
typedef uint8_t register_size_t;
#define REGISTER_SIZE_MAX UINT8_MAX

typedef int16_t ADC_resolution_t;
typedef void (*ADC_callback_t)(ADC_resolution_t);
#define SCNADCResolution SCNi16

typedef int16_t resistance_t;

#endif /* TYPES_AVR_H */
