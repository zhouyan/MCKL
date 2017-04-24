//============================================================================
// MCKL/include/mckl/random/internal/philox_avx2_32.hpp
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

#ifndef MCKL_RANDOM_INTERNAL_PHILOX_AVX2_32_HPP
#define MCKL_RANDOM_INTERNAL_PHILOX_AVX2_32_HPP

#include <mckl/random/internal/common.hpp>
#include <mckl/random/internal/philox_avx2_2x32.hpp>
#include <mckl/random/internal/philox_avx2_4x32.hpp>
#include <mckl/random/internal/philox_common.hpp>
#include <mckl/random/internal/philox_generic.hpp>
#include <mckl/random/internal/philox_unroll.hpp>
#include <mckl/random/increment.hpp>

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

template <std::size_t>
class PhiloxGeneratorAVX2Impl32Permute;

template <>
class PhiloxGeneratorAVX2Impl32Permute<2>
{
  public:
    template <std::size_t S>
    MCKL_INLINE static void first(std::array<__m256i, S> &)
    {
    }

    template <std::size_t S>
    MCKL_INLINE static void round(std::array<__m256i, S> &s)
    {
        shuffle_epi32<0xB1>(s);
    }

    template <std::size_t S>
    MCKL_INLINE static void last(std::array<__m256i, S> &s)
    {
        shuffle_epi32<0xB1>(s);
    }
}; // class PhiloxGeneratorAVX2Impl32Permute

template <>
class PhiloxGeneratorAVX2Impl32Permute<4>
{
  public:
    template <std::size_t S>
    MCKL_INLINE static void first(std::array<__m256i, S> &s)
    {
        shuffle_epi32<0xC6>(s);
    }

    template <std::size_t S>
    MCKL_INLINE static void round(std::array<__m256i, S> &s)
    {
        shuffle_epi32<0x93>(s);
    }

    template <std::size_t S>
    MCKL_INLINE static void last(std::array<__m256i, S> &s)
    {
        shuffle_epi32<0xB1>(s);
    }
}; // class PhiloxGeneratorAVX2Impl32Permute

