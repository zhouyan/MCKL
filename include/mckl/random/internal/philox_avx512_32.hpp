//============================================================================
// MCKL/include/mckl/random/internal/philox_avx512_32.hpp
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

#ifndef MCKL_RANDOM_INTERNAL_PHILOX_AVX512_32_HPP
#define MCKL_RANDOM_INTERNAL_PHILOX_AVX512_32_HPP

#include <mckl/random/internal/common.hpp>
#include <mckl/random/internal/philox_avx512_2x32.hpp>
#include <mckl/random/internal/philox_avx512_4x32.hpp>
#include <mckl/random/internal/philox_common.hpp>
#include <mckl/random/internal/philox_generic.hpp>
#include <mckl/random/internal/philox_unroll.hpp>
#include <mckl/random/increment.hpp>

MCKL_PUSH_GCC_WARNING("-Wignored-attributes")

namespace mckl {

namespace internal {

template <std::size_t>
class PhiloxGeneratorAVX512Impl32Permute;

template <>
class PhiloxGeneratorAVX512Impl32Permute<2>
{
  public:
    template <std::size_t S>
    MCKL_INLINE static void first(std::array<__m512i, S> &)
    {
    }

    template <std::size_t S>
    MCKL_INLINE static void round(std::array<__m512i, S> &s)
    {
        shuffle_epi32<0xB1>(s);
    }

    template <std::size_t S>
    MCKL_INLINE static void last(std::array<__m512i, S> &s)
    {
        shuffle_epi32<0xB1>(s);
    }
}; // class PhiloxGeneratorAVX512Impl32Permute

template <>
class PhiloxGeneratorAVX512Impl32Permute<4>
{
  public:
    template <std::size_t S>
    MCKL_INLINE static void first(std::array<__m512i, S> &s)
    {
        shuffle_epi32<0xC6>(s);
    }

    template <std::size_t S>
    MCKL_INLINE static void round(std::array<__m512i, S> &s)
    {
        shuffle_epi32<0x93>(s);
    }

    template <std::size_t S>
    MCKL_INLINE static void last(std::array<__m512i, S> &s)
    {
        shuffle_epi32<0xB1>(s);
    }
}; // class PhiloxGeneratorAVX512Impl32Permute

template <typename T, std::size_t K, std::size_t Rounds, typename Constants>
class PhiloxGeneratorAVX512Impl32
{
  public:
    static void eval(
        const void *plain, void *cipher, const std::array<T, K / 2> &key)
    {
        PhiloxGeneratorGenericImpl<T, K, Rounds, Constants>::eval(
            plain, cipher, key);
    }

    template <typename ResultType>
    static void eval(
        Counter<T, K> &ctr, ResultType *r, const std::array<T, K / 2> &key)
    {
        PhiloxGeneratorGenericImpl<T, K, Rounds, Constants>::eval(ctr, r, key);
    }

    template <typename ResultType>
    static void eval(Counter<T, K> &ctr, std::size_t n, ResultType *r,
        const std::array<T, K / 2> &key)
    {
        constexpr std::size_t S = 8;
        constexpr std::size_t N = sizeof(__m512i) * S / (sizeof(T) * K);
        constexpr std::size_t R = sizeof(T) * K / sizeof(ResultType);

        const std::array<__m512i, Rounds> rk(round_key(key));
        while (n >= N) {
            std::array<__m512i, S> s;
            MCKL_INLINE_CALL increment_si512(ctr, s);
            MCKL_INLINE_CALL PhiloxGeneratorAVX512Impl32Permute<K>::first(s);
            MCKL_RANDOM_INTERNAL_PHILOX_UNROLL_ROUND(0, s, rk);
            MCKL_INLINE_CALL PhiloxGeneratorAVX512Impl32Permute<K>::last(s);
            std::memcpy(r, s.data(), sizeof(T) * K * N);
            n -= N;
            r += N * R;
        }

        alignas(MCKL_ALIGNMENT) std::array<ResultType, N * R> t;
        PhiloxGeneratorGenericImpl<T, K, Rounds, Constants>::eval(
            ctr, n, t.data(), key);
        std::memcpy(r, t.data(), sizeof(T) * K * n);
    }

  private:
    template <std::size_t, std::size_t S>
    static void round(std::array<__m512i, S> &,
        const std::array<__m512i, Rounds> &, std::false_type)
    {
    }

    template <std::size_t N, std::size_t S>
    static void round(std::array<__m512i, S> &s,
        const std::array<__m512i, Rounds> &rk, std::true_type)
    {
        MCKL_RANDOM_INTERNAL_PHILOX_UNROLL_ROUND(N, s, rk);
    }

    template <std::size_t N, std::size_t S>
    MCKL_INLINE static void rbox(
        std::array<__m512i, S> &s, const std::array<__m512i, Rounds> &rk)
    {
        rbox<N>(s, rk, std::integral_constant<bool, (N > 0 && N <= Rounds)>());
    }

    template <std::size_t, std::size_t S>
    static void rbox(std::array<__m512i, S> &,
        const std::array<__m512i, Rounds> &, std::false_type)
    {
    }

