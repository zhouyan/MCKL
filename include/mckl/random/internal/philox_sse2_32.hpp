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
class PhiloxGeneratorImplSSE2_32
{
    static_assert(K == 2 || K == 4,
        "PhiloxGeneratorImplSSE2_32 only support K equal to 2 or 4");

    public:
    static constexpr bool batch() { return true; }

    static constexpr std::size_t blocks() { return 32 / K; }

    static void eval(std::array<T, K> &state, const std::array<T, K / 2> &key)
    {
        PhiloxGeneratorGenericImpl<T, K, Rounds, Constants>::eval(state, key);
    }

    static void eval(std::array<std::array<T, K>, blocks()> &state,
        const std::array<T, K / 2> &key)
    {
        static constexpr std::size_t i0 = 0 % (K / 2);
        static constexpr std::size_t i1 = 1 % (K / 2);

        static constexpr int w0 = static_cast<int>(Constants::weyl[i0]);
        static constexpr int w1 = static_cast<int>(Constants::weyl[i1]);

        static constexpr int m0 = static_cast<int>(Constants::multiplier[i0]);
        static constexpr int m1 = static_cast<int>(Constants::multiplier[i1]);

        const int p0 = static_cast<int>(std::get<i0>(key));
        const int p1 = static_cast<int>(std::get<i1>(key));

        __m128i w = _mm_set_epi32(w1, 0, w0, 0);
        __m128i m = _mm_set_epi32(0, m1, 0, m0);
        __m128i p = _mm_set_epi32(p1, 0, p0, 0);

        std::array<__m128i, 8> s;
        __m128i *sptr = nullptr;

        sptr = reinterpret_cast<__m128i *>(state.data());
        std::get<0x0>(s) = _mm_load_si128(sptr++);
        std::get<0x1>(s) = _mm_load_si128(sptr++);
        std::get<0x2>(s) = _mm_load_si128(sptr++);
        std::get<0x3>(s) = _mm_load_si128(sptr++);
        std::get<0x4>(s) = _mm_load_si128(sptr++);
        std::get<0x5>(s) = _mm_load_si128(sptr++);
        std::get<0x6>(s) = _mm_load_si128(sptr++);
        std::get<0x7>(s) = _mm_load_si128(sptr++);

        Derived::permute_first(s);

        // clang-format off
        kbox<0x0>(p, w); spbox<0x0>(s, p, m);
        kbox<0x1>(p, w); spbox<0x1>(s, p, m);
        kbox<0x2>(p, w); spbox<0x2>(s, p, m);
        kbox<0x3>(p, w); spbox<0x3>(s, p, m);
        kbox<0x4>(p, w); spbox<0x4>(s, p, m);
        kbox<0x5>(p, w); spbox<0x5>(s, p, m);
        kbox<0x6>(p, w); spbox<0x6>(s, p, m);
        kbox<0x7>(p, w); spbox<0x7>(s, p, m);
        kbox<0x8>(p, w); spbox<0x8>(s, p, m);
        kbox<0x9>(p, w); spbox<0x9>(s, p, m);
        kbox<0xA>(p, w); spbox<0xA>(s, p, m);
        kbox<0xB>(p, w); spbox<0xB>(s, p, m);
        kbox<0xC>(p, w); spbox<0xC>(s, p, m);
        kbox<0xD>(p, w); spbox<0xD>(s, p, m);
        kbox<0xE>(p, w); spbox<0xE>(s, p, m);
        kbox<0xF>(p, w); spbox<0xF>(s, p, m);
        // clang-format on

        eval<0x10>(s, p, w, m, std::integral_constant<bool, 0x10 <= Rounds>());

        Derived::permute_last(s);

        sptr = reinterpret_cast<__m128i *>(state.data());
        _mm_store_si128(sptr++, std::get<0x0>(s));
        _mm_store_si128(sptr++, std::get<0x1>(s));
        _mm_store_si128(sptr++, std::get<0x2>(s));
        _mm_store_si128(sptr++, std::get<0x3>(s));
        _mm_store_si128(sptr++, std::get<0x4>(s));
        _mm_store_si128(sptr++, std::get<0x5>(s));
        _mm_store_si128(sptr++, std::get<0x6>(s));
        _mm_store_si128(sptr++, std::get<0x7>(s));
    }

    private:
    template <std::size_t>
    static void eval(std::array<__m128i, 8> &, __m128i &, const __m128i &,
        const __m128i &, std::false_type)
    {
    }

