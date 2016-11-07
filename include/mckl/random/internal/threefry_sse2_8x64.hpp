//============================================================================
// MCKL/include/mckl/random/internal/threefry_sse2_8x64.hpp
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

#ifndef MCKL_RANDOM_INTERNAL_THREEFRY_SSE2_8X64_HPP
#define MCKL_RANDOM_INTERNAL_THREEFRY_SSE2_8X64_HPP

#include <mckl/random/internal/common.hpp>
#include <mckl/random/internal/threefry_common.hpp>
#include <mckl/random/internal/threefry_constants.hpp>
#include <mckl/random/internal/threefry_generic_8x64.hpp>
#include <mckl/random/increment.hpp>

#ifdef MCKL_GCC
#if MCKL_GCC_VERSION >= 60000
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wignored-attributes"
#endif
#endif

#define MCKL_RANDOM_INTERNAL_THREEFRY_SSE2_8X64_KBOX(N)                       \
    xmmt0 = _mm_set1_epi64x(static_cast<MCKL_INT64>(                          \
        ThreefryKBox<T, K, N>::template key<0>(par)));                        \
    xmmt1 = _mm_set1_epi64x(static_cast<MCKL_INT64>(                          \
        ThreefryKBox<T, K, N>::template key<1>(par)));                        \
    xmmt2 = _mm_set1_epi64x(static_cast<MCKL_INT64>(                          \
        ThreefryKBox<T, K, N>::template key<2>(par)));                        \
    xmmt3 = _mm_set1_epi64x(static_cast<MCKL_INT64>(                          \
        ThreefryKBox<T, K, N>::template key<3>(par)));                        \
    xmmt4 = _mm_set1_epi64x(static_cast<MCKL_INT64>(                          \
        ThreefryKBox<T, K, N>::template key<4>(par)));                        \
    xmmt5 = _mm_set1_epi64x(static_cast<MCKL_INT64>(                          \
        ThreefryKBox<T, K, N>::template key<5>(par)));                        \
    xmmt6 = _mm_set1_epi64x(static_cast<MCKL_INT64>(                          \
        ThreefryKBox<T, K, N>::template key<6>(par)));                        \
    xmmt7 = _mm_set1_epi64x(static_cast<MCKL_INT64>(                          \
        ThreefryKBox<T, K, N>::template key<7>(par)));                        \
    xmms0 = _mm_add_epi64(xmms0, xmmt0);                                      \
    xmms1 = _mm_add_epi64(xmms1, xmmt1);                                      \
    xmms2 = _mm_add_epi64(xmms2, xmmt2);                                      \
    xmms3 = _mm_add_epi64(xmms3, xmmt3);                                      \
    xmms4 = _mm_add_epi64(xmms4, xmmt4);                                      \
    xmms5 = _mm_add_epi64(xmms5, xmmt5);                                      \
    xmms6 = _mm_add_epi64(xmms6, xmmt6);                                      \
    xmms7 = _mm_add_epi64(xmms7, xmmt7);

#define MCKL_RANDOM_INTERNAL_THREEFRY_SSE2_8X64_RBOX(N)                       \
    {                                                                         \
        constexpr int L0 = Constants::rotate::value[0][(N - 1) % 8];          \
        constexpr int L1 = Constants::rotate::value[1][(N - 1) % 8];          \
        constexpr int L2 = Constants::rotate::value[2][(N - 1) % 8];          \
        constexpr int L3 = Constants::rotate::value[3][(N - 1) % 8];          \
        constexpr int R0 = 64 - L0;                                           \
        constexpr int R1 = 64 - L1;                                           \
        constexpr int R2 = 64 - L2;                                           \
        constexpr int R3 = 64 - L3;                                           \
        xmmt0 = _mm_add_epi64(xmms0, xmms1);                                  \
        xmmt2 = _mm_add_epi64(xmms2, xmms3);                                  \
        xmmt4 = _mm_add_epi64(xmms4, xmms5);                                  \
        xmmt6 = _mm_add_epi64(xmms6, xmms7);                                  \
        xmmt1 = _mm_slli_epi64(xmms1, L0);                                    \
        xmmt3 = _mm_slli_epi64(xmms3, L1);                                    \
        xmmt5 = _mm_slli_epi64(xmms5, L2);                                    \
        xmmt7 = _mm_slli_epi64(xmms7, L3);                                    \
        xmms1 = _mm_srli_epi64(xmms1, R0);                                    \
        xmms3 = _mm_srli_epi64(xmms3, R1);                                    \
        xmms5 = _mm_srli_epi64(xmms5, R2);                                    \
        xmms7 = _mm_srli_epi64(xmms7, R3);                                    \
        xmmt1 = _mm_or_si128(xmms1, xmmt1);                                   \
        xmmt3 = _mm_or_si128(xmms3, xmmt3);                                   \
        xmmt5 = _mm_or_si128(xmms5, xmmt5);                                   \
        xmmt7 = _mm_or_si128(xmms7, xmmt7);                                   \
        xmms0 = xmmt2;                                                        \
        xmms2 = xmmt4;                                                        \
        xmms4 = xmmt6;                                                        \
        xmms6 = xmmt0;                                                        \
        xmms1 = _mm_xor_si128(xmmt0, xmmt1);                                  \
        xmms3 = _mm_xor_si128(xmmt6, xmmt7);                                  \
        xmms5 = _mm_xor_si128(xmmt4, xmmt5);                                  \
        xmms7 = _mm_xor_si128(xmmt2, xmmt3);                                  \
    }

