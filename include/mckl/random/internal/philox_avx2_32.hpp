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

template <typename T, std::size_t K, std::size_t Rounds, typename Constants,
    typename Derived>
class PhiloxGeneratorImplAVX2_32
{
    static_assert(K == 2 || K == 4 || K == 8,
        "PhiloxGeneratorImplSSE2_32 only support K equal to 2, 4 or 8");

    public:
    static constexpr bool batch() { return true; }

    static constexpr std::size_t blocks() { return 64 / K; }

    static void eval(std::array<T, K> &state, const std::array<T, K / 2> &key)
    {
        PhiloxGeneratorGenericImpl<T, K, Rounds, Constants>::eval(state, key);
    }

    static void eval(std::array<std::array<T, K>, blocks()> &state,
        const std::array<T, K / 2> &key)
    {
        static constexpr std::size_t i0 = 0 % (K / 2);
        static constexpr std::size_t i1 = 1 % (K / 2);
        static constexpr std::size_t i2 = 2 % (K / 2);
        static constexpr std::size_t i3 = 3 % (K / 2);

        static constexpr int w0 = static_cast<int>(Constants::weyl[i0]);
        static constexpr int w1 = static_cast<int>(Constants::weyl[i1]);
        static constexpr int w2 = static_cast<int>(Constants::weyl[i2]);
        static constexpr int w3 = static_cast<int>(Constants::weyl[i3]);

        static constexpr int m0 = static_cast<int>(Constants::multiplier[i0]);
        static constexpr int m1 = static_cast<int>(Constants::multiplier[i1]);
        static constexpr int m2 = static_cast<int>(Constants::multiplier[i2]);
        static constexpr int m3 = static_cast<int>(Constants::multiplier[i3]);

        const int p0 = static_cast<int>(std::get<i0>(key));
        const int p1 = static_cast<int>(std::get<i1>(key));
        const int p2 = static_cast<int>(std::get<i2>(key));
        const int p3 = static_cast<int>(std::get<i3>(key));

        __m256i w = _mm256_set_epi32(w3, 0, w2, 0, w1, 0, w0, 0);
        __m256i m = _mm256_set_epi32(0, m3, 0, m2, 0, m1, 0, m0);
        __m256i p = _mm256_set_epi32(p3, 0, p2, 0, p1, 0, p0, 0);

        std::array<__m256i, 8> s;
        pack(state, s);
        Derived::permute_first(s);

        // clang-format off
        kbox<0x00>(p, w); spbox<0x00>(s, p, m);
        kbox<0x01>(p, w); spbox<0x01>(s, p, m);
        kbox<0x02>(p, w); spbox<0x02>(s, p, m);
        kbox<0x03>(p, w); spbox<0x03>(s, p, m);
        kbox<0x04>(p, w); spbox<0x04>(s, p, m);
        kbox<0x05>(p, w); spbox<0x05>(s, p, m);
        kbox<0x06>(p, w); spbox<0x06>(s, p, m);
        kbox<0x07>(p, w); spbox<0x07>(s, p, m);
        kbox<0x08>(p, w); spbox<0x08>(s, p, m);
        kbox<0x09>(p, w); spbox<0x09>(s, p, m);
        kbox<0x0A>(p, w); spbox<0x0A>(s, p, m);
        kbox<0x0B>(p, w); spbox<0x0B>(s, p, m);
        kbox<0x0C>(p, w); spbox<0x0C>(s, p, m);
        kbox<0x0D>(p, w); spbox<0x0D>(s, p, m);
        kbox<0x0E>(p, w); spbox<0x0E>(s, p, m);
        kbox<0x0F>(p, w); spbox<0x0F>(s, p, m);
        // clang-format on

        eval<0x10>(s, p, w, m, std::integral_constant<bool, 0x10 <= Rounds>());
        Derived::permute_last(s);
        unpack(state, s);
    }

    private:
    template <std::size_t>
    static void eval(std::array<__m256i, 8> &, __m256i &, const __m256i &,
        const __m256i &, std::false_type)
    {
    }

