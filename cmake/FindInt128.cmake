# ============================================================================
#  MCKL/cmake/FindInt128.cmake
# ----------------------------------------------------------------------------
#  MCKL: Monte Carlo Kernel Library
# ----------------------------------------------------------------------------
#  Copyright (c) 2013-2016, Yan Zhou
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

# Find 128-bits integer type
#
# The following variable is set
#
# INT128_FOUND - TRUE if 128-bits integer type is found and works correctly
# INT128_TYPE  - The type of the 128-bits integer

IF(DEFINED INT128_FOUND)
    RETURN()
ENDIF(DEFINED INT128_FOUND)

SET(SAFE_CMAKE_REQUIRED_DEFINITIONS ${CMAKE_REQUIRED_DEFINITIONS})

FILE(READ ${CMAKE_CURRENT_LIST_DIR}/FindInt128.cpp INT128_TEST_SOURCE)

SET(INT128_TRY_TYPE "__int128")
SET(CMAKE_REQUIRED_DEFINITIONS ${SAFE_CMAKE_REQUIRED_DEFINITIONS}
    -DINT128=${INT128_TRY_TYPE})
INCLUDE(CheckCXXSourceCompiles)
CHECK_CXX_SOURCE_COMPILES("${INT128_TEST_SOURCE}" INT128_TEST)
IF(INT128_TEST)
    SET(INT128_TYPE ${INT128_TRY_TYPE} CACHE STRING "128-bit type")
    SET(INT128_FOUND TRUE CACHE BOOL "Found 128-bits integer type")
ELSE(INT128_TEST)
    SET(INT128_FOUND FALSE CACHE BOOL "NOT Found 128-bit integer type")
ENDIF(INT128_TEST)

SET(CMAKE_REQUIRED_DEFINITIONS ${SAFE_CMAKE_REQUIRED_DEFINITIONS})
