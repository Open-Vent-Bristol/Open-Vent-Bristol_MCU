// Copyright (c) 2021 <OpenVent-Bristol, Donald Robson>

#ifndef TYPES_ARM_H
#define TYPES_ARM_H

#include "stm32l4xx.h"
#include <stdint.h>

typedef GPIO_TypeDef* gpio_register_t;
typedef uint32_t register_size_t;
#define REGISTER_SIZE_MAX   (0xFFFF)

typedef int16_t ADC_resolution_t;
typedef void (*ADC_callback_t)(ADC_resolution_t);

typedef int16_t resistance_t;

#endif /* TYPES_ARM_H */
