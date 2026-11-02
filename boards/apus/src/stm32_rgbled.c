/****************************************************************************
 * boards/apus/src/stm32_rgbled.c
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

#include <nuttx/timers/pwm.h>
#include <nuttx/leds/rgbled.h>

#include "stm32_pwm.h"
#include "apus.h"

#if !defined(CONFIG_STM32_TIM3_PWM) || !defined(CONFIG_STM32_PWM_MULTICHAN) || \
    !defined(CONFIG_STM32_TIM3_CH1OUT) || !defined(CONFIG_STM32_TIM3_CH2OUT) || \
    !defined(CONFIG_STM32_TIM3_CH3OUT)
#  error "The RGB LED needs TIM3 PWM with channels 1-3 as outputs"
#endif

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: stm32_rgbled_setup
 ****************************************************************************/

int stm32_rgbled_setup(void)
{
  struct pwm_lowerhalf_s *pwm;
  int ret;

  pwm = stm32_pwminitialize(APUS_RGBLED_TIMER);
  if (pwm == NULL)
    {
      lederr("ERROR: Failed to get TIM%d PWM\n", APUS_RGBLED_TIMER);
      return -ENODEV;
    }

  ret = pwm->ops->setup(pwm);
  if (ret < 0)
    {
      return ret;
    }

  return rgbled_register("/dev/rgbled0", pwm, pwm, pwm,
                         APUS_RGBLED_RCHAN, APUS_RGBLED_GCHAN,
                         APUS_RGBLED_BCHAN);
}
