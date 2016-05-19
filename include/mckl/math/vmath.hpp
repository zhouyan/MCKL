//============================================================================
// MCKL/include/mckl/math/vmath.hpp
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

#ifndef MCKL_MATH_VMATH_HPP
#define MCKL_MATH_VMATH_HPP

#include <mckl/internal/basic.hpp>
#include <mckl/math/constants.hpp>

#if MCKL_USE_MKL_VML

#include <mkl_vml.h>

#define MCKL_DEFINE_MATH_VMATH_VML_1(func, name)                              \
    inline void name(std::size_t n, const float *a, float *y)                 \
    {                                                                         \
        internal::size_check<MKL_INT>(n, #name);                              \
        ::vs##func(static_cast<MKL_INT>(n), a, y);                            \
    }                                                                         \
    inline void name(std::size_t n, const double *a, double *y)               \
    {                                                                         \
        internal::size_check<MKL_INT>(n, #name);                              \
        ::vd##func(static_cast<MKL_INT>(n), a, y);                            \
    }

#define MCKL_DEFINE_MATH_VMATH_VML_2(func, name)                              \
    inline void name(std::size_t n, const float *a, const float *b, float *y) \
    {                                                                         \
        internal::size_check<MKL_INT>(n, #name);                              \
        ::vs##func(static_cast<MKL_INT>(n), a, b, y);                         \
    }                                                                         \
    inline void name(                                                         \
        std::size_t n, const double *a, const double *b, double *y)           \
    {                                                                         \
        internal::size_check<MKL_INT>(n, #name);                              \
        ::vd##func(static_cast<MKL_INT>(n), a, b, y);                         \
    }

namespace mckl
{

MCKL_DEFINE_MATH_VMATH_VML_2(Add, add)
MCKL_DEFINE_MATH_VMATH_VML_2(Sub, sub)
MCKL_DEFINE_MATH_VMATH_VML_1(Sqr, sqr)
MCKL_DEFINE_MATH_VMATH_VML_2(Mul, mul)
MCKL_DEFINE_MATH_VMATH_VML_1(Abs, abs)
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

MCKL_DEFINE_MATH_VMATH_VML_1(Inv, inv)
MCKL_DEFINE_MATH_VMATH_VML_2(Div, div)
MCKL_DEFINE_MATH_VMATH_VML_1(Sqrt, sqrt)
MCKL_DEFINE_MATH_VMATH_VML_1(InvSqrt, invsqrt)
MCKL_DEFINE_MATH_VMATH_VML_1(Cbrt, cbrt)
MCKL_DEFINE_MATH_VMATH_VML_1(InvCbrt, invcbrt)
MCKL_DEFINE_MATH_VMATH_VML_1(Pow2o3, pow2o3)
MCKL_DEFINE_MATH_VMATH_VML_1(Pow3o2, pow3o2)
MCKL_DEFINE_MATH_VMATH_VML_2(Pow, pow)
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
MCKL_DEFINE_MATH_VMATH_VML_2(Hypot, hypot)

MCKL_DEFINE_MATH_VMATH_VML_1(Exp, exp)
MCKL_DEFINE_MATH_VMATH_VML_1(Expm1, expm1)
MCKL_DEFINE_MATH_VMATH_VML_1(Ln, log)
MCKL_DEFINE_MATH_VMATH_VML_1(Log10, log10)
MCKL_DEFINE_MATH_VMATH_VML_1(Log1p, log1p)

MCKL_DEFINE_MATH_VMATH_VML_1(Cos, cos)
MCKL_DEFINE_MATH_VMATH_VML_1(Sin, sin)
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
MCKL_DEFINE_MATH_VMATH_VML_1(Tan, tan)
MCKL_DEFINE_MATH_VMATH_VML_1(Acos, acos)
MCKL_DEFINE_MATH_VMATH_VML_1(Asin, asin)
MCKL_DEFINE_MATH_VMATH_VML_1(Atan, atan)
MCKL_DEFINE_MATH_VMATH_VML_2(Atan2, atan2)

MCKL_DEFINE_MATH_VMATH_VML_1(Cosh, cosh)
MCKL_DEFINE_MATH_VMATH_VML_1(Sinh, sinh)
MCKL_DEFINE_MATH_VMATH_VML_1(Tanh, tanh)
MCKL_DEFINE_MATH_VMATH_VML_1(Acosh, acosh)
MCKL_DEFINE_MATH_VMATH_VML_1(Asinh, asinh)
MCKL_DEFINE_MATH_VMATH_VML_1(Atanh, atanh)

MCKL_DEFINE_MATH_VMATH_VML_1(Erf, erf)
MCKL_DEFINE_MATH_VMATH_VML_1(Erfc, erfc)
MCKL_DEFINE_MATH_VMATH_VML_1(CdfNorm, cdfnorm)
MCKL_DEFINE_MATH_VMATH_VML_1(ErfInv, erfinv)
MCKL_DEFINE_MATH_VMATH_VML_1(ErfcInv, erfcinv)
MCKL_DEFINE_MATH_VMATH_VML_1(CdfNormInv, cdfnorminv)
MCKL_DEFINE_MATH_VMATH_VML_1(LGamma, lgamma)
MCKL_DEFINE_MATH_VMATH_VML_1(TGamma, tgamm)

MCKL_DEFINE_MATH_VMATH_VML_1(Floor, floor)
MCKL_DEFINE_MATH_VMATH_VML_1(Ceil, ceil)
MCKL_DEFINE_MATH_VMATH_VML_1(Trunc, trunc)
MCKL_DEFINE_MATH_VMATH_VML_1(Round, round)
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

#endif // MCKL_USE_MKL_VML

#define MCKL_DEFINE_MATH_VMATH_1(func, name)                                  \
    template <typename T>                                                     \
    inline void name(std::size_t n, const T *a, T *y)                         \
    {                                                                         \
        for (std::size_t i = 0; i != n; ++i)                                  \
            y[i] = func(a[i]);                                                \
    }

#define MCKL_DEFINE_MATH_VMATH_2(func, name)                                  \
    template <typename T>                                                     \
    inline void name(std::size_t n, const T *a, const T *b, T *y)             \
    {                                                                         \
        for (std::size_t i = 0; i != n; ++i)                                  \
            y[i] = func(a[i], b[i]);                                          \
    }

#define MCKL_DEFINE_MATH_VMATH_B(op, name)                                    \
    template <typename T>                                                     \
    inline void name(std::size_t n, const T *a, const T *b, T *y)             \
    {                                                                         \
        for (std::size_t i = 0; i != n; ++i)                                  \
            y[i] = a[i] op b[i];                                              \
    }

#define MCKL_DEFINE_MATH_VMATH_VS(op, name)                                   \
    template <typename T>                                                     \
    inline void name(std::size_t n, const T *a, T b, T *y)                    \
    {                                                                         \
        for (std::size_t i = 0; i != n; ++i)                                  \
            y[i] = a[i] op b;                                                 \
    }

#define MCKL_DEFINE_MATH_VMATH_SV(op, name)                                   \
    template <typename T>                                                     \
    inline void name(std::size_t n, T a, const T *b, T *y)                    \
    {                                                                         \
        for (std::size_t i = 0; i != n; ++i)                                  \
            y[i] = a op b[i];                                                 \
    }

namespace mckl
{

/// \defgroup vArithmetic Arithmetic functions
/// \ingroup vMath
/// @{

/// \brief For \f$i=1,\ldots,n\f$, compute \f$y_i = a_i + b_i\f$
MCKL_DEFINE_MATH_VMATH_B(+, add)

/// \brief For \f$i=1,\ldots,n\f$, compute \f$y_i = a_i + b\f$
MCKL_DEFINE_MATH_VMATH_VS(+, add)

/// \brief For \f$i=1,\ldots,n\f$, compute \f$y_i = a + b_i\f$
MCKL_DEFINE_MATH_VMATH_SV(+, add)

/// \brief For \f$i=1,\ldots,n\f$, compute \f$y_i = a_i - b_i\f$
MCKL_DEFINE_MATH_VMATH_B(-, sub)

/// \brief For \f$i=1,\ldots,n\f$, compute \f$y_i = a_i - b\f$
MCKL_DEFINE_MATH_VMATH_VS(-, sub)

/// \brief For \f$i=1,\ldots,n\f$, compute \f$y_i = a - b_i\f$
MCKL_DEFINE_MATH_VMATH_SV(-, sub)

/// \brief For \f$i=1,\ldots,n\f$, compute \f$y_i = a_i^2\f$
template <typename T>
inline void sqr(std::size_t n, const T *a, T *y)
{
    for (std::size_t i = 0; i != n; ++i)
        y[i] = a[i] * a[i];
}

/// \brief For \f$i=1,\ldots,n\f$, compute \f$y_i = a_i b_i\f$
MCKL_DEFINE_MATH_VMATH_B(*, mul)

/// \brief For \f$i=1,\ldots,n\f$, compute \f$y_i = a_i b\f$
MCKL_DEFINE_MATH_VMATH_VS(*, mul)

/// \brief For \f$i=1,\ldots,n\f$, compute \f$y_i = a b_i\f$
MCKL_DEFINE_MATH_VMATH_SV(*, mul)

/// \brief For \f$i=1,\ldots,n\f$, compute \f$y_i = |a_i|\f$
MCKL_DEFINE_MATH_VMATH_1(std::abs, abs)

/// \brief For \f$i=1,\ldots,n\f$, compute
/// \f$y_i = (\beta_a a_i + \mu_a) / (\beta_b b_i + \mu_b)\f$
template <typename T>
inline void linear_frac(std::size_t n, const T *a, const T *b, T beta_a,
    T beta_b, T mu_a, T mu_b, T *y)
{
    const std::size_t k = internal::BufferSize<T>::value;
    const std::size_t m = n / k;
    const std::size_t l = n % k;
    for (std::size_t i = 0; i != m; ++i, a += k, y += k) {
        for (std::size_t j = 0; j != k; ++j)
            y[j] = beta_a * a[j] + mu_a;
        for (std::size_t j = 0; j != k; ++j)
            y[j] /= beta_b * b[j] + mu_b;
    }
    for (std::size_t i = 0; i != l; ++i)
        y[i] = beta_a * a[i] + mu_a;
    for (std::size_t i = 0; i != l; ++i)
        y[i] /= beta_b * b[i] + mu_b;
}

/// \brief For \f$i=1,\ldots,n\f$, compute \f$y_i = a_i b_i + c_i\f$.
template <typename T>
inline void fma(std::size_t n, const T *a, const T *b, const T *c, T *y)
{
    for (std::size_t i = 0; i != n; ++i)
        y[i] = a[i] * b[i] + c[i];
}

/// \brief For \f$i=1,\ldots,n\f$, compute \f$y_i = a_i b_i + c\f$.
template <typename T>
inline void fma(std::size_t n, const T *a, const T *b, T c, T *y)
{
    if (internal::is_zero(c)) {
        mul(n, a, b, y);
    } else {
        for (std::size_t i = 0; i != n; ++i)
            y[i] = a[i] * b[i] + c;
    }
}

/// \brief For \f$i=1,\ldots,n\f$, compute \f$y_i = a_i b + c_i\f$.
template <typename T>
inline void fma(std::size_t n, const T *a, T b, const T *c, T *y)
{
    if (internal::is_one(b)) {
        add(n, a, c, y);
    } else {
        for (std::size_t i = 0; i != n; ++i)
            y[i] = a[i] * b + c[i];
    }
}

/// \brief For \f$i=1,\ldots,n\f$, compute \f$y_i = a_i b + c\f$.
template <typename T>
inline void fma(std::size_t n, const T *a, T b, T c, T *y)
{
    if (internal::is_one(b) && internal::is_zero(c)) {
        std::copy_n(a, n, y);
    } else if (internal::is_one(b)) {
        add(n, a, c, y);
    } else if (internal::is_zero(c)) {
        mul(n, a, b, y);
    } else {
        for (std::size_t i = 0; i != n; ++i)
            y[i] = a[i] * b + c;
    }
}

/// \brief For \f$i=1,\ldots,n\f$, compute \f$y_i = a b_i + c_i\f$.
template <typename T>
inline void fma(std::size_t n, T a, const T *b, const T *c, T *y)
{
    if (internal::is_one(a)) {
        add(n, b, c, y);
    } else {
        for (std::size_t i = 0; i != n; ++i)
            y[i] = a * b[i] + c[i];
    }
}

/// \brief For \f$i=1,\ldots,n\f$, compute \f$y_i = a b_i + c\f$.
template <typename T>
inline void fma(std::size_t n, T a, const T *b, T c, T *y)
{
    if (internal::is_zero(c)) {
        mul(n, a, b, y);
    } else {
        for (std::size_t i = 0; i != n; ++i)
            y[i] = a * b[i] + c;
    }
}

/// \brief For \f$i=1,\ldots,n\f$, compute \f$y_i = a b + c_i\f$.
template <typename T>
inline void fma(std::size_t n, T a, T b, const T *c, T *y)
{
    add(n, a * b, c, y);
}

/// @} vArithmetic

/// \defgroup vPower Power root functions
/// \ingroup vMath
/// @{

/// \brief For \f$i=1,\ldots,n\f$, compute \f$y_i = a_i^{-1}\f$
template <typename T>
inline void inv(std::size_t n, const T *a, T *y)
{
    for (std::size_t i = 0; i != n; ++i)
        y[i] = const_one<T>() / a[i];
}

/// \brief For \f$i=1,\ldots,n\f$, compute \f$y_i = a_i / b_i\f$
MCKL_DEFINE_MATH_VMATH_B(/, div)

/// \brief For \f$i=1,\ldots,n\f$, compute \f$y_i = a_i / b\f$
MCKL_DEFINE_MATH_VMATH_VS(/, div)

/// \brief For \f$i=1,\ldots,n\f$, compute \f$y_i = a / b_i\f$
MCKL_DEFINE_MATH_VMATH_SV(/, div)

/// \brief For \f$i=1,\ldots,n\f$, compute \f$y_i = \sqrt{a_i}\f$
MCKL_DEFINE_MATH_VMATH_1(std::sqrt, sqrt)

/// \brief For \f$i=1,\ldots,n\f$, compute \f$y_i = 1 / \sqrt{a_i}\f$
template <typename T>
inline void invsqrt(std::size_t n, const T *a, T *y)
{
    const std::size_t k = internal::BufferSize<T>::value;
    const std::size_t m = n / k;
    const std::size_t l = n % k;
    for (std::size_t i = 0; i != m; ++i, a += k, y += k) {
        sqrt(k, a, y);
        inv(k, y, y);
    }
    sqrt(l, a, y);
    inv(l, y, y);
}

/// \brief For \f$i=1,\ldots,n\f$, compute \f$y_i = \sqrt[3]{a_i}\f$
MCKL_DEFINE_MATH_VMATH_1(std::cbrt, cbrt)

/// \brief For \f$i=1,\ldots,n\f$, compute \f$y_i = 1 / \sqrt[3]{a_i}\f$
template <typename T>
inline void invcbrt(std::size_t n, const T *a, T *y)
{
    const std::size_t k = internal::BufferSize<T>::value;
    const std::size_t m = n / k;
    const std::size_t l = n % k;
    for (std::size_t i = 0; i != m; ++i, a += k, y += k) {
        cbrt(k, a, y);
        inv(k, y, y);
    }
    cbrt(l, a, y);
    inv(l, y, y);
}

/// \brief For \f$i=1,\ldots,n\f$, compute \f$y_i = a_i^{2/3}\f$
template <typename T>
inline void pow2o3(std::size_t n, const T *a, T *y)
{
    const std::size_t k = internal::BufferSize<T>::value;
    const std::size_t m = n / k;
    const std::size_t l = n % k;
    for (std::size_t i = 0; i != m; ++i, a += k, y += k) {
        cbrt(k, a, y);
        sqr(k, y, y);
    }
    cbrt(l, a, y);
    sqr(l, y, y);
}

/// \brief For \f$i=1,\ldots,n\f$, compute \f$y_i = a_i^{3/2}\f$
template <typename T>
inline void pow3o2(std::size_t n, const T *a, T *y)
{
    const std::size_t k = internal::BufferSize<T>::value;
    const std::size_t m = n / k;
    const std::size_t l = n % k;
    for (std::size_t i = 0; i != m; ++i, a += k, y += k) {
        sqrt(k, a, y);
        for (std::size_t j = 0; j != k; ++j)
            y[j] = y[j] * y[j] * y[j];
    }
    sqrt(l, a, y);
    for (std::size_t i = 0; i != l; ++i)
        y[i] = y[i] * y[i] * y[i];
}

/// \brief For \f$i=1,\ldots,n\f$, compute \f$y_i = a_i^{b_i}\f$
MCKL_DEFINE_MATH_VMATH_2(std::pow, pow)

/// \brief For \f$i=1,\ldots,n\f$, compute \f$y_i = a_i^b\f$
template <typename T>
inline void pow(std::size_t n, const T *a, T b, T *y)
{
    for (std::size_t i = 0; i != n; ++i)
        y[i] = std::pow(a[i], b);
}

/// \brief For \f$i=1,\ldots,n\f$, compute \f$y_i = \sqrt{a_i^2 + b_i^2}\f$
MCKL_DEFINE_MATH_VMATH_2(std::hypot, hypot)

/// @} vPower

/// \defgroup vExponential Exponential and logarithm functions
/// \ingroup vMath
/// @{

/// \brief For \f$i=1,\ldots,n\f$, compute \f$y_i = e^{a_i}\f$
MCKL_DEFINE_MATH_VMATH_1(std::exp, exp)

/// \brief For \f$i=1,\ldots,n\f$, compute \f$y_i = 2^{a_i}\f$
MCKL_DEFINE_MATH_VMATH_1(std::exp2, exp2)

/// \brief For \f$i=1,\ldots,n\f$, compute \f$y_i = 10^{a_i}\f$
template <typename T>
inline void exp10(std::size_t n, const T *a, T *y)
{
    const std::size_t k = internal::BufferSize<T>::value;
    const std::size_t m = n / k;
    const std::size_t l = n % k;
    for (std::size_t i = 0; i != m; ++i, a += k, y += k) {
        mul(k, const_ln_10<T>(), a, y);
        exp(k, y, y);
    }
    mul(l, const_ln_10<T>(), a, y);
    exp(l, y, y);
}

/// \brief For \f$i=1,\ldots,n\f$, compute \f$y_i = e^{a_i} - 1\f$
MCKL_DEFINE_MATH_VMATH_1(std::expm1, expm1)

/// \brief For \f$i=1,\ldots,n\f$, compute \f$y_i = \log(a_i)\f$
MCKL_DEFINE_MATH_VMATH_1(std::log, log)

/// \brief For \f$i=1,\ldots,n\f$, compute \f$y_i = \log_2(a_i)\f$
MCKL_DEFINE_MATH_VMATH_1(std::log2, log2)

/// \brief For \f$i=1,\ldots,n\f$, compute \f$y_i = \log_{10}(a_i)\f$
MCKL_DEFINE_MATH_VMATH_1(std::log10, log10)

/// \brief For \f$i=1,\ldots,n\f$, compute \f$y_i = \log(a_i + 1)\f$
MCKL_DEFINE_MATH_VMATH_1(std::log1p, log1p)

/// @} vExponential

/// \defgroup vTrigonometric Trigonometric functions
/// \ingroup vMath
/// @{

/// \brief For \f$i=1,\ldots,n\f$, compute \f$y_i = \sin(a_i)\f$
MCKL_DEFINE_MATH_VMATH_1(std::cos, cos)

/// \brief For \f$i=1,\ldots,n\f$, compute \f$y_i = \cos(a_i)\f$
MCKL_DEFINE_MATH_VMATH_1(std::sin, sin)

/// \brief For \f$i=1,\ldots,n\f$, compute
/// \f$y_i = \sin(a_i), z_i = \cos(a_i)\f$
template <typename T>
inline void sincos(std::size_t n, const T *a, T *y, T *z)
{
    const std::size_t k = internal::BufferSize<T>::value;
    const std::size_t m = n / k;
    const std::size_t l = n % k;
    for (std::size_t i = 0; i != m; ++i, a += k, y += k, z += k) {
        sin(k, a, y);
        cos(k, a, z);
    }
    sin(l, a, y);
    cos(l, a, z);
}

/// \brief For \f$i=1,\ldots,n\f$, compute \f$y_i = \tan(a_i)\f$
MCKL_DEFINE_MATH_VMATH_1(std::tan, tan)

/// \brief For \f$i=1,\ldots,n\f$, compute \f$y_i = \arccos(a_i)\f$
MCKL_DEFINE_MATH_VMATH_1(std::acos, acos)

/// \brief For \f$i=1,\ldots,n\f$, compute \f$y_i = \arcsin(a_i)\f$
MCKL_DEFINE_MATH_VMATH_1(std::asin, asin)

/// \brief For \f$i=1,\ldots,n\f$, compute \f$y_i = \arctan(a_i)\f$
MCKL_DEFINE_MATH_VMATH_1(std::atan, atan)

/// \brief For \f$i=1,\ldots,n\f$, compute \f$y_i = \arctan(a_i / b_i)\f$ with
/// signs to determine the quadrant
MCKL_DEFINE_MATH_VMATH_2(std::atan2, atan2)

/// @} vTrigonometric

/// \defgroup vHyperbolic Hyperbolic functions
/// \ingroup vMath
/// @{

/// \brief For \f$i=1,\ldots,n\f$, compute \f$y_i = \cosh(a_i)\f$
MCKL_DEFINE_MATH_VMATH_1(std::cosh, cosh)

/// \brief For \f$i=1,\ldots,n\f$, compute \f$y_i = \sinh(a_i)\f$
MCKL_DEFINE_MATH_VMATH_1(std::sinh, sinh)

/// \brief For \f$i=1,\ldots,n\f$, compute \f$y_i = \tanh(a_i)\f$
MCKL_DEFINE_MATH_VMATH_1(std::tanh, tanh)

/// \brief For \f$i=1,\ldots,n\f$, compute \f$y_i = \mathrm{arc}\cosh(a_i)\f$
MCKL_DEFINE_MATH_VMATH_1(std::acosh, acosh)

/// \brief For \f$i=1,\ldots,n\f$, compute \f$y_i = \mathrm{arc}\sinh(a_i)\f$
MCKL_DEFINE_MATH_VMATH_1(std::asinh, asinh)

/// \brief For \f$i=1,\ldots,n\f$, compute \f$y_i = \mathrm{arc}\tanh(a_i)\f$
MCKL_DEFINE_MATH_VMATH_1(std::atanh, atanh)

/// @} vHyperbolic

/// \defgroup vSpecial Special functions
/// \ingroup vMath
/// @{

/// \brief For \f$i=1,\ldots,n\f$, compute \f$y_i = \mathrm{Erf}(a_i)\f$
MCKL_DEFINE_MATH_VMATH_1(std::erf, erf)

/// \brief For \f$i=1,\ldots,n\f$, compute
/// \f$y_i = \mathrm{Erfc}(a_i) = \mathrm{Erf}(1 - a_i)\f$
MCKL_DEFINE_MATH_VMATH_1(std::erfc, erfc)

/// \brief For \f$i=1,\ldots,n\f$, compute
/// \f$y_i = 1 - \mathrm{Erfc}(a_i / \sqrt{2}) / 2\f$, the standard Normal CDF
template <typename T>
inline void cdfnorm(std::size_t n, const T *a, T *y)
{
    const std::size_t k = internal::BufferSize<T>::value;
    const std::size_t m = n / k;
    const std::size_t l = n % k;
    for (std::size_t i = 0; i != m; ++i, a += k, y += k) {
        mul(k, const_one<T>() / const_sqrt_2<T>(), a, y);
        erf(k, y, y);
        fma(k, static_cast<T>(0.5), static_cast<T>(0.5), y, y);
    }
    mul(l, const_one<T>() / const_sqrt_2<T>(), a, y);
    erf(l, y, y);
    fma(l, static_cast<T>(0.5), static_cast<T>(0.5), y, y);
}

/// \brief For \f$i=1,\ldots,n\f$, compute \f$y_i = \ln\Gamma(a_i)\f$
MCKL_DEFINE_MATH_VMATH_1(std::lgamma, lgamma)

/// \brief For \f$i=1,\ldots,n\f$, compute \f$y_i = \Gamma(a_i)\f$
MCKL_DEFINE_MATH_VMATH_1(std::tgamma, tgamma)

/// \brief For \f$i=1,\ldots,n\f$, compute \f$y_i = \lfloor a_i \rfloor\f$
MCKL_DEFINE_MATH_VMATH_1(std::floor, floor)

/// \brief For \f$i=1,\ldots,n\f$, compute \f$y_i = \lceil a_i \rceil\f$
MCKL_DEFINE_MATH_VMATH_1(std::ceil, ceil)

/// \brief For \f$i=1,\ldots,n\f$, compute
/// \f$y_i = \mathrm{sgn}(a_i)\lfloor|a_i|\rfoor\f$
MCKL_DEFINE_MATH_VMATH_1(std::trunc, trunc)

/// \brief For \f$i=1,\ldots,n\f$, compute rounding
MCKL_DEFINE_MATH_VMATH_1(std::round, round)

/// \brief For \f$i=1,\ldots,n\f$, compute integeral and fraction parts
template <typename T>
inline void modf(std::size_t n, const T *a, T *y, T *z)
{
    for (std::size_t i = 0; i != n; ++i, ++a, ++y, ++z)
        *z = std::modf(*a, y);
}

/// @} vSpecial

} // namespace mckl

#endif // MCKL_MATH_VMATH_HPP
