//============================================================================
// MCKL/include/mckl/random/internal/threefry_avx2_8x64.hpp
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

#ifndef MCKL_RANDOM_INTERNAL_THREEFRY_AVX2_8X64_HPP
#define MCKL_RANDOM_INTERNAL_THREEFRY_AVX2_8X64_HPP

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

#define MCKL_RANDOM_INTERNAL_THREEFRY_AVX2_8X64_KBOX_B(N)                     \
    ymmt0 = _mm256_set1_epi64x(static_cast<MCKL_INT64>(                       \
        ThreefryKBox<T, K, N, Constants>::template key<0>(par)));             \
    ymmt1 = _mm256_set1_epi64x(static_cast<MCKL_INT64>(                       \
        ThreefryKBox<T, K, N, Constants>::template key<1>(par)));             \
    ymmt2 = _mm256_set1_epi64x(static_cast<MCKL_INT64>(                       \
        ThreefryKBox<T, K, N, Constants>::template key<2>(par)));             \
    ymmt3 = _mm256_set1_epi64x(static_cast<MCKL_INT64>(                       \
        ThreefryKBox<T, K, N, Constants>::template key<3>(par)));             \
    ymmt4 = _mm256_set1_epi64x(static_cast<MCKL_INT64>(                       \
        ThreefryKBox<T, K, N, Constants>::template key<4>(par)));             \
    ymmt5 = _mm256_set1_epi64x(static_cast<MCKL_INT64>(                       \
        ThreefryKBox<T, K, N, Constants>::template key<5>(par)));             \
    ymmt6 = _mm256_set1_epi64x(static_cast<MCKL_INT64>(                       \
        ThreefryKBox<T, K, N, Constants>::template key<6>(par)));             \
    ymmt7 = _mm256_set1_epi64x(static_cast<MCKL_INT64>(                       \
        ThreefryKBox<T, K, N, Constants>::template key<7>(par)));             \
    ymms0 = _mm256_add_epi64(ymms0, ymmt0);                                   \
    ymms1 = _mm256_add_epi64(ymms1, ymmt1);                                   \
    ymms2 = _mm256_add_epi64(ymms2, ymmt2);                                   \
    ymms3 = _mm256_add_epi64(ymms3, ymmt3);                                   \
    ymms4 = _mm256_add_epi64(ymms4, ymmt4);                                   \
    ymms5 = _mm256_add_epi64(ymms5, ymmt5);                                   \
    ymms6 = _mm256_add_epi64(ymms6, ymmt6);                                   \
    ymms7 = _mm256_add_epi64(ymms7, ymmt7);

