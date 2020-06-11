#include "motor/motor.h"
#include "board/board.h"
#include "gpio/gpio.h"
#include "clock/clock.h"

void motor_init(void)
{
  // Ensure the In-A and In-B inputs are both low
  gpio_clear_mask(&MOTOR_IN_A_PORT, MOTOR_IN_A_PIN);
  gpio_clear_mask(&MOTOR_IN_B_PORT, MOTOR_IN_B_PIN);
  // Set the PWM frequency to zero (fully off)
  clock_motor_pwm_set(0u);
  // Enable the PWM timer
  clock_motor_pwm_enable();
}

void motor_set_speed(uint8_t speed)
{
  clock_motor_pwm_set(speed);
}

void motor_set_mode(motor_mode_t mode)
{
  switch(mode)
  {
    case MOTOR_OFF:
      // Set the speed to zero so we don't get a surprise when re-enabling
      clock_motor_pwm_set(0);
      gpio_clear_pin(&MOTOR_IN_A_PORT, MOTOR_IN_A_PIN);
      gpio_clear_pin(&MOTOR_IN_B_PORT, MOTOR_IN_B_PIN);
      break;
    case MOTOR_FORWARD:
      gpio_clear_pin(&MOTOR_IN_B_PORT, MOTOR_IN_B_PIN);
      gpio_set_pin(&MOTOR_IN_A_PORT, MOTOR_IN_A_PIN);
      break;
    case MOTOR_REVERSE:
      gpio_clear_pin(&MOTOR_IN_A_PORT, MOTOR_IN_A_PIN);
      gpio_set_pin(&MOTOR_IN_B_PORT, MOTOR_IN_B_PIN);
      break;
    default:
      break; // Do nothing
  }
}
