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
MCKL_FLATTEN inline void transpose8x32_si256(std::array<__m256i, N> &s)
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

MCKL_FLATTEN inline void transpose8x32_load_si256(std::array<__m256i, 8> &s)
{
    transpose8x32_si256<0, 1, 2, 3, 4, 5, 6, 7>(s);
}

MCKL_FLATTEN inline void transpose8x32_store_si256(std::array<__m256i, 8> &s)
{
    transpose8x32_si256<0, 1, 2, 3, 4, 5, 6, 7>(s);
}

MCKL_FLATTEN inline void transpose8x32_load_si256(std::array<__m256i, 16> &s)
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

MCKL_FLATTEN inline void transpose8x32_store_si256(std::array<__m256i, 16> &s)
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
MCKL_FLATTEN inline void transpose4x64_si256(std::array<__m256i, N> &s)
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

MCKL_FLATTEN inline void transpose4x64_load_si256(std::array<__m256i, 4> &s)
{
    transpose4x64_si256<0, 1, 2, 3>(s);
}

MCKL_FLATTEN inline void transpose4x64_store_si256(std::array<__m256i, 4> &s)
{
    transpose4x64_si256<0, 1, 2, 3>(s);
}

MCKL_FLATTEN inline void transpose4x64_load_si256(std::array<__m256i, 8> &s)
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

MCKL_FLATTEN inline void transpose4x64_store_si256(std::array<__m256i, 8> &s)
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

MCKL_FLATTEN inline void transpose4x64_load_si256(std::array<__m256i, 16> &s)
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

MCKL_FLATTEN inline void transpose4x64_store_si256(std::array<__m256i, 16> &s)
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

} // namespace mckl::internal

} // namespace mckl

#ifdef MCKL_GCC
#if MCKL_GCC_VERSION >= 60000
#pragma GCC diagnostic pop
#endif
#endif

#endif // MCKL_INTERNAL_AVX2_HPP