#define MCKL_RANDOM_INTERNAL_THREEFRY_AVX2_8X64_RBOX_B(N)                     \
    {                                                                         \
        constexpr int L0 = Constants::rotate::value[0][(N - 1) % 8];          \
        constexpr int L1 = Constants::rotate::value[1][(N - 1) % 8];          \
        constexpr int L2 = Constants::rotate::value[2][(N - 1) % 8];          \
        constexpr int L3 = Constants::rotate::value[3][(N - 1) % 8];          \
        constexpr int R0 = 64 - L0;                                           \
        constexpr int R1 = 64 - L1;                                           \
        constexpr int R2 = 64 - L2;                                           \
        constexpr int R3 = 64 - L3;                                           \
        ymmt0 = _mm256_add_epi64(ymms0, ymms1);                               \
        ymmt2 = _mm256_add_epi64(ymms2, ymms3);                               \
        ymmt4 = _mm256_add_epi64(ymms4, ymms5);                               \
        ymmt6 = _mm256_add_epi64(ymms6, ymms7);                               \
        ymmt1 = _mm256_slli_epi64(ymms1, L0);                                 \
        ymmt3 = _mm256_slli_epi64(ymms3, L1);                                 \
        ymmt5 = _mm256_slli_epi64(ymms5, L2);                                 \
        ymmt7 = _mm256_slli_epi64(ymms7, L3);                                 \
        ymms1 = _mm256_srli_epi64(ymms1, R0);                                 \
        ymms3 = _mm256_srli_epi64(ymms3, R1);                                 \
        ymms5 = _mm256_srli_epi64(ymms5, R2);                                 \
        ymms7 = _mm256_srli_epi64(ymms7, R3);                                 \
        ymmt1 = _mm256_or_si256(ymms1, ymmt1);                                \
        ymmt3 = _mm256_or_si256(ymms3, ymmt3);                                \
        ymmt5 = _mm256_or_si256(ymms5, ymmt5);                                \
        ymmt7 = _mm256_or_si256(ymms7, ymmt7);                                \
        ymms0 = ymmt2;                                                        \
        ymms2 = ymmt4;                                                        \
        ymms4 = ymmt6;                                                        \
        ymms6 = ymmt0;                                                        \
        ymms1 = _mm256_xor_si256(ymmt0, ymmt1);                               \
        ymms3 = _mm256_xor_si256(ymmt6, ymmt7);                               \
        ymms5 = _mm256_xor_si256(ymmt4, ymmt5);                               \
        ymms7 = _mm256_xor_si256(ymmt2, ymmt3);                               \
    }

#define MCKL_RANDOM_INTERNAL_THREEFRY_AVX2_8X64_KBOX_1(N)                     \
    ymmt0 = _mm256_set_epi64x(                                                \
        static_cast<MCKL_INT64>(                                              \
            ThreefryKBox<T, K, N, Constants>::template key<6>(par)),          \
        static_cast<MCKL_INT64>(                                              \
            ThreefryKBox<T, K, N, Constants>::template key<4>(par)),          \
        static_cast<MCKL_INT64>(                                              \
            ThreefryKBox<T, K, N, Constants>::template key<2>(par)),          \
        static_cast<MCKL_INT64>(                                              \
            ThreefryKBox<T, K, N, Constants>::template key<0>(par)));         \
    ymmt1 = _mm256_set_epi64x(                                                \
        static_cast<MCKL_INT64>(                                              \
            ThreefryKBox<T, K, N, Constants>::template key<7>(par)),          \
        static_cast<MCKL_INT64>(                                              \
            ThreefryKBox<T, K, N, Constants>::template key<5>(par)),          \
        static_cast<MCKL_INT64>(                                              \
            ThreefryKBox<T, K, N, Constants>::template key<3>(par)),          \
        static_cast<MCKL_INT64>(                                              \
            ThreefryKBox<T, K, N, Constants>::template key<1>(par)));         \
    ymms0 = _mm256_add_epi64(ymms0, ymmt0);                                   \
    ymms1 = _mm256_add_epi64(ymms1, ymmt1);

#define MCKL_RANDOM_INTERNAL_THREEFRY_AVX2_8X64_RBOX_1(N)                     \
    {                                                                         \
        constexpr int L0 = Constants::rotate::value[0][(N - 1) % 8];          \
        constexpr int L1 = Constants::rotate::value[1][(N - 1) % 8];          \
        constexpr int L2 = Constants::rotate::value[2][(N - 1) % 8];          \
        constexpr int L3 = Constants::rotate::value[3][(N - 1) % 8];          \
        constexpr int R0 = 64 - L0;                                           \
        constexpr int R1 = 64 - L1;                                           \
        constexpr int R2 = 64 - L2;                                           \
        constexpr int R3 = 64 - L3;                                           \
        ymmt0 = _mm256_set_epi64x(L3, L2, L1, L0);                            \
        ymmt1 = _mm256_set_epi64x(R3, R2, R1, R0);                            \
        ymms0 = _mm256_add_epi64(ymms0, ymms1);                               \
        ymmt0 = _mm256_sllv_epi64(ymms1, ymmt0);                              \
        ymmt1 = _mm256_srlv_epi64(ymms1, ymmt1);                              \
        ymms1 = _mm256_or_si256(ymmt0, ymmt1);                                \
        ymms1 = _mm256_xor_si256(ymms0, ymms1);                               \
        ymms0 = _mm256_permute4x64_epi64(ymms0, 0x39); /* 0 3 2 1 */          \
        ymms1 = _mm256_permute4x64_epi64(ymms1, 0x6C); /* 1 2 3 0 */          \
    }

