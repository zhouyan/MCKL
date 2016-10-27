//============================================================================
// MCKL/include/mckl/internal/avx2.hpp
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

#ifndef MCKL_INTERNAL_AVX2_HPP
#define MCKL_INTERNAL_AVX2_HPP

#include <mckl/internal/config.h>
#include <array>

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

template <std::size_t I0, std::size_t I1, std::size_t I2, std::size_t I3,
    std::size_t I4, std::size_t I5, std::size_t I6, std::size_t I7,
    std::size_t N>
inline void transpose8x32_si256(std::array<__m256i, N> &s)
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

inline void transpose8x32_load_si256(std::array<__m256i, 8> &s)
{
    transpose8x32_si256<0, 1, 2, 3, 4, 5, 6, 7>(s);
}

inline void transpose8x32_store_si256(std::array<__m256i, 8> &s)
{
    transpose8x32_si256<0, 1, 2, 3, 4, 5, 6, 7>(s);
}

inline void transpose8x32_load_si256(std::array<__m256i, 16> &s)
{
    __m256i t0 = std::get<0x1>(s);
    std::get<0x1>(s) = std::get<0x2>(s);
    std::get<0x2>(s) = std::get<0x4>(s);
    std::get<0x4>(s) = std::get<0x8>(s);
    std::get<0x8>(s) = t0;

    __m256i t1 = std::get<0x3>(s);
    std::get<0x3>(s) = std::get<0x6>(s);
    std::get<0x6>(s) = std::get<0xC>(s);
    std::get<0xC>(s) = std::get<0x9>(s);
    std::get<0x9>(s) = t1;

    __m256i t2 = std::get<0x7>(s);
    std::get<0x7>(s) = std::get<0xE>(s);
    std::get<0xE>(s) = std::get<0xD>(s);
    std::get<0xD>(s) = std::get<0xB>(s);
    std::get<0xB>(s) = t2;

    __m256i t3 = std::get<0x5>(s);
    std::get<0x5>(s) = std::get<0xA>(s);
    std::get<0xA>(s) = t3;

    transpose8x32_si256<0x0, 0x1, 0x2, 0x3, 0x4, 0x5, 0x6, 0x7>(s);
    transpose8x32_si256<0x8, 0x9, 0xA, 0xB, 0xC, 0xD, 0xE, 0xF>(s);
}

inline void transpose8x32_store_si256(std::array<__m256i, 16> &s)
{
    transpose8x32_si256<0x0, 0x1, 0x2, 0x3, 0x4, 0x5, 0x6, 0x7>(s);
    transpose8x32_si256<0x8, 0x9, 0xA, 0xB, 0xC, 0xD, 0xE, 0xF>(s);

    __m256i t0 = std::get<0x2>(s);
    std::get<0x2>(s) = std::get<0x1>(s);
    std::get<0x1>(s) = std::get<0x8>(s);
    std::get<0x8>(s) = std::get<0x4>(s);
    std::get<0x4>(s) = t0;

    __m256i t1 = std::get<0x6>(s);
    std::get<0x6>(s) = std::get<0x3>(s);
    std::get<0x3>(s) = std::get<0x9>(s);
    std::get<0x9>(s) = std::get<0xC>(s);
    std::get<0xC>(s) = t1;

    __m256i t2 = std::get<0xE>(s);
    std::get<0xE>(s) = std::get<0x7>(s);
    std::get<0x7>(s) = std::get<0xB>(s);
    std::get<0xB>(s) = std::get<0xD>(s);
    std::get<0xD>(s) = t2;

    __m256i t3 = std::get<0x5>(s);
    std::get<0x5>(s) = std::get<0xA>(s);
    std::get<0xA>(s) = t3;
}

template <std::size_t I0, std::size_t I1, std::size_t I2, std::size_t I3,
    std::size_t N>
inline void transpose4x64_si256(std::array<__m256i, N> &s)
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

inline void transpose4x64_load_si256(std::array<__m256i, 4> &s)
{
    transpose4x64_si256<0, 1, 2, 3>(s);
}

inline void transpose4x64_store_si256(std::array<__m256i, 4> &s)
{
    transpose4x64_si256<0, 1, 2, 3>(s);
}

inline void transpose4x64_load_si256(std::array<__m256i, 8> &s)
{
    __m256i t0 = std::get<1>(s);
    std::get<1>(s) = std::get<2>(s);
    std::get<2>(s) = std::get<4>(s);
    std::get<4>(s) = t0;

    __m256i t1 = std::get<3>(s);
    std::get<3>(s) = std::get<6>(s);
    std::get<6>(s) = std::get<5>(s);
    std::get<5>(s) = t1;

    transpose4x64_si256<0, 1, 2, 3>(s);
    transpose4x64_si256<4, 5, 6, 7>(s);
}

inline void transpose4x64_store_si256(std::array<__m256i, 8> &s)
{
    transpose4x64_si256<0, 1, 2, 3>(s);
    transpose4x64_si256<4, 5, 6, 7>(s);

    __m256i t0 = std::get<2>(s);
    std::get<2>(s) = std::get<1>(s);
    std::get<1>(s) = std::get<4>(s);
    std::get<4>(s) = t0;

    __m256i t1 = std::get<6>(s);
    std::get<6>(s) = std::get<3>(s);
    std::get<3>(s) = std::get<5>(s);
    std::get<5>(s) = t1;
}

inline void transpose4x64_load_si256(std::array<__m256i, 16> &s)
{
    std::swap(std::get<0x1>(s), std::get<0x4>(s));
    std::swap(std::get<0x2>(s), std::get<0x8>(s));
    std::swap(std::get<0x3>(s), std::get<0xC>(s));
    std::swap(std::get<0x6>(s), std::get<0x9>(s));
    std::swap(std::get<0x7>(s), std::get<0xD>(s));
    std::swap(std::get<0xB>(s), std::get<0xE>(s));

    transpose4x64_si256<0x0, 0x1, 0x2, 0x3>(s);
    transpose4x64_si256<0x4, 0x5, 0x6, 0x7>(s);
    transpose4x64_si256<0x8, 0x9, 0xA, 0xB>(s);
    transpose4x64_si256<0xC, 0xD, 0xE, 0xF>(s);
}