#define MCKL_RANDOM_INTERNAL_THREEFRY_SSE2_8X64_CYCLE_4(N)                    \
    MCKL_RANDOM_INTERNAL_THREEFRY_SSE2_8X64_RBOX(N * 8 + 1);                  \
    MCKL_RANDOM_INTERNAL_THREEFRY_SSE2_8X64_RBOX(N * 8 + 2);                  \
    MCKL_RANDOM_INTERNAL_THREEFRY_SSE2_8X64_RBOX(N * 8 + 3);                  \
    MCKL_RANDOM_INTERNAL_THREEFRY_SSE2_8X64_RBOX(N * 8 + 4);                  \
    MCKL_RANDOM_INTERNAL_THREEFRY_SSE2_8X64_KBOX(N * 8 + 4);

#define MCKL_RANDOM_INTERNAL_THREEFRY_SSE2_8X64_CYCLE_8(N)                    \
    MCKL_RANDOM_INTERNAL_THREEFRY_SSE2_8X64_RBOX(N * 8 + 1);                  \
    MCKL_RANDOM_INTERNAL_THREEFRY_SSE2_8X64_RBOX(N * 8 + 2);                  \
    MCKL_RANDOM_INTERNAL_THREEFRY_SSE2_8X64_RBOX(N * 8 + 3);                  \
    MCKL_RANDOM_INTERNAL_THREEFRY_SSE2_8X64_RBOX(N * 8 + 4);                  \
    MCKL_RANDOM_INTERNAL_THREEFRY_SSE2_8X64_KBOX(N * 8 + 4);                  \
    MCKL_RANDOM_INTERNAL_THREEFRY_SSE2_8X64_RBOX(N * 8 + 5);                  \
    MCKL_RANDOM_INTERNAL_THREEFRY_SSE2_8X64_RBOX(N * 8 + 6);                  \
    MCKL_RANDOM_INTERNAL_THREEFRY_SSE2_8X64_RBOX(N * 8 + 7);                  \
    MCKL_RANDOM_INTERNAL_THREEFRY_SSE2_8X64_RBOX(N * 8 + 8);                  \
    MCKL_RANDOM_INTERNAL_THREEFRY_SSE2_8X64_KBOX(N * 8 + 8);

#define MCKL_RANDOM_INTERNAL_THREEFRY_SSE2_8X64_ROUND_20                      \
    MCKL_RANDOM_INTERNAL_THREEFRY_SSE2_8X64_KBOX(0);                          \
    MCKL_RANDOM_INTERNAL_THREEFRY_SSE2_8X64_CYCLE_8(0);                       \
    MCKL_RANDOM_INTERNAL_THREEFRY_SSE2_8X64_CYCLE_8(1);                       \
    MCKL_RANDOM_INTERNAL_THREEFRY_SSE2_8X64_CYCLE_4(2);

#define MCKL_RANDOM_INTERNAL_THREEFRY_SSE2_8X64_ROUND_72                      \
    MCKL_RANDOM_INTERNAL_THREEFRY_SSE2_8X64_KBOX(0);                          \
    MCKL_RANDOM_INTERNAL_THREEFRY_SSE2_8X64_CYCLE_8(0);                       \
    MCKL_RANDOM_INTERNAL_THREEFRY_SSE2_8X64_CYCLE_8(1);                       \
    MCKL_RANDOM_INTERNAL_THREEFRY_SSE2_8X64_CYCLE_8(2);                       \
    MCKL_RANDOM_INTERNAL_THREEFRY_SSE2_8X64_CYCLE_8(3);                       \
    MCKL_RANDOM_INTERNAL_THREEFRY_SSE2_8X64_CYCLE_8(4);                       \
    MCKL_RANDOM_INTERNAL_THREEFRY_SSE2_8X64_CYCLE_8(5);                       \
    MCKL_RANDOM_INTERNAL_THREEFRY_SSE2_8X64_CYCLE_8(6);                       \
    MCKL_RANDOM_INTERNAL_THREEFRY_SSE2_8X64_CYCLE_8(7);                       \
    MCKL_RANDOM_INTERNAL_THREEFRY_SSE2_8X64_CYCLE_8(8);

