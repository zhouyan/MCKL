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

template <typename T, std::size_t K, std::size_t Rounds, typename Constants,
    typename Derived>
class PhiloxGeneratorSSE2Impl<T, K, Rounds, Constants, Derived, 32>
{
    public:
    static constexpr bool batch()
    {
        return K != 0 && 32 % K == 0 ?
            true :
            PhiloxGeneratorGenericImpl<T, K, Rounds, Constants>::blocks();
    }

    static constexpr std::size_t blocks()
    {
        return K != 0 && 32 % K == 0 ?
            32 / K :
            PhiloxGeneratorGenericImpl<T, K, Rounds, Constants>::blocks();
    }

    static void eval(std::array<T, K> &state, const std::array<T, K / 2> &key)
    {
        eval(state, key,
            std::integral_constant<bool, (K != 0 && K % 4 == 0)>());
    }

    static void eval(std::array<std::array<T, K>, blocks()> &state,
        const std::array<T, K / 2> &key)
    {
        eval(state, key,
            std::integral_constant<bool, (K != 0 && 32 % K == 0)>());
    }

    private:
    static constexpr std::size_t M_ = K / 4 + (K % 4 == 0 ? 0 : 1);

    static void eval(std::array<T, K> &state, const std::array<T, K / 2> &key,
        std::false_type)
    {
        PhiloxGeneratorGenericImpl<T, K, Rounds, Constants>::eval(state, key);
    }

    static void eval(std::array<T, K> &state, const std::array<T, K / 2> &key,
        std::true_type)
    {
        PhiloxGeneratorGenericImpl<T, K, Rounds, Constants>::eval(state, key);
    }

    static void eval(std::array<std::array<T, K>, blocks()> &state,
        const std::array<T, K / 2> &key, std::false_type)
    {
        PhiloxGeneratorGenericImpl<T, K, Rounds, Constants>::eval(state, key);
    }

    static void eval(std::array<std::array<T, K>, blocks()> &state,
        const std::array<T, K / 2> &key, std::true_type)
    {
        std::array<__m128i, M_> k;
        std::array<__m128i, M_> w;
        std::array<__m128i, M_> m;
        std::array<__m128i, 8> s;
        std::array<__m128i, 8> t;

        init_k<0>(k, key, std::integral_constant<bool, 0 < M_>());
        init_w<0>(w, std::integral_constant<bool, 0 < M_>());
        init_m<0>(m, std::integral_constant<bool, 0 < M_>());

        pack(state, s, t);

        // clang-format off
        kbox<0x00>(k, w); pbox<0x00>(s, t); sbox<0x00>(s, t, k, m);
        kbox<0x01>(k, w); pbox<0x01>(s, t); sbox<0x01>(s, t, k, m);
        kbox<0x02>(k, w); pbox<0x02>(s, t); sbox<0x02>(s, t, k, m);
        kbox<0x03>(k, w); pbox<0x03>(s, t); sbox<0x03>(s, t, k, m);
        kbox<0x04>(k, w); pbox<0x04>(s, t); sbox<0x04>(s, t, k, m);
        kbox<0x05>(k, w); pbox<0x05>(s, t); sbox<0x05>(s, t, k, m);
        kbox<0x06>(k, w); pbox<0x06>(s, t); sbox<0x06>(s, t, k, m);
        kbox<0x07>(k, w); pbox<0x07>(s, t); sbox<0x07>(s, t, k, m);
        kbox<0x08>(k, w); pbox<0x08>(s, t); sbox<0x08>(s, t, k, m);
        kbox<0x09>(k, w); pbox<0x09>(s, t); sbox<0x09>(s, t, k, m);
        kbox<0x0A>(k, w); pbox<0x0A>(s, t); sbox<0x0A>(s, t, k, m);
        kbox<0x0B>(k, w); pbox<0x0B>(s, t); sbox<0x0B>(s, t, k, m);
        kbox<0x0C>(k, w); pbox<0x0C>(s, t); sbox<0x0C>(s, t, k, m);
        kbox<0x0D>(k, w); pbox<0x0D>(s, t); sbox<0x0D>(s, t, k, m);
        kbox<0x0E>(k, w); pbox<0x0E>(s, t); sbox<0x0E>(s, t, k, m);
        kbox<0x0F>(k, w); pbox<0x0F>(s, t); sbox<0x0F>(s, t, k, m);
        // clang-format on

        eval<0x10>(
            s, t, k, w, m, std::integral_constant<bool, 0x10 <= Rounds>());

        unpack(state, s, t);
    }

