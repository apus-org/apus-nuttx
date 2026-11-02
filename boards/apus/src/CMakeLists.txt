# ##############################################################################
# boards/apus/src/CMakeLists.txt
#
# Copyright (c) 2026 Jakob Jungreuthmayer
#
# SPDX-License-Identifier: Apache-2.0
#
# ##############################################################################

set(SRCS stm32_boot.c stm32_bringup.c)

if(CONFIG_ARCH_BOARD_STM32_CUSTOM_CLOCKCONFIG)
  list(APPEND SRCS stm32_clockconfig.c)
endif()

if(CONFIG_ARCH_LEDS)
  list(APPEND SRCS stm32_autoleds.c)
else()
  list(APPEND SRCS stm32_userleds.c)
endif()

if(CONFIG_STM32_USB)
  list(APPEND SRCS stm32_usb.c)
endif()

if(CONFIG_DEV_GPIO)
  list(APPEND SRCS stm32_gpio.c)
endif()

if(CONFIG_STM32_ADC1)
  list(APPEND SRCS stm32_adc.c)
endif()

if(CONFIG_RGBLED)
  list(APPEND SRCS stm32_rgbled.c)
endif()

if(CONFIG_ARCH_BUTTONS)
  list(APPEND SRCS stm32_buttons.c)
endif()

target_sources(board PRIVATE ${SRCS})

set_property(GLOBAL PROPERTY LD_SCRIPT "${NUTTX_BOARD_DIR}/scripts/flash.ld")
