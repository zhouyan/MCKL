//============================================================================
// MCKL/include/mckl/internal/avx2.hpp
//----------------------------------------------------------------------------
// MCKL: Monte Carlo Kernel Library
//----------------------------------------------------------------------------
// Copyright (c) 2013-2017, Yan Zhou
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

#ifndef MCKL_INTERNAL_AVX2_HPP
#define MCKL_INTERNAL_AVX2_HPP

#include <mckl/internal/config.h>
#include <mckl/internal/avx2_op.hpp>
#include <mckl/internal/const_math.hpp>
#include <array>

#if MCKL_HAS_FMA
#include <mckl/internal/fma.hpp>
#endif

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

MCKL_INLINE inline void set_m128i(
    const std::array<__m128i, 8> &s, std::array<__m256i, 4> &t)
{
    std::get<0>(t) = _mm256_insertf128_si256(
        _mm256_castsi128_si256(std::get<0>(s)), std::get<1>(s), 1);
    std::get<1>(t) = _mm256_insertf128_si256(
        _mm256_castsi128_si256(std::get<2>(s)), std::get<3>(s), 1);
    std::get<2>(t) = _mm256_insertf128_si256(
        _mm256_castsi128_si256(std::get<4>(s)), std::get<5>(s), 1);
    std::get<3>(t) = _mm256_insertf128_si256(
        _mm256_castsi128_si256(std::get<6>(s)), std::get<7>(s), 1);
}

MCKL_INLINE inline void set_m128i(
    const std::array<__m128i, 16> &s, std::array<__m256i, 8> &t)
{
    std::get<0>(t) = _mm256_insertf128_si256(
        _mm256_castsi128_si256(std::get<0x0>(s)), std::get<0x1>(s), 1);
    std::get<1>(t) = _mm256_insertf128_si256(
        _mm256_castsi128_si256(std::get<0x2>(s)), std::get<0x3>(s), 1);
    std::get<2>(t) = _mm256_insertf128_si256(
        _mm256_castsi128_si256(std::get<0x4>(s)), std::get<0x5>(s), 1);
    std::get<3>(t) = _mm256_insertf128_si256(
        _mm256_castsi128_si256(std::get<0x6>(s)), std::get<0x7>(s), 1);
    std::get<4>(t) = _mm256_insertf128_si256(
        _mm256_castsi128_si256(std::get<0x8>(s)), std::get<0x9>(s), 1);
    std::get<5>(t) = _mm256_insertf128_si256(
        _mm256_castsi128_si256(std::get<0xA>(s)), std::get<0xB>(s), 1);
    std::get<6>(t) = _mm256_insertf128_si256(
        _mm256_castsi128_si256(std::get<0xC>(s)), std::get<0xD>(s), 1);
    std::get<7>(t) = _mm256_insertf128_si256(
        _mm256_castsi128_si256(std::get<0xE>(s)), std::get<0xF>(s), 1);
}

MCKL_INLINE inline void cvtepu32_epi64(
    const std::array<__m128i, 4> &s, std::array<__m256i, 4> &t)
{
    std::get<0>(t) = _mm256_cvtepu32_epi64(std::get<0>(s));
    std::get<1>(t) = _mm256_cvtepu32_epi64(std::get<1>(s));
    std::get<2>(t) = _mm256_cvtepu32_epi64(std::get<2>(s));
    std::get<3>(t) = _mm256_cvtepu32_epi64(std::get<3>(s));
}

MCKL_INLINE inline void cvtepu32_epi64(
    const std::array<__m128i, 8> &s, std::array<__m256i, 8> &t)
{
    std::get<0>(t) = _mm256_cvtepu32_epi64(std::get<0>(s));
    std::get<1>(t) = _mm256_cvtepu32_epi64(std::get<1>(s));
    std::get<2>(t) = _mm256_cvtepu32_epi64(std::get<2>(s));
    std::get<3>(t) = _mm256_cvtepu32_epi64(std::get<3>(s));
    std::get<4>(t) = _mm256_cvtepu32_epi64(std::get<4>(s));
    std::get<5>(t) = _mm256_cvtepu32_epi64(std::get<5>(s));
    std::get<6>(t) = _mm256_cvtepu32_epi64(std::get<6>(s));
    std::get<7>(t) = _mm256_cvtepu32_epi64(std::get<7>(s));
}

