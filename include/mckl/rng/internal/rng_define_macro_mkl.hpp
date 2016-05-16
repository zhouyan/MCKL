//============================================================================
// MCKL/include/rng/internal/rng_define_macro_mkl.hpp
//----------------------------------------------------------------------------
//                         MCKL: Monte Carlo Kernel Library
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

#ifndef MCKL_RNG_DEFINE_MACRO_NA
#define MCKL_RNG_DEFINE_MACRO_NA(RNGType, Name, name)
#endif

#if MCKL_HAS_MKL
#define MCKL_RNG_DEFINE_MACRO_MKL MCKL_RNG_DEFINE_MACRO
#else
#define MCKL_RNG_DEFINE_MACRO_MKL MCKL_RNG_DEFINE_MACRO_NA
#endif

MCKL_RNG_DEFINE_MACRO_MKL(::mckl::MKL_MCG59, MKL_MCG59, mkl_mcg69)
MCKL_RNG_DEFINE_MACRO_MKL(::mckl::MKL_MCG59_64, MKL_MCG59_64, mkl_mcg69_64)
MCKL_RNG_DEFINE_MACRO_MKL(::mckl::MKL_MT19937, MKL_MT19937, mkl_mt19937)
MCKL_RNG_DEFINE_MACRO_MKL(
    ::mckl::MKL_MT19937_64, MKL_MT19937_64, mkl_mt19937_64)
MCKL_RNG_DEFINE_MACRO_MKL(::mckl::MKL_MT2203, MKL_MT2203, mkl_mt2203)
MCKL_RNG_DEFINE_MACRO_MKL(::mckl::MKL_MT2203_64, MKL_MT2203_64, mkl_mt2203_64)
MCKL_RNG_DEFINE_MACRO_MKL(::mckl::MKL_SFMT19937, MKL_STMT19937, mkl_sftmt19937)
MCKL_RNG_DEFINE_MACRO_MKL(
    ::mckl::MKL_SFMT19937_64, MKL_STMT19937_64, mkl_sftmt19937_64)

#undef MCKL_RNG_DEFINE_MACRO_MKL

#if MCKL_HAS_MKL && MCKL_HAS_RDRAND
#define MCKL_RNG_DEFINE_MACRO_MKL MCKL_RNG_DEFINE_MACRO
#else
#define MCKL_RNG_DEFINE_MACRO_MKL MCKL_RNG_DEFINE_MACRO_NA
#endif

MCKL_RNG_DEFINE_MACRO_MKL(::mckl::MKL_NONDETERM, MKL_NONDETERM, mkl_nondeterm)
MCKL_RNG_DEFINE_MACRO_MKL(
    ::mckl::MKL_NONDETERM_64, MKL_NONDETERM_64, mkl_nondeterm_64)

#undef MCKL_RNG_DEFINE_MACRO_MKL

#if MCKL_HAS_MKL && INTEL_MKL_VERSION >= 110300
#define MCKL_RNG_DEFINE_MACRO_MKL MCKL_RNG_DEFINE_MACRO
#else
#define MCKL_RNG_DEFINE_MACRO_MKL MCKL_RNG_DEFINE_MACRO_NA
#endif

MCKL_RNG_DEFINE_MACRO_MKL(
    ::mckl::MKL_PHILOX4X32X10, MKL_PHILOX4X32X10, mkl_philox4x32x10)
MCKL_RNG_DEFINE_MACRO_MKL(
    ::mckl::MKL_PHILOX4X32X10_64, MKL_PHILOX4X32X10_64, mkl_philox4x32x10_64)

#undef MCKL_RNG_DEFINE_MACRO_MKL

#if MCKL_HAS_MKL && MCKL_HAS_AES_NI && INTEL_MKL_VERSION >= 110300
#define MCKL_RNG_DEFINE_MACRO_MKL MCKL_RNG_DEFINE_MACRO
#else
#define MCKL_RNG_DEFINE_MACRO_MKL MCKL_RNG_DEFINE_MACRO_NA
#endif

MCKL_RNG_DEFINE_MACRO_MKL(::mckl::MKL_ARS5, MKL_ARS5, mkl_ars5)
MCKL_RNG_DEFINE_MACRO_MKL(::mckl::MKL_ARS5_64, MKL_ARS5_64, mkl_ars5_64)

#undef MCKL_RNG_DEFINE_MACRO_MKL