    template <std::size_t N>
    static void eval(std::array<__m256i, 8> &s, __m256i &p, const __m256i &w,
        const __m256i &m, std::true_type)
    {
        kbox<N>(p, w);
        spbox<N>(s, p, m);
        eval<N + 1>(
            s, p, w, m, std::integral_constant<bool, N + 1 <= Rounds>());
    }

    static void pack(std::array<std::array<T, K>, blocks()> &state,
        std::array<__m256i, 8> &s)
    {
        const __m256i *sptr = reinterpret_cast<const __m256i *>(state.data());
        std::get<0>(s) = _mm256_load_si256(sptr++);
        std::get<1>(s) = _mm256_load_si256(sptr++);
        std::get<2>(s) = _mm256_load_si256(sptr++);
        std::get<3>(s) = _mm256_load_si256(sptr++);
        std::get<4>(s) = _mm256_load_si256(sptr++);
        std::get<5>(s) = _mm256_load_si256(sptr++);
        std::get<6>(s) = _mm256_load_si256(sptr++);
        std::get<7>(s) = _mm256_load_si256(sptr++);
    }

    static void unpack(std::array<std::array<T, K>, blocks()> &state,
        std::array<__m256i, 8> &s)
    {
        __m256i *sptr = reinterpret_cast<__m256i *>(state.data());
        _mm256_store_si256(sptr++, std::get<0>(s));
        _mm256_store_si256(sptr++, std::get<1>(s));
        _mm256_store_si256(sptr++, std::get<2>(s));
        _mm256_store_si256(sptr++, std::get<3>(s));
        _mm256_store_si256(sptr++, std::get<4>(s));
        _mm256_store_si256(sptr++, std::get<5>(s));
        _mm256_store_si256(sptr++, std::get<6>(s));
        _mm256_store_si256(sptr++, std::get<7>(s));
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

    template <std::size_t N>
    static void spbox(
        std::array<__m256i, 8> &s, const __m256i &p, const __m256i &m)
    {
        spbox<N>(
            s, p, m, std::integral_constant<bool, (N > 0 && N <= Rounds)>());
    }

    template <std::size_t>
    static void spbox(std::array<__m256i, 8> &, const __m256i &,
        const __m256i &, std::false_type)
    {
    }

    template <std::size_t N>
    static void spbox(std::array<__m256i, 8> &s, const __m256i &p,
        const __m256i &m, std::true_type)
    {
        static constexpr int msk = static_cast<int>(0xFFFFFFFF);

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

    template <std::size_t N>
    static void permute(std::array<__m256i, 8> &s)
    {
        permute(s, std::integral_constant<bool, (N > 0 && N < Rounds)>());
    }

    static void permute(std::array<__m256i, 8> &, std::false_type) {}

    static void permute(std::array<__m256i, 8> &s, std::true_type)
    {
        Derived::permute(s);
    }
}; // class PhiloxGeneratorImplAVX2_32

template <typename T, std::size_t Rounds, typename Constants>
class PhiloxGeneratorImpl<T, 2, Rounds, Constants, 32>
    : public PhiloxGeneratorImplAVX2_32<T, 2, Rounds, Constants,
          PhiloxGeneratorImpl<T, 2, Rounds, Constants>>
{
    friend PhiloxGeneratorImplAVX2_32<T, 2, Rounds, Constants,
        PhiloxGeneratorImpl<T, 2, Rounds, Constants>>;

    static void permute_first(std::array<__m256i, 8> &) {}

    static void permute(std::array<__m256i, 8> &s)
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

    static void permute_last(std::array<__m256i, 8> &s)
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
}; // class PhiloxGeneratorImpl

template <typename T, std::size_t Rounds, typename Constants>
class PhiloxGeneratorImpl<T, 4, Rounds, Constants, 32>
    : public PhiloxGeneratorImplAVX2_32<T, 4, Rounds, Constants,
          PhiloxGeneratorImpl<T, 4, Rounds, Constants>>
{
    friend PhiloxGeneratorImplAVX2_32<T, 4, Rounds, Constants,
        PhiloxGeneratorImpl<T, 4, Rounds, Constants>>;

    static void permute_first(std::array<__m256i, 8> &s)
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

    static void permute(std::array<__m256i, 8> &s)
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

    static void permute_last(std::array<__m256i, 8> &s)
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
}; // class PhiloxGeneratorImpl
