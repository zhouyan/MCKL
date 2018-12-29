//============================================================================
// MCKL/include/mckl/random/internal/increment_avx512_64_8.hpp
//----------------------------------------------------------------------------
// MCKL: Monte Carlo Kernel Library
//----------------------------------------------------------------------------
// Copyright (c) 2013-2018, Yan Zhou
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

#ifndef MCKL_RANDOM_INTERNAL_INCREMENT_AVX512_64_4_HPP
#define MCKL_RANDOM_INTERNAL_INCREMENT_AVX512_64_4_HPP

#include <mckl/random/internal/increment_generic.hpp>

MCKL_PUSH_GCC_WARNING("-Wignored-attributes")

namespace mckl {

namespace internal {

template <typename T>
class IncrementBlockSI512<T, 1, 32, 64>
{
    static constexpr std::size_t K_ = 1;

  public:
    static void eval(const std::array<T, K_> &ctr, std::array<__m512i, 4> &s)
    {
        __m512i c =
            _mm512_set1_epi64(static_cast<MCKL_INT64>(std::get<0>(ctr)));

        std::get<0>(s) = _mm512_add_epi64(c,
            _mm512_set_epi64(0x08, 0x07, 0x06, 0x05, 0x04, 0x03, 0x02, 0x01));
        std::get<1>(s) = _mm512_add_epi64(c,
            _mm512_set_epi64(0x10, 0x0F, 0x0E, 0x0D, 0x0C, 0x0B, 0x0A, 0x09));
        std::get<2>(s) = _mm512_add_epi64(c,
            _mm512_set_epi64(0x18, 0x17, 0x16, 0x15, 0x14, 0x13, 0x12, 0x11));
        std::get<3>(s) = _mm512_add_epi64(c,
            _mm512_set_epi64(0x20, 0x1F, 0x1E, 0x1D, 0x1C, 0x1B, 0x1A, 0x19));
    }
}; // class IncrementBlockSI512

template <typename T>
class IncrementBlockSI512<T, 2, 16, 64>
{
    static constexpr std::size_t K_ = 2;

  public:
    static void eval(const std::array<T, K_> &ctr, std::array<__m512i, 4> &s)
    {
        __m512i c = _mm512_set_epi64(static_cast<MCKL_INT64>(std::get<1>(ctr)),
            static_cast<MCKL_INT64>(std::get<0>(ctr)),
            static_cast<MCKL_INT64>(std::get<1>(ctr)),
            static_cast<MCKL_INT64>(std::get<0>(ctr)),
            static_cast<MCKL_INT64>(std::get<1>(ctr)),
            static_cast<MCKL_INT64>(std::get<0>(ctr)),
            static_cast<MCKL_INT64>(std::get<1>(ctr)),
            static_cast<MCKL_INT64>(std::get<0>(ctr)));

        std::get<0>(s) = _mm512_add_epi64(
            c, _mm512_set_epi64(0, 0x04, 0, 0x03, 0, 0x02, 0, 0x01));
        std::get<1>(s) = _mm512_add_epi64(
            c, _mm512_set_epi64(0, 0x08, 0, 0x07, 0, 0x06, 0, 0x05));
        std::get<2>(s) = _mm512_add_epi64(
            c, _mm512_set_epi64(0, 0x0C, 0, 0x0B, 0, 0x0A, 0, 0x09));
        std::get<3>(s) = _mm512_add_epi64(
            c, _mm512_set_epi64(0, 0x10, 0, 0x0F, 0, 0x0E, 0, 0x0D));
    }
}; // class IncrementBlockSI512

template <typename T>
class IncrementBlockSI512<T, 4, 8, 64>
{
    static constexpr std::size_t K_ = 4;

  public:
    static void eval(const std::array<T, K_> &ctr, std::array<__m512i, 4> &s)
    {
        __m512i c = _mm512_set_epi64(static_cast<MCKL_INT64>(std::get<3>(ctr)),
            static_cast<MCKL_INT64>(std::get<2>(ctr)),
            static_cast<MCKL_INT64>(std::get<1>(ctr)),
            static_cast<MCKL_INT64>(std::get<0>(ctr)),
            static_cast<MCKL_INT64>(std::get<3>(ctr)),
            static_cast<MCKL_INT64>(std::get<2>(ctr)),
            static_cast<MCKL_INT64>(std::get<1>(ctr)),
            static_cast<MCKL_INT64>(std::get<0>(ctr)));

        std::get<0>(s) =
            _mm512_add_epi64(c, _mm512_set_epi64(0, 0, 0, 2, 0, 0, 0, 1));
        std::get<1>(s) =
            _mm512_add_epi64(c, _mm512_set_epi64(0, 0, 0, 4, 0, 0, 0, 3));
        std::get<2>(s) =
            _mm512_add_epi64(c, _mm512_set_epi64(0, 0, 0, 6, 0, 0, 0, 5));
        std::get<3>(s) =
            _mm512_add_epi64(c, _mm512_set_epi64(0, 0, 0, 8, 0, 0, 0, 7));
    }
}; // class IncrementBlockSI512

template <typename T>
class IncrementBlockSI512<T, 8, 4, 64>
{
    static constexpr std::size_t K_ = 8;

