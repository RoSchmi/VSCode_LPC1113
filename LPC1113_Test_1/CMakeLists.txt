cmake_minimum_required(VERSION 3.0.0)

project(LPC1113_Test_1 VERSION 0.1.0)

option (USE_MYMATH "Use tutorial provided math implementation" OFF)

include(CTest)
enable_testing()

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)

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

set (TOOLCHAIN_PREFIX C:/nanoFramework_Tools/GNU_ARM_Toolchain/7-2018-q2-update/bin)

message(STATUS "ToolChain_Prefix is: ${TOOLCHAIN_PREFIX}")

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
project(nanoFramework VERSION ${BUILD_VERSION})

message(STATUS "Dummy message: Version Major from LPC1113_Test_1Config.h is '${LPC1113_Test_1_VERSION_MAJOR}'")

# should we use our own math functions?

if (USE_MYMATH)

include_directories ("${PROJECT_SOURCE_DIR}/MathFunctions")
add_subdirectory (MathFunctions)
set (EXTRA_LIBS ${EXTRA_LIBS} MathFunctions)

endif (USE_MYMATH)

#+++++++++++++++++  added +++++++++++++++++++++++
# from the example in the official CMake Tutorial
# configure a header file to pass some of the CMake settings
# to the source code
configure_file (
  "${PROJECT_SOURCE_DIR}/LPC1113_Test_1Config.h.in"
  "${PROJECT_BINARY_DIR}/LPC1113_Test_1Config.h"
  )
 
# add the binary tree to the search path for include files
# so that we will find TutorialConfig.h
include_directories("${PROJECT_BINARY_DIR}")

#******************** end added *************************

 
# add the executable
add_executable (LPC1113_Test_1 Main/main.cpp)
target_link_libraries (LPC1113_Test_1 ${EXTRA_LIBS})


#######################
message(STATUS "")
message(STATUS "Building LPC1113_Test_1 version ${PROJECT_VERSION} using build type '${CMAKE_BUILD_TYPE}'.")
message(STATUS "Source directory is '${PROJECT_SOURCE_DIR}'.")
message(STATUS "Build  directory is '${PROJECT_BINARY_DIR}'.")
message(STATUS "Toolchain is '${TOOLCHAIN}'.")
message(STATUS "")
#######################
