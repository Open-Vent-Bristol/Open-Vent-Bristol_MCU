// Copyright (c) 2021 <OpenVent-Bristol, Donald Robson>

#ifndef TYPES_UNITTEST_H
#define TYPES_UNITTEST_H

#include <stdint.h>

typedef uint32_t gpio_register_t;
typedef uint32_t register_size_t;
#define REGISTER_SIZE_MAX   (0xFFFF)

typedef int16_t ADC_resolution_t;
typedef void (*ADC_callback_t)(ADC_resolution_t);

#endif /* TYPES_UNITTEST_H */
