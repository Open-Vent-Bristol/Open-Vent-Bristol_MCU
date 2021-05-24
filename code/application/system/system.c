// Copyright (c) 2021 <OpenVent-Bristol, Donald Robson>

#include "system.h"
#include "actuator/actuator.h"
#include "adc/adc.h"
#include "alarm/alarm.h"
#include "clock/clock.h"
#include "cooling/fan.h"
#include "crc/crc.h"
#include "pinmux/pinmux.h"
#include "spi/spi.h"
#include "uart/uart.h"
#include "ventilation/ventilation.h"
#include "fpga/messages.h"
#include "hardware_event/interrupt_handlers.h"

void system_init(void)
{
  // Initialise HAL components (not watchdog!)
  clock_init();
  pinmux_init();
  crc_init();
  adc_init();
  spi_init();
  uart_init();

  // Initialise driver components
  actuator_init(15.0, 4.0, 0.05);
  alarm_init();
  fan_init();
  message_init();
  interrupt_init();

  // Initialise app components
  ventilation_init();
}
