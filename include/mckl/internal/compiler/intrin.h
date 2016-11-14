//============================================================================
// MCKL/include/mckl/internal/compiler/intrin.h
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

#ifndef MCKL_INTERNAL_COMPILER_INTRIN_H
#define MCKL_INTERNAL_COMPILER_INTRIN_H

#ifndef MCKL_HAS_AVX2
#define MCKL_HAS_AVX2 0
#endif

#ifndef MCKL_HAS_AVX
#define MCKL_HAS_AVX MCKL_HAS_AVX2
#endif

#ifndef MCKL_HAS_SSE4_2
#define MCKL_HAS_SSE4_2 MCKL_HAS_AVX
#endif

#ifndef MCKL_HAS_SSE4_1
#define MCKL_HAS_SSE4_1 MCKL_HAS_SSE4_2
#endif

#ifndef MCKL_HAS_SSSE3
#define MCKL_HAS_SSSE3 MCKL_HAS_SSE4_1
#endif

#ifndef MCKL_HAS_SSE3
#define MCKL_HAS_SSE3 MCKL_HAS_SSSE3
#endif

#ifndef MCKL_HAS_SSE2
#define MCKL_HAS_SSE2 MCKL_HAS_SSE3
#endif

#ifndef MCKL_HAS_AESNI
#define MCKL_HAS_AESNI MCKL_HAS_AVX
#endif

#ifndef MCKL_HAS_RDRAND
#define MCKL_HAS_RDRAND MCKL_HAS_AVX2
#endif

#ifndef MCKL_HAS_BMI
#define MCKL_HAS_BMI MCKL_HAS_AVX2
#endif

#ifndef MCKL_HAS_BMI2
#define MCKL_HAS_BMI2 MCKL_HAS_AVX2
#endif

#ifndef MCKL_HAS_FMA
#define MCKL_HAS_FMA MCKL_HAS_AVX2
#endif

#ifndef MCKL_HAS_RDPMC
#define MCKL_HAS_RDPMC MCKL_HAS_X86
#endif

#ifndef MCKL_HAS_RDTSC
#define MCKL_HAS_RDTSC MCKL_HAS_X86
#endif

#ifndef MCKL_HAS_RDTSCP
#define MCKL_HAS_RDTSCP MCKL_HAS_SSE4_2
#endif

#ifndef MCKL_USE_SSE2
#define MCKL_USE_SSE2 MCKL_HAS_SSE2
#endif

#ifndef MCKL_USE_SSE3
#define MCKL_USE_SSE3 MCKL_HAS_SSE3
#endif

#ifndef MCKL_USE_SSSE3
#define MCKL_USE_SSSE3 MCKL_HAS_SSSE3
#endif

#ifndef MCKL_USE_SSE4_1
#define MCKL_USE_SSE4_1 MCKL_HAS_SSE4_1
#endif

#ifndef MCKL_USE_SSE4_2
#define MCKL_USE_SSE4_2 MCKL_HAS_SSE4_2
#endif

#ifndef MCKL_USE_AVX
#define MCKL_USE_AVX MCKL_HAS_AVX
#endif

#ifndef MCKL_USE_AVX2
#define MCKL_USE_AVX2 MCKL_HAS_AVX2
#endif

#ifndef MCKL_HAS_AESNI
#define MCKL_HAS_AESNI MCKL_HAS_AVX2
#endif

#ifndef MCKL_USE_AESNI
#define MCKL_USE_AESNI MCKL_HAS_AESNI
#endif

#ifndef MCKL_USE_RDRAND
#define MCKL_USE_RDRAND MCKL_HAS_RDRAND
#endif

#ifndef MCKL_USE_BMI
#define MCKL_USE_BMI MCKL_HAS_BMI
#endif

#ifndef MCKL_USE_BMI2
#define MCKL_USE_BMI2 MCKL_HAS_BMI2
#endif

#ifndef MCKL_USE_FMA
#define MCKL_USE_FMA MCKL_HAS_FMA
#endif

#ifndef MCKL_USE_RDPMC
#define MCKL_USE_RDPMC 0
#endif

#ifndef MCKL_USE_RDTSC
#define MCKL_USE_RDTSC MCKL_HAS_RDTSC
#endif

#ifndef MCKL_USE_RDTSCP
#define MCKL_USE_RDTSCP MCKL_HAS_RDTSCP
#endif

#ifndef MCKL_INT64
#define MCKL_INT64 long long
#endif

#ifndef MCKL_HAS_INT128
#define MCKL_HAS_INT128 0
#endif

#endif // MCKL_INTERNAL_COMPILER_INTRIN_H
