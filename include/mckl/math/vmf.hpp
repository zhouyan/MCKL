//============================================================================
// MCKL/include/mckl/math/vmf.hpp
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

#ifndef MCKL_MATH_VMF_HPP
#define MCKL_MATH_VMF_HPP

#include <mckl/internal/config.h>

#include <mckl/internal/assert.hpp>
#include <mckl/math/constants.hpp>
#include <mckl/math/erf.hpp>

#include <algorithm>
#include <array>
#include <cmath>
#include <complex>

#if MCKL_USE_MKL_VML

#include <mkl_vml.h>

#define MCKL_DEFINE_MATH_VMF_VML_1R(func, name)                               \
    inline void name(std::size_t n, const float *a, float *y)                 \
    {                                                                         \
        internal::size_check<MKL_INT>(n, #name);                              \
        ::vs##func(static_cast<MKL_INT>(n), a, y);                            \
    }                                                                         \
                                                                              \
    inline void name(std::size_t n, const double *a, double *y)               \
    {                                                                         \
        internal::size_check<MKL_INT>(n, #name);                              \
        ::vd##func(static_cast<MKL_INT>(n), a, y);                            \
    }

#define MCKL_DEFINE_MATH_VMF_VML_1C(func, name)                               \
    inline void name(                                                         \
        std::size_t n, const std::complex<float> *a, std::complex<float> *y)  \
    {                                                                         \
        internal::size_check<MKL_INT>(n, #name);                              \
        ::vc##func(static_cast<MKL_INT>(n),                                   \
            reinterpret_cast<const ::MKL_Complex8 *>(a),                      \
            reinterpret_cast<::MKL_Complex8 *>(y));                           \
    }                                                                         \
                                                                              \
    inline void name(std::size_t n, const std::complex<double> *a,            \
        std::complex<double> *y)                                              \
    {                                                                         \
        internal::size_check<MKL_INT>(n, #name);                              \
        ::vz##func(static_cast<MKL_INT>(n),                                   \
            reinterpret_cast<const ::MKL_Complex16 *>(a),                     \
            reinterpret_cast<::MKL_Complex16 *>(y));                          \
    }

#define MCKL_DEFINE_MATH_VMF_VML_2R(func, name)                               \
    inline void name(std::size_t n, const float *a, const float *b, float *y) \
    {                                                                         \
        internal::size_check<MKL_INT>(n, #name);                              \
        ::vs##func(static_cast<MKL_INT>(n), a, b, y);                         \
    }                                                                         \
                                                                              \
    inline void name(                                                         \
        std::size_t n, const double *a, const double *b, double *y)           \
    {                                                                         \
        internal::size_check<MKL_INT>(n, #name);                              \
        ::vd##func(static_cast<MKL_INT>(n), a, b, y);                         \
    }

#define MCKL_DEFINE_MATH_VMF_VML_2C(func, name)                               \
    inline void name(std::size_t n, const std::complex<float> *a,             \
        const std::complex<float> *b, std::complex<float> *y)                 \
    {                                                                         \
        internal::size_check<MKL_INT>(n, #name);                              \
        ::vc##func(static_cast<MKL_INT>(n),                                   \
            reinterpret_cast<const ::MKL_Complex8 *>(a),                      \
            reinterpret_cast<const ::MKL_Complex8 *>(b),                      \
            reinterpret_cast<::MKL_Complex8 *>(y));                           \
    }                                                                         \
                                                                              \
    inline void name(std::size_t n, const std::complex<double> *a,            \
        const std::complex<double> *b, std::complex<double> *y)               \
    {                                                                         \
        internal::size_check<MKL_INT>(n, #name);                              \
        ::vz##func(static_cast<MKL_INT>(n),                                   \
            reinterpret_cast<const ::MKL_Complex16 *>(a),                     \
            reinterpret_cast<const ::MKL_Complex16 *>(b),                     \
            reinterpret_cast<::MKL_Complex16 *>(y));                          \
    }

namespace mckl
{

MCKL_DEFINE_MATH_VMF_VML_2R(Add, add)
MCKL_DEFINE_MATH_VMF_VML_2C(Add, add)
MCKL_DEFINE_MATH_VMF_VML_2R(Sub, sub)
MCKL_DEFINE_MATH_VMF_VML_2C(Sub, sub)
MCKL_DEFINE_MATH_VMF_VML_1R(Sqr, sqr)
MCKL_DEFINE_MATH_VMF_VML_2R(Mul, mul)
MCKL_DEFINE_MATH_VMF_VML_2C(Mul, mul)
MCKL_DEFINE_MATH_VMF_VML_2C(MulByConj, mulbyconj)
MCKL_DEFINE_MATH_VMF_VML_1C(Conj, conj)
MCKL_DEFINE_MATH_VMF_VML_1R(Abs, abs)

inline void abs(std::size_t n, const std::complex<float> *a, float *y)
{
    internal::size_check<MKL_INT>(n, "abs");
    ::vcAbs(static_cast<MKL_INT>(n),
        reinterpret_cast<const ::MKL_Complex8 *>(a), y);
}

inline void abs(std::size_t n, const std::complex<double> *a, double *y)
{
    internal::size_check<MKL_INT>(n, "abs");
    ::vzAbs(static_cast<MKL_INT>(n),
        reinterpret_cast<const ::MKL_Complex16 *>(a), y);
}

inline void arg(std::size_t n, const std::complex<float> *a, float *y)
{
    internal::size_check<MKL_INT>(n, "arg");
    ::vcArg(static_cast<MKL_INT>(n),
        reinterpret_cast<const ::MKL_Complex8 *>(a), y);
}

inline void arg(std::size_t n, const std::complex<double> *a, double *y)
{
    internal::size_check<MKL_INT>(n, "arg");
    ::vzArg(static_cast<MKL_INT>(n),
        reinterpret_cast<const ::MKL_Complex16 *>(a), y);
}

inline void linear_frac(std::size_t n, const float *a, const float *b,
    float beta_a, float beta_b, float mu_a, float mu_b, float *y)
{
    internal::size_check<MKL_INT>(n, "linear_frac");
    ::vsLinearFrac(
        static_cast<MKL_INT>(n), a, b, beta_a, beta_b, mu_a, mu_b, y);
}

inline void linear_frac(std::size_t n, const double *a, const double *b,
    double beta_a, double beta_b, double mu_a, double mu_b, double *y)
{
    internal::size_check<MKL_INT>(n, "linear_frac");
    ::vdLinearFrac(
        static_cast<MKL_INT>(n), a, b, beta_a, beta_b, mu_a, mu_b, y);
}

MCKL_DEFINE_MATH_VMF_VML_1R(Inv, inv)
MCKL_DEFINE_MATH_VMF_VML_2R(Div, div)
MCKL_DEFINE_MATH_VMF_VML_2C(Div, div)
MCKL_DEFINE_MATH_VMF_VML_1R(Sqrt, sqrt)
MCKL_DEFINE_MATH_VMF_VML_1C(Sqrt, sqrt)
MCKL_DEFINE_MATH_VMF_VML_1R(InvSqrt, invsqrt)
MCKL_DEFINE_MATH_VMF_VML_1R(Cbrt, cbrt)
MCKL_DEFINE_MATH_VMF_VML_1R(InvCbrt, invcbrt)
MCKL_DEFINE_MATH_VMF_VML_1R(Pow2o3, pow2o3)
MCKL_DEFINE_MATH_VMF_VML_1R(Pow3o2, pow3o2)
MCKL_DEFINE_MATH_VMF_VML_2R(Pow, pow)
MCKL_DEFINE_MATH_VMF_VML_2C(Pow, pow)

inline void pow(std::size_t n, const float *a, float b, float *y)
{
    internal::size_check<MKL_INT>(n, "pow");
    ::vsPowx(static_cast<MKL_INT>(n), a, b, y);
}

inline void pow(std::size_t n, const double *a, double b, double *y)
{
    internal::size_check<MKL_INT>(n, "pow");
    ::vdPowx(static_cast<MKL_INT>(n), a, b, y);
}

inline void pow(std::size_t n, const std::complex<float> *a,
    std::complex<float> b, std::complex<float> *y)
{
    internal::size_check<MKL_INT>(n, "pow");
    ::MKL_Complex8 c = {b.real(), b.imag()};
    ::vcPowx(static_cast<MKL_INT>(n),
        reinterpret_cast<const ::MKL_Complex8 *>(a), c,
        reinterpret_cast<::MKL_Complex8 *>(y));
}

inline void pow(std::size_t n, const std::complex<double> *a,
    std::complex<double> b, std::complex<double> *y)
{
    internal::size_check<MKL_INT>(n, "pow");
    ::MKL_Complex16 c = {b.real(), b.imag()};
    ::vzPowx(static_cast<MKL_INT>(n),
        reinterpret_cast<const ::MKL_Complex16 *>(a), c,
        reinterpret_cast<::MKL_Complex16 *>(y));
}

MCKL_DEFINE_MATH_VMF_VML_2R(Hypot, hypot)

MCKL_DEFINE_MATH_VMF_VML_1R(Exp, exp)
MCKL_DEFINE_MATH_VMF_VML_1C(Exp, exp)
MCKL_DEFINE_MATH_VMF_VML_1R(Expm1, expm1)
MCKL_DEFINE_MATH_VMF_VML_1R(Ln, log)
MCKL_DEFINE_MATH_VMF_VML_1C(Ln, log)
MCKL_DEFINE_MATH_VMF_VML_1R(Log10, log10)
MCKL_DEFINE_MATH_VMF_VML_1C(Log10, log10)
MCKL_DEFINE_MATH_VMF_VML_1R(Log1p, log1p)

MCKL_DEFINE_MATH_VMF_VML_1R(Cos, cos)
MCKL_DEFINE_MATH_VMF_VML_1C(Cos, cos)
MCKL_DEFINE_MATH_VMF_VML_1R(Sin, sin)
MCKL_DEFINE_MATH_VMF_VML_1C(Sin, sin)

inline void sincos(std::size_t n, const float *a, float *y, float *z)
{
    internal::size_check<MKL_INT>(n, "sincos");
    ::vsSinCos(static_cast<MKL_INT>(n), a, y, z);
}

inline void sincos(std::size_t n, const double *a, double *y, double *z)
{
    internal::size_check<MKL_INT>(n, "sincos");
    ::vdSinCos(static_cast<MKL_INT>(n), a, y, z);
}

inline void cis(std::size_t n, const float *a, std::complex<float> *y)
{
    internal::size_check<MKL_INT>(n, "cis");
    ::vcCIS(static_cast<MKL_INT>(n), a, reinterpret_cast<::MKL_Complex8 *>(y));
}

inline void cis(std::size_t n, const double *a, std::complex<double> *y)
{
    internal::size_check<MKL_INT>(n, "cis");
    ::vzCIS(
        static_cast<MKL_INT>(n), a, reinterpret_cast<::MKL_Complex16 *>(y));
}

MCKL_DEFINE_MATH_VMF_VML_1R(Tan, tan)
MCKL_DEFINE_MATH_VMF_VML_1C(Tan, tan)
MCKL_DEFINE_MATH_VMF_VML_1R(Acos, acos)
MCKL_DEFINE_MATH_VMF_VML_1C(Acos, acos)
MCKL_DEFINE_MATH_VMF_VML_1R(Asin, asin)
MCKL_DEFINE_MATH_VMF_VML_1C(Asin, asin)
MCKL_DEFINE_MATH_VMF_VML_1R(Atan, atan)
MCKL_DEFINE_MATH_VMF_VML_1C(Atan, atan)
MCKL_DEFINE_MATH_VMF_VML_2R(Atan2, atan2)

MCKL_DEFINE_MATH_VMF_VML_1R(Cosh, cosh)
MCKL_DEFINE_MATH_VMF_VML_1C(Cosh, cosh)
MCKL_DEFINE_MATH_VMF_VML_1R(Sinh, sinh)
MCKL_DEFINE_MATH_VMF_VML_1C(Sinh, sinh)
MCKL_DEFINE_MATH_VMF_VML_1R(Tanh, tanh)
MCKL_DEFINE_MATH_VMF_VML_1C(Tanh, tanh)
MCKL_DEFINE_MATH_VMF_VML_1R(Acosh, acosh)
MCKL_DEFINE_MATH_VMF_VML_1C(Acosh, acosh)
MCKL_DEFINE_MATH_VMF_VML_1R(Asinh, asinh)
MCKL_DEFINE_MATH_VMF_VML_1C(Asinh, asinh)
MCKL_DEFINE_MATH_VMF_VML_1R(Atanh, atanh)
MCKL_DEFINE_MATH_VMF_VML_1C(Atanh, atanh)

MCKL_DEFINE_MATH_VMF_VML_1R(Erf, erf)
MCKL_DEFINE_MATH_VMF_VML_1R(Erfc, erfc)
MCKL_DEFINE_MATH_VMF_VML_1R(CdfNorm, cdfnorm)
MCKL_DEFINE_MATH_VMF_VML_1R(ErfInv, erfinv)
MCKL_DEFINE_MATH_VMF_VML_1R(ErfcInv, erfcinv)
MCKL_DEFINE_MATH_VMF_VML_1R(CdfNormInv, cdfnorminv)
MCKL_DEFINE_MATH_VMF_VML_1R(LGamma, lgamma)
MCKL_DEFINE_MATH_VMF_VML_1R(TGamma, tgamma)

MCKL_DEFINE_MATH_VMF_VML_1R(Floor, floor)
MCKL_DEFINE_MATH_VMF_VML_1R(Ceil, ceil)
MCKL_DEFINE_MATH_VMF_VML_1R(Trunc, trunc)
MCKL_DEFINE_MATH_VMF_VML_1R(Round, round)
MCKL_DEFINE_MATH_VMF_VML_1R(NearbyInt, nearbyint)
MCKL_DEFINE_MATH_VMF_VML_1R(Rint, rint)

inline void modf(std::size_t n, const float *a, float *y, float *z)
{
    internal::size_check<MKL_INT>(n, "modf");
    ::vsModf(static_cast<MKL_INT>(n), a, y, z);
}

inline void modf(std::size_t n, const double *a, double *y, double *z)
{
    internal::size_check<MKL_INT>(n, "modf");
    ::vdModf(static_cast<MKL_INT>(n), a, y, z);
}

} // namespace mckl

#elif MCKL_USE_ASM_LIBRARY && MCKL_USE_ASM_VMF && MCKL_USE_FMA

#define MCKL_DEFINE_MATH_VMF_ASM_1(func)                                      \
    inline void func(std::size_t n, const double *a, double *y)               \
    {                                                                         \
        ::mckl_vd_##func(n, a, y);                                            \
    }

#define MCKL_DEFINE_MATH_VMF_ASM_2(func)                                      \
    inline void func(                                                         \
        std::size_t n, const double *a, const double *b, double *y)           \
    {                                                                         \
        ::mckl_vd_##func(n, a, b, y);                                         \
    }

namespace mckl
{

MCKL_DEFINE_MATH_VMF_ASM_1(sqrt)

MCKL_DEFINE_MATH_VMF_ASM_1(exp)
MCKL_DEFINE_MATH_VMF_ASM_1(exp2)
MCKL_DEFINE_MATH_VMF_ASM_1(expm1)
MCKL_DEFINE_MATH_VMF_ASM_1(log)
MCKL_DEFINE_MATH_VMF_ASM_1(log2)
MCKL_DEFINE_MATH_VMF_ASM_1(log10)
MCKL_DEFINE_MATH_VMF_ASM_1(log1p)

MCKL_DEFINE_MATH_VMF_ASM_1(cos)
MCKL_DEFINE_MATH_VMF_ASM_1(sin)

inline void sincos(std::size_t n, const double *a, double *y, double *z)
{
    ::mckl_vd_sincos(n, a, y, z);
}

MCKL_DEFINE_MATH_VMF_ASM_1(tan)

} // namespace mckl

#endif // MCKL_USE_MKL_VML

#if MCKL_USE_ASM_LIBRARY && MCKL_USE_ASM_FMA

#define MCKL_DEFINE_MATH_FMA_FMA(op, name)                                    \
    inline void name(std::size_t n, const float *a, const float *b,           \
        const float *c, float *y)                                             \
    {                                                                         \
        ::mckl_##op##_vvv_ps(n, a, b, c, y);                                  \
    }                                                                         \
                                                                              \
    inline void name(                                                         \
        std::size_t n, const float *a, const float *b, float c, float *y)     \
    {                                                                         \
        ::mckl_##op##_vvs_ps(n, a, b, c, y);                                  \
    }                                                                         \
                                                                              \
    inline void name(                                                         \
        std::size_t n, const float *a, float b, const float *c, float *y)     \
    {                                                                         \
        ::mckl_##op##_vsv_ps(n, a, b, c, y);                                  \
    }                                                                         \
                                                                              \
    inline void name(                                                         \
        std::size_t n, float a, const float *b, const float *c, float *y)     \
    {                                                                         \
        ::mckl_##op##_svv_ps(n, a, b, c, y);                                  \
    }                                                                         \
                                                                              \
    inline void name(                                                         \
        std::size_t n, float a, float b, const float *c, float *y)            \
    {                                                                         \
        ::mckl_##op##_ssv_ps(n, a, b, c, y);                                  \
    }                                                                         \
                                                                              \
    inline void name(                                                         \
        std::size_t n, float a, const float *b, float c, float *y)            \
    {                                                                         \
        ::mckl_##op##_svs_ps(n, a, b, c, y);                                  \
    }                                                                         \
                                                                              \
    inline void name(                                                         \
        std::size_t n, const float *a, float b, float c, float *y)            \
    {                                                                         \
        ::mckl_##op##_vss_ps(n, a, b, c, y);                                  \
    }                                                                         \
                                                                              \
    inline void name(std::size_t n, const double *a, const double *b,         \
        const double *c, double *y)                                           \
    {                                                                         \
        ::mckl_##op##_vvv_pd(n, a, b, c, y);                                  \
    }                                                                         \
                                                                              \
    inline void name(                                                         \
        std::size_t n, const double *a, const double *b, double c, double *y) \
    {                                                                         \
        ::mckl_##op##_vvs_pd(n, a, b, c, y);                                  \
    }                                                                         \
                                                                              \
    inline void name(                                                         \
        std::size_t n, const double *a, double b, const double *c, double *y) \
    {                                                                         \
        ::mckl_##op##_vsv_pd(n, a, b, c, y);                                  \
    }                                                                         \
                                                                              \
    inline void name(                                                         \
        std::size_t n, double a, const double *b, const double *c, double *y) \
    {                                                                         \
        ::mckl_##op##_svv_pd(n, a, b, c, y);                                  \
    }                                                                         \
                                                                              \
    inline void name(                                                         \
        std::size_t n, double a, double b, const double *c, double *y)        \
    {                                                                         \
        ::mckl_##op##_ssv_pd(n, a, b, c, y);                                  \
    }                                                                         \
                                                                              \
    inline void name(                                                         \
        std::size_t n, double a, const double *b, double c, double *y)        \
    {                                                                         \
        ::mckl_##op##_svs_pd(n, a, b, c, y);                                  \
    }                                                                         \
                                                                              \
    inline void name(                                                         \
        std::size_t n, const double *a, double b, double c, double *y)        \
    {                                                                         \
        ::mckl_##op##_vss_pd(n, a, b, c, y);                                  \
    }

namespace mckl
{

MCKL_DEFINE_MATH_FMA_FMA(fmadd, muladd)
MCKL_DEFINE_MATH_FMA_FMA(fmsub, mulsub)
MCKL_DEFINE_MATH_FMA_FMA(fnmadd, nmuladd)
MCKL_DEFINE_MATH_FMA_FMA(fnmsub, nmulsub)

MCKL_DEFINE_MATH_FMA_FMA(fmadd, fmadd)
MCKL_DEFINE_MATH_FMA_FMA(fmsub, fmsub)
MCKL_DEFINE_MATH_FMA_FMA(fnmadd, fnmadd)
MCKL_DEFINE_MATH_FMA_FMA(fnmsub, fnmsub)

} // namespace mckl

#endif // MCKL_USE_ASM_LIBRARY && MCKL_USE_ASM_FMA

#define MCKL_DEFINE_MATH_VMF_1(func, name)                                    \
    template <typename T>                                                     \
    inline void name(std::size_t n, const T *a, T *y)                         \
    {                                                                         \
        for (std::size_t i = 0; i != n; ++i)                                  \
            y[i] = func(a[i]);                                                \
    }

#define MCKL_DEFINE_MATH_VMF_2(func, name)                                    \
    template <typename T>                                                     \
    inline void name(std::size_t n, const T *a, const T *b, T *y)             \
    {                                                                         \
        for (std::size_t i = 0; i != n; ++i)                                  \
            y[i] = func(a[i], b[i]);                                          \
    }

#define MCKL_DEFINE_MATH_VMF_2VS(func, name)                                  \
    template <typename T>                                                     \
    inline void name(std::size_t n, const T *a, T b, T *y)                    \
    {                                                                         \
        for (std::size_t i = 0; i != n; ++i)                                  \
            y[i] = func(a[i], b);                                             \
    }

#define MCKL_DEFINE_MATH_VMF_2SV(func, name)                                  \
    template <typename T>                                                     \
    inline void name(std::size_t n, T a, const T *b, T *y)                    \
    {                                                                         \
        for (std::size_t i = 0; i != n; ++i)                                  \
            y[i] = func(a, b[i]);                                             \
    }

#define MCKL_DEFINE_MATH_VMF_B(op, name)                                      \
    template <typename T>                                                     \
    inline void name(std::size_t n, const T *a, const T *b, T *y)             \
    {                                                                         \
        for (std::size_t i = 0; i != n; ++i)                                  \
            y[i] = a[i] op b[i];                                              \
    }

#define MCKL_DEFINE_MATH_VMF_BVS(op, name)                                    \
    template <typename T>                                                     \
    inline void name(std::size_t n, const T *a, T b, T *y)                    \
    {                                                                         \
        for (std::size_t i = 0; i != n; ++i)                                  \
            y[i] = a[i] op b;                                                 \
    }

#define MCKL_DEFINE_MATH_VMF_BSV(op, name)                                    \
    template <typename T>                                                     \
    inline void name(std::size_t n, T a, const T *b, T *y)                    \
    {                                                                         \
        for (std::size_t i = 0; i != n; ++i)                                  \
            y[i] = a op b[i];                                                 \
    }

#define MCKL_DEFINE_MATH_VMF_FMA(op, name, attr)                              \
    template <typename T>                                                     \
    attr inline void name(                                                    \
        std::size_t n, const T *a, const T *b, const T *c, T *y)              \
    {                                                                         \
        for (std::size_t i = 0; i != n; ++i)                                  \
            y[i] = op(a[i], b[i], c[i]);                                      \
    }                                                                         \
                                                                              \
    template <typename T>                                                     \
    attr inline void name(std::size_t n, const T *a, const T *b, T c, T *y)   \
    {                                                                         \
        for (std::size_t i = 0; i != n; ++i)                                  \
            y[i] = op(a[i], b[i], c);                                         \
    }                                                                         \
                                                                              \
    template <typename T>                                                     \
    attr inline void name(std::size_t n, const T *a, T b, const T *c, T *y)   \
    {                                                                         \
        for (std::size_t i = 0; i != n; ++i)                                  \
            y[i] = op(a[i], b, c[i]);                                         \
    }                                                                         \
                                                                              \
    template <typename T>                                                     \
    attr inline void name(std::size_t n, T a, const T *b, const T *c, T *y)   \
    {                                                                         \
        for (std::size_t i = 0; i != n; ++i)                                  \
            y[i] = op(a, b[i], c[i]);                                         \
    }                                                                         \
                                                                              \
    template <typename T>                                                     \
    attr inline void name(std::size_t n, T a, T b, const T *c, T *y)          \
    {                                                                         \
        for (std::size_t i = 0; i != n; ++i)                                  \
            y[i] = op(a, b, c[i]);                                            \
    }                                                                         \
                                                                              \
    template <typename T>                                                     \
    attr inline void name(std::size_t n, T a, const T *b, T c, T *y)          \
    {                                                                         \
        for (std::size_t i = 0; i != n; ++i)                                  \
            y[i] = op(a, b[i], c);                                            \
    }                                                                         \
                                                                              \
    template <typename T>                                                     \
    attr inline void name(std::size_t n, const T *a, T b, T c, T *y)          \
    {                                                                         \
        for (std::size_t i = 0; i != n; ++i)                                  \
            y[i] = op(a[i], b, c);                                            \
    }

namespace mckl
{

namespace internal
{

template <typename T>
inline T mulbyconj_impl(T a, T b)
{
    return a * std::conj(b);
}

template <typename T>
inline T muladd_impl(T a, T b, T c)
{
    return a * b + c;
}

template <typename T>
inline T mulsub_impl(T a, T b, T c)
{
    return a * b - c;
}

template <typename T>
inline T nmuladd_impl(T a, T b, T c)
{
    return -(a * b) + c;
}

template <typename T>
inline T nmulsub_impl(T a, T b, T c)
{
    return -(a * b + c);
}

template <typename T>
inline T fmadd_impl(T a, T b, T c)
{
    return std::fma(a, b, c);
}

template <typename T>
inline T fmsub_impl(T a, T b, T c)
{
    return std::fma(a, b, -c);
}

template <typename T>
inline T fnmadd_impl(T a, T b, T c)
{
    return -std::fma(a, b, -c);
}

template <typename T>
inline T fnmsub_impl(T a, T b, T c)
{
    return -std::fma(a, b, c);
}

} // namespace mckl::internal

/// \defgroup vArithmetic Arithmetic functions
/// \ingroup VMF
/// @{

/// \brief For \f$i=1,\ldots,n\f$, compute \f$y_i = a_i + b_i\f$
MCKL_DEFINE_MATH_VMF_B(+, add)

/// \brief For \f$i=1,\ldots,n\f$, compute \f$y_i = a_i + b\f$
MCKL_DEFINE_MATH_VMF_BVS(+, add)

/// \brief For \f$i=1,\ldots,n\f$, compute \f$y_i = a + b_i\f$
MCKL_DEFINE_MATH_VMF_BSV(+, add)

/// \brief For \f$i=1,\ldots,n\f$, compute \f$y_i = a_i - b_i\f$
MCKL_DEFINE_MATH_VMF_B(-, sub)

/// \brief For \f$i=1,\ldots,n\f$, compute \f$y_i = a_i - b\f$
MCKL_DEFINE_MATH_VMF_BVS(-, sub)

/// \brief For \f$i=1,\ldots,n\f$, compute \f$y_i = a - b_i\f$
MCKL_DEFINE_MATH_VMF_BSV(-, sub)

/// \brief For \f$i=1,\ldots,n\f$, compute \f$y_i = a_i^2\f$
template <typename T>
inline void sqr(std::size_t n, const T *a, T *y)
{
    for (std::size_t i = 0; i != n; ++i)
        y[i] = a[i] * a[i];
}

/// \brief For \f$i=1,\ldots,n\f$, compute \f$y_i = a_i b_i\f$
MCKL_DEFINE_MATH_VMF_B(*, mul)

/// \brief For \f$i=1,\ldots,n\f$, compute \f$y_i = a_i b\f$
MCKL_DEFINE_MATH_VMF_BVS(*, mul)

/// \brief For \f$i=1,\ldots,n\f$, compute \f$y_i = a b_i\f$
MCKL_DEFINE_MATH_VMF_BSV(*, mul)

/// \brief For \f$i=1,\ldots,n\f$, copute \f$y_i = a_i \bar{b}_i\f$
MCKL_DEFINE_MATH_VMF_2(internal::mulbyconj_impl, mulbyconj)

/// \brief For \f$i=1,\ldots,n\f$, copute \f$y_i = a_i \bar{b}\f$
MCKL_DEFINE_MATH_VMF_2VS(internal::mulbyconj_impl, mulbyconj)

/// \brief For \f$i=1,\ldots,n\f$, copute \f$y_i = a \bar{b}_i\f$
MCKL_DEFINE_MATH_VMF_2SV(internal::mulbyconj_impl, mulbyconj)

/// \brief For \f$i=1,\ldots,n\f$, copute \f$y_i = \bar{a}_i\f$
MCKL_DEFINE_MATH_VMF_1(std::conj, conj)

/// \brief For \f$i=1,\ldots,n\f$, compute \f$y_i = |a_i|\f$
MCKL_DEFINE_MATH_VMF_1(std::abs, abs)

/// \brief For \f$i=1,\ldots,n\f$, compute \f$y_i = |a_i|\f$
template <typename T>
inline void abs(std::size_t n, const std::complex<T> *a, T *y)
{
    for (std::size_t i = 0; i != n; ++i)
        y[i] = std::abs(a[i]);
}

/// \brief For \f$i=1,\ldots,n\f$, compute \f$y_i = \arg(a_i)\f$
template <typename T>
inline void arg(std::size_t n, const std::complex<T> *a, T *y)
{
    for (std::size_t i = 0; i != n; ++i)
        y[i] = std::arg(a[i]);
}

/// \brief For \f$i=1,\ldots,n\f$, compute
/// \f$y_i = (\beta_a a_i + \mu_a) / (\beta_b b_i + \mu_b)\f$
template <typename T>
inline void linear_frac(std::size_t n, const T *a, const T *b, T beta_a,
    T beta_b, T mu_a, T mu_b, T *y)
{
    const std::size_t k = 1024;
    const std::size_t m = n / k;
    const std::size_t l = n % k;
    for (std::size_t i = 0; i != m; ++i, a += k, y += k) {
        for (std::size_t j = 0; j != k; ++j)
            y[j] = internal::muladd_impl(beta_a, a[j], mu_a);
        for (std::size_t j = 0; j != k; ++j)
            y[j] /= internal::muladd_impl(beta_b, b[j], mu_b);
    }
    for (std::size_t i = 0; i != l; ++i)
        y[i] = internal::muladd_impl(beta_a, a[i], mu_a);
    for (std::size_t i = 0; i != l; ++i)
        y[i] /= internal::muladd_impl(beta_b, b[i], mu_b);
}

/// \brief For \f$i = 1,\ldots,n\f$, compute = \f$y_i = a_i b_i + c_i\f$
MCKL_DEFINE_MATH_VMF_FMA(internal::muladd_impl, fma, [[deprecated]])

/// \brief For \f$i = 1,\ldots,n\f$, compute = \f$y_i = a_i b_i + c_i\f$
MCKL_DEFINE_MATH_VMF_FMA(internal::muladd_impl, muladd, )

/// \brief For \f$i = 1,\ldots,n\f$, compute = \f$y_i = a_i b_i - c_i\f$
MCKL_DEFINE_MATH_VMF_FMA(internal::mulsub_impl, mulsub, )

/// \brief For \f$i = 1,\ldots,n\f$, compute = \f$y_i = -a_i b_i + c_i\f$
MCKL_DEFINE_MATH_VMF_FMA(internal::nmuladd_impl, nmuladd, )

/// \brief For \f$i = 1,\ldots,n\f$, compute = \f$y_i = -a_i b_i - c_i\f$
MCKL_DEFINE_MATH_VMF_FMA(internal::nmulsub_impl, nmulsub, )

/// \brief For \f$i = 1,\ldots,n\f$, compute = \f$y_i = a_i b_i + c_i\f$
MCKL_DEFINE_MATH_VMF_FMA(internal::fmadd_impl, fmadd, )

/// \brief For \f$i = 1,\ldots,n\f$, compute = \f$y_i = a_i b_i - c_i\f$
MCKL_DEFINE_MATH_VMF_FMA(internal::fmsub_impl, fmsub, )

/// \brief For \f$i = 1,\ldots,n\f$, compute = \f$y_i = -a_i b_i + c_i\f$
MCKL_DEFINE_MATH_VMF_FMA(internal::fnmadd_impl, fnmadd, )

/// \brief For \f$i = 1,\ldots,n\f$, compute = \f$y_i = -a_i b_i - c_i\f$
MCKL_DEFINE_MATH_VMF_FMA(internal::fnmsub_impl, fnmsub, )

/// @} vArithmetic

/// \defgroup vPower Power root functions
/// \ingroup VMF
/// @{

/// \brief For \f$i=1,\ldots,n\f$, compute \f$y_i = a_i^{-1}\f$
template <typename T>
inline void inv(std::size_t n, const T *a, T *y)
{
    for (std::size_t i = 0; i != n; ++i)
        y[i] = const_one<T>() / a[i];
}

/// \brief For \f$i=1,\ldots,n\f$, compute \f$y_i = a_i / b_i\f$
MCKL_DEFINE_MATH_VMF_B(/, div)

/// \brief For \f$i=1,\ldots,n\f$, compute \f$y_i = a_i / b\f$
MCKL_DEFINE_MATH_VMF_BVS(/, div)

/// \brief For \f$i=1,\ldots,n\f$, compute \f$y_i = a / b_i\f$
MCKL_DEFINE_MATH_VMF_BSV(/, div)

/// \brief For \f$i=1,\ldots,n\f$, compute \f$y_i = \sqrt{a_i}\f$
MCKL_DEFINE_MATH_VMF_1(std::sqrt, sqrt)

/// \brief For \f$i=1,\ldots,n\f$, compute \f$y_i = 1 / \sqrt{a_i}\f$
template <typename T>
inline void invsqrt(std::size_t n, const T *a, T *y)
{
    const std::size_t k = 1024;
    const std::size_t m = n / k;
    const std::size_t l = n % k;
    for (std::size_t i = 0; i != m; ++i, a += k, y += k) {
        inv<T>(k, a, y);
        sqrt<T>(k, y, y);
    }
    inv<T>(l, a, y);
    sqrt<T>(l, y, y);
}

/// \brief For \f$i=1,\ldots,n\f$, compute \f$y_i = \sqrt[3]{a_i}\f$
MCKL_DEFINE_MATH_VMF_1(std::cbrt, cbrt)

/// \brief For \f$i=1,\ldots,n\f$, compute \f$y_i = 1 / \sqrt[3]{a_i}\f$
template <typename T>
inline void invcbrt(std::size_t n, const T *a, T *y)
{
    const std::size_t k = 1024;
    const std::size_t m = n / k;
    const std::size_t l = n % k;
    for (std::size_t i = 0; i != m; ++i, a += k, y += k) {
        cbrt<T>(k, a, y);
        inv<T>(k, y, y);
    }
    cbrt<T>(l, a, y);
    inv<T>(l, y, y);
}

/// \brief For \f$i=1,\ldots,n\f$, compute \f$y_i = a_i^{2/3}\f$
template <typename T>
inline void pow2o3(std::size_t n, const T *a, T *y)
{
    const std::size_t k = 1024;
    const std::size_t m = n / k;
    const std::size_t l = n % k;
    for (std::size_t i = 0; i != m; ++i, a += k, y += k) {
        cbrt<T>(k, a, y);
        sqr<T>(k, y, y);
    }
    cbrt<T>(l, a, y);
    sqr<T>(l, y, y);
}

/// \brief For \f$i=1,\ldots,n\f$, compute \f$y_i = a_i^{3/2}\f$
template <typename T>
inline void pow3o2(std::size_t n, const T *a, T *y)
{
    const std::size_t k = 1024;
    const std::size_t m = n / k;
    const std::size_t l = n % k;
    for (std::size_t i = 0; i != m; ++i, a += k, y += k) {
        sqrt<T>(k, a, y);
        for (std::size_t j = 0; j != k; ++j)
            y[j] = y[j] * y[j] * y[j];
    }
    sqrt<T>(l, a, y);
    for (std::size_t i = 0; i != l; ++i)
        y[i] = y[i] * y[i] * y[i];
}

/// \brief For \f$i=1,\ldots,n\f$, compute \f$y_i = a_i^{b_i}\f$
MCKL_DEFINE_MATH_VMF_2(std::pow, pow)

/// \brief For \f$i=1,\ldots,n\f$, compute \f$y_i = a_i^b\f$
MCKL_DEFINE_MATH_VMF_2VS(std::pow, pow)

/// \brief For \f$i=1,\ldots,n\f$, compute \f$y_i = a^{b_i}\f$
MCKL_DEFINE_MATH_VMF_2SV(std::pow, pow)

/// \brief For \f$i=1,\ldots,n\f$, compute \f$y_i = \sqrt{a_i^2 + b_i^2}\f$
MCKL_DEFINE_MATH_VMF_2(std::hypot, hypot)

/// \brief For \f$i=1,\ldots,n\f$, compute \f$y_i = \sqrt{a_i^2 + b^2}\f$
MCKL_DEFINE_MATH_VMF_2VS(std::hypot, hypot)

/// \brief For \f$i=1,\ldots,n\f$, compute \f$y_i = \sqrt{a^2 + b_i^2}\f$
MCKL_DEFINE_MATH_VMF_2SV(std::hypot, hypot)

/// @} vPower

/// \defgroup vExponential Exponential and logarithm functions
/// \ingroup VMF
/// @{

/// \brief For \f$i=1,\ldots,n\f$, compute \f$y_i = e^{a_i}\f$
MCKL_DEFINE_MATH_VMF_1(std::exp, exp)

/// \brief For \f$i=1,\ldots,n\f$, compute \f$y_i = 2^{a_i}\f$
MCKL_DEFINE_MATH_VMF_1(std::exp2, exp2)

/// \brief For \f$i=1,\ldots,n\f$, compute \f$y_i = e^{a_i} - 1\f$
MCKL_DEFINE_MATH_VMF_1(std::expm1, expm1)

/// \brief For \f$i=1,\ldots,n\f$, compute \f$y_i = \log(a_i)\f$
MCKL_DEFINE_MATH_VMF_1(std::log, log)

/// \brief For \f$i=1,\ldots,n\f$, compute \f$y_i = \log_2(a_i)\f$
MCKL_DEFINE_MATH_VMF_1(std::log2, log2)

/// \brief For \f$i=1,\ldots,n\f$, compute \f$y_i = \log_{10}(a_i)\f$
MCKL_DEFINE_MATH_VMF_1(std::log10, log10)

/// \brief For \f$i=1,\ldots,n\f$, compute \f$y_i = \log(a_i + 1)\f$
MCKL_DEFINE_MATH_VMF_1(std::log1p, log1p)

/// @} vExponential

/// \defgroup vTrigonometric Trigonometric functions
/// \ingroup VMF
/// @{

/// \brief For \f$i=1,\ldots,n\f$, compute \f$y_i = \sin(a_i)\f$
MCKL_DEFINE_MATH_VMF_1(std::cos, cos)

/// \brief For \f$i=1,\ldots,n\f$, compute \f$y_i = \cos(a_i)\f$
MCKL_DEFINE_MATH_VMF_1(std::sin, sin)

/// \brief For \f$i=1,\ldots,n\f$, compute
/// \f$y_i = \sin(a_i), z_i = \cos(a_i)\f$
template <typename T>
inline void sincos(std::size_t n, const T *a, T *y, T *z)
{
    const std::size_t k = 1024;
    const std::size_t m = n / k;
    const std::size_t l = n % k;

    std::intptr_t vn = static_cast<std::intptr_t>(n);
    std::intptr_t va = reinterpret_cast<std::intptr_t>(a);
    std::intptr_t vy = reinterpret_cast<std::intptr_t>(y);
    if (va - vy < vn || vy - va < vn) {
        alignas(32) std::array<T, k> s;
        for (std::size_t i = 0; i != m; ++i, a += k, y += k, z += k) {
            sin<T>(k, a, s.data());
            cos<T>(k, a, z);
            std::copy_n(s.data(), k, y);
        }
        sin<T>(l, a, s.data());
        cos<T>(l, a, z);
        std::copy_n(s.data(), l, y);
    } else {
        for (std::size_t i = 0; i != m; ++i, a += k, y += k, z += k) {
            sin<T>(k, a, y);
            cos<T>(k, a, z);
        }
        sin<T>(l, a, y);
        cos<T>(l, a, z);
    }
}

/// \brief For \f$i=1,\ldots,n\f$, compute \f$y_i = \cos(a_i) + i\sin(a_i)\f$
template <typename T>
inline void cis(std::size_t n, const T *a, std::complex<T> *y)
{
    const std::size_t k = 1024;
    const std::size_t m = n / k;
    const std::size_t l = n % k;
    alignas(32) T s[k];
    alignas(32) T c[k];
    for (std::size_t i = 0; i != m; ++i, a += k, y += k) {
        sincos<T>(k, a, s, c);
        for (std::size_t j = 0; j != k; ++j) {
            y[j].real() = c[j];
            y[j].imag() = s[j];
        }
    }
    sincos<T>(l, a, s, c);
    for (std::size_t j = 0; j != l; ++j) {
        y[j].real() = c[j];
        y[j].imag() = s[j];
    }
}

/// \brief For \f$i=1,\ldots,n\f$, compute \f$y_i = \tan(a_i)\f$
MCKL_DEFINE_MATH_VMF_1(std::tan, tan)

/// \brief For \f$i=1,\ldots,n\f$, compute \f$y_i = \arccos(a_i)\f$
MCKL_DEFINE_MATH_VMF_1(std::acos, acos)

/// \brief For \f$i=1,\ldots,n\f$, compute \f$y_i = \arcsin(a_i)\f$
MCKL_DEFINE_MATH_VMF_1(std::asin, asin)

/// \brief For \f$i=1,\ldots,n\f$, compute \f$y_i = \arctan(a_i)\f$
MCKL_DEFINE_MATH_VMF_1(std::atan, atan)

/// \brief For \f$i=1,\ldots,n\f$, compute \f$y_i = \arctan(a_i / b_i)\f$ with
/// signs to determine the quadrant
MCKL_DEFINE_MATH_VMF_2(std::atan2, atan2)

/// \brief For \f$i=1,\ldots,n\f$, compute \f$y_i = \arctan(a_i / b)\f$ with
/// signs to determine the quadrant
MCKL_DEFINE_MATH_VMF_2VS(std::atan2, atan2)

/// \brief For \f$i=1,\ldots,n\f$, compute \f$y_i = \arctan(a / b_i)\f$ with
/// signs to determine the quadrant
MCKL_DEFINE_MATH_VMF_2SV(std::atan2, atan2)

/// @} vTrigonometric

/// \defgroup vHyperbolic Hyperbolic functions
/// \ingroup VMF
/// @{

/// \brief For \f$i=1,\ldots,n\f$, compute \f$y_i = \cosh(a_i)\f$
MCKL_DEFINE_MATH_VMF_1(std::cosh, cosh)

/// \brief For \f$i=1,\ldots,n\f$, compute \f$y_i = \sinh(a_i)\f$
MCKL_DEFINE_MATH_VMF_1(std::sinh, sinh)

/// \brief For \f$i=1,\ldots,n\f$, compute \f$y_i = \tanh(a_i)\f$
MCKL_DEFINE_MATH_VMF_1(std::tanh, tanh)

/// \brief For \f$i=1,\ldots,n\f$, compute \f$y_i = \mathrm{arc}\cosh(a_i)\f$
MCKL_DEFINE_MATH_VMF_1(std::acosh, acosh)

/// \brief For \f$i=1,\ldots,n\f$, compute \f$y_i = \mathrm{arc}\sinh(a_i)\f$
MCKL_DEFINE_MATH_VMF_1(std::asinh, asinh)

/// \brief For \f$i=1,\ldots,n\f$, compute \f$y_i = \mathrm{arc}\tanh(a_i)\f$
MCKL_DEFINE_MATH_VMF_1(std::atanh, atanh)

/// @} vHyperbolic

/// \defgroup vSpecial Special functions
/// \ingroup VMF
/// @{

/// \brief For \f$i=1,\ldots,n\f$, compute \f$y_i = \mathrm{erf}(a_i)\f$
MCKL_DEFINE_MATH_VMF_1(std::erf, erf)

/// \brief For \f$i=1,\ldots,n\f$, compute
/// \f$y_i = \mathrm{erfc}(a_i) = \mathrm{erfc}(a_i)\f$
MCKL_DEFINE_MATH_VMF_1(std::erfc, erfc)

/// \brief For \f$i=1,\ldots,n\f$, compute \f$y_i = \mathrm{erf}^{-1}(a_i)\f$
MCKL_DEFINE_MATH_VMF_1(mckl::erfinv, erfinv)

/// \brief For \f$i=1,\ldots,n\f$, compute
/// \f$y_i = \mathrm{erfc}(a_i) = \mathrm{erfc}^{-1}(a_i)\f$
MCKL_DEFINE_MATH_VMF_1(mckl::erfcinv, erfcinv)

/// \brief For \f$i=1,\ldots,n\f$, compute
/// \f$y_i = (1 + \mathrm{erfc}(a_i / \sqrt{2})) / 2\f$
template <typename T>
inline void cdfnorm(std::size_t n, const T *a, T *y)
{
    const std::size_t k = 1024;
    const std::size_t m = n / k;
    const std::size_t l = n % k;
    for (std::size_t i = 0; i != m; ++i, a += k, y += k) {
        mul<T>(k, const_sqrt_1by2<T>(), a, y);
        erf<T>(k, y, y);
        muladd<T>(k, y, static_cast<T>(0.5), static_cast<T>(0.5), y);
    }
    mul<T>(l, const_sqrt_1by2<T>(), a, y);
    erf<T>(l, y, y);
    muladd<T>(l, y, static_cast<T>(0.5), static_cast<T>(0.5), y);
}

/// \brief For \f$i=1,\ldots,n\f$, compute
/// \f$y_i = \sqrt{2}\mathrm{erfc}^{-1}(2 - 2a_i)\f$
template <typename T>
inline void cdfnorminv(std::size_t n, const T *a, T *y)
{
    const std::size_t k = 1024;
    const std::size_t m = n / k;
    const std::size_t l = n % k;
    for (std::size_t i = 0; i != m; ++i, a += k, y += k) {
        muladd<T>(k, a, static_cast<T>(-2), static_cast<T>(2), y);
        erfcinv<T>(k, y, y);
        mul<T>(k, const_sqrt_2<T>(), y, y);
    }
    muladd<T>(l, a, static_cast<T>(-2), static_cast<T>(2), y);
    erfcinv<T>(l, y, y);
    mul<T>(l, const_sqrt_2<T>(), y, y);
}

/// \brief For \f$i=1,\ldots,n\f$, compute \f$y_i = \ln\Gamma(a_i)\f$
MCKL_DEFINE_MATH_VMF_1(std::lgamma, lgamma)

/// \brief For \f$i=1,\ldots,n\f$, compute \f$y_i = \Gamma(a_i)\f$
MCKL_DEFINE_MATH_VMF_1(std::tgamma, tgamma)

/// @} vSpecial

/// \defgroup vRounding Rounding functions
/// \ingroup VMF
/// @{

/// \brief For \f$i=1,\ldots,n\f$, compute \f$y_i = \lfloor a_i \rfloor\f$
MCKL_DEFINE_MATH_VMF_1(std::floor, floor)

/// \brief For \f$i=1,\ldots,n\f$, compute \f$y_i = \lceil a_i \rceil\f$
MCKL_DEFINE_MATH_VMF_1(std::ceil, ceil)

/// \brief For \f$i=1,\ldots,n\f$, compute
/// \f$y_i = \mathrm{sgn}(a_i)\lfloor|a_i|\rfloor\f$
MCKL_DEFINE_MATH_VMF_1(std::trunc, trunc)

/// \brief For \f$i=1,\ldots,n\f$, compute the nearest integers, rounding away
/// from zero
MCKL_DEFINE_MATH_VMF_1(std::round, round)

/// \brief For \f$i=1,\ldots,n\f$, compute the nearest integers, using the
/// current rounding mode
MCKL_DEFINE_MATH_VMF_1(std::nearbyint, nearbyint)

/// \brief For \f$i=1,\ldots,n\f$, compute the nearest integers, using the
/// current rounding mode
MCKL_DEFINE_MATH_VMF_1(std::rint, rint)

/// \brief For \f$i=1,\ldots,n\f$, compute the integeral and fraction parts
template <typename T>
inline void modf(std::size_t n, const T *a, T *y, T *z)
{
    for (std::size_t i = 0; i != n; ++i, ++a, ++y, ++z)
        *z = std::modf(*a, y);
}

/// @} vRounding

} // namespace mckl

#endif // MCKL_MATH_VMF_HPP
