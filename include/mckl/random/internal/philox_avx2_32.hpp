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

#define MCKL_RANDOM_INTERNAL_PHILOX_AVX2_32_SBOX_CONSTANTS                    \
    constexpr int mul0 =                                                      \
        static_cast<int>(Constants::multiplier::value[0 % (K / 2)]);          \
    constexpr int mul1 =                                                      \
        static_cast<int>(Constants::multiplier::value[1 % (K / 2)]);          \
    constexpr int mul2 =                                                      \
        static_cast<int>(Constants::multiplier::value[2 % (K / 2)]);          \
    constexpr int mul3 =                                                      \
        static_cast<int>(Constants::multiplier::value[3 % (K / 2)]);          \
    constexpr int msk = static_cast<int>(0xFFFFFFFF);                         \
                                                                              \
    const __m256i k = std::get<N - 1>(rk);                                    \
    const __m256i m = _mm256_set_epi32(0, mul3, 0, mul2, 0, mul1, 0, mul0);   \
    const __m256i mask = _mm256_set_epi32(msk, 0, msk, 0, msk, 0, msk, 0);

#define MCKL_RANDOM_INTERNAL_PHILOX_AVX2_32_BATCH(S)                          \
    while (n >= sizeof(__m256i) * S / (sizeof(T) * K)) {                      \
        constexpr std::size_t cstride = sizeof(__m256i) * S;                  \
        constexpr std::size_t nstride = cstride / (sizeof(T) * K);            \
        constexpr std::size_t rstride = cstride / sizeof(ResultType);         \
                                                                              \
        alignas(32) union {                                                   \
            std::array<__m256i, S> s;                                         \
            std::array<Counter<T, K>, nstride> c;                             \
        } buf;                                                                \
                                                                              \
        MCKL_FLATTEN_CALL increment(ctr, buf.c);                              \
                                                                              \
        MCKL_FLATTEN_CALL PhiloxGeneratorImplPermute32<K>::first(buf.s);      \
                                                                              \
        MCKL_FLATTEN_CALL sbox<0x0>(buf.s, rk);                               \
        MCKL_FLATTEN_CALL sbox<0x1>(buf.s, rk);                               \
        MCKL_FLATTEN_CALL sbox<0x2>(buf.s, rk);                               \
        MCKL_FLATTEN_CALL sbox<0x3>(buf.s, rk);                               \
        MCKL_FLATTEN_CALL sbox<0x4>(buf.s, rk);                               \
        MCKL_FLATTEN_CALL sbox<0x5>(buf.s, rk);                               \
        MCKL_FLATTEN_CALL sbox<0x6>(buf.s, rk);                               \
        MCKL_FLATTEN_CALL sbox<0x7>(buf.s, rk);                               \
        MCKL_FLATTEN_CALL sbox<0x8>(buf.s, rk);                               \
        MCKL_FLATTEN_CALL sbox<0x9>(buf.s, rk);                               \
        MCKL_FLATTEN_CALL sbox<0xA>(buf.s, rk);                               \
        MCKL_FLATTEN_CALL sbox<0xB>(buf.s, rk);                               \
        MCKL_FLATTEN_CALL sbox<0xC>(buf.s, rk);                               \
        MCKL_FLATTEN_CALL sbox<0xD>(buf.s, rk);                               \
        MCKL_FLATTEN_CALL sbox<0xE>(buf.s, rk);                               \
        MCKL_FLATTEN_CALL sbox<0xF>(buf.s, rk);                               \
                                                                              \
        round<0x10>(                                                          \
            buf.s, rk, std::integral_constant<bool, 0x10 <= Rounds>());       \
                                                                              \
        MCKL_FLATTEN_CALL PhiloxGeneratorImplPermute32<K>::last(buf.s);       \
                                                                              \
        std::memcpy(r, buf.s.data(), cstride);                                \
        n -= nstride;                                                         \
        r += rstride;                                                         \
    }

template <std::size_t>
class PhiloxGeneratorImplPermute32;

template <>
class PhiloxGeneratorImplPermute32<2>
{
    public:
    template <std::size_t S>
    static void first(std::array<__m256i, S> &)
    {
    }

    static void round(std::array<__m256i, 4> &s)
    {
        // 2 3 0 1
        std::get<0>(s) = _mm256_shuffle_epi32(std::get<0>(s), 0xB1);
        std::get<1>(s) = _mm256_shuffle_epi32(std::get<1>(s), 0xB1);
        std::get<2>(s) = _mm256_shuffle_epi32(std::get<2>(s), 0xB1);
        std::get<3>(s) = _mm256_shuffle_epi32(std::get<3>(s), 0xB1);
    }

