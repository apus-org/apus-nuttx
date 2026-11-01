/****************************************************************************
 * boards/apus/include/board.h
 *
 * Copyright (c) 2026 Jakob Jungreuthmayer
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 ****************************************************************************/

#ifndef __BOARDS_APUS_INCLUDE_BOARD_H
#define __BOARDS_APUS_INCLUDE_BOARD_H

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <nuttx/config.h>

#ifndef __ASSEMBLY__
#  include <stdint.h>
#endif

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

/* Clocking *****************************************************************/

#define STM32_BOARD_XTAL         8000000ul        /* X1, 8 MHz */

#define STM32_HSI_FREQUENCY      8000000ul
#define STM32_HSI14_FREQUENCY    14000000ul
#define STM32_HSI48_FREQUENCY    48000000ul
#define STM32_HSE_FREQUENCY      STM32_BOARD_XTAL
#define STM32_LSI_FREQUENCY      40000
#define STM32_LSE_FREQUENCY      32768

/* PLL: HSE/1 x 6 = 48 MHz (CONFIG_ARCH_BOARD_STM32_CUSTOM_CLOCKCONFIG) */

#define STM32_CFGR_PLLSRC        RCC_CFGR_PLLSRC_HSE_PREDIV
#define STM32_CFGR2_PREDIV       RCC_CFGR2_PREDIV1d1
#define STM32_PLLSRC_FREQUENCY   STM32_HSE_FREQUENCY
#define STM32_CFGR_PLLMUL        RCC_CFGR_PLLMUL_CLKx6
#define STM32_PLL_FREQUENCY      (6 * STM32_PLLSRC_FREQUENCY)

#define STM32_SYSCLK_SW          RCC_CFGR_SW_PLL
#define STM32_SYSCLK_SWS         RCC_CFGR_SWS_PLL
#define STM32_SYSCLK_FREQUENCY   STM32_PLL_FREQUENCY

#define STM32_RCC_CFGR_HPRE      RCC_CFGR_HPRE_SYSCLK
#define STM32_HCLK_FREQUENCY     STM32_SYSCLK_FREQUENCY

#define STM32_RCC_CFGR_PPRE1     RCC_CFGR_PPRE1_HCLK
#define STM32_PCLK1_FREQUENCY    (STM32_HCLK_FREQUENCY)

#define STM32_RCC_CFGR_PPRE2     RCC_CFGR_PPRE2_HCLK
#define STM32_PCLK2_FREQUENCY    STM32_HCLK_FREQUENCY
#define STM32_APB2_CLKIN         (STM32_PCLK2_FREQUENCY)

#define STM32_TIM1_CLKIN         (STM32_PCLK1_FREQUENCY)
#define STM32_TIM2_CLKIN         (STM32_PCLK1_FREQUENCY)
#define STM32_TIM3_CLKIN         (STM32_PCLK1_FREQUENCY)
#define STM32_TIM6_CLKIN         (STM32_PCLK1_FREQUENCY)
#define STM32_TIM7_CLKIN         (STM32_PCLK1_FREQUENCY)
#define STM32_TIM14_CLKIN        (STM32_PCLK1_FREQUENCY)
#define STM32_TIM15_CLKIN        (STM32_PCLK1_FREQUENCY)
#define STM32_TIM16_CLKIN        (STM32_PCLK1_FREQUENCY)
#define STM32_TIM17_CLKIN        (STM32_PCLK1_FREQUENCY)

/* LEDs *********************************************************************/

/* LED index values for use with board_userled() */

#define BOARD_LED3               0
#define BOARD_LED4               1
#define BOARD_LED5               2
#define BOARD_NLEDS              3

/* LED bits for use with board_userled_all() */

#define BOARD_LED3_BIT           (1 << BOARD_LED3)
#define BOARD_LED4_BIT           (1 << BOARD_LED4)
#define BOARD_LED5_BIT           (1 << BOARD_LED5)

/* With CONFIG_ARCH_LEDS:
 *
 *   SYMBOL              Value  LED3  LED4  LED5
 *   LED_STARTED           0    off   off   off
 *   LED_HEAPALLOCATE      0    off   off   off
 *   LED_IRQSENABLED       0    off   off   off
 *   LED_STACKCREATED      1    on    off   off
 *   LED_INIRQ             2    -     on    -
 *   LED_SIGNAL            2    -     on    -
 *   LED_ASSERTION         2    -     on    -
 *   LED_PANIC             3    -     -     blinking
 */

#define LED_STARTED              0
#define LED_HEAPALLOCATE         0
#define LED_IRQSENABLED          0
#define LED_STACKCREATED         1
#define LED_INIRQ                2
#define LED_SIGNAL               2
#define LED_ASSERTION            2
#define LED_PANIC                3

/* Buttons ******************************************************************/

#define BUTTON_USER0             0
#define BUTTON_USER1             1
#define BUTTON_WAKEUP            2
#define NUM_BUTTONS              3

#define BUTTON_USER0_BIT         (1 << BUTTON_USER0)
#define BUTTON_USER1_BIT         (1 << BUTTON_USER1)
#define BUTTON_WAKEUP_BIT        (1 << BUTTON_WAKEUP)

/* Alternate Pin Functions **************************************************/

/* USART3: VCOM of the on-board ST-Link */

#define GPIO_USART3_TX           (GPIO_USART3_TX_1 | GPIO_SPEED_HIGH)  /* PD8 */
#define GPIO_USART3_RX           (GPIO_USART3_RX_1 | GPIO_SPEED_HIGH)  /* PD9 */

/* USART1: Arduino D0/D1 */

#define GPIO_USART1_TX           (GPIO_USART1_TX_2 | GPIO_SPEED_HIGH)  /* PA9 */
#define GPIO_USART1_RX           (GPIO_USART1_RX_2 | GPIO_SPEED_HIGH)  /* PA10 */

#endif /* __BOARDS_APUS_INCLUDE_BOARD_H */
