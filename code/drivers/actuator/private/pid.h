// Copyright (c) 2021 <OpenVent-Bristol, Donald Robson>

#ifndef PID_H
#define PID_H

#include <stdint.h>

typedef struct
{
  const int16_t output_max;
  const int16_t output_min;
  int16_t output;
  int16_t set_point;
  int32_t previous_error;
  float integral;
  float kp;
  float ki;
  float kd;
} pid_data_t;

/**
 * @brief Update the PID control measurement and recalculate the output
 *
 * @param pid
 * @param measurement
 */
void pid_update(pid_data_t* const pid, uint32_t time_elapsed, int16_t measurement);

#endif /* PID_H */
