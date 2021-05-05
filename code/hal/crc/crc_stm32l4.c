// Copyright (c) 2021 <OpenVent-Bristol, Donald Robson>

#include "crc.h"
#include "stm32l4/stm32l4xx_ll_bus.h"
#include "stm32l4/stm32l4xx_ll_crc.h"

void crc_init(void)
{
  // Enable clocks
  LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_CRC);

  // TODO - FPGA implementation may require data reversal (TBC)
  // LL_CRC_SetInputDataReverseMode(CRC, LL_CRC_INDATA_REVERSE_NONE);
  // LL_CRC_SetOutputDataReverseMode(CRC, LL_CRC_OUTDATA_REVERSE_NONE);
}

uint32_t crc_calculate(const void* const data, size_t byte_length)
{
  uint32_t buffer = 0u;
  const uint8_t* const byte_data = (const uint8_t* const)data;
  size_t i = 0u;

  // Send four bytes at a time - cannot cast directly on ARM M4 due to mandatory 4-byte alignment
  for (; i < (byte_length / 4u); i++)
  {
    size_t offset = i * 4u;
    buffer = (uint32_t)((byte_data[offset + 3u] << 24u) |
                        (byte_data[offset + 2u] << 16u) |
                        (byte_data[offset + 1u] << 8u) |
                        byte_data[offset]);
    LL_CRC_FeedData32(CRC, buffer);
  }

  size_t odd_bytes = byte_length & 3u;

  if (odd_bytes == 3u)
  {
    size_t offset = i * 4u;
    buffer = (uint16_t)((byte_data[offset + 1u] << 8u) |
                        byte_data[offset]);
    LL_CRC_FeedData16(CRC, (uint16_t)(buffer & 0xFFFF));
    buffer = (uint8_t)byte_data[offset + 2u];
    LL_CRC_FeedData8(CRC, (uint8_t)(buffer & 0xFF));
  }
  else if (odd_bytes == 2u)
  {
    size_t offset = i * 4u;
    buffer = (uint16_t)((byte_data[offset + 1u] << 8u) |
                        byte_data[offset]);
    LL_CRC_FeedData16(CRC, (uint16_t)(buffer & 0xFFFF));
  }
  else if (odd_bytes == 1u)
  {
    size_t offset = i * 4u;
    buffer = (uint8_t)byte_data[offset];
    LL_CRC_FeedData8(CRC, (uint8_t)(buffer & 0xFF));
  }

  return(LL_CRC_ReadData32(CRC));
}
