#include "scheduler/dispatcher.h"

void dispatcher_call(event_t event, int16_t arg)
{
  switch (event)
  {
      case EV_NONE:
        break;

      case EV_DO_FLOW_READ:
      case EV_DO_PRESSURE_READ:
      case EV_DO_VBATT_READ:
      case EV_DO_MOTOR_CURR_READ:
      case EV_DO_TEMP_READ:
        break;

      case EV_FLOW_READ_COMPLETE:
      case EV_PRESSURE_READ_COMPLETE:
      case EV_VBATT_READ_COMPLETE:
      case EV_MOTOR_CURR_READ_COMPLETE:
      case EV_TEMP_READ_COMPLETE:
        break;

      case EV_TIMER_COMPLETE:
        break;

      case EV_GPIO_ALERTS:
      case EV_GPIO_SWITCHES:
        break;

      case EV_CASE_INTLK_ALERT:
        break;

      case EV_CASE_INTLK_ALERT_CLEAR:
        break;

      case EV_PRESSURE_ALERT:
        break;

      case EV_PRESSURE_ALERT_CLEAR:
        break;

      case EV_FLOW_ALERT:
        break;

      case EV_FLOW_ALERT_CLEAR:
        break;

      case EV_VBATT_ALERT:
        break;

      case EV_VBATT_ALERT_CLEAR:
        break;

      case EV_GPIO_SWITCH_0:
      case EV_GPIO_SWITCH_1:
      case EV_GPIO_SWITCH_2:
      case EV_GPIO_SWITCH_3:
        break;

      case EV_DO_UPDATE_DISPLAY_0:
      case EV_DO_UPDATE_DISPLAY_1:
        break;

      case EV_DO_UART_LOG:
        break;

      /* SYSTEM TEST */
      case EV_UART_EV_INJECT:
        break;

      case EV_UART_EV_DISPLAY_0_WRITE:
      case EV_UART_EV_DISPLAY_1_WRITE:
        break;

      default:
        // TODO - should this have a consequence?
        break;
  }
}