MCKL_INLINE inline void cvtepu32_epi64(
    const std::array<__m128i, 16> &s, std::array<__m256i, 16> &t)
{
    std::get<0x0>(t) = _mm256_cvtepu32_epi64(std::get<0x0>(s));
    std::get<0x1>(t) = _mm256_cvtepu32_epi64(std::get<0x1>(s));
    std::get<0x2>(t) = _mm256_cvtepu32_epi64(std::get<0x2>(s));
    std::get<0x3>(t) = _mm256_cvtepu32_epi64(std::get<0x3>(s));
    std::get<0x4>(t) = _mm256_cvtepu32_epi64(std::get<0x4>(s));
    std::get<0x5>(t) = _mm256_cvtepu32_epi64(std::get<0x5>(s));
    std::get<0x6>(t) = _mm256_cvtepu32_epi64(std::get<0x6>(s));
    std::get<0x7>(t) = _mm256_cvtepu32_epi64(std::get<0x7>(s));
    std::get<0x8>(t) = _mm256_cvtepu32_epi64(std::get<0x8>(s));
    std::get<0x9>(t) = _mm256_cvtepu32_epi64(std::get<0x9>(s));
    std::get<0xA>(t) = _mm256_cvtepu32_epi64(std::get<0xA>(s));
    std::get<0xB>(t) = _mm256_cvtepu32_epi64(std::get<0xB>(s));
    std::get<0xC>(t) = _mm256_cvtepu32_epi64(std::get<0xC>(s));
    std::get<0xD>(t) = _mm256_cvtepu32_epi64(std::get<0xD>(s));
    std::get<0xE>(t) = _mm256_cvtepu32_epi64(std::get<0xE>(s));
    std::get<0xF>(t) = _mm256_cvtepu32_epi64(std::get<0xF>(s));
}

MCKL_INLINE inline void cvtepu32_epi64(
    const std::array<__m256i, 2> &s, std::array<__m256i, 4> &t)
{
    const __m256i p = _mm256_set_epi32(7, 3, 6, 2, 5, 1, 4, 0);
    const __m256i m = _mm256_set1_epi64x(static_cast<MCKL_INT64>(0xFFFFFFFF));

    std::get<0>(t) = _mm256_permutevar8x32_epi32(std::get<0>(s), p);
    std::get<2>(t) = _mm256_permutevar8x32_epi32(std::get<1>(s), p);

    std::get<1>(t) = _mm256_srli_epi64(std::get<0>(t), 32);
    std::get<3>(t) = _mm256_srli_epi64(std::get<2>(t), 32);

    std::get<0>(t) = _mm256_and_si256(std::get<0>(t), m);
    std::get<2>(t) = _mm256_and_si256(std::get<2>(t), m);
}

MCKL_INLINE inline void cvtepu32_epi64(
    const std::array<__m256i, 4> &s, std::array<__m256i, 8> &t)
{
    const __m256i p = _mm256_set_epi32(7, 3, 6, 2, 5, 1, 4, 0);
    const __m256i m = _mm256_set1_epi64x(static_cast<MCKL_INT64>(0xFFFFFFFF));

    std::get<0>(t) = _mm256_permutevar8x32_epi32(std::get<0>(s), p);
    std::get<2>(t) = _mm256_permutevar8x32_epi32(std::get<1>(s), p);
    std::get<4>(t) = _mm256_permutevar8x32_epi32(std::get<2>(s), p);
    std::get<6>(t) = _mm256_permutevar8x32_epi32(std::get<3>(s), p);

    std::get<1>(t) = _mm256_srli_epi64(std::get<0>(t), 32);
    std::get<3>(t) = _mm256_srli_epi64(std::get<2>(t), 32);
    std::get<5>(t) = _mm256_srli_epi64(std::get<4>(t), 32);
    std::get<7>(t) = _mm256_srli_epi64(std::get<6>(t), 32);

    std::get<0>(t) = _mm256_and_si256(std::get<0>(t), m);
    std::get<2>(t) = _mm256_and_si256(std::get<2>(t), m);
    std::get<4>(t) = _mm256_and_si256(std::get<4>(t), m);
    std::get<6>(t) = _mm256_and_si256(std::get<6>(t), m);
}