inline void transpose4x64_store_si256(std::array<__m256i, 16> &s)
{
    transpose4x64_si256<0x0, 0x1, 0x2, 0x3>(s);
    transpose4x64_si256<0x4, 0x5, 0x6, 0x7>(s);
    transpose4x64_si256<0x8, 0x9, 0xA, 0xB>(s);
    transpose4x64_si256<0xC, 0xD, 0xE, 0xF>(s);

    std::swap(std::get<0x1>(s), std::get<0x4>(s));
    std::swap(std::get<0x2>(s), std::get<0x8>(s));
    std::swap(std::get<0x3>(s), std::get<0xC>(s));
    std::swap(std::get<0x6>(s), std::get<0x9>(s));
    std::swap(std::get<0x7>(s), std::get<0xD>(s));
    std::swap(std::get<0xB>(s), std::get<0xE>(s));
}

inline void add_256d(std::array<__m256d, 8> &s, const __m256d &a)
{
    std::get<0>(s) = _mm256_add_pd(std::get<0>(s), a);
    std::get<1>(s) = _mm256_add_pd(std::get<1>(s), a);
    std::get<2>(s) = _mm256_add_pd(std::get<2>(s), a);
    std::get<3>(s) = _mm256_add_pd(std::get<3>(s), a);
    std::get<4>(s) = _mm256_add_pd(std::get<4>(s), a);
    std::get<5>(s) = _mm256_add_pd(std::get<5>(s), a);
    std::get<6>(s) = _mm256_add_pd(std::get<6>(s), a);
    std::get<7>(s) = _mm256_add_pd(std::get<7>(s), a);
}

inline void add_256d(
    std::array<__m256d, 8> &s, const std::array<__m256d, 8> &a)
{
    std::get<0>(s) = _mm256_add_pd(std::get<0>(s), std::get<0>(a));
    std::get<1>(s) = _mm256_add_pd(std::get<1>(s), std::get<1>(a));
    std::get<2>(s) = _mm256_add_pd(std::get<2>(s), std::get<2>(a));
    std::get<3>(s) = _mm256_add_pd(std::get<3>(s), std::get<3>(a));
    std::get<4>(s) = _mm256_add_pd(std::get<4>(s), std::get<4>(a));
    std::get<5>(s) = _mm256_add_pd(std::get<5>(s), std::get<5>(a));
    std::get<6>(s) = _mm256_add_pd(std::get<6>(s), std::get<6>(a));
    std::get<7>(s) = _mm256_add_pd(std::get<7>(s), std::get<7>(a));
}

inline void add_256d(std::array<__m256d, 16> &s, const __m256d &a)
{
    std::get<0x0>(s) = _mm256_add_pd(std::get<0x0>(s), a);
    std::get<0x1>(s) = _mm256_add_pd(std::get<0x1>(s), a);
    std::get<0x2>(s) = _mm256_add_pd(std::get<0x2>(s), a);
    std::get<0x3>(s) = _mm256_add_pd(std::get<0x3>(s), a);
    std::get<0x4>(s) = _mm256_add_pd(std::get<0x4>(s), a);
    std::get<0x5>(s) = _mm256_add_pd(std::get<0x5>(s), a);
    std::get<0x6>(s) = _mm256_add_pd(std::get<0x6>(s), a);
    std::get<0x7>(s) = _mm256_add_pd(std::get<0x7>(s), a);
    std::get<0x8>(s) = _mm256_add_pd(std::get<0x8>(s), a);
    std::get<0x9>(s) = _mm256_add_pd(std::get<0x9>(s), a);
    std::get<0xA>(s) = _mm256_add_pd(std::get<0xA>(s), a);
    std::get<0xB>(s) = _mm256_add_pd(std::get<0xB>(s), a);
    std::get<0xC>(s) = _mm256_add_pd(std::get<0xC>(s), a);
    std::get<0xD>(s) = _mm256_add_pd(std::get<0xD>(s), a);
    std::get<0xE>(s) = _mm256_add_pd(std::get<0xE>(s), a);
    std::get<0xF>(s) = _mm256_add_pd(std::get<0xF>(s), a);
}

inline void add_256d(
    std::array<__m256d, 16> &s, const std::array<__m256d, 16> &a)
{
    std::get<0x0>(s) = _mm256_add_pd(std::get<0x0>(s), std::get<0x0>(a));
    std::get<0x1>(s) = _mm256_add_pd(std::get<0x1>(s), std::get<0x1>(a));
    std::get<0x2>(s) = _mm256_add_pd(std::get<0x2>(s), std::get<0x2>(a));
    std::get<0x3>(s) = _mm256_add_pd(std::get<0x3>(s), std::get<0x3>(a));
    std::get<0x4>(s) = _mm256_add_pd(std::get<0x4>(s), std::get<0x4>(a));
    std::get<0x5>(s) = _mm256_add_pd(std::get<0x5>(s), std::get<0x5>(a));
    std::get<0x6>(s) = _mm256_add_pd(std::get<0x6>(s), std::get<0x6>(a));
    std::get<0x7>(s) = _mm256_add_pd(std::get<0x7>(s), std::get<0x7>(a));
    std::get<0x8>(s) = _mm256_add_pd(std::get<0x8>(s), std::get<0x8>(a));
    std::get<0x9>(s) = _mm256_add_pd(std::get<0x9>(s), std::get<0x9>(a));
    std::get<0xA>(s) = _mm256_add_pd(std::get<0xA>(s), std::get<0xA>(a));
    std::get<0xB>(s) = _mm256_add_pd(std::get<0xB>(s), std::get<0xB>(a));
    std::get<0xC>(s) = _mm256_add_pd(std::get<0xC>(s), std::get<0xC>(a));
    std::get<0xD>(s) = _mm256_add_pd(std::get<0xD>(s), std::get<0xD>(a));
    std::get<0xE>(s) = _mm256_add_pd(std::get<0xE>(s), std::get<0xE>(a));
    std::get<0xF>(s) = _mm256_add_pd(std::get<0xF>(s), std::get<0xF>(a));
}

