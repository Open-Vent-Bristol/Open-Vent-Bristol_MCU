#ifndef FTP_TYPES_H
#define FTP_TYPES_H

#include <stdint.h>

typedef enum
{
  NONE                    = '\0',
  READ_FPGA_READY         = 'q',
  READ_MOTOR_FAULT_A      = 'w',
  READ_MOTOR_FAULT_B      = 'e',
  READ_PGOOD              = 'r',
  READ_PRESSURE_ALERT     = 't',
  READ_PB_SEL             = 'a',
  READ_PB_MUTE            = 's',
  READ_PB_MINUS           = 'd',
  READ_PB_PLUS            = 'f',
  READ_SW_BACKUP          = 'g',
  READ_SW_END             = 'h',
  READ_SW_HOME            = 'j',
  READ_ANALOG_VBATT       = 'z',
  READ_ANALOG_MOTOR_CURR  = 'x',
  READ_ANALOG_ALARM_CURR  = 'c',
  READ_ANALOG_SPARE       = 'v',
  MOTOR_STOP              = 'u',
  MOTOR_RUN_IN            = 'i',
  MOTOR_RUN_OUT           = 'o',
  PRINT_SPI               = 'p',
  BUZZER_CONTROL          = '[',
  BATT_LED_CONTROL        = ']',
  CE_CONTROL              = ',',
  LDO_EN_CONTROL          = '.',
  PWR_UP_CONTROL          = '?',
}
ftp_instruction_t;

typedef struct
{
  ftp_instruction_t instruction;
  uint16_t arg;
} ftp_command_t;

#endif /* FTP_TYPES_H */
