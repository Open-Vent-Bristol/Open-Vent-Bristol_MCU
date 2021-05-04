#ifndef VENTILATION_H
#define VENTILATION_H

#include "state/system_event.h"
#include <stdint.h>

#ifdef __cplusplus
extern "C"
{
#endif

typedef enum
{
  VENTILATION_MODE_CONTROL, // PCV - triggers on timeout according to the I-E ratio and resp. rate
  VENTILATION_MODE_SUPPORT  // PSV - triggers on pressure drop and ends at the lower flow threshold
} ventilation_mode_t;

void ventilation_init(void);

void ventilation_set_mode(ventilation_mode_t mode);

void ventilation_run(enum system_event* const event_mask);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* VENTILATION_H */
