/****************************************************************************
 * boards/apus/src/stm32_usb.c
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
#include <stdint.h>

#include <nuttx/irq.h>
#include <nuttx/usb/usbdev.h>

#include "arm_internal.h"
#include "hardware/stm32_usbdev.h"
#include "stm32_usbdev.h"

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: stm32_usbpullup
 *
 * Description:
 *   The F072 has the D+ pull-up built in (USB_BCDR DPPU).
 *
 ****************************************************************************/

int stm32_usbpullup(struct usbdev_s *dev, bool enable)
{
  irqstate_t flags;
  uint16_t regval;

  flags  = enter_critical_section();
  regval = getreg16(STM32_USB_BCDR);

  if (enable)
    {
      regval |= USB_BCDR_DPPU;
    }
  else
    {
      regval &= ~USB_BCDR_DPPU;
    }

  putreg16(regval, STM32_USB_BCDR);
  leave_critical_section(flags);
  return OK;
}

/****************************************************************************
 * Name: stm32_usbsuspend
 ****************************************************************************/

void stm32_usbsuspend(struct usbdev_s *dev, bool resume)
{
}
