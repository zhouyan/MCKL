//============================================================================
// MCKL/include/mckl/random/internal/threefry_sse2_16x64.hpp
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

#ifndef MCKL_RANDOM_INTERNAL_THREEFRY_SSE2_16X64_HPP
#define MCKL_RANDOM_INTERNAL_THREEFRY_SSE2_16X64_HPP

#include <mckl/random/internal/common.hpp>
#include <mckl/random/internal/threefry_common.hpp>
#include <mckl/random/internal/threefry_constants.hpp>
#include <mckl/random/internal/threefry_generic_16x64.hpp>
#include <mckl/random/increment.hpp>

#ifdef MCKL_GCC
#if MCKL_GCC_VERSION >= 60000
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wignored-attributes"
#endif
#endif

#define MCKL_RANDOM_INTERNAL_THREEFRY_SSE2_16X64_KBOX(N)                      \
    xmmt0 = _mm_set1_epi64x(static_cast<MCKL_INT64>(                          \
        ThreefryKBox<T, K, N>::template key<0x0>(par)));                      \
    xmmt1 = _mm_set1_epi64x(static_cast<MCKL_INT64>(                          \
        ThreefryKBox<T, K, N>::template key<0x1>(par)));                      \
    xmmt2 = _mm_set1_epi64x(static_cast<MCKL_INT64>(                          \
        ThreefryKBox<T, K, N>::template key<0x2>(par)));                      \
    xmmt3 = _mm_set1_epi64x(static_cast<MCKL_INT64>(                          \
        ThreefryKBox<T, K, N>::template key<0x3>(par)));                      \
    xmmt4 = _mm_set1_epi64x(static_cast<MCKL_INT64>(                          \
        ThreefryKBox<T, K, N>::template key<0x4>(par)));                      \
    xmmt5 = _mm_set1_epi64x(static_cast<MCKL_INT64>(                          \
        ThreefryKBox<T, K, N>::template key<0x5>(par)));                      \
    xmmt6 = _mm_set1_epi64x(static_cast<MCKL_INT64>(                          \
        ThreefryKBox<T, K, N>::template key<0x6>(par)));                      \
    xmmt7 = _mm_set1_epi64x(static_cast<MCKL_INT64>(                          \
        ThreefryKBox<T, K, N>::template key<0x7>(par)));                      \
    xmmt8 = _mm_set1_epi64x(static_cast<MCKL_INT64>(                          \
        ThreefryKBox<T, K, N>::template key<0x8>(par)));                      \
    xmmt9 = _mm_set1_epi64x(static_cast<MCKL_INT64>(                          \
        ThreefryKBox<T, K, N>::template key<0x9>(par)));                      \
    xmmtA = _mm_set1_epi64x(static_cast<MCKL_INT64>(                          \
        ThreefryKBox<T, K, N>::template key<0xA>(par)));                      \
    xmmtB = _mm_set1_epi64x(static_cast<MCKL_INT64>(                          \
        ThreefryKBox<T, K, N>::template key<0xB>(par)));                      \
    xmmtC = _mm_set1_epi64x(static_cast<MCKL_INT64>(                          \
        ThreefryKBox<T, K, N>::template key<0xC>(par)));                      \
    xmmtD = _mm_set1_epi64x(static_cast<MCKL_INT64>(                          \
        ThreefryKBox<T, K, N>::template key<0xD>(par)));                      \
    xmmtE = _mm_set1_epi64x(static_cast<MCKL_INT64>(                          \
        ThreefryKBox<T, K, N>::template key<0xE>(par)));                      \
    xmmtF = _mm_set1_epi64x(static_cast<MCKL_INT64>(                          \
        ThreefryKBox<T, K, N>::template key<0xF>(par)));                      \
    xmms0 = _mm_add_epi64(xmms0, xmmt0);                                      \
    xmms1 = _mm_add_epi64(xmms1, xmmt1);                                      \
    xmms2 = _mm_add_epi64(xmms2, xmmt2);                                      \
    xmms3 = _mm_add_epi64(xmms3, xmmt3);                                      \
    xmms4 = _mm_add_epi64(xmms4, xmmt4);                                      \
    xmms5 = _mm_add_epi64(xmms5, xmmt5);                                      \
    xmms6 = _mm_add_epi64(xmms6, xmmt6);                                      \
    xmms7 = _mm_add_epi64(xmms7, xmmt7);                                      \
    xmms8 = _mm_add_epi64(xmms8, xmmt8);                                      \
    xmms9 = _mm_add_epi64(xmms9, xmmt9);                                      \
    xmmsA = _mm_add_epi64(xmmsA, xmmtA);                                      \
    xmmsB = _mm_add_epi64(xmmsB, xmmtB);                                      \
    xmmsC = _mm_add_epi64(xmmsC, xmmtC);                                      \
    xmmsD = _mm_add_epi64(xmmsD, xmmtD);                                      \
    xmmsE = _mm_add_epi64(xmmsE, xmmtE);                                      \
    xmmsF = _mm_add_epi64(xmmsF, xmmtF);

