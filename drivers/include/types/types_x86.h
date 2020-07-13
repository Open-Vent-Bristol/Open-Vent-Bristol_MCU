#ifndef TYPES_AVR_H
#define TYPES_AVR_H

#define __STDC_FORMAT_MACROS
#include <inttypes.h>
#include <limits.h>

typedef volatile unsigned* MCU_register_t;
typedef unsigned register_size_t;
#define REGISTER_SIZE_MAX UINT_MAX

typedef int16_t ADC_resolution_t;
typedef void (*ADC_callback_t)(ADC_resolution_t);
#define SCN_ADC_RESOLUTION SCNi16

typedef int16_t resistance_t;

#endif /* TYPES_AVR_H */
