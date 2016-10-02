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

    static constexpr std::size_t blocks() { return 16 / K; }

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

        const __m128i w = _mm_set_epi32(w1, 0, w0, 0);
        const __m128i m = _mm_set_epi32(0, m1, 0, m0);
        __m128i p = _mm_set_epi32(p1, 0, p0, 0);

        std::array<__m128i, 4> s;
        __m128i *sptr = nullptr;

        sptr = reinterpret_cast<__m128i *>(state.data());
        std::get<0>(s) = _mm_load_si128(sptr++);
        std::get<1>(s) = _mm_load_si128(sptr++);
        std::get<2>(s) = _mm_load_si128(sptr++);
        std::get<3>(s) = _mm_load_si128(sptr++);

        MCKL_FLATTEN_CALL Derived::permute_first(s);

        MCKL_FLATTEN_CALL kbox<0x0>(p, w);
        MCKL_FLATTEN_CALL spbox<0x0>(s, p, m);
        MCKL_FLATTEN_CALL kbox<0x1>(p, w);
        MCKL_FLATTEN_CALL spbox<0x1>(s, p, m);
        MCKL_FLATTEN_CALL kbox<0x2>(p, w);
        MCKL_FLATTEN_CALL spbox<0x2>(s, p, m);
        MCKL_FLATTEN_CALL kbox<0x3>(p, w);
        MCKL_FLATTEN_CALL spbox<0x3>(s, p, m);
        MCKL_FLATTEN_CALL kbox<0x4>(p, w);
        MCKL_FLATTEN_CALL spbox<0x4>(s, p, m);
        MCKL_FLATTEN_CALL kbox<0x5>(p, w);
        MCKL_FLATTEN_CALL spbox<0x5>(s, p, m);
        MCKL_FLATTEN_CALL kbox<0x6>(p, w);
        MCKL_FLATTEN_CALL spbox<0x6>(s, p, m);
        MCKL_FLATTEN_CALL kbox<0x7>(p, w);
        MCKL_FLATTEN_CALL spbox<0x7>(s, p, m);
        MCKL_FLATTEN_CALL kbox<0x8>(p, w);
        MCKL_FLATTEN_CALL spbox<0x8>(s, p, m);
        MCKL_FLATTEN_CALL kbox<0x9>(p, w);
        MCKL_FLATTEN_CALL spbox<0x9>(s, p, m);
        MCKL_FLATTEN_CALL kbox<0xA>(p, w);
        MCKL_FLATTEN_CALL spbox<0xA>(s, p, m);
        MCKL_FLATTEN_CALL kbox<0xB>(p, w);
        MCKL_FLATTEN_CALL spbox<0xB>(s, p, m);
        MCKL_FLATTEN_CALL kbox<0xC>(p, w);
        MCKL_FLATTEN_CALL spbox<0xC>(s, p, m);
        MCKL_FLATTEN_CALL kbox<0xD>(p, w);
        MCKL_FLATTEN_CALL spbox<0xD>(s, p, m);
        MCKL_FLATTEN_CALL kbox<0xE>(p, w);
        MCKL_FLATTEN_CALL spbox<0xE>(s, p, m);
        MCKL_FLATTEN_CALL kbox<0xF>(p, w);
        MCKL_FLATTEN_CALL spbox<0xF>(s, p, m);

        round<0x10>(
            s, p, w, m, std::integral_constant<bool, 0x10 <= Rounds>());

        MCKL_FLATTEN_CALL Derived::permute_last(s);

        sptr = reinterpret_cast<__m128i *>(state.data());
        _mm_store_si128(sptr++, std::get<0>(s));
        _mm_store_si128(sptr++, std::get<1>(s));
        _mm_store_si128(sptr++, std::get<2>(s));
        _mm_store_si128(sptr++, std::get<3>(s));
    }

    private:
    template <std::size_t>
    static void round(std::array<__m128i, 4> &, __m128i &, const __m128i &,
        const __m128i &, std::false_type)
    {
    }

    template <std::size_t N>
    static void round(std::array<__m128i, 4> &s, __m128i &p, const __m128i &w,
        const __m128i &m, std::true_type)
    {
        MCKL_FLATTEN_CALL kbox<N>(p, w);
        MCKL_FLATTEN_CALL spbox<N>(s, p, m);
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

    template <std::size_t N>
    static void spbox(
        std::array<__m128i, 4> &s, const __m128i &p, const __m128i &m)
    {
        spbox<N>(
            s, p, m, std::integral_constant<bool, (N > 0 && N <= Rounds)>());
    }

    template <std::size_t>
    static void spbox(std::array<__m128i, 4> &, const __m128i &,
        const __m128i &, std::false_type)
    {
    }

    template <std::size_t N>
    static void spbox(std::array<__m128i, 4> &s, const __m128i &p,
        const __m128i &m, std::true_type)
    {
        static constexpr int msk = static_cast<int>(0xFFFFFFFF);

        const __m128i mask = _mm_set_epi32(msk, 0, msk, 0);

        __m128i m0 = _mm_mul_epu32(std::get<0>(s), m);
        __m128i m1 = _mm_mul_epu32(std::get<1>(s), m);
        __m128i m2 = _mm_mul_epu32(std::get<2>(s), m);
        __m128i m3 = _mm_mul_epu32(std::get<3>(s), m);

        m0 = _mm_xor_si128(m0, p);
        m1 = _mm_xor_si128(m1, p);
        m2 = _mm_xor_si128(m2, p);
        m3 = _mm_xor_si128(m3, p);

        std::get<0>(s) = _mm_and_si128(std::get<0>(s), mask);
        std::get<1>(s) = _mm_and_si128(std::get<1>(s), mask);
        std::get<2>(s) = _mm_and_si128(std::get<2>(s), mask);
        std::get<3>(s) = _mm_and_si128(std::get<3>(s), mask);

        std::get<0>(s) = _mm_xor_si128(std::get<0>(s), m0);
        std::get<1>(s) = _mm_xor_si128(std::get<1>(s), m1);
        std::get<2>(s) = _mm_xor_si128(std::get<2>(s), m2);
        std::get<3>(s) = _mm_xor_si128(std::get<3>(s), m3);

        permute<N>(s);
    }

    template <std::size_t N>
    static void permute(std::array<__m128i, 4> &s)
    {
        permute(s, std::integral_constant<bool, (N > 0 && N < Rounds)>());
    }

    static void permute(std::array<__m128i, 4> &, std::false_type) {}

    static void permute(std::array<__m128i, 4> &s, std::true_type)
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

    static void permute_first(std::array<__m128i, 4> &) {}

    static void permute(std::array<__m128i, 4> &s)
    {
        // 2 3 0 1
        std::get<0>(s) = _mm_shuffle_epi32(std::get<0>(s), 0xB1);
        std::get<1>(s) = _mm_shuffle_epi32(std::get<1>(s), 0xB1);
        std::get<2>(s) = _mm_shuffle_epi32(std::get<2>(s), 0xB1);
        std::get<3>(s) = _mm_shuffle_epi32(std::get<3>(s), 0xB1);
    }

    static void permute_last(std::array<__m128i, 4> &s)
    {
        // 2 3 0 1
        std::get<0>(s) = _mm_shuffle_epi32(std::get<0>(s), 0xB1);
        std::get<1>(s) = _mm_shuffle_epi32(std::get<1>(s), 0xB1);
        std::get<2>(s) = _mm_shuffle_epi32(std::get<2>(s), 0xB1);
        std::get<3>(s) = _mm_shuffle_epi32(std::get<3>(s), 0xB1);
    }
}; // class PhiloxGeneratorImpl

