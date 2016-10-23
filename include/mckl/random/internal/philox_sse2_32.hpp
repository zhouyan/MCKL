//============================================================================
// MCKL/include/mckl/random/internal/philox_sse2_32.hpp
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

#define MCKL_DEFINE_RANDOM_INTERNAL_PHILOX_SSE2_32_EVAL(S)                    \
    static void eval(std::array<std::array<T, K>, S * 4 / K> &state,          \
        const std::array<T, K / 2> &key,                                      \
        std::integral_constant<std::size_t, S> * = nullptr)                   \
    {                                                                         \
        constexpr std::size_t i0 = 0 % (K / 2);                               \
        constexpr std::size_t i1 = 1 % (K / 2);                               \
                                                                              \
        constexpr int w0 = static_cast<int>(Constants::weyl::value[i0]);      \
        constexpr int w1 = static_cast<int>(Constants::weyl::value[i1]);      \
                                                                              \
        constexpr int m0 =                                                    \
            static_cast<int>(Constants::multiplier::value[i0]);               \
        constexpr int m1 =                                                    \
            static_cast<int>(Constants::multiplier::value[i1]);               \
                                                                              \
        const int p0 = static_cast<int>(std::get<i0>(key));                   \
        const int p1 = static_cast<int>(std::get<i1>(key));                   \
                                                                              \
        const __m128i w = _mm_set_epi32(w1, 0, w0, 0);                        \
        const __m128i m = _mm_set_epi32(0, m1, 0, m0);                        \
        __m128i p = _mm_set_epi32(p1, 0, p0, 0);                              \
                                                                              \
        std::array<__m128i, S> s;                                             \
        __m128i *const sptr = reinterpret_cast<__m128i *>(state.data());      \
                                                                              \
        MCKL_LOAD_SI128_##S(s, sptr);                                         \
                                                                              \
        MCKL_FLATTEN_CALL PhiloxGeneratorImplPermute32<K>::first(s);          \
                                                                              \
        MCKL_FLATTEN_CALL kbox<0x0>(p, w);                                    \
        MCKL_FLATTEN_CALL sbox<0x0>(s, p, m);                                 \
        MCKL_FLATTEN_CALL kbox<0x1>(p, w);                                    \
        MCKL_FLATTEN_CALL sbox<0x1>(s, p, m);                                 \
        MCKL_FLATTEN_CALL kbox<0x2>(p, w);                                    \
        MCKL_FLATTEN_CALL sbox<0x2>(s, p, m);                                 \
        MCKL_FLATTEN_CALL kbox<0x3>(p, w);                                    \
        MCKL_FLATTEN_CALL sbox<0x3>(s, p, m);                                 \
        MCKL_FLATTEN_CALL kbox<0x4>(p, w);                                    \
        MCKL_FLATTEN_CALL sbox<0x4>(s, p, m);                                 \
        MCKL_FLATTEN_CALL kbox<0x5>(p, w);                                    \
        MCKL_FLATTEN_CALL sbox<0x5>(s, p, m);                                 \
        MCKL_FLATTEN_CALL kbox<0x6>(p, w);                                    \
        MCKL_FLATTEN_CALL sbox<0x6>(s, p, m);                                 \
        MCKL_FLATTEN_CALL kbox<0x7>(p, w);                                    \
        MCKL_FLATTEN_CALL sbox<0x7>(s, p, m);                                 \
        MCKL_FLATTEN_CALL kbox<0x8>(p, w);                                    \
        MCKL_FLATTEN_CALL sbox<0x8>(s, p, m);                                 \
        MCKL_FLATTEN_CALL kbox<0x9>(p, w);                                    \
        MCKL_FLATTEN_CALL sbox<0x9>(s, p, m);                                 \
        MCKL_FLATTEN_CALL kbox<0xA>(p, w);                                    \
        MCKL_FLATTEN_CALL sbox<0xA>(s, p, m);                                 \
        MCKL_FLATTEN_CALL kbox<0xB>(p, w);                                    \
        MCKL_FLATTEN_CALL sbox<0xB>(s, p, m);                                 \
        MCKL_FLATTEN_CALL kbox<0xC>(p, w);                                    \
        MCKL_FLATTEN_CALL sbox<0xC>(s, p, m);                                 \
        MCKL_FLATTEN_CALL kbox<0xD>(p, w);                                    \
        MCKL_FLATTEN_CALL sbox<0xD>(s, p, m);                                 \
        MCKL_FLATTEN_CALL kbox<0xE>(p, w);                                    \
        MCKL_FLATTEN_CALL sbox<0xE>(s, p, m);                                 \
        MCKL_FLATTEN_CALL kbox<0xF>(p, w);                                    \
        MCKL_FLATTEN_CALL sbox<0xF>(s, p, m);                                 \
                                                                              \
        round<0x10>(                                                          \
            s, p, w, m, std::integral_constant<bool, 0x10 <= Rounds>());      \
                                                                              \
        MCKL_FLATTEN_CALL PhiloxGeneratorImplPermute32<K>::last(s);           \
                                                                              \
        MCKL_STORE_SI128_##S(s, sptr);                                        \
    }

