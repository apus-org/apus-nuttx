/****************************************************************************
 * boards/apus/src/stm32_boot.c
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

#include <nuttx/board.h>
#include <arch/board/board.h>

#include "arm_internal.h"
#include "hardware/stm32_rcc.h"
#include "apus.h"

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: stm32_boardinitialize
 ****************************************************************************/

void stm32_boardinitialize(void)
{
#ifndef CONFIG_STM32_SYSCFG
  /* The F0 Kconfig never offers STM32_SYSCFG, yet EXTI port selection
   * needs the SYSCFG clock.
   */

  modifyreg32(STM32_RCC_APB2ENR, 0, RCC_APB2ENR_SYSCFGCOMPEN);
#endif

#ifdef CONFIG_STM32_SPI1
  stm32_spidev_initialize();
#endif

#ifdef CONFIG_ARCH_LEDS
  board_autoled_initialize();
#endif
}

/****************************************************************************
 * Name: board_late_initialize
 ****************************************************************************/

#ifdef CONFIG_BOARD_LATE_INITIALIZE
void board_late_initialize(void)
{
  stm32_bringup();
}
#endif