MCKL_INLINE inline void cvtepu32_epi64(
    const std::array<__m256i, 8> &s, std::array<__m256i, 16> &t)
{
    const __m256i p = _mm256_set_epi32(7, 3, 6, 2, 5, 1, 4, 0);
    const __m256i m = _mm256_set1_epi64x(static_cast<MCKL_INT64>(0xFFFFFFFF));

    std::get<0x0>(t) = _mm256_permutevar8x32_epi32(std::get<0>(s), p);
    std::get<0x2>(t) = _mm256_permutevar8x32_epi32(std::get<1>(s), p);
    std::get<0x4>(t) = _mm256_permutevar8x32_epi32(std::get<2>(s), p);
    std::get<0x6>(t) = _mm256_permutevar8x32_epi32(std::get<3>(s), p);
    std::get<0x8>(t) = _mm256_permutevar8x32_epi32(std::get<4>(s), p);
    std::get<0xA>(t) = _mm256_permutevar8x32_epi32(std::get<5>(s), p);
    std::get<0xC>(t) = _mm256_permutevar8x32_epi32(std::get<6>(s), p);
    std::get<0xE>(t) = _mm256_permutevar8x32_epi32(std::get<7>(s), p);

    std::get<0x1>(t) = _mm256_srli_epi64(std::get<0x0>(t), 32);
    std::get<0x3>(t) = _mm256_srli_epi64(std::get<0x2>(t), 32);
    std::get<0x5>(t) = _mm256_srli_epi64(std::get<0x4>(t), 32);
    std::get<0x7>(t) = _mm256_srli_epi64(std::get<0x6>(t), 32);
    std::get<0x9>(t) = _mm256_srli_epi64(std::get<0x8>(t), 32);
    std::get<0xB>(t) = _mm256_srli_epi64(std::get<0xA>(t), 32);
    std::get<0xD>(t) = _mm256_srli_epi64(std::get<0xC>(t), 32);
    std::get<0xF>(t) = _mm256_srli_epi64(std::get<0xE>(t), 32);

    std::get<0x0>(t) = _mm256_and_si256(std::get<0x0>(t), m);
    std::get<0x2>(t) = _mm256_and_si256(std::get<0x2>(t), m);
    std::get<0x4>(t) = _mm256_and_si256(std::get<0x4>(t), m);
    std::get<0x6>(t) = _mm256_and_si256(std::get<0x6>(t), m);
    std::get<0x8>(t) = _mm256_and_si256(std::get<0x8>(t), m);
    std::get<0xA>(t) = _mm256_and_si256(std::get<0xA>(t), m);
    std::get<0xC>(t) = _mm256_and_si256(std::get<0xC>(t), m);
    std::get<0xE>(t) = _mm256_and_si256(std::get<0xE>(t), m);
}

MCKL_INLINE inline void cvtepi32_ps(std::array<__m256i, 4> &s)
{
    std::get<0>(s) = _mm256_castps_si256(_mm256_cvtepi32_ps(std::get<0>(s)));
    std::get<1>(s) = _mm256_castps_si256(_mm256_cvtepi32_ps(std::get<1>(s)));
    std::get<2>(s) = _mm256_castps_si256(_mm256_cvtepi32_ps(std::get<2>(s)));
    std::get<3>(s) = _mm256_castps_si256(_mm256_cvtepi32_ps(std::get<3>(s)));
}