#define MCKL_RANDOM_INTERNAL_THREEFRY_SSE2_16X64_RBOX(N)                      \
    {                                                                         \
        constexpr int L0 = Constants::rotate::value[0][(N - 1) % 8];          \
        constexpr int L1 = Constants::rotate::value[1][(N - 1) % 8];          \
        constexpr int L2 = Constants::rotate::value[2][(N - 1) % 8];          \
        constexpr int L3 = Constants::rotate::value[3][(N - 1) % 8];          \
        constexpr int L4 = Constants::rotate::value[4][(N - 1) % 8];          \
        constexpr int L5 = Constants::rotate::value[5][(N - 1) % 8];          \
        constexpr int L6 = Constants::rotate::value[6][(N - 1) % 8];          \
        constexpr int L7 = Constants::rotate::value[7][(N - 1) % 8];          \
        constexpr int R0 = 64 - L0;                                           \
        constexpr int R1 = 64 - L1;                                           \
        constexpr int R2 = 64 - L2;                                           \
        constexpr int R3 = 64 - L3;                                           \
        constexpr int R4 = 64 - L4;                                           \
        constexpr int R5 = 64 - L5;                                           \
        constexpr int R6 = 64 - L6;                                           \
        constexpr int R7 = 64 - L7;                                           \
        xmmt0 = _mm_add_epi64(xmms0, xmms1);                                  \
        xmmt2 = _mm_add_epi64(xmms2, xmms3);                                  \
        xmmt4 = _mm_add_epi64(xmms4, xmms5);                                  \
        xmmt6 = _mm_add_epi64(xmms6, xmms7);                                  \
        xmmt8 = _mm_add_epi64(xmms8, xmms9);                                  \
        xmmtA = _mm_add_epi64(xmmsA, xmmsB);                                  \
        xmmtC = _mm_add_epi64(xmmsC, xmmsD);                                  \
        xmmtE = _mm_add_epi64(xmmsE, xmmsF);                                  \
        xmmt1 = _mm_slli_epi64(xmms1, L0);                                    \
        xmmt3 = _mm_slli_epi64(xmms3, L1);                                    \
        xmmt5 = _mm_slli_epi64(xmms5, L2);                                    \
        xmmt7 = _mm_slli_epi64(xmms7, L3);                                    \
        xmmt9 = _mm_slli_epi64(xmms9, L4);                                    \
        xmmtB = _mm_slli_epi64(xmmsB, L5);                                    \
        xmmtD = _mm_slli_epi64(xmmsD, L6);                                    \
        xmmtF = _mm_slli_epi64(xmmsF, L7);                                    \
        xmms1 = _mm_srli_epi64(xmms1, R0);                                    \
        xmms3 = _mm_srli_epi64(xmms3, R1);                                    \
        xmms5 = _mm_srli_epi64(xmms5, R2);                                    \
        xmms7 = _mm_srli_epi64(xmms7, R3);                                    \
        xmms9 = _mm_srli_epi64(xmms9, R4);                                    \
        xmmsB = _mm_srli_epi64(xmmsB, R5);                                    \
        xmmsD = _mm_srli_epi64(xmmsD, R6);                                    \
        xmmsF = _mm_srli_epi64(xmmsF, R7);                                    \
        xmmt1 = _mm_or_si128(xmms1, xmmt1);                                   \
        xmmt3 = _mm_or_si128(xmms3, xmmt3);                                   \
        xmmt5 = _mm_or_si128(xmms5, xmmt5);                                   \
        xmmt7 = _mm_or_si128(xmms7, xmmt7);                                   \
        xmmt9 = _mm_or_si128(xmms9, xmmt9);                                   \
        xmmtB = _mm_or_si128(xmmsB, xmmtB);                                   \
        xmmtD = _mm_or_si128(xmmsD, xmmtD);                                   \
        xmmtF = _mm_or_si128(xmmsF, xmmtF);                                   \
        xmms0 = xmmt0;                                                        \
        xmms2 = xmmt2;                                                        \
        xmms4 = xmmt6;                                                        \
        xmms6 = xmmt4;                                                        \
        xmms8 = xmmtA;                                                        \
        xmmsA = xmmtC;                                                        \
        xmmsC = xmmtE;                                                        \
        xmmsE = xmmt8;                                                        \
        xmms1 = _mm_xor_si128(xmmt8, xmmt9);                                  \
        xmms3 = _mm_xor_si128(xmmtC, xmmtD);                                  \
        xmms5 = _mm_xor_si128(xmmtA, xmmtB);                                  \
        xmms7 = _mm_xor_si128(xmmtE, xmmtF);                                  \
        xmms9 = _mm_xor_si128(xmmt6, xmmt7);                                  \
        xmmsB = _mm_xor_si128(xmmt2, xmmt3);                                  \
        xmmsD = _mm_xor_si128(xmmt4, xmmt5);                                  \
        xmmsF = _mm_xor_si128(xmmt0, xmmt1);                                  \
    }

