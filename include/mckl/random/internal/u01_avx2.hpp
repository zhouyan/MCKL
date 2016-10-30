//============================================================================
// MCKL/include/mckl/random/internal/u01_avx2.hpp
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

#ifndef MCKL_RANDOM_INTERNAL_U01_AVX2_HPP
#define MCKL_RANDOM_INTERNAL_U01_AVX2_HPP

#include <mckl/random/internal/common.hpp>
#include <mckl/random/internal/u01_generic.hpp>

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

template <typename UIntType, typename RealType, typename Lower, typename Upper,
    int = std::numeric_limits<UIntType>::digits>
class U01AVX2Impl;

template <typename UIntType>
class U01AVX2Impl<UIntType, float, Closed, Closed, 32>
{
    public:
    template <std::size_t S>
    MCKL_FLATTEN static float *eval(std::array<__m128i, S> &s, float *r)
    {
        std::array<__m256i, S / 2> t;
        set_m128i(s, t);

        return eval(t, r);
    }

    template <std::size_t S>
    MCKL_FLATTEN static float *eval(std::array<__m256i, S> &s, float *r)
    {
        const __m256i c25 = _mm256_set1_epi32(static_cast<int>(0x33000000));
        const __m256i mask = _mm256_set1_epi32(1);

        std::array<__m256i, S> t;
        slli_epi32<1>(s);
        srli_epi32<7>(s);
        and_si256(s, mask, t);
        add_epi32(s, t);
        cvtepi32_ps(s);
        mul_ps(s, c25);
        std::memcpy(r, s.data(), sizeof(s));

        return r + sizeof(s) / sizeof(float);
    }
}; // class U01AVX2Impl

template <typename UIntType>
class U01AVX2Impl<UIntType, float, Closed, Open, 32>
{
    public:
    template <std::size_t S>
    MCKL_FLATTEN static float *eval(std::array<__m128i, S> &s, float *r)
    {
        std::array<__m256i, S / 2> t;
        set_m128i(s, t);

        return eval(t, r);
    }

    template <std::size_t S>
    MCKL_FLATTEN static float *eval(std::array<__m256i, S> &s, float *r)
    {
        const __m256i c24 = _mm256_set1_epi32(static_cast<int>(0x33800000));

        srli_epi32<8>(s);
        cvtepi32_ps(s);
        mul_ps(s, c24);
        std::memcpy(r, s.data(), sizeof(s));

        return r + sizeof(s) / sizeof(float);
    }
}; // class U01AVX2Impl

template <typename UIntType>
class U01AVX2Impl<UIntType, float, Open, Closed, 32>
{
    public:
    template <std::size_t S>
    MCKL_FLATTEN static float *eval(std::array<__m128i, S> &s, float *r)
    {
        std::array<__m256i, S / 2> t;
        set_m128i(s, t);

        return eval(t, r);
    }

    template <std::size_t S>
    MCKL_FLATTEN static float *eval(std::array<__m256i, S> &s, float *r)
    {
        const __m256i c24 = _mm256_set1_epi32(static_cast<int>(0x33800000));

        srli_epi32<8>(s);
        cvtepi32_ps(s);
#if MCKL_USE_FMA
        fmadd_ps(s, c24, c24);
#else
        mul_ps(s, c24);
        add_ps(s, c24);
#endif
        std::memcpy(r, s.data(), sizeof(s));

        return r + sizeof(s) / sizeof(float);
    }
}; // class U01AVX2Impl

template <typename UIntType>
class U01AVX2Impl<UIntType, float, Open, Open, 32>
{
    public:
    template <std::size_t S>
    MCKL_FLATTEN static float *eval(std::array<__m128i, S> &s, float *r)
    {
        std::array<__m256i, S / 2> t;
        set_m128i(s, t);

        return eval(t, r);
    }

