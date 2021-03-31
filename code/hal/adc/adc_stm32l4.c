#include "adc/adc.h"
#include "board/board.h"
#include <stddef.h>

typedef enum
{
  ADC_PERIPH_1      = 0,
  ADC_PERIPH_2      = 1,

  // Peripheral indexes must be above this line
  ADC_PERIPH_COUNT,
  ADC_PERIPH_NONE   = -1
} ADC_peripheral_t;

typedef struct
{
  bool available;
  ADC_TypeDef* adc_registers;
  ADC_callback_t callback;
} ADC_handle_t;

static ADC_handle_t s_adc_handles[ADC_PERIPH_COUNT] =
{
  { .available = false, .adc_registers = ADC1, .callback = NULL },
  { .available = false, .adc_registers = ADC2, .callback = NULL }
};

static ADC_peripheral_t assign_peripheral(ADC_channel_t channel);

void adc_init(void)
{
  LL_RCC_SetADCClockSource(LL_RCC_ADC_CLKSOURCE_SYSCLK);

  // Enable clocks
  LL_AHB2_GRP1_EnableClock(LL_AHB2_GRP1_PERIPH_ADC);

  LL_ADC_CommonDeInit(ADC123_COMMON);
  LL_ADC_DeInit(ADC1);
  LL_ADC_DeInit(ADC2);

  LL_ADC_DisableDeepPowerDown(ADC1);
  LL_ADC_DisableDeepPowerDown(ADC2);

  LL_ADC_EnableInternalRegulator(ADC1);
  CLOCK_DELAY(LL_ADC_DELAY_INTERNAL_REGUL_STAB_US);

  LL_ADC_EnableInternalRegulator(ADC2);
  CLOCK_DELAY(LL_ADC_DELAY_INTERNAL_REGUL_STAB_US);

  // Reduce the peripheral clock to <= 125kHz
  LL_ADC_SetCommonClock(ADC123_COMMON, LL_ADC_CLOCK_ASYNC_DIV128);

  LL_ADC_StartCalibration(ADC1, LL_ADC_SINGLE_ENDED);
  while (LL_ADC_IsCalibrationOnGoing(ADC1) != 0);

  LL_ADC_StartCalibration(ADC2, LL_ADC_SINGLE_ENDED);
  while (LL_ADC_IsCalibrationOnGoing(ADC2) != 0);

  // Set ADV to use the internal regulator as a reference voltage
  LL_ADC_SetCommonPathInternalCh(ADC123_COMMON, LL_ADC_PATH_INTERNAL_VREFINT);

  // LL_ADC_InitTypeDef adc_init_struct;
  // LL_ADC_StructInit(&adc_init_struct);
  // LL_ADC_Init(ADC1, &adc_init_struct);
  // LL_ADC_Init(ADC2, &adc_init_struct);

  // LL_ADC_REG_InitTypeDef adc_reg_init_struct;
  // LL_ADC_REG_StructInit(&adc_reg_init_struct);
  // LL_ADC_REG_Init(ADC1, &adc_reg_init_struct);
  // LL_ADC_REG_Init(ADC2, &adc_reg_init_struct);


  LL_ADC_Enable(ADC1);
  LL_ADC_Enable(ADC2);

  // Use the interrupts so we don't have to wait for the ADCs to finish being ready
  LL_ADC_EnableIT_ADRDY(ADC1);
  LL_ADC_EnableIT_ADRDY(ADC2);

  // Setup interrupts in CPU
  NVIC_SetPriority(ADC1_2_IRQn, 0);
  NVIC_EnableIRQ(ADC1_2_IRQn);
}

bool adc_read_interrupt(ADC_channel_t channel, ADC_callback_t callback)
{
  bool return_val = false;

  ADC_peripheral_t peripheral = assign_peripheral(channel);
  if (peripheral != ADC_PERIPH_NONE)
  {
    // Set channel
    LL_ADC_REG_SetSequencerRanks(
      s_adc_handles[peripheral].adc_registers, LL_ADC_REG_RANK_1, channel);
	  LL_ADC_REG_SetSequencerLength(
      s_adc_handles[peripheral].adc_registers, LL_ADC_REG_SEQ_SCAN_DISABLE);

    // Enable interrupt
    LL_ADC_EnableIT_EOC(s_adc_handles[peripheral].adc_registers);

    // Start reading
    LL_ADC_REG_StartConversion(s_adc_handles[peripheral].adc_registers);
  }

  return return_val;
}

