//============================================================================
// MCKL/include/mckl/random/internal/threefry_sse2_4x32.hpp
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

#ifndef MCKL_RANDOM_INTERNAL_THREEFRY_SSE2_4X32_HPP
#define MCKL_RANDOM_INTERNAL_THREEFRY_SSE2_4X32_HPP

#include <mckl/random/internal/common.hpp>
#include <mckl/random/internal/threefry_common.hpp>
#include <mckl/random/internal/threefry_constants.hpp>
#include <mckl/random/internal/threefry_generic_4x32.hpp>

MCKL_PUSH_GCC_WARNING("-Wignored-attributes")

#define MCKL_RANDOM_INTERNAL_THREEFRY_SSE2_4X32_KBOX(N)                       \
    xmmt0 = _mm_set1_epi32(                                                   \
        static_cast<int>(ThreefryKBox<T, K, N>::template key<0>(par)));       \
    xmmt1 = _mm_set1_epi32(                                                   \
        static_cast<int>(ThreefryKBox<T, K, N>::template key<1>(par)));       \
    xmmt2 = _mm_set1_epi32(                                                   \
        static_cast<int>(ThreefryKBox<T, K, N>::template key<2>(par)));       \
    xmmt3 = _mm_set1_epi32(                                                   \
        static_cast<int>(ThreefryKBox<T, K, N>::template key<3>(par)));       \
    xmms0 = _mm_add_epi32(xmms0, xmmt0);                                      \
    xmms1 = _mm_add_epi32(xmms1, xmmt1);                                      \
    xmms2 = _mm_add_epi32(xmms2, xmmt2);                                      \
    xmms3 = _mm_add_epi32(xmms3, xmmt3);                                      \
    xmms4 = _mm_add_epi32(xmms4, xmmt0);                                      \
    xmms5 = _mm_add_epi32(xmms5, xmmt1);                                      \
    xmms6 = _mm_add_epi32(xmms6, xmmt2);                                      \
    xmms7 = _mm_add_epi32(xmms7, xmmt3);

#define MCKL_RANDOM_INTERNAL_THREEFRY_SSE2_4X32_RBOX(N)                       \
    {                                                                         \
        constexpr int L0 = Constants::rotate::value[0][(N - 1) % 8];          \
        constexpr int L1 = Constants::rotate::value[1][(N - 1) % 8];          \
        constexpr int R0 = 32 - L0;                                           \
        constexpr int R1 = 32 - L1;                                           \
        xmms0 = _mm_add_epi32(xmms0, xmms1);                                  \
        xmms2 = _mm_add_epi32(xmms2, xmms3);                                  \
        xmms4 = _mm_add_epi32(xmms4, xmms5);                                  \
        xmms6 = _mm_add_epi32(xmms6, xmms7);                                  \
        xmmt1 = _mm_slli_epi32(xmms1, L0);                                    \
        xmmt3 = _mm_slli_epi32(xmms3, L1);                                    \
        xmmt5 = _mm_slli_epi32(xmms5, L0);                                    \
        xmmt7 = _mm_slli_epi32(xmms7, L1);                                    \
        xmms1 = _mm_srli_epi32(xmms1, R0);                                    \
        xmms3 = _mm_srli_epi32(xmms3, R1);                                    \
        xmms5 = _mm_srli_epi32(xmms5, R0);                                    \
        xmms7 = _mm_srli_epi32(xmms7, R1);                                    \
        xmmt1 = _mm_or_si128(xmms1, xmmt1);                                   \
        xmmt3 = _mm_or_si128(xmms3, xmmt3);                                   \
        xmmt5 = _mm_or_si128(xmms5, xmmt5);                                   \
        xmmt7 = _mm_or_si128(xmms7, xmmt7);                                   \
        xmms1 = _mm_xor_si128(xmms2, xmmt3);                                  \
        xmms3 = _mm_xor_si128(xmms0, xmmt1);                                  \
        xmms5 = _mm_xor_si128(xmms6, xmmt7);                                  \
        xmms7 = _mm_xor_si128(xmms4, xmmt5);                                  \
    }

#define MCKL_RANDOM_INTERNAL_THREEFRY_SSE2_4X32_CYCLE_4(N)                    \
    MCKL_RANDOM_INTERNAL_THREEFRY_SSE2_4X32_RBOX(N * 8 + 1);                  \
    MCKL_RANDOM_INTERNAL_THREEFRY_SSE2_4X32_RBOX(N * 8 + 2);                  \
    MCKL_RANDOM_INTERNAL_THREEFRY_SSE2_4X32_RBOX(N * 8 + 3);                  \
    MCKL_RANDOM_INTERNAL_THREEFRY_SSE2_4X32_RBOX(N * 8 + 4);                  \
    MCKL_RANDOM_INTERNAL_THREEFRY_SSE2_4X32_KBOX(N * 8 + 4);

