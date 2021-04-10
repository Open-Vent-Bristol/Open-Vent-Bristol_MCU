// Copyright (c) 2021 <OpenVent-Bristol, Donald Robson>

#ifndef EVENT_H
#define EVENT_H

#include <stdint.h>

typedef void (*system_event_callback)(int32_t);

enum system_event
{
  EV_FPGA_READY,
  EV_UI,
  EV_OP_MODE_CHANGE,
  EV_MOTOR_DISABLED,
  EV_TIDAL_VOLUME_EXCEEDED,
  EV_VBATT_CRITICAL,
  EV_DO_SOUND_ALARM,
  EV_DO_UPDATE_DISPLAY,
  EV_DO_LOG,

  /* SYSTEM TEST */
  EV_UART_EV_INJECT,

  /* Define above this line only - limit 32 events */
  EV_NUMBER_EVENTS
};

#endif /* EVENT_H */