inline void mul_256d(std::array<__m256d, 8> &s, const __m256d &a)
{
    std::get<0>(s) = _mm256_mul_pd(std::get<0>(s), a);
    std::get<1>(s) = _mm256_mul_pd(std::get<1>(s), a);
    std::get<2>(s) = _mm256_mul_pd(std::get<2>(s), a);
    std::get<3>(s) = _mm256_mul_pd(std::get<3>(s), a);
    std::get<4>(s) = _mm256_mul_pd(std::get<4>(s), a);
    std::get<5>(s) = _mm256_mul_pd(std::get<5>(s), a);
    std::get<6>(s) = _mm256_mul_pd(std::get<6>(s), a);
    std::get<7>(s) = _mm256_mul_pd(std::get<7>(s), a);
}

inline void mul_256d(
    std::array<__m256d, 8> &s, const std::array<__m256d, 8> &a)
{
    std::get<0>(s) = _mm256_mul_pd(std::get<0>(s), std::get<0>(a));
    std::get<1>(s) = _mm256_mul_pd(std::get<1>(s), std::get<1>(a));
    std::get<2>(s) = _mm256_mul_pd(std::get<2>(s), std::get<2>(a));
    std::get<3>(s) = _mm256_mul_pd(std::get<3>(s), std::get<3>(a));
    std::get<4>(s) = _mm256_mul_pd(std::get<4>(s), std::get<4>(a));
    std::get<5>(s) = _mm256_mul_pd(std::get<5>(s), std::get<5>(a));
    std::get<6>(s) = _mm256_mul_pd(std::get<6>(s), std::get<6>(a));
    std::get<7>(s) = _mm256_mul_pd(std::get<7>(s), std::get<7>(a));
}

inline void mul_256d(std::array<__m256d, 16> &s, const __m256d &a)
{
    std::get<0x0>(s) = _mm256_mul_pd(std::get<0x0>(s), a);
    std::get<0x1>(s) = _mm256_mul_pd(std::get<0x1>(s), a);
    std::get<0x2>(s) = _mm256_mul_pd(std::get<0x2>(s), a);
    std::get<0x3>(s) = _mm256_mul_pd(std::get<0x3>(s), a);
    std::get<0x4>(s) = _mm256_mul_pd(std::get<0x4>(s), a);
    std::get<0x5>(s) = _mm256_mul_pd(std::get<0x5>(s), a);
    std::get<0x6>(s) = _mm256_mul_pd(std::get<0x6>(s), a);
    std::get<0x7>(s) = _mm256_mul_pd(std::get<0x7>(s), a);
    std::get<0x8>(s) = _mm256_mul_pd(std::get<0x8>(s), a);
    std::get<0x9>(s) = _mm256_mul_pd(std::get<0x9>(s), a);
    std::get<0xA>(s) = _mm256_mul_pd(std::get<0xA>(s), a);
    std::get<0xB>(s) = _mm256_mul_pd(std::get<0xB>(s), a);
    std::get<0xC>(s) = _mm256_mul_pd(std::get<0xC>(s), a);
    std::get<0xD>(s) = _mm256_mul_pd(std::get<0xD>(s), a);
    std::get<0xE>(s) = _mm256_mul_pd(std::get<0xE>(s), a);
    std::get<0xF>(s) = _mm256_mul_pd(std::get<0xF>(s), a);
}

inline void mul_256d(
    std::array<__m256d, 16> &s, const std::array<__m256d, 16> &a)
{
    std::get<0x0>(s) = _mm256_mul_pd(std::get<0x0>(s), std::get<0x0>(a));
    std::get<0x1>(s) = _mm256_mul_pd(std::get<0x1>(s), std::get<0x1>(a));
    std::get<0x2>(s) = _mm256_mul_pd(std::get<0x2>(s), std::get<0x2>(a));
    std::get<0x3>(s) = _mm256_mul_pd(std::get<0x3>(s), std::get<0x3>(a));
    std::get<0x4>(s) = _mm256_mul_pd(std::get<0x4>(s), std::get<0x4>(a));
    std::get<0x5>(s) = _mm256_mul_pd(std::get<0x5>(s), std::get<0x5>(a));
    std::get<0x6>(s) = _mm256_mul_pd(std::get<0x6>(s), std::get<0x6>(a));
    std::get<0x7>(s) = _mm256_mul_pd(std::get<0x7>(s), std::get<0x7>(a));
    std::get<0x8>(s) = _mm256_mul_pd(std::get<0x8>(s), std::get<0x8>(a));
    std::get<0x9>(s) = _mm256_mul_pd(std::get<0x9>(s), std::get<0x9>(a));
    std::get<0xA>(s) = _mm256_mul_pd(std::get<0xA>(s), std::get<0xA>(a));
    std::get<0xB>(s) = _mm256_mul_pd(std::get<0xB>(s), std::get<0xB>(a));
    std::get<0xC>(s) = _mm256_mul_pd(std::get<0xC>(s), std::get<0xC>(a));
    std::get<0xD>(s) = _mm256_mul_pd(std::get<0xD>(s), std::get<0xD>(a));
    std::get<0xE>(s) = _mm256_mul_pd(std::get<0xE>(s), std::get<0xE>(a));
    std::get<0xF>(s) = _mm256_mul_pd(std::get<0xF>(s), std::get<0xF>(a));
}

#if MCKL_USE_FMA

inline void fma_256d(
    std::array<__m256d, 8> &s, const __m256d &a, const __m256d &b)
{
    std::get<0>(s) = _mm256_fmadd_pd(std::get<0>(s), a, b);
    std::get<1>(s) = _mm256_fmadd_pd(std::get<1>(s), a, b);
    std::get<2>(s) = _mm256_fmadd_pd(std::get<2>(s), a, b);
    std::get<3>(s) = _mm256_fmadd_pd(std::get<3>(s), a, b);
    std::get<4>(s) = _mm256_fmadd_pd(std::get<4>(s), a, b);
    std::get<5>(s) = _mm256_fmadd_pd(std::get<5>(s), a, b);
    std::get<6>(s) = _mm256_fmadd_pd(std::get<6>(s), a, b);
    std::get<7>(s) = _mm256_fmadd_pd(std::get<7>(s), a, b);
}

