# ============================================================================
#  MCKL/cmake/FindTBB.cmake
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

# Find Intel Threading Building Blocks library
#
# This module can be used to find TBB headers and libraries
#
# The following variables are set
#
# TBB_FOUND                       - TRUE if TBB is found and work correctly
# TBB_INCLUDE_DIR                 - The directory containing TBB headers
# TBB_LINK_LIBRARIES              - TBB libraries that shall be linked to
# TBB_MALLOC_LINK_LIBRARIES       - TBB malloc libraries
# TBB_MALLOC_PROXY_LINK_LIBRARIES - TBB malloc proxy libraries
#
# The following variables affect the behavior of this module
#
# TBB_ROOT     - The root path that CMake shall try
# TBB_INC_PATH - The path CMake shall try to find headers first
# TBB_LIB_PATH - The path CMake shall try to find libraries first

if(DEFINED TBB_FOUND)
    return()
endif(DEFINED TBB_FOUND)

if(${TBB_ROOT})
    if(NOT ${TBB_INC_PATH})
        set(TBB_INC_PATH ${TBB_ROOT}/include)
    endif(NOT ${TBB_INC_PATH})
    if(NOT ${TBB_LIB_PATH})
        if(APPLE)
            set(TBB_LIB_PATH ${TBB_ROOT}/lib)
        else(APPLE)
            set(TBB_LIB_PATH ${TBB_ROOT}/lib/intel64/gcc4.7)
        endif(APPLE)
    endif(NOT ${TBB_LIB_PATH})
endif(${TBB_ROOT})

file(READ ${CMAKE_CURRENT_LIST_DIR}/FindTBB.cpp TBB_TEST_SOURCE)

if(NOT DEFINED TBB_LINK_LIBRARIES)
    find_library(TBB_LINK_LIBRARIES_RELEASE_FOUND tbb
        PATHS ${TBB_LIB_PATH} ENV LIBRARY_PATH ENV LIB NO_DEFAULT_PATH)
    find_library(TBB_LINK_LIBRARIES_RELEASE_FOUND tbb)
    find_library(TBB_LINK_LIBRARIES_DEBUG_FOUND tbb_debug
        PATHS ${TBB_LIB_PATH} ENV LIBRARY_PATH ENV LIB NO_DEFAULT_PATH)
    find_library(TBB_LINK_LIBRARIES_DEBUG_FOUND tbb_debug)
    if(TBB_LINK_LIBRARIES_RELEASE_FOUND AND TBB_LINK_LIBRARIES_DEBUG_FOUND)
        set(TBB_LINK_LIBRARIES
            optimized ${TBB_LINK_LIBRARIES_RELEASE_FOUND}
            debug ${TBB_LINK_LIBRARIES_DEBUG_FOUND}
            CACHE STRING "Link to TBB")
        set(TBB_LINK_LIBRARIES_RELEASE ${TBB_LINK_LIBRARIES_RELEASE_FOUND}
            CACHE STRING "Link to TBB Release")
        set(TBB_LINK_LIBRARIES_DEBUG ${TBB_LINK_LIBRARIES_DEBUG_FOUND}
            CACHE STRING "Link to TBB Debug")
        message(STATUS "Found TBB libraries: ${TBB_LINK_LIBRARIES}")
    elseif(TBB_LINK_LIBRARIES_RELEASE_FOUND)
        set(TBB_LINK_LIBRARIES ${TBB_LINK_LIBRARIES_RELEASE_FOUND}
            CACHE STRING "Link to TBB")
        set(TBB_LINK_LIBRARIES_RELEASE ${TBB_LINK_LIBRARIES_RELEASE_FOUND}
            CACHE STRING "Link to TBB Release")
        message(STATUS "Found TBB libraries: ${TBB_LINK_LIBRARIES}")
    else(TBB_LINK_LIBRARIES_RELEASE_FOUND AND TBB_LINK_LIBRARIES_DEBUG_FOUND)
        message(STATUS "NOT Found TBB libraries")
    endif(TBB_LINK_LIBRARIES_RELEASE_FOUND AND TBB_LINK_LIBRARIES_DEBUG_FOUND)
