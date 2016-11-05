//============================================================================
// MCKL/include/mckl/random/internal/philox_avx2_32.hpp
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

#ifndef MCKL_RANDOM_INTERNAL_PHILOX_AVX2_32_HPP
#define MCKL_RANDOM_INTERNAL_PHILOX_AVX2_32_HPP

#include <mckl/random/internal/common.hpp>
#include <mckl/random/internal/philox_generic.hpp>
#include <mckl/random/internal/philox_unroll.hpp>
#include <mckl/random/internal/u01_avx2.hpp>
#include <mckl/random/internal/uniform_real_avx2.hpp>
#include <mckl/random/increment.hpp>

#define MCKL_DEFINE_RANDOM_INTERNAL_PHILOX_AVX2_32_U01(                       \
    lr, bits, Lower, Upper)                                                   \
    template <typename RealType>                                              \
    static void u01_##lr##_u##bits(Counter<T, K> &ctr, std::size_t n,         \
        RealType *r, const std::array<T, K / 2> &key)                         \
    {                                                                         \
        eval<U01AVX2Impl<std::uint##bits##_t, RealType, Lower, Upper>>(       \
            ctr, n, r, key);                                                  \
    }

#define MCKL_DEFINE_RANDOM_INTERNAL_PHILOX_AVX2_32_UNIFORM_REAL(bits)         \
    template <typename RealType>                                              \
    static void uniform_real_u##bits(Counter<T, K> &ctr, std::size_t n,       \
        RealType *r, const std::array<T, K / 2> &key, RealType a, RealType b) \
    {                                                                         \
        eval<UniformRealAVX2Impl<std::uint##bits##_t, RealType>>(             \
            ctr, n, r, key, a, b);                                            \
    }

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
    MCKL_FLATTEN static void first(std::array<__m256i, S> &)
    {
    }

    template <std::size_t S>
    MCKL_FLATTEN static void round(std::array<__m256i, S> &s)
    {
        shuffle_epi32<0xB1>(s); // 2 3 0 1
    }

    template <std::size_t S>
    MCKL_FLATTEN static void last(std::array<__m256i, S> &s)
    {
        shuffle_epi32<0xB1>(s); // 2 3 0 1
    }
}; // class PhiloxGeneratorAVX2Impl32Permute

template <>
class PhiloxGeneratorAVX2Impl32Permute<4>
{
    public:
    template <std::size_t S>
    MCKL_FLATTEN static void first(std::array<__m256i, S> &s)
    {
        shuffle_epi32<0xC6>(s); // 3 0 1 2
    }

    template <std::size_t S>
    MCKL_FLATTEN static void round(std::array<__m256i, S> &s)
    {
        shuffle_epi32<0x93>(s); // 2 1 0 3
    }

    template <std::size_t S>
    MCKL_FLATTEN static void last(std::array<__m256i, S> &s)
    {
        shuffle_epi32<0xB1>(s); // 2 3 0 1
    }
}; // class PhiloxGeneratorAVX2Impl32Permute

template <typename T, std::size_t K, std::size_t Rounds, typename Constants>
class PhiloxGeneratorAVX2Impl32
{
    static_assert(std::numeric_limits<T>::digits == 32,
        "**PhiloxGeneratorAVX2Impl32 used with T other than a 32-bit unsigned "
        "integer type");

