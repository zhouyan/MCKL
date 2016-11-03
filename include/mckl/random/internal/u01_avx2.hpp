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
class U01AVX2Impl : public U01GenericImpl<UIntType, RealType, Lower, Upper>
{
}; // U01AVX2Impl

template <typename UIntType, typename RealType, typename Lower, typename Upper>
class U01AVX2ImplBase
{
    public:
    using uint_type = UIntType;

    static RealType eval(UIntType u)
    {
        return U01GenericImpl<UIntType, RealType, Lower, Upper>::eval(u);
    }

    static void eval(std::size_t n, const UIntType *u, RealType *r)
    {
        constexpr std::size_t S = 8;
        constexpr std::size_t cstride = sizeof(__m256i) * S;
        constexpr std::size_t nstride = cstride / sizeof(UIntType);

        std::array<__m256i, S> s;
        while (n >= nstride) {
            std::memcpy(s.data(), u, cstride);
            r = U01AVX2Impl<UIntType, RealType, Lower, Upper>::eval(s, r);
            n -= nstride;
            u += nstride;
        }
        for (std::size_t i = 0; i != n; ++i)
            r[i] = eval(u[i]);
    }
}; // class U01AVX2ImplBase

template <typename UIntType>
class U01AVX2Impl<UIntType, float, Closed, Closed, 32>
    : public U01AVX2ImplBase<UIntType, float, Closed, Closed>
{
    static_assert(std::numeric_limits<UIntType>::digits == 32,
        "**U01AVX2Impl** used with unsigned integer type with incorrect "
        "width");

    public:
    using U01AVX2ImplBase<UIntType, float, Closed, Closed>::eval;

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
        const __m256i d25 =
            _mm256_castps_si256(_mm256_set1_ps(Pow2<float, -25>::value));
        const __m256i mask = _mm256_set1_epi32(1);

        std::array<__m256i, S> t;
        slli_epi32<1>(s);
        srli_epi32<7>(s);
        and_si256(s, mask, t);
        add_epi32(s, t);
        cvtepi32_ps(s);
        mul_ps(s, d25);
        std::memcpy(r, s.data(), sizeof(s));

        return r + sizeof(s) / sizeof(float);
    }
}; // class U01AVX2Impl

template <typename UIntType>
class U01AVX2Impl<UIntType, float, Closed, Open, 32>
    : public U01AVX2ImplBase<UIntType, float, Closed, Open>
{
    static_assert(std::numeric_limits<UIntType>::digits == 32,
        "**U01AVX2Impl** used with unsigned integer type with incorrect "
        "width");

    public:
    using U01AVX2ImplBase<UIntType, float, Closed, Open>::eval;

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
        const __m256i d24 =
            _mm256_castps_si256(_mm256_set1_ps(Pow2<float, -24>::value));

        srli_epi32<8>(s);
        cvtepi32_ps(s);
        mul_ps(s, d24);
        std::memcpy(r, s.data(), sizeof(s));

        return r + sizeof(s) / sizeof(float);
    }
}; // class U01AVX2Impl

template <typename UIntType>
class U01AVX2Impl<UIntType, float, Open, Closed, 32>
    : public U01AVX2ImplBase<UIntType, float, Open, Closed>
{
    static_assert(std::numeric_limits<UIntType>::digits == 32,
        "**U01AVX2Impl** used with unsigned integer type with incorrect "
        "width");

    public:
    using U01AVX2ImplBase<UIntType, float, Open, Closed>::eval;

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
        const __m256i d24 =
            _mm256_castps_si256(_mm256_set1_ps(Pow2<float, -24>::value));

        srli_epi32<8>(s);
        cvtepi32_ps(s);
#if MCKL_USE_FMA
        fmadd_ps(s, d24, d24);
#else
        mul_ps(s, d24);
        add_ps(s, d24);
#endif
        std::memcpy(r, s.data(), sizeof(s));

        return r + sizeof(s) / sizeof(float);
    }
}; // class U01AVX2Impl