#define MCKL_RANDOM_INTERNAL_THREEFRY_SSE2_16X64_CYCLE_4(N)                   \
    MCKL_RANDOM_INTERNAL_THREEFRY_SSE2_16X64_RBOX(N * 8 + 1);                 \
    MCKL_RANDOM_INTERNAL_THREEFRY_SSE2_16X64_RBOX(N * 8 + 2);                 \
    MCKL_RANDOM_INTERNAL_THREEFRY_SSE2_16X64_RBOX(N * 8 + 3);                 \
    MCKL_RANDOM_INTERNAL_THREEFRY_SSE2_16X64_RBOX(N * 8 + 4);                 \
    MCKL_RANDOM_INTERNAL_THREEFRY_SSE2_16X64_KBOX(N * 8 + 4);

#define MCKL_RANDOM_INTERNAL_THREEFRY_SSE2_16X64_CYCLE_8(N)                   \
    MCKL_RANDOM_INTERNAL_THREEFRY_SSE2_16X64_RBOX(N * 8 + 1);                 \
    MCKL_RANDOM_INTERNAL_THREEFRY_SSE2_16X64_RBOX(N * 8 + 2);                 \
    MCKL_RANDOM_INTERNAL_THREEFRY_SSE2_16X64_RBOX(N * 8 + 3);                 \
    MCKL_RANDOM_INTERNAL_THREEFRY_SSE2_16X64_RBOX(N * 8 + 4);                 \
    MCKL_RANDOM_INTERNAL_THREEFRY_SSE2_16X64_KBOX(N * 8 + 4);                 \
    MCKL_RANDOM_INTERNAL_THREEFRY_SSE2_16X64_RBOX(N * 8 + 5);                 \
    MCKL_RANDOM_INTERNAL_THREEFRY_SSE2_16X64_RBOX(N * 8 + 6);                 \
    MCKL_RANDOM_INTERNAL_THREEFRY_SSE2_16X64_RBOX(N * 8 + 7);                 \
    MCKL_RANDOM_INTERNAL_THREEFRY_SSE2_16X64_RBOX(N * 8 + 8);                 \
    MCKL_RANDOM_INTERNAL_THREEFRY_SSE2_16X64_KBOX(N * 8 + 8);

namespace mckl
{

namespace internal
{

template <typename T>
class Threefry16x64GeneratorSSE2Impl
{
    static_assert(std::numeric_limits<T>::digits == 64,
        "**Threefry16x64GeneratorSSE2Impl** used with T other than a 64-bit "
        "unsigned integers");

    static constexpr std::size_t K = 16;
    static constexpr std::size_t Rounds = 20;
    using Constants = ThreefryConstants<T, 16>;

    public:
    static void eval(
        const void *plain, void *cipher, const std::array<T, K + 4> &par)
    {
        Threefry16x64GeneratorGenericImpl<T>::eval(plain, cipher, par);
    }

    template <typename ResultType>
    static void eval(std::array<std::uint64_t, 16> &ctr, ResultType *r,
        const std::array<T, K + 4> &par)
    {
        Threefry16x64GeneratorGenericImpl<T>::eval(ctr, r, par);
    }

    template <typename ResultType>
    static void eval(std::array<std::uint64_t, 16> &ctr, std::size_t n,
        ResultType *r, const std::array<T, K + 4> &par)
    {
        constexpr std::size_t R = sizeof(T) * K / sizeof(ResultType);

        const std::size_t n0 =
            static_cast<std::size_t>(std::min(static_cast<std::uint64_t>(n),
                std::numeric_limits<std::uint64_t>::max() - ctr.front()));

        eval_kernel(ctr, n0, r, par);
        n -= n0;
        r += n0 * R;

        if (n != 0) {
            eval(ctr, r, par);
            n -= 1;
            r += R;
        }

        eval_kernel(ctr, n, r, par);
    }