template <std::size_t>
class PhiloxGeneratorImplPermute32;

template <>
class PhiloxGeneratorImplPermute32<2>
{
    public:
    template <std::size_t S>
    static void first(std::array<__m128i, S> &)
    {
    }

    static void round(std::array<__m128i, 1> &s)
    {
        // 2 3 0 1
        std::get<0>(s) = _mm_shuffle_epi32(std::get<0>(s), 0xB1);
    }

    static void round(std::array<__m128i, 2> &s)
    {
        // 2 3 0 1
        std::get<0>(s) = _mm_shuffle_epi32(std::get<0>(s), 0xB1);
        std::get<1>(s) = _mm_shuffle_epi32(std::get<1>(s), 0xB1);
    }

    static void round(std::array<__m128i, 4> &s)
    {
        // 2 3 0 1
        std::get<0>(s) = _mm_shuffle_epi32(std::get<0>(s), 0xB1);
        std::get<1>(s) = _mm_shuffle_epi32(std::get<1>(s), 0xB1);
        std::get<2>(s) = _mm_shuffle_epi32(std::get<2>(s), 0xB1);
        std::get<3>(s) = _mm_shuffle_epi32(std::get<3>(s), 0xB1);
    }

    static void round(std::array<__m128i, 8> &s)
    {
        // 2 3 0 1
        std::get<0>(s) = _mm_shuffle_epi32(std::get<0>(s), 0xB1);
        std::get<1>(s) = _mm_shuffle_epi32(std::get<1>(s), 0xB1);
        std::get<2>(s) = _mm_shuffle_epi32(std::get<2>(s), 0xB1);
        std::get<3>(s) = _mm_shuffle_epi32(std::get<3>(s), 0xB1);
        std::get<4>(s) = _mm_shuffle_epi32(std::get<4>(s), 0xB1);
        std::get<5>(s) = _mm_shuffle_epi32(std::get<5>(s), 0xB1);
        std::get<6>(s) = _mm_shuffle_epi32(std::get<6>(s), 0xB1);
        std::get<7>(s) = _mm_shuffle_epi32(std::get<7>(s), 0xB1);
    }

    static void last(std::array<__m128i, 1> &s)
    {
        // 2 3 0 1
        std::get<0>(s) = _mm_shuffle_epi32(std::get<0>(s), 0xB1);
    }

    static void last(std::array<__m128i, 2> &s)
    {
        // 2 3 0 1
        std::get<0>(s) = _mm_shuffle_epi32(std::get<0>(s), 0xB1);
        std::get<1>(s) = _mm_shuffle_epi32(std::get<1>(s), 0xB1);
    }

    static void last(std::array<__m128i, 4> &s)
    {
        // 2 3 0 1
        std::get<0>(s) = _mm_shuffle_epi32(std::get<0>(s), 0xB1);
        std::get<1>(s) = _mm_shuffle_epi32(std::get<1>(s), 0xB1);
        std::get<2>(s) = _mm_shuffle_epi32(std::get<2>(s), 0xB1);
        std::get<3>(s) = _mm_shuffle_epi32(std::get<3>(s), 0xB1);
    }

