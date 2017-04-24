//============================================================================
// MCKL/include/mckl/random/internal/threefry_avx2_32.hpp
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

#ifndef MCKL_RANDOM_INTERNAL_THREEFRY_AVX2_32_HPP
#define MCKL_RANDOM_INTERNAL_THREEFRY_AVX2_32_HPP

#include <mckl/random/internal/common.hpp>
#include <mckl/random/internal/threefry_avx2_2x32.hpp>
#include <mckl/random/internal/threefry_avx2_4x32.hpp>
#include <mckl/random/internal/threefry_common.hpp>
#include <mckl/random/internal/threefry_constants.hpp>
#include <mckl/random/internal/threefry_generic.hpp>
#include <mckl/random/internal/threefry_unroll.hpp>
#include <mckl/random/increment.hpp>

#ifdef MCKL_GCC
#if MCKL_GCC_VERSION >= 60000
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wignored-attributes"
#endif
#endif

namespace mckl {

namespace internal {

template <typename T, std::size_t K, std::size_t Rounds, typename Constants>
class ThreefryGeneratorAVX2Impl32
{
  public:
    static void eval(
        const void *plain, void *cipher, const std::array<T, K + 4> &par)
    {
        ThreefryGeneratorGenericImpl<T, K, Rounds, Constants>::eval(
            plain, cipher, par);
    }

    template <typename ResultType>
    static void eval(
        Counter<T, K> &ctr, ResultType *r, const std::array<T, K + 4> &par)
    {
        ThreefryGeneratorGenericImpl<T, K, Rounds, Constants>::eval(
            ctr, r, par);
    }

    template <typename ResultType>
    static void eval(Counter<T, K> &ctr, std::size_t n, ResultType *r,
        const std::array<T, K + 4> &par)
    {
        constexpr std::size_t S = K <= 8 ? 8 : K;
        constexpr std::size_t N = sizeof(__m256i) * S / (sizeof(T) * K);
        constexpr std::size_t R = sizeof(T) * K / sizeof(ResultType);

        while (n >= N) {
            std::array<__m256i, S> s;
            MCKL_INLINE_CALL increment_si256(ctr, s);
            MCKL_INLINE_CALL transpose8x32_load_si256(s);
            MCKL_RANDOM_INTERNAL_THREEFRY_UNROLL_ROUND(0, s, par);
            MCKL_INLINE_CALL transpose8x32_store_si256(s);
            std::memcpy(r, s.data(), sizeof(T) * K * N);
            n -= N;
            r += N * R;
        }

        alignas(32) std::array<ResultType, N * R> t;
        ThreefryGeneratorGenericImpl<T, K, Rounds, Constants>::eval(
            ctr, n, t.data(), par);
        std::memcpy(r, t.data(), sizeof(T) * K * n);
    }

  private:
    template <std::size_t, std::size_t S>
    static void round(std::array<__m256i, S> &, const std::array<T, K + 4> &,
        std::false_type)
    {
    }

    template <std::size_t N, std::size_t S>
    static void round(std::array<__m256i, S> &s,
        const std::array<T, K + 4> &par, std::true_type)
    {
        MCKL_RANDOM_INTERNAL_THREEFRY_UNROLL_ROUND(N, s, par);
    }

    template <std::size_t N, std::size_t S>
    MCKL_INLINE static void kbox(
        std::array<__m256i, S> &s, const std::array<T, K + 4> &par)
    {
        kbox<N>(s, par,
            std::integral_constant<bool, (N % 4 == 0 && N <= Rounds)>());
    }

    template <std::size_t, std::size_t S>
    static void kbox(std::array<__m256i, S> &, const std::array<T, K + 4> &,
        std::false_type)
    {
    }

    template <std::size_t N>
    static void kbox(std::array<__m256i, 8> &s,
        const std::array<T, K + 4> &par, std::true_type)
    {
        std::array<__m256i, K> k;
        set_key<N>(k, par);

        std::get<0>(s) = _mm256_add_epi32(std::get<0>(s), std::get<0 % K>(k));
        std::get<1>(s) = _mm256_add_epi32(std::get<1>(s), std::get<1 % K>(k));
        std::get<2>(s) = _mm256_add_epi32(std::get<2>(s), std::get<2 % K>(k));
        std::get<3>(s) = _mm256_add_epi32(std::get<3>(s), std::get<3 % K>(k));
        std::get<4>(s) = _mm256_add_epi32(std::get<4>(s), std::get<4 % K>(k));
        std::get<5>(s) = _mm256_add_epi32(std::get<5>(s), std::get<5 % K>(k));
        std::get<6>(s) = _mm256_add_epi32(std::get<6>(s), std::get<6 % K>(k));
        std::get<7>(s) = _mm256_add_epi32(std::get<7>(s), std::get<7 % K>(k));
    }

