#ifndef DISPATCHER_H
#define DISPATCHER_H

#include "scheduler/event.h"
#include <stdint.h>

/**
 * @brief Call the function associated with the given event.
 * @note Functions cannot return, because this is a kernel function.
 * Values can be returned indirectly using kernel_signal_event.
 *
 * @param event   See event_t
 * @param arg     Argument for function call - normally passed to
 *                kernel with the event signal
 */
void dispatcher_call(event_t event, int16_t arg);

#endif /* DISPATCHER_H */