MCKL_INLINE inline void cvtepi32_ps(std::array<__m256i, 8> &s)
{
    std::get<0>(s) = _mm256_castps_si256(_mm256_cvtepi32_ps(std::get<0>(s)));
    std::get<1>(s) = _mm256_castps_si256(_mm256_cvtepi32_ps(std::get<1>(s)));
    std::get<2>(s) = _mm256_castps_si256(_mm256_cvtepi32_ps(std::get<2>(s)));
    std::get<3>(s) = _mm256_castps_si256(_mm256_cvtepi32_ps(std::get<3>(s)));
    std::get<4>(s) = _mm256_castps_si256(_mm256_cvtepi32_ps(std::get<4>(s)));
    std::get<5>(s) = _mm256_castps_si256(_mm256_cvtepi32_ps(std::get<5>(s)));
    std::get<6>(s) = _mm256_castps_si256(_mm256_cvtepi32_ps(std::get<6>(s)));
    std::get<7>(s) = _mm256_castps_si256(_mm256_cvtepi32_ps(std::get<7>(s)));
}

MCKL_INLINE inline void cvtepi32_ps(std::array<__m256i, 16> &s)
{
    std::get<0x0>(s) =
        _mm256_castps_si256(_mm256_cvtepi32_ps(std::get<0x0>(s)));
    std::get<0x1>(s) =
        _mm256_castps_si256(_mm256_cvtepi32_ps(std::get<0x1>(s)));
    std::get<0x2>(s) =
        _mm256_castps_si256(_mm256_cvtepi32_ps(std::get<0x2>(s)));
    std::get<0x3>(s) =
        _mm256_castps_si256(_mm256_cvtepi32_ps(std::get<0x3>(s)));
    std::get<0x4>(s) =
        _mm256_castps_si256(_mm256_cvtepi32_ps(std::get<0x4>(s)));
    std::get<0x5>(s) =
        _mm256_castps_si256(_mm256_cvtepi32_ps(std::get<0x5>(s)));
    std::get<0x6>(s) =
        _mm256_castps_si256(_mm256_cvtepi32_ps(std::get<0x6>(s)));
    std::get<0x7>(s) =
        _mm256_castps_si256(_mm256_cvtepi32_ps(std::get<0x7>(s)));
    std::get<0x8>(s) =
        _mm256_castps_si256(_mm256_cvtepi32_ps(std::get<0x8>(s)));
    std::get<0x9>(s) =
        _mm256_castps_si256(_mm256_cvtepi32_ps(std::get<0x9>(s)));
    std::get<0xA>(s) =
        _mm256_castps_si256(_mm256_cvtepi32_ps(std::get<0xA>(s)));
    std::get<0xB>(s) =
        _mm256_castps_si256(_mm256_cvtepi32_ps(std::get<0xB>(s)));
    std::get<0xC>(s) =
        _mm256_castps_si256(_mm256_cvtepi32_ps(std::get<0xC>(s)));
    std::get<0xD>(s) =
        _mm256_castps_si256(_mm256_cvtepi32_ps(std::get<0xD>(s)));
    std::get<0xE>(s) =
        _mm256_castps_si256(_mm256_cvtepi32_ps(std::get<0xE>(s)));
    std::get<0xF>(s) =
        _mm256_castps_si256(_mm256_cvtepi32_ps(std::get<0xF>(s)));
}

template <std::size_t S>
MCKL_INLINE inline void cvtepi64_pd(std::array<__m256i, S> &s)
{
    const __m256i m32 =
        _mm256_castpd_si256(_mm256_set1_pd(Pow2<double, 32>::value));
    const __m256i m52 =
        _mm256_castpd_si256(_mm256_set1_pd(Pow2<double, 52>::value));
    const __m256i mask =
        _mm256_set1_epi64x(static_cast<MCKL_INT64>(0xFFFFFFFF));

    std::array<__m256i, S> t;
    and_si256(s, mask, t);
    srli_epi64<32>(s);
    add_epi64(s, m52);
    add_epi64(t, m52);
    sub_pd(s, m52);
    sub_pd(t, m52);
#if MCKL_USE_FMA
    fmadd_pd(s, m32, t);
#else
    mul_pd(s, m32);
    add_pd(s, t);
#endif
}

