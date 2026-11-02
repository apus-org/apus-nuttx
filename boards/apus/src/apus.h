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

/* I2C1 devices */

#define APUS_I2C_BUS        1
#define APUS_MCP79410_ADDR  0x6f
#define APUS_EEPROM_ADDR    0x57

/* RGB LED on TIM3 */

#define APUS_RGBLED_TIMER   3
#define APUS_RGBLED_RCHAN   1
#define APUS_RGBLED_GCHAN   2
#define APUS_RGBLED_BCHAN   3

/****************************************************************************
 * Public Function Prototypes
 ****************************************************************************/

#ifndef __ASSEMBLY__

int stm32_bringup(void);

#ifdef CONFIG_RGBLED
int stm32_rgbled_setup(void);
#endif

#endif /* __ASSEMBLY__ */
#endif /* __BOARDS_APUS_SRC_APUS_H */