    template <std::size_t>
    static void eval(std::array<__m128i, 8> &, std::array<__m128i, 8> &,
        std::array<__m128i, M_> &, const std::array<__m128i, M_> &,
        const std::array<__m128i, M_> &, std::false_type)
    {
    }

    template <std::size_t N>
    static void eval(std::array<__m128i, 8> &s, std::array<__m128i, 8> &t,
        std::array<__m128i, M_> &k, const std::array<__m128i, M_> &w,
        const std::array<__m128i, M_> &m, std::true_type)
    {
        kbox(k, w);
        pbox<N>(s, t);
        sbox(s, t, k, m);
        eval<N + 1>(
            s, t, k, w, m, std::integral_constant<bool, N + 1 <= Rounds>());
    }

    static void pack(std::array<std::array<T, K>, blocks()> &state,
        std::array<__m128i, 8> &s, std::array<__m128i, 8> &t)
    {
        const __m128i *sptr = reinterpret_cast<const __m128i *>(state.data());
        std::get<0>(s) = _mm_load_si128(sptr++);
        std::get<1>(s) = _mm_load_si128(sptr++);
        std::get<2>(s) = _mm_load_si128(sptr++);
        std::get<3>(s) = _mm_load_si128(sptr++);
        std::get<4>(s) = _mm_load_si128(sptr++);
        std::get<5>(s) = _mm_load_si128(sptr++);
        std::get<6>(s) = _mm_load_si128(sptr++);
        std::get<7>(s) = _mm_load_si128(sptr++);

        std::get<0>(t) = _mm_srli_epi64(std::get<0>(s), 32);
        std::get<1>(t) = _mm_srli_epi64(std::get<1>(s), 32);
        std::get<2>(t) = _mm_srli_epi64(std::get<2>(s), 32);
        std::get<3>(t) = _mm_srli_epi64(std::get<3>(s), 32);
        std::get<4>(t) = _mm_srli_epi64(std::get<4>(s), 32);
        std::get<5>(t) = _mm_srli_epi64(std::get<5>(s), 32);
        std::get<6>(t) = _mm_srli_epi64(std::get<6>(s), 32);
        std::get<7>(t) = _mm_srli_epi64(std::get<7>(s), 32);
    }