template <typename UIntType>
class U01AVX2Impl<UIntType, float, Open, Open, 32>
    : public U01AVX2ImplBase<UIntType, float, Open, Open>
{
    static_assert(std::numeric_limits<UIntType>::digits == 32,
        "**U01AVX2Impl** used with unsigned integer type with incorrect "
        "width");

    public:
    using U01AVX2ImplBase<UIntType, float, Open, Open>::eval;

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
        const __m256i d23 =
            _mm256_castps_si256(_mm256_set1_ps(Pow2<float, -23>::value));
        const __m256i d24 =
            _mm256_castps_si256(_mm256_set1_ps(Pow2<float, -24>::value));

        srli_epi32<9>(s);
        cvtepi32_ps(s);
#if MCKL_USE_FMA
        fmadd_ps(s, d23, d24);
#else
        mul_ps(s, d24);
        add_ps(s, d24);
#endif
        std::memcpy(r, s.data(), sizeof(s));

        return r + sizeof(s) / sizeof(float);
    }
}; // class U01AVX2Impl

template <typename UIntType>
class U01AVX2Impl<UIntType, double, Closed, Closed, 32>
    : public U01AVX2ImplBase<UIntType, double, Closed, Closed>
{
    static_assert(std::numeric_limits<UIntType>::digits == 32,
        "**U01AVX2Impl** used with unsigned integer type with incorrect "
        "width");

    public:
    using U01AVX2ImplBase<UIntType, double, Closed, Closed>::eval;

    template <typename T, std::size_t S>
    MCKL_FLATTEN static double *eval(std::array<T, S> &s, double *r)
    {
        const __m256i d32 =
            _mm256_castpd_si256(_mm256_set1_pd(Pow2<double, -32>::value));
        const __m256i m52 =
            _mm256_castpd_si256(_mm256_set1_pd(Pow2<double, 52>::value));
        const __m256i mask = _mm256_set1_epi64x(1);

        std::array<__m256i, S * sizeof(T) * 2 / sizeof(__m256i)> t;
        std::array<__m256i, S * sizeof(T) * 2 / sizeof(__m256i)> u;
        cvtepu32_epi64(s, t);
        and_si256(t, mask, u);
        add_epi64(t, u);
        add_epi64(t, m52);
        sub_pd(t, m52);
        mul_pd(t, d32);
        std::memcpy(r, t.data(), sizeof(t));

        return r + sizeof(t) / sizeof(double);
    }
}; // class U01AVX2Impl

template <typename UIntType>
class U01AVX2Impl<UIntType, double, Closed, Open, 32>
    : public U01AVX2ImplBase<UIntType, double, Closed, Open>
{
    static_assert(std::numeric_limits<UIntType>::digits == 32,
        "**U01AVX2Impl** used with unsigned integer type with incorrect "
        "width");

    public:
    using U01AVX2ImplBase<UIntType, double, Closed, Open>::eval;

    template <typename T, std::size_t S>
    MCKL_FLATTEN static double *eval(std::array<T, S> &s, double *r)
    {
        const __m256i d32 =
            _mm256_castpd_si256(_mm256_set1_pd(Pow2<double, -32>::value));
        const __m256i m52 =
            _mm256_castpd_si256(_mm256_set1_pd(Pow2<double, 52>::value));

        std::array<__m256i, S * sizeof(T) * 2 / sizeof(__m256i)> t;
        cvtepu32_epi64(s, t);
        add_epi64(t, m52);
        sub_pd(t, m52);
        mul_pd(t, d32);
        std::memcpy(r, t.data(), sizeof(t));

        return r + sizeof(t) / sizeof(double);
    }
}; // class U01AVX2Impl

