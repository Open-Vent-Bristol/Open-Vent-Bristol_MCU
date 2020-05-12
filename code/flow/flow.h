#ifndef FLOW_H
#define FLOW_H

#include <stdint.h>

#ifdef __cplusplus
extern "C"
{
#endif

/**
 * @brief Temperature in Kelvins multiplied by 128
 */
typedef uint16_t Flow_Temperature_t;

/**
 * @brief Fixed point scalar deviation
 * @note The scalar deviation per unit is equivalent to 1/256, or approximately 0.0039.
 * For example, a value of 100 will result in a scalar of 1 + (100 * 1/256) = 1.39 approx
 * and a value of -50 will result in a scalar of 1 + (-50 * 1/256) = 0.80 approx
 */
typedef int8_t Flow_CorrectionFactor_t;

/**
 * @brief Get the Flow Rate through the orifice plate
 *
 * @param pressure_delta        Pressure difference in Pa (positive for flow into patient)
 * @param back_pressure         Pressure downstream of orifice plate in Pa
 * @param ambient_temperature   See Flow_Temperature_t
 * @param correction_factor     See Flow_CorrectionFactor_t
 * @return int16_t              Flow rate in ml per second
 */
int16_t Get_Flow_Rate(int16_t pressure_delta,
                      int16_t back_pressure,
                      Flow_Temperature_t ambient_temperature,
                      Flow_CorrectionFactor_t correction_factor);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* FLOW_H */