    template <std::size_t N, std::size_t S>
    static void rbox(std::array<__m512i, S> &s,
        const std::array<__m512i, Rounds> &rk, std::true_type)
    {
        constexpr int mul0 =
            static_cast<int>(Constants::multiplier::value[0 % (K / 2)]);
        constexpr int mul1 =
            static_cast<int>(Constants::multiplier::value[1 % (K / 2)]);
        constexpr int mul2 =
            static_cast<int>(Constants::multiplier::value[2 % (K / 2)]);
        constexpr int mul3 =
            static_cast<int>(Constants::multiplier::value[3 % (K / 2)]);
        constexpr int mul4 =
            static_cast<int>(Constants::multiplier::value[4 % (K / 2)]);
        constexpr int mul5 =
            static_cast<int>(Constants::multiplier::value[5 % (K / 2)]);
        constexpr int mul6 =
            static_cast<int>(Constants::multiplier::value[6 % (K / 2)]);
        constexpr int mul7 =
            static_cast<int>(Constants::multiplier::value[7 % (K / 2)]);
        constexpr int mask = static_cast<int>(0xFFFFFFFF);

        const __m512i m = _mm512_set_epi32(0, mul7, 0, mul6, 0, mul5, 0, mul4,
            0, mul3, 0, mul2, 0, mul1, 0, mul0);
        const __m512i a = _mm512_set_epi32(mask, 0, mask, 0, mask, 0, mask, 0,
            mask, 0, mask, 0, mask, 0, mask, 0);
        const __m512i k = std::get<N - 1>(rk);

        std::array<__m512i, S> t;
        mul_epu32(s, m, t);
        and_si512(s, a);
        xor_si512(s, k);
        xor_si512(s, t);
        permute<N>(s);
    }

    MCKL_INLINE static std::array<__m512i, Rounds> round_key(
        const std::array<T, K / 2> &k)
    {
        const int k0 = static_cast<int>(std::get<0 % (K / 2)>(k));
        const int k1 = static_cast<int>(std::get<1 % (K / 2)>(k));
        const int k2 = static_cast<int>(std::get<2 % (K / 2)>(k));
        const int k3 = static_cast<int>(std::get<3 % (K / 2)>(k));
        const int k4 = static_cast<int>(std::get<4 % (K / 2)>(k));
        const int k5 = static_cast<int>(std::get<5 % (K / 2)>(k));
        const int k6 = static_cast<int>(std::get<6 % (K / 2)>(k));
        const int k7 = static_cast<int>(std::get<7 % (K / 2)>(k));

        std::array<__m512i, Rounds> rk;
        set_key<0>(rk,
            _mm512_set_epi32(
                k7, 0, k6, 0, k5, 0, k4, 0, k3, 0, k2, 0, k1, 0, k0, 0),
            std::true_type());

        return rk;
    }

    template <std::size_t>
    static void set_key(
        std::array<__m512i, Rounds> &, const __m512i &, std::false_type)
    {
    }

    template <std::size_t N>
    static void set_key(
        std::array<__m512i, Rounds> &rk, const __m512i &k, std::true_type)
    {
        constexpr int w0 =
            static_cast<int>(Constants::weyl::value[0 % (K / 2)] * N);
        constexpr int w1 =
            static_cast<int>(Constants::weyl::value[1 % (K / 2)] * N);
        constexpr int w2 =
            static_cast<int>(Constants::weyl::value[2 % (K / 2)] * N);
        constexpr int w3 =
            static_cast<int>(Constants::weyl::value[3 % (K / 2)] * N);
        constexpr int w4 =
            static_cast<int>(Constants::weyl::value[4 % (K / 2)] * N);
        constexpr int w5 =
            static_cast<int>(Constants::weyl::value[5 % (K / 2)] * N);
        constexpr int w6 =
            static_cast<int>(Constants::weyl::value[6 % (K / 2)] * N);
        constexpr int w7 =
            static_cast<int>(Constants::weyl::value[7 % (K / 2)] * N);

        std::get<N>(rk) = _mm512_add_epi32(k,
            _mm512_set_epi32(
                w7, 0, w6, 0, w5, 0, w4, 0, w3, 0, w2, 0, w1, 0, w0, 0));
        set_key<N + 1>(rk, k, std::integral_constant<bool, N + 1 < Rounds>());
    }

    template <std::size_t N, std::size_t S>
    static void permute(std::array<__m512i, S> &s)
    {
        permute(s, std::integral_constant<bool, (N > 0 && N < Rounds)>());
    }

    template <std::size_t S>
    static void permute(std::array<__m512i, S> &, std::false_type)
    {
    }

    template <std::size_t S>
    static void permute(std::array<__m512i, S> &s, std::true_type)
    {
        PhiloxGeneratorAVX512Impl32Permute<K>::round(s);
    }
}; // class PhiloxGeneratorImplAVX512

template <typename T, typename Constants>
class PhiloxGeneratorAVX512Impl32<T, 2, 10, Constants>
    : public Philox2x32GeneratorAVX512Impl<T, Constants>
{
}; // class PhiloxGeneratorAVX512Impl32

template <typename T, typename Constants>
class PhiloxGeneratorAVX512Impl32<T, 4, 10, Constants>
    : public Philox4x32GeneratorAVX512Impl<T, Constants>
{
}; // class PhiloxGeneratorAVX512Impl32

} // namespace internal

} // namespace mckl

MCKL_POP_GCC_WARNING

#endif // MCKL_RANDOM_INTERNAL_PHILOX_AVX512_32_HPP
