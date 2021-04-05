#ifndef DISPATCHER_H
#define DISPATCHER_H

#include "system_event.h"
#include <stddef.h>

/**
 * @brief Signal that one or more events have occurred.  The events will be handled according to
 * defined priority (lower bits first)
 *
 * @param event_mask    Mask of one or more system events
 * @param arg           An argument to pass to the event callbacks
 */
void dispatcher_signal_event_mask(enum system_event event_mask, int32_t arg);

/**
 * @brief Call the functions associated with the unhandled events.
 */
void dispatcher_service(void);

/**
 * @brief Bind specific system events to a callback function.
 *
 * @param event_mask    Mask of one or more system events
 * @param callback_fn
 */
void dispatcher_bind(enum system_event event, system_event_callback callback_fn);

/**
 * @brief Write an array of bindings over the internal dispatcher bindings.
 * @note This is useful during boot, or to easily change system behaviour at runtime.
 *
 * @param callback_fns  Pointer to the start of a system_event_callback array
 * @param number_binds  Length of the array
 */
void dispatcher_bind_all(const system_event_callback* callback_fns, size_t number_binds);

#endif /* DISPATCHER_H */