#define MCKL_RANDOM_INTERNAL_THREEFRY_SSE2_8X64_ROUND(L)                      \
    constexpr std::size_t S = 8;                                              \
    constexpr std::size_t N = sizeof(__m128i) * S / (sizeof(T) * K);          \
    constexpr std::size_t M = sizeof(__m128i) / sizeof(ResultType);           \
    constexpr std::size_t R = sizeof(T) * K / sizeof(ResultType);             \
                                                                              \
    __m128i xmmc0 = _mm_set_epi64x(static_cast<MCKL_INT64>(std::get<1>(ctr)), \
        static_cast<MCKL_INT64>(std::get<0>(ctr)));                           \
    __m128i xmmc1 = _mm_set_epi64x(static_cast<MCKL_INT64>(std::get<3>(ctr)), \
        static_cast<MCKL_INT64>(std::get<2>(ctr)));                           \
    __m128i xmmc2 = _mm_set_epi64x(static_cast<MCKL_INT64>(std::get<5>(ctr)), \
        static_cast<MCKL_INT64>(std::get<4>(ctr)));                           \
    __m128i xmmc3 = _mm_set_epi64x(static_cast<MCKL_INT64>(std::get<7>(ctr)), \
        static_cast<MCKL_INT64>(std::get<6>(ctr)));                           \
                                                                              \
    while (n >= N) {                                                          \
        __m128i xmmt0 = _mm_add_epi64(xmmc0, _mm_set_epi64x(0, 1));           \
        __m128i xmmt4 = _mm_add_epi64(xmmc0, _mm_set_epi64x(0, 2));           \
        xmmc0 = xmmt4;                                                        \
                                                                              \
        __m128i xmmt1 = xmmc1;                                                \
        __m128i xmmt2 = xmmc2;                                                \
        __m128i xmmt3 = xmmc3;                                                \
        __m128i xmmt5 = xmmc1;                                                \
        __m128i xmmt6 = xmmc2;                                                \
        __m128i xmmt7 = xmmc3;                                                \
                                                                              \
        __m128i xmms0;                                                        \
        __m128i xmms1;                                                        \
        __m128i xmms2;                                                        \
        __m128i xmms3;                                                        \
        __m128i xmms4;                                                        \
        __m128i xmms5;                                                        \
        __m128i xmms6;                                                        \
        __m128i xmms7;                                                        \
                                                                              \
        xmms2 = xmmt2;                                                        \
        xmmt2 = xmmt1;                                                        \
        xmmt1 = xmmt4;                                                        \
        xmmt4 = xmms2;                                                        \
                                                                              \
        xmms6 = xmmt6;                                                        \
        xmmt6 = xmmt3;                                                        \
        xmmt3 = xmmt5;                                                        \
        xmmt5 = xmms6;                                                        \
                                                                              \
        xmms0 = _mm_unpacklo_epi64(xmmt0, xmmt1);                             \
        xmms1 = _mm_unpackhi_epi64(xmmt0, xmmt1);                             \
        xmms2 = _mm_unpacklo_epi64(xmmt2, xmmt3);                             \
        xmms3 = _mm_unpackhi_epi64(xmmt2, xmmt3);                             \
        xmms4 = _mm_unpacklo_epi64(xmmt4, xmmt5);                             \
        xmms5 = _mm_unpackhi_epi64(xmmt4, xmmt5);                             \
        xmms6 = _mm_unpacklo_epi64(xmmt6, xmmt7);                             \
        xmms7 = _mm_unpackhi_epi64(xmmt6, xmmt7);                             \
                                                                              \
        MCKL_RANDOM_INTERNAL_THREEFRY_SSE2_8X64_ROUND_##L;                    \
                                                                              \
        xmmt0 = _mm_unpacklo_epi64(xmms0, xmms1);                             \
        xmmt1 = _mm_unpackhi_epi64(xmms0, xmms1);                             \
        xmmt2 = _mm_unpacklo_epi64(xmms2, xmms3);                             \
        xmmt3 = _mm_unpackhi_epi64(xmms2, xmms3);                             \
        xmmt4 = _mm_unpacklo_epi64(xmms4, xmms5);                             \
        xmmt5 = _mm_unpackhi_epi64(xmms4, xmms5);                             \
        xmmt6 = _mm_unpacklo_epi64(xmms6, xmms7);                             \
        xmmt7 = _mm_unpackhi_epi64(xmms6, xmms7);                             \
                                                                              \
        xmms1 = xmmt1;                                                        \
        xmmt1 = xmmt2;                                                        \
        xmmt2 = xmmt4;                                                        \
        xmmt4 = xmms1;                                                        \
                                                                              \
        xmms3 = xmmt3;                                                        \
        xmmt3 = xmmt6;                                                        \
        xmmt6 = xmmt5;                                                        \
        xmmt5 = xmms3;                                                        \
                                                                              \
        _mm_storeu_si128(reinterpret_cast<__m128i *>(r + M * 0), xmmt0);      \
        _mm_storeu_si128(reinterpret_cast<__m128i *>(r + M * 1), xmmt1);      \
        _mm_storeu_si128(reinterpret_cast<__m128i *>(r + M * 2), xmmt2);      \
        _mm_storeu_si128(reinterpret_cast<__m128i *>(r + M * 3), xmmt3);      \
        _mm_storeu_si128(reinterpret_cast<__m128i *>(r + M * 4), xmmt4);      \
        _mm_storeu_si128(reinterpret_cast<__m128i *>(r + M * 5), xmmt5);      \
        _mm_storeu_si128(reinterpret_cast<__m128i *>(r + M * 6), xmmt6);      \
        _mm_storeu_si128(reinterpret_cast<__m128i *>(r + M * 7), xmmt7);      \
                                                                              \
        n -= N;                                                               \
        r += N * R;                                                           \
    }                                                                         \
    _mm_storeu_si128(reinterpret_cast<__m128i *>(ctr.data()), xmmc0);