    static void round(std::array<__m256i, 8> &s)
    {
        // 2 3 0 1
        std::get<0>(s) = _mm256_shuffle_epi32(std::get<0>(s), 0xB1);
        std::get<1>(s) = _mm256_shuffle_epi32(std::get<1>(s), 0xB1);
        std::get<2>(s) = _mm256_shuffle_epi32(std::get<2>(s), 0xB1);
        std::get<3>(s) = _mm256_shuffle_epi32(std::get<3>(s), 0xB1);
        std::get<4>(s) = _mm256_shuffle_epi32(std::get<4>(s), 0xB1);
        std::get<5>(s) = _mm256_shuffle_epi32(std::get<5>(s), 0xB1);
        std::get<6>(s) = _mm256_shuffle_epi32(std::get<6>(s), 0xB1);
        std::get<7>(s) = _mm256_shuffle_epi32(std::get<7>(s), 0xB1);
    }

    static void last(std::array<__m256i, 4> &s)
    {
        // 2 3 0 1
        std::get<0>(s) = _mm256_shuffle_epi32(std::get<0>(s), 0xB1);
        std::get<1>(s) = _mm256_shuffle_epi32(std::get<1>(s), 0xB1);
        std::get<2>(s) = _mm256_shuffle_epi32(std::get<2>(s), 0xB1);
        std::get<3>(s) = _mm256_shuffle_epi32(std::get<3>(s), 0xB1);
    }

    static void last(std::array<__m256i, 8> &s)
    {
        // 2 3 0 1
        std::get<0>(s) = _mm256_shuffle_epi32(std::get<0>(s), 0xB1);
        std::get<1>(s) = _mm256_shuffle_epi32(std::get<1>(s), 0xB1);
        std::get<2>(s) = _mm256_shuffle_epi32(std::get<2>(s), 0xB1);
        std::get<3>(s) = _mm256_shuffle_epi32(std::get<3>(s), 0xB1);
        std::get<4>(s) = _mm256_shuffle_epi32(std::get<4>(s), 0xB1);
        std::get<5>(s) = _mm256_shuffle_epi32(std::get<5>(s), 0xB1);
        std::get<6>(s) = _mm256_shuffle_epi32(std::get<6>(s), 0xB1);
        std::get<7>(s) = _mm256_shuffle_epi32(std::get<7>(s), 0xB1);
    }
}; // class PhiloxGeneratorImplPermute32

template <>
class PhiloxGeneratorImplPermute32<4>
{
    public:
    static void first(std::array<__m256i, 4> &s)
    {
        // 3 0 1 2
        std::get<0>(s) = _mm256_shuffle_epi32(std::get<0>(s), 0xC6);
        std::get<1>(s) = _mm256_shuffle_epi32(std::get<1>(s), 0xC6);
        std::get<2>(s) = _mm256_shuffle_epi32(std::get<2>(s), 0xC6);
        std::get<3>(s) = _mm256_shuffle_epi32(std::get<3>(s), 0xC6);
    }

    static void first(std::array<__m256i, 8> &s)
    {
        // 3 0 1 2
        std::get<0>(s) = _mm256_shuffle_epi32(std::get<0>(s), 0xC6);
        std::get<1>(s) = _mm256_shuffle_epi32(std::get<1>(s), 0xC6);
        std::get<2>(s) = _mm256_shuffle_epi32(std::get<2>(s), 0xC6);
        std::get<3>(s) = _mm256_shuffle_epi32(std::get<3>(s), 0xC6);
        std::get<4>(s) = _mm256_shuffle_epi32(std::get<4>(s), 0xC6);
        std::get<5>(s) = _mm256_shuffle_epi32(std::get<5>(s), 0xC6);
        std::get<6>(s) = _mm256_shuffle_epi32(std::get<6>(s), 0xC6);
        std::get<7>(s) = _mm256_shuffle_epi32(std::get<7>(s), 0xC6);
    }

    static void round(std::array<__m256i, 4> &s)
    {
        // 2 1 0 3
        std::get<0>(s) = _mm256_shuffle_epi32(std::get<0>(s), 0x93);
        std::get<1>(s) = _mm256_shuffle_epi32(std::get<1>(s), 0x93);
        std::get<2>(s) = _mm256_shuffle_epi32(std::get<2>(s), 0x93);
        std::get<3>(s) = _mm256_shuffle_epi32(std::get<3>(s), 0x93);
    }

