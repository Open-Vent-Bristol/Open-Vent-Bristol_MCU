// Copyright (c) 2021 <OpenVent-Bristol, Donald Robson>

#ifndef ERROR_H
#define ERROR_H

#include <stdint.h>

typedef enum
{
  ERR_NONE      = 0,
  ERR_IO        = 1,
  ERR_BUSY      = 2,
  ERR_NOT_IMPL  = 3,
} error_t;

#endif /* ERROR_H */
