// Copyright (c) 2021 <OpenVent-Bristol, Donald Robson>

#include <stdint.h>
#include "board/board.h"
#include "clock/clock.h"
#include "pinmux/pinmux.h"

int main(void)
{
  clock_init();
  pinmux_init();

  while(1u)
  {
    LL_GPIO_TogglePin(BATTERY_LED_PORT, (1u << BATTERY_LED_PIN));

    for (volatile uint32_t i = 0u; i < 100u; i++);
  }
}
