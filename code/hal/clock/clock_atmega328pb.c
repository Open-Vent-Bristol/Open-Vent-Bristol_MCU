#include "clock/clock.h"
#include "board/board.h"

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
  PRR0 |= (1u << PRTWI0) | (1u << PRUSART1); // Disable I2C0 and USART1 interfaces
  PRR1 |= (1u << PRTIM3)                     // Disable timer 3
          | (1u << PRSPI1)                   // Disable SPI1 interface
          | (1u << PRTIM4)                   // Disable timer 4
          | (1u << PRPTC)                    // Disable touch interface
          | (1u << PRTWI1);                  // Disable I2C1 interface

  SCHEDULER_TIM_CFG();
  MOTOR_PWM_TIM_CFG();
}
