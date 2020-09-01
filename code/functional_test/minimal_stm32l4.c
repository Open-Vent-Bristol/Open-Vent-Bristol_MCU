#include <stdint.h>
#include "board/board.h"
#include "clock/clock.h"
#include "gpio/gpio.h"

int main(void)
{
  clock_init();
  gpio_init();

  while(1u)
  {
    // TODO - this pin (PB5) is used by the LCD display - temporarily using for NUCLEO-476RG user LED
    LL_GPIO_TogglePin(LCD_PORT, (1u << LCD_D6_PIN));

    for (volatile uint32_t i = 0u; i < 100u; i++);
  }
}