#define MCKL_RANDOM_INTERNAL_THREEFRY_AVX2_8X64_CYCLE_4(S, N)                 \
    MCKL_RANDOM_INTERNAL_THREEFRY_AVX2_8X64_RBOX_##S(N * 8 + 1);              \
    MCKL_RANDOM_INTERNAL_THREEFRY_AVX2_8X64_RBOX_##S(N * 8 + 2);              \
    MCKL_RANDOM_INTERNAL_THREEFRY_AVX2_8X64_RBOX_##S(N * 8 + 3);              \
    MCKL_RANDOM_INTERNAL_THREEFRY_AVX2_8X64_RBOX_##S(N * 8 + 4);              \
    MCKL_RANDOM_INTERNAL_THREEFRY_AVX2_8X64_KBOX_##S(N * 8 + 4);

#define MCKL_RANDOM_INTERNAL_THREEFRY_AVX2_8X64_CYCLE_8(S, N)                 \
    MCKL_RANDOM_INTERNAL_THREEFRY_AVX2_8X64_RBOX_##S(N * 8 + 1);              \
    MCKL_RANDOM_INTERNAL_THREEFRY_AVX2_8X64_RBOX_##S(N * 8 + 2);              \
    MCKL_RANDOM_INTERNAL_THREEFRY_AVX2_8X64_RBOX_##S(N * 8 + 3);              \
    MCKL_RANDOM_INTERNAL_THREEFRY_AVX2_8X64_RBOX_##S(N * 8 + 4);              \
    MCKL_RANDOM_INTERNAL_THREEFRY_AVX2_8X64_KBOX_##S(N * 8 + 4);              \
    MCKL_RANDOM_INTERNAL_THREEFRY_AVX2_8X64_RBOX_##S(N * 8 + 5);              \
    MCKL_RANDOM_INTERNAL_THREEFRY_AVX2_8X64_RBOX_##S(N * 8 + 6);              \
    MCKL_RANDOM_INTERNAL_THREEFRY_AVX2_8X64_RBOX_##S(N * 8 + 7);              \
    MCKL_RANDOM_INTERNAL_THREEFRY_AVX2_8X64_RBOX_##S(N * 8 + 8);              \
    MCKL_RANDOM_INTERNAL_THREEFRY_AVX2_8X64_KBOX_##S(N * 8 + 8);

#define MCKL_RANDOM_INTERNAL_THREEFRY_AVX2_8X64_ROUND_20(S)                   \
    MCKL_RANDOM_INTERNAL_THREEFRY_AVX2_8X64_KBOX_##S(0);                      \
    MCKL_RANDOM_INTERNAL_THREEFRY_AVX2_8X64_CYCLE_8(S, 0);                    \
    MCKL_RANDOM_INTERNAL_THREEFRY_AVX2_8X64_CYCLE_8(S, 1);                    \
    MCKL_RANDOM_INTERNAL_THREEFRY_AVX2_8X64_CYCLE_4(S, 2);

