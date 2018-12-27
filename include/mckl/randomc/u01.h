//============================================================================
// MCKL/include/mckl/randomc/u01.h
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

#ifndef MCKL_RANDOMC_U01_H
#define MCKL_RANDOMC_U01_H

#include <mckl/internal/config.h>

#ifndef UINT32_C
#error __STDC_CONSTANT_MACROS not defined before #include<stdint.h>
#endif

#ifdef MCKL_OPENCL
#define MCKL_RANDOMC_U01_23F (1.0f / 8388608.0f)
#define MCKL_RANDOMC_U01_24F (1.0f / 16777216.0f)
#define MCKL_RANDOMC_U01_31F (1.0f / 2147483648.0f)
#define MCKL_RANDOMC_U01_32F (1.0f / 4294967296.0f)
#define MCKL_RANDOMC_U01_33F (1.0f / 8589934592.0f)
#define MCKL_RANDOMC_U01_64F (1.0f / 18446744073709551616.0f)
#define MCKL_RANDOMC_U01_65F (1.0f / 36893488147419103232.0f)
#if MCKL_HAS_OPENCL_DOUBLE
#define MCKL_RANDOMC_U01_32D (1.0 / 4294967296.0)
#define MCKL_RANDOMC_U01_33D (1.0 / 8589934592.0)
#define MCKL_RANDOMC_U01_52D (1.0 / 4503599627370496.0)
#define MCKL_RANDOMC_U01_53D (1.0 / 9007199254740992.0)
#define MCKL_RANDOMC_U01_63D (1.0 / 9223372036854775808.0)
#define MCKL_RANDOMC_U01_64D (1.0 / 18446744073709551616.0)
#define MCKL_RANDOMC_U01_65D (1.0 / 36893488147419103232.0)
#endif // MCKL_HAS_OPENCL_DOUBLE
#else  // MCKL_OPENCL
static const float MCKL_RANDOMC_U01_23F = 1.0f / 8388608.0f;
static const float MCKL_RANDOMC_U01_24F = 1.0f / 16777216.0f;
static const float MCKL_RANDOMC_U01_31F = 1.0f / 2147483648.0f;
static const float MCKL_RANDOMC_U01_32F = 1.0f / 4294967296.0f;
static const float MCKL_RANDOMC_U01_33F = 1.0f / 8589934592.0f;
static const float MCKL_RANDOMC_U01_64F = 1.0f / 18446744073709551616.0f;
static const float MCKL_RANDOMC_U01_65F = 1.0f / 36893488147419103232.0f;
static const double MCKL_RANDOMC_U01_32D = 1.0 / 4294967296.0;
static const double MCKL_RANDOMC_U01_33D = 1.0 / 8589934592.0;
static const double MCKL_RANDOMC_U01_52D = 1.0 / 4503599627370496.0;
static const double MCKL_RANDOMC_U01_53D = 1.0 / 9007199254740992.0;
static const double MCKL_RANDOMC_U01_63D = 1.0 / 9223372036854775808.0;
static const double MCKL_RANDOMC_U01_64D = 1.0 / 18446744073709551616.0;
static const double MCKL_RANDOMC_U01_65D = 1.0 / 36893488147419103232.0;
#endif // MCKL_OPENCL

/// \brief Converting 32-bit unsigned to single precision uniform \f$(0, 1)\f$
/// \ingroup U01C
static inline float mckl_u01_u32f(uint32_t u)
{
    return u * MCKL_RANDOMC_U01_32F + MCKL_RANDOMC_U01_33F;
}

/// \brief Converting 64-bit unsigned to single precision uniform \f$(0, 1)\f$
/// \ingroup U01C
static inline float mckl_u01_u64f(uint64_t u)
{
    return u * MCKL_RANDOMC_U01_64F + MCKL_RANDOMC_U01_65F;
}

#if !defined(MCKL_OPENCL) || MCKL_HAS_OPENCL_DOUBLE

/// \brief Converting 32-bit unsigned to double precision uniform \f$(0, 1)\f$
/// \ingroup U01C
static inline double mckl_u01_u32d(uint32_t u)
{
    return u * MCKL_RANDOMC_U01_32D + MCKL_RANDOMC_U01_33D;
}

/// \brief Converting 64-bit unsigned to double precision uniform \f$(0, 1)\f$
/// \ingroup U01C
static inline double mckl_u01_u64d(uint64_t u)
{
    return u * MCKL_RANDOMC_U01_64D + MCKL_RANDOMC_U01_65D;
}

#endif // !defined(MCKL_OPENCL) || MCKL_HAS_OPENCL_DOUBLE

/// \brief Converting 32-bit unsigned to single precision uniform \f$[0,1]\f$
/// \ingroup U01C
static inline float mckl_u01_cc_u32f(uint32_t u)
{
    return ((u & UINT32_C(0x40)) + (u & UINT32_C(0x7FFFFFC0))) *
        MCKL_RANDOMC_U01_31F;
}

/// \brief Converting 32-bit unsigned to single precision uniform \f$[0,1)\f$
/// \ingroup U01C
static inline float mckl_u01_co_u32f(uint32_t u)
{
    return (u >> 8) * MCKL_RANDOMC_U01_24F;
}