    template <std::size_t N>
    static void kbox(std::array<__m256i, 16> &s,
        const std::array<T, K + 4> &par, std::true_type)
    {
        std::array<__m256i, K> k;
        set_key<N>(k, par);

        std::get<0x0>(s) =
            _mm256_add_epi32(std::get<0x0>(s), std::get<0x0 % K>(k));
        std::get<0x1>(s) =
            _mm256_add_epi32(std::get<0x1>(s), std::get<0x1 % K>(k));
        std::get<0x2>(s) =
            _mm256_add_epi32(std::get<0x2>(s), std::get<0x2 % K>(k));
        std::get<0x3>(s) =
            _mm256_add_epi32(std::get<0x3>(s), std::get<0x3 % K>(k));
        std::get<0x4>(s) =
            _mm256_add_epi32(std::get<0x4>(s), std::get<0x4 % K>(k));
        std::get<0x5>(s) =
            _mm256_add_epi32(std::get<0x5>(s), std::get<0x5 % K>(k));
        std::get<0x6>(s) =
            _mm256_add_epi32(std::get<0x6>(s), std::get<0x6 % K>(k));
        std::get<0x7>(s) =
            _mm256_add_epi32(std::get<0x7>(s), std::get<0x7 % K>(k));
        std::get<0x8>(s) =
            _mm256_add_epi32(std::get<0x8>(s), std::get<0x8 % K>(k));
        std::get<0x9>(s) =
            _mm256_add_epi32(std::get<0x9>(s), std::get<0x9 % K>(k));
        std::get<0xA>(s) =
            _mm256_add_epi32(std::get<0xA>(s), std::get<0xA % K>(k));
        std::get<0xB>(s) =
            _mm256_add_epi32(std::get<0xB>(s), std::get<0xB % K>(k));
        std::get<0xC>(s) =
            _mm256_add_epi32(std::get<0xC>(s), std::get<0xC % K>(k));
        std::get<0xD>(s) =
            _mm256_add_epi32(std::get<0xD>(s), std::get<0xD % K>(k));
        std::get<0xE>(s) =
            _mm256_add_epi32(std::get<0xE>(s), std::get<0xE % K>(k));
        std::get<0xF>(s) =
            _mm256_add_epi32(std::get<0xF>(s), std::get<0xF % K>(k));
    }

    template <std::size_t N, std::size_t S>
    MCKL_INLINE static void rbox(std::array<__m256i, S> &s)
    {
        rbox<N>(s, std::integral_constant<bool, (N > 0 && N <= Rounds)>());
    }

    template <std::size_t, std::size_t S>
    static void rbox(std::array<__m256i, S> &, std::false_type)
    {
    }

    template <std::size_t N>
    static void rbox(std::array<__m256i, 8> &s, std::true_type)
    {
        constexpr int L0 = Constants::rotate::value[0 % (K / 2)][(N - 1) % 8];
        constexpr int L1 = Constants::rotate::value[1 % (K / 2)][(N - 1) % 8];
        constexpr int L2 = Constants::rotate::value[2 % (K / 2)][(N - 1) % 8];
        constexpr int L3 = Constants::rotate::value[3 % (K / 2)][(N - 1) % 8];
        constexpr int R0 = 32 - L0;
        constexpr int R1 = 32 - L1;
        constexpr int R2 = 32 - L2;
        constexpr int R3 = 32 - L3;

        std::get<0>(s) = _mm256_add_epi32(std::get<0>(s), std::get<1>(s));
        std::get<2>(s) = _mm256_add_epi32(std::get<2>(s), std::get<3>(s));
        std::get<4>(s) = _mm256_add_epi32(std::get<4>(s), std::get<5>(s));
        std::get<6>(s) = _mm256_add_epi32(std::get<6>(s), std::get<7>(s));

        __m256i l0 = _mm256_slli_epi32(std::get<1>(s), L0);
        __m256i l1 = _mm256_slli_epi32(std::get<3>(s), L1);
        __m256i l2 = _mm256_slli_epi32(std::get<5>(s), L2);
        __m256i l3 = _mm256_slli_epi32(std::get<7>(s), L3);

        __m256i r0 = _mm256_srli_epi32(std::get<1>(s), R0);
        __m256i r1 = _mm256_srli_epi32(std::get<3>(s), R1);
        __m256i r2 = _mm256_srli_epi32(std::get<5>(s), R2);
        __m256i r3 = _mm256_srli_epi32(std::get<7>(s), R3);

        std::get<1>(s) = _mm256_or_si256(l0, r0);
        std::get<3>(s) = _mm256_or_si256(l1, r1);
        std::get<5>(s) = _mm256_or_si256(l2, r2);
        std::get<7>(s) = _mm256_or_si256(l3, r3);

        std::get<1>(s) = _mm256_xor_si256(std::get<0>(s), std::get<1>(s));
        std::get<3>(s) = _mm256_xor_si256(std::get<2>(s), std::get<3>(s));
        std::get<5>(s) = _mm256_xor_si256(std::get<4>(s), std::get<5>(s));
        std::get<7>(s) = _mm256_xor_si256(std::get<6>(s), std::get<7>(s));

        permute(s);
    }