#define MCKL_RANDOM_INTERNAL_THREEFRY_AVX2_8X64_ROUND_72(S)                   \
    MCKL_RANDOM_INTERNAL_THREEFRY_AVX2_8X64_KBOX_##S(0);                      \
    MCKL_RANDOM_INTERNAL_THREEFRY_AVX2_8X64_CYCLE_8(S, 0);                    \
    MCKL_RANDOM_INTERNAL_THREEFRY_AVX2_8X64_CYCLE_8(S, 1);                    \
    MCKL_RANDOM_INTERNAL_THREEFRY_AVX2_8X64_CYCLE_8(S, 2);                    \
    MCKL_RANDOM_INTERNAL_THREEFRY_AVX2_8X64_CYCLE_8(S, 3);                    \
    MCKL_RANDOM_INTERNAL_THREEFRY_AVX2_8X64_CYCLE_8(S, 4);                    \
    MCKL_RANDOM_INTERNAL_THREEFRY_AVX2_8X64_CYCLE_8(S, 5);                    \
    MCKL_RANDOM_INTERNAL_THREEFRY_AVX2_8X64_CYCLE_8(S, 6);                    \
    MCKL_RANDOM_INTERNAL_THREEFRY_AVX2_8X64_CYCLE_8(S, 7);                    \
    MCKL_RANDOM_INTERNAL_THREEFRY_AVX2_8X64_CYCLE_8(S, 8);