    static void last(std::array<__m128i, 8> &s)
    {
        // 2 3 0 1
        std::get<0>(s) = _mm_shuffle_epi32(std::get<0>(s), 0xB1);
        std::get<1>(s) = _mm_shuffle_epi32(std::get<1>(s), 0xB1);
        std::get<2>(s) = _mm_shuffle_epi32(std::get<2>(s), 0xB1);
        std::get<3>(s) = _mm_shuffle_epi32(std::get<3>(s), 0xB1);
        std::get<4>(s) = _mm_shuffle_epi32(std::get<4>(s), 0xB1);
        std::get<5>(s) = _mm_shuffle_epi32(std::get<5>(s), 0xB1);
        std::get<6>(s) = _mm_shuffle_epi32(std::get<6>(s), 0xB1);
        std::get<7>(s) = _mm_shuffle_epi32(std::get<7>(s), 0xB1);
    }
}; // class PhiloxGeneratorImplPermute32

template <>
class PhiloxGeneratorImplPermute32<4>
{
    public:
    static void first(std::array<__m128i, 1> &s)
    {
        // 3 0 1 2
        std::get<0>(s) = _mm_shuffle_epi32(std::get<0>(s), 0xC6);
    }

    static void first(std::array<__m128i, 2> &s)
    {
        // 3 0 1 2
        std::get<0>(s) = _mm_shuffle_epi32(std::get<0>(s), 0xC6);
        std::get<1>(s) = _mm_shuffle_epi32(std::get<1>(s), 0xC6);
    }

    static void first(std::array<__m128i, 4> &s)
    {
        // 3 0 1 2
        std::get<0>(s) = _mm_shuffle_epi32(std::get<0>(s), 0xC6);
        std::get<1>(s) = _mm_shuffle_epi32(std::get<1>(s), 0xC6);
        std::get<2>(s) = _mm_shuffle_epi32(std::get<2>(s), 0xC6);
        std::get<3>(s) = _mm_shuffle_epi32(std::get<3>(s), 0xC6);
    }

    static void first(std::array<__m128i, 8> &s)
    {
        // 3 0 1 2
        std::get<0>(s) = _mm_shuffle_epi32(std::get<0>(s), 0xC6);
        std::get<1>(s) = _mm_shuffle_epi32(std::get<1>(s), 0xC6);
        std::get<2>(s) = _mm_shuffle_epi32(std::get<2>(s), 0xC6);
        std::get<3>(s) = _mm_shuffle_epi32(std::get<3>(s), 0xC6);
        std::get<4>(s) = _mm_shuffle_epi32(std::get<4>(s), 0xC6);
        std::get<5>(s) = _mm_shuffle_epi32(std::get<5>(s), 0xC6);
        std::get<6>(s) = _mm_shuffle_epi32(std::get<6>(s), 0xC6);
        std::get<7>(s) = _mm_shuffle_epi32(std::get<7>(s), 0xC6);
    }

    static void round(std::array<__m128i, 1> &s)
    {
        // 2 1 0 3
        std::get<0>(s) = _mm_shuffle_epi32(std::get<0>(s), 0x93);
    }

    static void round(std::array<__m128i, 2> &s)
    {
        // 2 1 0 3
        std::get<0>(s) = _mm_shuffle_epi32(std::get<0>(s), 0x93);
        std::get<1>(s) = _mm_shuffle_epi32(std::get<1>(s), 0x93);
    }

    static void round(std::array<__m128i, 4> &s)
    {
        // 2 1 0 3
        std::get<0>(s) = _mm_shuffle_epi32(std::get<0>(s), 0x93);
        std::get<1>(s) = _mm_shuffle_epi32(std::get<1>(s), 0x93);
        std::get<2>(s) = _mm_shuffle_epi32(std::get<2>(s), 0x93);
        std::get<3>(s) = _mm_shuffle_epi32(std::get<3>(s), 0x93);
    }

