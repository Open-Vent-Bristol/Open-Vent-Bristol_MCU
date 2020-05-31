// This file should not be included directly.  Include board/board.h instead and define a board ID, e.g. BOARD_MK1
#ifndef BOARD_MK1_H
#define BOARD_MK1_H

#include <avr/io.h>

#define CPU_CLOCK_HZ        (8000000ul)

// This must be a power of two!
#define UART_BUFFER_SIZE    (32u)

#define UART_BAUD_RATE      (57600u)
#define UART_BAUD_SET()     UBRR0L = (CPU_CLOCK_HZ / (UART_BAUD_RATE * 16u)) - 1u; \
                            UBRR0H = 0u
#define UART_CFG()          UCSR0B |= (1u << RXCIE0) | (1u << TXCIE0) | (1u << RXEN0) | (1u << TXEN0)

// Scheduler timer will interrupt at 1 kHz
#define SCHEDULER_TIM_CFG() TCCR1B = (1u << WGM12) | (1u << CS11); \
                            OCR1A = 499u
#define SCHEDULER_START()   TIMSK1 |= (1u << OCIE1A)
#define SCHEDULER_STOP()    TIMSK1 &= ~(1u << OCIE1A)

// Motor PWM 125 kHz, phase correct
#define MOTOR_PWM_TIM_CFG() TCCR0A = (1u << COM0B1) | (1u << WGM02) | (1u << WGM00)
#define MOTOR_PWM_START()   TCCR0B |= (1u << CS01) | (1u << CS00)
#define MOTOR_PWM_STOP()    TCCR0B &= ~((1u << CS01) | (1u << CS00))
#define MOTOR_PWM(value)    OCR0B = (value & 0xFF)

// ADC prescalar 64 chosen for <200 kHz ADC clock (needed for 10-bit resolution)
#define ADC_PRESCALER       (1u << ADPS2) | (1u << ADPS1)
#define ADC_DIGITAL_DISABLE DIDR0

#define PIN_CHANGE_INT_CFG() PCICR = (1u << PCIE2) | (1u << PCIE0)

typedef enum
{
  ADC_FLOW          = ADC0D,
  ADC_PRESSURE      = ADC1D,
  ADC_VBATT         = ADC2D,
  ADC_MOTOR_CURRENT = ADC3D,
  ADC_TEMP          = ADC7D
} ADC_channel_t;

// pins 12-17
#define ALERTS_PORTIN       PINB
#define ALERTS_PULLUP       PORTB
#define ALERTS_MODE         DDRB
#define ALERTS_PIN          PORTB0
#define ALERTS_INT_CHAN     PCINT0
#define ALERTS_INT_REG      PCMSK0
#define ALERTS_INT_ISR      PCINT0_vect
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
#define MOTOR_IN_A_PORT     PORTC
#define MOTOR_IN_A_MODE     DDRC
#define MOTOR_IN_A_PIN      PORTC4
#define ADC_SPARE_PORT      PORTC
#define ADC_SPARE_MODE      DDRC
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

// pins 10-11
#define SR_MR_n_PORT        PORTD
#define SR_MR_n_MODE        DDRD
#define SR_MR_n_PIN         PORTD6
#define SWITCHES_PORTIN     PIND
#define SWITCHES_PULLUP     PORTD
#define SWITCHES_MODE       DDRD
#define SWITCHES_PIN        PORTD7
#define SWITCHES_INT_CHAN   PCINT23
#define SWITCHES_INT_REG    PCMSK2
#define SWITCHES_INT_ISR    PCINT2_vect

// pin 19
#define LATCH_PORT          PORTE
#define LATCH_MODE          DDRE
#define LATCH_PIN           PORTE2

#endif /* BOARD_MK1_H */