    template <std::size_t N>
    static void rbox(std::array<__m256i, 16> &s, std::true_type)
    {
        constexpr int L0 = Constants::rotate::value[0 % (K / 2)][(N - 1) % 8];
        constexpr int L1 = Constants::rotate::value[1 % (K / 2)][(N - 1) % 8];
        constexpr int L2 = Constants::rotate::value[2 % (K / 2)][(N - 1) % 8];
        constexpr int L3 = Constants::rotate::value[3 % (K / 2)][(N - 1) % 8];
        constexpr int L4 = Constants::rotate::value[4 % (K / 2)][(N - 1) % 8];
        constexpr int L5 = Constants::rotate::value[5 % (K / 2)][(N - 1) % 8];
        constexpr int L6 = Constants::rotate::value[6 % (K / 2)][(N - 1) % 8];
        constexpr int L7 = Constants::rotate::value[7 % (K / 2)][(N - 1) % 8];
        constexpr int R0 = 32 - L0;
        constexpr int R1 = 32 - L1;
        constexpr int R2 = 32 - L2;
        constexpr int R3 = 32 - L3;
        constexpr int R4 = 32 - L4;
        constexpr int R5 = 32 - L5;
        constexpr int R6 = 32 - L6;
        constexpr int R7 = 32 - L7;

        std::get<0x0>(s) =
            _mm256_add_epi32(std::get<0x0>(s), std::get<0x1>(s));
        std::get<0x2>(s) =
            _mm256_add_epi32(std::get<0x2>(s), std::get<0x3>(s));
        std::get<0x4>(s) =
            _mm256_add_epi32(std::get<0x4>(s), std::get<0x5>(s));
        std::get<0x6>(s) =
            _mm256_add_epi32(std::get<0x6>(s), std::get<0x7>(s));
        std::get<0x8>(s) =
            _mm256_add_epi32(std::get<0x8>(s), std::get<0x9>(s));
        std::get<0xA>(s) =
            _mm256_add_epi32(std::get<0xA>(s), std::get<0xB>(s));
        std::get<0xC>(s) =
            _mm256_add_epi32(std::get<0xC>(s), std::get<0xD>(s));
        std::get<0xE>(s) =
            _mm256_add_epi32(std::get<0xE>(s), std::get<0xF>(s));

        __m256i l0 = _mm256_slli_epi32(std::get<0x1>(s), L0);
        __m256i l1 = _mm256_slli_epi32(std::get<0x3>(s), L1);
        __m256i l2 = _mm256_slli_epi32(std::get<0x5>(s), L2);
        __m256i l3 = _mm256_slli_epi32(std::get<0x7>(s), L3);
        __m256i l4 = _mm256_slli_epi32(std::get<0x9>(s), L4);
        __m256i l5 = _mm256_slli_epi32(std::get<0xB>(s), L5);
        __m256i l6 = _mm256_slli_epi32(std::get<0xD>(s), L6);
        __m256i l7 = _mm256_slli_epi32(std::get<0xF>(s), L7);

        std::get<0x1>(s) = _mm256_srli_epi32(std::get<0x1>(s), R0);
        std::get<0x3>(s) = _mm256_srli_epi32(std::get<0x3>(s), R1);
        std::get<0x5>(s) = _mm256_srli_epi32(std::get<0x5>(s), R2);
        std::get<0x7>(s) = _mm256_srli_epi32(std::get<0x7>(s), R3);
        std::get<0x9>(s) = _mm256_srli_epi32(std::get<0x9>(s), R4);
        std::get<0xB>(s) = _mm256_srli_epi32(std::get<0xB>(s), R5);
        std::get<0xD>(s) = _mm256_srli_epi32(std::get<0xD>(s), R6);
        std::get<0xF>(s) = _mm256_srli_epi32(std::get<0xF>(s), R7);

        std::get<0x1>(s) = _mm256_or_si256(l0, std::get<0x1>(s));
        std::get<0x3>(s) = _mm256_or_si256(l1, std::get<0x3>(s));
        std::get<0x5>(s) = _mm256_or_si256(l2, std::get<0x5>(s));
        std::get<0x7>(s) = _mm256_or_si256(l3, std::get<0x7>(s));
        std::get<0x9>(s) = _mm256_or_si256(l4, std::get<0x9>(s));
        std::get<0xB>(s) = _mm256_or_si256(l5, std::get<0xB>(s));
        std::get<0xD>(s) = _mm256_or_si256(l6, std::get<0xD>(s));
        std::get<0xF>(s) = _mm256_or_si256(l7, std::get<0xF>(s));

        std::get<0x1>(s) =
            _mm256_xor_si256(std::get<0x0>(s), std::get<0x1>(s));
        std::get<0x3>(s) =
            _mm256_xor_si256(std::get<0x2>(s), std::get<0x3>(s));
        std::get<0x5>(s) =
            _mm256_xor_si256(std::get<0x4>(s), std::get<0x5>(s));
        std::get<0x7>(s) =
            _mm256_xor_si256(std::get<0x6>(s), std::get<0x7>(s));
        std::get<0x9>(s) =
            _mm256_xor_si256(std::get<0x8>(s), std::get<0x9>(s));
        std::get<0xB>(s) =
            _mm256_xor_si256(std::get<0xA>(s), std::get<0xB>(s));
        std::get<0xD>(s) =
            _mm256_xor_si256(std::get<0xC>(s), std::get<0xD>(s));
        std::get<0xF>(s) =
            _mm256_xor_si256(std::get<0xE>(s), std::get<0xF>(s));

        permute(s);
    }

