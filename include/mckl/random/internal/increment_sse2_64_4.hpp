//============================================================================
// MCKL/include/mckl/random/internal/increment_sse2_64_4.hpp
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

#ifndef MCKL_RANDOM_INTERNAL_INCREMENT_SSE2_64_4_HPP
#define MCKL_RANDOM_INTERNAL_INCREMENT_SSE2_64_4_HPP

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
class IncrementBlock<T, 1, 8, 64>
{
    static constexpr std::size_t K_ = 1;
    static constexpr std::size_t blocks_ = 8;

    public:
    MCKL_FLATTEN static bool aligned(void *ptr)
    {
        return reinterpret_cast<std::uintptr_t>(ptr) % 16 == 0;
    }

    MCKL_FLATTEN static void eval(std::array<T, K_> &ctr,
        std::array<std::array<T, K_>, blocks_> &ctr_block)
    {
        __m128i a0 =
            _mm_set1_epi64x(static_cast<MCKL_INT64>(std::get<0>(ctr)));

        __m128i c0 = _mm_add_epi64(a0, _mm_set_epi64x(2, 1));
        __m128i c1 = _mm_add_epi64(a0, _mm_set_epi64x(4, 3));
        __m128i c2 = _mm_add_epi64(a0, _mm_set_epi64x(6, 5));
        __m128i c3 = _mm_add_epi64(a0, _mm_set_epi64x(8, 7));

        __m128i *cptr = reinterpret_cast<__m128i *>(ctr_block.data());
        _mm_store_si128(cptr++, c0);
        _mm_store_si128(cptr++, c1);
        _mm_store_si128(cptr++, c2);
        _mm_store_si128(cptr++, c3);
    }
}; // class IncrementBlock

template <typename T>
class IncrementBlock<T, 2, 4, 64>
{
    static constexpr std::size_t K_ = 2;
    static constexpr std::size_t blocks_ = 4;

    public:
    MCKL_FLATTEN static bool aligned(void *ptr)
    {
        return reinterpret_cast<std::uintptr_t>(ptr) % 16 == 0;
    }

    MCKL_FLATTEN static void eval(std::array<T, K_> &ctr,
        std::array<std::array<T, K_>, blocks_> &ctr_block)
    {
        __m128i a0 = _mm_set_epi64x(static_cast<MCKL_INT64>(std::get<1>(ctr)),
            static_cast<MCKL_INT64>(std::get<0>(ctr)));

        __m128i c0 = _mm_add_epi64(a0, _mm_set_epi64x(0, 1));
        __m128i c1 = _mm_add_epi64(a0, _mm_set_epi64x(0, 2));
        __m128i c2 = _mm_add_epi64(a0, _mm_set_epi64x(0, 3));
        __m128i c3 = _mm_add_epi64(a0, _mm_set_epi64x(0, 4));

        __m128i *cptr = reinterpret_cast<__m128i *>(ctr_block.data());
        _mm_store_si128(cptr++, c0);
        _mm_store_si128(cptr++, c1);
        _mm_store_si128(cptr++, c2);
        _mm_store_si128(cptr++, c3);
    }
}; // class IncrementBlock

template <typename T>
class IncrementBlock<T, 4, 2, 64>
{
    static constexpr std::size_t K_ = 4;
    static constexpr std::size_t blocks_ = 2;

    public:
    MCKL_FLATTEN static bool aligned(void *ptr)
    {
        return reinterpret_cast<std::uintptr_t>(ptr) % 16 == 0;
    }

    MCKL_FLATTEN static void eval(std::array<T, K_> &ctr,
        std::array<std::array<T, K_>, blocks_> &ctr_block)
    {
        __m128i a0 = _mm_set_epi64x(static_cast<MCKL_INT64>(std::get<1>(ctr)),
            static_cast<MCKL_INT64>(std::get<0>(ctr)));
        __m128i a1 = _mm_set_epi64x(static_cast<MCKL_INT64>(std::get<3>(ctr)),
            static_cast<MCKL_INT64>(std::get<2>(ctr)));

        __m128i c0 = _mm_add_epi64(a0, _mm_set_epi64x(0, 1));
        __m128i c2 = _mm_add_epi64(a0, _mm_set_epi64x(0, 2));

        __m128i *cptr = reinterpret_cast<__m128i *>(ctr_block.data());
        _mm_store_si128(cptr++, c0);
        _mm_store_si128(cptr++, a1);
        _mm_store_si128(cptr++, c2);
        _mm_store_si128(cptr++, a1);
    }
}; // class IncrementBlock

} // namespace mckl::internal

} // namespace mckl

#ifdef MCKL_GCC
#if MCKL_GCC_VERSION >= 60000
#pragma GCC diagnostic pop
#endif
#endif

#endif // MCKL_RANDOM_INTERNAL_INCREMENT_SSE2_64_4_HPP
