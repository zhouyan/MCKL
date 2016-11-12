//============================================================================
// MCKL/include/mckl/random/internal/increment_sse2_64.hpp
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

#ifndef MCKL_RANDOM_INTERNAL_INCREMENT_SSE2_64_HPP
#define MCKL_RANDOM_INTERNAL_INCREMENT_SSE2_64_HPP

#include <mckl/random/internal/increment_generic.hpp>
#include <mckl/random/internal/increment_sse2_64_4.hpp>
#include <mckl/random/internal/increment_sse2_64_8.hpp>
#include <mckl/random/internal/increment_sse2_64_f.hpp>

#ifdef MCKL_GCC
#if MCKL_GCC_VERSION >= 60000
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wignored-attributes"
#endif
#endif

#define MCKL_RANDOM_INTERNAL_INCREMENT_SSE2_64_1_8(xmmc)                      \
    __m128i xmm0 = _mm_add_epi64(xmmc, _mm_set_epi64x(0x02, 0x01));           \
    __m128i xmm1 = _mm_add_epi64(xmmc, _mm_set_epi64x(0x04, 0x03));           \
    __m128i xmm2 = _mm_add_epi64(xmmc, _mm_set_epi64x(0x06, 0x05));           \
    __m128i xmm3 = _mm_add_epi64(xmmc, _mm_set_epi64x(0x08, 0x07));           \
    __m128i xmm4 = _mm_add_epi64(xmmc, _mm_set_epi64x(0x0A, 0x09));           \
    __m128i xmm5 = _mm_add_epi64(xmmc, _mm_set_epi64x(0x0C, 0x0B));           \
    __m128i xmm6 = _mm_add_epi64(xmmc, _mm_set_epi64x(0x0E, 0x0D));           \
    __m128i xmm7 = _mm_add_epi64(xmmc, _mm_set_epi64x(0x10, 0x0F));           \
    xmmc = xmm7;

#define MCKL_RANDOM_INTERNAL_INCREMENT_SSE2_64_2_8(xmmc)                      \
    __m128i xmm0 = _mm_add_epi64(xmmc, _mm_set_epi64x(0, 1));                 \
    __m128i xmm1 = _mm_add_epi64(xmmc, _mm_set_epi64x(0, 2));                 \
    __m128i xmm2 = _mm_add_epi64(xmmc, _mm_set_epi64x(0, 3));                 \
    __m128i xmm3 = _mm_add_epi64(xmmc, _mm_set_epi64x(0, 4));                 \
    __m128i xmm4 = _mm_add_epi64(xmmc, _mm_set_epi64x(0, 5));                 \
    __m128i xmm5 = _mm_add_epi64(xmmc, _mm_set_epi64x(0, 6));                 \
    __m128i xmm6 = _mm_add_epi64(xmmc, _mm_set_epi64x(0, 7));                 \
    __m128i xmm7 = _mm_add_epi64(xmmc, _mm_set_epi64x(0, 8));                 \
    xmmc = xmm7;

namespace mckl
{

namespace internal
{

template <typename T, std::size_t K, std::size_t S>
inline void increment_si128(
    std::array<T, K> &ctr, std::array<__m128i, S> &s, std::false_type)
{
    constexpr T blocks = (sizeof(__m128i) * S) / (sizeof(T) * K);

    alignas(32) std::array<std::array<T, K>, blocks> ctr_block;
    increment(ctr, ctr_block);
    std::memcpy(s.data(), ctr_block.data(), sizeof(__m128i) * S);
}

template <typename T, std::size_t K, std::size_t S>
inline void increment_si128(
    std::array<T, K> &ctr, std::array<__m128i, S> &s, std::true_type)
{
    constexpr T blocks = (sizeof(__m128i) * S) / (sizeof(T) * K);

    if (std::get<0>(ctr) < std::numeric_limits<T>::max() - blocks) {
        IncrementBlockSI128<T, K, blocks>::eval(ctr, s);
        std::get<0>(ctr) += blocks;
    } else {
        increment_si128(ctr, s, std::false_type());
    }
}

template <typename T, std::size_t K, std::size_t S>
MCKL_INLINE inline void increment_si128(
    std::array<T, K> &ctr, std::array<__m128i, S> &s)
{
    constexpr bool direct = S == 4 || S == 8 || S == 16;
    constexpr bool bits = std::numeric_limits<T>::digits == 64;

    internal::increment_si128(
        ctr, s, std::integral_constant<bool, (direct && bits)>());
}

} // namespace mckl::internal

} // namespace mckl

#ifdef MCKL_GCC
#if MCKL_GCC_VERSION >= 60000
#pragma GCC diagnostic pop
#endif
#endif

#endif // MCKL_RANDOM_INTERNAL_INCREMENT_SSE2_64_HPP
