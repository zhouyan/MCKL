//============================================================================
// MCKL/include/mckl/random/internal/threefry_sse2_2x32.hpp
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

#ifndef MCKL_RANDOM_INTERNAL_THREEFRY_SSE2_2X32_HPP
#define MCKL_RANDOM_INTERNAL_THREEFRY_SSE2_2X32_HPP

#include <mckl/random/internal/common.hpp>
#include <mckl/random/internal/threefry_common.hpp>
#include <mckl/random/internal/threefry_constants.hpp>
#include <mckl/random/internal/threefry_generic_2x32.hpp>

#ifdef MCKL_GCC
#if MCKL_GCC_VERSION >= 60000
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wignored-attributes"
#endif
#endif

#define MCKL_RANDOM_INTERNAL_THREEFRY_SSE2_2X32_KBOX(N)                       \
    xmmt0 = _mm_set1_epi32(                                                   \
        static_cast<int>(ThreefryKBox<T, K, N>::template key<0>(par)));       \
    xmmt1 = _mm_set1_epi32(                                                   \
        static_cast<int>(ThreefryKBox<T, K, N>::template key<1>(par)));       \
    xmms0 = _mm_add_epi32(xmms0, xmmt0);                                      \
    xmms1 = _mm_add_epi32(xmms1, xmmt1);                                      \
    xmms2 = _mm_add_epi32(xmms2, xmmt0);                                      \
    xmms3 = _mm_add_epi32(xmms3, xmmt1);                                      \
    xmms4 = _mm_add_epi32(xmms4, xmmt0);                                      \
    xmms5 = _mm_add_epi32(xmms5, xmmt1);                                      \
    xmms6 = _mm_add_epi32(xmms6, xmmt0);                                      \
    xmms7 = _mm_add_epi32(xmms7, xmmt1);

#define MCKL_RANDOM_INTERNAL_THREEFRY_SSE2_2X32_RBOX(N)                       \
    {                                                                         \
        constexpr int L0 = Constants::rotate::value[0][(N - 1) % 8];          \
        constexpr int R0 = 32 - L0;                                           \
        xmms0 = _mm_add_epi32(xmms0, xmms1);                                  \
        xmms2 = _mm_add_epi32(xmms2, xmms3);                                  \
        xmms4 = _mm_add_epi32(xmms4, xmms5);                                  \
        xmms6 = _mm_add_epi32(xmms6, xmms7);                                  \
        xmmt1 = _mm_slli_epi32(xmms1, L0);                                    \
        xmmt3 = _mm_slli_epi32(xmms3, L0);                                    \
        xmmt5 = _mm_slli_epi32(xmms5, L0);                                    \
        xmmt7 = _mm_slli_epi32(xmms7, L0);                                    \
        xmms1 = _mm_srli_epi32(xmms1, R0);                                    \
        xmms3 = _mm_srli_epi32(xmms3, R0);                                    \
        xmms5 = _mm_srli_epi32(xmms5, R0);                                    \
        xmms7 = _mm_srli_epi32(xmms7, R0);                                    \
        xmmt1 = _mm_or_si128(xmms1, xmmt1);                                   \
        xmmt3 = _mm_or_si128(xmms3, xmmt3);                                   \
        xmmt5 = _mm_or_si128(xmms5, xmmt5);                                   \
        xmmt7 = _mm_or_si128(xmms7, xmmt7);                                   \
        xmms1 = _mm_xor_si128(xmms0, xmmt1);                                  \
        xmms3 = _mm_xor_si128(xmms2, xmmt3);                                  \
        xmms5 = _mm_xor_si128(xmms4, xmmt5);                                  \
        xmms7 = _mm_xor_si128(xmms6, xmmt7);                                  \
    }

#define MCKL_RANDOM_INTERNAL_THREEFRY_SSE2_2X32_CYCLE_4(N)                    \
    MCKL_RANDOM_INTERNAL_THREEFRY_SSE2_2X32_RBOX(N * 8 + 1);                  \
    MCKL_RANDOM_INTERNAL_THREEFRY_SSE2_2X32_RBOX(N * 8 + 2);                  \
    MCKL_RANDOM_INTERNAL_THREEFRY_SSE2_2X32_RBOX(N * 8 + 3);                  \
    MCKL_RANDOM_INTERNAL_THREEFRY_SSE2_2X32_RBOX(N * 8 + 4);                  \
    MCKL_RANDOM_INTERNAL_THREEFRY_SSE2_2X32_KBOX(N * 8 + 4);

