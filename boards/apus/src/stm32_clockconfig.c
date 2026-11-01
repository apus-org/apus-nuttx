/****************************************************************************
 * boards/apus/src/stm32_clockconfig.c
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

#include <stdint.h>

#include <arch/board/board.h>

#include "arm_internal.h"
#include "stm32_rcc.h"
#include "hardware/stm32_flash.h"

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: stm32_board_clockconfig
 *
 * Description:
 *   SYSCLK = HSE / PREDIV * PLLMUL, from the values in board.h.
 *
 ****************************************************************************/

void stm32_board_clockconfig(void)
{
  uint32_t regval;

  if ((getreg32(STM32_RCC_CFGR) & RCC_CFGR_SWS_MASK) == RCC_CFGR_SWS_PLL)
    {
      regval  = getreg32(STM32_RCC_CFGR);
      regval &= ~RCC_CFGR_SW_MASK;
      putreg32(regval, STM32_RCC_CFGR);

      while ((getreg32(STM32_RCC_CFGR) & RCC_CFGR_SWS_MASK) !=
              RCC_CFGR_SWS_HSI);
    }

  regval  = getreg32(STM32_RCC_CR);
  regval &= ~RCC_CR_PLLON;
  putreg32(regval, STM32_RCC_CR);
  while ((getreg32(STM32_RCC_CR) & RCC_CR_PLLRDY) != 0);

  regval  = getreg32(STM32_RCC_CR);
  regval |= RCC_CR_HSEON;
  putreg32(regval, STM32_RCC_CR);
  while ((getreg32(STM32_RCC_CR) & RCC_CR_HSERDY) == 0);

  regval  = getreg32(STM32_FLASH_ACR);
  regval &= ~FLASH_ACR_LATENCY_MASK;
  regval |= (FLASH_ACR_LATENCY_1 | FLASH_ACR_PRTFBE);
  putreg32(regval, STM32_FLASH_ACR);

  regval  = getreg32(STM32_RCC_CFGR);
  regval &= ~(RCC_CFGR_HPRE_MASK | RCC_CFGR_PPRE1_MASK);
  regval |= (STM32_RCC_CFGR_HPRE | STM32_RCC_CFGR_PPRE1);
  putreg32(regval, STM32_RCC_CFGR);

  regval  = getreg32(STM32_RCC_CFGR2);
  regval &= ~RCC_CFGR2_PREDIV1_MASK;
  regval |= STM32_CFGR2_PREDIV;
  putreg32(regval, STM32_RCC_CFGR2);

  regval  = getreg32(STM32_RCC_CFGR);
  regval &= ~(RCC_CFGR_PLLSRC_MASK | RCC_CFGR_PLLXTPRE_MASK |
              RCC_CFGR_PLLMUL_MASK);
  regval |= (STM32_CFGR_PLLSRC | STM32_CFGR_PLLMUL);
  putreg32(regval, STM32_RCC_CFGR);

  regval  = getreg32(STM32_RCC_CR);
  regval |= RCC_CR_PLLON;
  putreg32(regval, STM32_RCC_CR);
  while ((getreg32(STM32_RCC_CR) & RCC_CR_PLLRDY) == 0);

  regval  = getreg32(STM32_RCC_CFGR);
  regval &= ~RCC_CFGR_SW_MASK;
  regval |= STM32_SYSCLK_SW;
  putreg32(regval, STM32_RCC_CFGR);
  while ((getreg32(STM32_RCC_CFGR) & RCC_CFGR_SWS_MASK) != STM32_SYSCLK_SWS);
}
