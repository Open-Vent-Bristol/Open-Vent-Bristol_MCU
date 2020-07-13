#ifndef DRIVERS_SCHEDULER_H
#define DRIVERS_SCHEDULER_H


#include "types/types.h"
#include <atomic>
#include <cstdint>

const unsigned MAX_CALLBACKS = 16;

class MillisecondCallable
{
public:
  virtual void callMs() = 0;
};

class Timer: public MillisecondCallable
{
public:
  virtual void callMs() { this->counter++; }
  uint32_t elapsed(void) { return this->counter.load(); }
  void reset(void) { this->counter = 0u; }
private:
  std::atomic<uint32_t> counter;
};

class Scheduler
{
public:
  Scheduler(Scheduler const&) = delete;
  Scheduler& operator=(Scheduler const&) = delete;
  void registerCallback(MillisecondCallable &callable);
  void triggerCallbacks(void);
private:
  Scheduler() = default;
  void initialize();
  MillisecondCallable* callbacks[MAX_CALLBACKS];
  unsigned numCallbacks = 0;

  friend class System;
};

#endif /* DRIVERS_SCHEDULER_H */