#include "drivers/scheduler.hpp"
#include <cassert>

void Scheduler::registerCallback(MillisecondCallable &callable)
{
  assert(this->numCallbacks < MAX_CALLBACKS);
  this->callbacks[this->numCallbacks++] = &callable;
}

void Scheduler::triggerCallbacks(void)
{
  for(unsigned i=0;i<this->numCallbacks;i++) {
    this->callbacks[i]->callMs();
  }
}

