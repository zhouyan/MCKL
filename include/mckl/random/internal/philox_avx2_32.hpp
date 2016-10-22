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

    static void round(std::array<__m256i, 1> &s)
    {
        // 2 3 0 1
        std::get<0>(s) = _mm256_shuffle_epi32(std::get<0>(s), 0xB1);
    }

    static void round(std::array<__m256i, 2> &s)
    {
        // 2 3 0 1
        std::get<0>(s) = _mm256_shuffle_epi32(std::get<0>(s), 0xB1);
        std::get<1>(s) = _mm256_shuffle_epi32(std::get<1>(s), 0xB1);
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

    static void last(std::array<__m256i, 1> &s)
    {
        // 2 3 0 1
        std::get<0>(s) = _mm256_shuffle_epi32(std::get<0>(s), 0xB1);
    }

    static void last(std::array<__m256i, 2> &s)
    {
        // 2 3 0 1
        std::get<0>(s) = _mm256_shuffle_epi32(std::get<0>(s), 0xB1);
        std::get<1>(s) = _mm256_shuffle_epi32(std::get<1>(s), 0xB1);
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
    static void first(std::array<__m256i, 1> &s)
    {
        // 3 0 1 2
        std::get<0>(s) = _mm256_shuffle_epi32(std::get<0>(s), 0xC6);
    }

    static void first(std::array<__m256i, 2> &s)
    {
        // 3 0 1 2
        std::get<0>(s) = _mm256_shuffle_epi32(std::get<0>(s), 0xC6);
        std::get<1>(s) = _mm256_shuffle_epi32(std::get<1>(s), 0xC6);
    }

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

    static void round(std::array<__m256i, 1> &s)
    {
        // 2 1 0 3
        std::get<0>(s) = _mm256_shuffle_epi32(std::get<0>(s), 0x93);
    }

    static void round(std::array<__m256i, 2> &s)
    {
        // 2 1 0 3
        std::get<0>(s) = _mm256_shuffle_epi32(std::get<0>(s), 0x93);
        std::get<1>(s) = _mm256_shuffle_epi32(std::get<1>(s), 0x93);
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

    static void last(std::array<__m256i, 1> &s)
    {
        // 2 3 0 1
        std::get<0>(s) = _mm256_shuffle_epi32(std::get<0>(s), 0xB1);
    }

    static void last(std::array<__m256i, 2> &s)
    {
        // 2 3 0 1
        std::get<0>(s) = _mm256_shuffle_epi32(std::get<0>(s), 0xB1);
        std::get<1>(s) = _mm256_shuffle_epi32(std::get<1>(s), 0xB1);
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
        eval(state, key, std::integral_constant<bool, K == 8>());
    }

    template <std::size_t B>
    static void eval(std::array<std::array<T, K>, B> &state,
        const std::array<T, K / 2> &key)
    {
        constexpr std::size_t S = K * B / 8;

        static_assert(S != 0 && (S & (S - 1)) == 0 && S <= 8,
            "**PhiloxGeneratorImpl::eval** used with invalid block size (S = "
            "K * B / 8)");

        constexpr std::size_t i0 = 0 % (K / 2);
        constexpr std::size_t i1 = 1 % (K / 2);
        constexpr std::size_t i2 = 2 % (K / 2);
        constexpr std::size_t i3 = 3 % (K / 2);

        constexpr int w0 = static_cast<int>(Constants::weyl::value[i0]);
        constexpr int w1 = static_cast<int>(Constants::weyl::value[i1]);
        constexpr int w2 = static_cast<int>(Constants::weyl::value[i2]);
        constexpr int w3 = static_cast<int>(Constants::weyl::value[i3]);

        constexpr int m0 = static_cast<int>(Constants::multiplier::value[i0]);
        constexpr int m1 = static_cast<int>(Constants::multiplier::value[i1]);
        constexpr int m2 = static_cast<int>(Constants::multiplier::value[i2]);
        constexpr int m3 = static_cast<int>(Constants::multiplier::value[i3]);

        const int p0 = static_cast<int>(std::get<i0>(key));
        const int p1 = static_cast<int>(std::get<i1>(key));
        const int p2 = static_cast<int>(std::get<i2>(key));
        const int p3 = static_cast<int>(std::get<i3>(key));

        const __m256i w = _mm256_set_epi32(w3, 0, w2, 0, w1, 0, w0, 0);
        const __m256i m = _mm256_set_epi32(0, m3, 0, m2, 0, m1, 0, m0);
        __m256i p = _mm256_set_epi32(p3, 0, p2, 0, p1, 0, p0, 0);

        std::array<__m256i, S> s;

        load_si256(s, reinterpret_cast<const __m256i *>(state.data()));

        PhiloxGeneratorImplPermute32<K>::first(s);

        kbox<0x0>(p, w);
        sbox<0x0>(s, p, m);
        kbox<0x1>(p, w);
        sbox<0x1>(s, p, m);
        kbox<0x2>(p, w);
        sbox<0x2>(s, p, m);
        kbox<0x3>(p, w);
        sbox<0x3>(s, p, m);
        kbox<0x4>(p, w);
        sbox<0x4>(s, p, m);
        kbox<0x5>(p, w);
        sbox<0x5>(s, p, m);
        kbox<0x6>(p, w);
        sbox<0x6>(s, p, m);
        kbox<0x7>(p, w);
        sbox<0x7>(s, p, m);
        kbox<0x8>(p, w);
        sbox<0x8>(s, p, m);
        kbox<0x9>(p, w);
        sbox<0x9>(s, p, m);
        kbox<0xA>(p, w);
        sbox<0xA>(s, p, m);
        kbox<0xB>(p, w);
        sbox<0xB>(s, p, m);
        kbox<0xC>(p, w);
        sbox<0xC>(s, p, m);
        kbox<0xD>(p, w);
        sbox<0xD>(s, p, m);
        kbox<0xE>(p, w);
        sbox<0xE>(s, p, m);
        kbox<0xF>(p, w);
        sbox<0xF>(s, p, m);

        round<0x10>(
            s, p, w, m, std::integral_constant<bool, 0x10 <= Rounds>());

        PhiloxGeneratorImplPermute32<K>::last(s);

        store_si256(s, reinterpret_cast<__m256i *>(state.data()));
    }

    private:
    static void eval(std::array<T, K> &state, const std::array<T, K / 2> &key,
        std::false_type)
    {
        PhiloxGeneratorGenericImpl<T, K, Rounds, Constants>::eval(state, key);
    }

    static void eval(std::array<T, K> &state, const std::array<T, K / 2> &key,
        std::true_type)
    {
        std::array<std::array<T, K>, 1> s;
        std::get<0>(s) = state;
        eval(s, key);
        state = std::get<0>(s);
    }

    template <std::size_t, std::size_t S>
    static void round(std::array<__m256i, S> &, __m256i &, const __m256i &,
        const __m256i &, std::false_type)
    {
    }

    template <std::size_t N, std::size_t S>
    static void round(std::array<__m256i, S> &s, __m256i &p, const __m256i &w,
        const __m256i &m, std::true_type)
    {
        kbox<N>(p, w);
        sbox<N>(s, p, m);
        round<N + 1>(
            s, p, w, m, std::integral_constant<bool, N + 1 <= Rounds>());
    }

    template <std::size_t N>
    static void kbox(__m256i &p, const __m256i &w)
    {
        kbox(p, w, std::integral_constant<bool, (N > 1 && N <= Rounds)>());
    }

    static void kbox(__m256i &, const __m256i &, std::false_type) {}

    static void kbox(__m256i &p, const __m256i &w, std::true_type)
    {
        p = _mm256_add_epi32(p, w);
    }

    template <std::size_t N, std::size_t S>
    static void sbox(
        std::array<__m256i, S> &s, const __m256i &p, const __m256i &m)
    {
        sbox<N>(
            s, p, m, std::integral_constant<bool, (N > 0 && N <= Rounds)>());
    }

    template <std::size_t, std::size_t S>
    static void sbox(std::array<__m256i, S> &, const __m256i &,
        const __m256i &, std::false_type)
    {
    }

    template <std::size_t N>
    static void sbox(std::array<__m256i, 1> &s, const __m256i &p,
        const __m256i &m, std::true_type)
    {
        constexpr int msk = static_cast<int>(0xFFFFFFFF);

        const __m256i mask = _mm256_set_epi32(msk, 0, msk, 0, msk, 0, msk, 0);

        __m256i m0 = _mm256_mul_epu32(std::get<0>(s), m);

        __m256i x0 = _mm256_xor_si256(std::get<0>(s), p);

        std::get<0>(s) = _mm256_and_si256(x0, mask);

        std::get<0>(s) = _mm256_xor_si256(std::get<0>(s), m0);

        permute<N>(s);
    }

    template <std::size_t N>
    static void sbox(std::array<__m256i, 2> &s, const __m256i &p,
        const __m256i &m, std::true_type)
    {
        constexpr int msk = static_cast<int>(0xFFFFFFFF);

        const __m256i mask = _mm256_set_epi32(msk, 0, msk, 0, msk, 0, msk, 0);

        __m256i m0 = _mm256_mul_epu32(std::get<0>(s), m);
        __m256i m1 = _mm256_mul_epu32(std::get<1>(s), m);

        __m256i x0 = _mm256_xor_si256(std::get<0>(s), p);
        __m256i x1 = _mm256_xor_si256(std::get<1>(s), p);

        std::get<0>(s) = _mm256_and_si256(x0, mask);
        std::get<1>(s) = _mm256_and_si256(x1, mask);

        std::get<0>(s) = _mm256_xor_si256(std::get<0>(s), m0);
        std::get<1>(s) = _mm256_xor_si256(std::get<1>(s), m1);

        permute<N>(s);
    }

    template <std::size_t N>
    static void sbox(std::array<__m256i, 4> &s, const __m256i &p,
        const __m256i &m, std::true_type)
    {
        constexpr int msk = static_cast<int>(0xFFFFFFFF);

        const __m256i mask = _mm256_set_epi32(msk, 0, msk, 0, msk, 0, msk, 0);

        __m256i m0 = _mm256_mul_epu32(std::get<0>(s), m);
        __m256i m1 = _mm256_mul_epu32(std::get<1>(s), m);
        __m256i m2 = _mm256_mul_epu32(std::get<2>(s), m);
        __m256i m3 = _mm256_mul_epu32(std::get<3>(s), m);

        __m256i x0 = _mm256_xor_si256(std::get<0>(s), p);
        __m256i x1 = _mm256_xor_si256(std::get<1>(s), p);
        __m256i x2 = _mm256_xor_si256(std::get<2>(s), p);
        __m256i x3 = _mm256_xor_si256(std::get<3>(s), p);

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
    static void sbox(std::array<__m256i, 8> &s, const __m256i &p,
        const __m256i &m, std::true_type)
    {
        constexpr int msk = static_cast<int>(0xFFFFFFFF);

        const __m256i mask = _mm256_set_epi32(msk, 0, msk, 0, msk, 0, msk, 0);

        __m256i m0 = _mm256_mul_epu32(std::get<0>(s), m);
        __m256i m1 = _mm256_mul_epu32(std::get<1>(s), m);
        __m256i m2 = _mm256_mul_epu32(std::get<2>(s), m);
        __m256i m3 = _mm256_mul_epu32(std::get<3>(s), m);
        __m256i m4 = _mm256_mul_epu32(std::get<4>(s), m);
        __m256i m5 = _mm256_mul_epu32(std::get<5>(s), m);
        __m256i m6 = _mm256_mul_epu32(std::get<6>(s), m);
        __m256i m7 = _mm256_mul_epu32(std::get<7>(s), m);

        m0 = _mm256_xor_si256(m0, p);
        m1 = _mm256_xor_si256(m1, p);
        m2 = _mm256_xor_si256(m2, p);
        m3 = _mm256_xor_si256(m3, p);
        m4 = _mm256_xor_si256(m4, p);
        m5 = _mm256_xor_si256(m5, p);
        m6 = _mm256_xor_si256(m6, p);
        m7 = _mm256_xor_si256(m7, p);

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