    static_assert(K != 0 && 8 % K == 0,
        "**PhiloxGeneratorAVX2Impl32 used with K that does not divide by 8");

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
        eval<transform>(ctr, n, r, key);
    }

    MCKL_DEFINE_RANDOM_INTERNAL_PHILOX_AVX2_32_U01(cc, 32, Closed, Closed)
    MCKL_DEFINE_RANDOM_INTERNAL_PHILOX_AVX2_32_U01(co, 32, Closed, Open)
    MCKL_DEFINE_RANDOM_INTERNAL_PHILOX_AVX2_32_U01(oc, 32, Open, Closed)
    MCKL_DEFINE_RANDOM_INTERNAL_PHILOX_AVX2_32_U01(oo, 32, Open, Open)
    MCKL_DEFINE_RANDOM_INTERNAL_PHILOX_AVX2_32_UNIFORM_REAL(32)

    MCKL_DEFINE_RANDOM_INTERNAL_PHILOX_AVX2_32_U01(cc, 64, Closed, Closed)
    MCKL_DEFINE_RANDOM_INTERNAL_PHILOX_AVX2_32_U01(co, 64, Closed, Open)
    MCKL_DEFINE_RANDOM_INTERNAL_PHILOX_AVX2_32_U01(oc, 64, Open, Closed)
    MCKL_DEFINE_RANDOM_INTERNAL_PHILOX_AVX2_32_U01(oo, 64, Open, Open)
    MCKL_DEFINE_RANDOM_INTERNAL_PHILOX_AVX2_32_UNIFORM_REAL(64)

    private:
    class transform
    {
        public:
        using uint_type = T;

        template <std::size_t S, typename ResultType>
        MCKL_FLATTEN static ResultType *eval(
            const std::array<__m256i, S> &s, ResultType *r)
        {
            std::memcpy(r, s.data(), sizeof(s));

            return r + sizeof(s) / sizeof(ResultType);
        }

        template <typename ResultType>
        MCKL_FLATTEN static ResultType *eval(
            std::size_t n, const uint_type *s, ResultType *r)
        {
            std::memcpy(r, s, sizeof(uint_type) * n);

            return r + sizeof(uint_type) * n / sizeof(ResultType);
        }
    }; // class transform

    template <typename Transform, typename ResultType, typename... Args>
    static void eval(Counter<T, K> &ctr, std::size_t n, ResultType *r,
        const std::array<T, K / 2> &key, Args &&... args)
    {
        using uint_type = typename Transform::uint_type;

        static_assert(sizeof(T) * K % sizeof(uint_type) == 0,
            "**PhiloxGeneratorAVX2Impl32::eval** sizeof(T) * K is not "
            "divisible by sizeof(Transform::uint_type)");

        constexpr std::size_t S = 8;
        constexpr std::size_t nstride = sizeof(__m256i) * S / (sizeof(T) * K);
        constexpr std::size_t ustride = sizeof(T) * K / sizeof(uint_type);

        std::array<__m256i, S> s;
        std::array<__m256i, Rounds> rk;
        MCKL_FLATTEN_CALL set_key(rk, key);
        while (n >= nstride) {
            MCKL_FLATTEN_CALL increment_si256(ctr, s);
            MCKL_FLATTEN_CALL PhiloxGeneratorAVX2Impl32Permute<K>::first(s);
            MCKL_RANDOM_INTERNAL_PHILOX_UNROLL_ROUND(0, s, rk);
            MCKL_FLATTEN_CALL PhiloxGeneratorAVX2Impl32Permute<K>::last(s);
            MCKL_FLATTEN_CALL r =
                Transform::eval(s, r, std::forward<Args>(args)...);
            n -= nstride;
        }

        alignas(32) std::array<uint_type, nstride * ustride> u;
        PhiloxGeneratorGenericImpl<T, K, Rounds, Constants>::eval(
            ctr, n, u.data(), key);
        MCKL_FLATTEN_CALL Transform::eval(
            n * ustride, u.data(), r, std::forward<Args>(args)...);
    }

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
    MCKL_FLATTEN static void rbox(
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

    MCKL_FLATTEN static void set_key(
        std::array<__m256i, Rounds> &rk, const std::array<T, K / 2> &k)
    {
        const int k0 = static_cast<int>(std::get<0 % (K / 2)>(k));
        const int k1 = static_cast<int>(std::get<1 % (K / 2)>(k));
        const int k2 = static_cast<int>(std::get<2 % (K / 2)>(k));
        const int k3 = static_cast<int>(std::get<3 % (K / 2)>(k));

        set_key<0>(rk, _mm256_set_epi32(k3, 0, k2, 0, k1, 0, k0, 0),
            std::true_type());
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

} // namespace mckl::internal

} // namespace mckl

#ifdef MCKL_GCC
#if MCKL_GCC_VERSION >= 60000
#pragma GCC diagnostic pop
#endif
#endif

#endif // MCKL_RANDOM_INTERNAL_PHILOX_AVX2_32_HPP