    private:
    template <typename ResultType>
    static void eval_kernel(std::array<std::uint64_t, 16> &ctr, std::size_t n,
        ResultType *r, const std::array<T, K + 4> &par)
    {
        constexpr std::size_t S = 16;
        constexpr std::size_t N = sizeof(__m128i) * S / (sizeof(T) * K);

        __m128i xmmc0 =
            _mm_set_epi64x(static_cast<MCKL_INT64>(std::get<0x1>(ctr)),
                static_cast<MCKL_INT64>(std::get<0x0>(ctr)));
        __m128i xmmc1 =
            _mm_set_epi64x(static_cast<MCKL_INT64>(std::get<0x3>(ctr)),
                static_cast<MCKL_INT64>(std::get<0x2>(ctr)));
        __m128i xmmc2 =
            _mm_set_epi64x(static_cast<MCKL_INT64>(std::get<0x5>(ctr)),
                static_cast<MCKL_INT64>(std::get<0x4>(ctr)));
        __m128i xmmc3 =
            _mm_set_epi64x(static_cast<MCKL_INT64>(std::get<0x7>(ctr)),
                static_cast<MCKL_INT64>(std::get<0x6>(ctr)));
        __m128i xmmc4 =
            _mm_set_epi64x(static_cast<MCKL_INT64>(std::get<0x9>(ctr)),
                static_cast<MCKL_INT64>(std::get<0x8>(ctr)));
        __m128i xmmc5 =
            _mm_set_epi64x(static_cast<MCKL_INT64>(std::get<0xB>(ctr)),
                static_cast<MCKL_INT64>(std::get<0xA>(ctr)));
        __m128i xmmc6 =
            _mm_set_epi64x(static_cast<MCKL_INT64>(std::get<0xD>(ctr)),
                static_cast<MCKL_INT64>(std::get<0xC>(ctr)));
        __m128i xmmc7 =
            _mm_set_epi64x(static_cast<MCKL_INT64>(std::get<0xF>(ctr)),
                static_cast<MCKL_INT64>(std::get<0xE>(ctr)));
        ctr.front() += n;

        __m128i *rptr = reinterpret_cast<__m128i *>(r);
        while (n != 0) {
            __m128i xmmt0 = _mm_add_epi64(xmmc0, _mm_set_epi64x(0, 1));
            __m128i xmmt8 = _mm_add_epi64(xmmc0, _mm_set_epi64x(0, 2));
            xmmc0 = xmmt8;

            __m128i xmmt1 = xmmc1;
            __m128i xmmt2 = xmmc2;
            __m128i xmmt3 = xmmc3;
            __m128i xmmt4 = xmmc4;
            __m128i xmmt5 = xmmc5;
            __m128i xmmt6 = xmmc6;
            __m128i xmmt7 = xmmc7;
            __m128i xmmt9 = xmmc1;
            __m128i xmmtA = xmmc2;
            __m128i xmmtB = xmmc3;
            __m128i xmmtC = xmmc4;
            __m128i xmmtD = xmmc5;
            __m128i xmmtE = xmmc6;
            __m128i xmmtF = xmmc7;

            __m128i xmms0;
            __m128i xmms1;
            __m128i xmms2;
            __m128i xmms3;
            __m128i xmms4;
            __m128i xmms5;
            __m128i xmms6;
            __m128i xmms7;
            __m128i xmms8;
            __m128i xmms9;
            __m128i xmmsA;
            __m128i xmmsB;
            __m128i xmmsC;
            __m128i xmmsD;
            __m128i xmmsE;
            __m128i xmmsF;

            xmms2 = xmmt2;
            xmmt2 = xmmt1;
            xmmt1 = xmmt8;
            xmmt8 = xmmt4;
            xmmt4 = xmms2;

            xmms6 = xmmt6;
            xmmt6 = xmmt3;
            xmmt3 = xmmt9;
            xmmt9 = xmmtC;
            xmmtC = xmms6;

            xmmsE = xmmtE;
            xmmtE = xmmt7;
            xmmt7 = xmmtB;
            xmmtB = xmmtD;
            xmmtD = xmmsE;

            xmms5 = xmmt5;
            xmmt5 = xmmtA;
            xmmtA = xmms5;

            xmms0 = _mm_unpacklo_epi64(xmmt0, xmmt1);
            xmms1 = _mm_unpackhi_epi64(xmmt0, xmmt1);
            xmms2 = _mm_unpacklo_epi64(xmmt2, xmmt3);
            xmms3 = _mm_unpackhi_epi64(xmmt2, xmmt3);
            xmms4 = _mm_unpacklo_epi64(xmmt4, xmmt5);
            xmms5 = _mm_unpackhi_epi64(xmmt4, xmmt5);
            xmms6 = _mm_unpacklo_epi64(xmmt6, xmmt7);
            xmms7 = _mm_unpackhi_epi64(xmmt6, xmmt7);
            xmms8 = _mm_unpacklo_epi64(xmmt8, xmmt9);
            xmms9 = _mm_unpackhi_epi64(xmmt8, xmmt9);
            xmmsA = _mm_unpacklo_epi64(xmmtA, xmmtB);
            xmmsB = _mm_unpackhi_epi64(xmmtA, xmmtB);
            xmmsC = _mm_unpacklo_epi64(xmmtC, xmmtD);
            xmmsD = _mm_unpackhi_epi64(xmmtC, xmmtD);
            xmmsE = _mm_unpacklo_epi64(xmmtE, xmmtF);
            xmmsF = _mm_unpackhi_epi64(xmmtE, xmmtF);

            MCKL_RANDOM_INTERNAL_THREEFRY_SSE2_16X64_KBOX(0);
            MCKL_RANDOM_INTERNAL_THREEFRY_SSE2_16X64_CYCLE_8(0);
            MCKL_RANDOM_INTERNAL_THREEFRY_SSE2_16X64_CYCLE_8(1);
            MCKL_RANDOM_INTERNAL_THREEFRY_SSE2_16X64_CYCLE_4(2);

            xmmt0 = _mm_unpacklo_epi64(xmms0, xmms1);
            xmmt1 = _mm_unpackhi_epi64(xmms0, xmms1);
            xmmt2 = _mm_unpacklo_epi64(xmms2, xmms3);
            xmmt3 = _mm_unpackhi_epi64(xmms2, xmms3);
            xmmt4 = _mm_unpacklo_epi64(xmms4, xmms5);
            xmmt5 = _mm_unpackhi_epi64(xmms4, xmms5);
            xmmt6 = _mm_unpacklo_epi64(xmms6, xmms7);
            xmmt7 = _mm_unpackhi_epi64(xmms6, xmms7);
            xmmt8 = _mm_unpacklo_epi64(xmms8, xmms9);
            xmmt9 = _mm_unpackhi_epi64(xmms8, xmms9);
            xmmtA = _mm_unpacklo_epi64(xmmsA, xmmsB);
            xmmtB = _mm_unpackhi_epi64(xmmsA, xmmsB);
            xmmtC = _mm_unpacklo_epi64(xmmsC, xmmsD);
            xmmtD = _mm_unpackhi_epi64(xmmsC, xmmsD);
            xmmtE = _mm_unpacklo_epi64(xmmsE, xmmsF);
            xmmtF = _mm_unpackhi_epi64(xmmsE, xmmsF);

            xmms1 = xmmt1;
            xmmt1 = xmmt2;
            xmmt2 = xmmt4;
            xmmt4 = xmmt8;
            xmmt8 = xmms1;

            xmms3 = xmmt3;
            xmmt3 = xmmt6;
            xmmt6 = xmmtC;
            xmmtC = xmmt9;
            xmmt9 = xmms3;

            xmms7 = xmmt7;
            xmmt7 = xmmtE;
            xmmtE = xmmtD;
            xmmtD = xmmtB;
            xmmtB = xmms7;

            xmms5 = xmmt5;
            xmmt5 = xmmtA;
            xmmtA = xmms5;

            if (n >= N) {
                n -= N;
                _mm_storeu_si128(rptr++, xmmt0);
                _mm_storeu_si128(rptr++, xmmt1);
                _mm_storeu_si128(rptr++, xmmt2);
                _mm_storeu_si128(rptr++, xmmt3);
                _mm_storeu_si128(rptr++, xmmt4);
                _mm_storeu_si128(rptr++, xmmt5);
                _mm_storeu_si128(rptr++, xmmt6);
                _mm_storeu_si128(rptr++, xmmt7);
                _mm_storeu_si128(rptr++, xmmt8);
                _mm_storeu_si128(rptr++, xmmt9);
                _mm_storeu_si128(rptr++, xmmtA);
                _mm_storeu_si128(rptr++, xmmtB);
                _mm_storeu_si128(rptr++, xmmtC);
                _mm_storeu_si128(rptr++, xmmtD);
                _mm_storeu_si128(rptr++, xmmtE);
                _mm_storeu_si128(rptr++, xmmtF);
            } else {
                std::array<__m128i, S> s;
                std::get<0x0>(s) = xmmt0;
                std::get<0x1>(s) = xmmt1;
                std::get<0x2>(s) = xmmt2;
                std::get<0x3>(s) = xmmt3;
                std::get<0x4>(s) = xmmt4;
                std::get<0x5>(s) = xmmt5;
                std::get<0x6>(s) = xmmt6;
                std::get<0x7>(s) = xmmt7;
                std::get<0x8>(s) = xmmt8;
                std::get<0x9>(s) = xmmt9;
                std::get<0xA>(s) = xmmtA;
                std::get<0xB>(s) = xmmtB;
                std::get<0xC>(s) = xmmtC;
                std::get<0xD>(s) = xmmtD;
                std::get<0xE>(s) = xmmtE;
                std::get<0xF>(s) = xmmtF;
                std::memcpy(rptr, s.data(), n * sizeof(T) * K);
                break;
            }
        }
    }
}; // class Threefry16x64GeneratorSSE2Impl

template <typename T>
class Threefish1024GeneratorSSE2Impl
{
    static_assert(std::numeric_limits<T>::digits == 64,
        "**Threefish1024GeneratorSSE2Impl** used with T other than a 64-bit "
        "unsigned integers");

