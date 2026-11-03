/****************************************************************************
 * boards/apus/src/stm32_lcd.c
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
#include <debug.h>

#include <nuttx/arch.h>
#include <nuttx/board.h>
#include <nuttx/spi/spi.h>
#include <nuttx/lcd/lcd.h>
#include <nuttx/lcd/st7789.h>

#include "arm_internal.h"
#include "hardware/stm32_gpio.h"
#include "stm32_gpio.h"
#include "apus.h"

/* The ST7789 driver speaks SPI; this board presents the 8080 bus as an SPI
 * device: select is CS, cmddata is DC, every word is one or two WR strobes.
 */

#ifndef CONFIG_SPI_CMDDATA
#  error "The display bus needs CONFIG_SPI_CMDDATA"
#endif

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

#define LCD_DATA_MASK   0x00ff
#define LCD_CS          (1 << 8)
#define LCD_DC          (1 << 9)
#define LCD_WR          (1 << 10)
#define LCD_RD          (1 << 11)

#define LCD_SET(bits)   putreg32((bits), STM32_GPIOE_BSRR)
#define LCD_CLR(bits)   putreg32((uint32_t)(bits) << 16, STM32_GPIOE_BSRR)

/****************************************************************************
 * Private Types
 ****************************************************************************/

struct lcdbus_s
{
  struct spi_dev_s spi;
  int nbits;
};

/****************************************************************************
 * Private Function Prototypes
 ****************************************************************************/

static int      lcdbus_lock(struct spi_dev_s *dev, bool lock);
static void     lcdbus_select(struct spi_dev_s *dev, uint32_t devid,
                              bool selected);
static uint32_t lcdbus_setfrequency(struct spi_dev_s *dev,
                                    uint32_t frequency);
static void     lcdbus_setmode(struct spi_dev_s *dev, enum spi_mode_e mode);
static void     lcdbus_setbits(struct spi_dev_s *dev, int nbits);
static uint8_t  lcdbus_status(struct spi_dev_s *dev, uint32_t devid);
static int      lcdbus_cmddata(struct spi_dev_s *dev, uint32_t devid,
                               bool cmd);
static uint32_t lcdbus_send(struct spi_dev_s *dev, uint32_t wd);
#ifdef CONFIG_SPI_EXCHANGE
static void     lcdbus_exchange(struct spi_dev_s *dev, const void *txbuffer,
                                void *rxbuffer, size_t nwords);
#else
static void     lcdbus_sndblock(struct spi_dev_s *dev, const void *buffer,
                                size_t nwords);
static void     lcdbus_recvblock(struct spi_dev_s *dev, void *buffer,
                                 size_t nwords);
#endif

/****************************************************************************
 * Private Data
 ****************************************************************************/

static const struct spi_ops_s g_lcdbus_ops =
{
  .lock         = lcdbus_lock,
  .select       = lcdbus_select,
  .setfrequency = lcdbus_setfrequency,
  .setmode      = lcdbus_setmode,
  .setbits      = lcdbus_setbits,
  .status       = lcdbus_status,
  .cmddata      = lcdbus_cmddata,
  .send         = lcdbus_send,
#ifdef CONFIG_SPI_EXCHANGE
  .exchange     = lcdbus_exchange,
#else
  .sndblock     = lcdbus_sndblock,
  .recvblock    = lcdbus_recvblock,
#endif
};

static struct lcdbus_s g_lcdbus =
{
  .spi   =
  {
    .ops = &g_lcdbus_ops,
  },
  .nbits = 8,
};

static struct lcd_dev_s *g_lcd;

/****************************************************************************
 * Private Functions
 ****************************************************************************/

static inline void lcdbus_write8(uint8_t value)
{
  putreg32(((uint32_t)(~value & LCD_DATA_MASK) << 16) | value,
           STM32_GPIOE_BSRR);
  LCD_CLR(LCD_WR);
  LCD_SET(LCD_WR);
}

static inline uint8_t lcdbus_read8(void)
{
  uint8_t value;

  LCD_CLR(LCD_RD);
  up_udelay(1);
  value = getreg32(STM32_GPIOE_IDR) & LCD_DATA_MASK;
  LCD_SET(LCD_RD);
  return value;
}

static void lcdbus_datadir(bool input)
{
  uint32_t regval = getreg32(STM32_GPIOE_MODER) & ~0xffff;

  if (!input)
    {
      regval |= 0x5555;
    }

  putreg32(regval, STM32_GPIOE_MODER);
}

static void lcdbus_writeword(int nbits, uint16_t wd)
{
  if (nbits > 8)
    {
      lcdbus_write8(wd >> 8);
    }

  lcdbus_write8(wd);
}

static int lcdbus_lock(struct spi_dev_s *dev, bool lock)
{
  return OK;
}

