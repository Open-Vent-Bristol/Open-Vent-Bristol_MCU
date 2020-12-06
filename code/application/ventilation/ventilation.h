#ifndef VENTILATION_H
#define VENTILATION_H

#include "state/event.h"
#include <stdint.h>

#ifdef __cplusplus
extern "C"
{
#endif

typedef enum
{
  VENTILATION_MODE_AUTO,    // Triggers on a timeout according to the I-E ratio and respiration rate
  VENTILATION_MODE_SUPPORT  // Triggers on a pressure drop and ends at the lower flow threshold
} ventilation_mode_t;

void ventilation_init(void);

void ventilation_set_mode(ventilation_mode_t mode);

void ventilation_run(event_t* const event_mask);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* VENTILATION_H */
