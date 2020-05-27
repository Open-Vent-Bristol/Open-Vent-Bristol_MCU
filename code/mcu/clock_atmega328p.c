#include "mcu/clock.h"
#include <avr/io.h>

// NOTE: in order to use the onboard 8MHz crystal, it is necessary to program fuses.  If this is not
// performed, the internal RC oscillator is used.
// CKSEL = 0xF
// CKDIV8 = 0x0

void clock_init(void)
{
  // Set clock prescaler to 1.  This step is not required if CKDIV8 fuse has been programmed to
  // value 0x0.
  CLKPR = 1u << CLKPCE;
  CLKPR = 0u;

  // Disable clocks to unused peripherals
  PRR |= (1u << PRTWI); // Disable I2C interface
}