template <std::size_t I0, std::size_t I1, std::size_t I2, std::size_t I3,
    std::size_t I4, std::size_t I5, std::size_t I6, std::size_t I7,
    std::size_t N>
MCKL_INLINE inline void transpose8x32_si256(std::array<__m256i, N> &s)
{
    __m256i t0 = _mm256_unpacklo_epi32(std::get<I0>(s), std::get<I1>(s));
    __m256i t1 = _mm256_unpacklo_epi32(std::get<I2>(s), std::get<I3>(s));
    __m256i t2 = _mm256_unpacklo_epi32(std::get<I4>(s), std::get<I5>(s));
    __m256i t3 = _mm256_unpacklo_epi32(std::get<I6>(s), std::get<I7>(s));
    __m256i t4 = _mm256_unpackhi_epi32(std::get<I0>(s), std::get<I1>(s));
    __m256i t5 = _mm256_unpackhi_epi32(std::get<I2>(s), std::get<I3>(s));
    __m256i t6 = _mm256_unpackhi_epi32(std::get<I4>(s), std::get<I5>(s));
    __m256i t7 = _mm256_unpackhi_epi32(std::get<I6>(s), std::get<I7>(s));
    __m256i t8 = _mm256_unpacklo_epi64(t0, t1);
    __m256i t9 = _mm256_unpacklo_epi64(t2, t3);
    __m256i tA = _mm256_unpacklo_epi64(t4, t5);
    __m256i tB = _mm256_unpacklo_epi64(t6, t7);
    __m256i tC = _mm256_unpackhi_epi64(t0, t1);
    __m256i tD = _mm256_unpackhi_epi64(t2, t3);
    __m256i tE = _mm256_unpackhi_epi64(t4, t5);
    __m256i tF = _mm256_unpackhi_epi64(t6, t7);
    std::get<I0>(s) = _mm256_permute2x128_si256(t8, t9, 0x20);
    std::get<I1>(s) = _mm256_permute2x128_si256(tC, tD, 0x20);
    std::get<I2>(s) = _mm256_permute2x128_si256(tA, tB, 0x20);
    std::get<I3>(s) = _mm256_permute2x128_si256(tE, tF, 0x20);
    std::get<I4>(s) = _mm256_permute2x128_si256(t8, t9, 0x31);
    std::get<I5>(s) = _mm256_permute2x128_si256(tC, tD, 0x31);
    std::get<I6>(s) = _mm256_permute2x128_si256(tA, tB, 0x31);
    std::get<I7>(s) = _mm256_permute2x128_si256(tE, tF, 0x31);
}

MCKL_INLINE inline void transpose8x32_load_si256(std::array<__m256i, 8> &s)
{
    transpose8x32_si256<0, 1, 2, 3, 4, 5, 6, 7>(s);
}

MCKL_INLINE inline void transpose8x32_store_si256(std::array<__m256i, 8> &s)
{
    transpose8x32_si256<0, 1, 2, 3, 4, 5, 6, 7>(s);
}

