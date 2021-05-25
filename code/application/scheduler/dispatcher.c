// Copyright (c) 2021 <OpenVent-Bristol, Donald Robson>

#include "dispatcher.h"
#include "board/board.h"
#include "misc/util.h"
#include "private/state_machine_priv.h"
#include <string.h>

// This holds the callback function pointers
TESTABLE system_event_callback s_bindings[EV_NUMBER_EVENTS] = {NULL};

// This holds the most recent argument for each signalled event
TESTABLE int32_t s_signalled_args[EV_NUMBER_EVENTS] = {0};

TESTABLE system_event_mask_t s_unhandled_events = 0u;

TESTABLE uint32_t s_dispatch_errors = 0u;

static void default_handler(system_event_mask_t event, int32_t arg)
{
  s_dispatch_errors++;

  // TODO - log the error and the number of errors
  // LOG_ERROR("dispatcher: event=%s, arg=%d, total_errors=%u",
  //   event_to_string(event), arg, s_dispatch_errors);
}

void dispatcher_signal_event_mask(system_event_mask_t event_mask, int32_t arg)
{
  GLOBAL_INTERRUPTS_DISABLE();

  // Update the arguments array
  for (size_t i = 0u; i < EV_NUMBER_EVENTS; i++)
  {
    if (event_mask < (1u << i))
    {
      // Exit early - no more events to signal
      break;
    }
    else if (event_mask & (1u << i))
    {
      s_signalled_args[i] = arg;
    }
  }

  // Record the events
  s_unhandled_events |= event_mask;

  GLOBAL_INTERRUPTS_ENABLE();
}

void dispatcher_clear_event_mask(system_event_mask_t event_mask)
{
  GLOBAL_INTERRUPTS_DISABLE();

  // Clear the events
  s_unhandled_events &= ~event_mask;

  GLOBAL_INTERRUPTS_ENABLE();
}

void dispatcher_service(void)
{
  // Run state machines first
  state_machine_run_all(&s_unhandled_events);

  // Handle events via callbacks
  for (size_t i = 0u; i < EV_NUMBER_EVENTS; i++)
  {
    if (s_unhandled_events == 0u)
    {
      // Exit early - no more events to handle
      break;
    }
    else if (s_unhandled_events & (1u << i))
    {
      if (s_bindings[i])
      {
        GLOBAL_INTERRUPTS_DISABLE();
        s_bindings[i](s_signalled_args[i]);
        GLOBAL_INTERRUPTS_ENABLE();
      }
      else
      {
        default_handler(i, s_signalled_args[i]);
        s_unhandled_events &= ~(1u << i);
      }
    }
  }

  // Catch invalid events
  if (s_unhandled_events != 0u)
  {
    for (size_t i = EV_NUMBER_EVENTS; i < 32u; i++)
    {
      if (s_unhandled_events == 0u)
      {
        // Exit early - no more events to handle
        break;
      }
      else if (s_unhandled_events & (1u << i))
      {
        default_handler(i, INT32_MIN);
        s_unhandled_events &= ~(1u << i);
      }
    }
  }
}

void dispatcher_bind(system_event_mask_t event_mask, system_event_callback callback_fn)
{
  GLOBAL_INTERRUPTS_DISABLE();

  for (size_t i = 0u; i < EV_NUMBER_EVENTS; i++)
  {
    if (event_mask < (1u << i))
    {
      // Exit early - no more events to bind
      break;
    }
    else if (event_mask & (1u << i))
    {
      s_bindings[i] = callback_fn;
    }
  }

  GLOBAL_INTERRUPTS_ENABLE();
}

void dispatcher_bind_all(const system_event_callback* callback_fns, size_t number_binds)
{
  GLOBAL_INTERRUPTS_DISABLE();

  if (number_binds < EV_NUMBER_EVENTS)
  {
    memcpy(s_bindings, callback_fns, (number_binds * sizeof(system_event_callback*)));
  }

  GLOBAL_INTERRUPTS_ENABLE();
}