    template <std::size_t S>
    MCKL_FLATTEN static float *eval(std::array<__m256i, S> &s, float *r)
    {
        const __m256i c23 = _mm256_set1_epi32(static_cast<int>(0x34000000));
        const __m256i c24 = _mm256_set1_epi32(static_cast<int>(0x33800000));

        srli_epi32<9>(s);
        cvtepi32_ps(s);
#if MCKL_USE_FMA
        fmadd_ps(s, c23, c24);
#else
        mul_ps(s, c24);
        add_ps(s, c24);
#endif
        std::memcpy(r, s.data(), sizeof(s));

        return r + sizeof(s) / sizeof(float);
    }
}; // class U01AVX2Impl

template <typename UIntType>
class U01AVX2Impl<UIntType, double, Closed, Closed, 32>
{
    public:
    template <typename T, std::size_t S>
    MCKL_FLATTEN static double *eval(std::array<T, S> &s, double *r)
    {
        const __m256i c52 =
            _mm256_set1_epi64x(static_cast<MCKL_INT64>(0x4330000000000000));
        const __m256i c32 =
            _mm256_set1_epi64x(static_cast<MCKL_INT64>(0x3DF0000000000000));
        const __m256i mask = _mm256_set1_epi64x(1);

        std::array<__m256i, S * sizeof(T) * 2 / sizeof(__m256i)> t;
        std::array<__m256i, S * sizeof(T) * 2 / sizeof(__m256i)> u;
        cvtepu32_epi64(s, t);
        and_si256(t, mask, u);
        add_epi64(t, u);
        add_epi64(t, c52);
        sub_pd(t, c52);
        mul_pd(t, c32);
        std::memcpy(r, t.data(), sizeof(t));

        return r + sizeof(t) / sizeof(double);
    }
}; // class U01AVX2Impl

template <typename UIntType>
class U01AVX2Impl<UIntType, double, Closed, Open, 32>
{
    public:
    template <typename T, std::size_t S>
    MCKL_FLATTEN static double *eval(std::array<T, S> &s, double *r)
    {
        const __m256i c32 =
            _mm256_set1_epi64x(static_cast<MCKL_INT64>(0x3DF0000000000000));
        const __m256i c52 =
            _mm256_set1_epi64x(static_cast<MCKL_INT64>(0x4330000000000000));

        std::array<__m256i, S * sizeof(T) * 2 / sizeof(__m256i)> t;
        cvtepu32_epi64(s, t);
        add_epi64(t, c52);
        sub_pd(t, c52);
        mul_pd(t, c32);
        std::memcpy(r, t.data(), sizeof(t));

        return r + sizeof(t) / sizeof(double);
    }
}; // class U01AVX2Impl

template <typename UIntType>
class U01AVX2Impl<UIntType, double, Open, Closed, 32>
{
    public:
    template <typename T, std::size_t S>
    MCKL_FLATTEN static double *eval(std::array<T, S> &s, double *r)
    {
        const __m256i c32 =
            _mm256_set1_epi64x(static_cast<MCKL_INT64>(0x3DF0000000000000));
        const __m256i c52 =
            _mm256_set1_epi64x(static_cast<MCKL_INT64>(0x4330000000000000));

        std::array<__m256i, S * sizeof(T) * 2 / sizeof(__m256i)> t;
        cvtepu32_epi64(s, t);
        add_epi64(t, c52);
        sub_pd(t, c52);
#if MCKL_USE_FMA
        fmadd_pd(t, c32, c32);
#else
        mul_pd(t, c32);
        add_pd(t, c32);
#endif
        std::memcpy(r, t.data(), sizeof(t));

        return r + sizeof(t) / sizeof(double);
    }
}; // class U01AVX2Impl

template <typename UIntType>
class U01AVX2Impl<UIntType, double, Open, Open, 32>
{
    public:
    template <typename T, std::size_t S>
    MCKL_FLATTEN static double *eval(std::array<T, S> &s, double *r)
    {
        const __m256i c32 =
            _mm256_set1_epi64x(static_cast<MCKL_INT64>(0x3DF0000000000000));
        const __m256i c33 =
            _mm256_set1_epi64x(static_cast<MCKL_INT64>(0x3DE0000000000000));
        const __m256i c52 =
            _mm256_set1_epi64x(static_cast<MCKL_INT64>(0x4330000000000000));

        std::array<__m256i, S * sizeof(T) * 2 / sizeof(__m256i)> t;
        cvtepu32_epi64(s, t);
        add_epi64(t, c52);
        sub_pd(t, c52);
#if MCKL_USE_FMA
        fmadd_pd(t, c32, c33);
#else
        mul_pd(t, c32);
        add_pd(t, c33);
#endif
        std::memcpy(r, t.data(), sizeof(t));

        return r + sizeof(t) / sizeof(double);
    }
}; // class U01AVX2Impl

