//============================================================================
// MCKL/include/mckl/random/internal/threefry_avx2_16x64.hpp
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

#ifndef MCKL_RANDOM_INTERNAL_THREEFRY_AVX2_16X64_HPP
#define MCKL_RANDOM_INTERNAL_THREEFRY_AVX2_16X64_HPP

#include <mckl/random/internal/common.hpp>
#include <mckl/random/internal/threefry_common.hpp>
#include <mckl/random/internal/threefry_constants.hpp>
#include <mckl/random/internal/threefry_generic_16x64.hpp>
#include <mckl/random/increment.hpp>

MCKL_PUSH_GCC_WARNING("-Wignored-attributes")

#define MCKL_RANDOM_INTERNAL_THREEFRY_AVX2_16X64_KBOX(N)                      \
    ymmt0 = _mm256_set1_epi64x(static_cast<MCKL_INT64>(                       \
        ThreefryKBox<T, K, N>::template key<0x0>(par)));                      \
    ymmt1 = _mm256_set1_epi64x(static_cast<MCKL_INT64>(                       \
        ThreefryKBox<T, K, N>::template key<0x1>(par)));                      \
    ymmt2 = _mm256_set1_epi64x(static_cast<MCKL_INT64>(                       \
        ThreefryKBox<T, K, N>::template key<0x2>(par)));                      \
    ymmt3 = _mm256_set1_epi64x(static_cast<MCKL_INT64>(                       \
        ThreefryKBox<T, K, N>::template key<0x3>(par)));                      \
    ymmt4 = _mm256_set1_epi64x(static_cast<MCKL_INT64>(                       \
        ThreefryKBox<T, K, N>::template key<0x4>(par)));                      \
    ymmt5 = _mm256_set1_epi64x(static_cast<MCKL_INT64>(                       \
        ThreefryKBox<T, K, N>::template key<0x5>(par)));                      \
    ymmt6 = _mm256_set1_epi64x(static_cast<MCKL_INT64>(                       \
        ThreefryKBox<T, K, N>::template key<0x6>(par)));                      \
    ymmt7 = _mm256_set1_epi64x(static_cast<MCKL_INT64>(                       \
        ThreefryKBox<T, K, N>::template key<0x7>(par)));                      \
    ymmt8 = _mm256_set1_epi64x(static_cast<MCKL_INT64>(                       \
        ThreefryKBox<T, K, N>::template key<0x8>(par)));                      \
    ymmt9 = _mm256_set1_epi64x(static_cast<MCKL_INT64>(                       \
        ThreefryKBox<T, K, N>::template key<0x9>(par)));                      \
    ymmtA = _mm256_set1_epi64x(static_cast<MCKL_INT64>(                       \
        ThreefryKBox<T, K, N>::template key<0xA>(par)));                      \
    ymmtB = _mm256_set1_epi64x(static_cast<MCKL_INT64>(                       \
        ThreefryKBox<T, K, N>::template key<0xB>(par)));                      \
    ymmtC = _mm256_set1_epi64x(static_cast<MCKL_INT64>(                       \
        ThreefryKBox<T, K, N>::template key<0xC>(par)));                      \
    ymmtD = _mm256_set1_epi64x(static_cast<MCKL_INT64>(                       \
        ThreefryKBox<T, K, N>::template key<0xD>(par)));                      \
    ymmtE = _mm256_set1_epi64x(static_cast<MCKL_INT64>(                       \
        ThreefryKBox<T, K, N>::template key<0xE>(par)));                      \
    ymmtF = _mm256_set1_epi64x(static_cast<MCKL_INT64>(                       \
        ThreefryKBox<T, K, N>::template key<0xF>(par)));                      \
    ymms0 = _mm256_add_epi64(ymms0, ymmt0);                                   \
    ymms1 = _mm256_add_epi64(ymms1, ymmt1);                                   \
    ymms2 = _mm256_add_epi64(ymms2, ymmt2);                                   \
    ymms3 = _mm256_add_epi64(ymms3, ymmt3);                                   \
    ymms4 = _mm256_add_epi64(ymms4, ymmt4);                                   \
    ymms5 = _mm256_add_epi64(ymms5, ymmt5);                                   \
    ymms6 = _mm256_add_epi64(ymms6, ymmt6);                                   \
    ymms7 = _mm256_add_epi64(ymms7, ymmt7);                                   \
    ymms8 = _mm256_add_epi64(ymms8, ymmt8);                                   \
    ymms9 = _mm256_add_epi64(ymms9, ymmt9);                                   \
    ymmsA = _mm256_add_epi64(ymmsA, ymmtA);                                   \
    ymmsB = _mm256_add_epi64(ymmsB, ymmtB);                                   \
    ymmsC = _mm256_add_epi64(ymmsC, ymmtC);                                   \
    ymmsD = _mm256_add_epi64(ymmsD, ymmtD);                                   \
    ymmsE = _mm256_add_epi64(ymmsE, ymmtE);                                   \
    ymmsF = _mm256_add_epi64(ymmsF, ymmtF);

