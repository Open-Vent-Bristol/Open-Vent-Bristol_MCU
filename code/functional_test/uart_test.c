#include "uart/uart.h"
#include "ftp_types.h"
#include <string.h>

static const char string_error_large[] = " - arg too large (0-255)\r\n>";
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
      case READ_ALERT_INTERLOCK:
      case READ_ALERT_PRESSURE:
      case READ_ALERT_FLOW:
      case READ_ALERT_VBATT:
      case READ_SWITCH_1:
      case READ_SWITCH_2:
      case READ_SWITCH_3:
      case READ_SWITCH_4:
      case READ_ANALOGUE_FLOW:
      case READ_ANALOGUE_PRESSURE:
      case READ_ANALOGUE_VBATT:
      case READ_ANALOGUE_MOTOR:
      case READ_ANALOGUE_TEMP:
      case MOTOR_STOP:
      case RUN_MOTOR_IN:
      case RUN_MOTOR_OUT:
      case PRINT_SPI:
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
          if (s_command.arg > (UINT8_MAX / 10u))
          {
            uart_write(string_error_large, sizeof(string_error_large));
          }
          else
          {
            s_command.arg *= 10u;
            s_command.arg += (new_char - '0');
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