inline void fma_256d(std::array<__m256d, 8> &s,
    const std::array<__m256d, 8> &a, const std::array<__m256d, 8> &b)
{
    std::get<0>(s) =
        _mm256_fmadd_pd(std::get<0>(s), std::get<0>(a), std::get<0>(b));
    std::get<1>(s) =
        _mm256_fmadd_pd(std::get<1>(s), std::get<1>(a), std::get<1>(b));
    std::get<2>(s) =
        _mm256_fmadd_pd(std::get<2>(s), std::get<2>(a), std::get<2>(b));
    std::get<3>(s) =
        _mm256_fmadd_pd(std::get<3>(s), std::get<3>(a), std::get<3>(b));
    std::get<4>(s) =
        _mm256_fmadd_pd(std::get<4>(s), std::get<4>(a), std::get<4>(b));
    std::get<5>(s) =
        _mm256_fmadd_pd(std::get<5>(s), std::get<5>(a), std::get<5>(b));
    std::get<6>(s) =
        _mm256_fmadd_pd(std::get<6>(s), std::get<6>(a), std::get<6>(b));
    std::get<7>(s) =
        _mm256_fmadd_pd(std::get<7>(s), std::get<7>(a), std::get<7>(b));
}

inline void fma_256d(
    std::array<__m256d, 16> &s, const __m256d &a, const __m256d &b)
{
    std::get<0x0>(s) = _mm256_fmadd_pd(std::get<0x0>(s), a, b);
    std::get<0x1>(s) = _mm256_fmadd_pd(std::get<0x1>(s), a, b);
    std::get<0x2>(s) = _mm256_fmadd_pd(std::get<0x2>(s), a, b);
    std::get<0x3>(s) = _mm256_fmadd_pd(std::get<0x3>(s), a, b);
    std::get<0x4>(s) = _mm256_fmadd_pd(std::get<0x4>(s), a, b);
    std::get<0x5>(s) = _mm256_fmadd_pd(std::get<0x5>(s), a, b);
    std::get<0x6>(s) = _mm256_fmadd_pd(std::get<0x6>(s), a, b);
    std::get<0x7>(s) = _mm256_fmadd_pd(std::get<0x7>(s), a, b);
    std::get<0x8>(s) = _mm256_fmadd_pd(std::get<0x8>(s), a, b);
    std::get<0x9>(s) = _mm256_fmadd_pd(std::get<0x9>(s), a, b);
    std::get<0xA>(s) = _mm256_fmadd_pd(std::get<0xA>(s), a, b);
    std::get<0xB>(s) = _mm256_fmadd_pd(std::get<0xB>(s), a, b);
    std::get<0xC>(s) = _mm256_fmadd_pd(std::get<0xC>(s), a, b);
    std::get<0xD>(s) = _mm256_fmadd_pd(std::get<0xD>(s), a, b);
    std::get<0xE>(s) = _mm256_fmadd_pd(std::get<0xE>(s), a, b);
    std::get<0xF>(s) = _mm256_fmadd_pd(std::get<0xF>(s), a, b);
}

inline void fma_256d(std::array<__m256d, 16> &s,
    const std::array<__m256d, 16> &a, const std::array<__m256d, 16> &b)
{
    std::get<0x0>(s) =
        _mm256_fmadd_pd(std::get<0x0>(s), std::get<0x0>(a), std::get<0x0>(b));
    std::get<0x1>(s) =
        _mm256_fmadd_pd(std::get<0x1>(s), std::get<0x1>(a), std::get<0x1>(b));
    std::get<0x2>(s) =
        _mm256_fmadd_pd(std::get<0x2>(s), std::get<0x2>(a), std::get<0x2>(b));
    std::get<0x3>(s) =
        _mm256_fmadd_pd(std::get<0x3>(s), std::get<0x3>(a), std::get<0x3>(b));
    std::get<0x4>(s) =
        _mm256_fmadd_pd(std::get<0x4>(s), std::get<0x4>(a), std::get<0x4>(b));
    std::get<0x5>(s) =
        _mm256_fmadd_pd(std::get<0x5>(s), std::get<0x5>(a), std::get<0x5>(b));
    std::get<0x6>(s) =
        _mm256_fmadd_pd(std::get<0x6>(s), std::get<0x6>(a), std::get<0x6>(b));
    std::get<0x7>(s) =
        _mm256_fmadd_pd(std::get<0x7>(s), std::get<0x7>(a), std::get<0x7>(b));
    std::get<0x8>(s) =
        _mm256_fmadd_pd(std::get<0x8>(s), std::get<0x8>(a), std::get<0x8>(b));
    std::get<0x9>(s) =
        _mm256_fmadd_pd(std::get<0x9>(s), std::get<0x9>(a), std::get<0x9>(b));
    std::get<0xA>(s) =
        _mm256_fmadd_pd(std::get<0xA>(s), std::get<0xA>(a), std::get<0xA>(b));
    std::get<0xB>(s) =
        _mm256_fmadd_pd(std::get<0xB>(s), std::get<0xB>(a), std::get<0xB>(b));
    std::get<0xC>(s) =
        _mm256_fmadd_pd(std::get<0xC>(s), std::get<0xC>(a), std::get<0xC>(b));
    std::get<0xD>(s) =
        _mm256_fmadd_pd(std::get<0xD>(s), std::get<0xD>(a), std::get<0xD>(b));
    std::get<0xE>(s) =
        _mm256_fmadd_pd(std::get<0xE>(s), std::get<0xE>(a), std::get<0xE>(b));
    std::get<0xF>(s) =
        _mm256_fmadd_pd(std::get<0xF>(s), std::get<0xF>(a), std::get<0xF>(b));
}

#else // MCKL_USE_FMA

template <std::size_t S>
inline void fma_256d(
    std::array<__m256d, S> &s, const __m256d &a, const __m256d &b)
{
    mul_256d(s, a);
    add_256d(s, b);
}

#endif // MCKL_USE_FMA

