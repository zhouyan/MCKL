//============================================================================
// MCKL/include/mckl/random/internal/philox_sse2_4x32.hpp
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

#ifndef MCKL_RANDOM_INTERNAL_PHILOX_SSE2_4X32_HPP
#define MCKL_RANDOM_INTERNAL_PHILOX_SSE2_4X32_HPP

#include <mckl/random/internal/common.hpp>
#include <mckl/random/internal/philox_common.hpp>
#include <mckl/random/internal/philox_constants.hpp>
#include <mckl/random/internal/philox_generic_4x.hpp>

#ifdef MCKL_GCC
#if MCKL_GCC_VERSION >= 60000
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wignored-attributes"
#endif
#endif

#define MCKL_RANDOM_INTERNAL_PHILOX_SSE2_4X32_RBOX(xmmk, imm8)                \
    xmmt0 = _mm_mul_epu32(xmms0, xmmm);                                       \
    xmmt1 = _mm_mul_epu32(xmms1, xmmm);                                       \
    xmmt2 = _mm_mul_epu32(xmms2, xmmm);                                       \
    xmmt3 = _mm_mul_epu32(xmms3, xmmm);                                       \
    xmmt4 = _mm_mul_epu32(xmms4, xmmm);                                       \
    xmmt5 = _mm_mul_epu32(xmms5, xmmm);                                       \
    xmmt6 = _mm_mul_epu32(xmms6, xmmm);                                       \
    xmmt7 = _mm_mul_epu32(xmms7, xmmm);                                       \
    xmms0 = _mm_and_si128(xmms0, xmma);                                       \
    xmms1 = _mm_and_si128(xmms1, xmma);                                       \
    xmms2 = _mm_and_si128(xmms2, xmma);                                       \
    xmms3 = _mm_and_si128(xmms3, xmma);                                       \
    xmms4 = _mm_and_si128(xmms4, xmma);                                       \
    xmms5 = _mm_and_si128(xmms5, xmma);                                       \
    xmms6 = _mm_and_si128(xmms6, xmma);                                       \
    xmms7 = _mm_and_si128(xmms7, xmma);                                       \
    xmms0 = _mm_xor_si128(xmms0, xmmk);                                       \
    xmms1 = _mm_xor_si128(xmms1, xmmk);                                       \
    xmms2 = _mm_xor_si128(xmms2, xmmk);                                       \
    xmms3 = _mm_xor_si128(xmms3, xmmk);                                       \
    xmms4 = _mm_xor_si128(xmms4, xmmk);                                       \
    xmms5 = _mm_xor_si128(xmms5, xmmk);                                       \
    xmms6 = _mm_xor_si128(xmms6, xmmk);                                       \
    xmms7 = _mm_xor_si128(xmms7, xmmk);                                       \
    xmms0 = _mm_xor_si128(xmms0, xmmt0);                                      \
    xmms1 = _mm_xor_si128(xmms1, xmmt1);                                      \
    xmms2 = _mm_xor_si128(xmms2, xmmt2);                                      \
    xmms3 = _mm_xor_si128(xmms3, xmmt3);                                      \
    xmms4 = _mm_xor_si128(xmms4, xmmt4);                                      \
    xmms5 = _mm_xor_si128(xmms5, xmmt5);                                      \
    xmms6 = _mm_xor_si128(xmms6, xmmt6);                                      \
    xmms7 = _mm_xor_si128(xmms7, xmmt7);                                      \
    xmms0 = _mm_shuffle_epi32(xmms0, imm8);                                   \
    xmms1 = _mm_shuffle_epi32(xmms1, imm8);                                   \
    xmms2 = _mm_shuffle_epi32(xmms2, imm8);                                   \
    xmms3 = _mm_shuffle_epi32(xmms3, imm8);                                   \
    xmms4 = _mm_shuffle_epi32(xmms4, imm8);                                   \
    xmms5 = _mm_shuffle_epi32(xmms5, imm8);                                   \
    xmms6 = _mm_shuffle_epi32(xmms6, imm8);                                   \
    xmms7 = _mm_shuffle_epi32(xmms7, imm8);

