// Copyright (c) 2021 <OpenVent-Bristol, Donald Robson>

#include "board/board.h"
#include "fan.h"
#include "scheduler/dispatcher.h"
#include "scheduler/timer.h"
#include "sensor/sensor.h"
#include "misc/util.h"
#include <stddef.h>
#include <stdint.h>

#define TEN_SECONDS             (10000)

TESTABLE timer_t s_fan_service_timer =
{
  .type = TIMER_TYPE_CONTINUOUS,
  .initial_ticks = TEN_SECONDS,
  .remaining_ticks = TEN_SECONDS,
  .events_signalled = EV_FAN_SERVICE,
  .unique_id = TIMER_FAN_SERVICE
};

// Note: the motor temperature is assumed based on ambient temperature and set pressure
static const uint8_t s_duty_lookup[5][3] =
{
  // Set pressure <= 30 cmH2O
  {
    // temperature < 30 degC      >= 30 degC                  >= 40 degC
    PERCENT_TO_UINT8(0u),         PERCENT_TO_UINT8(0u),       PERCENT_TO_UINT8(0u)
  },
  // Set pressure >= 30 cmH2O
  {
    // temperature < 30 degC      >= 30 degC                  >= 40 degC
    PERCENT_TO_UINT8(0u),         PERCENT_TO_UINT8(0u),       PERCENT_TO_UINT8(25u)
  },
  // Set pressure >= 35 cmH2O
  {
    // temperature < 30 degC      >= 30 degC                  >= 40 degC
    PERCENT_TO_UINT8(0u),         PERCENT_TO_UINT8(0u),       PERCENT_TO_UINT8(50u)
  },
  // Set pressure >= 40 cmH2O
  {
    // temperature < 30 degC      >= 30 degC                  >= 40 degC
    PERCENT_TO_UINT8(10u),        PERCENT_TO_UINT8(25u),      PERCENT_TO_UINT8(75u)
  },
  // Set pressure >= 45 cmH2O
  {
    // temperature < 30 degC      >= 30 degC                  >= 40 degC
    PERCENT_TO_UINT8(30u),        PERCENT_TO_UINT8(75u),      PERCENT_TO_UINT8(100u)
  },
};

#define PRESSURE_TO_INDEX(p)    ((p - 25) / 5)
#define TEMPERATURE_TO_INDEX(t) ((t - 20) / 10)

TESTABLE size_t s_pressure_lookup_index = 0u;
TESTABLE uint8_t s_current_duty = 0u;

TESTABLE void fan_service(int32_t arg)
{
  (void)arg;

  dispatcher_clear_event_mask(1u << EV_FAN_SERVICE);

  int32_t temperature_index = TEMPERATURE_TO_INDEX(sensor_get_reading(SENSOR_TEMPERATURE));

  if (temperature_index < 0)
  {
    temperature_index = 0;
  }
  else if (temperature_index >= ARRAY_LENGTH(s_duty_lookup))
  {
    temperature_index = ARRAY_LENGTH(s_duty_lookup[0]) - 1u;
  }

  uint8_t duty = s_duty_lookup[s_pressure_lookup_index][temperature_index];

  if (duty > 0)
  {
    if (s_current_duty == 0)
    {
      FAN_PWM_START();
    }

    FAN_PWM(duty);
  }
  else
  {
    FAN_PWM_STOP();
  }

  s_current_duty = duty;
}

TESTABLE void fan_switch_lookup(int32_t set_pressure)
{
  dispatcher_clear_event_mask(1u << EV_FAN_PRESSURE_UPDATE);

  int32_t pressure_index = PRESSURE_TO_INDEX(set_pressure);

  if (pressure_index < 0)
  {
    pressure_index = 0u;
  }
  else if (pressure_index >= ARRAY_LENGTH(s_duty_lookup))
  {
    pressure_index = ARRAY_LENGTH(s_duty_lookup) - 1u;
  }

  if (s_pressure_lookup_index != pressure_index)
  {
    s_pressure_lookup_index = pressure_index;
    fan_service(0);
  }
}

void fan_init(void)
{
  dispatcher_bind(1u << EV_FAN_PRESSURE_UPDATE, fan_switch_lookup);
  dispatcher_bind(1u << EV_FAN_SERVICE, fan_service);
  timer_attach(&s_fan_service_timer);
}