template <std::size_t S>
inline void fma_256d(std::array<__m256d, S> &s, double a, double b)
{
    if (!(a < 1 || a > 1)) {
        add_256d(s, _mm256_set1_pd(b));
        return;
    }

    if (!(b < 0 || b > 0)) {
        mul_256d(s, _mm256_set1_pd(a));
        return;
    }

    fma_256d(s, _mm256_set1_pd(a), _mm256_set1_pd(b));
}

inline void convert_u32_256d(
    const std::array<__m128i, 8> &s, std::array<__m256d, 8> &t)
{
    const __m256i p = _mm256_set_epi32(3, 7, 2, 6, 1, 5, 0, 4);
    const __m256d c = _mm256_castsi256_pd( // 2^52 + 2^51
        _mm256_set1_epi64x(static_cast<MCKL_INT64>(0x4338000000000000)));

    std::get<0>(t) = _mm256_castsi256_pd(_mm256_permutevar8x32_epi32(
        _mm256_castsi128_si256(std::get<0>(s)), p));
    std::get<1>(t) = _mm256_castsi256_pd(_mm256_permutevar8x32_epi32(
        _mm256_castsi128_si256(std::get<1>(s)), p));
    std::get<2>(t) = _mm256_castsi256_pd(_mm256_permutevar8x32_epi32(
        _mm256_castsi128_si256(std::get<2>(s)), p));
    std::get<3>(t) = _mm256_castsi256_pd(_mm256_permutevar8x32_epi32(
        _mm256_castsi128_si256(std::get<3>(s)), p));
    std::get<4>(t) = _mm256_castsi256_pd(_mm256_permutevar8x32_epi32(
        _mm256_castsi128_si256(std::get<4>(s)), p));
    std::get<5>(t) = _mm256_castsi256_pd(_mm256_permutevar8x32_epi32(
        _mm256_castsi128_si256(std::get<5>(s)), p));
    std::get<6>(t) = _mm256_castsi256_pd(_mm256_permutevar8x32_epi32(
        _mm256_castsi128_si256(std::get<6>(s)), p));
    std::get<7>(t) = _mm256_castsi256_pd(_mm256_permutevar8x32_epi32(
        _mm256_castsi128_si256(std::get<7>(s)), p));

    std::get<0>(t) = _mm256_castsi256_pd(
        _mm256_srli_epi64(_mm256_castpd_si256(std::get<0>(t)), 32));
    std::get<1>(t) = _mm256_castsi256_pd(
        _mm256_srli_epi64(_mm256_castpd_si256(std::get<1>(t)), 32));
    std::get<2>(t) = _mm256_castsi256_pd(
        _mm256_srli_epi64(_mm256_castpd_si256(std::get<2>(t)), 32));
    std::get<3>(t) = _mm256_castsi256_pd(
        _mm256_srli_epi64(_mm256_castpd_si256(std::get<3>(t)), 32));
    std::get<4>(t) = _mm256_castsi256_pd(
        _mm256_srli_epi64(_mm256_castpd_si256(std::get<4>(t)), 32));
    std::get<5>(t) = _mm256_castsi256_pd(
        _mm256_srli_epi64(_mm256_castpd_si256(std::get<5>(t)), 32));
    std::get<6>(t) = _mm256_castsi256_pd(
        _mm256_srli_epi64(_mm256_castpd_si256(std::get<6>(t)), 32));
    std::get<7>(t) = _mm256_castsi256_pd(
        _mm256_srli_epi64(_mm256_castpd_si256(std::get<7>(t)), 32));

    std::get<0>(t) = _mm256_castsi256_pd(_mm256_add_epi64(
        _mm256_castpd_si256(std::get<0>(t)), _mm256_castpd_si256(c)));
    std::get<1>(t) = _mm256_castsi256_pd(_mm256_add_epi64(
        _mm256_castpd_si256(std::get<1>(t)), _mm256_castpd_si256(c)));
    std::get<2>(t) = _mm256_castsi256_pd(_mm256_add_epi64(
        _mm256_castpd_si256(std::get<2>(t)), _mm256_castpd_si256(c)));
    std::get<3>(t) = _mm256_castsi256_pd(_mm256_add_epi64(
        _mm256_castpd_si256(std::get<3>(t)), _mm256_castpd_si256(c)));
    std::get<4>(t) = _mm256_castsi256_pd(_mm256_add_epi64(
        _mm256_castpd_si256(std::get<4>(t)), _mm256_castpd_si256(c)));
    std::get<5>(t) = _mm256_castsi256_pd(_mm256_add_epi64(
        _mm256_castpd_si256(std::get<5>(t)), _mm256_castpd_si256(c)));
    std::get<6>(t) = _mm256_castsi256_pd(_mm256_add_epi64(
        _mm256_castpd_si256(std::get<6>(t)), _mm256_castpd_si256(c)));
    std::get<7>(t) = _mm256_castsi256_pd(_mm256_add_epi64(
        _mm256_castpd_si256(std::get<7>(t)), _mm256_castpd_si256(c)));

    std::get<0>(t) = _mm256_sub_pd(std::get<0>(t), c);
    std::get<1>(t) = _mm256_sub_pd(std::get<1>(t), c);
    std::get<2>(t) = _mm256_sub_pd(std::get<2>(t), c);
    std::get<3>(t) = _mm256_sub_pd(std::get<3>(t), c);
    std::get<4>(t) = _mm256_sub_pd(std::get<4>(t), c);
    std::get<5>(t) = _mm256_sub_pd(std::get<5>(t), c);
    std::get<6>(t) = _mm256_sub_pd(std::get<6>(t), c);
    std::get<7>(t) = _mm256_sub_pd(std::get<7>(t), c);
}

