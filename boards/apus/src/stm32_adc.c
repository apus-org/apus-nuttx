/****************************************************************************
 * boards/apus/src/stm32_adc.c
 *
 * Copyright (c) 2026 Jakob Jungreuthmayer
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 ****************************************************************************/

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <nuttx/config.h>

#include <errno.h>
#include <debug.h>

#include <nuttx/analog/adc.h>

#include "stm32.h"
#include "apus.h"

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

#define APUS_ADC_NCHANNELS 7

#if CONFIG_STM32_ADC_MAX_SAMPLES < APUS_ADC_NCHANNELS
#  error "CONFIG_STM32_ADC_MAX_SAMPLES must cover all APUS ADC channels"
#endif

/****************************************************************************
 * Private Data
 ****************************************************************************/

/* The F0 converts in ascending channel order, so keep the list sorted */

static const uint8_t g_chanlist[APUS_ADC_NCHANNELS] =
{
  1,   /* A0 */
  2,   /* A1 */
  4,   /* A2 */
  6,   /* A3 */
  8,   /* A4 */
  9,   /* A5 */
  14,  /* potentiometer */
};

static const uint32_t g_pinlist[APUS_ADC_NCHANNELS] =
{
  GPIO_ADC1_IN1_0,   /* PA1 */
  GPIO_ADC1_IN2_0,   /* PA2 */
  GPIO_ADC1_IN4_0,   /* PA4 */
  GPIO_ADC1_IN6_0,   /* PA6 */
  GPIO_ADC1_IN8_0,   /* PB0 */
  GPIO_ADC1_IN9_0,   /* PB1 */
  GPIO_ADC1_IN14_0,  /* PC4 */
};

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: stm32_adc_setup
 ****************************************************************************/

int stm32_adc_setup(void)
{
  struct adc_dev_s *adc;
  int i;

  for (i = 0; i < APUS_ADC_NCHANNELS; i++)
    {
      stm32_configgpio(g_pinlist[i]);
    }

  adc = stm32_adcinitialize(1, g_chanlist, APUS_ADC_NCHANNELS);
  if (adc == NULL)
    {
      aerr("ERROR: Failed to get ADC1\n");
      return -ENODEV;
    }

  return adc_register("/dev/adc0", adc);
}