    template <std::size_t N>
    static void set_key(std::array<__m256i, 2> &k, const std::array<T, 6> &par)
    {
        std::get<0>(k) = _mm256_set1_epi32(
            static_cast<int>(ThreefryKBox<T, 2, N>::template key<0>(par)));
        std::get<1>(k) = _mm256_set1_epi32(
            static_cast<int>(ThreefryKBox<T, 2, N>::template key<1>(par)));
    }

    template <std::size_t N>
    static void set_key(std::array<__m256i, 4> &k, const std::array<T, 8> &par)
    {
        std::get<0>(k) = _mm256_set1_epi32(
            static_cast<int>(ThreefryKBox<T, 4, N>::template key<0>(par)));
        std::get<1>(k) = _mm256_set1_epi32(
            static_cast<int>(ThreefryKBox<T, 4, N>::template key<1>(par)));
        std::get<2>(k) = _mm256_set1_epi32(
            static_cast<int>(ThreefryKBox<T, 4, N>::template key<2>(par)));
        std::get<3>(k) = _mm256_set1_epi32(
            static_cast<int>(ThreefryKBox<T, 4, N>::template key<3>(par)));
    }

    template <std::size_t N>
    static void set_key(
        std::array<__m256i, 8> &k, const std::array<T, 12> &par)
    {
        std::get<0>(k) = _mm256_set1_epi32(
            static_cast<int>(ThreefryKBox<T, 8, N>::template key<0>(par)));
        std::get<1>(k) = _mm256_set1_epi32(
            static_cast<int>(ThreefryKBox<T, 8, N>::template key<1>(par)));
        std::get<2>(k) = _mm256_set1_epi32(
            static_cast<int>(ThreefryKBox<T, 8, N>::template key<2>(par)));
        std::get<3>(k) = _mm256_set1_epi32(
            static_cast<int>(ThreefryKBox<T, 8, N>::template key<3>(par)));
        std::get<4>(k) = _mm256_set1_epi32(
            static_cast<int>(ThreefryKBox<T, 8, N>::template key<4>(par)));
        std::get<5>(k) = _mm256_set1_epi32(
            static_cast<int>(ThreefryKBox<T, 8, N>::template key<5>(par)));
        std::get<6>(k) = _mm256_set1_epi32(
            static_cast<int>(ThreefryKBox<T, 8, N>::template key<6>(par)));
        std::get<7>(k) = _mm256_set1_epi32(
            static_cast<int>(ThreefryKBox<T, 8, N>::template key<7>(par)));
    }