inline void convert_u32_256d(
    const std::array<__m128i, 16> &s, std::array<__m256d, 16> &t)
{
    const __m256i p = _mm256_set_epi32(0x3, 0x7, 0x2, 0x6, 0x1, 0x5, 0x0, 0x4);
    const __m256d c = _mm256_castsi256_pd( // 2^52 + 2^51
        _mm256_set1_epi64x(static_cast<MCKL_INT64>(0x4338000000000000)));

    std::get<0x0>(t) = _mm256_castsi256_pd(_mm256_permutevar8x32_epi32(
        _mm256_castsi128_si256(std::get<0x0>(s)), p));
    std::get<0x1>(t) = _mm256_castsi256_pd(_mm256_permutevar8x32_epi32(
        _mm256_castsi128_si256(std::get<0x1>(s)), p));
    std::get<0x2>(t) = _mm256_castsi256_pd(_mm256_permutevar8x32_epi32(
        _mm256_castsi128_si256(std::get<0x2>(s)), p));
    std::get<0x3>(t) = _mm256_castsi256_pd(_mm256_permutevar8x32_epi32(
        _mm256_castsi128_si256(std::get<0x3>(s)), p));
    std::get<0x4>(t) = _mm256_castsi256_pd(_mm256_permutevar8x32_epi32(
        _mm256_castsi128_si256(std::get<0x4>(s)), p));
    std::get<0x5>(t) = _mm256_castsi256_pd(_mm256_permutevar8x32_epi32(
        _mm256_castsi128_si256(std::get<0x5>(s)), p));
    std::get<0x6>(t) = _mm256_castsi256_pd(_mm256_permutevar8x32_epi32(
        _mm256_castsi128_si256(std::get<0x6>(s)), p));
    std::get<0x7>(t) = _mm256_castsi256_pd(_mm256_permutevar8x32_epi32(
        _mm256_castsi128_si256(std::get<0x7>(s)), p));
    std::get<0x8>(t) = _mm256_castsi256_pd(_mm256_permutevar8x32_epi32(
        _mm256_castsi128_si256(std::get<0x8>(s)), p));
    std::get<0x9>(t) = _mm256_castsi256_pd(_mm256_permutevar8x32_epi32(
        _mm256_castsi128_si256(std::get<0x9>(s)), p));
    std::get<0xA>(t) = _mm256_castsi256_pd(_mm256_permutevar8x32_epi32(
        _mm256_castsi128_si256(std::get<0xA>(s)), p));
    std::get<0xB>(t) = _mm256_castsi256_pd(_mm256_permutevar8x32_epi32(
        _mm256_castsi128_si256(std::get<0xB>(s)), p));
    std::get<0xC>(t) = _mm256_castsi256_pd(_mm256_permutevar8x32_epi32(
        _mm256_castsi128_si256(std::get<0xC>(s)), p));
    std::get<0xD>(t) = _mm256_castsi256_pd(_mm256_permutevar8x32_epi32(
        _mm256_castsi128_si256(std::get<0xD>(s)), p));
    std::get<0xE>(t) = _mm256_castsi256_pd(_mm256_permutevar8x32_epi32(
        _mm256_castsi128_si256(std::get<0xE>(s)), p));
    std::get<0xF>(t) = _mm256_castsi256_pd(_mm256_permutevar8x32_epi32(
        _mm256_castsi128_si256(std::get<0xF>(s)), p));

    std::get<0x0>(t) = _mm256_castsi256_pd(
        _mm256_srli_epi64(_mm256_castpd_si256(std::get<0x0>(t)), 32));
    std::get<0x1>(t) = _mm256_castsi256_pd(
        _mm256_srli_epi64(_mm256_castpd_si256(std::get<0x1>(t)), 32));
    std::get<0x2>(t) = _mm256_castsi256_pd(
        _mm256_srli_epi64(_mm256_castpd_si256(std::get<0x2>(t)), 32));
    std::get<0x3>(t) = _mm256_castsi256_pd(
        _mm256_srli_epi64(_mm256_castpd_si256(std::get<0x3>(t)), 32));
    std::get<0x4>(t) = _mm256_castsi256_pd(
        _mm256_srli_epi64(_mm256_castpd_si256(std::get<0x4>(t)), 32));
    std::get<0x5>(t) = _mm256_castsi256_pd(
        _mm256_srli_epi64(_mm256_castpd_si256(std::get<0x5>(t)), 32));
    std::get<0x6>(t) = _mm256_castsi256_pd(
        _mm256_srli_epi64(_mm256_castpd_si256(std::get<0x6>(t)), 32));
    std::get<0x7>(t) = _mm256_castsi256_pd(
        _mm256_srli_epi64(_mm256_castpd_si256(std::get<0x7>(t)), 32));
    std::get<0x8>(t) = _mm256_castsi256_pd(
        _mm256_srli_epi64(_mm256_castpd_si256(std::get<0x8>(t)), 32));
    std::get<0x9>(t) = _mm256_castsi256_pd(
        _mm256_srli_epi64(_mm256_castpd_si256(std::get<0x9>(t)), 32));
    std::get<0xA>(t) = _mm256_castsi256_pd(
        _mm256_srli_epi64(_mm256_castpd_si256(std::get<0xA>(t)), 32));
    std::get<0xB>(t) = _mm256_castsi256_pd(
        _mm256_srli_epi64(_mm256_castpd_si256(std::get<0xB>(t)), 32));
    std::get<0xC>(t) = _mm256_castsi256_pd(
        _mm256_srli_epi64(_mm256_castpd_si256(std::get<0xC>(t)), 32));
    std::get<0xD>(t) = _mm256_castsi256_pd(
        _mm256_srli_epi64(_mm256_castpd_si256(std::get<0xD>(t)), 32));
    std::get<0xE>(t) = _mm256_castsi256_pd(
        _mm256_srli_epi64(_mm256_castpd_si256(std::get<0xE>(t)), 32));
    std::get<0xF>(t) = _mm256_castsi256_pd(
        _mm256_srli_epi64(_mm256_castpd_si256(std::get<0xF>(t)), 32));

    std::get<0x0>(t) = _mm256_castsi256_pd(_mm256_add_epi64(
        _mm256_castpd_si256(std::get<0x0>(t)), _mm256_castpd_si256(c)));
    std::get<0x1>(t) = _mm256_castsi256_pd(_mm256_add_epi64(
        _mm256_castpd_si256(std::get<0x1>(t)), _mm256_castpd_si256(c)));
    std::get<0x2>(t) = _mm256_castsi256_pd(_mm256_add_epi64(
        _mm256_castpd_si256(std::get<0x2>(t)), _mm256_castpd_si256(c)));
    std::get<0x3>(t) = _mm256_castsi256_pd(_mm256_add_epi64(
        _mm256_castpd_si256(std::get<0x3>(t)), _mm256_castpd_si256(c)));
    std::get<0x4>(t) = _mm256_castsi256_pd(_mm256_add_epi64(
        _mm256_castpd_si256(std::get<0x4>(t)), _mm256_castpd_si256(c)));
    std::get<0x5>(t) = _mm256_castsi256_pd(_mm256_add_epi64(
        _mm256_castpd_si256(std::get<0x5>(t)), _mm256_castpd_si256(c)));
    std::get<0x6>(t) = _mm256_castsi256_pd(_mm256_add_epi64(
        _mm256_castpd_si256(std::get<0x6>(t)), _mm256_castpd_si256(c)));
    std::get<0x7>(t) = _mm256_castsi256_pd(_mm256_add_epi64(
        _mm256_castpd_si256(std::get<0x7>(t)), _mm256_castpd_si256(c)));
    std::get<0x8>(t) = _mm256_castsi256_pd(_mm256_add_epi64(
        _mm256_castpd_si256(std::get<0x8>(t)), _mm256_castpd_si256(c)));
    std::get<0x9>(t) = _mm256_castsi256_pd(_mm256_add_epi64(
        _mm256_castpd_si256(std::get<0x9>(t)), _mm256_castpd_si256(c)));
    std::get<0xA>(t) = _mm256_castsi256_pd(_mm256_add_epi64(
        _mm256_castpd_si256(std::get<0xA>(t)), _mm256_castpd_si256(c)));
    std::get<0xB>(t) = _mm256_castsi256_pd(_mm256_add_epi64(
        _mm256_castpd_si256(std::get<0xB>(t)), _mm256_castpd_si256(c)));
    std::get<0xC>(t) = _mm256_castsi256_pd(_mm256_add_epi64(
        _mm256_castpd_si256(std::get<0xC>(t)), _mm256_castpd_si256(c)));
    std::get<0xD>(t) = _mm256_castsi256_pd(_mm256_add_epi64(
        _mm256_castpd_si256(std::get<0xD>(t)), _mm256_castpd_si256(c)));
    std::get<0xE>(t) = _mm256_castsi256_pd(_mm256_add_epi64(
        _mm256_castpd_si256(std::get<0xE>(t)), _mm256_castpd_si256(c)));
    std::get<0xF>(t) = _mm256_castsi256_pd(_mm256_add_epi64(
        _mm256_castpd_si256(std::get<0xF>(t)), _mm256_castpd_si256(c)));

    std::get<0x0>(t) = _mm256_sub_pd(std::get<0x0>(t), c);
    std::get<0x1>(t) = _mm256_sub_pd(std::get<0x1>(t), c);
    std::get<0x2>(t) = _mm256_sub_pd(std::get<0x2>(t), c);
    std::get<0x3>(t) = _mm256_sub_pd(std::get<0x3>(t), c);
    std::get<0x4>(t) = _mm256_sub_pd(std::get<0x4>(t), c);
    std::get<0x5>(t) = _mm256_sub_pd(std::get<0x5>(t), c);
    std::get<0x6>(t) = _mm256_sub_pd(std::get<0x6>(t), c);
    std::get<0x7>(t) = _mm256_sub_pd(std::get<0x7>(t), c);
    std::get<0x8>(t) = _mm256_sub_pd(std::get<0x8>(t), c);
    std::get<0x9>(t) = _mm256_sub_pd(std::get<0x9>(t), c);
    std::get<0xA>(t) = _mm256_sub_pd(std::get<0xA>(t), c);
    std::get<0xB>(t) = _mm256_sub_pd(std::get<0xB>(t), c);
    std::get<0xC>(t) = _mm256_sub_pd(std::get<0xC>(t), c);
    std::get<0xD>(t) = _mm256_sub_pd(std::get<0xD>(t), c);
    std::get<0xE>(t) = _mm256_sub_pd(std::get<0xE>(t), c);
    std::get<0xF>(t) = _mm256_sub_pd(std::get<0xF>(t), c);
}

