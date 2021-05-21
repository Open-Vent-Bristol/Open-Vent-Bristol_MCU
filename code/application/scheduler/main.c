// Copyright (c) 2021 <OpenVent-Bristol, Donald Robson>

#include "dispatcher.h"
#include "system/system.h"
#include "watchdog/watchdog.h"

int main(void)
{
  system_init();
  watchdog_init();

  while (1u)
  {
    dispatcher_service();
    watchdog_reset();
  }
}