    static void round(std::array<__m128i, 8> &s)
    {
        // 2 1 0 3
        std::get<0>(s) = _mm_shuffle_epi32(std::get<0>(s), 0x93);
        std::get<1>(s) = _mm_shuffle_epi32(std::get<1>(s), 0x93);
        std::get<2>(s) = _mm_shuffle_epi32(std::get<2>(s), 0x93);
        std::get<3>(s) = _mm_shuffle_epi32(std::get<3>(s), 0x93);
        std::get<4>(s) = _mm_shuffle_epi32(std::get<4>(s), 0x93);
        std::get<5>(s) = _mm_shuffle_epi32(std::get<5>(s), 0x93);
        std::get<6>(s) = _mm_shuffle_epi32(std::get<6>(s), 0x93);
        std::get<7>(s) = _mm_shuffle_epi32(std::get<7>(s), 0x93);
    }

    static void last(std::array<__m128i, 1> &s)
    {
        // 2 3 0 1
        std::get<0>(s) = _mm_shuffle_epi32(std::get<0>(s), 0xB1);
    }

    static void last(std::array<__m128i, 2> &s)
    {
        // 2 3 0 1
        std::get<0>(s) = _mm_shuffle_epi32(std::get<0>(s), 0xB1);
        std::get<1>(s) = _mm_shuffle_epi32(std::get<1>(s), 0xB1);
    }

    static void last(std::array<__m128i, 4> &s)
    {
        // 2 3 0 1
        std::get<0>(s) = _mm_shuffle_epi32(std::get<0>(s), 0xB1);
        std::get<1>(s) = _mm_shuffle_epi32(std::get<1>(s), 0xB1);
        std::get<2>(s) = _mm_shuffle_epi32(std::get<2>(s), 0xB1);
        std::get<3>(s) = _mm_shuffle_epi32(std::get<3>(s), 0xB1);
    }

    static void last(std::array<__m128i, 8> &s)
    {
        // 2 3 0 1
        std::get<0>(s) = _mm_shuffle_epi32(std::get<0>(s), 0xB1);
        std::get<1>(s) = _mm_shuffle_epi32(std::get<1>(s), 0xB1);
        std::get<2>(s) = _mm_shuffle_epi32(std::get<2>(s), 0xB1);
        std::get<3>(s) = _mm_shuffle_epi32(std::get<3>(s), 0xB1);
        std::get<4>(s) = _mm_shuffle_epi32(std::get<4>(s), 0xB1);
        std::get<5>(s) = _mm_shuffle_epi32(std::get<5>(s), 0xB1);
        std::get<6>(s) = _mm_shuffle_epi32(std::get<6>(s), 0xB1);
        std::get<7>(s) = _mm_shuffle_epi32(std::get<7>(s), 0xB1);
    }
}; // class PhiloxGeneratorImplPermute32

template <typename T, std::size_t K, std::size_t Rounds, typename Constants>
class PhiloxGeneratorImpl<T, K, Rounds, Constants, 32>
{
#if MCKL_HAS_X86_64
    static constexpr std::size_t S_ = 8;
#else
    static constexpr std::size_t S_ = 4;
#endif

    public:
    static constexpr bool batch() { return K != 0 && 4 % K == 0; }

    static constexpr std::size_t blocks() { return S_ * 4 / K; }

    static void eval(std::array<T, K> &state, const std::array<T, K / 2> &key)
    {
        eval(state, key, std::integral_constant<bool, K == 4>());
    }

    MCKL_DEFINE_RANDOM_INTERNAL_PHILOX_SSE2_32_EVAL(1)
    MCKL_DEFINE_RANDOM_INTERNAL_PHILOX_SSE2_32_EVAL(2)
    MCKL_DEFINE_RANDOM_INTERNAL_PHILOX_SSE2_32_EVAL(4)
    MCKL_DEFINE_RANDOM_INTERNAL_PHILOX_SSE2_32_EVAL(8)

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
    static void round(std::array<__m128i, S> &, __m128i &, const __m128i &,
        const __m128i &, std::false_type)
    {
    }

