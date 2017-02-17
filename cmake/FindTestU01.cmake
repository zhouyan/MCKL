# ============================================================================
#  MCKL/cmake/FindTestU01.cmake
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
#  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS AS IS
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

# Find Intel Math Kernel Library
#
# This module can be used to find TestU01 headers and libraries
#
# The following variables are set
#
# TestU01_FOUND                 - TRUE if TestU01 is found and work correctly
# TestU01_INCLUDE_DIR           - The directory containing TestU01 headers
# TestU01_LINK_LIBRARIES        - TestU01 libraries that shall be linked to
#
# The following variables affect the behavior of this module
#
# TestU01_INC_PATH - The path CMake shall try to find headers first
# TestU01_LIB_PATH - The path CMake shall try to find libraries first

if(DEFINED TestU01_FOUND)
    return()
endif(DEFINED TestU01_FOUND)

if(NOT DEFINED TestU01_LINK_LIBRARIES)
    find_library(TestU01 testu01
        PATHS ${TestU01_LIB_PATH} ENV LIBRARY_PATH NO_DEFAULT_PATH)
    find_library(TestU01 testu01)

    find_library(TestU01_PROBDIST probdist
        PATHS ${TestU01_LIB_PATH} ENV LIBRARY_PATH NO_DEFAULT_PATH)
    find_library(TestU01_PROBDIST probdist)

    find_library(TestU01_MYLIB mylib
        PATHS ${TestU01_LIB_PATH} ENV LIBRARY_PATH NO_DEFAULT_PATH)
    find_library(TestU01_MYLIB mylib)

    if(TestU01 AND TestU01_PROBDIST AND TestU01_MYLIB)
        set(TestU01_LINK_LIBRARIES
            ${TestU01} ${TestU01_PROBDIST} ${TestU01_MYLIB}
            CACHE STRING "TestU01 Libraries")
        message(STATUS "Found TestU01 libraries: ${TestU01_LINK_LIBRARIES}")
    else(TestU01 AND TestU01_PROBDIST AND TestU01_MYLIB)
        message(STATUS "NOT Found TestU01 libraries")
    endif(TestU01 AND TestU01_PROBDIST AND TestU01_MYLIB)
endif(NOT DEFINED TestU01_LINK_LIBRARIES)

if(NOT DEFINED TestU01_INCLUDE_DIR)
    find_path(TestU01_INCLUDE_DIR TestU01.h
        PATHS ${TestU01_INC_PATH} ENV CPATH NO_DEFAULT_PATH)
    find_path(TestU01_INCLUDE_DIR TestU01.h)
    if(TestU01_INCLUDE_DIR)
        message(STATUS "Found TestU01 headers: ${TestU01_INCLUDE_DIR}")
    else(TestU01_INCLUDE_DIR)
        message(STATUS "NOT Found TestU01 headers")
    endif(TestU01_INCLUDE_DIR)
endif(NOT DEFINED TestU01_INCLUDE_DIR)

if(TestU01_LINK_LIBRARIES AND TestU01_INCLUDE_DIR)
    set(TestU01_FOUND TRUE CACHE BOOL "Found TestU01")
else(TestU01_LINK_LIBRARIES AND TestU01_INCLUDE_DIR)
    set(TestU01_FOUND FALSE CACHE BOOL "NOT Found TestU01")
endif(TestU01_LINK_LIBRARIES AND TestU01_INCLUDE_DIR)