#define MCKL_RANDOM_INTERNAL_THREEFRY_AVX2_16X64_RBOX(N)                      \
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
        ymmt0 = _mm256_add_epi64(ymms0, ymms1);                               \
        ymmt2 = _mm256_add_epi64(ymms2, ymms3);                               \
        ymmt4 = _mm256_add_epi64(ymms4, ymms5);                               \
        ymmt6 = _mm256_add_epi64(ymms6, ymms7);                               \
        ymmt8 = _mm256_add_epi64(ymms8, ymms9);                               \
        ymmtA = _mm256_add_epi64(ymmsA, ymmsB);                               \
        ymmtC = _mm256_add_epi64(ymmsC, ymmsD);                               \
        ymmtE = _mm256_add_epi64(ymmsE, ymmsF);                               \
        ymmt1 = _mm256_slli_epi64(ymms1, L0);                                 \
        ymmt3 = _mm256_slli_epi64(ymms3, L1);                                 \
        ymmt5 = _mm256_slli_epi64(ymms5, L2);                                 \
        ymmt7 = _mm256_slli_epi64(ymms7, L3);                                 \
        ymmt9 = _mm256_slli_epi64(ymms9, L4);                                 \
        ymmtB = _mm256_slli_epi64(ymmsB, L5);                                 \
        ymmtD = _mm256_slli_epi64(ymmsD, L6);                                 \
        ymmtF = _mm256_slli_epi64(ymmsF, L7);                                 \
        ymms1 = _mm256_srli_epi64(ymms1, R0);                                 \
        ymms3 = _mm256_srli_epi64(ymms3, R1);                                 \
        ymms5 = _mm256_srli_epi64(ymms5, R2);                                 \
        ymms7 = _mm256_srli_epi64(ymms7, R3);                                 \
        ymms9 = _mm256_srli_epi64(ymms9, R4);                                 \
        ymmsB = _mm256_srli_epi64(ymmsB, R5);                                 \
        ymmsD = _mm256_srli_epi64(ymmsD, R6);                                 \
        ymmsF = _mm256_srli_epi64(ymmsF, R7);                                 \
        ymmt1 = _mm256_or_si256(ymms1, ymmt1);                                \
        ymmt3 = _mm256_or_si256(ymms3, ymmt3);                                \
        ymmt5 = _mm256_or_si256(ymms5, ymmt5);                                \
        ymmt7 = _mm256_or_si256(ymms7, ymmt7);                                \
        ymmt9 = _mm256_or_si256(ymms9, ymmt9);                                \
        ymmtB = _mm256_or_si256(ymmsB, ymmtB);                                \
        ymmtD = _mm256_or_si256(ymmsD, ymmtD);                                \
        ymmtF = _mm256_or_si256(ymmsF, ymmtF);                                \
        ymms0 = ymmt0;                                                        \
        ymms2 = ymmt2;                                                        \
        ymms4 = ymmt6;                                                        \
        ymms6 = ymmt4;                                                        \
        ymms8 = ymmtA;                                                        \
        ymmsA = ymmtC;                                                        \
        ymmsC = ymmtE;                                                        \
        ymmsE = ymmt8;                                                        \
        ymms1 = _mm256_xor_si256(ymmt8, ymmt9);                               \
        ymms3 = _mm256_xor_si256(ymmtC, ymmtD);                               \
        ymms5 = _mm256_xor_si256(ymmtA, ymmtB);                               \
        ymms7 = _mm256_xor_si256(ymmtE, ymmtF);                               \
        ymms9 = _mm256_xor_si256(ymmt6, ymmt7);                               \
        ymmsB = _mm256_xor_si256(ymmt2, ymmt3);                               \
        ymmsD = _mm256_xor_si256(ymmt4, ymmt5);                               \
        ymmsF = _mm256_xor_si256(ymmt0, ymmt1);                               \
    }

#define MCKL_RANDOM_INTERNAL_THREEFRY_AVX2_16X64_CYCLE_4(N)                   \
    MCKL_RANDOM_INTERNAL_THREEFRY_AVX2_16X64_RBOX(N * 8 + 1);                 \
    MCKL_RANDOM_INTERNAL_THREEFRY_AVX2_16X64_RBOX(N * 8 + 2);                 \
    MCKL_RANDOM_INTERNAL_THREEFRY_AVX2_16X64_RBOX(N * 8 + 3);                 \
    MCKL_RANDOM_INTERNAL_THREEFRY_AVX2_16X64_RBOX(N * 8 + 4);                 \
    MCKL_RANDOM_INTERNAL_THREEFRY_AVX2_16X64_KBOX(N * 8 + 4);