namespace mckl
{

namespace internal
{

template <typename T>
class Threefry8x64GeneratorSSE2Impl
{
    static_assert(std::numeric_limits<T>::digits == 64,
        "**Threefry8x64GeneratorSSE2Impl** used with T other than a 64-bit "
        "unsigned integers");

    static constexpr std::size_t K = 8;
    static constexpr std::size_t Rounds = 20;
    using Constants = ThreefryConstants<T, 8>;

    public:
    static void eval(
        const void *plain, void *cipher, const std::array<T, K + 4> &par)
    {
        Threefry8x64GeneratorGenericImpl<T>::eval(plain, cipher, par);
    }

    template <typename ResultType>
    static void eval(std::array<std::uint64_t, 8> &ctr, ResultType *r,
        const std::array<T, K + 4> &par)
    {
        Threefry8x64GeneratorGenericImpl<T>::eval(ctr, r, par);
    }

    template <typename ResultType>
    static void eval(std::array<std::uint64_t, 8> &ctr, std::size_t n,
        ResultType *r, const std::array<T, K + 4> &par)
    {
        if (ctr.front() >= std::numeric_limits<std::uint64_t>::max() - n) {
            MCKL_NOINLINE_CALL Threefry8x64GeneratorGenericImpl<T>::eval(
                ctr, n, r, par);
            return;
        }
        MCKL_RANDOM_INTERNAL_THREEFRY_SSE2_8X64_ROUND(20)
        MCKL_NOINLINE_CALL Threefry8x64GeneratorGenericImpl<T>::eval(
            ctr, n, r, par);
    }
}; // class Threefry8x64GeneratorSSE2Impl

template <typename T>
class Threefish512GeneratorSSE2Impl
{
    static_assert(std::numeric_limits<T>::digits == 64,
        "**Threefish512GeneratorSSE2Impl** used with T other than a 64-bit "
        "unsigned integers");

    static constexpr std::size_t K = 8;
    static constexpr std::size_t Rounds = 20;
    using Constants = ThreefryConstants<T, 8>;

    public:
    static void eval(
        const void *plain, void *cipher, const std::array<T, K + 4> &par)
    {
        Threefish512GeneratorGenericImpl<T>::eval(plain, cipher, par);
    }

    template <typename ResultType>
    static void eval(std::array<std::uint64_t, 8> &ctr, ResultType *r,
        const std::array<T, K + 4> &par)
    {
        Threefish512GeneratorGenericImpl<T>::eval(ctr, r, par);
    }

    template <typename ResultType>
    static void eval(std::array<std::uint64_t, 8> &ctr, std::size_t n,
        ResultType *r, const std::array<T, K + 4> &par)
    {
        if (ctr.front() >= std::numeric_limits<std::uint64_t>::max() - n) {
            MCKL_NOINLINE_CALL Threefish512GeneratorGenericImpl<T>::eval(
                ctr, n, r, par);
            return;
        }
        MCKL_RANDOM_INTERNAL_THREEFRY_SSE2_8X64_ROUND(72)
        MCKL_NOINLINE_CALL Threefish512GeneratorGenericImpl<T>::eval(
            ctr, n, r, par);
    }
}; // class Threefish512GeneratorSSE2Impl

} // namespace mckl::internal

} // namespace mckl

#ifdef MCKL_GCC
#if MCKL_GCC_VERSION >= 60000
#pragma GCC diagnostic pop
#endif
#endif

#endif // MCKL_RANDOM_INTERNAL_THREEFRY_SSE2_8X64_HPP