template <typename UIntType>
class U01AVX2Impl<UIntType, double, Open, Closed, 32>
    : public U01AVX2ImplBase<UIntType, double, Open, Closed>
{
    static_assert(std::numeric_limits<UIntType>::digits == 32,
        "**U01AVX2Impl** used with unsigned integer type with incorrect "
        "width");

    public:
    using U01AVX2ImplBase<UIntType, double, Open, Closed>::eval;

    template <typename T, std::size_t S>
    MCKL_FLATTEN static double *eval(std::array<T, S> &s, double *r)
    {
        const __m256i d32 =
            _mm256_castpd_si256(_mm256_set1_pd(Pow2<double, -32>::value));
        const __m256i m52 =
            _mm256_castpd_si256(_mm256_set1_pd(Pow2<double, 52>::value));

        std::array<__m256i, S * sizeof(T) * 2 / sizeof(__m256i)> t;
        cvtepu32_epi64(s, t);
        add_epi64(t, m52);
        sub_pd(t, m52);
#if MCKL_USE_FMA
        fmadd_pd(t, d32, d32);
#else
        mul_pd(t, d32);
        add_pd(t, d32);
#endif
        std::memcpy(r, t.data(), sizeof(t));

        return r + sizeof(t) / sizeof(double);
    }
}; // class U01AVX2Impl

template <typename UIntType>
class U01AVX2Impl<UIntType, double, Open, Open, 32>
    : public U01AVX2ImplBase<UIntType, double, Open, Open>
{
    static_assert(std::numeric_limits<UIntType>::digits == 32,
        "**U01AVX2Impl** used with unsigned integer type with incorrect "
        "width");

    public:
    using U01AVX2ImplBase<UIntType, double, Open, Open>::eval;

    template <typename T, std::size_t S>
    MCKL_FLATTEN static double *eval(std::array<T, S> &s, double *r)
    {
        const __m256i d32 =
            _mm256_castpd_si256(_mm256_set1_pd(Pow2<double, -32>::value));
        const __m256i d33 =
            _mm256_castpd_si256(_mm256_set1_pd(Pow2<double, -33>::value));
        const __m256i m52 =
            _mm256_castpd_si256(_mm256_set1_pd(Pow2<double, 52>::value));

        std::array<__m256i, S * sizeof(T) * 2 / sizeof(__m256i)> t;
        cvtepu32_epi64(s, t);
        add_epi64(t, m52);
        sub_pd(t, m52);
#if MCKL_USE_FMA
        fmadd_pd(t, d32, d33);
#else
        mul_pd(t, d32);
        add_pd(t, d33);
#endif
        std::memcpy(r, t.data(), sizeof(t));

        return r + sizeof(t) / sizeof(double);
    }
}; // class U01AVX2Impl

template <typename UIntType>
class U01AVX2Impl<UIntType, double, Closed, Closed, 64>
    : public U01AVX2ImplBase<UIntType, double, Closed, Closed>
{
    static_assert(std::numeric_limits<UIntType>::digits == 64,
        "**U01AVX2Impl** used with unsigned integer type with incorrect "
        "width");

    public:
    using U01AVX2ImplBase<UIntType, double, Closed, Closed>::eval;

    template <std::size_t S>
    MCKL_FLATTEN static double *eval(std::array<__m128i, S> &s, double *r)
    {
        std::array<__m256i, S / 2> t;
        set_m128i(s, t);

        return eval(t, r);
    }

    template <std::size_t S>
    MCKL_FLATTEN static double *eval(std::array<__m256i, S> &s, double *r)
    {
        const __m256i d54 =
            _mm256_castpd_si256(_mm256_set1_pd(Pow2<double, -54>::value));
        const __m256i mask = _mm256_set1_epi64x(1);

        std::array<__m256i, S> t;
        slli_epi64<1>(s);
        srli_epi64<10>(s);
        and_si256(s, mask, t);
        add_epi64(s, t);
        cvtepi64_pd(s);
        mul_pd(s, d54);
        std::memcpy(r, s.data(), sizeof(s));

        return r + sizeof(s) / sizeof(double);
    }
}; // class U01AVX2Impl

