# RoSchmi 2019/09/08
# CMakeLists.txt for Uart and blinky example 

# If there are problems with CMAKE_SYSTEM_NAME Generic and ld option --major-image-version'          
# then delete the entire build folder                                                                
# https://stackoverflow.com/questions/23120137/cmake-cross-compiling-generate-invalid-linker-options 


cmake_minimum_required(VERSION 3.0.0)
include(CMakeToolsHelpers OPTIONAL)


# Take some setting from cmake-variants.json
# Delete CMakeCache.txt and the other files with settings 
# in the build folder to actualize for new values


set(TOOLCHAIN_PREFIX ${TOOLCHAIN_PREFIX})
set(BUILD_VERSION ${BUILD_VERSION})
set(LINKERSCRIPT_PATH ${LINKERSCRIPT_PATH})
set(LINKERSCRIPT_NAME ${LINKERSCRIPT_NAME})

message(STATUS "ToolChain_Prefix is: ${TOOLCHAIN_PREFIX}")

set(CMAKE_C_COMPILER "${TOOLCHAIN_PREFIX}/arm-none-eabi-gcc.exe")
set(CMAKE_CXX_COMPILER "${TOOLCHAIN_PREFIX}/arm-none-eabi-c++.exe")

# CMAKE_SYSTEM_NAME must be set to Generic 
# If variable is not set to Generic, the linker will stop with failure because of:
# failure with linker option --major-image-version'

set(CMAKE_SYSTEM_NAME Generic)

# This statement is needed to prevent failure of compiler check
set (CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)

# ENABLE ASM
# https://gitlab.kitware.com/cmake/cmake/issues/18406

if(CMAKE_C_COMPILER)
        set(CMAKE_ASM${ASM_DIALECT}_COMPILER_LIST ${CMAKE_C_COMPILER})
      elseif(CMAKE_CXX_COMPILER)
        set(CMAKE_ASM${ASM_DIALECT}_COMPILER_LIST ${CMAKE_CXX_COMPILER})
endif()

ENABLE_LANGUAGE("ASM")

# option (USE_MYMATH "Use tutorial provided math implementation" ON)

# include(CTest)
# enable_testing()

# set(CPACK_PROJECT_NAME ${PROJECT_NAME})
# set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
# include(CPack)

# the following prevents launchin a build in the source tree
set(CMAKE_DISABLE_SOURCE_CHANGES ON)
set(CMAKE_DISABLE_IN_SOURCE_BUILD ON)

# fatal error and message explaining this
if (CMAKE_SOURCE_DIR STREQUAL CMAKE_BINARY_DIR)
    message(STATUS "\n-- ###############################################\n")
    message(STATUS "Please run the build outside of the source tree.\n\n")
    message(STATUS "Hint: create a 'build' folder and run CMake from there..")
    message(STATUS "###############################################\n\n")
    message(FATAL_ERROR "Build launched in the source tree.")
endif()
#########################################


########################################################
# path to local CMake modules
set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/CMake/Modules)
########################################################


######################################################
# set build type to release if not specified otherwise
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Debug")
endif()
######################################################

######################################################
# set time stamp of build
string(TIMESTAMP BUILD_TIMESTAMP UTC)
######################################################

##########################################
# set default toolchain to GCC
set(TOOLCHAIN GCC)
# have it lower case too for file names
string(TOLOWER TOOLCHAIN_LOWER ${TOOLCHAIN})
##########################################

########################################################
# check availability of hex2dfu tool if specified
# only relevant if this is running on a Windows machine
if(WIN32)
    if(NOT "${TOOL_HEX2DFU_PREFIX}" STREQUAL "")
        if(NOT EXISTS ${TOOL_HEX2DFU_PREFIX}/hex2dfu.exe)
            message(STATUS "")
            message(STATUS "Couldn't find the hex2dfu tool at the specified path: ${TOOL_HEX2DFU_PREFIX}/hex2dfu.exe")
            message(STATUS "Make sure that the CMake option TOOL_HEX2DFU_PREFIX has the correct path.")
            message(STATUS "If you don't have this tool download it from https://github.com/nanoframework/nf-tools/releases")
            message(STATUS "")
            message(FATAL_ERROR "hex2dfu tool not found")
        else()
            set(HEX2DFU_TOOL_AVAILABLE TRUE CACHE INTERNAL "hex2dfu tool available")
        endif()
    endif()
endif()

# set default version
if(NOT BUILD_VERSION)
    set(BUILD_VERSION 0.0.0.0)
endif()
project(LPC1113_GCC_VSCode_Uart VERSION ${BUILD_VERSION})

include_directories ("${PROJECT_SOURCE_DIR}/CMSIS/Include")
include_directories ("${PROJECT_SOURCE_DIR}/CMSIS")
# include_directories ("${PROJECT_SOURCE_DIR}/CMakeVariants")
include_directories ("${PROJECT_SOURCE_DIR}/src")
include_directories ("${PROJECT_SOURCE_DIR}/Startup")
include_directories ("${PROJECT_SOURCE_DIR}/inc")

# ************  The following lines are not used in this example  ************
# add_subdirectory (src)
# set (EXTRA_LIBS ${SRC_LIBS} src)

# should we use our own math functions?
# if (USE_MYMATH)
# include_directories ("${PROJECT_SOURCE_DIR}/MathFunctions")
# add_subdirectory (MathFunctions)
# set (EXTRA_LIBS ${EXTRA_LIBS} MathFunctions)

# endif (USE_MYMATH)

