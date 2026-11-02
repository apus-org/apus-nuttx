/****************************************************************************
 * boards/apus/src/apus.h
 *
 * Copyright (c) 2026 Jakob Jungreuthmayer
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 ****************************************************************************/

#ifndef __BOARDS_APUS_SRC_APUS_H
#define __BOARDS_APUS_SRC_APUS_H

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <nuttx/config.h>
#include <nuttx/compiler.h>

#include <stdint.h>

#include "stm32_gpio.h"

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

/* LEDs: active low */

#define GPIO_LED3       (GPIO_OUTPUT | GPIO_PUSHPULL | GPIO_SPEED_MEDIUM | \
                         GPIO_OUTPUT_SET | GPIO_PORTC | GPIO_PIN0)
#define GPIO_LED4       (GPIO_OUTPUT | GPIO_PUSHPULL | GPIO_SPEED_MEDIUM | \
                         GPIO_OUTPUT_SET | GPIO_PORTC | GPIO_PIN1)
#define GPIO_LED5       (GPIO_OUTPUT | GPIO_PUSHPULL | GPIO_SPEED_MEDIUM | \
                         GPIO_OUTPUT_SET | GPIO_PORTC | GPIO_PIN2)

/* Buttons: active high.  WAKE_UP (PA0) shares EXTI0 with USER0 (PD0), so it
 * has no interrupt.
 */

#define MIN_IRQBUTTON   BUTTON_USER0
#define MAX_IRQBUTTON   BUTTON_USER1
#define NUM_IRQBUTTONS  2

#define GPIO_BTN_USER0  (GPIO_INPUT | GPIO_PULLDOWN | GPIO_EXTI | \
                         GPIO_PORTD | GPIO_PIN0)
#define GPIO_BTN_USER1  (GPIO_INPUT | GPIO_PULLDOWN | GPIO_EXTI | \
                         GPIO_PORTD | GPIO_PIN1)
#define GPIO_BTN_WAKEUP (GPIO_INPUT | GPIO_PULLDOWN | GPIO_PORTA | GPIO_PIN0)

/* Active buzzer: sounds while low */

#define GPIO_BUZZER     (GPIO_OUTPUT | GPIO_PUSHPULL | GPIO_SPEED_LOW | \
                         GPIO_OUTPUT_SET | GPIO_PORTC | GPIO_PIN9)

/* I2C1 devices */

#define APUS_I2C_BUS        1
#define APUS_MCP79410_ADDR  0x6f
#define APUS_EEPROM_ADDR    0x57

/* RGB LED on TIM3 */

#define APUS_RGBLED_TIMER   3
#define APUS_RGBLED_RCHAN   1
#define APUS_RGBLED_GCHAN   2
#define APUS_RGBLED_BCHAN   3

/* ST7789V display on an 8080 8-bit bus: data PE0-7, control PE8-11 */

#define GPIO_LCD_DATA(n) (GPIO_OUTPUT | GPIO_PUSHPULL | GPIO_SPEED_HIGH | \
                          GPIO_OUTPUT_CLEAR | GPIO_PORTE | \
                          ((n) << GPIO_PIN_SHIFT))
#define GPIO_LCD_CS     (GPIO_OUTPUT | GPIO_PUSHPULL | GPIO_SPEED_HIGH | \
                         GPIO_OUTPUT_SET | GPIO_PORTE | GPIO_PIN8)
#define GPIO_LCD_DC     (GPIO_OUTPUT | GPIO_PUSHPULL | GPIO_SPEED_HIGH | \
                         GPIO_OUTPUT_SET | GPIO_PORTE | GPIO_PIN9)
#define GPIO_LCD_WR     (GPIO_OUTPUT | GPIO_PUSHPULL | GPIO_SPEED_HIGH | \
                         GPIO_OUTPUT_SET | GPIO_PORTE | GPIO_PIN10)
#define GPIO_LCD_RD     (GPIO_OUTPUT | GPIO_PUSHPULL | GPIO_SPEED_HIGH | \
                         GPIO_OUTPUT_SET | GPIO_PORTE | GPIO_PIN11)
#define GPIO_LCD_RST    (GPIO_OUTPUT | GPIO_PUSHPULL | GPIO_SPEED_LOW | \
                         GPIO_OUTPUT_CLEAR | GPIO_PORTF | GPIO_PIN10)

/* XPT2046 touch controller on SPI1: chip select and pen interrupt */

#define GPIO_TP_CS      (GPIO_OUTPUT | GPIO_PUSHPULL | GPIO_SPEED_HIGH | \
                         GPIO_OUTPUT_SET | GPIO_PORTE | GPIO_PIN12)
#define GPIO_TP_INT     (GPIO_INPUT | GPIO_PULLUP | GPIO_EXTI | \
                         GPIO_PORTF | GPIO_PIN2)

/* Arduino D10: SPI2 chip select */

#define GPIO_ARDUINO_CS (GPIO_OUTPUT | GPIO_PUSHPULL | GPIO_SPEED_HIGH | \
                         GPIO_OUTPUT_SET | GPIO_PORTB | GPIO_PIN12)

/****************************************************************************
 * Public Function Prototypes
 ****************************************************************************/

#ifndef __ASSEMBLY__

int stm32_bringup(void);

#ifdef CONFIG_RGBLED
int stm32_rgbled_setup(void);
#endif

#if defined(CONFIG_ADC) && defined(CONFIG_STM32_ADC1)
int stm32_adc_setup(void);
#endif

#ifdef CONFIG_DEV_GPIO
int stm32_gpio_initialize(void);
#endif

#ifdef CONFIG_STM32_SPI
void stm32_spidev_initialize(void);
#endif

#ifdef CONFIG_INPUT_ADS7843E
int stm32_touchscreen_setup(int minor);
#endif

#endif /* __ASSEMBLY__ */
#endif /* __BOARDS_APUS_SRC_APUS_H */
