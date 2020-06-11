#include "clock/clock.h"
#include "board/board.h"

#include <avr/interrupt.h>

static void(*scheduler_callback)(void) = 0;

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

void clock_scheduler_start(void)
{
  SCHEDULER_START();
}

void clock_scheduler_stop(void)
{
  SCHEDULER_STOP();
}

void clock_set_scheduler_ms_callback(void(*callback)(void))
{
  scheduler_callback = callback;
}

ISR(SCHEDULER_INT_ISR)
{
  if(scheduler_callback)
  {
    (*scheduler_callback)();
  }
}
