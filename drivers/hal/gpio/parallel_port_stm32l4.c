#include "gpio/parallel_port.h"
#include "board/board.h"

// This function uses the bit-set-reset register, so we can write to the port
// without affecting the other outputs on the port.  If implementing for another
// chip it is important to check that this feature is available.  If not, it will
// be necessary to ensure the other pins are not outputs, or do a read-modify-write
// with interrupts disabled.
// The 0xFF0000 will cause bits 0-7 to be reset unless they are set in value.

void parallel_port_write_byte(uint8_t value)
{
  unsigned bitsets = (unsigned)value & 0x000000ff;
  unsigned bitclears = ((unsigned)(~value) << 16) & 0x00ff0000;

  LL_GPIO_SetOutputPin(PARALLEL_PORT, (bitsets | bitclears));
}

void parallel_port_write(const uint8_t* const values, size_t length)
{
  for (size_t i = 0u; i < length; i++)
  {
    unsigned bitsets = (unsigned)(values[i]) & 0x000000ff;
    unsigned bitclears = ((unsigned)(~values[i]) << 16) & 0x00ff0000;

    LL_GPIO_SetOutputPin(PARALLEL_PORT, (bitsets | bitclears));
  }
}