#define MCKL_RANDOM_INTERNAL_THREEFRY_AVX2_8X64_ROUND(L)                      \
    constexpr std::size_t S = 8;                                              \
    constexpr std::size_t N = sizeof(__m256i) * S / (sizeof(T) * K);          \
    constexpr std::size_t M = sizeof(__m256i) / sizeof(ResultType);           \
    constexpr std::size_t R = sizeof(T) * K / sizeof(ResultType);             \
                                                                              \
    __m256i ymmc0 =                                                           \
        _mm256_set_epi64x(static_cast<MCKL_INT64>(std::get<3>(ctr)),          \
            static_cast<MCKL_INT64>(std::get<2>(ctr)),                        \
            static_cast<MCKL_INT64>(std::get<1>(ctr)),                        \
            static_cast<MCKL_INT64>(std::get<0>(ctr)));                       \
    __m256i ymmc1 =                                                           \
        _mm256_set_epi64x(static_cast<MCKL_INT64>(std::get<7>(ctr)),          \
            static_cast<MCKL_INT64>(std::get<6>(ctr)),                        \
            static_cast<MCKL_INT64>(std::get<5>(ctr)),                        \
            static_cast<MCKL_INT64>(std::get<4>(ctr)));                       \
                                                                              \
    while (n >= N) {                                                          \
        __m256i ymms0 =                                                       \
            _mm256_add_epi64(ymmc0, _mm256_set_epi64x(0, 0, 0, 1));           \
        __m256i ymms2 =                                                       \
            _mm256_add_epi64(ymmc0, _mm256_set_epi64x(0, 0, 0, 2));           \
        __m256i ymms4 =                                                       \
            _mm256_add_epi64(ymmc0, _mm256_set_epi64x(0, 0, 0, 3));           \
        __m256i ymms6 =                                                       \
            _mm256_add_epi64(ymmc0, _mm256_set_epi64x(0, 0, 0, 4));           \
        ymmc0 = ymms6;                                                        \
                                                                              \
        __m256i ymms1 = ymmc1;                                                \
        __m256i ymms3 = ymmc1;                                                \
        __m256i ymms5 = ymmc1;                                                \
        __m256i ymms7 = ymmc1;                                                \
                                                                              \
        __m256i ymmt0;                                                        \
        __m256i ymmt1;                                                        \
        __m256i ymmt2;                                                        \
        __m256i ymmt3;                                                        \
        __m256i ymmt4;                                                        \
        __m256i ymmt5;                                                        \
        __m256i ymmt6;                                                        \
        __m256i ymmt7;                                                        \
                                                                              \
        ymmt0 = ymms1;                                                        \
        ymms1 = ymms2;                                                        \
        ymms2 = ymms4;                                                        \
        ymms4 = ymmt0;                                                        \
                                                                              \
        ymmt1 = ymms3;                                                        \
        ymms3 = ymms6;                                                        \
        ymms6 = ymms5;                                                        \
        ymms5 = ymmt1;                                                        \
                                                                              \
        ymmt0 = _mm256_unpacklo_epi64(ymms0, ymms1);                          \
        ymmt1 = _mm256_unpacklo_epi64(ymms2, ymms3);                          \
        ymmt2 = _mm256_unpackhi_epi64(ymms0, ymms1);                          \
        ymmt3 = _mm256_unpackhi_epi64(ymms2, ymms3);                          \
        ymmt4 = _mm256_unpacklo_epi64(ymms4, ymms5);                          \
        ymmt5 = _mm256_unpacklo_epi64(ymms6, ymms7);                          \
        ymmt6 = _mm256_unpackhi_epi64(ymms4, ymms5);                          \
        ymmt7 = _mm256_unpackhi_epi64(ymms6, ymms7);                          \
        ymms0 = _mm256_permute2x128_si256(ymmt0, ymmt1, 0x20);                \
        ymms1 = _mm256_permute2x128_si256(ymmt2, ymmt3, 0x20);                \
        ymms2 = _mm256_permute2x128_si256(ymmt0, ymmt1, 0x31);                \
        ymms3 = _mm256_permute2x128_si256(ymmt2, ymmt3, 0x31);                \
        ymms4 = _mm256_permute2x128_si256(ymmt4, ymmt5, 0x20);                \
        ymms5 = _mm256_permute2x128_si256(ymmt6, ymmt7, 0x20);                \
        ymms6 = _mm256_permute2x128_si256(ymmt4, ymmt5, 0x31);                \
        ymms7 = _mm256_permute2x128_si256(ymmt6, ymmt7, 0x31);                \
                                                                              \
        MCKL_RANDOM_INTERNAL_THREEFRY_AVX2_8X64_ROUND_##L(B);                 \
                                                                              \
        ymmt0 = _mm256_unpacklo_epi64(ymms0, ymms1);                          \
        ymmt1 = _mm256_unpacklo_epi64(ymms2, ymms3);                          \
        ymmt2 = _mm256_unpackhi_epi64(ymms0, ymms1);                          \
        ymmt3 = _mm256_unpackhi_epi64(ymms2, ymms3);                          \
        ymmt4 = _mm256_unpacklo_epi64(ymms4, ymms5);                          \
        ymmt5 = _mm256_unpacklo_epi64(ymms6, ymms7);                          \
        ymmt6 = _mm256_unpackhi_epi64(ymms4, ymms5);                          \
        ymmt7 = _mm256_unpackhi_epi64(ymms6, ymms7);                          \
        ymms0 = _mm256_permute2x128_si256(ymmt0, ymmt1, 0x20);                \
        ymms1 = _mm256_permute2x128_si256(ymmt2, ymmt3, 0x20);                \
        ymms2 = _mm256_permute2x128_si256(ymmt0, ymmt1, 0x31);                \
        ymms3 = _mm256_permute2x128_si256(ymmt2, ymmt3, 0x31);                \
        ymms4 = _mm256_permute2x128_si256(ymmt4, ymmt5, 0x20);                \
        ymms5 = _mm256_permute2x128_si256(ymmt6, ymmt7, 0x20);                \
        ymms6 = _mm256_permute2x128_si256(ymmt4, ymmt5, 0x31);                \
        ymms7 = _mm256_permute2x128_si256(ymmt6, ymmt7, 0x31);                \
                                                                              \
        ymmt0 = ymms2;                                                        \
        ymms2 = ymms1;                                                        \
        ymms1 = ymms4;                                                        \
        ymms4 = ymmt0;                                                        \
                                                                              \
        ymmt1 = ymms6;                                                        \
        ymms6 = ymms3;                                                        \
        ymms3 = ymms5;                                                        \
        ymms5 = ymmt1;                                                        \
                                                                              \
        _mm256_storeu_si256(reinterpret_cast<__m256i *>(r + M * 0), ymms0);   \
        _mm256_storeu_si256(reinterpret_cast<__m256i *>(r + M * 1), ymms1);   \
        _mm256_storeu_si256(reinterpret_cast<__m256i *>(r + M * 2), ymms2);   \
        _mm256_storeu_si256(reinterpret_cast<__m256i *>(r + M * 3), ymms3);   \
        _mm256_storeu_si256(reinterpret_cast<__m256i *>(r + M * 4), ymms4);   \
        _mm256_storeu_si256(reinterpret_cast<__m256i *>(r + M * 5), ymms5);   \
        _mm256_storeu_si256(reinterpret_cast<__m256i *>(r + M * 6), ymms6);   \
        _mm256_storeu_si256(reinterpret_cast<__m256i *>(r + M * 7), ymms7);   \
                                                                              \
        n -= N;                                                               \
        r += N * R;                                                           \
    }                                                                         \
                                                                              \
    for (std::size_t i = 0; i != n; ++i, r += R) {                            \
        ymmc0 = _mm256_add_epi64(ymmc0, _mm256_set_epi64x(0, 0, 0, 1));       \
                                                                              \
        __m256i ymms0 = _mm256_unpacklo_epi64(ymmc0, ymmc1);                  \
        __m256i ymms1 = _mm256_unpackhi_epi64(ymmc0, ymmc1);                  \
        ymms0 = _mm256_permute4x64_epi64(ymms0, 0xD8); /* 3 1 2 0 */          \
        ymms1 = _mm256_permute4x64_epi64(ymms1, 0xD8); /* 3 1 2 0 */          \
        __m256i ymmt0;                                                        \
        __m256i ymmt1;                                                        \
                                                                              \
        MCKL_RANDOM_INTERNAL_THREEFRY_AVX2_8X64_ROUND_##L(1);                 \
                                                                              \
        ymms0 = _mm256_permute4x64_epi64(ymms0, 0xD8); /* 3 1 2 0 */          \
        ymms1 = _mm256_permute4x64_epi64(ymms1, 0xD8); /* 3 1 2 0 */          \
        ymmt0 = _mm256_unpacklo_epi64(ymms0, ymms1);                          \
        ymmt1 = _mm256_unpackhi_epi64(ymms0, ymms1);                          \
                                                                              \
        _mm256_storeu_si256(reinterpret_cast<__m256i *>(r + M * 0), ymmt0);   \
        _mm256_storeu_si256(reinterpret_cast<__m256i *>(r + M * 1), ymmt1);   \
    }                                                                         \
                                                                              \
    _mm256_storeu_si256(reinterpret_cast<__m256i *>(ctr.data()), ymmc0);

namespace mckl
{

namespace internal
{

template <typename T>
class Threefry8x64GeneratorAVX2Impl
{
    static_assert(std::numeric_limits<T>::digits == 64,
        "**Threefry8x64GeneratorAVX2Impl** used with T other than a 64-bit "
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
        MCKL_RANDOM_INTERNAL_THREEFRY_AVX2_8X64_ROUND(20)
    }
}; // class Threefry8x64GeneratorAVX2Impl

template <typename T>
class Threefish512GeneratorAVX2Impl
{
    static_assert(std::numeric_limits<T>::digits == 64,
        "**Threefish512GeneratorAVX2Impl** used with T other than a 64-bit "
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
        MCKL_RANDOM_INTERNAL_THREEFRY_AVX2_8X64_ROUND(72)
    }
}; // class Threefish512GeneratorAVX2Impl

} // namespace mckl::internal

} // namespace mckl

#ifdef MCKL_GCC
#if MCKL_GCC_VERSION >= 60000
#pragma GCC diagnostic pop
#endif
#endif

#endif // MCKL_RANDOM_INTERNAL_THREEFRY_AVX2_8X64_HPP