template <typename UIntType, typename RealType, typename Lower, typename Upper>
class U01AVX2Transform
{
    public:
    using uint_type = UIntType;

    template <typename T, std::size_t S>
    MCKL_FLATTEN static RealType *eval(std::array<T, S> &s, RealType *r)
    {
        return U01AVX2Impl<UIntType, RealType, Lower, Upper>::eval(s, r);
    }

    MCKL_FLATTEN static RealType *eval(
        std::size_t n, const UIntType *u, RealType *r)
    {
        for (std::size_t i = 0; i != n; ++i, ++u, ++r)
            *r = U01GenericImpl<UIntType, RealType, Lower, Upper>::eval(*u);

        return r;
    }
}; // class U01AVX2Transform

template <typename UIntType, typename RealType,
    int = std::numeric_limits<UIntType>::digits>
class UniformRealAVX2Impl;

template <typename UIntType>
class UniformRealAVX2Impl<UIntType, float, 32>
{
    public:
    template <std::size_t S>
    MCKL_FLATTEN static float *eval(
        std::array<__m128i, S> &s, float *r, float a, float b)
    {
        std::array<__m256i, S / 2> t;
        set_m128i(s, t);

        return eval(t, r, a, b);
    }

    template <std::size_t S>
    MCKL_FLATTEN static float *eval(
        std::array<__m256i, S> &s, float *r, float a, float b)
    {
        const float d = (b - a) * Pow2Inv<float, 24>::value;

        srli_epi32<8>(s);
        cvtepi32_ps(s);
#if MCKL_USE_FMA
        fmadd_ps(s, _mm256_castps_si256(_mm256_set1_ps(d)),
            _mm256_castps_si256(_mm256_set1_ps(a)));
#else  // MCKL_USE_FMA
        mul_ps(s, _mm256_castps_si256(_mm256_set1_ps(d)));
        add_ps(s, _mm256_castps_si256(_mm256_set1_ps(a)));
#endif // MCKL_USE_FMA
        std::memcpy(r, s.data(), sizeof(s));

        return r + sizeof(s) / sizeof(float);
    }
}; // class UniformRealAVX2Impl

template <typename UIntType>
class UniformRealAVX2Impl<UIntType, double, 32>
{
    public:
    template <typename T, std::size_t S>
    MCKL_FLATTEN static double *eval(
        std::array<T, S> &s, double *r, double a, double b)
    {
        const __m256i c52 =
            _mm256_set1_epi64x(static_cast<MCKL_INT64>(0x4330000000000000));
        const double d = (b - a) * Pow2Inv<double, 32>::value;

        std::array<__m256i, S * sizeof(T) * 2 / sizeof(__m256i)> t;
        cvtepu32_epi64(s, t);
        add_epi64(t, c52);
        sub_pd(t, c52);
#if MCKL_USE_FMA
        fmadd_pd(t, _mm256_castpd_si256(_mm256_set1_pd(d)),
            _mm256_castpd_si256(_mm256_set1_pd(a)));
#else  // MCKL_USE_FMA
        mul_pd(t, _mm256_castpd_si256(_mm256_set1_pd(d)));
        add_ps(t, _mm256_castpd_si256(_mm256_set1_pd(a)));
#endif // MCKL_USE_FMA
        std::memcpy(r, t.data(), sizeof(t));

        return r + sizeof(t) / sizeof(double);
    }
}; // class UniformRealAVX2Impl

template <typename UIntType, typename RealType>
class UniformRealAVX2Transform
{
    public:
    using uint_type = UIntType;

