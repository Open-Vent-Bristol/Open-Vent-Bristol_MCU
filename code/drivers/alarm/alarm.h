#ifndef SOUNDER_H
#define SOUNDER_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

enum alarm_mode
{
  ALARM_OVERRIDE_OFF,     // Disables backup alarm mode for use by FPGA - silent
  ALARM_OVERRIDE_ON,      // Backup alarm mode for use by FPGA - intermittent
  ALARM_SYSTEM_FAILURE    // Sticky - once set, cannot be disabled - continuous
};

/**
 * @brief Initialise the alarm function
 * @note The alarm will *not* function properly in ALARM_OVERRIDE_ON mode until this has been
 * called (the alarm will sound continuously instead of intermittently).
 */
void alarm_init(void);

/**
 * @brief Sound the alarm
 *
 * @param mode    Mode as in enuim alarm_mode
 */
void alarm_mode(int32_t mode);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* SOUNDER_H */