    template <std::size_t N>
    static void set_key(
        std::array<__m256i, 16> &k, const std::array<T, 20> &par)
    {
        std::get<0x0>(k) = _mm256_set1_epi32(
            static_cast<int>(ThreefryKBox<T, 16, N>::template key<0x0>(par)));
        std::get<0x1>(k) = _mm256_set1_epi32(
            static_cast<int>(ThreefryKBox<T, 16, N>::template key<0x1>(par)));
        std::get<0x2>(k) = _mm256_set1_epi32(
            static_cast<int>(ThreefryKBox<T, 16, N>::template key<0x2>(par)));
        std::get<0x3>(k) = _mm256_set1_epi32(
            static_cast<int>(ThreefryKBox<T, 16, N>::template key<0x3>(par)));
        std::get<0x4>(k) = _mm256_set1_epi32(
            static_cast<int>(ThreefryKBox<T, 16, N>::template key<0x4>(par)));
        std::get<0x5>(k) = _mm256_set1_epi32(
            static_cast<int>(ThreefryKBox<T, 16, N>::template key<0x5>(par)));
        std::get<0x6>(k) = _mm256_set1_epi32(
            static_cast<int>(ThreefryKBox<T, 16, N>::template key<0x6>(par)));
        std::get<0x7>(k) = _mm256_set1_epi32(
            static_cast<int>(ThreefryKBox<T, 16, N>::template key<0x7>(par)));
        std::get<0x8>(k) = _mm256_set1_epi32(
            static_cast<int>(ThreefryKBox<T, 16, N>::template key<0x8>(par)));
        std::get<0x9>(k) = _mm256_set1_epi32(
            static_cast<int>(ThreefryKBox<T, 16, N>::template key<0x9>(par)));
        std::get<0xA>(k) = _mm256_set1_epi32(
            static_cast<int>(ThreefryKBox<T, 16, N>::template key<0xA>(par)));
        std::get<0xB>(k) = _mm256_set1_epi32(
            static_cast<int>(ThreefryKBox<T, 16, N>::template key<0xB>(par)));
        std::get<0xC>(k) = _mm256_set1_epi32(
            static_cast<int>(ThreefryKBox<T, 16, N>::template key<0xC>(par)));
        std::get<0xD>(k) = _mm256_set1_epi32(
            static_cast<int>(ThreefryKBox<T, 16, N>::template key<0xD>(par)));
        std::get<0xE>(k) = _mm256_set1_epi32(
            static_cast<int>(ThreefryKBox<T, 16, N>::template key<0xE>(par)));
        std::get<0xF>(k) = _mm256_set1_epi32(
            static_cast<int>(ThreefryKBox<T, 16, N>::template key<0xF>(par)));
    }

    template <std::size_t S>
    static void permute(std::array<__m256i, S> &s)
    {
        permute<0>(s, std::integral_constant<bool, 0 < S / K>());
    }

    template <std::size_t, std::size_t S>
    static void permute(std::array<__m256i, S> &, std::false_type)
    {
    }

    template <std::size_t I, std::size_t S>
    static void permute(std::array<__m256i, S> &s, std::true_type)
    {
        ThreefryPBox<__m256i, K, Constants>::eval(s.data() + I * K);
        permute<I + 1>(s, std::integral_constant<bool, I + 1 < S / K>());
    }
}; // class ThreefryGeneratorAVX2Impl32

template <typename T>
class ThreefryGeneratorAVX2Impl32<T, 2, 20, ThreefryConstants<T, 2>>
    : public Threefry2x32GeneratorAVX2Impl<T>
{
}; // class ThreefryGeneratorAVX2Impl32

template <typename T>
class ThreefryGeneratorAVX2Impl32<T, 4, 20, ThreefryConstants<T, 4>>
    : public Threefry4x32GeneratorAVX2Impl<T>
{
}; // class ThreefryGeneratorAVX2Impl32

} // namespace mckl::internal

} // namespace mckl

#ifdef MCKL_GCC
#if MCKL_GCC_VERSION >= 60000
#pragma GCC diagnostic pop
#endif
#endif

#endif // MCKL_RANDOM_INTERNAL_THREEFRY_AVX2_32_HPP
