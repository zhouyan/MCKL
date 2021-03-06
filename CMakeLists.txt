# ============================================================================
#  MCKL/CMakeLists.txt
# ----------------------------------------------------------------------------
#  MCKL: Monte Carlo Kernel Library
# ----------------------------------------------------------------------------
#  Copyright (c) 2013-2017, Yan Zhou
#  All rights reserved.
#
#  Redistribution and use in source and binary forms, with or without
#  modification, are permitted provided that the following conditions are met:
#
#    Redistributions of source code must retain the above copyright notice,
#    this list of conditions and the following disclaimer.
#
#    Redistributions in binary form must reproduce the above copyright notice,
#    this list of conditions and the following disclaimer in the documentation
#    and/or other materials provided with the distribution.
#
#  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
#  AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
#  IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
#  ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
#  LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
#  CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
#  SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
#  INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
#  CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
#  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
#  POSSIBILITY OF SUCH DAMAGE.
# ============================================================================

cmake_minimum_required(VERSION 2.8.4)

##############################################################################
# Disable in source build
##############################################################################

if(${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_BINARY_DIR})
    message(FATAL_ERROR "In-source builds not allowed")
endif(${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_BINARY_DIR})

##############################################################################
# Basics
##############################################################################

project(MCKL C CXX)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${PROJECT_SOURCE_DIR}/cmake)

##############################################################################
# Options
##############################################################################

option(MCKL_ENABLE_EXAMPLE "Enable building of example" ON)
option(MCKL_ENABLE_LIBRARY "Enable building of library" ON)

##############################################################################
# Installation
##############################################################################

if(NOT DEFINED MCKL_INSTALL_INC_DIR)
    SET(MCKL_INSTALL_INC_DIR include)
endif(NOT DEFINED MCKL_INSTALL_INC_DIR)
install(DIRECTORY ${PROJECT_SOURCE_DIR}/include/mckl
    DESTINATION ${MCKL_INSTALL_INC_DIR}
    PATTERN ".DS_Store" EXCLUDE PATTERN "*.swp" EXCLUDE)

##############################################################################
# Check features
##############################################################################

if(MCKL_ENABLE_EXAMPLE OR MCKL_ENABLE_LIBRARY)
    include(MCKL)
else(MCKL_ENABLE_EXAMPLE OR MCKL_ENABLE_LIBRARY)
    return()
endif(MCKL_ENABLE_EXAMPLE OR MCKL_ENABLE_LIBRARY)

##############################################################################
# Library
##############################################################################

if(MCKL_ENABLE_LIBRARY AND CMAKE_ASM_NASM_COMPILER_LOADED AND UNIX)
    add_custom_target(lib)
    add_subdirectory(lib)
    set(MCKL_LINK_LIBRARIES ${MCKL_LINK_LIBRARIES} libmckl)
    add_definitions(-DMCKL_USE_ASM_LIBRARY=1)
    add_definitions(-DMCKL_USE_ASM_VMF=1)
else(MCKL_ENABLE_LIBRARY AND CMAKE_ASM_NASM_COMPILER_LOADED AND UNIX)
    unset(MCKL_ENABLE_LIBRARY CACHE)
    set(MCKL_ENABLE_LIBRARY CACHE BOOL FALSE)
endif(MCKL_ENABLE_LIBRARY AND CMAKE_ASM_NASM_COMPILER_LOADED AND UNIX)

##############################################################################
# Example
##############################################################################

if(MCKL_ENABLE_EXAMPLE)
    add_custom_target(example)
    add_subdirectory(example EXCLUDE_FROM_ALL)
endif(MCKL_ENABLE_EXAMPLE)