template <typename UIntType>
class U01AVX2Impl<UIntType, double, Closed, Open, 64>
    : public U01AVX2ImplBase<UIntType, double, Closed, Open>
{
    static_assert(std::numeric_limits<UIntType>::digits == 64,
        "**U01AVX2Impl** used with unsigned integer type with incorrect "
        "width");

    public:
    using U01AVX2ImplBase<UIntType, double, Closed, Open>::eval;

    template <std::size_t S>
    MCKL_FLATTEN static double *eval(std::array<__m128i, S> &s, double *r)
    {
        std::array<__m256i, S / 2> t;
        set_m128i(s, t);

        return eval(t, r);
    }

    template <std::size_t S>
    MCKL_FLATTEN static double *eval(std::array<__m256i, S> &s, double *r)
    {
        const __m256i d53 =
            _mm256_castpd_si256(_mm256_set1_pd(Pow2<double, -53>::value));

        srli_epi64<11>(s);
        cvtepi64_pd(s);
        mul_pd(s, d53);
        std::memcpy(r, s.data(), sizeof(s));

        return r + sizeof(s) / sizeof(double);
    }
}; // class U01AVX2Impl

template <typename UIntType>
class U01AVX2Impl<UIntType, double, Open, Closed, 64>
    : public U01AVX2ImplBase<UIntType, double, Open, Closed>
{
    static_assert(std::numeric_limits<UIntType>::digits == 64,
        "**U01AVX2Impl** used with unsigned integer type with incorrect "
        "width");

    public:
    using U01AVX2ImplBase<UIntType, double, Open, Closed>::eval;

    template <std::size_t S>
    MCKL_FLATTEN static double *eval(std::array<__m128i, S> &s, double *r)
    {
        std::array<__m256i, S / 2> t;
        set_m128i(s, t);

        return eval(t, r);
    }

    template <std::size_t S>
    MCKL_FLATTEN static double *eval(std::array<__m256i, S> &s, double *r)
    {
        const __m256i d53 =
            _mm256_castpd_si256(_mm256_set1_pd(Pow2<double, -53>::value));

        srli_epi64<11>(s);
        cvtepi64_pd(s);
#if MCKL_USE_FMA
        fmadd_pd(s, d53, d53);
#else
        mul_pd(s, d53);
        add_pd(s, d53);
#endif
        std::memcpy(r, s.data(), sizeof(s));

        return r + sizeof(s) / sizeof(double);
    }
}; // class U01AVX2Impl

template <typename UIntType>
class U01AVX2Impl<UIntType, double, Open, Open, 64>
    : public U01AVX2ImplBase<UIntType, double, Open, Open>
{
    static_assert(std::numeric_limits<UIntType>::digits == 64,
        "**U01AVX2Impl** used with unsigned integer type with incorrect "
        "width");

    public:
    using U01AVX2ImplBase<UIntType, double, Open, Open>::eval;

    template <std::size_t S>
    MCKL_FLATTEN static double *eval(std::array<__m128i, S> &s, double *r)
    {
        std::array<__m256i, S / 2> t;
        set_m128i(s, t);

        return eval(t, r);
    }

    template <std::size_t S>
    MCKL_FLATTEN static double *eval(std::array<__m256i, S> &s, double *r)
    {
        const __m256i d52 =
            _mm256_castpd_si256(_mm256_set1_pd(Pow2<double, -52>::value));
        const __m256i d53 =
            _mm256_castpd_si256(_mm256_set1_pd(Pow2<double, -53>::value));

        srli_epi64<12>(s);
        cvtepi64_pd(s);
#if MCKL_USE_FMA
        fmadd_pd(s, d52, d53);
#else
        mul_pd(s, d53);
        add_pd(s, d53);
#endif
        std::memcpy(r, s.data(), sizeof(s));

        return r + sizeof(s) / sizeof(double);
    }
}; // class U01AVX2Impl

} // namespace mckl::internal

} // namespace mckl

#ifdef MCKL_GCC
#if MCKL_GCC_VERSION >= 60000
#pragma GCC diagnostic pop
#endif
#endif

#endif // MCKL_RANDOM_INTERNAL_U01_AVX2_HPP
