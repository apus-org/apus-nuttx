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

if(CONFIG_ARCH_BUTTONS)
  list(APPEND SRCS stm32_buttons.c)
endif()

target_sources(board PRIVATE ${SRCS})

set_property(GLOBAL PROPERTY LD_SCRIPT "${NUTTX_BOARD_DIR}/scripts/flash.ld")
