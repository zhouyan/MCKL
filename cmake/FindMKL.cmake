# ============================================================================
#  MCKL/cmake/FindMKL.cmake
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

# Find Intel Math Kernel Library
#
# This module can be used to find MKL headers and libraries
#
# The following variables are set
#
# MKL_FOUND                 - TRUE if MKL is found and work correctly
# MKL_INCLUDE_DIR           - The directory containing MKL headers
# MKL_LINK_LIBRARIES        - MKL libraries that shall be linked to
# MKL_BLAS95_LINK_LIBRARIES - MKL libraries for Fortran 95 interface to BLAS
#
# The following variables affect the behavior of this module
#
# MKL_INC_PATH - The path CMake shall try to find headers first
# MKL_LIB_PATH - The path CMake shall try to find libraries first

if(DEFINED MKL_FOUND)
    return()
endif(DEFINED MKL_FOUND)

if(NOT DEFINED MKL_LINK_LIBRARIES)
    include(FindThreads)
    find_library(MKL_LINK_LIBRARIES mkl_rt
        PATHS ${MKL_LIB_PATH} ENV LIBRARY_PATH NO_DEFAULT_PATH)
    find_library(MKL_LINK_LIBRARIES mkl_rt)

    find_library(MKL_BLAS95_LINK_LIBRARIES mkl_blas95
        PATHS ${MKL_LIB_PATH} ENV LIBRARY_PATH NO_DEFAULT_PATH)
    find_library(MKL_BLAS95_LINK_LIBRARIES mkl_blas95)

    find_library(MKL_BLAS95_LP64_LINK_LIBRARIES mkl_blas95_lp64
        PATHS ${MKL_LIB_PATH} ENV LIBRARY_PATH NO_DEFAULT_PATH)
    find_library(MKL_BLAS95_LP64_LINK_LIBRARIES mkl_blas95_lp64)

    find_library(MKL_BLAS95_ILP64_LINK_LIBRARIES mkl_blas95_ilp64
        PATHS ${MKL_LIB_PATH} ENV LIBRARY_PATH NO_DEFAULT_PATH)
    find_library(MKL_BLAS95_ILP64_LINK_LIBRARIES mkl_blas95_ilp64)

    if(MKL_LINK_LIBRARIES)
        set(MKL_LINK_LIBRARIES ${MKL_LINK_LIBRARIES}
            ${CMAKE_THREAD_LIBS_INIT} CACHE STRING "MKL Libraries")
        message(STATUS "Found MKL libraries: ${MKL_LINK_LIBRARIES}")
    else(MKL_LINK_LIBRARIES)
        message(STATUS "NOT Found MKL libraries")
    endif(MKL_LINK_LIBRARIES)
endif(NOT DEFINED MKL_LINK_LIBRARIES)

if(NOT DEFINED MKL_INCLUDE_DIR)
    find_path(MKL_INCLUDE_DIR mkl_vml.h
        PATHS ${MKL_INC_PATH} ENV CPATH NO_DEFAULT_PATH)
    find_path(MKL_INCLUDE_DIR mkl_vml.h)
    if(MKL_INCLUDE_DIR)
        message(STATUS "Found MKL headers: ${MKL_INCLUDE_DIR}")
    else(MKL_INCLUDE_DIR)
        message(STATUS "NOT Found MKL headers")
    endif(MKL_INCLUDE_DIR)
endif(NOT DEFINED MKL_INCLUDE_DIR)

if(MKL_LINK_LIBRARIES AND MKL_INCLUDE_DIR)
    set(MKL_FOUND TRUE CACHE BOOL "Found MKL")
else(MKL_LINK_LIBRARIES AND MKL_INCLUDE_DIR)
    set(MKL_FOUND FALSE CACHE BOOL "NOT Found MKL")
endif(MKL_LINK_LIBRARIES AND MKL_INCLUDE_DIR)
