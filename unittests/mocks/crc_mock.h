// Copyright (c) 2021 <OpenVent-Bristol, Donald Robson>

#ifndef CRC_MOCK_H
#define CRC_MOCK_H

#include "fff.h"
#include "crc/crc.h"

#define FFF_GCC_FUNCTION_ATTRIBUTES

FAKE_VOID_FUNC(crc_init);

FAKE_VALUE_FUNC(uint32_t, crc_calculate, const void* const, size_t);

#define CRC_MOCKS(x) \
  x(crc_init) \
  x(crc_calculate)

#endif /* CRC_MOCK_H */