    template <std::size_t N>
    static void eval(std::array<__m128i, 8> &s, __m128i &p, const __m128i &w,
        const __m128i &m, std::true_type)
    {
        kbox<N>(p, w);
        spbox<N>(s, p, m);
        eval<N + 1>(
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

    template <std::size_t N>
    static void spbox(
        std::array<__m128i, 8> &s, const __m128i &p, const __m128i &m)
    {
        spbox<N>(
            s, p, m, std::integral_constant<bool, (N > 0 && N <= Rounds)>());
    }

    template <std::size_t>
    static void spbox(std::array<__m128i, 8> &, const __m128i &,
        const __m128i &, std::false_type)
    {
    }

    template <std::size_t N>
    static void spbox(std::array<__m128i, 8> &s, const __m128i &p,
        const __m128i &m, std::true_type)
    {
        static constexpr int msk = static_cast<int>(0xFFFFFFFF);

        const __m128i mask = _mm_set_epi32(msk, 0, msk, 0);

        __m128i m0 = _mm_mul_epu32(std::get<0x0>(s), m);
        __m128i m1 = _mm_mul_epu32(std::get<0x1>(s), m);
        __m128i m2 = _mm_mul_epu32(std::get<0x2>(s), m);
        __m128i m3 = _mm_mul_epu32(std::get<0x3>(s), m);
        __m128i m4 = _mm_mul_epu32(std::get<0x4>(s), m);
        __m128i m5 = _mm_mul_epu32(std::get<0x5>(s), m);
        __m128i m6 = _mm_mul_epu32(std::get<0x6>(s), m);
        __m128i m7 = _mm_mul_epu32(std::get<0x7>(s), m);

        m0 = _mm_xor_si128(m0, p);
        m1 = _mm_xor_si128(m1, p);
        m2 = _mm_xor_si128(m2, p);
        m3 = _mm_xor_si128(m3, p);
        m4 = _mm_xor_si128(m4, p);
        m5 = _mm_xor_si128(m5, p);
        m6 = _mm_xor_si128(m6, p);
        m7 = _mm_xor_si128(m7, p);

        std::get<0x0>(s) = _mm_and_si128(std::get<0x0>(s), mask);
        std::get<0x1>(s) = _mm_and_si128(std::get<0x1>(s), mask);
        std::get<0x2>(s) = _mm_and_si128(std::get<0x2>(s), mask);
        std::get<0x3>(s) = _mm_and_si128(std::get<0x3>(s), mask);
        std::get<0x4>(s) = _mm_and_si128(std::get<0x4>(s), mask);
        std::get<0x5>(s) = _mm_and_si128(std::get<0x5>(s), mask);
        std::get<0x6>(s) = _mm_and_si128(std::get<0x6>(s), mask);
        std::get<0x7>(s) = _mm_and_si128(std::get<0x7>(s), mask);

        std::get<0x0>(s) = _mm_xor_si128(std::get<0x0>(s), m0);
        std::get<0x1>(s) = _mm_xor_si128(std::get<0x1>(s), m1);
        std::get<0x2>(s) = _mm_xor_si128(std::get<0x2>(s), m2);
        std::get<0x3>(s) = _mm_xor_si128(std::get<0x3>(s), m3);
        std::get<0x4>(s) = _mm_xor_si128(std::get<0x4>(s), m4);
        std::get<0x5>(s) = _mm_xor_si128(std::get<0x5>(s), m5);
        std::get<0x6>(s) = _mm_xor_si128(std::get<0x6>(s), m6);
        std::get<0x7>(s) = _mm_xor_si128(std::get<0x7>(s), m7);

        permute<N>(s);
    }

    template <std::size_t N>
    static void permute(std::array<__m128i, 8> &s)
    {
        permute(s, std::integral_constant<bool, (N > 0 && N < Rounds)>());
    }

    static void permute(std::array<__m128i, 8> &, std::false_type) {}

    static void permute(std::array<__m128i, 8> &s, std::true_type)
    {
        Derived::permute(s);
    }
}; // class PhiloxGeneratorImplSSE2_32

template <typename T, std::size_t Rounds, typename Constants>
class PhiloxGeneratorImpl<T, 2, Rounds, Constants, 32>
    : public PhiloxGeneratorImplSSE2_32<T, 2, Rounds, Constants,
          PhiloxGeneratorImpl<T, 2, Rounds, Constants>>
{
    friend PhiloxGeneratorImplSSE2_32<T, 2, Rounds, Constants,
        PhiloxGeneratorImpl<T, 2, Rounds, Constants>>;

    static void permute_first(std::array<__m128i, 8> &) {}

    static void permute(std::array<__m128i, 8> &s)
    {
        // 2 3 0 1
        std::get<0x0>(s) = _mm_shuffle_epi32(std::get<0x0>(s), 0xB1);
        std::get<0x1>(s) = _mm_shuffle_epi32(std::get<0x1>(s), 0xB1);
        std::get<0x2>(s) = _mm_shuffle_epi32(std::get<0x2>(s), 0xB1);
        std::get<0x3>(s) = _mm_shuffle_epi32(std::get<0x3>(s), 0xB1);
        std::get<0x4>(s) = _mm_shuffle_epi32(std::get<0x4>(s), 0xB1);
        std::get<0x5>(s) = _mm_shuffle_epi32(std::get<0x5>(s), 0xB1);
        std::get<0x6>(s) = _mm_shuffle_epi32(std::get<0x6>(s), 0xB1);
        std::get<0x7>(s) = _mm_shuffle_epi32(std::get<0x7>(s), 0xB1);
    }

    static void permute_last(std::array<__m128i, 8> &s)
    {
        // 2 3 0 1
        std::get<0x0>(s) = _mm_shuffle_epi32(std::get<0x0>(s), 0xB1);
        std::get<0x1>(s) = _mm_shuffle_epi32(std::get<0x1>(s), 0xB1);
        std::get<0x2>(s) = _mm_shuffle_epi32(std::get<0x2>(s), 0xB1);
        std::get<0x3>(s) = _mm_shuffle_epi32(std::get<0x3>(s), 0xB1);
        std::get<0x4>(s) = _mm_shuffle_epi32(std::get<0x4>(s), 0xB1);
        std::get<0x5>(s) = _mm_shuffle_epi32(std::get<0x5>(s), 0xB1);
        std::get<0x6>(s) = _mm_shuffle_epi32(std::get<0x6>(s), 0xB1);
        std::get<0x7>(s) = _mm_shuffle_epi32(std::get<0x7>(s), 0xB1);
    }
}; // class PhiloxGeneratorImpl

template <typename T, std::size_t Rounds, typename Constants>
class PhiloxGeneratorImpl<T, 4, Rounds, Constants, 32>
    : public PhiloxGeneratorImplSSE2_32<T, 4, Rounds, Constants,
          PhiloxGeneratorImpl<T, 4, Rounds, Constants>>
{
    friend PhiloxGeneratorImplSSE2_32<T, 4, Rounds, Constants,
        PhiloxGeneratorImpl<T, 4, Rounds, Constants>>;

    static void permute_first(std::array<__m128i, 8> &s)
    {
        // 3 0 1 2
        std::get<0x0>(s) = _mm_shuffle_epi32(std::get<0x0>(s), 0xC6);
        std::get<0x1>(s) = _mm_shuffle_epi32(std::get<0x1>(s), 0xC6);
        std::get<0x2>(s) = _mm_shuffle_epi32(std::get<0x2>(s), 0xC6);
        std::get<0x3>(s) = _mm_shuffle_epi32(std::get<0x3>(s), 0xC6);
        std::get<0x4>(s) = _mm_shuffle_epi32(std::get<0x4>(s), 0xC6);
        std::get<0x5>(s) = _mm_shuffle_epi32(std::get<0x5>(s), 0xC6);
        std::get<0x6>(s) = _mm_shuffle_epi32(std::get<0x6>(s), 0xC6);
        std::get<0x7>(s) = _mm_shuffle_epi32(std::get<0x7>(s), 0xC6);
    }

    static void permute(std::array<__m128i, 8> &s)
    {
        // 2 1 0 3
        std::get<0x0>(s) = _mm_shuffle_epi32(std::get<0x0>(s), 0x93);
        std::get<0x1>(s) = _mm_shuffle_epi32(std::get<0x1>(s), 0x93);
        std::get<0x2>(s) = _mm_shuffle_epi32(std::get<0x2>(s), 0x93);
        std::get<0x3>(s) = _mm_shuffle_epi32(std::get<0x3>(s), 0x93);
        std::get<0x4>(s) = _mm_shuffle_epi32(std::get<0x4>(s), 0x93);
        std::get<0x5>(s) = _mm_shuffle_epi32(std::get<0x5>(s), 0x93);
        std::get<0x6>(s) = _mm_shuffle_epi32(std::get<0x6>(s), 0x93);
        std::get<0x7>(s) = _mm_shuffle_epi32(std::get<0x7>(s), 0x93);
    }

    static void permute_last(std::array<__m128i, 8> &s)
    {
        // 2 3 0 1
        std::get<0x0>(s) = _mm_shuffle_epi32(std::get<0x0>(s), 0xB1);
        std::get<0x1>(s) = _mm_shuffle_epi32(std::get<0x1>(s), 0xB1);
        std::get<0x2>(s) = _mm_shuffle_epi32(std::get<0x2>(s), 0xB1);
        std::get<0x3>(s) = _mm_shuffle_epi32(std::get<0x3>(s), 0xB1);
        std::get<0x4>(s) = _mm_shuffle_epi32(std::get<0x4>(s), 0xB1);
        std::get<0x5>(s) = _mm_shuffle_epi32(std::get<0x5>(s), 0xB1);
        std::get<0x6>(s) = _mm_shuffle_epi32(std::get<0x6>(s), 0xB1);
        std::get<0x7>(s) = _mm_shuffle_epi32(std::get<0x7>(s), 0xB1);
    }
}; // class PhiloxGeneratorImpl