ADC_resolution_t adc_read_blocking(ADC_channel_t channel)
{
  ADC_resolution_t reading = -1;

  ADC_peripheral_t peripheral = assign_peripheral(channel);
  if (peripheral != ADC_PERIPH_NONE)
  {
    // Set channel
    LL_ADC_REG_SetSequencerRanks(
      s_adc_handles[peripheral].adc_registers, LL_ADC_REG_RANK_1, channel);
	  LL_ADC_REG_SetSequencerLength(
      s_adc_handles[peripheral].adc_registers, LL_ADC_REG_SEQ_SCAN_DISABLE);

    // Start reading
    LL_ADC_REG_StartConversion(s_adc_handles[peripheral].adc_registers);

    // Block until the reading bit is cleared by hardware
    while (LL_ADC_REG_IsConversionOngoing(s_adc_handles[peripheral].adc_registers) != 0u) {};

    // Get the reading from the ADC registers
    reading = LL_ADC_REG_ReadConversionData12(s_adc_handles[peripheral].adc_registers);

    s_adc_handles[peripheral].available = true;
  }

  return reading;
}

static ADC_peripheral_t assign_peripheral(ADC_channel_t channel)
{
  ADC_peripheral_t peripheral = ADC_PERIPH_NONE;

  if (s_adc_handles[ADC_PERIPH_2].available)
  {
    peripheral = ADC_PERIPH_2;
  }
  else if (s_adc_handles[ADC_PERIPH_1].available)
  {
    peripheral = ADC_PERIPH_1;
  }

  if (peripheral != ADC_PERIPH_NONE)
  {
    s_adc_handles[peripheral].available = false;
  }

  return peripheral;
}

// ADC complete interrupt will take the reading and push it out via the callback
void ADC1_2_IRQHandler(void)
{
  if (LL_ADC_IsActiveFlag_EOC(ADC1) != 0u)
  {
    LL_ADC_ClearFlag_EOC(ADC1);

    if (s_adc_handles[ADC_PERIPH_1].callback != NULL)
    {
      LL_ADC_DisableIT_EOC(ADC1);

      // Get the reading from the ADC register
      ADC_resolution_t reading = LL_ADC_REG_ReadConversionData12(ADC1);

      s_adc_handles[ADC_PERIPH_1].callback(reading);
    }

    s_adc_handles[ADC_PERIPH_1].available = true;
  }
  else if (LL_ADC_IsActiveFlag_EOC(ADC2) != 0u)
  {
    LL_ADC_ClearFlag_EOC(ADC2);

    if (s_adc_handles[ADC_PERIPH_2].callback != NULL)
    {
      LL_ADC_DisableIT_EOC(ADC2);

      // Get the reading from the ADC register
      ADC_resolution_t reading = LL_ADC_REG_ReadConversionData12(ADC2);

      s_adc_handles[ADC_PERIPH_2].callback(reading);
    }

    s_adc_handles[ADC_PERIPH_2].available = true;
  }
  // This clause should be last as it only applies at initialisation
  else if (LL_ADC_IsEnabledIT_ADRDY(ADC1) || LL_ADC_IsEnabledIT_ADRDY(ADC2))
  {
    if (LL_ADC_IsActiveFlag_ADRDY(ADC1) != 0u)
    {
      s_adc_handles[ADC_PERIPH_1].available = true;
      LL_ADC_ClearFlag_ADRDY(ADC1);
      LL_ADC_DisableIT_ADRDY(ADC1);
    }

    if (LL_ADC_IsActiveFlag_ADRDY(ADC2) != 0u)
    {
      s_adc_handles[ADC_PERIPH_2].available = true;
      LL_ADC_ClearFlag_ADRDY(ADC2);
      LL_ADC_DisableIT_ADRDY(ADC2);
    }
  }
}
