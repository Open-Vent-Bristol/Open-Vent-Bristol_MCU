// Copyright (c) 2021 <OpenVent-Bristol, Donald Robson>

#ifndef SPI_MOCK_H
#define SPI_MOCK_H

#include "fff.h"
#include "spi/spi.h"

#define FFF_GCC_FUNCTION_ATTRIBUTES

FAKE_VOID_FUNC(spi_init);

FAKE_VALUE_FUNC(bool, spi_command, uint8_t);

FAKE_VALUE_FUNC(bool, spi_write, uint8_t, const uint8_t* const, uint8_t);

FAKE_VALUE_FUNC(bool, spi_read, uint8_t, uint8_t* const, uint8_t);

#define SPI_MOCKS(x) \
  x(spi_init) \
  x(spi_command) \
  x(spi_write) \
  x(spi_read)

#endif /* SPI_MOCK_H */
