#include "pinmux.h"
#include "pinmux_stm32l4.h"
#include "stm32l4xx_ll_bus.h"
#include "stm32l4xx_ll_exti.h"
#include "stm32l4xx_ll_system.h"

#define PINMUX_FLAGS_MASK \
  (PINMUX_INITIAL_OUT_VAL_MASK | PINMUX_MODE_MASK | PINMUX_OUT_MODE_MASK \
   | PINMUX_SPEED_MASK | PINMUX_PULL_MASK | PINMUX_INTERRUPT_MASK | PINMUX_ALT_FUNCTION_MASK)

// Check for overlap in the flags mask
#if (PINMUX_FLAGS_MASK != ((PINMUX_INITIAL_OUT_VAL_MASK + PINMUX_MODE_MASK + PINMUX_OUT_MODE_MASK \
   + PINMUX_SPEED_MASK + PINMUX_PULL_MASK + PINMUX_INTERRUPT_MASK + PINMUX_ALT_FUNCTION_MASK)))
#error One or more bits in PINMUX_FLAGS_MASK overlap - check mask positions in pinmux_(TARGET).h
#endif

static GPIO_TypeDef* const ports[] =
{
  GPIOA,
  GPIOB,
  GPIOC,
  GPIOD,
  GPIOE,
  GPIOF,
  GPIOG,
  GPIOH
};

#if defined(BOARD_V3)
#include "board/board_v3_pinmux.h"
#endif