MCKL_INLINE inline void transpose8x32_load_si256(std::array<__m256i, 16> &s)
{
    __m256i s1 = std::get<0x1>(s);
    std::get<0x1>(s) = std::get<0x2>(s);
    std::get<0x2>(s) = std::get<0x4>(s);
    std::get<0x4>(s) = std::get<0x8>(s);
    std::get<0x8>(s) = s1;

    __m256i s3 = std::get<0x3>(s);
    std::get<0x3>(s) = std::get<0x6>(s);
    std::get<0x6>(s) = std::get<0xC>(s);
    std::get<0xC>(s) = std::get<0x9>(s);
    std::get<0x9>(s) = s3;

    __m256i s7 = std::get<0x7>(s);
    std::get<0x7>(s) = std::get<0xE>(s);
    std::get<0xE>(s) = std::get<0xD>(s);
    std::get<0xD>(s) = std::get<0xB>(s);
    std::get<0xB>(s) = s7;

    __m256i s5 = std::get<0x5>(s);
    std::get<0x5>(s) = std::get<0xA>(s);
    std::get<0xA>(s) = s5;

    transpose8x32_si256<0x0, 0x1, 0x2, 0x3, 0x4, 0x5, 0x6, 0x7>(s);
    transpose8x32_si256<0x8, 0x9, 0xA, 0xB, 0xC, 0xD, 0xE, 0xF>(s);
}

MCKL_INLINE inline void transpose8x32_store_si256(std::array<__m256i, 16> &s)
{
    transpose8x32_si256<0x0, 0x1, 0x2, 0x3, 0x4, 0x5, 0x6, 0x7>(s);
    transpose8x32_si256<0x8, 0x9, 0xA, 0xB, 0xC, 0xD, 0xE, 0xF>(s);

    __m256i s2 = std::get<0x2>(s);
    std::get<0x2>(s) = std::get<0x1>(s);
    std::get<0x1>(s) = std::get<0x8>(s);
    std::get<0x8>(s) = std::get<0x4>(s);
    std::get<0x4>(s) = s2;

    __m256i s6 = std::get<0x6>(s);
    std::get<0x6>(s) = std::get<0x3>(s);
    std::get<0x3>(s) = std::get<0x9>(s);
    std::get<0x9>(s) = std::get<0xC>(s);
    std::get<0xC>(s) = s6;

    __m256i sE = std::get<0xE>(s);
    std::get<0xE>(s) = std::get<0x7>(s);
    std::get<0x7>(s) = std::get<0xB>(s);
    std::get<0xB>(s) = std::get<0xD>(s);
    std::get<0xD>(s) = sE;

    __m256i s5 = std::get<0x5>(s);
    std::get<0x5>(s) = std::get<0xA>(s);
    std::get<0xA>(s) = s5;
}

template <std::size_t I0, std::size_t I1, std::size_t I2, std::size_t I3,
    std::size_t N>
MCKL_INLINE inline void transpose4x64_si256(std::array<__m256i, N> &s)
{
    __m256i t0 = _mm256_unpacklo_epi64(std::get<I0>(s), std::get<I1>(s));
    __m256i t1 = _mm256_unpacklo_epi64(std::get<I2>(s), std::get<I3>(s));
    __m256i t2 = _mm256_unpackhi_epi64(std::get<I0>(s), std::get<I1>(s));
    __m256i t3 = _mm256_unpackhi_epi64(std::get<I2>(s), std::get<I3>(s));
    std::get<I0>(s) = _mm256_permute2x128_si256(t0, t1, 0x20);
    std::get<I1>(s) = _mm256_permute2x128_si256(t2, t3, 0x20);
    std::get<I2>(s) = _mm256_permute2x128_si256(t0, t1, 0x31);
    std::get<I3>(s) = _mm256_permute2x128_si256(t2, t3, 0x31);
}

MCKL_INLINE inline void transpose4x64_load_si256(std::array<__m256i, 4> &s)
{
    transpose4x64_si256<0, 1, 2, 3>(s);
}

MCKL_INLINE inline void transpose4x64_store_si256(std::array<__m256i, 4> &s)
{
    transpose4x64_si256<0, 1, 2, 3>(s);
}

MCKL_INLINE inline void transpose4x64_load_si256(std::array<__m256i, 8> &s)
{
    __m256i s1 = std::get<1>(s);
    std::get<1>(s) = std::get<2>(s);
    std::get<2>(s) = std::get<4>(s);
    std::get<4>(s) = s1;

    __m256i s3 = std::get<3>(s);
    std::get<3>(s) = std::get<6>(s);
    std::get<6>(s) = std::get<5>(s);
    std::get<5>(s) = s3;

    transpose4x64_si256<0, 1, 2, 3>(s);
    transpose4x64_si256<4, 5, 6, 7>(s);
}