    template <typename T, std::size_t S>
    MCKL_FLATTEN static RealType *eval(
        std::array<T, S> &s, RealType *r, RealType a, RealType b)
    {
        return UniformRealAVX2Impl<UIntType, RealType>::eval(s, r, a, b);
    }

    MCKL_FLATTEN static RealType *eval(
        std::size_t n, const UIntType *u, RealType *r, RealType a, RealType b)
    {
        const RealType d = b - a;
        for (std::size_t i = 0; i != n; ++i, ++u, ++r) {
            *r = U01GenericImpl<UIntType, RealType, Closed, Open>::eval(*u);
#if MCKL_USE_FMA
            *r = std::fma(*r, d, a);
#else
            *r = *r * d + a;
#endif
        }

        return r;
    }
}; // class UniformRealAVX2Transform

template <typename UIntType>
inline void u01_canonical_distribution_avx2_impl_trans(std::size_t n,
    const UIntType *u, double *r, std::integral_constant<int, 32>)
{
    constexpr std::size_t S = 8;
    constexpr std::size_t cstride = sizeof(__m256i) * S;
    constexpr std::size_t nstride = cstride / sizeof(double);
    constexpr std::size_t ustride = cstride / sizeof(UIntType);
    constexpr std::size_t rstride = cstride / sizeof(double);
    const __m256i c32 =
        _mm256_set1_epi64x(static_cast<MCKL_INT64>(0x3DF0000000000000));
    const __m256i c64 =
        _mm256_set1_epi64x(static_cast<MCKL_INT64>(0x3BF0000000000000));
    const __m256i c52 =
        _mm256_set1_epi64x(static_cast<MCKL_INT64>(0x4330000000000000));
    const __m256i m = _mm256_set1_epi64x(static_cast<MCKL_INT64>(0xFFFFFFFF));

    std::array<__m256i, S> s;
    std::array<__m256i, S> t;
    while (n >= nstride) {
        std::memcpy(s.data(), u, cstride);
        std::get<0>(t) = _mm256_srli_epi64(std::get<0>(s), 32);
        std::get<1>(t) = _mm256_srli_epi64(std::get<1>(s), 32);
        std::get<2>(t) = _mm256_srli_epi64(std::get<2>(s), 32);
        std::get<3>(t) = _mm256_srli_epi64(std::get<3>(s), 32);
        std::get<4>(t) = _mm256_srli_epi64(std::get<4>(s), 32);
        std::get<5>(t) = _mm256_srli_epi64(std::get<5>(s), 32);
        std::get<6>(t) = _mm256_srli_epi64(std::get<6>(s), 32);
        std::get<7>(t) = _mm256_srli_epi64(std::get<7>(s), 32);
        std::get<0>(s) = _mm256_and_si256(std::get<0>(s), m);
        std::get<1>(s) = _mm256_and_si256(std::get<1>(s), m);
        std::get<2>(s) = _mm256_and_si256(std::get<2>(s), m);
        std::get<3>(s) = _mm256_and_si256(std::get<3>(s), m);
        std::get<4>(s) = _mm256_and_si256(std::get<4>(s), m);
        std::get<5>(s) = _mm256_and_si256(std::get<5>(s), m);
        std::get<6>(s) = _mm256_and_si256(std::get<6>(s), m);
        std::get<7>(s) = _mm256_and_si256(std::get<7>(s), m);
        add_epi64(s, c52);
        add_epi64(t, c52);
        sub_pd(s, c52);
        sub_pd(t, c52);
        mul_pd(s, c64);
        mul_pd(t, c32);
        add_pd(s, t);
        std::memcpy(r, s.data(), cstride);
        n -= nstride;
        u += ustride;
        r += rstride;
    }
    for (std::size_t i = 0; i != n; ++i, u += 2, ++r) {
        *r = u[0] * Pow2Inv<double, 64>::value +
            u[1] * Pow2Inv<double, 32>::value;
    }
}

} // namespace mckl::internal

} // namespace mckl

#ifdef MCKL_GCC
#if MCKL_GCC_VERSION >= 60000
#pragma GCC diagnostic pop
#endif
#endif

#endif // MCKL_RANDOM_INTERNAL_U01_AVX2_HPP