template <typename T, std::size_t Rounds, typename Constants>
class PhiloxGeneratorImpl<T, 4, Rounds, Constants, 32>
    : public PhiloxGeneratorImplSSE2_32<T, 4, Rounds, Constants,
          PhiloxGeneratorImpl<T, 4, Rounds, Constants>>
{
    public:
    using PhiloxGeneratorImplSSE2_32<T, 4, Rounds, Constants,
        PhiloxGeneratorImpl<T, 4, Rounds, Constants>>::eval;

    static void eval(std::array<T, 4> &state, const std::array<T, 2> &key)
    {
        static constexpr int w0 = static_cast<int>(Constants::weyl[0]);
        static constexpr int w1 = static_cast<int>(Constants::weyl[1]);

        static constexpr int m0 = static_cast<int>(Constants::multiplier[0]);
        static constexpr int m1 = static_cast<int>(Constants::multiplier[1]);

        const int p0 = static_cast<int>(std::get<0>(key));
        const int p1 = static_cast<int>(std::get<1>(key));

        const __m128i w = _mm_set_epi32(w0, 0, w1, 0);
        const __m128i m = _mm_set_epi32(0, m0, 0, m1);
        __m128i p = _mm_set_epi32(p0, 0, p1, 0);

        std::array<__m128i, 1> s;
        __m128i *const sptr = reinterpret_cast<__m128i *>(state.data());

        std::get<0>(s) = _mm_load_si128(sptr);

        std::get<0>(s) = _mm_shuffle_epi32(std::get<0>(s), 0x6C); // 1 2 3 0

        MCKL_FLATTEN_CALL kbox<0x0>(p, w);
        MCKL_FLATTEN_CALL spbox<0x0>(s, p, m);
        MCKL_FLATTEN_CALL kbox<0x1>(p, w);
        MCKL_FLATTEN_CALL spbox<0x1>(s, p, m);
        MCKL_FLATTEN_CALL kbox<0x2>(p, w);
        MCKL_FLATTEN_CALL spbox<0x2>(s, p, m);
        MCKL_FLATTEN_CALL kbox<0x3>(p, w);
        MCKL_FLATTEN_CALL spbox<0x3>(s, p, m);
        MCKL_FLATTEN_CALL kbox<0x4>(p, w);
        MCKL_FLATTEN_CALL spbox<0x4>(s, p, m);
        MCKL_FLATTEN_CALL kbox<0x5>(p, w);
        MCKL_FLATTEN_CALL spbox<0x5>(s, p, m);
        MCKL_FLATTEN_CALL kbox<0x6>(p, w);
        MCKL_FLATTEN_CALL spbox<0x6>(s, p, m);
        MCKL_FLATTEN_CALL kbox<0x7>(p, w);
        MCKL_FLATTEN_CALL spbox<0x7>(s, p, m);
        MCKL_FLATTEN_CALL kbox<0x8>(p, w);
        MCKL_FLATTEN_CALL spbox<0x8>(s, p, m);
        MCKL_FLATTEN_CALL kbox<0x9>(p, w);
        MCKL_FLATTEN_CALL spbox<0x9>(s, p, m);
        MCKL_FLATTEN_CALL kbox<0xA>(p, w);
        MCKL_FLATTEN_CALL spbox<0xA>(s, p, m);
        MCKL_FLATTEN_CALL kbox<0xB>(p, w);
        MCKL_FLATTEN_CALL spbox<0xB>(s, p, m);
        MCKL_FLATTEN_CALL kbox<0xC>(p, w);
        MCKL_FLATTEN_CALL spbox<0xC>(s, p, m);
        MCKL_FLATTEN_CALL kbox<0xD>(p, w);
        MCKL_FLATTEN_CALL spbox<0xD>(s, p, m);
        MCKL_FLATTEN_CALL kbox<0xE>(p, w);
        MCKL_FLATTEN_CALL spbox<0xE>(s, p, m);
        MCKL_FLATTEN_CALL kbox<0xF>(p, w);
        MCKL_FLATTEN_CALL spbox<0xF>(s, p, m);

        round<0x10>(
            s, p, w, m, std::integral_constant<bool, 0x10 <= Rounds>());

        std::get<0>(s) = _mm_shuffle_epi32(std::get<0>(s), 0x6C); // 1 2 3 0

        _mm_store_si128(sptr, std::get<0>(s));
    }

    private:
    template <std::size_t>
    static void round(std::array<__m128i, 1> &, __m128i &, const __m128i &,
        const __m128i &, std::false_type)
    {
    }

    template <std::size_t N>
    static void round(std::array<__m128i, 1> &s, __m128i &p, const __m128i &w,
        const __m128i &m, std::true_type)
    {
        MCKL_FLATTEN_CALL kbox<N>(p, w);
        MCKL_FLATTEN_CALL spbox<N>(s, p, m);
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

    template <std::size_t N>
    static void spbox(
        std::array<__m128i, 1> &s, const __m128i &p, const __m128i &m)
    {
        spbox<N>(
            s, p, m, std::integral_constant<bool, (N > 0 && N <= Rounds)>());
    }

    template <std::size_t>
    static void spbox(std::array<__m128i, 1> &, const __m128i &,
        const __m128i &, std::false_type)
    {
    }

    template <std::size_t N>
    static void spbox(std::array<__m128i, 1> &s, const __m128i &p,
        const __m128i &m, std::true_type)
    {
        static constexpr int msk = static_cast<int>(0xFFFFFFFF);

        const __m128i mask = _mm_set_epi32(msk, 0, msk, 0);

        const __m128i m0 = _mm_mul_epu32(std::get<0>(s), m);
        const __m128i x0 = _mm_xor_si128(std::get<0>(s), p);

        std::get<0>(s) = _mm_and_si128(x0, mask);
        std::get<0>(s) = _mm_xor_si128(std::get<0>(s), m0);
        std::get<0>(s) = _mm_shuffle_epi32(std::get<0>(s), 0x93); // 2 1 0 3
    }

    friend PhiloxGeneratorImplSSE2_32<T, 4, Rounds, Constants,
        PhiloxGeneratorImpl<T, 4, Rounds, Constants>>;

    static void permute_first(std::array<__m128i, 4> &s)
    {
        // 3 0 1 2
        std::get<0>(s) = _mm_shuffle_epi32(std::get<0>(s), 0xC6);
        std::get<1>(s) = _mm_shuffle_epi32(std::get<1>(s), 0xC6);
        std::get<2>(s) = _mm_shuffle_epi32(std::get<2>(s), 0xC6);
        std::get<3>(s) = _mm_shuffle_epi32(std::get<3>(s), 0xC6);
    }

    static void permute(std::array<__m128i, 4> &s)
    {
        // 2 1 0 3
        std::get<0>(s) = _mm_shuffle_epi32(std::get<0>(s), 0x93);
        std::get<1>(s) = _mm_shuffle_epi32(std::get<1>(s), 0x93);
        std::get<2>(s) = _mm_shuffle_epi32(std::get<2>(s), 0x93);
        std::get<3>(s) = _mm_shuffle_epi32(std::get<3>(s), 0x93);
    }

    static void permute_last(std::array<__m128i, 4> &s)
    {
        // 2 3 0 1
        std::get<0>(s) = _mm_shuffle_epi32(std::get<0>(s), 0xB1);
        std::get<1>(s) = _mm_shuffle_epi32(std::get<1>(s), 0xB1);
        std::get<2>(s) = _mm_shuffle_epi32(std::get<2>(s), 0xB1);
        std::get<3>(s) = _mm_shuffle_epi32(std::get<3>(s), 0xB1);
    }
}; // class PhiloxGeneratorImpl