    static void round(std::array<__m256i, 8> &s)
    {
        // 2 1 0 3
        std::get<0>(s) = _mm256_shuffle_epi32(std::get<0>(s), 0x93);
        std::get<1>(s) = _mm256_shuffle_epi32(std::get<1>(s), 0x93);
        std::get<2>(s) = _mm256_shuffle_epi32(std::get<2>(s), 0x93);
        std::get<3>(s) = _mm256_shuffle_epi32(std::get<3>(s), 0x93);
        std::get<4>(s) = _mm256_shuffle_epi32(std::get<4>(s), 0x93);
        std::get<5>(s) = _mm256_shuffle_epi32(std::get<5>(s), 0x93);
        std::get<6>(s) = _mm256_shuffle_epi32(std::get<6>(s), 0x93);
        std::get<7>(s) = _mm256_shuffle_epi32(std::get<7>(s), 0x93);
    }

    static void last(std::array<__m256i, 4> &s)
    {
        // 2 3 0 1
        std::get<0>(s) = _mm256_shuffle_epi32(std::get<0>(s), 0xB1);
        std::get<1>(s) = _mm256_shuffle_epi32(std::get<1>(s), 0xB1);
        std::get<2>(s) = _mm256_shuffle_epi32(std::get<2>(s), 0xB1);
        std::get<3>(s) = _mm256_shuffle_epi32(std::get<3>(s), 0xB1);
    }

    static void last(std::array<__m256i, 8> &s)
    {
        // 2 3 0 1
        std::get<0>(s) = _mm256_shuffle_epi32(std::get<0>(s), 0xB1);
        std::get<1>(s) = _mm256_shuffle_epi32(std::get<1>(s), 0xB1);
        std::get<2>(s) = _mm256_shuffle_epi32(std::get<2>(s), 0xB1);
        std::get<3>(s) = _mm256_shuffle_epi32(std::get<3>(s), 0xB1);
        std::get<4>(s) = _mm256_shuffle_epi32(std::get<4>(s), 0xB1);
        std::get<5>(s) = _mm256_shuffle_epi32(std::get<5>(s), 0xB1);
        std::get<6>(s) = _mm256_shuffle_epi32(std::get<6>(s), 0xB1);
        std::get<7>(s) = _mm256_shuffle_epi32(std::get<7>(s), 0xB1);
    }
}; // class PhiloxGeneratorImplPermute32

template <typename T, std::size_t K, std::size_t Rounds, typename Constants>
class PhiloxGeneratorImpl<T, K, Rounds, Constants, 32>
{
    static constexpr std::size_t S_ = 8;

    public:
    static constexpr bool batch() { return K != 0 && 4 % K == 0; }

    static constexpr std::size_t blocks() { return S_ * 8 / K; }

    static void eval(std::array<T, K> &state, const std::array<T, K / 2> &key)
    {
        PhiloxGeneratorGenericImpl<T, K, Rounds, Constants>::eval(state, key);
    }

    template <typename ResultType>
    static void eval(Counter<T, K> &ctr, const std::array<T, K / 2> &key,
        std::size_t n, ResultType *r)
    {
        constexpr std::size_t stride = sizeof(T) * K / sizeof(ResultType);

        std::array<__m256i, Rounds> rk;
        MCKL_FLATTEN_CALL set_key(rk, key);

        MCKL_RANDOM_INTERNAL_PHILOX_AVX2_32_BATCH(8)
        MCKL_RANDOM_INTERNAL_PHILOX_AVX2_32_BATCH(4)

        alignas(32) union {
            std::array<T, K> state;
            Counter<T, K> ctr;
        } buf;

        for (std::size_t i = 0; i != n; ++i, r += stride) {
            MCKL_FLATTEN_CALL increment(ctr);
            buf.ctr = ctr;
            eval(buf.state, key);
            std::memcpy(r, buf.state.data(), sizeof(T) * K);
        }
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
        MCKL_FLATTEN_CALL sbox<N>(s, rk);
        round<N + 1>(s, rk, std::integral_constant<bool, N + 1 <= Rounds>());
    }

    template <std::size_t N, std::size_t S>
    static void sbox(
        std::array<__m256i, S> &s, const std::array<__m256i, Rounds> &rk)
    {
        sbox<N>(s, rk, std::integral_constant<bool, (N > 0 && N <= Rounds)>());
    }

    template <std::size_t, std::size_t S>
    static void sbox(std::array<__m256i, S> &,
        const std::array<__m256i, Rounds> &, std::false_type)
    {
    }

