// Copyright (c) 2021 <OpenVent-Bristol, Donald Robson>

#ifndef TIMER_PRIV_H
#define TIMER_PRIV_H

#include "state/system_event.h"

#define TIMER_COUNT (TIMER_END_INDEX - TIMER_START_INDEX)

/**
 * @brief Increment all attached timers and allow them to expire
 */
void timer_tick_all(void);

#endif /* TIMER_PRIV_H */
