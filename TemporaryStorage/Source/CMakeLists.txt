cmake_minimum_required(VERSION 3.0.0)

set (TOOLCHAIN_PREFIX C:/nanoFramework_Tools/GNU_ARM_Toolchain/8-2018-q4_major)

message(STATUS "ToolChain_Prefix for src is: ${TOOLCHAIN_PREFIX}")

#Added by RoSchmi
set (CPU cortex-m0)

set (LPC11XX_MODEL lpc1113)

set (NVIC_EnableIRQ true)


##########################################
# set default toolchain to GCC
set(TOOLCHAIN GCC)
# have it lower case too for file names
string(TOLOWER TOOLCHAIN_LOWER ${TOOLCHAIN})
##########################################

include_directories ("${PROJECT_SOURCE_DIR}/inc")
include_directories ("${PROJECT_SOURCE_DIR}/inc/lpc11xx")
include_directories ("${PROJECT_SOURCE_DIR}/src")
# add_library(src lpc11xx_crt0.c lpc11xx_crp.c lpc11xx_iap.c lpc11xx_pll.c lpclib_assert.c)
add_library(src lpc11xx_crt0.c lpc11xx_iap.c lpc11xx_pll.c lpclib_assert.c)