#ifndef RESULT_H
#define RESULT_H

#include "error.h"
#include <stdint.h>

typedef struct
{
  Error_t err;
  union value
  {
    uint32_t u32_val;
    int32_t s32_val;
  } val;
} Result_t;

#endif /* RESULT_H */