#define MCKL_RANDOM_INTERNAL_THREEFRY_SSE2_2X32_CYCLE_8(N)                    \
    MCKL_RANDOM_INTERNAL_THREEFRY_SSE2_2X32_RBOX(N * 8 + 1);                  \
    MCKL_RANDOM_INTERNAL_THREEFRY_SSE2_2X32_RBOX(N * 8 + 2);                  \
    MCKL_RANDOM_INTERNAL_THREEFRY_SSE2_2X32_RBOX(N * 8 + 3);                  \
    MCKL_RANDOM_INTERNAL_THREEFRY_SSE2_2X32_RBOX(N * 8 + 4);                  \
    MCKL_RANDOM_INTERNAL_THREEFRY_SSE2_2X32_KBOX(N * 8 + 4);                  \
    MCKL_RANDOM_INTERNAL_THREEFRY_SSE2_2X32_RBOX(N * 8 + 5);                  \
    MCKL_RANDOM_INTERNAL_THREEFRY_SSE2_2X32_RBOX(N * 8 + 6);                  \
    MCKL_RANDOM_INTERNAL_THREEFRY_SSE2_2X32_RBOX(N * 8 + 7);                  \
    MCKL_RANDOM_INTERNAL_THREEFRY_SSE2_2X32_RBOX(N * 8 + 8);                  \
    MCKL_RANDOM_INTERNAL_THREEFRY_SSE2_2X32_KBOX(N * 8 + 8);

namespace mckl
{

namespace internal
{

template <typename T>
class Threefry2x32GeneratorSSE2Impl
{
    static_assert(std::numeric_limits<T>::digits == 32,
        "**Threefry2x32GeneratorSSE2Impl** used with T other than a "
        "32-bit unsigned integers");

    static constexpr std::size_t K = 2;
    static constexpr std::size_t Rounds = 20;
    using Constants = ThreefryConstants<T, 2>;

    public:
    static void eval(
        const void *plain, void *cipher, const std::array<T, K + 4> &par)
    {
        Threefry2x32GeneratorGenericImpl<T>::eval(plain, cipher, par);
    }

    template <typename ResultType>
    static void eval(std::array<std::uint64_t, 1> &ctr, ResultType *r,
        const std::array<T, K + 4> &par)
    {
        Threefry2x32GeneratorGenericImpl<T>::eval(ctr, r, par);
    }

