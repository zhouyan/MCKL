//============================================================================
// MCKL/include/mckl/random/internal/increment_avx2_64_4.hpp
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

#ifndef MCKL_RANDOM_INTERNAL_INCREMENT_AVX2_64_4_HPP
#define MCKL_RANDOM_INTERNAL_INCREMENT_AVX2_64_4_HPP

#include <mckl/random/internal/increment_generic.hpp>

#ifdef MCKL_GCC
#if MCKL_GCC_VERSION >= 60000
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wignored-attributes"
#endif
#endif

namespace mckl {

namespace internal {

template <typename T>
class IncrementBlockSI256<T, 1, 16, 64>
{
    static constexpr std::size_t K_ = 1;

  public:
    static void eval(const std::array<T, K_> &ctr, std::array<__m256i, 4> &s)
    {
        __m256i c =
            _mm256_set1_epi64x(static_cast<MCKL_INT64>(std::get<0>(ctr)));

        std::get<0>(s) =
            _mm256_add_epi64(c, _mm256_set_epi64x(0x04, 0x03, 0x02, 0x01));
        std::get<1>(s) =
            _mm256_add_epi64(c, _mm256_set_epi64x(0x08, 0x07, 0x06, 0x05));
        std::get<2>(s) =
            _mm256_add_epi64(c, _mm256_set_epi64x(0x0C, 0x0B, 0x0A, 0x09));
        std::get<3>(s) =
            _mm256_add_epi64(c, _mm256_set_epi64x(0x10, 0x0F, 0x0E, 0x0D));
    }
}; // class IncrementBlockSI256

template <typename T>
class IncrementBlockSI256<T, 2, 8, 64>
{
    static constexpr std::size_t K_ = 2;

  public:
    static void eval(const std::array<T, K_> &ctr, std::array<__m256i, 4> &s)
    {
        __m256i c =
            _mm256_set_epi64x(static_cast<MCKL_INT64>(std::get<1>(ctr)),
                static_cast<MCKL_INT64>(std::get<0>(ctr)),
                static_cast<MCKL_INT64>(std::get<1>(ctr)),
                static_cast<MCKL_INT64>(std::get<0>(ctr)));

        std::get<0>(s) = _mm256_add_epi64(c, _mm256_set_epi64x(0, 2, 0, 1));
        std::get<1>(s) = _mm256_add_epi64(c, _mm256_set_epi64x(0, 4, 0, 3));
        std::get<2>(s) = _mm256_add_epi64(c, _mm256_set_epi64x(0, 6, 0, 5));
        std::get<3>(s) = _mm256_add_epi64(c, _mm256_set_epi64x(0, 8, 0, 7));
    }
}; // class IncrementBlockSI256

template <typename T>
class IncrementBlockSI256<T, 4, 4, 64>
{
    static constexpr std::size_t K_ = 4;

  public:
    static void eval(const std::array<T, K_> &ctr, std::array<__m256i, 4> &s)
    {
        __m256i c =
            _mm256_set_epi64x(static_cast<MCKL_INT64>(std::get<3>(ctr)),
                static_cast<MCKL_INT64>(std::get<2>(ctr)),
                static_cast<MCKL_INT64>(std::get<1>(ctr)),
                static_cast<MCKL_INT64>(std::get<0>(ctr)));

        std::get<0>(s) = _mm256_add_epi64(c, _mm256_set_epi64x(0, 0, 0, 1));
        std::get<1>(s) = _mm256_add_epi64(c, _mm256_set_epi64x(0, 0, 0, 2));
        std::get<2>(s) = _mm256_add_epi64(c, _mm256_set_epi64x(0, 0, 0, 3));
        std::get<3>(s) = _mm256_add_epi64(c, _mm256_set_epi64x(0, 0, 0, 4));
    }
}; // class IncrementBlockSI256

template <typename T>
class IncrementBlockSI256<T, 8, 2, 64>
{
    static constexpr std::size_t K_ = 8;

  public:
    static void eval(const std::array<T, K_> &ctr, std::array<__m256i, 4> &s)
    {
        __m256i c =
            _mm256_set_epi64x(static_cast<MCKL_INT64>(std::get<3>(ctr)),
                static_cast<MCKL_INT64>(std::get<2>(ctr)),
                static_cast<MCKL_INT64>(std::get<1>(ctr)),
                static_cast<MCKL_INT64>(std::get<0>(ctr)));

        std::get<0>(s) = _mm256_add_epi64(c, _mm256_set_epi64x(0, 0, 0, 1));
        std::get<1>(s) =
            _mm256_set_epi64x(static_cast<MCKL_INT64>(std::get<7>(ctr)),
                static_cast<MCKL_INT64>(std::get<6>(ctr)),
                static_cast<MCKL_INT64>(std::get<5>(ctr)),
                static_cast<MCKL_INT64>(std::get<4>(ctr)));
        std::get<2>(s) = _mm256_add_epi64(c, _mm256_set_epi64x(0, 0, 0, 2));
        std::get<3>(s) = std::get<1>(s);
    }
}; // class IncrementBlockSI256

template <typename T>
class IncrementBlockSI256<T, 16, 1, 64>
{
    static constexpr std::size_t K_ = 16;

  public:
    static void eval(const std::array<T, K_> &ctr, std::array<__m256i, 4> &s)
    {
        std::memcpy(s.data(), ctr.data(), 128);
        std::get<0>(s) =
            _mm256_add_epi64(std::get<0>(s), _mm256_set_epi64x(0, 0, 0, 1));
    }
}; // class IncrementBlockSI256

} // namespace mckl::internal

} // namespace mckl

#ifdef MCKL_GCC
#if MCKL_GCC_VERSION >= 60000
#pragma GCC diagnostic pop
#endif
#endif

#endif // MCKL_RANDOM_INTERNAL_INCREMENT_AVX2_64_4_HPP