    static constexpr std::size_t K = 16;
    static constexpr std::size_t Rounds = 80;
    using Constants = ThreefryConstants<T, 16>;

    public:
    static void eval(
        const void *plain, void *cipher, const std::array<T, K + 4> &par)
    {
        Threefish1024GeneratorGenericImpl<T>::eval(plain, cipher, par);
    }

    template <typename ResultType>
    static void eval(std::array<std::uint64_t, 16> &ctr, ResultType *r,
        const std::array<T, K + 4> &par)
    {
        Threefish1024GeneratorGenericImpl<T>::eval(ctr, r, par);
    }

    template <typename ResultType>
    static void eval(std::array<std::uint64_t, 16> &ctr, std::size_t n,
        ResultType *r, const std::array<T, K + 4> &par)
    {
        constexpr std::size_t R = sizeof(T) * K / sizeof(ResultType);

        const std::size_t n0 =
            static_cast<std::size_t>(std::min(static_cast<std::uint64_t>(n),
                std::numeric_limits<std::uint64_t>::max() - ctr.front()));

        eval_kernel(ctr, n0, r, par);
        n -= n0;
        r += n0 * R;

        if (n != 0) {
            eval(ctr, r, par);
            n -= 1;
            r += R;
        }

        eval_kernel(ctr, n, r, par);
    }

