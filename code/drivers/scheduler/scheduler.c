#include "scheduler/scheduler.h"
#include "clock/clock.h"

void scheduler_set_ms_callback(void(*callback)(void))
{
  clock_scheduler_stop();
  clock_set_scheduler_ms_callback(callback);
  if(callback)
  {
    // Only start the scheduler if we have a valid callback
    clock_scheduler_start();
  }
}