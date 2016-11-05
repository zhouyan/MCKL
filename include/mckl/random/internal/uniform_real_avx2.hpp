//============================================================================
// MCKL/include/mckl/random/internal/uniform_real_avx2.hpp
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

#ifndef MCKL_RANDOM_INTERNAL_UNIFORM_REAL_AVX2_HPP
#define MCKL_RANDOM_INTERNAL_UNIFORM_REAL_AVX2_HPP

#include <mckl/random/internal/common.hpp>
#include <mckl/random/internal/u01_avx2.hpp>

#ifdef MCKL_GCC
#if MCKL_GCC_VERSION >= 60000
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wignored-attributes"
#endif
#endif

namespace mckl
{

namespace internal
{

template <typename UIntType, typename RealType,
    int = std::numeric_limits<UIntType>::digits>
class UniformRealAVX2Impl;

template <typename UIntType, typename RealType>
class UniformRealAVX2ImplBase
{
    public:
    static RealType eval(UIntType u, RealType a, RealType b)
    {
#if MCKL_USE_FMA
        return std::fma(
            U01GenericImpl<UIntType, RealType, Closed, Open>::eval(u), b - a,
            a);
#else
        return U01GenericImpl<UIntType, RealType, Closed, Open>::eval(u) *
            (b - a) +
            a;
#endif
    }

    static void eval(
        std::size_t n, const UIntType *u, RealType *r, RealType a, RealType b)
    {
        constexpr std::size_t S = 8;
        constexpr std::size_t N = sizeof(__m256i) * S / sizeof(UIntType);

        while (n >= N) {
            std::array<__m256i, S> s;
            std::memcpy(s.data(), u, sizeof(s));
            UniformRealAVX2Impl<UIntType, RealType>::eval(s, r, a, b);
            n -= N;
            u += N;
            r += N;
        }

        for (std::size_t i = 0; i != n; ++i)
            r[i] = eval(u[i], a, b);
    }
}; // class UniformRealAVX2ImplBase

template <typename UIntType>
class UniformRealAVX2Impl<UIntType, float, 32>
    : public UniformRealAVX2ImplBase<UIntType, float>
{
    public:
    using UniformRealAVX2ImplBase<UIntType, float>::eval;

    template <std::size_t S>
    MCKL_INLINE static void eval(
        std::array<__m128i, S> &s, float *r, float a, float b)
    {
        std::array<__m256i, S / 2> t;
        set_m128i(s, t);
        eval(t, r, a, b);
    }

    template <std::size_t S>
    MCKL_INLINE static void eval(
        std::array<__m256i, S> &s, float *r, float a, float b)
    {
        const __m256i d24 =
            _mm256_castps_si256(_mm256_set1_ps(Pow2<float, -24>::value));
        const __m256i ma = _mm256_castps_si256(_mm256_set1_ps(a));
        const __m256i md = _mm256_castps_si256(_mm256_set1_ps(b - a));

        srli_epi32<8>(s);
        cvtepi32_ps(s);
        mul_ps(s, d24);
#if MCKL_USE_FMA
        fmadd_ps(s, md, ma);
#else
        mul_ps(s, md);
        add_ps(s, ma);
#endif
        std::memcpy(r, s.data(), sizeof(s));
    }
}; // class UniformRealAVX2Impl

template <typename UIntType>
class UniformRealAVX2Impl<UIntType, double, 32>
    : public UniformRealAVX2ImplBase<UIntType, double>
{
    public:
    using UniformRealAVX2ImplBase<UIntType, double>::eval;

    template <typename T, std::size_t S>
    MCKL_INLINE static void eval(
        std::array<T, S> &s, double *r, double a, double b)
    {
        const __m256i d32 =
            _mm256_castpd_si256(_mm256_set1_pd(Pow2<double, -32>::value));
        const __m256i m52 =
            _mm256_castpd_si256(_mm256_set1_pd(Pow2<double, 52>::value));
        const __m256i ma = _mm256_castpd_si256(_mm256_set1_pd(a));
        const __m256i md = _mm256_castpd_si256(_mm256_set1_pd(b - a));

        std::array<__m256i, S * sizeof(T) * 2 / sizeof(__m256i)> t;
        cvtepu32_epi64(s, t);
        add_epi64(t, m52);
        sub_pd(t, m52);
        mul_pd(t, d32);
#if MCKL_USE_FMA
        fmadd_pd(t, md, ma);
#else
        mul_pd(t, md);
        add_pd(t, ma);
#endif
        std::memcpy(r, t.data(), sizeof(t));
    }
}; // class UniformRealAVX2Impl

template <typename UIntType>
class UniformRealAVX2Impl<UIntType, double, 64>
    : public UniformRealAVX2ImplBase<UIntType, double>
{
    public:
    using UniformRealAVX2ImplBase<UIntType, double>::eval;

    template <std::size_t S>
    MCKL_INLINE static void eval(
        std::array<__m128i, S> &s, double *r, double a, double b)
    {
        std::array<__m256i, S / 2> t;
        set_m128i(s, t);

        return eval(t, r, a, b);
    }

    template <std::size_t S>
    MCKL_INLINE static void eval(
        std::array<__m256i, S> &s, double *r, double a, double b)
    {
        const __m256i d53 =
            _mm256_castpd_si256(_mm256_set1_pd(Pow2<double, -53>::value));
        const __m256i ma = _mm256_castpd_si256(_mm256_set1_pd(a));
        const __m256i md = _mm256_castpd_si256(_mm256_set1_pd(b - a));

        srli_epi64<11>(s);
        cvtepi64_pd(s);
        mul_pd(s, d53);
#if MCKL_USE_FMA
        fmadd_pd(s, md, ma);
#else
        mul_pd(s, md);
        add_pd(s, ma);
#endif
        std::memcpy(r, s.data(), sizeof(s));
    }
}; // class UniformRealAVX2Impl

} // namespace mckl::internal

} // namespace mckl

#ifdef MCKL_GCC
#if MCKL_GCC_VERSION >= 60000
#pragma GCC diagnostic pop
#endif
#endif

#endif // MCKL_RANDOM_INTERNAL_UNIFORM_REAL_AVX2_HPP
