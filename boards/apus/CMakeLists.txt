# ##############################################################################
# boards/apus/CMakeLists.txt
#
# Copyright (c) 2026 Jakob Jungreuthmayer
#
# SPDX-License-Identifier: Apache-2.0
#
# ##############################################################################

add_subdirectory(src)

if(CONFIG_ARCH_BOARD_COMMON)
  add_subdirectory(${NUTTX_DIR}/boards/${CONFIG_ARCH}/${CONFIG_ARCH_CHIP}/common
                   board_common)
endif()
