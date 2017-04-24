//============================================================================
// MCKL/include/mckl/random/internal/increment_avx2_64.hpp
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

namespace mckl {

namespace internal {

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
