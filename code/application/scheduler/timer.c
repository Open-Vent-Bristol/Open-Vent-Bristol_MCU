// Copyright (c) 2021 <OpenVent-Bristol, Donald Robson>

#include "timer.h"
#include "private/timer_priv.h"
#include "dispatcher.h"
#include "misc/util.h"
#include <stddef.h>

TESTABLE timer_t* s_timers[TIMER_COUNT] = {NULL};

void timer_attach(timer_t* timer)
{
  if (timer)
  {
    size_t offset = timer->unique_id - TIMER_START_INDEX;

    if (offset < TIMER_COUNT)
    {
      s_timers[offset] = timer;
    }
  }
}

void timer_reset(timer_t* timer)
{
  if (timer)
  {
    timer->remaining_ticks = timer->initial_ticks;
  }
}

void timer_stop(timer_t* timer)
{
  if (timer)
  {
    timer->remaining_ticks = -1;
  }
}

void timer_tick_all(void)
{
  for (size_t i = 0u; i < TIMER_COUNT; i++)
  {
    if (s_timers[i])
    {
      if (s_timers[i]->remaining_ticks > 0)
      {
        s_timers[i]->remaining_ticks--;

        if (s_timers[i]->remaining_ticks == 0)
        {
          dispatcher_signal_event_mask(s_timers[i]->events_signalled, s_timers[i]->unique_id);

          if (s_timers[i]->type == TIMER_TYPE_CONTINUOUS)
          {
            s_timers[i]->remaining_ticks = s_timers[i]->initial_ticks;
          }
          else
          {
            s_timers[i]->remaining_ticks = -1;
          }
        }
      }
    }
  }
}