endif(NOT DEFINED TBB_LINK_LIBRARIES)

if(NOT DEFINED TBB_MALLOC_LINK_LIBRARIES)
    find_library(TBB_MALLOC_LINK_LIBRARIES_RELEASE_FOUND tbbmalloc
        PATHS ${TBB_LIB_PATH} ENV LIBRARY_PATH ENV LIB NO_DEFAULT_PATH)
    find_library(TBB_MALLOC_LINK_LIBRARIES_RELEASE_FOUND tbbmalloc)
    find_library(TBB_MALLOC_LINK_LIBRARIES_DEBUG_FOUND tbbmalloc_debug
        PATHS ${TBB_LIB_PATH} ENV LIBRARY_PATH ENV LIB NO_DEFAULT_PATH)
    find_library(TBB_MALLOC_LINK_LIBRARIES_DEBUG_FOUND tbbmalloc_debug)
    if(TBB_MALLOC_LINK_LIBRARIES_RELEASE_FOUND AND
            TBB_MALLOC_LINK_LIBRARIES_DEBUG_FOUND)
        set(TBB_MALLOC_LINK_LIBRARIES
            optimized ${TBB_MALLOC_LINK_LIBRARIES_RELEASE_FOUND}
            debug ${TBB_MALLOC_LINK_LIBRARIES_DEBUG_FOUND}
            CACHE STRING "Link to TBB malloc")
        set(TBB_MALLOC_LINK_LIBRARIES_RELEASE
            ${TBB_MALLOC_LINK_LIBRARIES_RELEASE_FOUND}
            CACHE STRING "Link to TBB malloc Release")
        set(TBB_MALLOC_LINK_LIBRARIES_DEBUG
            ${TBB_MALLOC_LINK_LIBRARIES_DEBUG_FOUND}
            CACHE STRING "Link to TBB malloc Debug")
        message(STATUS
            "Found TBB malloc libraries: ${TBB_MALLOC_LINK_LIBRARIES}")
    elseif(TBB_MALLOC_LINK_LIBRARIES_RELEASE_FOUND)
        set(TBB_MALLOC_LINK_LIBRARIES
            ${TBB_MALLOC_LINK_LIBRARIES_RELEASE_FOUND}
            CACHE STRING "Link to TBB malloc")
        set(TBB_MALLOC_LINK_LIBRARIES_RELEASE
            ${TBB_MALLOC_LINK_LIBRARIES_RELEASE_FOUND}
            CACHE STRING "Link to TBB malloc Release")
        message(STATUS
            "Found TBB malloc libraries: ${TBB_MALLOC_LINK_LIBRARIES}")
    else(TBB_MALLOC_LINK_LIBRARIES_RELEASE_FOUND AND
            TBB_MALLOC_LINK_LIBRARIES_DEBUG_FOUND)
        message(STATUS "NOT Found TBB malloc libraries")
    endif(TBB_MALLOC_LINK_LIBRARIES_RELEASE_FOUND AND
        TBB_MALLOC_LINK_LIBRARIES_DEBUG_FOUND)
endif(NOT DEFINED TBB_MALLOC_LINK_LIBRARIES)