    static void unpack(std::array<std::array<T, K>, blocks()> &state,
        std::array<__m128i, 8> &s, std::array<__m128i, 8> &t)
    {
        // 2 0 1 3
        std::get<0>(s) = _mm_shuffle_epi32(std::get<0>(s), 0x87);
        std::get<1>(s) = _mm_shuffle_epi32(std::get<1>(s), 0x87);
        std::get<2>(s) = _mm_shuffle_epi32(std::get<2>(s), 0x87);
        std::get<3>(s) = _mm_shuffle_epi32(std::get<3>(s), 0x87);
        std::get<4>(s) = _mm_shuffle_epi32(std::get<4>(s), 0x87);
        std::get<5>(s) = _mm_shuffle_epi32(std::get<5>(s), 0x87);
        std::get<6>(s) = _mm_shuffle_epi32(std::get<6>(s), 0x87);
        std::get<7>(s) = _mm_shuffle_epi32(std::get<7>(s), 0x87);

        // 2 0 1 3
        std::get<0>(t) = _mm_shuffle_epi32(std::get<0>(t), 0x87);
        std::get<1>(t) = _mm_shuffle_epi32(std::get<1>(t), 0x87);
        std::get<2>(t) = _mm_shuffle_epi32(std::get<2>(t), 0x87);
        std::get<3>(t) = _mm_shuffle_epi32(std::get<3>(t), 0x87);
        std::get<4>(t) = _mm_shuffle_epi32(std::get<4>(t), 0x87);
        std::get<5>(t) = _mm_shuffle_epi32(std::get<5>(t), 0x87);
        std::get<6>(t) = _mm_shuffle_epi32(std::get<6>(t), 0x87);
        std::get<7>(t) = _mm_shuffle_epi32(std::get<7>(t), 0x87);

        std::get<0>(s) = _mm_unpackhi_epi32(std::get<0>(s), std::get<0>(t));
        std::get<1>(s) = _mm_unpackhi_epi32(std::get<1>(s), std::get<1>(t));
        std::get<2>(s) = _mm_unpackhi_epi32(std::get<2>(s), std::get<2>(t));
        std::get<3>(s) = _mm_unpackhi_epi32(std::get<3>(s), std::get<3>(t));
        std::get<4>(s) = _mm_unpackhi_epi32(std::get<4>(s), std::get<4>(t));
        std::get<5>(s) = _mm_unpackhi_epi32(std::get<5>(s), std::get<5>(t));
        std::get<6>(s) = _mm_unpackhi_epi32(std::get<6>(s), std::get<6>(t));
        std::get<7>(s) = _mm_unpackhi_epi32(std::get<7>(s), std::get<7>(t));

        __m128i *sptr = reinterpret_cast<__m128i *>(state.data());
        _mm_store_si128(sptr++, std::get<0x0>(s));
        _mm_store_si128(sptr++, std::get<0x1>(s));
        _mm_store_si128(sptr++, std::get<0x2>(s));
        _mm_store_si128(sptr++, std::get<0x3>(s));
        _mm_store_si128(sptr++, std::get<0x4>(s));
        _mm_store_si128(sptr++, std::get<0x5>(s));
        _mm_store_si128(sptr++, std::get<0x6>(s));
        _mm_store_si128(sptr++, std::get<0x7>(s));
    }

    template <std::size_t>
    static void init_k(std::array<__m128i, M_> &, const std::array<T, K / 2> &,
        std::false_type)
    {
    }

    template <std::size_t I>
    static void init_k(std::array<__m128i, M_> &k,
        const std::array<T, K / 2> &key, std::true_type)
    {
        static constexpr std::size_t i0 = (I * 2 + 0) % (K / 2);
        static constexpr std::size_t i1 = (I * 2 + 1) % (K / 2);

        const int e0 = static_cast<int>(std::get<i0>(key));
        const int e1 = static_cast<int>(std::get<i1>(key));

        std::get<I>(k) = _mm_set_epi32(0, e1, 0, e0);
        init_k<I + 1>(k, key, std::integral_constant<bool, I + 1 < M_>());
    }

    template <std::size_t>
    static void init_w(std::array<__m128i, M_> &, std::false_type)
    {
    }

    template <std::size_t I>
    static void init_w(std::array<__m128i, M_> &w, std::true_type)
    {
        static constexpr std::size_t i0 = (I * 2 + 0) % (K / 2);
        static constexpr std::size_t i1 = (I * 2 + 1) % (K / 2);

        static constexpr int e0 = static_cast<int>(Constants::weyl[i0]);
        static constexpr int e1 = static_cast<int>(Constants::weyl[i1]);

        std::get<I>(w) = _mm_set_epi32(0, e1, 0, e0);
        init_w<I + 1>(w, std::integral_constant<bool, I + 1 < M_>());
    }