    template <std::size_t N, std::size_t S>
    static void round(std::array<__m128i, S> &s, __m128i &p, const __m128i &w,
        const __m128i &m, std::true_type)
    {
        MCKL_FLATTEN_CALL kbox<N>(p, w);
        MCKL_FLATTEN_CALL sbox<N>(s, p, m);
        round<N + 1>(
            s, p, w, m, std::integral_constant<bool, N + 1 <= Rounds>());
    }

    template <std::size_t N>
    static void kbox(__m128i &p, const __m128i &w)
    {
        kbox(p, w, std::integral_constant<bool, (N > 1 && N <= Rounds)>());
    }

    static void kbox(__m128i &, const __m128i &, std::false_type) {}

    static void kbox(__m128i &p, const __m128i &w, std::true_type)
    {
        p = _mm_add_epi32(p, w);
    }

    template <std::size_t N, std::size_t S>
    static void sbox(
        std::array<__m128i, S> &s, const __m128i &p, const __m128i &m)
    {
        sbox<N>(
            s, p, m, std::integral_constant<bool, (N > 0 && N <= Rounds)>());
    }

    template <std::size_t, std::size_t S>
    static void sbox(std::array<__m128i, S> &, const __m128i &,
        const __m128i &, std::false_type)
    {
    }

    template <std::size_t N>
    static void sbox(std::array<__m128i, 1> &s, const __m128i &p,
        const __m128i &m, std::true_type)
    {
        constexpr int msk = static_cast<int>(0xFFFFFFFF);

        const __m128i mask = _mm_set_epi32(msk, 0, msk, 0);

        __m128i m0 = _mm_mul_epu32(std::get<0>(s), m);

        __m128i x0 = _mm_xor_si128(std::get<0>(s), p);

        std::get<0>(s) = _mm_and_si128(x0, mask);

        std::get<0>(s) = _mm_xor_si128(std::get<0>(s), m0);

        MCKL_FLATTEN_CALL permute<N>(s);
    }

    template <std::size_t N>
    static void sbox(std::array<__m128i, 2> &s, const __m128i &p,
        const __m128i &m, std::true_type)
    {
        constexpr int msk = static_cast<int>(0xFFFFFFFF);

        const __m128i mask = _mm_set_epi32(msk, 0, msk, 0);

        __m128i m0 = _mm_mul_epu32(std::get<0>(s), m);
        __m128i m1 = _mm_mul_epu32(std::get<1>(s), m);

        __m128i x0 = _mm_xor_si128(std::get<0>(s), p);
        __m128i x1 = _mm_xor_si128(std::get<1>(s), p);

        std::get<0>(s) = _mm_and_si128(x0, mask);
        std::get<1>(s) = _mm_and_si128(x1, mask);

        std::get<0>(s) = _mm_xor_si128(std::get<0>(s), m0);
        std::get<1>(s) = _mm_xor_si128(std::get<1>(s), m1);

        MCKL_FLATTEN_CALL permute<N>(s);
    }

    template <std::size_t N>
    static void sbox(std::array<__m128i, 4> &s, const __m128i &p,
        const __m128i &m, std::true_type)
    {
        constexpr int msk = static_cast<int>(0xFFFFFFFF);

        const __m128i mask = _mm_set_epi32(msk, 0, msk, 0);

        __m128i m0 = _mm_mul_epu32(std::get<0>(s), m);
        __m128i m1 = _mm_mul_epu32(std::get<1>(s), m);
        __m128i m2 = _mm_mul_epu32(std::get<2>(s), m);
        __m128i m3 = _mm_mul_epu32(std::get<3>(s), m);

        __m128i x0 = _mm_xor_si128(std::get<0>(s), p);
        __m128i x1 = _mm_xor_si128(std::get<1>(s), p);
        __m128i x2 = _mm_xor_si128(std::get<2>(s), p);
        __m128i x3 = _mm_xor_si128(std::get<3>(s), p);

        std::get<0>(s) = _mm_and_si128(x0, mask);
        std::get<1>(s) = _mm_and_si128(x1, mask);
        std::get<2>(s) = _mm_and_si128(x2, mask);
        std::get<3>(s) = _mm_and_si128(x3, mask);

        std::get<0>(s) = _mm_xor_si128(std::get<0>(s), m0);
        std::get<1>(s) = _mm_xor_si128(std::get<1>(s), m1);
        std::get<2>(s) = _mm_xor_si128(std::get<2>(s), m2);
        std::get<3>(s) = _mm_xor_si128(std::get<3>(s), m3);

        permute<N>(s);
    }

