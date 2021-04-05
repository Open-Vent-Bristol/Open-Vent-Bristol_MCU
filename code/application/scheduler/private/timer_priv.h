// Copyright (c) 2021 <OpenVent-Bristol, Donald Robson>

#ifndef TIMER_PRIV_H
#define TIMER_PRIV_H

#include "scheduler/system_event.h"

/**
 * @brief Increment all attached timers and allow them to expire
 */
void timer_tick_all(void);

#endif /* TIMER_PRIV_H */