#+++++++++++++++++  added +++++++++++++++++++++++
# from the example in the official CMake Tutorial
# configure a header file to pass some of the CMake settings
# to the source code
#
# configure_file (
#  "${PROJECT_SOURCE_DIR}/LPC1113_Test_2Config.h.in"
#  "${PROJECT_BINARY_DIR}/LPC1113_Test_2Config.h"
#  )
 
# add the binary tree to the search path for include files

# include_directories("${PROJECT_BINARY_DIR}")

#******************** end added *************************

SET(CMAKE_ASM_FLAGS_DEBUG "${CMAKE_ASM_FLAGS_DEBUG} -DDEBUG")

SET(CMAKE_ASM_FLAGS_DEBUG "${CMAKE_ASM_FLAGS_DEBUG} -D__STARTUP_CLEAR_BSS")

SET(CMAKE_ASM_FLAGS_DEBUG "${CMAKE_ASM_FLAGS_DEBUG} -g")

SET(CMAKE_ASM_FLAGS_DEBUG "${CMAKE_ASM_FLAGS_DEBUG} -Wall")

SET(CMAKE_ASM_FLAGS_DEBUG "${CMAKE_ASM_FLAGS_DEBUG} -fno-common")

SET(CMAKE_ASM_FLAGS_DEBUG "${CMAKE_ASM_FLAGS_DEBUG} -ffunction-sections")

SET(CMAKE_ASM_FLAGS_DEBUG "${CMAKE_ASM_FLAGS_DEBUG} -fdata-sections")

# SET(CMAKE_ASM_FLAGS_DEBUG "${CMAKE_ASM_FLAGS_DEBUG} -ffreestanding")

# SET(CMAKE_ASM_FLAGS_DEBUG "${CMAKE_ASM_FLAGS_DEBUG} -fno-builtin")

SET(CMAKE_ASM_FLAGS_DEBUG "${CMAKE_ASM_FLAGS_DEBUG} -mthumb")

SET(CMAKE_ASM_FLAGS_DEBUG "${CMAKE_ASM_FLAGS_DEBUG} -mapcs")

# SET(CMAKE_ASM_FLAGS_DEBUG "${CMAKE_ASM_FLAGS_DEBUG} -std=gnu99")

SET(CMAKE_ASM_FLAGS_DEBUG "${CMAKE_ASM_FLAGS_DEBUG} -mcpu=cortex-m0")

# SET(CMAKE_ASM_FLAGS_DEBUG "${CMAKE_ASM_FLAGS_DEBUG} -mfloat-abi=soft")

# SET(CMAKE_ASM_FLAGS_RELEASE "${CMAKE_ASM_FLAGS_RELEASE} -DNDEBUG")

# SET(CMAKE_ASM_FLAGS_RELEASE "${CMAKE_ASM_FLAGS_RELEASE} -D__STARTUP_CLEAR_BSS")

# SET(CMAKE_ASM_FLAGS_RELEASE "${CMAKE_ASM_FLAGS_RELEASE} -Wall")

# SET(CMAKE_ASM_FLAGS_RELEASE "${CMAKE_ASM_FLAGS_RELEASE} -mthumb")


SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DDEBUG")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DPRINTF_FLOAT_ENABLE=0")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DSCANF_FLOAT_ENABLE=0")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -g")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -Wall")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -mthumb")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -mcpu=cortex-m0")

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -Os")


SET(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} -Wl,-gc-sections")

SET(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} -lgcc")

SET(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} -lc")

SET(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} -lnosys")

SET(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} -ffunction-sections")

SET(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} -fdata-sections")

SET(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} --specs=nano.specs")

SET(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} --specs=nosys.specs")

SET(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} -Wall")

SET(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} -Wl,-Map=output.map")


message(STATUS "Message: ProjDirPath is '${CMAKE_CURRENT_SOURCE_DIR}'")

# Set linker script
# set(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} -T${CMAKE_CURRENT_SOURCE_DIR}./Linkerscripts/lpc1113F_301.ld -static")

message(STATUS "Message: Linkerscript is: '${CMAKE_CURRENT_SOURCE_DIR}${LINKERSCRIPT_PATH}/${LINKERSCRIPT_NAME}'")

set(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} -T${CMAKE_CURRENT_SOURCE_DIR}${LINKERSCRIPT_PATH}/${LINKERSCRIPT_NAME} -static")

message(STATUS "Message: ld-command string = '${CMAKE_EXE_LINKER_FLAGS_DEBUG}'")
message(STATUS "Message: C++-command string = '${CMAKE_CXX_FLAGS_DEBUG}'")
message(STATUS "Message: C-command string = '${CMAKE_C_FLAGS_DEBUG}'")

message(STATUS "Message: System_Name = '${CMAKE_SYSTEM_NAME}'")
message(STATUS "Message: System_Version = '${CMAKE_SYSTEM_VERSION}'")
message(STATUS "Message: Crosscompiling = '${CMAKE_CROSSCOMPILING}'")
message(STATUS "Message: Host_System_Name = '${CMAKE_HOST_SYSTEM_NAME}'")

# add the executable
add_executable (LPC1113_Uart_Event.elf Main/main.c
Startup/startup_LPC11xx.S
Startup/system_LPC11xx.c
src/led_funcs.c
src/uart_utils.c
src/ring_buffer.c
)

# target_link_libraries (LPC1113_Test_2 ${EXTRA_LIBS})

#######################
message(STATUS "")
message(STATUS "Building LPC1113_Test_2 version ${PROJECT_VERSION} using build type '${CMAKE_BUILD_TYPE}'.")
message(STATUS "Source directory is '${PROJECT_SOURCE_DIR}'.")
message(STATUS "Build  directory is '${PROJECT_BINARY_DIR}'.")
message(STATUS "Toolchain is '${TOOLCHAIN}'.")
message(STATUS "")
#######################