namespace mckl
{

namespace internal
{

template <typename T, typename Constants>
class Philox4x32GeneratorSSE2Impl
{
    static_assert(std::numeric_limits<T>::digits == 32,
        "**Philox4x32GeneratorSSE2Impl** used with T other than a 32-bit "
        "unsigned integers");

    static constexpr std::size_t K = 4;
    static constexpr std::size_t Rounds = 10;

    public:
    static void eval(
        const void *plain, void *cipher, const std::array<T, K / 2> &key)
    {
        Philox4xGeneratorGenericImpl<T, Constants>::eval(plain, cipher, key);
    }

    template <typename ResultType>
    static void eval(std::array<std::uint64_t, 2> &ctr, ResultType *r,
        const std::array<T, K / 2> &key)
    {
        Philox4xGeneratorGenericImpl<T, Constants>::eval(ctr, r, key);
    }

    template <typename ResultType>
    static void eval(std::array<std::uint64_t, 2> &ctr, std::size_t n,
        ResultType *r, const std::array<T, K / 2> &key)
    {
        if (ctr.front() >= std::numeric_limits<std::uint64_t>::max() - n) {
            MCKL_NOINLINE_CALL Philox4xGeneratorGenericImpl<T,
                Constants>::eval(ctr, n, r, key);
            return;
        }

        constexpr std::size_t S = 8;
        constexpr std::size_t N = sizeof(__m128i) * S / (sizeof(T) * K);
        constexpr std::size_t M = sizeof(__m128i) / sizeof(ResultType);
        constexpr std::size_t R = sizeof(T) * K / sizeof(ResultType);

        constexpr int m0 = static_cast<int>(Constants::multiplier::value[0]);
        constexpr int m1 = static_cast<int>(Constants::multiplier::value[1]);
        __m128i xmmm = _mm_set_epi32(0, m1, 0, m0);

        constexpr int ma = static_cast<int>(0xFFFFFFFF);
        __m128i xmma = _mm_set_epi32(ma, 0, ma, 0);

        constexpr int w0 = static_cast<int>(Constants::weyl::value[0]);
        constexpr int w1 = static_cast<int>(Constants::weyl::value[1]);
        __m128i xmmw = _mm_set_epi32(w1, 0, w0, 0);

        const int k0 = static_cast<int>(std::get<0>(key));
        const int k1 = static_cast<int>(std::get<1>(key));
        __m128i xmmk0 = _mm_set_epi32(k1, 0, k0, 0);
        __m128i xmmk1 = _mm_add_epi32(xmmk0, xmmw);
        __m128i xmmk2 = _mm_add_epi32(xmmk1, xmmw);
        __m128i xmmk3 = _mm_add_epi32(xmmk2, xmmw);
        __m128i xmmk4 = _mm_add_epi32(xmmk3, xmmw);
        __m128i xmmk5 = _mm_add_epi32(xmmk4, xmmw);
        __m128i xmmk6 = _mm_add_epi32(xmmk5, xmmw);
        __m128i xmmk7 = _mm_add_epi32(xmmk6, xmmw);
        __m128i xmmk8 = _mm_add_epi32(xmmk7, xmmw);
        __m128i xmmk9 = _mm_add_epi32(xmmk8, xmmw);

        __m128i xmmc =
            _mm_set_epi64x(static_cast<MCKL_INT64>(std::get<1>(ctr)),
                static_cast<MCKL_INT64>(std::get<0>(ctr)));
        ctr.front() += n / N * N;

        while (n >= N) {
            __m128i xmms0 = _mm_add_epi64(xmmc, _mm_set_epi64x(0, 1));
            __m128i xmms1 = _mm_add_epi64(xmmc, _mm_set_epi64x(0, 2));
            __m128i xmms2 = _mm_add_epi64(xmmc, _mm_set_epi64x(0, 3));
            __m128i xmms3 = _mm_add_epi64(xmmc, _mm_set_epi64x(0, 4));
            __m128i xmms4 = _mm_add_epi64(xmmc, _mm_set_epi64x(0, 5));
            __m128i xmms5 = _mm_add_epi64(xmmc, _mm_set_epi64x(0, 6));
            __m128i xmms6 = _mm_add_epi64(xmmc, _mm_set_epi64x(0, 7));
            __m128i xmms7 = _mm_add_epi64(xmmc, _mm_set_epi64x(0, 8));

            xmmc = _mm_add_epi64(
                xmmc, _mm_set_epi64x(0, static_cast<MCKL_INT64>(N)));

            __m128i xmmt0;
            __m128i xmmt1;
            __m128i xmmt2;
            __m128i xmmt3;
            __m128i xmmt4;
            __m128i xmmt5;
            __m128i xmmt6;
            __m128i xmmt7;

            xmms0 = _mm_shuffle_epi32(xmms0, 0xC6); // 3 0 1 2
            xmms1 = _mm_shuffle_epi32(xmms1, 0xC6); // 3 0 1 2
            xmms2 = _mm_shuffle_epi32(xmms2, 0xC6); // 3 0 1 2
            xmms3 = _mm_shuffle_epi32(xmms3, 0xC6); // 3 0 1 2
            xmms4 = _mm_shuffle_epi32(xmms4, 0xC6); // 3 0 1 2
            xmms5 = _mm_shuffle_epi32(xmms5, 0xC6); // 3 0 1 2
            xmms6 = _mm_shuffle_epi32(xmms6, 0xC6); // 3 0 1 2
            xmms7 = _mm_shuffle_epi32(xmms7, 0xC6); // 3 0 1 2

            MCKL_RANDOM_INTERNAL_PHILOX_SSE2_4X32_RBOX(xmmk0, 0x93)
            MCKL_RANDOM_INTERNAL_PHILOX_SSE2_4X32_RBOX(xmmk1, 0x93)
            MCKL_RANDOM_INTERNAL_PHILOX_SSE2_4X32_RBOX(xmmk2, 0x93)
            MCKL_RANDOM_INTERNAL_PHILOX_SSE2_4X32_RBOX(xmmk3, 0x93)
            MCKL_RANDOM_INTERNAL_PHILOX_SSE2_4X32_RBOX(xmmk4, 0x93)
            MCKL_RANDOM_INTERNAL_PHILOX_SSE2_4X32_RBOX(xmmk5, 0x93)
            MCKL_RANDOM_INTERNAL_PHILOX_SSE2_4X32_RBOX(xmmk6, 0x93)
            MCKL_RANDOM_INTERNAL_PHILOX_SSE2_4X32_RBOX(xmmk7, 0x93)
            MCKL_RANDOM_INTERNAL_PHILOX_SSE2_4X32_RBOX(xmmk8, 0x93)
            MCKL_RANDOM_INTERNAL_PHILOX_SSE2_4X32_RBOX(xmmk9, 0xB1)

            _mm_storeu_si128(reinterpret_cast<__m128i *>(r + M * 0), xmms0);
            _mm_storeu_si128(reinterpret_cast<__m128i *>(r + M * 1), xmms1);
            _mm_storeu_si128(reinterpret_cast<__m128i *>(r + M * 2), xmms2);
            _mm_storeu_si128(reinterpret_cast<__m128i *>(r + M * 3), xmms3);
            _mm_storeu_si128(reinterpret_cast<__m128i *>(r + M * 4), xmms4);
            _mm_storeu_si128(reinterpret_cast<__m128i *>(r + M * 5), xmms5);
            _mm_storeu_si128(reinterpret_cast<__m128i *>(r + M * 6), xmms6);
            _mm_storeu_si128(reinterpret_cast<__m128i *>(r + M * 7), xmms7);

            n -= N;
            r += N * R;
        }

        MCKL_NOINLINE_CALL Philox4xGeneratorGenericImpl<T, Constants>::eval(
            ctr, n, r, key);
    }
}; // class Philox4x32GeneratorSSE2Impl

} // namespace mckl::internal

} // namespace mckl

#ifdef MCKL_GCC
#if MCKL_GCC_VERSION >= 60000
#pragma GCC diagnostic pop
#endif
#endif

#endif // MCKL_RANDOM_INTERNAL_PHILOX_SSE2_4X32_HPP
