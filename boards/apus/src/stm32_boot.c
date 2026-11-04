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

#include "apus.h"

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: stm32_boardinitialize
 ****************************************************************************/

void stm32_boardinitialize(void)
{
#ifdef CONFIG_STM32_SPI
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
