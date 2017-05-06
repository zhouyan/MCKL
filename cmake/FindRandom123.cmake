# ============================================================================
#  MCKL/cmake/FindRandom123.cmake
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

# Find the Random123 library
#
# This module can be used to find Random123 headers
#
# The following variables are set
#
# Random123_FOUND           - TRUE if Random123 is found and work correctly
# Random123_INCLUDE_DIR     - The directory containing MKL headers
#
# The following variables affect the behavior of this module
#
# Random123_ROOT     - The root path that CMake shall try
# Random123_INC_PATH - The path CMake shall try to find headers first

if(DEFINED Random123_FOUND)
    return()
endif(DEFINED Random123_FOUND)

if(DEFINED Random123_ROOT)
    if(NOT DEFINED Random123_INC_PATH)
        set(Random123_INC_PATH ${Random123_ROOT}/include)
    endif(NOT DEFINED Random123_INC_PATH)
endif(DEFINED Random123_ROOT)

if(NOT DEFINED Random123_INCLUDE_DIR)
    find_path(Random123_INCLUDE_DIR Random123/aes.h
        PATHS ${Random123_INC_PATH} ENV CPATH NO_DEFAULT_PATH)
    find_path(Random123_INCLUDE_DIR Random123/aes.h)
    if(Random123_INCLUDE_DIR)
        message(STATUS "Found Random123 headers: ${Random123_INCLUDE_DIR}")
    else(Random123_INCLUDE_DIR)
        message(STATUS "NOT Found Random123 headers")
    endif(Random123_INCLUDE_DIR)
endif(NOT DEFINED Random123_INCLUDE_DIR)

if(Random123_INCLUDE_DIR)
    set(Random123_FOUND TRUE CACHE BOOL "Found Random123")
else(Random123_INCLUDE_DIR)
    set(Random123_FOUND FALSE CACHE BOOL "NOT Found Random123")
endif(Random123_INCLUDE_DIR)