static void lcdbus_select(struct spi_dev_s *dev, uint32_t devid,
                          bool selected)
{
  if (selected)
    {
      LCD_CLR(LCD_CS);
    }
  else
    {
      LCD_SET(LCD_CS);
    }
}

static uint32_t lcdbus_setfrequency(struct spi_dev_s *dev,
                                    uint32_t frequency)
{
  return frequency;
}

static void lcdbus_setmode(struct spi_dev_s *dev, enum spi_mode_e mode)
{
}

static void lcdbus_setbits(struct spi_dev_s *dev, int nbits)
{
  ((struct lcdbus_s *)dev)->nbits = nbits;
}

static uint8_t lcdbus_status(struct spi_dev_s *dev, uint32_t devid)
{
  return 0;
}

static int lcdbus_cmddata(struct spi_dev_s *dev, uint32_t devid, bool cmd)
{
  if (cmd)
    {
      LCD_CLR(LCD_DC);
    }
  else
    {
      LCD_SET(LCD_DC);
    }

  return OK;
}

static uint32_t lcdbus_send(struct spi_dev_s *dev, uint32_t wd)
{
  lcdbus_writeword(((struct lcdbus_s *)dev)->nbits, wd);
  return 0;
}

#ifdef CONFIG_SPI_EXCHANGE
static void lcdbus_exchange(struct spi_dev_s *dev, const void *txbuffer,
                            void *rxbuffer, size_t nwords)
{
  int nbits = ((struct lcdbus_s *)dev)->nbits;
  size_t i;

  if (txbuffer != NULL)
    {
      for (i = 0; i < nwords; i++)
        {
          lcdbus_writeword(nbits, nbits > 8 ?
                           ((const uint16_t *)txbuffer)[i] :
                           ((const uint8_t *)txbuffer)[i]);
        }
    }
  else if (rxbuffer != NULL)
    {
      lcdbus_datadir(true);

      for (i = 0; i < nwords; i++)
        {
          if (nbits > 8)
            {
              uint16_t wd = lcdbus_read8() << 8;

              ((uint16_t *)rxbuffer)[i] = wd | lcdbus_read8();
            }
          else
            {
              ((uint8_t *)rxbuffer)[i] = lcdbus_read8();
            }
        }

      lcdbus_datadir(false);
    }
}
#else
static void lcdbus_sndblock(struct spi_dev_s *dev, const void *buffer,
                            size_t nwords)
{
  int nbits = ((struct lcdbus_s *)dev)->nbits;
  size_t i;

  for (i = 0; i < nwords; i++)
    {
      lcdbus_writeword(nbits, nbits > 8 ? ((const uint16_t *)buffer)[i] :
                                          ((const uint8_t *)buffer)[i]);
    }
}

static void lcdbus_recvblock(struct spi_dev_s *dev, void *buffer,
                             size_t nwords)
{
  int nbits = ((struct lcdbus_s *)dev)->nbits;
  size_t i;

  lcdbus_datadir(true);

  for (i = 0; i < nwords; i++)
    {
      if (nbits > 8)
        {
          uint16_t wd = lcdbus_read8() << 8;

          ((uint16_t *)buffer)[i] = wd | lcdbus_read8();
        }
      else
        {
          ((uint8_t *)buffer)[i] = lcdbus_read8();
        }
    }

  lcdbus_datadir(false);
}
#endif

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: board_lcd_initialize
 ****************************************************************************/

int board_lcd_initialize(void)
{
  int i;

  for (i = 0; i < 8; i++)
    {
      stm32_configgpio(GPIO_LCD_DATA(i));
    }

  stm32_configgpio(GPIO_LCD_CS);
  stm32_configgpio(GPIO_LCD_DC);
  stm32_configgpio(GPIO_LCD_WR);
  stm32_configgpio(GPIO_LCD_RD);
  stm32_configgpio(GPIO_LCD_RST);

  up_mdelay(10);
  stm32_gpiowrite(GPIO_LCD_RST, true);
  up_mdelay(120);

  return OK;
}

/****************************************************************************
 * Name: board_lcd_getdev
 ****************************************************************************/

struct lcd_dev_s *board_lcd_getdev(int devno)
{
  if (devno != 0)
    {
      return NULL;
    }

  if (g_lcd == NULL)
    {
      g_lcd = st7789_lcdinitialize(&g_lcdbus.spi);
      if (g_lcd == NULL)
        {
          lcderr("ERROR: Failed to initialize the ST7789\n");
        }
    }

  return g_lcd;
}

/****************************************************************************
 * Name: board_lcd_uninitialize
 ****************************************************************************/

void board_lcd_uninitialize(void)
{
  if (g_lcd != NULL)
    {
      g_lcd->setpower(g_lcd, 0);
    }
}
