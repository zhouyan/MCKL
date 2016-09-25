//============================================================================
// MCKL/include/mckl/internal/compiler.h
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

#ifndef MCKL_INTERNAL_COMPILER_H
#define MCKL_INTERNAL_COMPILER_H

#ifndef __STDC_CONSTANT_MACROS
#define __STDC_CONSTANT_MACROS
#endif

#if defined(__OPENCL_VERSION__)
#define MCKL_OPENCL
#include <mckl/internal/compiler/opencl.h>
#elif defined(__INTEL_COMPILER)
#define MCKL_INTEL
#include <mckl/internal/compiler/intel.h>
#elif defined(__clang__)
#define MCKL_CLANG
#include <mckl/internal/compiler/clang.h>
#elif defined(__GNUC__)
#define MCKL_GCC
#include <mckl/internal/compiler/gcc.h>
#elif defined(_MSC_VER)
#define MCKL_MSVC
#include <mckl/internal/compiler/msvc.h>
#endif

#ifndef MCKL_OPENCL
#ifdef __cplusplus
#include <cmath>
#include <cstddef>
#include <cstdint>
#include <cstdlib>
#else
#include <math.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#endif
#endif

#ifndef UINT64_C
#error __STDC_CONSTANT_MACROS not defined before #include<stdint.h>
#endif

#ifndef MCKL_HAS_X86
#if defined(i386) || defined(__i386) || defined(__i386__) ||                  \
    defined(_M_IX86) || defined(_X86_) || defined(__x86_64) ||                \
    defined(__x86_64__) || defined(__amd64) || defined(__amd64__) ||          \
    defined(_M_AMD64) || defined(_M_X64)
#define MCKL_HAS_X86 1
#else
#define MCKL_HAS_X86 0
#endif
#endif

#ifndef MCKL_HAS_X86_64
#if defined(__x86_64) || defined(__x86_64__) || defined(__amd64) ||           \
    defined(__amd64__) || defined(_M_AMD64) || defined(_M_X64)
#define MCKL_HAS_X86_64 1
#else
#define MCKL_HAS_X86_64 0
#endif
#endif

#ifndef MCKL_HAS_AESNI
#define MCKL_HAS_AESNI 0
#endif

#ifndef MCKL_HAS_RDRAND
#define MCKL_HAS_RDRAND 0
#endif

#ifndef MCKL_HAS_AVX2
#define MCKL_HAS_AVX2 0
#endif

#ifndef MCKL_USE_AVX2
#define MCKL_USE_AVX2 MCKL_HAS_AVX2
#endif

#ifndef MCKL_HAS_AVX
#define MCKL_HAS_AVX MCKL_HAS_AVX2
#endif

#ifndef MCKL_USE_AVX
#define MCKL_USE_AVX MCKL_HAS_AVX
#endif

#ifndef MCKL_HAS_SSE4_2
#define MCKL_HAS_SSE4_2 MCKL_HAS_AVX
#endif

#ifndef MCKL_USE_SSE4_2
#define MCKL_USE_SSE4_2 MCKL_HAS_SSE4_2
#endif

#ifndef MCKL_HAS_SSE4_1
#define MCKL_HAS_SSE4_1 MCKL_HAS_SSE4_2
#endif

#ifndef MCKL_USE_SSE4_1
#define MCKL_USE_SSE4_1 MCKL_HAS_SSE4_1
#endif

#ifndef MCKL_HAS_SSSE3
#define MCKL_HAS_SSSE3 MCKL_HAS_SSE4_1
#endif

#ifndef MCKL_USE_SSSE3
#define MCKL_USE_SSSE3 MCKL_HAS_SSSE3
#endif

#ifndef MCKL_HAS_SSE3
#define MCKL_HAS_SSE3 MCKL_HAS_SSSE3
#endif

#ifndef MCKL_USE_SSE3
#define MCKL_USE_SSE3 MCKL_HAS_SSE3
#endif

#ifndef MCKL_HAS_SSE2
#define MCKL_HAS_SSE2 MCKL_HAS_SSE3
#endif

#ifndef MCKL_USE_SSE2
#define MCKL_USE_SSE2 MCKL_HAS_SSE2
#endif

#ifndef MCKL_HAS_INT128
#define MCKL_HAS_INT128 0
#endif

#ifndef MCKL_INT64
#define MCKL_INT64 long long
#endif

#ifndef MCKL_FLATTEN_CALL_SITE
#define MCKL_FLATTEN_CALL_SITE
#endif

#ifndef MCKL_FLATTEN_DEFINITION
#define MCKL_FLATTEN_DEFINITION
#endif

#if MCKL_USE_SSE2
#ifdef MCKL_MSVC
#include <intrin.h>
#else
#include <emmintrin.h>
#endif
#endif

#if MCKL_USE_SSE3
#ifdef MCKL_MSVC
#include <intrin.h>
#else
#include <pmmintrin.h>
#endif
#endif

#if MCKL_USE_SSSE3
#ifdef MCKL_MSVC
#include <intrin.h>
#else
#include <tmmintrin.h>
#endif
#endif

#if MCKL_USE_SSE4_1
#ifdef MCKL_MSVC
#include <intrin.h>
#else
#include <smmintrin.h>
#endif
#endif

#if MCKL_USE_SSE4_2
#ifdef MCKL_MSVC
#include <intrin.h>
#else
#include <nmmintrin.h>
#endif
#endif

#if MCKL_USE_AVX || MCKL_USE_AVX2
#ifdef MCKL_MSVC
#include <intrin.h>
#else
#include <immintrin.h>
#endif
#endif

#endif // MCKL_INTERNAL_COMPILER_HPP
