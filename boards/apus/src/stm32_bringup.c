/****************************************************************************
 * boards/apus/src/stm32_bringup.c
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

#include <sys/types.h>
#include <syslog.h>

#include <nuttx/clock.h>
#include <nuttx/fs/fs.h>
#include <nuttx/i2c/i2c_master.h>
#include <nuttx/eeprom/eeprom.h>
#include <nuttx/timers/mcp794xx.h>
#include <nuttx/usb/cdcacm.h>

#include "stm32_i2c.h"
#include "apus.h"

#ifdef CONFIG_SENSORS_LM75
#  include "stm32_lm75.h"
#endif

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

#if defined(CONFIG_CDCACM) && !defined(CONFIG_CDCACM_COMPOSITE)
#  define HAVE_CDCACM 1
#endif

/****************************************************************************
 * Private Functions
 ****************************************************************************/

#ifdef CONFIG_STM32_I2C1
static void stm32_i2c1_initialize(void)
{
  struct i2c_master_s *i2c;
  int ret;

  i2c = stm32_i2cbus_initialize(APUS_I2C_BUS);
  if (i2c == NULL)
    {
      syslog(LOG_ERR, "ERROR: Failed to get I2C%d\n", APUS_I2C_BUS);
      return;
    }

#ifdef CONFIG_I2C_DRIVER
  ret = i2c_register(i2c, APUS_I2C_BUS);
  if (ret < 0)
    {
      syslog(LOG_ERR, "ERROR: Failed to register I2C%d: %d\n",
             APUS_I2C_BUS, ret);
    }
#endif

#ifdef CONFIG_RTC_MCP794XX
  ret = mcp794xx_rtc_initialize(i2c, APUS_MCP79410_ADDR);
  if (ret < 0)
    {
      syslog(LOG_ERR, "ERROR: Failed to initialize MCP79410: %d\n", ret);
    }
  else
    {
      clock_synchronize(NULL);
    }
#endif

#ifdef CONFIG_I2C_EE_24XX
  ret = ee24xx_initialize(i2c, APUS_EEPROM_ADDR, "/dev/eeprom0",
                          EEPROM_24XX01, false);
  if (ret < 0)
    {
      syslog(LOG_ERR, "ERROR: Failed to initialize EEPROM: %d\n", ret);
    }
#endif

#ifdef CONFIG_SENSORS_LM75
  ret = board_lm75_initialize(0, APUS_I2C_BUS);
  if (ret < 0)
    {
      syslog(LOG_ERR, "ERROR: Failed to initialize MCP9803: %d\n", ret);
    }
#endif

  UNUSED(ret);
}
#endif

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: up_rtc_initialize
 *
 * Description:
 *   clock_initialize() calls this even when the only RTC is external.
 *
 ****************************************************************************/

#if defined(CONFIG_RTC) && defined(CONFIG_RTC_EXTERNAL)
int up_rtc_initialize(void)
{
  return OK;
}
#endif

/****************************************************************************
 * Name: stm32_bringup
 ****************************************************************************/

int stm32_bringup(void)
{
  int ret = OK;

#ifdef CONFIG_FS_PROCFS
  ret = nx_mount(NULL, "/proc", "procfs", 0, NULL);
  if (ret < 0)
    {
      syslog(LOG_ERR, "ERROR: Failed to mount procfs at /proc: %d\n", ret);
    }
#endif

#ifdef CONFIG_STM32_I2C1
  stm32_i2c1_initialize();
#endif

#ifdef CONFIG_RGBLED
  ret = stm32_rgbled_setup();
  if (ret < 0)
    {
      syslog(LOG_ERR, "ERROR: Failed to register RGB LED: %d\n", ret);
    }
#endif

#if defined(CONFIG_ADC) && defined(CONFIG_STM32_ADC1)
  ret = stm32_adc_setup();
  if (ret < 0)
    {
      syslog(LOG_ERR, "ERROR: Failed to register ADC: %d\n", ret);
    }
#endif

#ifdef CONFIG_DEV_GPIO
  ret = stm32_gpio_initialize();
  if (ret < 0)
    {
      syslog(LOG_ERR, "ERROR: Failed to register GPIOs: %d\n", ret);
    }
#endif

#ifdef HAVE_CDCACM
  ret = cdcacm_initialize(0, NULL);
  if (ret < 0)
    {
      syslog(LOG_ERR, "ERROR: Failed to register CDC ACM: %d\n", ret);
    }
#endif

  UNUSED(ret);
  return OK;
}