  public:
    static void eval(const std::array<T, K_> &ctr, std::array<__m512i, 4> &s)
    {
        __m512i c = _mm512_set_epi64(static_cast<MCKL_INT64>(std::get<7>(ctr)),
            static_cast<MCKL_INT64>(std::get<6>(ctr)),
            static_cast<MCKL_INT64>(std::get<5>(ctr)),
            static_cast<MCKL_INT64>(std::get<4>(ctr)),
            static_cast<MCKL_INT64>(std::get<3>(ctr)),
            static_cast<MCKL_INT64>(std::get<2>(ctr)),
            static_cast<MCKL_INT64>(std::get<1>(ctr)),
            static_cast<MCKL_INT64>(std::get<0>(ctr)));

        std::get<0>(s) =
            _mm512_add_epi64(c, _mm512_set_epi64(0, 0, 0, 0, 0, 0, 0, 1));
        std::get<1>(s) =
            _mm512_add_epi64(c, _mm512_set_epi64(0, 0, 0, 0, 0, 0, 0, 2));
        std::get<2>(s) =
            _mm512_add_epi64(c, _mm512_set_epi64(0, 0, 0, 0, 0, 0, 0, 3));
        std::get<3>(s) =
            _mm512_add_epi64(c, _mm512_set_epi64(0, 0, 0, 0, 0, 0, 0, 4));
    }
}; // class IncrementBlockSI512

template <typename T>
class IncrementBlockSI512<T, 16, 2, 64>
{
    static constexpr std::size_t K_ = 16;

  public:
    static void eval(const std::array<T, K_> &ctr, std::array<__m512i, 4> &s)
    {
        __m512i c =
            _mm512_set_epi64(static_cast<MCKL_INT64>(std::get<0x7>(ctr)),
                static_cast<MCKL_INT64>(std::get<0x5>(ctr)),
                static_cast<MCKL_INT64>(std::get<0x5>(ctr)),
                static_cast<MCKL_INT64>(std::get<0x4>(ctr)),
                static_cast<MCKL_INT64>(std::get<0x3>(ctr)),
                static_cast<MCKL_INT64>(std::get<0x2>(ctr)),
                static_cast<MCKL_INT64>(std::get<0x1>(ctr)),
                static_cast<MCKL_INT64>(std::get<0x0>(ctr)));

        std::get<0>(s) =
            _mm512_add_epi64(c, _mm512_set_epi64(0, 0, 0, 0, 0, 0, 0, 1));
        std::get<1>(s) =
            _mm512_set_epi64(static_cast<MCKL_INT64>(std::get<0xF>(ctr)),
                static_cast<MCKL_INT64>(std::get<0xE>(ctr)),
                static_cast<MCKL_INT64>(std::get<0xD>(ctr)),
                static_cast<MCKL_INT64>(std::get<0xC>(ctr)),
                static_cast<MCKL_INT64>(std::get<0xB>(ctr)),
                static_cast<MCKL_INT64>(std::get<0xA>(ctr)),
                static_cast<MCKL_INT64>(std::get<0x9>(ctr)),
                static_cast<MCKL_INT64>(std::get<0x8>(ctr)));
        std::get<2>(s) =
            _mm512_add_epi64(c, _mm512_set_epi64(0, 0, 0, 0, 0, 0, 0, 2));
        std::get<3>(s) = std::get<1>(s);
    }
}; // class IncrementBlockSI512

template <typename T>
class IncrementBlockSI512<T, 32, 1, 64>
{
    static constexpr std::size_t K_ = 32;

  public:
    static void eval(const std::array<T, K_> &ctr, std::array<__m512i, 4> &s)
    {
        MCKL_MEMCPY(s.data(), ctr.data(), 512);
        std::get<0>(s) = _mm512_add_epi64(
            std::get<0>(s), _mm512_set_epi64(0, 0, 0, 0, 0, 0, 0, 1));
    }
}; // class IncrementBlockSI512

} // namespace internal

} // namespace mckl

MCKL_POP_GCC_WARNING

#endif // MCKL_RANDOM_INTERNAL_INCREMENT_AVX512_64_4_HPP