    private:
    template <typename ResultType>
    static void eval_kernel(std::array<std::uint64_t, 16> &ctr, std::size_t n,
        ResultType *r, const std::array<T, K + 4> &par)
    {
        constexpr std::size_t S = 16;
        constexpr std::size_t N = sizeof(__m128i) * S / (sizeof(T) * K);

        __m128i xmmc0 =
            _mm_set_epi64x(static_cast<MCKL_INT64>(std::get<0x1>(ctr)),
                static_cast<MCKL_INT64>(std::get<0x0>(ctr)));
        __m128i xmmc1 =
            _mm_set_epi64x(static_cast<MCKL_INT64>(std::get<0x3>(ctr)),
                static_cast<MCKL_INT64>(std::get<0x2>(ctr)));
        __m128i xmmc2 =
            _mm_set_epi64x(static_cast<MCKL_INT64>(std::get<0x5>(ctr)),
                static_cast<MCKL_INT64>(std::get<0x4>(ctr)));
        __m128i xmmc3 =
            _mm_set_epi64x(static_cast<MCKL_INT64>(std::get<0x7>(ctr)),
                static_cast<MCKL_INT64>(std::get<0x6>(ctr)));
        __m128i xmmc4 =
            _mm_set_epi64x(static_cast<MCKL_INT64>(std::get<0x9>(ctr)),
                static_cast<MCKL_INT64>(std::get<0x8>(ctr)));
        __m128i xmmc5 =
            _mm_set_epi64x(static_cast<MCKL_INT64>(std::get<0xB>(ctr)),
                static_cast<MCKL_INT64>(std::get<0xA>(ctr)));
        __m128i xmmc6 =
            _mm_set_epi64x(static_cast<MCKL_INT64>(std::get<0xD>(ctr)),
                static_cast<MCKL_INT64>(std::get<0xC>(ctr)));
        __m128i xmmc7 =
            _mm_set_epi64x(static_cast<MCKL_INT64>(std::get<0xF>(ctr)),
                static_cast<MCKL_INT64>(std::get<0xE>(ctr)));
        ctr.front() += n;

        __m128i *rptr = reinterpret_cast<__m128i *>(r);
        while (n != 0) {
            __m128i xmmt0 = _mm_add_epi64(xmmc0, _mm_set_epi64x(0, 1));
            __m128i xmmt8 = _mm_add_epi64(xmmc0, _mm_set_epi64x(0, 2));
            xmmc0 = xmmt8;

            __m128i xmmt1 = xmmc1;
            __m128i xmmt2 = xmmc2;
            __m128i xmmt3 = xmmc3;
            __m128i xmmt4 = xmmc4;
            __m128i xmmt5 = xmmc5;
            __m128i xmmt6 = xmmc6;
            __m128i xmmt7 = xmmc7;
            __m128i xmmt9 = xmmc1;
            __m128i xmmtA = xmmc2;
            __m128i xmmtB = xmmc3;
            __m128i xmmtC = xmmc4;
            __m128i xmmtD = xmmc5;
            __m128i xmmtE = xmmc6;
            __m128i xmmtF = xmmc7;

            __m128i xmms0;
            __m128i xmms1;
            __m128i xmms2;
            __m128i xmms3;
            __m128i xmms4;
            __m128i xmms5;
            __m128i xmms6;
            __m128i xmms7;
            __m128i xmms8;
            __m128i xmms9;
            __m128i xmmsA;
            __m128i xmmsB;
            __m128i xmmsC;
            __m128i xmmsD;
            __m128i xmmsE;
            __m128i xmmsF;

            xmms2 = xmmt2;
            xmmt2 = xmmt1;
            xmmt1 = xmmt8;
            xmmt8 = xmmt4;
            xmmt4 = xmms2;

            xmms6 = xmmt6;
            xmmt6 = xmmt3;
            xmmt3 = xmmt9;
            xmmt9 = xmmtC;
            xmmtC = xmms6;

            xmmsE = xmmtE;
            xmmtE = xmmt7;
            xmmt7 = xmmtB;
            xmmtB = xmmtD;
            xmmtD = xmmsE;

            xmms5 = xmmt5;
            xmmt5 = xmmtA;
            xmmtA = xmms5;

            xmms0 = _mm_unpacklo_epi64(xmmt0, xmmt1);
            xmms1 = _mm_unpackhi_epi64(xmmt0, xmmt1);
            xmms2 = _mm_unpacklo_epi64(xmmt2, xmmt3);
            xmms3 = _mm_unpackhi_epi64(xmmt2, xmmt3);
            xmms4 = _mm_unpacklo_epi64(xmmt4, xmmt5);
            xmms5 = _mm_unpackhi_epi64(xmmt4, xmmt5);
            xmms6 = _mm_unpacklo_epi64(xmmt6, xmmt7);
            xmms7 = _mm_unpackhi_epi64(xmmt6, xmmt7);
            xmms8 = _mm_unpacklo_epi64(xmmt8, xmmt9);
            xmms9 = _mm_unpackhi_epi64(xmmt8, xmmt9);
            xmmsA = _mm_unpacklo_epi64(xmmtA, xmmtB);
            xmmsB = _mm_unpackhi_epi64(xmmtA, xmmtB);
            xmmsC = _mm_unpacklo_epi64(xmmtC, xmmtD);
            xmmsD = _mm_unpackhi_epi64(xmmtC, xmmtD);
            xmmsE = _mm_unpacklo_epi64(xmmtE, xmmtF);
            xmmsF = _mm_unpackhi_epi64(xmmtE, xmmtF);

            MCKL_RANDOM_INTERNAL_THREEFRY_SSE2_16X64_KBOX(0);
            MCKL_RANDOM_INTERNAL_THREEFRY_SSE2_16X64_CYCLE_8(0);
            MCKL_RANDOM_INTERNAL_THREEFRY_SSE2_16X64_CYCLE_8(1);
            MCKL_RANDOM_INTERNAL_THREEFRY_SSE2_16X64_CYCLE_8(2);
            MCKL_RANDOM_INTERNAL_THREEFRY_SSE2_16X64_CYCLE_8(3);
            MCKL_RANDOM_INTERNAL_THREEFRY_SSE2_16X64_CYCLE_8(4);
            MCKL_RANDOM_INTERNAL_THREEFRY_SSE2_16X64_CYCLE_8(5);
            MCKL_RANDOM_INTERNAL_THREEFRY_SSE2_16X64_CYCLE_8(6);
            MCKL_RANDOM_INTERNAL_THREEFRY_SSE2_16X64_CYCLE_8(7);
            MCKL_RANDOM_INTERNAL_THREEFRY_SSE2_16X64_CYCLE_8(8);
            MCKL_RANDOM_INTERNAL_THREEFRY_SSE2_16X64_CYCLE_8(9);

            xmmt0 = _mm_unpacklo_epi64(xmms0, xmms1);
            xmmt1 = _mm_unpackhi_epi64(xmms0, xmms1);
            xmmt2 = _mm_unpacklo_epi64(xmms2, xmms3);
            xmmt3 = _mm_unpackhi_epi64(xmms2, xmms3);
            xmmt4 = _mm_unpacklo_epi64(xmms4, xmms5);
            xmmt5 = _mm_unpackhi_epi64(xmms4, xmms5);
            xmmt6 = _mm_unpacklo_epi64(xmms6, xmms7);
            xmmt7 = _mm_unpackhi_epi64(xmms6, xmms7);
            xmmt8 = _mm_unpacklo_epi64(xmms8, xmms9);
            xmmt9 = _mm_unpackhi_epi64(xmms8, xmms9);
            xmmtA = _mm_unpacklo_epi64(xmmsA, xmmsB);
            xmmtB = _mm_unpackhi_epi64(xmmsA, xmmsB);
            xmmtC = _mm_unpacklo_epi64(xmmsC, xmmsD);
            xmmtD = _mm_unpackhi_epi64(xmmsC, xmmsD);
            xmmtE = _mm_unpacklo_epi64(xmmsE, xmmsF);
            xmmtF = _mm_unpackhi_epi64(xmmsE, xmmsF);

            xmms1 = xmmt1;
            xmmt1 = xmmt2;
            xmmt2 = xmmt4;
            xmmt4 = xmmt8;
            xmmt8 = xmms1;

            xmms3 = xmmt3;
            xmmt3 = xmmt6;
            xmmt6 = xmmtC;
            xmmtC = xmmt9;
            xmmt9 = xmms3;

            xmms7 = xmmt7;
            xmmt7 = xmmtE;
            xmmtE = xmmtD;
            xmmtD = xmmtB;
            xmmtB = xmms7;

            xmms5 = xmmt5;
            xmmt5 = xmmtA;
            xmmtA = xmms5;

            if (n >= N) {
                n -= N;
                _mm_storeu_si128(rptr++, xmmt0);
                _mm_storeu_si128(rptr++, xmmt1);
                _mm_storeu_si128(rptr++, xmmt2);
                _mm_storeu_si128(rptr++, xmmt3);
                _mm_storeu_si128(rptr++, xmmt4);
                _mm_storeu_si128(rptr++, xmmt5);
                _mm_storeu_si128(rptr++, xmmt6);
                _mm_storeu_si128(rptr++, xmmt7);
                _mm_storeu_si128(rptr++, xmmt8);
                _mm_storeu_si128(rptr++, xmmt9);
                _mm_storeu_si128(rptr++, xmmtA);
                _mm_storeu_si128(rptr++, xmmtB);
                _mm_storeu_si128(rptr++, xmmtC);
                _mm_storeu_si128(rptr++, xmmtD);
                _mm_storeu_si128(rptr++, xmmtE);
                _mm_storeu_si128(rptr++, xmmtF);
            } else {
                std::array<__m128i, S> s;
                std::get<0x0>(s) = xmmt0;
                std::get<0x1>(s) = xmmt1;
                std::get<0x2>(s) = xmmt2;
                std::get<0x3>(s) = xmmt3;
                std::get<0x4>(s) = xmmt4;
                std::get<0x5>(s) = xmmt5;
                std::get<0x6>(s) = xmmt6;
                std::get<0x7>(s) = xmmt7;
                std::get<0x8>(s) = xmmt8;
                std::get<0x9>(s) = xmmt9;
                std::get<0xA>(s) = xmmtA;
                std::get<0xB>(s) = xmmtB;
                std::get<0xC>(s) = xmmtC;
                std::get<0xD>(s) = xmmtD;
                std::get<0xE>(s) = xmmtE;
                std::get<0xF>(s) = xmmtF;
                std::memcpy(rptr, s.data(), n * sizeof(T) * K);
                break;
            }
        }
    }
}; // class Threefish1024GeneratorSSE2Impl

} // namespace mckl::internal

} // namespace mckl

#ifdef MCKL_GCC
#if MCKL_GCC_VERSION >= 60000
#pragma GCC diagnostic pop
#endif
#endif

#endif // MCKL_RANDOM_INTERNAL_THREEFRY_SSE2_16X64_HPP
