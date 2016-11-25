//============================================================================
// MCKL/include/mckl/internal/config.h
//----------------------------------------------------------------------------
// MCKL: Monte Carlo Kernel Library
//----------------------------------------------------------------------------
// Copyright (c) 2013-2016, Yan Zhou
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//
//   Redistributions of source code must retain the above copyright notice,
//   this list of conditions and the following disclaimer.
//
//   Redistributions in binary form must reproduce the above copyright notice,
//   this list of conditions and the following disclaimer in the documentation
//   and/or other materials provided with the distribution.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS AS IS
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
// LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
// SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
// CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
// POSSIBILITY OF SUCH DAMAGE.
//============================================================================

#ifndef MCKL_INTERNAL_CONFIG_H
#define MCKL_INTERNAL_CONFIG_H

// Compiler dependent macros

#include <mckl/internal/compiler.h>

// ASM library header file
#include <mckl/mckl.h>

// Assertion macros

#ifndef MCKL_NO_RUNTIME_ASSERT
#ifndef NDEBUG
#define MCKL_NO_RUNTIME_ASSERT 0
#else
#define MCKL_NO_RUNTIME_ASSERT 1
#endif
#endif

#ifndef MCKL_NO_RUNTIME_WARNING
#ifndef NDEBUG
#define MCKL_NO_RUNTIME_WARNING 0
#else
#define MCKL_NO_RUNTIME_WARNING 1
#endif
#endif

/// \brief Turn MCKL runtime assertions into exceptions
/// \ingroup Config
#ifndef MCKL_RUNTIME_ASSERT_AS_EXCEPTION
#define MCKL_RUNTIME_ASSERT_AS_EXCEPTION 0
#endif

/// \brief Turn MCKL runtime warnings into exceptions
/// \ingroup Config
#ifndef MCKL_RUNTIME_WARNING_AS_EXCEPTION
#define MCKL_RUNTIME_WARNING_AS_EXCEPTION 0
#endif

/// \brief Produce exact the same results regardless of endianness
/// \ingroup Config
#ifndef MCKL_REQUIRE_ENDIANNESS_NEUTURAL
#define MCKL_REQUIRE_ENDIANNESS_NEUTURAL 0
#endif

// OS dependent macros

#ifndef MCKL_OPENCL
#ifdef __APPLE__
#include <Availability.h>
#if defined(__MAC_OS_X_VERSION_MIN_REQUIRED) && defined(__MAC_10_5)
#if __MAC_OS_X_VERSION_MIN_REQUIRED >= __MAC_10_5
#ifndef MCKL_HAS_POSIX
#define MCKL_HAS_POSIX 1
#endif
#endif // __MAC_OS_X_VERSION_MIN_REQUIRED >= __MAC_10_5
#endif // defined(__MAC_OS_X_VERSION_MIN_REQUIRED) && defined(__MAC_10_5)
#elif defined(__POSIX_C_SOURCE)
#if _POSIX_C_SOURCE >= 200112L
#ifndef MCKL_HAS_POSIX
#define MCKL_HAS_POSIX 1
#endif
#endif // _POSIX_C_SOURCE >= 200112L
#elif defined(_XOPEN_SOURCE)
#if _XOPEN_SOURCE >= 600
#ifndef MCKL_HAS_POSIX
#define MCKL_HAS_POSIX 1
#endif
#endif // _XOPEN_SOURCE >= 600
#endif // __APPLE__
#endif // MCKL_OPENCL

#ifndef MCKL_HAS_POSIX
#define MCKL_HAS_POSIX 0
#endif

// Optional libraries

#ifndef MCKL_USE_ASM_LIBRARY
#define MCKL_USE_ASM_LIBRARY 0
#endif

#ifndef MCKL_USE_ASM_VMF
#define MCKL_USE_ASM_VMF 0
#endif

#ifndef MCKL_HAS_OMP
#ifdef _OPENMP
#define MCKL_HAS_OMP 1
#else
#define MCKL_HAS_OMP 0
#endif
#endif

#ifndef MCKL_USE_OMP
#define MCKL_USE_OMP MCKL_HAS_OMP
#endif

#ifndef MCKL_HAS_TBB
#define MCKL_HAS_TBB 0
#endif

#ifndef MCKL_USE_TBB
#define MCKL_USE_TBB MCKL_HAS_TBB
#endif

#ifndef MCKL_USE_TBB_MALLOC
#define MCKL_USE_TBB_MALLOC MCKL_HAS_TBB
#endif

#ifndef MCKL_USE_TBB_TLS
#define MCKL_USE_TBB_TLS MCKL_HAS_TBB
#endif

#ifndef MCKL_HAS_MKL
#define MCKL_HAS_MKL 0
#endif

#ifndef MCKL_USE_MKL_CBLAS
#define MCKL_USE_MKL_CBLAS MCKL_HAS_MKL
#endif

#ifndef MCKL_USE_MKL_VML
#define MCKL_USE_MKL_VML MCKL_HAS_MKL
#endif

#ifndef MCKL_USE_MKL_VSL
#define MCKL_USE_MKL_VSL MCKL_HAS_MKL
#endif

#ifndef MCKL_HAS_BLAS
#define MCKL_HAS_BLAS MCKL_HAS_MKL
#endif

#ifndef MCKL_USE_CBLAS
#define MCKL_USE_CBLAS MCKL_USE_MKL_CBLAS
#endif

#ifndef MCKL_HAS_HDF5
#define MCKL_HAS_HDF5 0
#endif

#ifndef MCKL_HAS_TESTU01
#define MCKL_HAS_TESTU01 0
#endif

#endif // MCKL_INTERNAL_CONFIG_H