MCKL_INLINE inline void transpose4x64_store_si256(std::array<__m256i, 8> &s)
{
    transpose4x64_si256<0, 1, 2, 3>(s);
    transpose4x64_si256<4, 5, 6, 7>(s);

    __m256i s2 = std::get<2>(s);
    std::get<2>(s) = std::get<1>(s);
    std::get<1>(s) = std::get<4>(s);
    std::get<4>(s) = s2;

    __m256i s6 = std::get<6>(s);
    std::get<6>(s) = std::get<3>(s);
    std::get<3>(s) = std::get<5>(s);
    std::get<5>(s) = s6;
}

MCKL_INLINE inline void transpose4x64_load_si256(std::array<__m256i, 16> &s)
{
    __m256i s1 = std::get<0x1>(s);
    __m256i s2 = std::get<0x2>(s);
    __m256i s3 = std::get<0x3>(s);
    __m256i s6 = std::get<0x6>(s);
    __m256i s7 = std::get<0x7>(s);
    __m256i sB = std::get<0xB>(s);
    std::get<0x1>(s) = std::get<0x4>(s);
    std::get<0x2>(s) = std::get<0x8>(s);
    std::get<0x3>(s) = std::get<0xC>(s);
    std::get<0x6>(s) = std::get<0x9>(s);
    std::get<0x7>(s) = std::get<0xD>(s);
    std::get<0xB>(s) = std::get<0xE>(s);
    std::get<0x4>(s) = s1;
    std::get<0x8>(s) = s2;
    std::get<0xC>(s) = s3;
    std::get<0x9>(s) = s6;
    std::get<0xD>(s) = s7;
    std::get<0xE>(s) = sB;

    transpose4x64_si256<0x0, 0x1, 0x2, 0x3>(s);
    transpose4x64_si256<0x4, 0x5, 0x6, 0x7>(s);
    transpose4x64_si256<0x8, 0x9, 0xA, 0xB>(s);
    transpose4x64_si256<0xC, 0xD, 0xE, 0xF>(s);
}

MCKL_INLINE inline void transpose4x64_store_si256(std::array<__m256i, 16> &s)
{
    transpose4x64_si256<0x0, 0x1, 0x2, 0x3>(s);
    transpose4x64_si256<0x4, 0x5, 0x6, 0x7>(s);
    transpose4x64_si256<0x8, 0x9, 0xA, 0xB>(s);
    transpose4x64_si256<0xC, 0xD, 0xE, 0xF>(s);

    __m256i s1 = std::get<0x1>(s);
    __m256i s2 = std::get<0x2>(s);
    __m256i s3 = std::get<0x3>(s);
    __m256i s6 = std::get<0x6>(s);
    __m256i s7 = std::get<0x7>(s);
    __m256i sB = std::get<0xB>(s);
    std::get<0x1>(s) = std::get<0x4>(s);
    std::get<0x2>(s) = std::get<0x8>(s);
    std::get<0x3>(s) = std::get<0xC>(s);
    std::get<0x6>(s) = std::get<0x9>(s);
    std::get<0x7>(s) = std::get<0xD>(s);
    std::get<0xB>(s) = std::get<0xE>(s);
    std::get<0x4>(s) = s1;
    std::get<0x8>(s) = s2;
    std::get<0xC>(s) = s3;
    std::get<0x9>(s) = s6;
    std::get<0xD>(s) = s7;
    std::get<0xE>(s) = sB;
}

} // namespace mckl::internal

} // namespace mckl

#ifdef MCKL_GCC
#if MCKL_GCC_VERSION >= 60000
#pragma GCC diagnostic pop
#endif
#endif

#endif // MCKL_INTERNAL_AVX2_HPP
