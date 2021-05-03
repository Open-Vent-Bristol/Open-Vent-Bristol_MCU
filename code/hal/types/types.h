// Copyright (c) 2021 <OpenVent-Bristol, Donald Robson>

#ifndef TYPES_H
#define TYPES_H

#if defined(UTEST_MODE)
#include "types_unittest.h"
#elif defined(BOARD_V3)
#include "types/types_arm.h"
#endif

#endif /* TYPES_H */
