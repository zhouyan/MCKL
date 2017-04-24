# ============================================================================
#  MCKL/cmake/FindJEMalloc.cmake
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

# Find jemalloc support
#
# The following variable are set
#
# JEMalloc_FOUND          - TRUE if jemalloc is found and work correctly
# JEMalloc_INCLUDE_DIR    - The directory containing jemalloc headers
# JEMalloc_LINK_LIBRARIES - jemalloc libraries that shall be linked to
#
# The following variables affect the behavior of this module
#
# JEMalloc_ROOT     - The root path that CMake shall try
# JEMalloc_INC_PATH - The path CMake shall try to find headers first
# JEMalloc_LIB_PATH - The path CMake shall try to find libraries first

if(DEFINED JEMalloc_FOUND)
    return()
endif(DEFINED JEMalloc_FOUND)

if(DEFINED JEMalloc_ROOT)
    if(NOT DEFINED JEMalloc_INC_PATH)
        set(JEMalloc_INC_PATH ${JEMalloc_ROOT}/include)
    endif(NOT DEFINED JEMalloc_INC_PATH)
    if(NOT DEFINED JEMalloc_LIB_PATH)
        set(JEMalloc_LIB_PATH ${JEMalloc_ROOT}/lib)
    endif(NOT DEFINED JEMalloc_LIB_PATH)
endif(DEFINED JEMalloc_ROOT)

file(READ ${CMAKE_CURRENT_LIST_DIR}/FindJEMalloc.cpp JEMalloc_TEST_SOURCE)

if(NOT DEFINED JEMalloc_INCLUDE_DIR)
    find_path(JEMalloc_INCLUDE_DIR jemalloc/jemalloc.h
        PATH ${JEMalloc_INC_PATH} ENV CPATH NO_DEFAULT_PATH)
    find_path(JEMalloc_INCLUDE_DIR jemalloc/jemalloc.h)
    if(JEMalloc_INCLUDE_DIR)
        message(STATUS "Found jemalloc headers: ${JEMalloc_INCLUDE_DIR}")
    else(JEMalloc_INCLUDE_DIR)
        message(STATUS "NOT Found jemalloc headers")
    endif(JEMalloc_INCLUDE_DIR)
endif(NOT DEFINED JEMalloc_INCLUDE_DIR)

if(NOT DEFINED JEMalloc_LINK_LIBRARIES)
    find_library(JEMalloc_LINK_LIBRARIES jemalloc
        PATHS ${JEMalloc_LIB_PATH} ENV LIBRARY_PATH ENV LIB NO_DEFAULT_PATH)
    find_library(JEMalloc_LINK_LIBRARIES jemalloc)
    if(JEMalloc_LINK_LIBRARIES)
        message(STATUS "Found jemalloc libraries: ${JEMalloc_LINK_LIBRARIES}")
    else(JEMalloc_LINK_LIBRARIES)
        message(STATUS "NOT Found jemalloc libraries")
    endif(JEMalloc_LINK_LIBRARIES)
endif(NOT DEFINED JEMalloc_LINK_LIBRARIES)

if(JEMalloc_INCLUDE_DIR AND JEMalloc_LINK_LIBRARIES)
    include(CheckCXXSourceCompiles)
    set(SAFE_CMAKE_REQUIRED_DEFINITIONS ${CMAKE_REQUIRED_DEFINITIONS})
    set(SAFE_CMAKE_REQUIRED_INCLUDES ${CMAKE_REQUIRED_INCLUDES})
    set(SAFE_CMAKE_REQUIRED_LIBRARIES ${CMAKE_REQUIRED_LIBRARIES})
    set(CMAKE_REQUIRED_DEFINITIONS ${SAFE_CMAKE_REQUIRED_DEFINITIONS}
        -DJEMALLOC_NO_DEMANGLE)
    set(CMAKE_REQUIRED_INCLUDES ${SAFE_CMAKE_REQUIRED_INCLUDES}
        ${JEMalloc_INCLUDE_DIR})
    set(CMAKE_REQUIRED_LIBRARIES ${SAFE_CMAKE_REQUIRED_LIBRARIES}
        ${JEMalloc_LINK_LIBRARIES})
    check_cxx_source_compiles("${JEMalloc_TEST_SOURCE}" JEMalloc_TEST)
    if(JEMalloc_TEST)
        set(JEMalloc_FOUND TRUE CACHE BOOL "Found jemalloc support")
    else(JEMalloc_TEST)
        set(JEMalloc_FOUND FALSE CACHE BOOL "NOT Found jemalloc support")
    endif(JEMalloc_TEST)
    set(CMAKE_REQUIRED_DEFINITIONS ${SAFE_CMAKE_REQUIRED_DEFINITIONS})
    set(CMAKE_REQUIRED_INCLUDES ${SAFE_CMAKE_REQUIRED_INCLUDES})
    set(CMAKE_REQUIRED_LIBRARIES ${SAFE_CMAKE_REQUIRED_LIBRARIES})
endif(JEMalloc_INCLUDE_DIR AND JEMalloc_LINK_LIBRARIES)
