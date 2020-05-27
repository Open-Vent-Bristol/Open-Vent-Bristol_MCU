// This file should not be included directly.  Include board/board.h instead and define a board ID, e.g. BOARD_MK1
#include <avr/io.h>

// pins 12-17
#define ALERTS_PORTIN       PINB
#define ALERTS_PULLUP       PORTB
#define ALERTS_MODE         DDRB
#define ALERTS_PIN          PORTB0
#define MUX_A_PORT          PORTB
#define MUX_A_MODE          DDRB
#define MUX_A_PIN           PORTB1
#define MUX_B_PORT          PORTB
#define MUX_B_MODE          DDRB
#define MUX_B_PIN           PORTB2
#define SPI_MOSI_PORT       PORTB
#define SPI_MOSI_PIN        PORTB3
#define SPI_MISO_PORT       PORTB
#define SPI_MISO_PIN        PORTB4
#define SPI_SCK_PORT        PORTB
#define SPI_SCK_PIN         PORTB5
#define XTAL1_PORT          PORTB
#define XTAL1_PIN           PORTB6
#define XTAL2_PORT          PORTB
#define XTAL2_PIN           PORTB7

// pins 23-29
#define ADC_FLOW_PORT       PORTC
#define ADC_FLOW_PIN        PORTC0
#define ADC_PRESSURE_PORT   PORTC
#define ADC_PRESSURE_PIN    PORTC1
#define ADC_VBATT_PORT      PORTC
#define ADC_VBATT_PIN       PORTC2
#define ADC_MOTOR_CURR_PORT PORTC
#define ADC_MOTOR_CURR_PIN  PORTC3
#define MOTOR_IN_A_PORT     PORTC
#define MOTOR_IN_A_MODE     DDRC
#define MOTOR_IN_A_PIN      PORTC4
#define ADC_SPARE_PORT      PORTC
#define ADC_SPARE_PIN       PORTC5

// pins 30-32
#define UART_RX_PORT        PORTD
#define UART_RX_PIN         PORTD0
#define UART_TX_PORT        PORTD
#define UART_TX_PIN         PORTD1
#define MOTOR_IN_B_PORT     PORTD
#define MOTOR_IN_B_MODE     DDRD
#define MOTOR_IN_B_PIN      PORTD2

// pins 1-2
#define ALERT_ENABLE_n_PORT PORTD
#define ALERT_ENABLE_n_MODE DDRD
#define ALERT_ENABLE_n_PIN  PORTD3
#define GPIO_SPARE_PORT     PORTD
#define GPIO_SPARE_MODE     DDRD
#define GPIO_SPARE_PIN      PORTD4

// pins 9-11
#define MOTOR_PWM_PORT      PORTD
#define MOTOR_PWM_MODE      DDRD
#define MOTOR_PWM_PIN       PORTD5
#define SR_MR_n_PORT        PORTD
#define SR_MR_n_MODE        DDRD
#define SR_MR_n_PIN         PORTD6
#define SWITCHES_PORTIN     PIND
#define SWITCHES_PULLUP     PORTD
#define SWITCHES_MODE       DDRD
#define SWITCHES_PIN        PORTD7

// pin 19
#define LATCH_PORT          PORTE
#define LATCH_MODE          DDRE
#define LATCH_PIN           PORTE2

// pin 22
#define ADC_TEMP_PORT       PORTE
#define ADC_TEMP_PIN        PORTE3