if(NOT DEFINED TBB_MALLOC_PROXY_LINK_LIBRARIES)
    find_library(TBB_MALLOC_PROXY_LINK_LIBRARIES_RELEASE_FOUND
        tbbmalloc_proxy
        PATHS ${TBB_LIB_PATH} ENV LIBRARY_PATH ENV LIB NO_DEFAULT_PATH)
    find_library(TBB_MALLOC_PROXY_LINK_LIBRARIES_RELEASE_FOUND
        tbbmalloc_proxy)
    find_library(TBB_MALLOC_PROXY_LINK_LIBRARIES_DEBUG_FOUND
        tbbmalloc_proxy_debug
        PATHS ${TBB_LIB_PATH} ENV LIBRARY_PATH ENV LIB NO_DEFAULT_PATH)
    find_library(TBB_MALLOC_PROXY_LINK_LIBRARIES_DEBUG_FOUND
        tbbmalloc_proxy_debug)
    if(TBB_MALLOC_PROXY_LINK_LIBRARIES_RELEASE_FOUND AND
            TBB_MALLOC_PROXY_LINK_LIBRARIES_DEBUG_FOUND)
        set(TBB_MALLOC_PROXY_LINK_LIBRARIES
            optimized ${TBB_MALLOC_PROXY_LINK_LIBRARIES_RELEASE_FOUND}
            debug ${TBB_MALLOC_PROXY_LINK_LIBRARIES_DEBUG_FOUND}
            CACHE STRING "Link to TBB malloc proxy")
        set(TBB_MALLOC_PROXY_LINK_LIBRARIES_RELEASE
            ${TBB_MALLOC_PROXY_LINK_LIBRARIES_RELEASE_FOUND}
            CACHE STRING "Link to TBB malloc proxy Release")
        set(TBB_MALLOC_PROXY_LINK_LIBRARIES_DEBUG
            ${TBB_MALLOC_PROXY_LINK_LIBRARIES_DEBUG_FOUND}
            CACHE STRING "Link to TBB malloc proxy Debug")
        message(STATUS
            "Found TBB malloc proxy libraries: ${TBB_MALLOC_PROXY_LINK_LIBRARIES}")
    elseif(TBB_MALLOC_PROXY_LINK_LIBRARIES_RELEASE_FOUND)
        set(TBB_MALLOC_PROXY_LINK_LIBRARIES
            ${TBB_MALLOC_PROXY_LINK_LIBRARIES_RELEASE_FOUND}
            CACHE STRING "Link to TBB malloc proxy")
        set(TBB_MALLOC_PROXY_LINK_LIBRARIES_RELEASE
            ${TBB_MALLOC_PROXY_LINK_LIBRARIES_RELEASE_FOUND}
            CACHE STRING "Link to TBB malloc proxy Release")
        message(STATUS
            "Found TBB malloc proxy libraries: ${TBB_MALLOC_PROXY_LINK_LIBRARIES}")
    else(TBB_MALLOC_PROXY_LINK_LIBRARIES_RELEASE_FOUND AND
            TBB_MALLOC_PROXY_LINK_LIBRARIES_DEBUG_FOUND)
        message(STATUS "NOT Found TBB malloc proxy libraries")
    endif(TBB_MALLOC_PROXY_LINK_LIBRARIES_RELEASE_FOUND AND
        TBB_MALLOC_PROXY_LINK_LIBRARIES_DEBUG_FOUND)
endif(NOT DEFINED TBB_MALLOC_PROXY_LINK_LIBRARIES)

if(NOT DEFINED TBB_INCLUDE_DIR)
    find_path(TBB_INCLUDE_DIR tbb/tbb.h
        PATHS ${TBB_INC_PATH} ENV CPATH NO_DEFAULT_PATH)
    find_path(TBB_INCLUDE_DIR tbb/tbb.h)
    if(TBB_INCLUDE_DIR)
        message(STATUS "Found TBB headers: ${TBB_INCLUDE_DIR}")
    else(TBB_INCLUDE_DIR)
        message(STATUS "NOT Found TBB headers")
    endif(TBB_INCLUDE_DIR)
endif(NOT DEFINED TBB_INCLUDE_DIR)

if(TBB_LINK_LIBRARIES AND TBB_INCLUDE_DIR)
    set(TBB_FOUND TRUE CACHE BOOL "Found TBB")
else(TBB_LINK_LIBRARIES AND TBB_INCLUDE_DIR)
    set(TBB_FOUND FALSE CACHE BOOL "NOT Found TBB")
endif(TBB_LINK_LIBRARIES AND TBB_INCLUDE_DIR)
