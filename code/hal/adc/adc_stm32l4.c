#include "adc/adc.h"
#include "board/board.h"
#include <stddef.h>

static ADC_callback_t s_adc_callback = NULL;

void adc_init(void)
{
  LL_RCC_SetADCClockSource(LL_RCC_ADC_CLKSOURCE_SYSCLK);

  // Enable clocks
  LL_AHB2_GRP1_EnableClock(LL_AHB2_GRP1_PERIPH_ADC);

  LL_ADC_CommonDeInit(ADC123_COMMON);
  LL_ADC_DeInit(ADC1);
  LL_ADC_DeInit(ADC2);

  LL_ADC_CommonInitTypeDef adc_common_init_struct;
  LL_ADC_CommonStructInit(&adc_common_init_struct);
  adc_common_init_struct.CommonClock = LL_ADC_CLOCK_ASYNC_DIV128;
  LL_ADC_CommonInit(ADC123_COMMON, &adc_common_init_struct);

  LL_ADC_InitTypeDef adc_init_struct;
  LL_ADC_StructInit(&adc_init_struct);
  LL_ADC_Init(ADC1, &adc_init_struct);
  LL_ADC_Init(ADC2, &adc_init_struct);

  LL_ADC_INJ_InitTypeDef adc_inj_init_struct;
  LL_ADC_INJ_StructInit(&adc_inj_init_struct);
  LL_ADC_INJ_Init(ADC1, &adc_inj_init_struct);
  LL_ADC_INJ_Init(ADC2, &adc_inj_init_struct);

  LL_ADC_REG_InitTypeDef adc_reg_init_struct;
  LL_ADC_REG_StructInit(&adc_reg_init_struct);
  LL_ADC_REG_Init(ADC1, &adc_reg_init_struct);
  LL_ADC_REG_Init(ADC2, &adc_reg_init_struct);

  // Setup interrupts
  NVIC_SetPriority(ADC1_2_IRQn, 0);
  NVIC_EnableIRQ(ADC1_2_IRQn)
}

bool adc_read_interrupt(ADC_channel_t channel, ADC_callback_t callback)
{
  bool return_val = false;

  // Check for an existing measurement underway by inspecting the reading bit
  if (LL_ADC_REG_IsConversionOngoing(ADC1) == 0u)
  {
    ADMUX |= channel;

    // Assign the callback
    s_adc_callback = callback;

    // Start reading and enable interrupt
    LL_ADC_EnableIT_EOC(ADC1);
    LL_ADC_REG_StartConversion(ADC1);
    return_val = true;
  }

  return return_val;
}

ADC_resolution_t adc_read_blocking(ADC_channel_t channel)
{
  ADC_resolution_t reading = -1;

  // Check for an existing measurement underway by inspecting the reading bit
  if (LL_ADC_REG_IsConversionOngoing(ADC1) == 0u)
  {
    ADMUX |= channel;

    // Start reading
    LL_ADC_REG_StartConversion(ADC1);

    // Block until the reading bit is cleared by hardware
    while (LL_ADC_REG_IsConversionOngoing(ADC1) != 0u) {};

    // Get the reading from the ADC registers
    reading = LL_ADC_REG_ReadConversionData12(ADC1);;

    // Clear the channel
    ADMUX &= ~((1u << MUX3) | (1u << MUX2) | (1u << MUX1) | (1u << MUX0));
  }

  return reading;
}

// ADC complete interrupt will take the reading and push it out via the callback
void ADC1_2_IRQHandler(void)
{
  if (LL_ADC_IsActiveFlag_EOC(ADC1) != 0u)
  {
    LL_ADC_ClearFlag_EOC(ADC1);

    if (s_adc_callback != NULL)
    {
      LL_ADC_DisableIT_EOC(ADC1);

      // Get the reading from the ADC register
      ADC_resolution_t reading = LL_ADC_REG_ReadConversionData12(ADC1);

      s_adc_callback(reading);
    }
  }

  if (LL_ADC_IsActiveFlag_EOC(ADC2) != 0u)
  {
    LL_ADC_ClearFlag_EOC(ADC2);

    if (s_adc_callback != NULL)
    {
      LL_ADC_DisableIT_EOC(ADC2);

      // Get the reading from the ADC register
      ADC_resolution_t reading = LL_ADC_REG_ReadConversionData12(ADC2);

      s_adc_callback(reading);
    }
  }
}
