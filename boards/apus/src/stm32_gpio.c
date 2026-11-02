/****************************************************************************
 * boards/apus/src/stm32_gpio.c
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

#include <nuttx/ioexpander/gpio.h>

#include "stm32_gpio.h"
#include "apus.h"

/****************************************************************************
 * Private Function Prototypes
 ****************************************************************************/

static int buzzer_read(struct gpio_dev_s *dev, bool *value);
static int buzzer_write(struct gpio_dev_s *dev, bool value);

/****************************************************************************
 * Private Data
 ****************************************************************************/

static const struct gpio_operations_s g_buzzer_ops =
{
  .go_read   = buzzer_read,
  .go_write  = buzzer_write,
  .go_attach = NULL,
  .go_enable = NULL,
};

static struct gpio_dev_s g_buzzer =
{
  .gp_pintype = GPIO_OUTPUT_PIN,
  .gp_ops     = &g_buzzer_ops,
};

/****************************************************************************
 * Private Functions
 ****************************************************************************/

static int buzzer_read(struct gpio_dev_s *dev, bool *value)
{
  *value = !stm32_gpioread(GPIO_BUZZER);
  return OK;
}

static int buzzer_write(struct gpio_dev_s *dev, bool value)
{
  stm32_gpiowrite(GPIO_BUZZER, !value);
  return OK;
}

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: stm32_gpio_initialize
 ****************************************************************************/

int stm32_gpio_initialize(void)
{
  stm32_configgpio(GPIO_BUZZER);
  return gpio_pin_register_byname(&g_buzzer, "buzzer");
}
