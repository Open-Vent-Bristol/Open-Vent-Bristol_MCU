#include "alarm/alarm.h"
#include "board/board.h"
#include "misc/util.h"
#include "scheduler/dispatcher.h"
#include "scheduler/timer.h"
#include <stdbool.h>

#define ALARM_INTERMITTANCE_MS    (97)

timer_t s_alarm_timer =
{
  .type = TIMER_TYPE_CONTINUOUS,
  .initial_ticks = ALARM_INTERMITTANCE_MS,
  .events_signalled = EV_ALARM_SERVICE,
  .unique_id = TIMER_ALARM_SERVICE
};

TESTABLE enum alarm_mode s_current_mode;

TESTABLE void alarm_intermittance_cb(int32_t arg)
{
  static bool alarm_is_on = false;

  (void)arg;

  if (s_current_mode == ALARM_OVERRIDE_ON)
  {
    alarm_is_on = !alarm_is_on;

    if (alarm_is_on)
    {
      BUZZ_PWM_START();
    }
    else
    {
      BUZZ_PWM_STOP();
    }
  }
}

void alarm_init(void)
{
  dispatcher_bind(1u << EV_ALARM_SERVICE, alarm_intermittance_cb);
  timer_attach(&s_alarm_timer);
}

void alarm_mode(int32_t mode)
{
  if (s_current_mode != ALARM_SYSTEM_FAILURE)
  {
    switch (mode)
    {
    case ALARM_OVERRIDE_OFF:
      s_current_mode = mode;
      timer_stop(&s_alarm_timer);
      BUZZ_PWM_STOP();
      break;

    case ALARM_OVERRIDE_ON:
      s_current_mode = mode;
      timer_reset(&s_alarm_timer);
      BUZZ_PWM_START();
      break;

    case ALARM_SYSTEM_FAILURE:
      s_current_mode = mode;
      timer_stop(&s_alarm_timer);
      BUZZ_PWM_START();
      break;
    }
  }
}
