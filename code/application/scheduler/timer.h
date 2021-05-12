// Copyright (c) 2021 <OpenVent-Bristol, Donald Robson>

#ifndef TIMER_H
#define TIMER_H

#include "state/system_event.h"
#include <stdint.h>

#define TIMER_START_INDEX (0x40000000)

enum timer_id
{
  TIMER_ACTUATOR_SERVICE = TIMER_START_INDEX,
  TIMER_1,
  TIMER_2,
  TIMER_3,
  TIMER_4,
  TIMER_5,

  // Define timer IDs above this line
  TIMER_END_INDEX
};

typedef enum
{
  TIMER_TYPE_CONTINUOUS,  // The timer will reload and restart automatically after each expiry
  TIMER_TYPE_ONE_SHOT     // The timer will expire only once, but can be restarted manually
} timer_type_t;

typedef struct
{
  timer_type_t type;
  int32_t initial_ticks;
  int32_t remaining_ticks;
  system_event_mask_t events_signalled;  // A mask of one or more events
  enum timer_id unique_id;  // The id is also the argument when the above event is signalled
} timer_t;

/**
 * @brief Attach a timer to the kernel.  It will signal event_signalled with the timer's unique_id
 *        as its argument when it expires.
 *
 * @param timer   Pointer to object of type timer_t
 */
void timer_attach(timer_t* timer);

/**
 * Reset one timer to its initial tick value, allowing it to restart.
 *
 * @param timer   Pointer to object of type timer_t
 */
void timer_reset(timer_t* timer);

/**
 * Stop one timer, cancelling remaining ticks.  Use timer_reset to start again
 *
 * @param timer   Pointer to object of type timer_t
 */
void timer_stop(timer_t* timer);

#endif /* TIMER_H */
