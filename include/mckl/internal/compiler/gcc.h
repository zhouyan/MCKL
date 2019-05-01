//============================================================================
// MCKL/include/mckl/internal/compiler/gcc.h
//----------------------------------------------------------------------------
// MCKL: Monte Carlo Kernel Library
//----------------------------------------------------------------------------
// Copyright (c) 2013-2018, Yan Zhou
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

#ifndef MCKL_INTERNAL_COMPILER_GCC_H
#define MCKL_INTERNAL_COMPILER_GCC_H

#if MCKL_HAS_X86
#include <x86intrin.h>
#endif

#define MCKL_GCC_VERSION                                                      \
    (__GNUC__ * 10000 + __GNUC_MINOR__ * 100 + __GNUC_PATCHLEVEL__)

#if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
#ifndef MCKL_HAS_LITTLE_ENDIAN
#define MCKL_HAS_LITTLE_ENDIAN 1
#endif
#endif

#if __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__
#ifndef MCKL_HAS_BIG_ENDIAN
#define MCKL_HAS_BIG_ENDIAN 1
#endif
#endif

#ifdef __SSE2__
#ifndef MCKL_HAS_SSE2
#define MCKL_HAS_SSE2 1
#endif
#endif

#ifdef __SSE3__
#ifndef MCKL_HAS_SSE3
#define MCKL_HAS_SSE3 1
#endif
#endif

#ifdef __SSSE3__
#ifndef MCKL_HAS_SSSE3
#define MCKL_HAS_SSSE3 1
#endif
#endif

#ifdef __SSE4_1__
#ifndef MCKL_HAS_SSE4_1
#define MCKL_HAS_SSE4_1 1
#endif
#endif

#ifdef __SSE4_2__
#ifndef MCKL_HAS_SSE4_2
#define MCKL_HAS_SSE4_2 1
#endif
#endif

#ifdef __AVX__
#ifndef MCKL_HAS_AVX
#define MCKL_HAS_AVX 1
#endif
#endif

#ifdef __AVX2__
#ifndef MCKL_HAS_AVX2
#define MCKL_HAS_AVX2 1
#endif
#endif

#if defined(__AVX512BW__) && defined(__AVX512CD__) &&                         \
    defined(__AVX512DQ__) && defined(__AVX512F__) && defined(__AVX512VL__)
#ifndef MCKL_HAS_AVX512
#define MCKL_HAS_AVX512 1
#endif
#endif

#ifdef __AES__
#ifndef MCKL_HAS_AESNI
#define MCKL_HAS_AESNI 1
#endif
#endif

#ifdef __RDRND__
#ifndef MCKL_HAS_RDRAND
#define MCKL_HAS_RDRAND 1
#endif
#endif

#ifdef __BMI__
#ifndef MCKL_HAS_BMI
#define MCKL_HAS_BMI 1
#endif
#endif

#ifdef __BMI2__
#ifndef MCKL_HAS_BMI2
#define MCKL_HAS_BMI2 1
#endif
#endif

#ifdef __FMA__
#ifndef MCKL_HAS_FMA
#define MCKL_HAS_FMA 1
#endif
#endif

#ifndef MCKL_INT64
#define MCKL_INT64 long long
#endif

#if MCKL_HAS_X86_64
#ifndef MCKL_HAS_INT128
#define MCKL_HAS_INT128 1
#endif
#ifndef MCKL_INT128
#define MCKL_INT128 __int128
#endif
#endif

#ifndef MCKL_INLINE
#define MCKL_INLINE __attribute__((always_inline, flatten))
#endif

#endif // MCKL_INTERNAL_COMPILER_GCC_H
