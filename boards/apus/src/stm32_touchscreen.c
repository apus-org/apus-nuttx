/****************************************************************************
 * boards/apus/src/stm32_touchscreen.c
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
#include <errno.h>
#include <debug.h>

#include <nuttx/irq.h>
#include <nuttx/spi/spi.h>
#include <nuttx/input/ads7843e.h>

#include "stm32_gpio.h"
#include "stm32_spi.h"
#include "apus.h"

#ifndef CONFIG_STM32_SPI1
#  error "The touch controller needs CONFIG_STM32_SPI1"
#endif

/****************************************************************************
 * Private Types
 ****************************************************************************/

struct apus_tsc_s
{
  struct ads7843e_config_s config;
  xcpt_t handler;
};

/****************************************************************************
 * Private Function Prototypes
 ****************************************************************************/

static int  tsc_attach(struct ads7843e_config_s *state, xcpt_t isr);
static void tsc_enable(struct ads7843e_config_s *state, bool enable);
static void tsc_clear(struct ads7843e_config_s *state);
static bool tsc_busy(struct ads7843e_config_s *state);
static bool tsc_pendown(struct ads7843e_config_s *state);

/****************************************************************************
 * Private Data
 ****************************************************************************/

static struct apus_tsc_s g_tsc =
{
  .config =
  {
    .frequency = CONFIG_ADS7843E_FREQUENCY,
    .attach    = tsc_attach,
    .enable    = tsc_enable,
    .clear     = tsc_clear,
    .busy      = tsc_busy,
    .pendown   = tsc_pendown,
  },
};

/****************************************************************************
 * Private Functions
 ****************************************************************************/

static int tsc_attach(struct ads7843e_config_s *state, xcpt_t isr)
{
  struct apus_tsc_s *priv = (struct apus_tsc_s *)state;

  if (isr == NULL)
    {
      tsc_enable(state, false);
    }

  priv->handler = isr;
  return OK;
}

static void tsc_enable(struct ads7843e_config_s *state, bool enable)
{
  struct apus_tsc_s *priv = (struct apus_tsc_s *)state;
  irqstate_t flags;

  flags = enter_critical_section();

  if (enable && priv->handler != NULL)
    {
      stm32_gpiosetevent(GPIO_TP_INT, true, true, true, priv->handler,
                         NULL);
    }
  else
    {
      stm32_gpiosetevent(GPIO_TP_INT, false, false, false, NULL, NULL);
    }

  leave_critical_section(flags);
}

static void tsc_clear(struct ads7843e_config_s *state)
{
}

static bool tsc_busy(struct ads7843e_config_s *state)
{
  return false;
}

static bool tsc_pendown(struct ads7843e_config_s *state)
{
  return !stm32_gpioread(GPIO_TP_INT);
}

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: stm32_touchscreen_setup
 ****************************************************************************/

int stm32_touchscreen_setup(int minor)
{
  struct spi_dev_s *spi;

  stm32_configgpio(GPIO_TP_INT);

  spi = stm32_spibus_initialize(1);
  if (spi == NULL)
    {
      ierr("ERROR: Failed to get SPI1\n");
      return -ENODEV;
    }

  return ads7843e_register(spi, &g_tsc.config, minor);
}
