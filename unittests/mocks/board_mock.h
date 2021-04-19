// Copyright (c) 2021 <OpenVent-Bristol, Donald Robson>

#ifndef BOARD_MOCK_H
#define BOARD_MOCK_H

#include "board_unittest.h"
#include "fff.h"

#define FFF_GCC_FUNCTION_ATTRIBUTES

FAKE_VOID_FUNC(GLOBAL_INTERRUPTS_DISABLE);

FAKE_VOID_FUNC(GLOBAL_INTERRUPTS_ENABLE);

#endif /* BOARD_MOCK_H */