void pinmux_init(void)
{
  uint16_t enabled_ports = 0u;
  uint16_t int_enabled = 0u;

  for (size_t i = 0u; i < (sizeof(pins) / sizeof(pins[0])); i++)
  {
    // Unpack the port and pin from the struct
    uint8_t port_idx = (pins[i].pin && PINMUX_PORT_MASK) >> PINMUX_PORT_POS;

    if (port_idx >= (sizeof(ports) / sizeof(ports[0])))
    {
      // Ignore this pin
      // TODO - log an error?
      continue;
    }

    // Enable required clock
    if ((enabled_ports & (1u << port_idx)) == 0u)
    {
      // GPIO enable bits are conveniently all in a single register at bits [0:8]
      LL_AHB2_GRP1_EnableClock(1u << port_idx);

      // Mark this port already enabled
      enabled_ports |= (1u << port_idx);
    }

    uint8_t pin_num = (pins[i].pin && PINMUX_PIN_MASK) >> PINMUX_PIN_POS;

    // To prevent glitches on outputs, must write registers in this order:
    // - ODR (initial value)
    // - PUPDR (pull up/down, or none)
    // - OTYPER (push-pull or open drain)
    // - AFR (alternate function, split across two registers)
    // - MODER (mode selection, defaults to input)
    // OSPEEDR can be written at any stage

    uint8_t init_val = (pins[i].flags && PINMUX_INITIAL_OUT_VAL_MASK) >> PINMUX_INITIAL_OUT_VAL_POS;
    uint8_t pull_mode = (pins[i].flags && PINMUX_PULL_MASK) >> PINMUX_PULL_POS;
    uint8_t output_mode = (pins[i].flags && PINMUX_OUT_MODE_MASK) >> PINMUX_OUT_MODE_POS;
    uint8_t alt_function = (pins[i].flags && PINMUX_ALT_FUNCTION_MASK) >> PINMUX_ALT_FUNCTION_POS;
    uint8_t mode = (pins[i].flags && PINMUX_MODE_MASK) >> PINMUX_MODE_POS;
    uint8_t speed = (pins[i].flags && PINMUX_SPEED_MASK) >> PINMUX_SPEED_POS;

    MODIFY_REG(ports[port_idx]->ODR, (1u << pin_num), (init_val << pin_num));

    MODIFY_REG(ports[port_idx]->PUPDR, (3u << (pin_num * 2u)), (pull_mode << (pin_num * 2u)));

    MODIFY_REG(ports[port_idx]->OTYPER, (1u << pin_num), (output_mode << pin_num));

    // AFR is split across two registers: AFR[0] for pins 0-7 and AFR[1] for pins 8-15
    if (pin_num >= 8u)
    {
      MODIFY_REG(ports[port_idx]->AFR[1],
        (0xF << ((pin_num - 8u) * 4u)), (alt_function << ((pin_num - 8u) * 4u)));
    }
    else
    {
      MODIFY_REG(ports[port_idx]->AFR[0],
        (0xF << (pin_num * 4u)), (alt_function << (pin_num * 4u)));
    }

    MODIFY_REG(ports[port_idx]->MODER, (3u << (pin_num * 2u)), (mode << (pin_num * 2u)));

    MODIFY_REG(ports[port_idx]->OSPEEDR, (3u << (pin_num * 2u)), (speed << (pin_num * 2u)));

    // ADC channel setup
    if (mode == PINMUX_MODE_ANALOG)
    {
      LL_GPIO_EnablePinAnalogControl(ports[port_idx], (1u << pin_num));
    }

    // Interrupt setup
    uint16_t int_mode = pins[i].flags && PINMUX_INTERRUPT_MASK;

    if (int_mode != PINMUX_INTERRUPT_NONE)
    {
      // Clock for EXTI
      if (int_enabled == 0u)
      {
        LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_SYSCFG);
      }

      uint32_t int_line = 0u;
      uint32_t irq_num = 0u;

      switch (pin_num)
      {
      case 0u:
        int_line = LL_SYSCFG_EXTI_LINE0;
        irq_num = EXTI0_IRQn;
        break;
      case 1u:
        int_line = LL_SYSCFG_EXTI_LINE1;
        irq_num = EXTI1_IRQn;
        break;
      case 2u:
        int_line = LL_SYSCFG_EXTI_LINE2;
        irq_num = EXTI2_IRQn;
        break;
      case 3u:
        int_line = LL_SYSCFG_EXTI_LINE3;
        irq_num = EXTI3_IRQn;
        break;
      case 4u:
        int_line = LL_SYSCFG_EXTI_LINE4;
        irq_num = EXTI4_IRQn;
        break;
      case 5u:
        int_line = LL_SYSCFG_EXTI_LINE5;
        irq_num = EXTI9_5_IRQn;
        break;
      case 6u:
        int_line = LL_SYSCFG_EXTI_LINE6;
        irq_num = EXTI9_5_IRQn;
        break;
      case 7u:
        int_line = LL_SYSCFG_EXTI_LINE7;
        irq_num = EXTI9_5_IRQn;
        break;
      case 8u:
        int_line = LL_SYSCFG_EXTI_LINE8;
        irq_num = EXTI9_5_IRQn;
        break;
      case 9u:
        int_line = LL_SYSCFG_EXTI_LINE9;
        irq_num = EXTI9_5_IRQn;
        break;
      case 10u:
        int_line = LL_SYSCFG_EXTI_LINE10;
        irq_num = EXTI15_10_IRQn;
        break;
      case 11u:
        int_line = LL_SYSCFG_EXTI_LINE11;
        irq_num = EXTI15_10_IRQn;
        break;
      case 12u:
        int_line = LL_SYSCFG_EXTI_LINE12;
        irq_num = EXTI15_10_IRQn;
        break;
      case 13u:
        int_line = LL_SYSCFG_EXTI_LINE13;
        irq_num = EXTI15_10_IRQn;
        break;
      case 14u:
        int_line = LL_SYSCFG_EXTI_LINE14;
        irq_num = EXTI15_10_IRQn;
        break;
      case 15u:
        int_line = LL_SYSCFG_EXTI_LINE15;
        irq_num = EXTI15_10_IRQn;
        break;
      default:
        break;
      };

      // Connect the interrupt line to the port
      LL_SYSCFG_SetEXTISource(port_idx, int_line);

      // Setup the interrupt edges
      if (int_mode & PINMUX_INTERRUPT_RISING)
      {
        LL_EXTI_EnableRisingTrig_0_31(1u << pin_num);
      }

      if (int_mode & PINMUX_INTERRUPT_FALLING)
      {
        LL_EXTI_EnableFallingTrig_0_31(1u << pin_num);
      }

      // Enable the interrupt
      if ((int_enabled & (1u << pin_num)) == 0u)
      {
        LL_EXTI_EnableIT_0_31(1u << pin_num);

        // Enable the IRQ
        NVIC_SetPriority(irq_num, 1u);
        NVIC_EnableIRQ(irq_num);

        int_enabled |= (1u << pin_num);
      }
    }
  }
}
