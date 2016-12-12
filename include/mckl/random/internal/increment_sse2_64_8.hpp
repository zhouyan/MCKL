//============================================================================
// MCKL/include/mckl/random/internal/increment_sse2_64_8.hpp
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

#ifndef MCKL_RANDOM_INTERNAL_INCREMENT_SSE2_64_8_HPP
#define MCKL_RANDOM_INTERNAL_INCREMENT_SSE2_64_8_HPP

#include <mckl/random/internal/increment_generic.hpp>

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

template <typename T>
class IncrementBlockSI128<T, 1, 16, 64>
{
    static constexpr std::size_t K_ = 1;

    public:
    static void eval(std::array<T, K_> &ctr, std::array<__m128i, 8> &s)
    {
        __m128i c = _mm_set1_epi64x(static_cast<MCKL_INT64>(std::get<0>(ctr)));

        std::get<0>(s) = _mm_add_epi64(c, _mm_set_epi64x(0x02, 0x01));
        std::get<1>(s) = _mm_add_epi64(c, _mm_set_epi64x(0x04, 0x03));
        std::get<2>(s) = _mm_add_epi64(c, _mm_set_epi64x(0x06, 0x05));
        std::get<3>(s) = _mm_add_epi64(c, _mm_set_epi64x(0x08, 0x07));
        std::get<4>(s) = _mm_add_epi64(c, _mm_set_epi64x(0x0A, 0x09));
        std::get<5>(s) = _mm_add_epi64(c, _mm_set_epi64x(0x0C, 0x0B));
        std::get<6>(s) = _mm_add_epi64(c, _mm_set_epi64x(0x0E, 0x0D));
        std::get<7>(s) = _mm_add_epi64(c, _mm_set_epi64x(0x10, 0x0F));
    }
}; // class IncrementBlockSI128

template <typename T>
class IncrementBlockSI128<T, 2, 8, 64>
{
    static constexpr std::size_t K_ = 2;

    public:
    static void eval(std::array<T, K_> &ctr, std::array<__m128i, 8> &s)
    {
        __m128i c = _mm_set_epi64x(static_cast<MCKL_INT64>(std::get<1>(ctr)),
            static_cast<MCKL_INT64>(std::get<0>(ctr)));

        std::get<0>(s) = _mm_add_epi64(c, _mm_set_epi64x(0, 1));
        std::get<1>(s) = _mm_add_epi64(c, _mm_set_epi64x(0, 2));
        std::get<2>(s) = _mm_add_epi64(c, _mm_set_epi64x(0, 3));
        std::get<3>(s) = _mm_add_epi64(c, _mm_set_epi64x(0, 4));
        std::get<4>(s) = _mm_add_epi64(c, _mm_set_epi64x(0, 5));
        std::get<5>(s) = _mm_add_epi64(c, _mm_set_epi64x(0, 6));
        std::get<6>(s) = _mm_add_epi64(c, _mm_set_epi64x(0, 7));
        std::get<7>(s) = _mm_add_epi64(c, _mm_set_epi64x(0, 8));
    }
}; // class IncrementBlockSI128

template <typename T>
class IncrementBlockSI128<T, 4, 4, 64>
{
    static constexpr std::size_t K_ = 4;

    public:
    static void eval(std::array<T, K_> &ctr, std::array<__m128i, 8> &s)
    {
        __m128i c = _mm_set_epi64x(static_cast<MCKL_INT64>(std::get<1>(ctr)),
            static_cast<MCKL_INT64>(std::get<0>(ctr)));

        std::get<0>(s) = _mm_add_epi64(c, _mm_set_epi64x(0, 1));
        std::get<1>(s) =
            _mm_set_epi64x(static_cast<MCKL_INT64>(std::get<3>(ctr)),
                static_cast<MCKL_INT64>(std::get<2>(ctr)));
        std::get<2>(s) = _mm_add_epi64(c, _mm_set_epi64x(0, 2));
        std::get<3>(s) = std::get<1>(s);
        std::get<4>(s) = _mm_add_epi64(c, _mm_set_epi64x(0, 3));
        std::get<5>(s) = std::get<1>(s);
        std::get<6>(s) = _mm_add_epi64(c, _mm_set_epi64x(0, 4));
        std::get<7>(s) = std::get<1>(s);
    }
}; // class IncrementBlockSI128

template <typename T>
class IncrementBlockSI128<T, 8, 2, 64>
{
    static constexpr std::size_t K_ = 8;

    public:
    static void eval(std::array<T, K_> &ctr, std::array<__m128i, 8> &s)
    {
        __m128i c = _mm_set_epi64x(static_cast<MCKL_INT64>(std::get<1>(ctr)),
            static_cast<MCKL_INT64>(std::get<0>(ctr)));

        std::get<0>(s) = _mm_add_epi64(c, _mm_set_epi64x(0, 1));
        std::get<1>(s) =
            _mm_set_epi64x(static_cast<MCKL_INT64>(std::get<3>(ctr)),
                static_cast<MCKL_INT64>(std::get<2>(ctr)));
        std::get<2>(s) =
            _mm_set_epi64x(static_cast<MCKL_INT64>(std::get<5>(ctr)),
                static_cast<MCKL_INT64>(std::get<4>(ctr)));
        std::get<3>(s) =
            _mm_set_epi64x(static_cast<MCKL_INT64>(std::get<7>(ctr)),
                static_cast<MCKL_INT64>(std::get<6>(ctr)));
        std::get<4>(s) = _mm_add_epi64(c, _mm_set_epi64x(0, 2));
        std::get<5>(s) = std::get<1>(s);
        std::get<6>(s) = std::get<2>(s);
        std::get<7>(s) = std::get<3>(s);
    }
}; // class IncrementBlockSI128

template <typename T>
class IncrementBlockSI128<T, 16, 1, 64>
{
    static constexpr std::size_t K_ = 16;

    public:
    static void eval(const std::array<T, K_> &ctr, std::array<__m128i, 8> &s)
    {
        std::memcpy(s.data(), ctr.data(), 128);
        std::get<0>(s) = _mm_add_epi64(std::get<0>(s), _mm_set_epi64x(0, 1));
    }
}; // class IncrementBlockSI128

} // namespace mckl::internal

} // namespace mckl

#ifdef MCKL_GCC
#if MCKL_GCC_VERSION >= 60000
#pragma GCC diagnostic pop
#endif
#endif

#endif // MCKL_RANDOM_INTERNAL_INCREMENT_SSE2_64_8_HPP