    template <std::size_t>
    static void init_m(std::array<__m128i, M_> &, std::false_type)
    {
    }

    template <std::size_t I>
    static void init_m(std::array<__m128i, M_> &m, std::true_type)
    {
        static constexpr std::size_t i0 = (I * 2 + 0) % (K / 2);
        static constexpr std::size_t i1 = (I * 2 + 1) % (K / 2);

        static constexpr int e0 = static_cast<int>(Constants::multiplier[i0]);
        static constexpr int e1 = static_cast<int>(Constants::multiplier[i1]);

        std::get<I>(m) = _mm_set_epi32(0, e1, 0, e0);
        init_m<I + 1>(m, std::integral_constant<bool, I + 1 < M_>());
    }

    template <std::size_t N>
    static void kbox(
        std::array<__m128i, M_> &k, const std::array<__m128i, M_> &w)
    {
        kbox(k, w, std::integral_constant<bool, (N > 1 && N <= Rounds)>());
    }

    static void kbox(std::array<__m128i, M_> &,
        const std::array<__m128i, M_> &, std::false_type)
    {
    }

    static void kbox(std::array<__m128i, M_> &k,
        const std::array<__m128i, M_> &w, std::true_type)
    {
        kbox<0>(k, w, std::integral_constant<bool, 0 < M_>());
    }

    template <std::size_t>
    static void kbox(std::array<__m128i, M_> &,
        const std::array<__m128i, M_> &, std::false_type)
    {
    }

    template <std::size_t I>
    static void kbox(std::array<__m128i, M_> &k,
        const std::array<__m128i, M_> &w, std::true_type)
    {
        std::get<I>(k) = _mm_add_epi32(std::get<I>(k), std::get<I>(w));
        kbox<I + 1>(k, w, std::integral_constant<bool, I + 1 < M_>());
    }

    template <std::size_t N>
    static void sbox(std::array<__m128i, 8> &s, std::array<__m128i, 8> &t,
        const std::array<__m128i, M_> &k, const std::array<__m128i, M_> &m)
    {
        sbox(s, t, k, m,
            std::integral_constant<bool, (N > 0 && N <= Rounds)>());
    }

    static void sbox(std::array<__m128i, 8> &, std::array<__m128i, 8> &,
        const std::array<__m128i, M_> &, const std::array<__m128i, M_> &,
        std::false_type)
    {
    }