#define MCKL_RANDOM_INTERNAL_THREEFRY_AVX2_16X64_CYCLE_8(N)                   \
    MCKL_RANDOM_INTERNAL_THREEFRY_AVX2_16X64_RBOX(N * 8 + 1);                 \
    MCKL_RANDOM_INTERNAL_THREEFRY_AVX2_16X64_RBOX(N * 8 + 2);                 \
    MCKL_RANDOM_INTERNAL_THREEFRY_AVX2_16X64_RBOX(N * 8 + 3);                 \
    MCKL_RANDOM_INTERNAL_THREEFRY_AVX2_16X64_RBOX(N * 8 + 4);                 \
    MCKL_RANDOM_INTERNAL_THREEFRY_AVX2_16X64_KBOX(N * 8 + 4);                 \
    MCKL_RANDOM_INTERNAL_THREEFRY_AVX2_16X64_RBOX(N * 8 + 5);                 \
    MCKL_RANDOM_INTERNAL_THREEFRY_AVX2_16X64_RBOX(N * 8 + 6);                 \
    MCKL_RANDOM_INTERNAL_THREEFRY_AVX2_16X64_RBOX(N * 8 + 7);                 \
    MCKL_RANDOM_INTERNAL_THREEFRY_AVX2_16X64_RBOX(N * 8 + 8);                 \
    MCKL_RANDOM_INTERNAL_THREEFRY_AVX2_16X64_KBOX(N * 8 + 8);