    template <std::size_t N>
    static void sbox(std::array<__m256i, 4> &s,
        const std::array<__m256i, Rounds> &rk, std::true_type)
    {
        MCKL_RANDOM_INTERNAL_PHILOX_AVX2_32_SBOX_CONSTANTS

        __m256i m0 = _mm256_mul_epu32(std::get<0>(s), m);
        __m256i m1 = _mm256_mul_epu32(std::get<1>(s), m);
        __m256i m2 = _mm256_mul_epu32(std::get<2>(s), m);
        __m256i m3 = _mm256_mul_epu32(std::get<3>(s), m);

        __m256i x0 = _mm256_xor_si256(std::get<0>(s), k);
        __m256i x1 = _mm256_xor_si256(std::get<1>(s), k);
        __m256i x2 = _mm256_xor_si256(std::get<2>(s), k);
        __m256i x3 = _mm256_xor_si256(std::get<3>(s), k);

        std::get<0>(s) = _mm256_and_si256(x0, mask);
        std::get<1>(s) = _mm256_and_si256(x1, mask);
        std::get<2>(s) = _mm256_and_si256(x2, mask);
        std::get<3>(s) = _mm256_and_si256(x3, mask);

        std::get<0>(s) = _mm256_xor_si256(std::get<0>(s), m0);
        std::get<1>(s) = _mm256_xor_si256(std::get<1>(s), m1);
        std::get<2>(s) = _mm256_xor_si256(std::get<2>(s), m2);
        std::get<3>(s) = _mm256_xor_si256(std::get<3>(s), m3);

        permute<N>(s);
    }

    template <std::size_t N>
    static void sbox(std::array<__m256i, 8> &s,
        const std::array<__m256i, Rounds> &rk, std::true_type)
    {
        MCKL_RANDOM_INTERNAL_PHILOX_AVX2_32_SBOX_CONSTANTS

        __m256i m0 = _mm256_mul_epu32(std::get<0>(s), m);
        __m256i m1 = _mm256_mul_epu32(std::get<1>(s), m);
        __m256i m2 = _mm256_mul_epu32(std::get<2>(s), m);
        __m256i m3 = _mm256_mul_epu32(std::get<3>(s), m);
        __m256i m4 = _mm256_mul_epu32(std::get<4>(s), m);
        __m256i m5 = _mm256_mul_epu32(std::get<5>(s), m);
        __m256i m6 = _mm256_mul_epu32(std::get<6>(s), m);
        __m256i m7 = _mm256_mul_epu32(std::get<7>(s), m);

        m0 = _mm256_xor_si256(m0, k);
        m1 = _mm256_xor_si256(m1, k);
        m2 = _mm256_xor_si256(m2, k);
        m3 = _mm256_xor_si256(m3, k);
        m4 = _mm256_xor_si256(m4, k);
        m5 = _mm256_xor_si256(m5, k);
        m6 = _mm256_xor_si256(m6, k);
        m7 = _mm256_xor_si256(m7, k);

        std::get<0>(s) = _mm256_and_si256(std::get<0>(s), mask);
        std::get<1>(s) = _mm256_and_si256(std::get<1>(s), mask);
        std::get<2>(s) = _mm256_and_si256(std::get<2>(s), mask);
        std::get<3>(s) = _mm256_and_si256(std::get<3>(s), mask);
        std::get<4>(s) = _mm256_and_si256(std::get<4>(s), mask);
        std::get<5>(s) = _mm256_and_si256(std::get<5>(s), mask);
        std::get<6>(s) = _mm256_and_si256(std::get<6>(s), mask);
        std::get<7>(s) = _mm256_and_si256(std::get<7>(s), mask);

        std::get<0>(s) = _mm256_xor_si256(std::get<0>(s), m0);
        std::get<1>(s) = _mm256_xor_si256(std::get<1>(s), m1);
        std::get<2>(s) = _mm256_xor_si256(std::get<2>(s), m2);
        std::get<3>(s) = _mm256_xor_si256(std::get<3>(s), m3);
        std::get<4>(s) = _mm256_xor_si256(std::get<4>(s), m4);
        std::get<5>(s) = _mm256_xor_si256(std::get<5>(s), m5);
        std::get<6>(s) = _mm256_xor_si256(std::get<6>(s), m6);
        std::get<7>(s) = _mm256_xor_si256(std::get<7>(s), m7);

        permute<N>(s);
    }

    static void set_key(
        std::array<__m256i, Rounds> &rk, const std::array<T, K / 2> &key)
    {
        const int k0 = static_cast<int>(std::get<0 % (K / 2)>(key));
        const int k1 = static_cast<int>(std::get<1 % (K / 2)>(key));
        const int k2 = static_cast<int>(std::get<2 % (K / 2)>(key));
        const int k3 = static_cast<int>(std::get<3 % (K / 2)>(key));

        const __m256i k = _mm256_set_epi32(k3, 0, k2, 0, k1, 0, k0, 0);
        set_key<0>(rk, k, std::true_type());
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

        const __m256i w = _mm256_set_epi32(w3, 0, w2, 0, w1, 0, w0, 0);
        std::get<N>(rk) = _mm256_add_epi32(k, w);
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
        PhiloxGeneratorImplPermute32<K>::round(s);
    }
}; // class PhiloxGeneratorImplAVX2_32