#define MCKL_RANDOM_INTERNAL_THREEFRY_SSE2_4X32_CYCLE_8(N)                    \
    MCKL_RANDOM_INTERNAL_THREEFRY_SSE2_4X32_RBOX(N * 8 + 1);                  \
    MCKL_RANDOM_INTERNAL_THREEFRY_SSE2_4X32_RBOX(N * 8 + 2);                  \
    MCKL_RANDOM_INTERNAL_THREEFRY_SSE2_4X32_RBOX(N * 8 + 3);                  \
    MCKL_RANDOM_INTERNAL_THREEFRY_SSE2_4X32_RBOX(N * 8 + 4);                  \
    MCKL_RANDOM_INTERNAL_THREEFRY_SSE2_4X32_KBOX(N * 8 + 4);                  \
    MCKL_RANDOM_INTERNAL_THREEFRY_SSE2_4X32_RBOX(N * 8 + 5);                  \
    MCKL_RANDOM_INTERNAL_THREEFRY_SSE2_4X32_RBOX(N * 8 + 6);                  \
    MCKL_RANDOM_INTERNAL_THREEFRY_SSE2_4X32_RBOX(N * 8 + 7);                  \
    MCKL_RANDOM_INTERNAL_THREEFRY_SSE2_4X32_RBOX(N * 8 + 8);                  \
    MCKL_RANDOM_INTERNAL_THREEFRY_SSE2_4X32_KBOX(N * 8 + 8);

namespace mckl {

namespace internal {

template <typename T>
class Threefry4x32GeneratorSSE2Impl
{
    static_assert(std::numeric_limits<T>::digits == 32,
        "**Threefry4x32GeneratorSSE2Impl** used with T other than a "
        "32-bit unsigned integers");

    static constexpr std::size_t K = 4;
    static constexpr std::size_t Rounds = 20;
    using Constants = ThreefryConstants<T, 4>;

  public:
    static void eval(
        const void *plain, void *cipher, const std::array<T, K + 4> &par)
    {
        Threefry4x32GeneratorGenericImpl<T>::eval(plain, cipher, par);
    }

    template <typename ResultType>
    static void eval(std::array<std::uint64_t, 2> &ctr, ResultType *r,
        const std::array<T, K + 4> &par)
    {
        Threefry4x32GeneratorGenericImpl<T>::eval(ctr, r, par);
    }

    template <typename ResultType>
    static void eval(std::array<std::uint64_t, 2> &ctr, std::size_t n,
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
    static void eval_kernel(std::array<std::uint64_t, 2> &ctr, std::size_t n,
        ResultType *r, const std::array<T, K + 4> &par)
    {
        constexpr std::size_t S = 8;
        constexpr std::size_t N = sizeof(__m128i) * S / (sizeof(T) * K);

        __m128i xmmc =
            _mm_set_epi64x(static_cast<MCKL_INT64>(std::get<1>(ctr)),
                static_cast<MCKL_INT64>(std::get<0>(ctr)));
        ctr.front() += n;

        __m128i *rptr = reinterpret_cast<__m128i *>(r);
        while (n != 0) {
            __m128i xmms0 = _mm_add_epi64(xmmc, _mm_set_epi64x(0, 1));
            __m128i xmms1 = _mm_add_epi64(xmmc, _mm_set_epi64x(0, 2));
            __m128i xmms2 = _mm_add_epi64(xmmc, _mm_set_epi64x(0, 3));
            __m128i xmms3 = _mm_add_epi64(xmmc, _mm_set_epi64x(0, 4));
            __m128i xmms4 = _mm_add_epi64(xmmc, _mm_set_epi64x(0, 5));
            __m128i xmms5 = _mm_add_epi64(xmmc, _mm_set_epi64x(0, 6));
            __m128i xmms6 = _mm_add_epi64(xmmc, _mm_set_epi64x(0, 7));
            __m128i xmms7 = _mm_add_epi64(xmmc, _mm_set_epi64x(0, 8));
            xmmc = xmms7;

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

            MCKL_RANDOM_INTERNAL_THREEFRY_SSE2_4X32_KBOX(0)
            MCKL_RANDOM_INTERNAL_THREEFRY_SSE2_4X32_CYCLE_8(0)
            MCKL_RANDOM_INTERNAL_THREEFRY_SSE2_4X32_CYCLE_8(1)
            MCKL_RANDOM_INTERNAL_THREEFRY_SSE2_4X32_CYCLE_4(2)

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

            if (n >= N) {
                n -= N;
                _mm_storeu_si128(rptr++, xmms0);
                _mm_storeu_si128(rptr++, xmms1);
                _mm_storeu_si128(rptr++, xmms2);
                _mm_storeu_si128(rptr++, xmms3);
                _mm_storeu_si128(rptr++, xmms4);
                _mm_storeu_si128(rptr++, xmms5);
                _mm_storeu_si128(rptr++, xmms6);
                _mm_storeu_si128(rptr++, xmms7);
            } else {
                std::array<__m128i, S> s;
                std::get<0>(s) = xmms0;
                std::get<1>(s) = xmms1;
                std::get<2>(s) = xmms2;
                std::get<3>(s) = xmms3;
                std::get<4>(s) = xmms4;
                std::get<5>(s) = xmms5;
                std::get<6>(s) = xmms6;
                std::get<7>(s) = xmms7;
                std::memcpy(rptr, s.data(), n * sizeof(T) * K);
                break;
            }
        }
    }
}; // class Threefry4x32GeneratorSSE2Impl

} // namespace internal

} // namespace mckl

MCKL_POP_GCC_WARNING

#endif // MCKL_RANDOM_INTERNAL_THREEFRY_SSE2_4X32_HPP