namespace mckl {

namespace internal {

template <typename T>
class Threefry16x64GeneratorAVX2Impl
{
    static_assert(std::numeric_limits<T>::digits == 64,
        "**Threefry16x64GeneratorAVX2Impl** used with T other than a 64-bit "
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
        constexpr std::size_t N = sizeof(__m256i) * S / (sizeof(T) * K);

        __m256i ymmc0 =
            _mm256_set_epi64x(static_cast<MCKL_INT64>(std::get<0x3>(ctr)),
                static_cast<MCKL_INT64>(std::get<0x2>(ctr)),
                static_cast<MCKL_INT64>(std::get<0x1>(ctr)),
                static_cast<MCKL_INT64>(std::get<0x0>(ctr)));
        __m256i ymmc1 =
            _mm256_set_epi64x(static_cast<MCKL_INT64>(std::get<0x7>(ctr)),
                static_cast<MCKL_INT64>(std::get<0x6>(ctr)),
                static_cast<MCKL_INT64>(std::get<0x5>(ctr)),
                static_cast<MCKL_INT64>(std::get<0x4>(ctr)));
        __m256i ymmc2 =
            _mm256_set_epi64x(static_cast<MCKL_INT64>(std::get<0xB>(ctr)),
                static_cast<MCKL_INT64>(std::get<0xA>(ctr)),
                static_cast<MCKL_INT64>(std::get<0x9>(ctr)),
                static_cast<MCKL_INT64>(std::get<0x8>(ctr)));
        __m256i ymmc3 =
            _mm256_set_epi64x(static_cast<MCKL_INT64>(std::get<0xF>(ctr)),
                static_cast<MCKL_INT64>(std::get<0xE>(ctr)),
                static_cast<MCKL_INT64>(std::get<0xD>(ctr)),
                static_cast<MCKL_INT64>(std::get<0xC>(ctr)));
        ctr.front() += n;

        __m256i *rptr = reinterpret_cast<__m256i *>(r);
        while (n != 0) {
            __m256i ymms0 =
                _mm256_add_epi64(ymmc0, _mm256_set_epi64x(0, 0, 0, 1));
            __m256i ymms4 =
                _mm256_add_epi64(ymmc0, _mm256_set_epi64x(0, 0, 0, 2));
            __m256i ymms8 =
                _mm256_add_epi64(ymmc0, _mm256_set_epi64x(0, 0, 0, 3));
            __m256i ymmsC =
                _mm256_add_epi64(ymmc0, _mm256_set_epi64x(0, 0, 0, 4));
            ymmc0 = ymmsC;

            __m256i ymms1 = ymmc1;
            __m256i ymms2 = ymmc2;
            __m256i ymms3 = ymmc3;
            __m256i ymms5 = ymmc1;
            __m256i ymms6 = ymmc2;
            __m256i ymms7 = ymmc3;
            __m256i ymms9 = ymmc1;
            __m256i ymmsA = ymmc2;
            __m256i ymmsB = ymmc3;
            __m256i ymmsD = ymmc1;
            __m256i ymmsE = ymmc2;
            __m256i ymmsF = ymmc3;

            __m256i ymmt0;
            __m256i ymmt1;
            __m256i ymmt2;
            __m256i ymmt3;
            __m256i ymmt4;
            __m256i ymmt5;
            __m256i ymmt6;
            __m256i ymmt7;
            __m256i ymmt8;
            __m256i ymmt9;
            __m256i ymmtA;
            __m256i ymmtB;
            __m256i ymmtC;
            __m256i ymmtD;
            __m256i ymmtE;
            __m256i ymmtF;

            ymmt1 = ymms1;
            ymmt2 = ymms2;
            ymmt3 = ymms3;
            ymmt6 = ymms6;
            ymmt7 = ymms7;
            ymmtB = ymmsB;
            ymms1 = ymms4;
            ymms2 = ymms8;
            ymms3 = ymmsC;
            ymms6 = ymms9;
            ymms7 = ymmsD;
            ymmsB = ymmsE;
            ymms4 = ymmt1;
            ymms8 = ymmt2;
            ymmsC = ymmt3;
            ymms9 = ymmt6;
            ymmsD = ymmt7;
            ymmsE = ymmtB;

            ymmt0 = _mm256_unpacklo_epi64(ymms0, ymms1);
            ymmt1 = _mm256_unpacklo_epi64(ymms2, ymms3);
            ymmt2 = _mm256_unpackhi_epi64(ymms0, ymms1);
            ymmt3 = _mm256_unpackhi_epi64(ymms2, ymms3);
            ymmt4 = _mm256_unpacklo_epi64(ymms4, ymms5);
            ymmt5 = _mm256_unpacklo_epi64(ymms6, ymms7);
            ymmt6 = _mm256_unpackhi_epi64(ymms4, ymms5);
            ymmt7 = _mm256_unpackhi_epi64(ymms6, ymms7);
            ymmt8 = _mm256_unpacklo_epi64(ymms8, ymms9);
            ymmt9 = _mm256_unpacklo_epi64(ymmsA, ymmsB);
            ymmtA = _mm256_unpackhi_epi64(ymms8, ymms9);
            ymmtB = _mm256_unpackhi_epi64(ymmsA, ymmsB);
            ymmtC = _mm256_unpacklo_epi64(ymmsC, ymmsD);
            ymmtD = _mm256_unpacklo_epi64(ymmsE, ymmsF);
            ymmtE = _mm256_unpackhi_epi64(ymmsC, ymmsD);
            ymmtF = _mm256_unpackhi_epi64(ymmsE, ymmsF);
            ymms0 = _mm256_permute2x128_si256(ymmt0, ymmt1, 0x20);
            ymms1 = _mm256_permute2x128_si256(ymmt2, ymmt3, 0x20);
            ymms2 = _mm256_permute2x128_si256(ymmt0, ymmt1, 0x31);
            ymms3 = _mm256_permute2x128_si256(ymmt2, ymmt3, 0x31);
            ymms4 = _mm256_permute2x128_si256(ymmt4, ymmt5, 0x20);
            ymms5 = _mm256_permute2x128_si256(ymmt6, ymmt7, 0x20);
            ymms6 = _mm256_permute2x128_si256(ymmt4, ymmt5, 0x31);
            ymms7 = _mm256_permute2x128_si256(ymmt6, ymmt7, 0x31);
            ymms8 = _mm256_permute2x128_si256(ymmt8, ymmt9, 0x20);
            ymms9 = _mm256_permute2x128_si256(ymmtA, ymmtB, 0x20);
            ymmsA = _mm256_permute2x128_si256(ymmt8, ymmt9, 0x31);
            ymmsB = _mm256_permute2x128_si256(ymmtA, ymmtB, 0x31);
            ymmsC = _mm256_permute2x128_si256(ymmtC, ymmtD, 0x20);
            ymmsD = _mm256_permute2x128_si256(ymmtE, ymmtF, 0x20);
            ymmsE = _mm256_permute2x128_si256(ymmtC, ymmtD, 0x31);
            ymmsF = _mm256_permute2x128_si256(ymmtE, ymmtF, 0x31);

            MCKL_RANDOM_INTERNAL_THREEFRY_AVX2_16X64_KBOX(0);
            MCKL_RANDOM_INTERNAL_THREEFRY_AVX2_16X64_CYCLE_8(0);
            MCKL_RANDOM_INTERNAL_THREEFRY_AVX2_16X64_CYCLE_8(1);
            MCKL_RANDOM_INTERNAL_THREEFRY_AVX2_16X64_CYCLE_4(2);

            ymmt0 = _mm256_unpacklo_epi64(ymms0, ymms1);
            ymmt1 = _mm256_unpacklo_epi64(ymms2, ymms3);
            ymmt2 = _mm256_unpackhi_epi64(ymms0, ymms1);
            ymmt3 = _mm256_unpackhi_epi64(ymms2, ymms3);
            ymmt4 = _mm256_unpacklo_epi64(ymms4, ymms5);
            ymmt5 = _mm256_unpacklo_epi64(ymms6, ymms7);
            ymmt6 = _mm256_unpackhi_epi64(ymms4, ymms5);
            ymmt7 = _mm256_unpackhi_epi64(ymms6, ymms7);
            ymmt8 = _mm256_unpacklo_epi64(ymms8, ymms9);
            ymmt9 = _mm256_unpacklo_epi64(ymmsA, ymmsB);
            ymmtA = _mm256_unpackhi_epi64(ymms8, ymms9);
            ymmtB = _mm256_unpackhi_epi64(ymmsA, ymmsB);
            ymmtC = _mm256_unpacklo_epi64(ymmsC, ymmsD);
            ymmtD = _mm256_unpacklo_epi64(ymmsE, ymmsF);
            ymmtE = _mm256_unpackhi_epi64(ymmsC, ymmsD);
            ymmtF = _mm256_unpackhi_epi64(ymmsE, ymmsF);
            ymms0 = _mm256_permute2x128_si256(ymmt0, ymmt1, 0x20);
            ymms1 = _mm256_permute2x128_si256(ymmt2, ymmt3, 0x20);
            ymms2 = _mm256_permute2x128_si256(ymmt0, ymmt1, 0x31);
            ymms3 = _mm256_permute2x128_si256(ymmt2, ymmt3, 0x31);
            ymms4 = _mm256_permute2x128_si256(ymmt4, ymmt5, 0x20);
            ymms5 = _mm256_permute2x128_si256(ymmt6, ymmt7, 0x20);
            ymms6 = _mm256_permute2x128_si256(ymmt4, ymmt5, 0x31);
            ymms7 = _mm256_permute2x128_si256(ymmt6, ymmt7, 0x31);
            ymms8 = _mm256_permute2x128_si256(ymmt8, ymmt9, 0x20);
            ymms9 = _mm256_permute2x128_si256(ymmtA, ymmtB, 0x20);
            ymmsA = _mm256_permute2x128_si256(ymmt8, ymmt9, 0x31);
            ymmsB = _mm256_permute2x128_si256(ymmtA, ymmtB, 0x31);
            ymmsC = _mm256_permute2x128_si256(ymmtC, ymmtD, 0x20);
            ymmsD = _mm256_permute2x128_si256(ymmtE, ymmtF, 0x20);
            ymmsE = _mm256_permute2x128_si256(ymmtC, ymmtD, 0x31);
            ymmsF = _mm256_permute2x128_si256(ymmtE, ymmtF, 0x31);

            ymmt1 = ymms1;
            ymmt2 = ymms2;
            ymmt3 = ymms3;
            ymmt6 = ymms6;
            ymmt7 = ymms7;
            ymmtB = ymmsB;
            ymms1 = ymms4;
            ymms2 = ymms8;
            ymms3 = ymmsC;
            ymms6 = ymms9;
            ymms7 = ymmsD;
            ymmsB = ymmsE;
            ymms4 = ymmt1;
            ymms8 = ymmt2;
            ymmsC = ymmt3;
            ymms9 = ymmt6;
            ymmsD = ymmt7;
            ymmsE = ymmtB;

            if (n >= N) {
                n -= N;
                _mm256_storeu_si256(rptr++, ymms0);
                _mm256_storeu_si256(rptr++, ymms1);
                _mm256_storeu_si256(rptr++, ymms2);
                _mm256_storeu_si256(rptr++, ymms3);
                _mm256_storeu_si256(rptr++, ymms4);
                _mm256_storeu_si256(rptr++, ymms5);
                _mm256_storeu_si256(rptr++, ymms6);
                _mm256_storeu_si256(rptr++, ymms7);
                _mm256_storeu_si256(rptr++, ymms8);
                _mm256_storeu_si256(rptr++, ymms9);
                _mm256_storeu_si256(rptr++, ymmsA);
                _mm256_storeu_si256(rptr++, ymmsB);
                _mm256_storeu_si256(rptr++, ymmsC);
                _mm256_storeu_si256(rptr++, ymmsD);
                _mm256_storeu_si256(rptr++, ymmsE);
                _mm256_storeu_si256(rptr++, ymmsF);
            } else {
                std::array<__m256i, S> s;
                std::get<0x0>(s) = ymms0;
                std::get<0x1>(s) = ymms1;
                std::get<0x2>(s) = ymms2;
                std::get<0x3>(s) = ymms3;
                std::get<0x4>(s) = ymms4;
                std::get<0x5>(s) = ymms5;
                std::get<0x6>(s) = ymms6;
                std::get<0x7>(s) = ymms7;
                std::get<0x8>(s) = ymms8;
                std::get<0x9>(s) = ymms9;
                std::get<0xA>(s) = ymmsA;
                std::get<0xB>(s) = ymmsB;
                std::get<0xC>(s) = ymmsC;
                std::get<0xD>(s) = ymmsD;
                std::get<0xE>(s) = ymmsE;
                std::get<0xF>(s) = ymmsF;
                std::memcpy(rptr, s.data(), n * sizeof(T) * K);
                break;
            }
        }
    }
}; // class Threefry16x64GeneratorAVX2Impl

template <typename T>
class Threefish1024GeneratorAVX2Impl
{
    static_assert(std::numeric_limits<T>::digits == 64,
        "**Threefish1024GeneratorAVX2Impl** used with T other than a 64-bit "
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
        constexpr std::size_t N = sizeof(__m256i) * S / (sizeof(T) * K);

        __m256i ymmc0 =
            _mm256_set_epi64x(static_cast<MCKL_INT64>(std::get<0x3>(ctr)),
                static_cast<MCKL_INT64>(std::get<0x2>(ctr)),
                static_cast<MCKL_INT64>(std::get<0x1>(ctr)),
                static_cast<MCKL_INT64>(std::get<0x0>(ctr)));
        __m256i ymmc1 =
            _mm256_set_epi64x(static_cast<MCKL_INT64>(std::get<0x7>(ctr)),
                static_cast<MCKL_INT64>(std::get<0x6>(ctr)),
                static_cast<MCKL_INT64>(std::get<0x5>(ctr)),
                static_cast<MCKL_INT64>(std::get<0x4>(ctr)));
        __m256i ymmc2 =
            _mm256_set_epi64x(static_cast<MCKL_INT64>(std::get<0xB>(ctr)),
                static_cast<MCKL_INT64>(std::get<0xA>(ctr)),
                static_cast<MCKL_INT64>(std::get<0x9>(ctr)),
                static_cast<MCKL_INT64>(std::get<0x8>(ctr)));
        __m256i ymmc3 =
            _mm256_set_epi64x(static_cast<MCKL_INT64>(std::get<0xF>(ctr)),
                static_cast<MCKL_INT64>(std::get<0xE>(ctr)),
                static_cast<MCKL_INT64>(std::get<0xD>(ctr)),
                static_cast<MCKL_INT64>(std::get<0xC>(ctr)));
        ctr.front() += n;

        __m256i *rptr = reinterpret_cast<__m256i *>(r);
        while (n != 0) {
            __m256i ymms0 =
                _mm256_add_epi64(ymmc0, _mm256_set_epi64x(0, 0, 0, 1));
            __m256i ymms4 =
                _mm256_add_epi64(ymmc0, _mm256_set_epi64x(0, 0, 0, 2));
            __m256i ymms8 =
                _mm256_add_epi64(ymmc0, _mm256_set_epi64x(0, 0, 0, 3));
            __m256i ymmsC =
                _mm256_add_epi64(ymmc0, _mm256_set_epi64x(0, 0, 0, 4));
            ymmc0 = ymmsC;

            __m256i ymms1 = ymmc1;
            __m256i ymms2 = ymmc2;
            __m256i ymms3 = ymmc3;
            __m256i ymms5 = ymmc1;
            __m256i ymms6 = ymmc2;
            __m256i ymms7 = ymmc3;
            __m256i ymms9 = ymmc1;
            __m256i ymmsA = ymmc2;
            __m256i ymmsB = ymmc3;
            __m256i ymmsD = ymmc1;
            __m256i ymmsE = ymmc2;
            __m256i ymmsF = ymmc3;

            __m256i ymmt0;
            __m256i ymmt1;
            __m256i ymmt2;
            __m256i ymmt3;
            __m256i ymmt4;
            __m256i ymmt5;
            __m256i ymmt6;
            __m256i ymmt7;
            __m256i ymmt8;
            __m256i ymmt9;
            __m256i ymmtA;
            __m256i ymmtB;
            __m256i ymmtC;
            __m256i ymmtD;
            __m256i ymmtE;
            __m256i ymmtF;

            ymmt1 = ymms1;
            ymmt2 = ymms2;
            ymmt3 = ymms3;
            ymmt6 = ymms6;
            ymmt7 = ymms7;
            ymmtB = ymmsB;
            ymms1 = ymms4;
            ymms2 = ymms8;
            ymms3 = ymmsC;
            ymms6 = ymms9;
            ymms7 = ymmsD;
            ymmsB = ymmsE;
            ymms4 = ymmt1;
            ymms8 = ymmt2;
            ymmsC = ymmt3;
            ymms9 = ymmt6;
            ymmsD = ymmt7;
            ymmsE = ymmtB;

            ymmt0 = _mm256_unpacklo_epi64(ymms0, ymms1);
            ymmt1 = _mm256_unpacklo_epi64(ymms2, ymms3);
            ymmt2 = _mm256_unpackhi_epi64(ymms0, ymms1);
            ymmt3 = _mm256_unpackhi_epi64(ymms2, ymms3);
            ymmt4 = _mm256_unpacklo_epi64(ymms4, ymms5);
            ymmt5 = _mm256_unpacklo_epi64(ymms6, ymms7);
            ymmt6 = _mm256_unpackhi_epi64(ymms4, ymms5);
            ymmt7 = _mm256_unpackhi_epi64(ymms6, ymms7);
            ymmt8 = _mm256_unpacklo_epi64(ymms8, ymms9);
            ymmt9 = _mm256_unpacklo_epi64(ymmsA, ymmsB);
            ymmtA = _mm256_unpackhi_epi64(ymms8, ymms9);
            ymmtB = _mm256_unpackhi_epi64(ymmsA, ymmsB);
            ymmtC = _mm256_unpacklo_epi64(ymmsC, ymmsD);
            ymmtD = _mm256_unpacklo_epi64(ymmsE, ymmsF);
            ymmtE = _mm256_unpackhi_epi64(ymmsC, ymmsD);
            ymmtF = _mm256_unpackhi_epi64(ymmsE, ymmsF);
            ymms0 = _mm256_permute2x128_si256(ymmt0, ymmt1, 0x20);
            ymms1 = _mm256_permute2x128_si256(ymmt2, ymmt3, 0x20);
            ymms2 = _mm256_permute2x128_si256(ymmt0, ymmt1, 0x31);
            ymms3 = _mm256_permute2x128_si256(ymmt2, ymmt3, 0x31);
            ymms4 = _mm256_permute2x128_si256(ymmt4, ymmt5, 0x20);
            ymms5 = _mm256_permute2x128_si256(ymmt6, ymmt7, 0x20);
            ymms6 = _mm256_permute2x128_si256(ymmt4, ymmt5, 0x31);
            ymms7 = _mm256_permute2x128_si256(ymmt6, ymmt7, 0x31);
            ymms8 = _mm256_permute2x128_si256(ymmt8, ymmt9, 0x20);
            ymms9 = _mm256_permute2x128_si256(ymmtA, ymmtB, 0x20);
            ymmsA = _mm256_permute2x128_si256(ymmt8, ymmt9, 0x31);
            ymmsB = _mm256_permute2x128_si256(ymmtA, ymmtB, 0x31);
            ymmsC = _mm256_permute2x128_si256(ymmtC, ymmtD, 0x20);
            ymmsD = _mm256_permute2x128_si256(ymmtE, ymmtF, 0x20);
            ymmsE = _mm256_permute2x128_si256(ymmtC, ymmtD, 0x31);
            ymmsF = _mm256_permute2x128_si256(ymmtE, ymmtF, 0x31);

            MCKL_RANDOM_INTERNAL_THREEFRY_AVX2_16X64_KBOX(0);
            MCKL_RANDOM_INTERNAL_THREEFRY_AVX2_16X64_CYCLE_8(0);
            MCKL_RANDOM_INTERNAL_THREEFRY_AVX2_16X64_CYCLE_8(1);
            MCKL_RANDOM_INTERNAL_THREEFRY_AVX2_16X64_CYCLE_8(2);
            MCKL_RANDOM_INTERNAL_THREEFRY_AVX2_16X64_CYCLE_8(3);
            MCKL_RANDOM_INTERNAL_THREEFRY_AVX2_16X64_CYCLE_8(4);
            MCKL_RANDOM_INTERNAL_THREEFRY_AVX2_16X64_CYCLE_8(5);
            MCKL_RANDOM_INTERNAL_THREEFRY_AVX2_16X64_CYCLE_8(6);
            MCKL_RANDOM_INTERNAL_THREEFRY_AVX2_16X64_CYCLE_8(7);
            MCKL_RANDOM_INTERNAL_THREEFRY_AVX2_16X64_CYCLE_8(8);
            MCKL_RANDOM_INTERNAL_THREEFRY_AVX2_16X64_CYCLE_8(9);

            ymmt0 = _mm256_unpacklo_epi64(ymms0, ymms1);
            ymmt1 = _mm256_unpacklo_epi64(ymms2, ymms3);
            ymmt2 = _mm256_unpackhi_epi64(ymms0, ymms1);
            ymmt3 = _mm256_unpackhi_epi64(ymms2, ymms3);
            ymmt4 = _mm256_unpacklo_epi64(ymms4, ymms5);
            ymmt5 = _mm256_unpacklo_epi64(ymms6, ymms7);
            ymmt6 = _mm256_unpackhi_epi64(ymms4, ymms5);
            ymmt7 = _mm256_unpackhi_epi64(ymms6, ymms7);
            ymmt8 = _mm256_unpacklo_epi64(ymms8, ymms9);
            ymmt9 = _mm256_unpacklo_epi64(ymmsA, ymmsB);
            ymmtA = _mm256_unpackhi_epi64(ymms8, ymms9);
            ymmtB = _mm256_unpackhi_epi64(ymmsA, ymmsB);
            ymmtC = _mm256_unpacklo_epi64(ymmsC, ymmsD);
            ymmtD = _mm256_unpacklo_epi64(ymmsE, ymmsF);
            ymmtE = _mm256_unpackhi_epi64(ymmsC, ymmsD);
            ymmtF = _mm256_unpackhi_epi64(ymmsE, ymmsF);
            ymms0 = _mm256_permute2x128_si256(ymmt0, ymmt1, 0x20);
            ymms1 = _mm256_permute2x128_si256(ymmt2, ymmt3, 0x20);
            ymms2 = _mm256_permute2x128_si256(ymmt0, ymmt1, 0x31);
            ymms3 = _mm256_permute2x128_si256(ymmt2, ymmt3, 0x31);
            ymms4 = _mm256_permute2x128_si256(ymmt4, ymmt5, 0x20);
            ymms5 = _mm256_permute2x128_si256(ymmt6, ymmt7, 0x20);
            ymms6 = _mm256_permute2x128_si256(ymmt4, ymmt5, 0x31);
            ymms7 = _mm256_permute2x128_si256(ymmt6, ymmt7, 0x31);
            ymms8 = _mm256_permute2x128_si256(ymmt8, ymmt9, 0x20);
            ymms9 = _mm256_permute2x128_si256(ymmtA, ymmtB, 0x20);
            ymmsA = _mm256_permute2x128_si256(ymmt8, ymmt9, 0x31);
            ymmsB = _mm256_permute2x128_si256(ymmtA, ymmtB, 0x31);
            ymmsC = _mm256_permute2x128_si256(ymmtC, ymmtD, 0x20);
            ymmsD = _mm256_permute2x128_si256(ymmtE, ymmtF, 0x20);
            ymmsE = _mm256_permute2x128_si256(ymmtC, ymmtD, 0x31);
            ymmsF = _mm256_permute2x128_si256(ymmtE, ymmtF, 0x31);

            ymmt1 = ymms1;
            ymmt2 = ymms2;
            ymmt3 = ymms3;
            ymmt6 = ymms6;
            ymmt7 = ymms7;
            ymmtB = ymmsB;
            ymms1 = ymms4;
            ymms2 = ymms8;
            ymms3 = ymmsC;
            ymms6 = ymms9;
            ymms7 = ymmsD;
            ymmsB = ymmsE;
            ymms4 = ymmt1;
            ymms8 = ymmt2;
            ymmsC = ymmt3;
            ymms9 = ymmt6;
            ymmsD = ymmt7;
            ymmsE = ymmtB;

            if (n >= N) {
                n -= N;
                _mm256_storeu_si256(rptr++, ymms0);
                _mm256_storeu_si256(rptr++, ymms1);
                _mm256_storeu_si256(rptr++, ymms2);
                _mm256_storeu_si256(rptr++, ymms3);
                _mm256_storeu_si256(rptr++, ymms4);
                _mm256_storeu_si256(rptr++, ymms5);
                _mm256_storeu_si256(rptr++, ymms6);
                _mm256_storeu_si256(rptr++, ymms7);
                _mm256_storeu_si256(rptr++, ymms8);
                _mm256_storeu_si256(rptr++, ymms9);
                _mm256_storeu_si256(rptr++, ymmsA);
                _mm256_storeu_si256(rptr++, ymmsB);
                _mm256_storeu_si256(rptr++, ymmsC);
                _mm256_storeu_si256(rptr++, ymmsD);
                _mm256_storeu_si256(rptr++, ymmsE);
                _mm256_storeu_si256(rptr++, ymmsF);
            } else {
                std::array<__m256i, S> s;
                std::get<0x0>(s) = ymms0;
                std::get<0x1>(s) = ymms1;
                std::get<0x2>(s) = ymms2;
                std::get<0x3>(s) = ymms3;
                std::get<0x4>(s) = ymms4;
                std::get<0x5>(s) = ymms5;
                std::get<0x6>(s) = ymms6;
                std::get<0x7>(s) = ymms7;
                std::get<0x8>(s) = ymms8;
                std::get<0x9>(s) = ymms9;
                std::get<0xA>(s) = ymmsA;
                std::get<0xB>(s) = ymmsB;
                std::get<0xC>(s) = ymmsC;
                std::get<0xD>(s) = ymmsD;
                std::get<0xE>(s) = ymmsE;
                std::get<0xF>(s) = ymmsF;
                std::memcpy(rptr, s.data(), n * sizeof(T) * K);
                break;
            }
        }
    }
}; // class Threefish1024GeneratorAVX2Impl

} // namespace internal

} // namespace mckl

MCKL_POP_GCC_WARNING

#endif // MCKL_RANDOM_INTERNAL_THREEFRY_AVX2_16X64_HPP
