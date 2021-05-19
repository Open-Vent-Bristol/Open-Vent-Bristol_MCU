// Copyright (c) 2021 <OpenVent-Bristol, Donald Robson>

#include "uart/uart.h"
#include "ftp_types.h"
#include <string.h>

static const char string_error_large[] = " - arg too large (0-1023)\r\n>";
static const char string_error_unrecognised[] = " - unrecognised\r\n>";

/**
 * @brief Valid FTP command shall be a letter followed by a number
 */
ftp_command_t s_command = {0u};

/**
 * @brief Return a command via the supplied pointer, if a complete command has been received.
 * @note Command will be zeroed if no command waiting.
 */
void uart_ftp_get_command(ftp_command_t* command)
{
  command->instruction = NONE;
  command-> arg = 0u;

  if (uart_available())
  {
    char new_char = {'\0'};
    uart_read(&new_char, 1u);

    // Echo the character
    uart_write(&new_char, sizeof(new_char));

    switch(new_char)
    {
      case READ_FPGA_READY:
      case READ_MOTOR_FAULT_A:
      case READ_MOTOR_FAULT_B:
      case READ_PGOOD:
      case READ_PRESSURE_ALERT:
      case READ_PB_SEL:
      case READ_PB_MUTE:
      case READ_PB_MINUS:
      case READ_PB_PLUS:
      case READ_SW_BACKUP:
      case READ_SW_END:
      case READ_SW_HOME:
      case READ_ANALOG_VBATT:
      case READ_ANALOG_MOTOR_CURR:
      case READ_ANALOG_SPARE:
      case MOTOR_STOP:
      case MOTOR_RUN_IN:
      case MOTOR_RUN_OUT:
      case PRINT_SPI:
      case BUZZER_CONTROL:
      case BATT_LED_CONTROL:
      case CE_CONTROL:
      case LDO_EN_CONTROL:
      case PWR_UP_CONTROL:
        s_command.instruction = new_char;
        break;

      case '0':
      case '1':
      case '2':
      case '3':
      case '4':
      case '5':
      case '6':
      case '7':
      case '8':
      case '9':
        // Argument
        if (s_command.instruction != NONE)
        {
          s_command.arg *= 10u;
          s_command.arg += (new_char - '0');

          if (s_command.arg > 255u)
          {
            uart_write(string_error_large, sizeof(string_error_large));
            s_command.instruction = NONE;
            s_command.arg = 0u;
          }
        }
        break;

      case '\r':
        // Completed command
        command->instruction = s_command.instruction;
        command->arg = s_command.arg;
        s_command.instruction = '\0';
        s_command.arg = 0u;
        break;

      case '\n':
        // Compatibility - ignore
        break;

      default:
        uart_write(string_error_unrecognised, sizeof(string_error_unrecognised));
        s_command.instruction = NONE;
        s_command.arg = 0u;
    }
  }
}
