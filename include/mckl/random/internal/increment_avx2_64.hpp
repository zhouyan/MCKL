//============================================================================
// MCKL/include/mckl/random/internal/increment_avx2_64.hpp
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

#ifndef MCKL_RANDOM_INTERNAL_INCREMENT_AVX2_64_HPP
#define MCKL_RANDOM_INTERNAL_INCREMENT_AVX2_64_HPP

#include <mckl/random/internal/increment_avx2_64_4.hpp>
#include <mckl/random/internal/increment_avx2_64_8.hpp>
#include <mckl/random/internal/increment_avx2_64_f.hpp>
#include <mckl/random/internal/increment_generic.hpp>

#ifdef MCKL_GCC
#if MCKL_GCC_VERSION >= 60000
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wignored-attributes"
#endif
#endif

#define MCKL_RANDOM_INTERNAL_INCREMENT_AVX2_64_1_8(ymmc)                      \
    __m256i ymm0 =                                                            \
        _mm256_add_epi64(ymmc, _mm256_set_epi64x(0x04, 0x03, 0x02, 0x01));    \
    __m256i ymm1 =                                                            \
        _mm256_add_epi64(ymmc, _mm256_set_epi64x(0x08, 0x07, 0x06, 0x05));    \
    __m256i ymm2 =                                                            \
        _mm256_add_epi64(ymmc, _mm256_set_epi64x(0x0C, 0x0B, 0x0A, 0x09));    \
    __m256i ymm3 =                                                            \
        _mm256_add_epi64(ymmc, _mm256_set_epi64x(0x10, 0x0F, 0x0E, 0x0D));    \
    __m256i ymm4 =                                                            \
        _mm256_add_epi64(ymmc, _mm256_set_epi64x(0x14, 0x13, 0x12, 0x11));    \
    __m256i ymm5 =                                                            \
        _mm256_add_epi64(ymmc, _mm256_set_epi64x(0x18, 0x17, 0x16, 0x15));    \
    __m256i ymm6 =                                                            \
        _mm256_add_epi64(ymmc, _mm256_set_epi64x(0x1C, 0x1B, 0x1A, 0x19));    \
    __m256i ymm7 =                                                            \
        _mm256_add_epi64(ymmc, _mm256_set_epi64x(0x20, 0x1F, 0x1E, 0x1D));    \
    ymmc = _mm256_add_epi64(ymmc, _mm256_set1_epi64x(0x20));

#define MCKL_RANDOM_INTERNAL_INCREMENT_AVX2_64_2_8(ymmc)                      \
    __m256i ymm0 =                                                            \
        _mm256_add_epi64(ymmc, _mm256_set_epi64x(0, 0x02, 0, 0x01));          \
    __m256i ymm1 =                                                            \
        _mm256_add_epi64(ymmc, _mm256_set_epi64x(0, 0x04, 0, 0x03));          \
    __m256i ymm2 =                                                            \
        _mm256_add_epi64(ymmc, _mm256_set_epi64x(0, 0x06, 0, 0x05));          \
    __m256i ymm3 =                                                            \
        _mm256_add_epi64(ymmc, _mm256_set_epi64x(0, 0x08, 0, 0x07));          \
    __m256i ymm4 =                                                            \
        _mm256_add_epi64(ymmc, _mm256_set_epi64x(0, 0x0A, 0, 0x09));          \
    __m256i ymm5 =                                                            \
        _mm256_add_epi64(ymmc, _mm256_set_epi64x(0, 0x0C, 0, 0x0B));          \
    __m256i ymm6 =                                                            \
        _mm256_add_epi64(ymmc, _mm256_set_epi64x(0, 0x0E, 0, 0x0D));          \
    __m256i ymm7 =                                                            \
        _mm256_add_epi64(ymmc, _mm256_set_epi64x(0, 0x10, 0, 0x0F));          \
    ymmc = _mm256_add_epi64(ymmc, _mm256_set_epi64x(0, 0x10, 0, 0x10));

namespace mckl
{

namespace internal
{

template <typename T, std::size_t K, std::size_t S>
inline void increment_si256(
    std::array<T, K> &ctr, std::array<__m256i, S> &s, std::false_type)
{
    constexpr T blocks = (sizeof(__m256i) * S) / (sizeof(T) * K);

    alignas(32) std::array<std::array<T, K>, blocks> ctr_block;
    increment(ctr, ctr_block);
    std::memcpy(s.data(), ctr_block.data(), sizeof(__m256i) * S);
}

template <typename T, std::size_t K, std::size_t S>
inline void increment_si256(
    std::array<T, K> &ctr, std::array<__m256i, S> &s, std::true_type)
{
    constexpr T blocks = (sizeof(__m256i) * S) / (sizeof(T) * K);

    if (std::get<0>(ctr) < std::numeric_limits<T>::max() - blocks) {
        IncrementBlockSI256<T, K, blocks>::eval(ctr, s);
        std::get<0>(ctr) += blocks;
    } else {
        increment_si256(ctr, s, std::false_type());
    }
}

template <typename T, std::size_t K, std::size_t S>
MCKL_INLINE inline void increment_si256(
    std::array<T, K> &ctr, std::array<__m256i, S> &s)
{
    constexpr bool direct = S == 4 || S == 8 || S == 16;
    constexpr bool bits = std::numeric_limits<T>::digits == 64;

    internal::increment_si256(
        ctr, s, std::integral_constant<bool, (direct && bits)>());
}

} // namespace mckl::internal

} // namespace mckl

#ifdef MCKL_GCC
#if MCKL_GCC_VERSION >= 60000
#pragma GCC diagnostic pop
#endif
#endif

#endif // MCKL_RANDOM_INTERNAL_INCREMENT_AVX2_64_HPP
