// Copyright (c) 2021 <OpenVent-Bristol, Donald Robson>

#include "pid.h"

void pid_update(pid_data_t* const pid, uint32_t time_elapsed, int16_t measurement)
{
  if (time_elapsed != 0u)
  {
    int32_t error = (int32_t)pid->set_point - (int32_t)measurement;

    pid->integral = pid->integral + ((float)error * (float)time_elapsed);

    float derivative = (float)(error - pid->previous_error) / time_elapsed;
    pid->previous_error = error;

    float output = (pid->kp * (float)error) + (pid->ki * pid->integral) + (pid->kd * derivative);

    if (output < 0)
    {
      output = output - 0.5;
    }
    else
    {
      output = output + 0.5;
    }

    if (output > (float)INT16_MAX)
    {
      output = INT16_MAX;
    }
    else if (output < (float)INT16_MIN)
    {
      output = INT16_MIN;
    }

    pid->output = (int16_t)output;
  }
}
