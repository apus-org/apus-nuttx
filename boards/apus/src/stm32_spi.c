/****************************************************************************
 * boards/apus/src/stm32_spi.c
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
#include <errno.h>

#include <nuttx/spi/spi.h>

#include "stm32_gpio.h"
#include "stm32_spi.h"
#include "apus.h"

#ifdef CONFIG_STM32_SPI

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: stm32_spidev_initialize
 ****************************************************************************/

void stm32_spidev_initialize(void)
{
#ifdef CONFIG_STM32_SPI1
  stm32_configgpio(GPIO_TP_CS);
#endif

#ifdef CONFIG_STM32_SPI2
  stm32_configgpio(GPIO_ARDUINO_CS);
#endif
}

/****************************************************************************
 * Name: stm32_spi1select and stm32_spi1status
 ****************************************************************************/

#ifdef CONFIG_STM32_SPI1
void stm32_spi1select(struct spi_dev_s *dev, uint32_t devid, bool selected)
{
  if (devid == SPIDEV_TOUCHSCREEN(0))
    {
      stm32_gpiowrite(GPIO_TP_CS, !selected);
    }
}

uint8_t stm32_spi1status(struct spi_dev_s *dev, uint32_t devid)
{
  return 0;
}

#ifdef CONFIG_SPI_CMDDATA
int stm32_spi1cmddata(struct spi_dev_s *dev, uint32_t devid, bool cmd)
{
  return -ENODEV;
}
#endif
#endif /* CONFIG_STM32_SPI1 */

/****************************************************************************
 * Name: stm32_spi2select and stm32_spi2status
 *
 * Description:
 *   D10 is the only chip select on the Arduino header, so it is asserted
 *   for every device on SPI2.
 *
 ****************************************************************************/

#ifdef CONFIG_STM32_SPI2
void stm32_spi2select(struct spi_dev_s *dev, uint32_t devid, bool selected)
{
  stm32_gpiowrite(GPIO_ARDUINO_CS, !selected);
}

uint8_t stm32_spi2status(struct spi_dev_s *dev, uint32_t devid)
{
  return 0;
}

#ifdef CONFIG_SPI_CMDDATA
int stm32_spi2cmddata(struct spi_dev_s *dev, uint32_t devid, bool cmd)
{
  return cmd ? -ENODEV : OK;
}
#endif
#endif /* CONFIG_STM32_SPI2 */

#endif /* CONFIG_STM32_SPI */
