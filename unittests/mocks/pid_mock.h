// Copyright (c) 2021 <OpenVent-Bristol, Donald Robson>

#ifndef PID_MOCK_H
#define PID_MOCK_H

#include "actuator/private/pid.h"
#include "fff.h"

#define FFF_GCC_FUNCTION_ATTRIBUTES

FAKE_VALUE_FUNC(int16_t, pid_update, pid_data_t* const, uint32_t, int16_t);

#endif /* PID_MOCK_H */
