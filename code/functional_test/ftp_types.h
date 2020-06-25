#ifndef FTP_TYPES_H
#define FTP_TYPES_H

#include <stdint.h>

typedef enum
{
  NONE                    = '\0',
  READ_ALERT_INTERLOCK    = 'q',
  READ_ALERT_PRESSURE     = 'w',
  READ_ALERT_FLOW         = 'e',
  READ_ALERT_VBATT        = 'r',
  READ_SWITCH_1           = 'a',
  READ_SWITCH_2           = 's',
  READ_SWITCH_3           = 'd',
  READ_SWITCH_4           = 'f',
  READ_ANALOGUE_FLOW      = 'z',
  READ_ANALOGUE_PRESSURE  = 'x',
  READ_ANALOGUE_VBATT     = 'c',
  READ_ANALOGUE_MOTOR     = 'v',
  READ_ANALOGUE_TEMP      = 'b',
  MOTOR_STOP              = 'u',
  RUN_MOTOR_IN            = 'i',
  RUN_MOTOR_OUT           = 'o',
  PRINT_SPI               = 'p'
}
ftp_instruction_t;

typedef struct
{
  ftp_instruction_t instruction;
  uint8_t arg;
} ftp_command_t;

#endif /* FTP_TYPES_H */