inline void convert_u32_256d(
    const std::array<__m256i, 8> &s, std::array<__m256d, 16> &t)
{
    const __m256i p = _mm256_set_epi32(7, 3, 6, 2, 5, 1, 4, 0);
    const __m256d m = _mm256_castsi256_pd(
        _mm256_set1_epi64x(static_cast<MCKL_INT64>(0xFFFFFFFF)));
    const __m256d c = _mm256_castsi256_pd( // 2^52 + 2^51
        _mm256_set1_epi64x(static_cast<MCKL_INT64>(0x4338000000000000)));

    std::get<0x0>(t) =
        _mm256_castsi256_pd(_mm256_permutevar8x32_epi32(std::get<0>(s), p));
    std::get<0x2>(t) =
        _mm256_castsi256_pd(_mm256_permutevar8x32_epi32(std::get<1>(s), p));
    std::get<0x4>(t) =
        _mm256_castsi256_pd(_mm256_permutevar8x32_epi32(std::get<2>(s), p));
    std::get<0x6>(t) =
        _mm256_castsi256_pd(_mm256_permutevar8x32_epi32(std::get<3>(s), p));
    std::get<0x8>(t) =
        _mm256_castsi256_pd(_mm256_permutevar8x32_epi32(std::get<4>(s), p));
    std::get<0xA>(t) =
        _mm256_castsi256_pd(_mm256_permutevar8x32_epi32(std::get<5>(s), p));
    std::get<0xC>(t) =
        _mm256_castsi256_pd(_mm256_permutevar8x32_epi32(std::get<6>(s), p));
    std::get<0xE>(t) =
        _mm256_castsi256_pd(_mm256_permutevar8x32_epi32(std::get<7>(s), p));

    std::get<0x1>(t) = _mm256_castsi256_pd(
        _mm256_srli_epi64(_mm256_castpd_si256(std::get<0x0>(t)), 32));
    std::get<0x3>(t) = _mm256_castsi256_pd(
        _mm256_srli_epi64(_mm256_castpd_si256(std::get<0x2>(t)), 32));
    std::get<0x5>(t) = _mm256_castsi256_pd(
        _mm256_srli_epi64(_mm256_castpd_si256(std::get<0x4>(t)), 32));
    std::get<0x7>(t) = _mm256_castsi256_pd(
        _mm256_srli_epi64(_mm256_castpd_si256(std::get<0x6>(t)), 32));
    std::get<0x9>(t) = _mm256_castsi256_pd(
        _mm256_srli_epi64(_mm256_castpd_si256(std::get<0x8>(t)), 32));
    std::get<0xB>(t) = _mm256_castsi256_pd(
        _mm256_srli_epi64(_mm256_castpd_si256(std::get<0xA>(t)), 32));
    std::get<0xD>(t) = _mm256_castsi256_pd(
        _mm256_srli_epi64(_mm256_castpd_si256(std::get<0xC>(t)), 32));
    std::get<0xF>(t) = _mm256_castsi256_pd(
        _mm256_srli_epi64(_mm256_castpd_si256(std::get<0xE>(t)), 32));

    std::get<0x0>(t) = _mm256_and_pd(std::get<0x0>(t), m);
    std::get<0x2>(t) = _mm256_and_pd(std::get<0x2>(t), m);
    std::get<0x4>(t) = _mm256_and_pd(std::get<0x4>(t), m);
    std::get<0x6>(t) = _mm256_and_pd(std::get<0x6>(t), m);
    std::get<0x8>(t) = _mm256_and_pd(std::get<0x8>(t), m);
    std::get<0xA>(t) = _mm256_and_pd(std::get<0xA>(t), m);
    std::get<0xC>(t) = _mm256_and_pd(std::get<0xC>(t), m);
    std::get<0xE>(t) = _mm256_and_pd(std::get<0xE>(t), m);

    std::get<0x0>(t) = _mm256_add_epi64(
        _mm256_castpd_si256(std::get<0x0>(t)), _mm256_castpd_si256(c));
    std::get<0x1>(t) = _mm256_add_epi64(
        _mm256_castpd_si256(std::get<0x1>(t)), _mm256_castpd_si256(c));
    std::get<0x2>(t) = _mm256_add_epi64(
        _mm256_castpd_si256(std::get<0x2>(t)), _mm256_castpd_si256(c));
    std::get<0x3>(t) = _mm256_add_epi64(
        _mm256_castpd_si256(std::get<0x3>(t)), _mm256_castpd_si256(c));
    std::get<0x4>(t) = _mm256_add_epi64(
        _mm256_castpd_si256(std::get<0x4>(t)), _mm256_castpd_si256(c));
    std::get<0x5>(t) = _mm256_add_epi64(
        _mm256_castpd_si256(std::get<0x5>(t)), _mm256_castpd_si256(c));
    std::get<0x6>(t) = _mm256_add_epi64(
        _mm256_castpd_si256(std::get<0x6>(t)), _mm256_castpd_si256(c));
    std::get<0x7>(t) = _mm256_add_epi64(
        _mm256_castpd_si256(std::get<0x7>(t)), _mm256_castpd_si256(c));
    std::get<0x8>(t) = _mm256_add_epi64(
        _mm256_castpd_si256(std::get<0x8>(t)), _mm256_castpd_si256(c));
    std::get<0x9>(t) = _mm256_add_epi64(
        _mm256_castpd_si256(std::get<0x9>(t)), _mm256_castpd_si256(c));
    std::get<0xA>(t) = _mm256_add_epi64(
        _mm256_castpd_si256(std::get<0xA>(t)), _mm256_castpd_si256(c));
    std::get<0xB>(t) = _mm256_add_epi64(
        _mm256_castpd_si256(std::get<0xB>(t)), _mm256_castpd_si256(c));
    std::get<0xC>(t) = _mm256_add_epi64(
        _mm256_castpd_si256(std::get<0xC>(t)), _mm256_castpd_si256(c));
    std::get<0xD>(t) = _mm256_add_epi64(
        _mm256_castpd_si256(std::get<0xD>(t)), _mm256_castpd_si256(c));
    std::get<0xE>(t) = _mm256_add_epi64(
        _mm256_castpd_si256(std::get<0xE>(t)), _mm256_castpd_si256(c));
    std::get<0xF>(t) = _mm256_add_epi64(
        _mm256_castpd_si256(std::get<0xF>(t)), _mm256_castpd_si256(c));

    std::get<0x0>(t) = _mm256_sub_pd(std::get<0x0>(t), c);
    std::get<0x1>(t) = _mm256_sub_pd(std::get<0x1>(t), c);
    std::get<0x2>(t) = _mm256_sub_pd(std::get<0x2>(t), c);
    std::get<0x3>(t) = _mm256_sub_pd(std::get<0x3>(t), c);
    std::get<0x4>(t) = _mm256_sub_pd(std::get<0x4>(t), c);
    std::get<0x5>(t) = _mm256_sub_pd(std::get<0x5>(t), c);
    std::get<0x6>(t) = _mm256_sub_pd(std::get<0x6>(t), c);
    std::get<0x7>(t) = _mm256_sub_pd(std::get<0x7>(t), c);
    std::get<0x8>(t) = _mm256_sub_pd(std::get<0x8>(t), c);
    std::get<0x9>(t) = _mm256_sub_pd(std::get<0x9>(t), c);
    std::get<0xA>(t) = _mm256_sub_pd(std::get<0xA>(t), c);
    std::get<0xB>(t) = _mm256_sub_pd(std::get<0xB>(t), c);
    std::get<0xC>(t) = _mm256_sub_pd(std::get<0xC>(t), c);
    std::get<0xD>(t) = _mm256_sub_pd(std::get<0xD>(t), c);
    std::get<0xE>(t) = _mm256_sub_pd(std::get<0xE>(t), c);
    std::get<0xF>(t) = _mm256_sub_pd(std::get<0xF>(t), c);
}

} // namespace mckl::internal

} // namespace mckl

#ifdef MCKL_GCC
#if MCKL_GCC_VERSION >= 60000
#pragma GCC diagnostic pop
#endif
#endif

#endif // MCKL_INTERNAL_AVX2_HPP