    template <std::size_t N>
    static void sbox(std::array<__m128i, 8> &s, const __m128i &p,
        const __m128i &m, std::true_type)
    {
        constexpr int msk = static_cast<int>(0xFFFFFFFF);

        const __m128i mask = _mm_set_epi32(msk, 0, msk, 0);

        __m128i m0 = _mm_mul_epu32(std::get<0>(s), m);
        __m128i m1 = _mm_mul_epu32(std::get<1>(s), m);
        __m128i m2 = _mm_mul_epu32(std::get<2>(s), m);
        __m128i m3 = _mm_mul_epu32(std::get<3>(s), m);
        __m128i m4 = _mm_mul_epu32(std::get<4>(s), m);
        __m128i m5 = _mm_mul_epu32(std::get<5>(s), m);
        __m128i m6 = _mm_mul_epu32(std::get<6>(s), m);
        __m128i m7 = _mm_mul_epu32(std::get<7>(s), m);

        m0 = _mm_xor_si128(m0, p);
        m1 = _mm_xor_si128(m1, p);
        m2 = _mm_xor_si128(m2, p);
        m3 = _mm_xor_si128(m3, p);
        m4 = _mm_xor_si128(m4, p);
        m5 = _mm_xor_si128(m5, p);
        m6 = _mm_xor_si128(m6, p);
        m7 = _mm_xor_si128(m7, p);

        std::get<0>(s) = _mm_and_si128(std::get<0>(s), mask);
        std::get<1>(s) = _mm_and_si128(std::get<1>(s), mask);
        std::get<2>(s) = _mm_and_si128(std::get<2>(s), mask);
        std::get<3>(s) = _mm_and_si128(std::get<3>(s), mask);
        std::get<4>(s) = _mm_and_si128(std::get<4>(s), mask);
        std::get<5>(s) = _mm_and_si128(std::get<5>(s), mask);
        std::get<6>(s) = _mm_and_si128(std::get<6>(s), mask);
        std::get<7>(s) = _mm_and_si128(std::get<7>(s), mask);

        std::get<0>(s) = _mm_xor_si128(std::get<0>(s), m0);
        std::get<1>(s) = _mm_xor_si128(std::get<1>(s), m1);
        std::get<2>(s) = _mm_xor_si128(std::get<2>(s), m2);
        std::get<3>(s) = _mm_xor_si128(std::get<3>(s), m3);
        std::get<4>(s) = _mm_xor_si128(std::get<4>(s), m4);
        std::get<5>(s) = _mm_xor_si128(std::get<5>(s), m5);
        std::get<6>(s) = _mm_xor_si128(std::get<6>(s), m6);
        std::get<7>(s) = _mm_xor_si128(std::get<7>(s), m7);

        permute<N>(s);
    }

    template <std::size_t N, std::size_t S>
    static void permute(std::array<__m128i, S> &s)
    {
        permute(s, std::integral_constant<bool, (N > 0 && N < Rounds)>());
    }

    template <std::size_t S>
    static void permute(std::array<__m128i, S> &, std::false_type)
    {
    }

    template <std::size_t S>
    static void permute(std::array<__m128i, S> &s, std::true_type)
    {
        PhiloxGeneratorImplPermute32<K>::round(s);
    }
}; // class PhiloxGeneratorImpl
