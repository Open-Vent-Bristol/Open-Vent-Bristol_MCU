#ifndef ERROR_H
#define ERROR_H

#include <stdint.h>

typedef enum
{
  ERR_NONE      = 0,
  ERR_IO        = 1,
  ERR_BUSY      = 2,
  ERR_NOT_IMPL  = 3,
} Error_t;

#endif /* ERROR_H */