    template <typename ResultType>
    static void eval(std::array<std::uint64_t, 1> &ctr, std::size_t n,
        ResultType *r, const std::array<T, K + 4> &par)
    {
        if (ctr.front() >= std::numeric_limits<std::uint64_t>::max() - n) {
            MCKL_NOINLINE_CALL Threefry2x32GeneratorGenericImpl<T>::eval(
                ctr, n, r, par);
            return;
        }

        constexpr std::size_t S = 8;
        constexpr std::size_t N = sizeof(__m128i) * S / (sizeof(T) * K);
        constexpr std::size_t M = sizeof(__m128i) / sizeof(ResultType);
        constexpr std::size_t R = sizeof(T) * K / sizeof(ResultType);

        __m128i xmmc =
            _mm_set1_epi64x(static_cast<MCKL_INT64>(std::get<0>(ctr)));
        ctr.front() += n / N * N;

        while (n >= N) {
            __m128i xmms0 = _mm_add_epi64(xmmc, _mm_set_epi64x(0x02, 0x01));
            __m128i xmms1 = _mm_add_epi64(xmmc, _mm_set_epi64x(0x04, 0x03));
            __m128i xmms2 = _mm_add_epi64(xmmc, _mm_set_epi64x(0x06, 0x05));
            __m128i xmms3 = _mm_add_epi64(xmmc, _mm_set_epi64x(0x08, 0x07));
            __m128i xmms4 = _mm_add_epi64(xmmc, _mm_set_epi64x(0x0A, 0x09));
            __m128i xmms5 = _mm_add_epi64(xmmc, _mm_set_epi64x(0x0C, 0x0B));
            __m128i xmms6 = _mm_add_epi64(xmmc, _mm_set_epi64x(0x0E, 0x0D));
            __m128i xmms7 = _mm_add_epi64(xmmc, _mm_set_epi64x(0x10, 0x0F));

            xmmc = _mm_add_epi64(
                xmmc, _mm_set1_epi64x(static_cast<MCKL_INT64>(N)));

            __m128i xmmt0;
            __m128i xmmt1;
            __m128i xmmt2;
            __m128i xmmt3;
            __m128i xmmt4;
            __m128i xmmt5;
            __m128i xmmt6;
            __m128i xmmt7;

            xmmt1 = xmms1;
            xmms1 = xmms2;
            xmms2 = xmms4;
            xmms4 = xmmt1;

            xmmt3 = xmms3;
            xmms3 = xmms6;
            xmms6 = xmms5;
            xmms5 = xmmt3;

            xmmt0 = _mm_unpacklo_epi32(xmms0, xmms1);
            xmmt1 = _mm_unpacklo_epi32(xmms2, xmms3);
            xmmt2 = _mm_unpackhi_epi32(xmms0, xmms1);
            xmmt3 = _mm_unpackhi_epi32(xmms2, xmms3);
            xmmt4 = _mm_unpacklo_epi32(xmms4, xmms5);
            xmmt5 = _mm_unpacklo_epi32(xmms6, xmms7);
            xmmt6 = _mm_unpackhi_epi32(xmms4, xmms5);
            xmmt7 = _mm_unpackhi_epi32(xmms6, xmms7);
            xmms0 = _mm_unpacklo_epi64(xmmt0, xmmt1);
            xmms1 = _mm_unpackhi_epi64(xmmt0, xmmt1);
            xmms2 = _mm_unpacklo_epi64(xmmt2, xmmt3);
            xmms3 = _mm_unpackhi_epi64(xmmt2, xmmt3);
            xmms4 = _mm_unpacklo_epi64(xmmt4, xmmt5);
            xmms5 = _mm_unpackhi_epi64(xmmt4, xmmt5);
            xmms6 = _mm_unpacklo_epi64(xmmt6, xmmt7);
            xmms7 = _mm_unpackhi_epi64(xmmt6, xmmt7);

            MCKL_RANDOM_INTERNAL_THREEFRY_SSE2_2X32_KBOX(0)
            MCKL_RANDOM_INTERNAL_THREEFRY_SSE2_2X32_CYCLE_8(0)
            MCKL_RANDOM_INTERNAL_THREEFRY_SSE2_2X32_CYCLE_8(1)
            MCKL_RANDOM_INTERNAL_THREEFRY_SSE2_2X32_CYCLE_4(2)

            xmmt0 = _mm_unpacklo_epi32(xmms0, xmms1);
            xmmt1 = _mm_unpacklo_epi32(xmms2, xmms3);
            xmmt2 = _mm_unpackhi_epi32(xmms0, xmms1);
            xmmt3 = _mm_unpackhi_epi32(xmms2, xmms3);
            xmmt4 = _mm_unpacklo_epi32(xmms4, xmms5);
            xmmt5 = _mm_unpacklo_epi32(xmms6, xmms7);
            xmmt6 = _mm_unpackhi_epi32(xmms4, xmms5);
            xmmt7 = _mm_unpackhi_epi32(xmms6, xmms7);
            xmms0 = _mm_unpacklo_epi64(xmmt0, xmmt1);
            xmms1 = _mm_unpackhi_epi64(xmmt0, xmmt1);
            xmms2 = _mm_unpacklo_epi64(xmmt2, xmmt3);
            xmms3 = _mm_unpackhi_epi64(xmmt2, xmmt3);
            xmms4 = _mm_unpacklo_epi64(xmmt4, xmmt5);
            xmms5 = _mm_unpackhi_epi64(xmmt4, xmmt5);
            xmms6 = _mm_unpacklo_epi64(xmmt6, xmmt7);
            xmms7 = _mm_unpackhi_epi64(xmmt6, xmmt7);

            xmmt2 = xmms2;
            xmms2 = xmms1;
            xmms1 = xmms4;
            xmms4 = xmmt2;

            xmmt6 = xmms6;
            xmms6 = xmms3;
            xmms3 = xmms5;
            xmms5 = xmmt6;

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

        MCKL_NOINLINE_CALL Threefry2x32GeneratorGenericImpl<T>::eval(
            ctr, n, r, par);
    }
}; // class Threefry2x32GeneratorSSE2Impl

} // namespace mckl::internal

} // namespace mckl

#ifdef MCKL_GCC
#if MCKL_GCC_VERSION >= 60000
#pragma GCC diagnostic pop
#endif
#endif

#endif // MCKL_RANDOM_INTERNAL_THREEFRY_SSE2_2X32_HPP
