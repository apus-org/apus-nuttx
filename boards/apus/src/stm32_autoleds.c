/****************************************************************************
 * boards/apus/src/stm32_autoleds.c
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

#include <stdbool.h>

#include <nuttx/board.h>
#include <arch/board/board.h>

#include "chip.h"
#include "apus.h"

#ifdef CONFIG_ARCH_LEDS

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: board_autoled_initialize
 ****************************************************************************/

void board_autoled_initialize(void)
{
  stm32_configgpio(GPIO_LED3);
  stm32_configgpio(GPIO_LED4);
  stm32_configgpio(GPIO_LED5);
}

/****************************************************************************
 * Name: board_autoled_on
 ****************************************************************************/

void board_autoled_on(int led)
{
  switch (led)
    {
      case 1:
        stm32_gpiowrite(GPIO_LED3, false);
        break;

      case 2:
        stm32_gpiowrite(GPIO_LED4, false);
        break;

      case 3:
        stm32_gpiowrite(GPIO_LED5, false);
        break;

      default:
        break;
    }
}

/****************************************************************************
 * Name: board_autoled_off
 ****************************************************************************/

void board_autoled_off(int led)
{
  switch (led)
    {
      case 2:
        stm32_gpiowrite(GPIO_LED4, true);
        break;

      case 3:
        stm32_gpiowrite(GPIO_LED5, true);
        break;

      default:
        break;
    }
}

#endif /* CONFIG_ARCH_LEDS */