    static void sbox(std::array<__m128i, 8> &s, std::array<__m128i, 8> &t,
        const std::array<__m128i, M_> &k, const std::array<__m128i, M_> &m,
        std::true_type)
    {
        __m128i x0 = _mm_xor_si128(std::get<0>(t), std::get<0 % M_>(k));
        __m128i x1 = _mm_xor_si128(std::get<1>(t), std::get<1 % M_>(k));
        __m128i x2 = _mm_xor_si128(std::get<2>(t), std::get<2 % M_>(k));
        __m128i x3 = _mm_xor_si128(std::get<3>(t), std::get<3 % M_>(k));
        __m128i x4 = _mm_xor_si128(std::get<4>(t), std::get<4 % M_>(k));
        __m128i x5 = _mm_xor_si128(std::get<5>(t), std::get<5 % M_>(k));
        __m128i x6 = _mm_xor_si128(std::get<6>(t), std::get<6 % M_>(k));
        __m128i x7 = _mm_xor_si128(std::get<7>(t), std::get<7 % M_>(k));

        std::get<0>(t) = _mm_mul_epu32(std::get<0>(s), std::get<0 % M_>(m));
        std::get<1>(t) = _mm_mul_epu32(std::get<1>(s), std::get<1 % M_>(m));
        std::get<2>(t) = _mm_mul_epu32(std::get<2>(s), std::get<2 % M_>(m));
        std::get<3>(t) = _mm_mul_epu32(std::get<3>(s), std::get<3 % M_>(m));
        std::get<4>(t) = _mm_mul_epu32(std::get<4>(s), std::get<4 % M_>(m));
        std::get<5>(t) = _mm_mul_epu32(std::get<5>(s), std::get<5 % M_>(m));
        std::get<6>(t) = _mm_mul_epu32(std::get<6>(s), std::get<6 % M_>(m));
        std::get<7>(t) = _mm_mul_epu32(std::get<7>(s), std::get<7 % M_>(m));

        // 2 3 0 1
        std::get<0>(s) = _mm_shuffle_epi32(std::get<0>(t), 0xB1);
        std::get<1>(s) = _mm_shuffle_epi32(std::get<1>(t), 0xB1);
        std::get<2>(s) = _mm_shuffle_epi32(std::get<2>(t), 0xB1);
        std::get<3>(s) = _mm_shuffle_epi32(std::get<3>(t), 0xB1);
        std::get<4>(s) = _mm_shuffle_epi32(std::get<4>(t), 0xB1);
        std::get<5>(s) = _mm_shuffle_epi32(std::get<5>(t), 0xB1);
        std::get<6>(s) = _mm_shuffle_epi32(std::get<6>(t), 0xB1);
        std::get<7>(s) = _mm_shuffle_epi32(std::get<7>(t), 0xB1);

        std::get<0>(s) = _mm_xor_si128(std::get<0>(s), x0);
        std::get<1>(s) = _mm_xor_si128(std::get<1>(s), x1);
        std::get<2>(s) = _mm_xor_si128(std::get<2>(s), x2);
        std::get<3>(s) = _mm_xor_si128(std::get<3>(s), x3);
        std::get<4>(s) = _mm_xor_si128(std::get<4>(s), x4);
        std::get<5>(s) = _mm_xor_si128(std::get<5>(s), x5);
        std::get<6>(s) = _mm_xor_si128(std::get<6>(s), x6);
        std::get<7>(s) = _mm_xor_si128(std::get<7>(s), x7);
    }

    template <std::size_t N>
    static void pbox(std::array<__m128i, 8> &s, std::array<__m128i, 8> &t)
    {
        pbox<N>(s, t, std::integral_constant<bool, (N > 0 && N <= Rounds)>());
    }

    template <std::size_t N>
    static void pbox(
        std::array<__m128i, 8> &, std::array<__m128i, 8> &, std::false_type)
    {
    }

    template <std::size_t N>
    static void pbox(
        std::array<__m128i, 8> &s, std::array<__m128i, 8> &t, std::true_type)
    {
        Derived::template permute<N>(s, t);
    }
}; // class PhiloxGeneratorSSE2Impl

template <typename T, std::size_t Rounds, typename Constants>
class PhiloxGeneratorImpl<T, 2, Rounds, Constants, 32>
    : public PhiloxGeneratorSSE2Impl<T, 2, Rounds, Constants,
          PhiloxGeneratorImpl<T, 2, Rounds, Constants>>
{
    friend PhiloxGeneratorSSE2Impl<T, 2, Rounds, Constants,
        PhiloxGeneratorImpl<T, 2, Rounds, Constants>>;

    template <std::size_t>
    static void permute(std::array<__m128i, 8> &, std::array<__m128i, 8> &)
    {
    }
}; // class PhiloxGeneratorImpl

template <typename T, std::size_t Rounds, typename Constants>
class PhiloxGeneratorImpl<T, 4, Rounds, Constants, 32>
    : public PhiloxGeneratorSSE2Impl<T, 4, Rounds, Constants,
          PhiloxGeneratorImpl<T, 4, Rounds, Constants>>
{
    friend PhiloxGeneratorSSE2Impl<T, 4, Rounds, Constants,
        PhiloxGeneratorImpl<T, 4, Rounds, Constants>>;

    template <std::size_t>
    static void permute(std::array<__m128i, 8> &s, std::array<__m128i, 8> &)
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
}; // class PhiloxGeneratorImpl