template <typename T, std::size_t K, std::size_t Rounds, typename Constants>
class PhiloxGeneratorAVX2Impl32
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
        constexpr std::size_t N = sizeof(__m256i) * S / (sizeof(T) * K);
        constexpr std::size_t R = sizeof(T) * K / sizeof(ResultType);

        const std::array<__m256i, Rounds> rk(round_key(key));
        while (n >= N) {
            std::array<__m256i, S> s;
            MCKL_INLINE_CALL increment_si256(ctr, s);
            MCKL_INLINE_CALL PhiloxGeneratorAVX2Impl32Permute<K>::first(s);
            MCKL_RANDOM_INTERNAL_PHILOX_UNROLL_ROUND(0, s, rk);
            MCKL_INLINE_CALL PhiloxGeneratorAVX2Impl32Permute<K>::last(s);
            std::memcpy(r, s.data(), sizeof(T) * K * N);
            n -= N;
            r += N * R;
        }

        alignas(32) std::array<ResultType, N * R> t;
        PhiloxGeneratorGenericImpl<T, K, Rounds, Constants>::eval(
            ctr, n, t.data(), key);
        std::memcpy(r, t.data(), sizeof(T) * K * n);
    }

  private:
    template <std::size_t, std::size_t S>
    static void round(std::array<__m256i, S> &,
        const std::array<__m256i, Rounds> &, std::false_type)
    {
    }

    template <std::size_t N, std::size_t S>
    static void round(std::array<__m256i, S> &s,
        const std::array<__m256i, Rounds> &rk, std::true_type)
    {
        MCKL_RANDOM_INTERNAL_PHILOX_UNROLL_ROUND(N, s, rk);
    }

    template <std::size_t N, std::size_t S>
    MCKL_INLINE static void rbox(
        std::array<__m256i, S> &s, const std::array<__m256i, Rounds> &rk)
    {
        rbox<N>(s, rk, std::integral_constant<bool, (N > 0 && N <= Rounds)>());
    }

    template <std::size_t, std::size_t S>
    static void rbox(std::array<__m256i, S> &,
        const std::array<__m256i, Rounds> &, std::false_type)
    {
    }

    template <std::size_t N, std::size_t S>
    static void rbox(std::array<__m256i, S> &s,
        const std::array<__m256i, Rounds> &rk, std::true_type)
    {
        constexpr int mul0 =
            static_cast<int>(Constants::multiplier::value[0 % (K / 2)]);
        constexpr int mul1 =
            static_cast<int>(Constants::multiplier::value[1 % (K / 2)]);
        constexpr int mul2 =
            static_cast<int>(Constants::multiplier::value[2 % (K / 2)]);
        constexpr int mul3 =
            static_cast<int>(Constants::multiplier::value[3 % (K / 2)]);
        constexpr int mask = static_cast<int>(0xFFFFFFFF);

        const __m256i m = _mm256_set_epi32(0, mul3, 0, mul2, 0, mul1, 0, mul0);
        const __m256i a = _mm256_set_epi32(mask, 0, mask, 0, mask, 0, mask, 0);
        const __m256i k = std::get<N - 1>(rk);

        std::array<__m256i, S> t;
        mul_epu32(s, m, t);
        and_si256(s, a);
        xor_si256(s, k);
        xor_si256(s, t);
        permute<N>(s);
    }

    MCKL_INLINE static std::array<__m256i, Rounds> round_key(
        const std::array<T, K / 2> &k)
    {
        const int k0 = static_cast<int>(std::get<0 % (K / 2)>(k));
        const int k1 = static_cast<int>(std::get<1 % (K / 2)>(k));
        const int k2 = static_cast<int>(std::get<2 % (K / 2)>(k));
        const int k3 = static_cast<int>(std::get<3 % (K / 2)>(k));

        std::array<__m256i, Rounds> rk;
        set_key<0>(rk, _mm256_set_epi32(k3, 0, k2, 0, k1, 0, k0, 0),
            std::true_type());

        return rk;
    }

    template <std::size_t>
    static void set_key(
        std::array<__m256i, Rounds> &, const __m256i &, std::false_type)
    {
    }

    template <std::size_t N>
    static void set_key(
        std::array<__m256i, Rounds> &rk, const __m256i &k, std::true_type)
    {
        constexpr int w0 =
            static_cast<int>(Constants::weyl::value[0 % (K / 2)] * N);
        constexpr int w1 =
            static_cast<int>(Constants::weyl::value[1 % (K / 2)] * N);
        constexpr int w2 =
            static_cast<int>(Constants::weyl::value[2 % (K / 2)] * N);
        constexpr int w3 =
            static_cast<int>(Constants::weyl::value[3 % (K / 2)] * N);

        std::get<N>(rk) =
            _mm256_add_epi32(k, _mm256_set_epi32(w3, 0, w2, 0, w1, 0, w0, 0));
        set_key<N + 1>(rk, k, std::integral_constant<bool, N + 1 < Rounds>());
    }

    template <std::size_t N, std::size_t S>
    static void permute(std::array<__m256i, S> &s)
    {
        permute(s, std::integral_constant<bool, (N > 0 && N < Rounds)>());
    }

    template <std::size_t S>
    static void permute(std::array<__m256i, S> &, std::false_type)
    {
    }

    template <std::size_t S>
    static void permute(std::array<__m256i, S> &s, std::true_type)
    {
        PhiloxGeneratorAVX2Impl32Permute<K>::round(s);
    }
}; // class PhiloxGeneratorImplAVX2

template <typename T, typename Constants>
class PhiloxGeneratorAVX2Impl32<T, 2, 10, Constants>
    : public Philox2x32GeneratorAVX2Impl<T, Constants>
{
}; // class PhiloxGeneratorAVX2Impl32

template <typename T, typename Constants>
class PhiloxGeneratorAVX2Impl32<T, 4, 10, Constants>
    : public Philox4x32GeneratorAVX2Impl<T, Constants>
{
}; // class PhiloxGeneratorAVX2Impl32

} // namespace mckl::internal

} // namespace mckl

#ifdef MCKL_GCC
#if MCKL_GCC_VERSION >= 60000
#pragma GCC diagnostic pop
#endif
#endif

#endif // MCKL_RANDOM_INTERNAL_PHILOX_AVX2_32_HPP