/// \brief Converting 32-bit unsigned to single precision uniform \f$(0,1]\f$
/// \ingroup U01C
static inline float mckl_u01_oc_u32f(uint32_t u)
{
    return (u >> 8) * MCKL_RANDOMC_U01_24F + MCKL_RANDOMC_U01_24F;
}

/// \brief Converting 32-bit unsigned to single precision uniform \f$(0,1)\f$
/// \ingroup U01C
static inline float mckl_u01_oo_u32f(uint32_t u)
{
    return (u >> 9) * MCKL_RANDOMC_U01_23F + MCKL_RANDOMC_U01_24F;
}

/// \brief Converting 64-bit unsigned to single precision uniform \f$[0,1]\f$
/// \ingroup U01C
static inline float mckl_u01_cc_u64f(uint64_t u)
{
#ifdef __cplusplus
    return mckl_u01_cc_u32f(static_cast<uint32_t>(u >> 32));
#else
    return mckl_u01_cc_u32f(((uint32_t)(u >> 32)));
#endif
}

/// \brief Converting 64-bit unsigned to single precision uniform \f$[0,1)\f$
/// \ingroup U01C
static inline float mckl_u01_co_u64f(uint64_t u)
{
#ifdef __cplusplus
    return mckl_u01_co_u32f(static_cast<uint32_t>(u >> 32));
#else
    return mckl_u01_co_u32f(((uint32_t)(u >> 32)));
#endif
}

/// \brief Converting 64-bit unsigned to single precision uniform \f$(0,1]\f$
/// \ingroup U01C
static inline float mckl_u01_oc_u64f(uint64_t u)
{
#ifdef __cplusplus
    return mckl_u01_oc_u32f(static_cast<uint32_t>(u >> 32));
#else
    return mckl_u01_oc_u32f(((uint32_t)(u >> 32)));
#endif
}

/// \brief Converting 64-bit unsigned to single precision uniform \f$(0,1)\f$
/// \ingroup U01C
static inline float mckl_u01_oo_u64f(uint64_t u)
{
#ifdef __cplusplus
    return mckl_u01_oo_u32f(static_cast<uint32_t>(u >> 32));
#else
    return mckl_u01_oo_u32f(((uint32_t)(u >> 32)));
#endif
}

#if !defined(MCKL_OPENCL) || MCKL_HAS_OPENCL_DOUBLE

/// \brief Converting 32-bit unsigned to double precision uniform \f$[0,1]\f$
/// \ingroup U01C
static inline double mckl_u01_cc_u32d(uint32_t u)
{
#ifdef __cplusplus
    return (static_cast<double>(u & 1) + u) * MCKL_RANDOMC_U01_32D;
#else
    return (((double) (u & 1)) + u) * MCKL_RANDOMC_U01_32D;
#endif
}

/// \brief Converting 32-bit unsigned to double precision uniform \f$[0,1)\f$
/// \ingroup U01C
static inline double mckl_u01_co_u32d(uint32_t u)
{
    return u * MCKL_RANDOMC_U01_32D;
}

/// \brief Converting 32-bit unsigned to double precision uniform \f$(0,1]\f$
/// \ingroup U01C
static inline double mckl_u01_oc_u32d(uint32_t u)
{
    return MCKL_RANDOMC_U01_32D + u * MCKL_RANDOMC_U01_32D;
}

/// \brief Converting 32-bit unsigned to double precision uniform \f$(0,1)\f$
/// \ingroup U01C
static inline double mckl_u01_oo_u32d(uint32_t u)
{
    return MCKL_RANDOMC_U01_33D + u * MCKL_RANDOMC_U01_32D;
}

/// \brief Converting 64-bit unsigned to double precision uniform \f$[0,1]\f$
/// \ingroup U01C
static inline double mckl_u01_cc_u64d(uint64_t u)
{
    return ((u & UINT64_C(0x7FFFFFFFFFFFFE00)) + (u & UINT64_C(0x200))) *
        MCKL_RANDOMC_U01_63D;
}

/// \brief Converting 64-bit unsigned to double precision uniform \f$[0,1)\f$
/// \ingroup U01C
static inline double mckl_u01_co_u64d(uint64_t u)
{
    return (u >> 11) * MCKL_RANDOMC_U01_53D;
}

/// \brief Converting 64-bit unsigned to double precision uniform \f$(0,1]\f$
/// \ingroup U01C
static inline double mckl_u01_oc_u64d(uint64_t u)
{
    return MCKL_RANDOMC_U01_53D + (u >> 11) * MCKL_RANDOMC_U01_53D;
}

/// \brief Converting 64-bit unsigned to double precision uniform \f$(0,1)\f$
/// \ingroup U01C
static inline double mckl_u01_oo_u64d(uint64_t u)
{
    return MCKL_RANDOMC_U01_53D + (u >> 12) * MCKL_RANDOMC_U01_52D;
}

#endif // !defined(MCKL_OPENCL) || MCKL_HAS_OPENCL_DOUBLE

#endif // MCKL_RANDOMC_U01_H
