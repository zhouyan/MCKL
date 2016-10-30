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
#include <mckl/random/internal/u01_avx2.hpp>
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
    static void first(std::array<__m256i, S> &)
    {
    }

    template <std::size_t S>
    static void round(std::array<__m256i, S> &s)
    {
        shuffle_epi32<0xB1>(s); // 2 3 0 1
    }

    template <std::size_t S>
    static void last(std::array<__m256i, S> &s)
    {
        shuffle_epi32<0xB1>(s); // 2 3 0 1
    }
}; // class PhiloxGeneratorAVX2Impl32Permute

template <>
class PhiloxGeneratorAVX2Impl32Permute<4>
{
    public:
    template <std::size_t S>
    static void first(std::array<__m256i, S> &s)
    {
        shuffle_epi32<0xC6>(s); // 3 0 1 2
    }

    template <std::size_t S>
    static void round(std::array<__m256i, S> &s)
    {
        shuffle_epi32<0x93>(s); // 2 1 0 3
    }

    template <std::size_t S>
    static void last(std::array<__m256i, S> &s)
    {
        shuffle_epi32<0xB1>(s); // 2 3 0 1
    }
}; // class PhiloxGeneratorAVX2Impl32Permute

template <typename T, std::size_t K, std::size_t Rounds, typename Constants>
class PhiloxGeneratorAVX2Impl32
{
    public:
    static constexpr bool batch()
    {
        return std::numeric_limits<T>::digits == 32 && K != 0 && 4 % K == 0;
    }

    static void eval(std::array<T, K> &state, const std::array<T, K / 2> &key)
    {
        PhiloxGeneratorGenericImpl<T, K, Rounds, Constants>::eval(state, key);
    }

    template <typename ResultType>
    static void eval(Counter<T, K> &ctr, const std::array<T, K / 2> &key,
        std::size_t n, ResultType *r)
    {
        eval<transform>(ctr, key, n, r);
    }

    template <typename RealType>
    static void u01_cc_u32(Counter<T, K> &ctr, const std::array<T, K / 2> &key,
        std::size_t n, RealType *r)
    {
        eval<U01AVX2Transform<std::uint32_t, RealType, Closed, Closed>>(
            ctr, key, n, r);
    }

    template <typename RealType>
    static void u01_co_u32(Counter<T, K> &ctr, const std::array<T, K / 2> &key,
        std::size_t n, RealType *r)
    {
        eval<U01AVX2Transform<std::uint32_t, RealType, Closed, Open>>(
            ctr, key, n, r);
    }

    template <typename RealType>
    static void u01_oc_u32(Counter<T, K> &ctr, const std::array<T, K / 2> &key,
        std::size_t n, RealType *r)
    {
        eval<U01AVX2Transform<std::uint32_t, RealType, Open, Closed>>(
            ctr, key, n, r);
    }

    template <typename RealType>
    static void u01_oo_u32(Counter<T, K> &ctr, const std::array<T, K / 2> &key,
        std::size_t n, RealType *r)
    {
        eval<U01AVX2Transform<std::uint32_t, RealType, Open, Open>>(
            ctr, key, n, r);
    }

    template <typename RealType>
    static void uniform_real_u32(Counter<T, K> &ctr,
        const std::array<T, K / 2> &key, std::size_t n, RealType *r,
        RealType a, RealType b)
    {
        eval<UniformRealAVX2Transform<std::uint32_t, RealType>>(
            ctr, key, n, r, a, b);
    }

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
    static void eval(Counter<T, K> &ctr, const std::array<T, K / 2> &key,
        std::size_t n, ResultType *r, Args &&... args)
    {
        using uint_type = typename Transform::uint_type;

        constexpr std::size_t S = 8;
        constexpr std::size_t nstride = sizeof(__m256i) * S / (sizeof(T) * K);
        constexpr std::size_t ustride = sizeof(T) * K / sizeof(uint_type);

        std::array<__m256i, S> s;
        std::array<__m256i, Rounds> rk;
        set_key(rk, key);
        while (n >= nstride) {
            MCKL_FLATTEN_CALL increment_si256(ctr, s);

            MCKL_FLATTEN_CALL PhiloxGeneratorAVX2Impl32Permute<K>::first(s);

            MCKL_FLATTEN_CALL rbox<0x0>(s, rk);
            MCKL_FLATTEN_CALL rbox<0x1>(s, rk);
            MCKL_FLATTEN_CALL rbox<0x2>(s, rk);
            MCKL_FLATTEN_CALL rbox<0x3>(s, rk);
            MCKL_FLATTEN_CALL rbox<0x4>(s, rk);
            MCKL_FLATTEN_CALL rbox<0x5>(s, rk);
            MCKL_FLATTEN_CALL rbox<0x6>(s, rk);
            MCKL_FLATTEN_CALL rbox<0x7>(s, rk);
            MCKL_FLATTEN_CALL rbox<0x8>(s, rk);
            MCKL_FLATTEN_CALL rbox<0x9>(s, rk);
            MCKL_FLATTEN_CALL rbox<0xA>(s, rk);
            MCKL_FLATTEN_CALL rbox<0xB>(s, rk);
            MCKL_FLATTEN_CALL rbox<0xC>(s, rk);
            MCKL_FLATTEN_CALL rbox<0xD>(s, rk);
            MCKL_FLATTEN_CALL rbox<0xE>(s, rk);
            MCKL_FLATTEN_CALL rbox<0xF>(s, rk);

            round<0x10>(s, rk, std::integral_constant<bool, 0x10 <= Rounds>());

            MCKL_FLATTEN_CALL PhiloxGeneratorAVX2Impl32Permute<K>::last(s);

            MCKL_FLATTEN_CALL r =
                Transform::eval(s, r, std::forward<Args>(args)...);
            n -= nstride;
        }

        alignas(32) union {
            std::array<std::array<T, K>, nstride> state;
            std::array<Counter<T, K>, nstride> ctr;
            std::array<uint_type, nstride * ustride> u;
        } buf;
        for (std::size_t i = 0; i != n; ++i) {
            MCKL_FLATTEN_CALL increment(ctr);
            buf.ctr[i] = ctr;
            eval(buf.state[i], key);
        }
        MCKL_FLATTEN_CALL Transform::eval(
            n * ustride, buf.u.data(), r, std::forward<Args>(args)...);
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
        MCKL_FLATTEN_CALL rbox<N>(s, rk);
        round<N + 1>(s, rk, std::integral_constant<bool, N + 1 <= Rounds>());
    }

    template <std::size_t N, std::size_t S>
    static void rbox(
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

    static void set_key(
        std::array<__m256i, Rounds> &rk, const std::array<T, K / 2> &key)
    {
        const int k0 = static_cast<int>(std::get<0 % (K / 2)>(key));
        const int k1 = static_cast<int>(std::get<1 % (K / 2)>(key));
        const int k2 = static_cast<int>(std::get<2 % (K / 2)>(key));
        const int k3 = static_cast<int>(std::get<3 % (K / 2)>(key));

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
