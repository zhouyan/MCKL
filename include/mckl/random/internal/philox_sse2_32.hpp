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

        static constexpr int w0 = static_cast<int>(Constants::weyl[i0]);
        static constexpr int w1 = static_cast<int>(Constants::weyl[i1]);

        static constexpr int m0 = static_cast<int>(Constants::multiplier[i0]);
        static constexpr int m1 = static_cast<int>(Constants::multiplier[i1]);

        const int p0 = static_cast<int>(std::get<i0>(key));
        const int p1 = static_cast<int>(std::get<i1>(key));

        __m128i w = _mm_set_epi32(w1, 0, w0, 0);
        __m128i m = _mm_set_epi32(0, m1, 0, m0);
        __m128i p = _mm_set_epi32(p1, 0, p0, 0);

        std::array<__m128i, 16> s;
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
    static void eval(std::array<__m128i, 16> &, __m128i &, const __m128i &,
        const __m128i &, std::false_type)
    {
    }

    template <std::size_t N>
    static void eval(std::array<__m128i, 16> &s, __m128i &p, const __m128i &w,
        const __m128i &m, std::true_type)
    {
        kbox<N>(p, w);
        spbox<N>(s, p, m);
        eval<N + 1>(
            s, p, w, m, std::integral_constant<bool, N + 1 <= Rounds>());
    }

    static void pack(std::array<std::array<T, K>, blocks()> &state,
        std::array<__m128i, 16> &s)
    {
        const __m128i *sptr = reinterpret_cast<const __m128i *>(state.data());
        std::get<0x0>(s) = _mm_load_si128(sptr++);
        std::get<0x1>(s) = _mm_load_si128(sptr++);
        std::get<0x2>(s) = _mm_load_si128(sptr++);
        std::get<0x3>(s) = _mm_load_si128(sptr++);
        std::get<0x4>(s) = _mm_load_si128(sptr++);
        std::get<0x5>(s) = _mm_load_si128(sptr++);
        std::get<0x6>(s) = _mm_load_si128(sptr++);
        std::get<0x7>(s) = _mm_load_si128(sptr++);
        std::get<0x8>(s) = _mm_load_si128(sptr++);
        std::get<0x9>(s) = _mm_load_si128(sptr++);
        std::get<0xA>(s) = _mm_load_si128(sptr++);
        std::get<0xB>(s) = _mm_load_si128(sptr++);
        std::get<0xC>(s) = _mm_load_si128(sptr++);
        std::get<0xD>(s) = _mm_load_si128(sptr++);
        std::get<0xE>(s) = _mm_load_si128(sptr++);
        std::get<0xF>(s) = _mm_load_si128(sptr++);
    }

    static void unpack(std::array<std::array<T, K>, blocks()> &state,
        std::array<__m128i, 16> &s)
    {
        __m128i *sptr = reinterpret_cast<__m128i *>(state.data());
        _mm_store_si128(sptr++, std::get<0x0>(s));
        _mm_store_si128(sptr++, std::get<0x1>(s));
        _mm_store_si128(sptr++, std::get<0x2>(s));
        _mm_store_si128(sptr++, std::get<0x3>(s));
        _mm_store_si128(sptr++, std::get<0x4>(s));
        _mm_store_si128(sptr++, std::get<0x5>(s));
        _mm_store_si128(sptr++, std::get<0x6>(s));
        _mm_store_si128(sptr++, std::get<0x7>(s));
        _mm_store_si128(sptr++, std::get<0x8>(s));
        _mm_store_si128(sptr++, std::get<0x9>(s));
        _mm_store_si128(sptr++, std::get<0xA>(s));
        _mm_store_si128(sptr++, std::get<0xB>(s));
        _mm_store_si128(sptr++, std::get<0xC>(s));
        _mm_store_si128(sptr++, std::get<0xD>(s));
        _mm_store_si128(sptr++, std::get<0xE>(s));
        _mm_store_si128(sptr++, std::get<0xF>(s));
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
        std::array<__m128i, 16> &s, const __m128i &p, const __m128i &m)
    {
        spbox<N>(
            s, p, m, std::integral_constant<bool, (N > 0 && N <= Rounds)>());
    }

    template <std::size_t>
    static void spbox(std::array<__m128i, 16> &, const __m128i &,
        const __m128i &, std::false_type)
    {
    }

    template <std::size_t N>
    static void spbox(std::array<__m128i, 16> &s, const __m128i &p,
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
        __m128i m8 = _mm_mul_epu32(std::get<0x8>(s), m);
        __m128i m9 = _mm_mul_epu32(std::get<0x9>(s), m);
        __m128i mA = _mm_mul_epu32(std::get<0xA>(s), m);
        __m128i mB = _mm_mul_epu32(std::get<0xB>(s), m);
        __m128i mC = _mm_mul_epu32(std::get<0xC>(s), m);
        __m128i mD = _mm_mul_epu32(std::get<0xD>(s), m);
        __m128i mE = _mm_mul_epu32(std::get<0xE>(s), m);
        __m128i mF = _mm_mul_epu32(std::get<0xF>(s), m);

        m0 = _mm_xor_si128(m0, p);
        m1 = _mm_xor_si128(m1, p);
        m2 = _mm_xor_si128(m2, p);
        m3 = _mm_xor_si128(m3, p);
        m4 = _mm_xor_si128(m4, p);
        m5 = _mm_xor_si128(m5, p);
        m6 = _mm_xor_si128(m6, p);
        m7 = _mm_xor_si128(m7, p);
        m8 = _mm_xor_si128(m8, p);
        m9 = _mm_xor_si128(m9, p);
        mA = _mm_xor_si128(mA, p);
        mB = _mm_xor_si128(mB, p);
        mC = _mm_xor_si128(mC, p);
        mD = _mm_xor_si128(mD, p);
        mE = _mm_xor_si128(mE, p);
        mF = _mm_xor_si128(mF, p);

        std::get<0x0>(s) = _mm_and_si128(std::get<0x0>(s), mask);
        std::get<0x1>(s) = _mm_and_si128(std::get<0x1>(s), mask);
        std::get<0x2>(s) = _mm_and_si128(std::get<0x2>(s), mask);
        std::get<0x3>(s) = _mm_and_si128(std::get<0x3>(s), mask);
        std::get<0x4>(s) = _mm_and_si128(std::get<0x4>(s), mask);
        std::get<0x5>(s) = _mm_and_si128(std::get<0x5>(s), mask);
        std::get<0x6>(s) = _mm_and_si128(std::get<0x6>(s), mask);
        std::get<0x7>(s) = _mm_and_si128(std::get<0x7>(s), mask);
        std::get<0x8>(s) = _mm_and_si128(std::get<0x8>(s), mask);
        std::get<0x9>(s) = _mm_and_si128(std::get<0x9>(s), mask);
        std::get<0xA>(s) = _mm_and_si128(std::get<0xA>(s), mask);
        std::get<0xB>(s) = _mm_and_si128(std::get<0xB>(s), mask);
        std::get<0xC>(s) = _mm_and_si128(std::get<0xC>(s), mask);
        std::get<0xD>(s) = _mm_and_si128(std::get<0xD>(s), mask);
        std::get<0xE>(s) = _mm_and_si128(std::get<0xE>(s), mask);
        std::get<0xF>(s) = _mm_and_si128(std::get<0xF>(s), mask);

        std::get<0x0>(s) = _mm_xor_si128(std::get<0x0>(s), m0);
        std::get<0x1>(s) = _mm_xor_si128(std::get<0x1>(s), m1);
        std::get<0x2>(s) = _mm_xor_si128(std::get<0x2>(s), m2);
        std::get<0x3>(s) = _mm_xor_si128(std::get<0x3>(s), m3);
        std::get<0x4>(s) = _mm_xor_si128(std::get<0x4>(s), m4);
        std::get<0x5>(s) = _mm_xor_si128(std::get<0x5>(s), m5);
        std::get<0x6>(s) = _mm_xor_si128(std::get<0x6>(s), m6);
        std::get<0x7>(s) = _mm_xor_si128(std::get<0x7>(s), m7);
        std::get<0x8>(s) = _mm_xor_si128(std::get<0x8>(s), m8);
        std::get<0x9>(s) = _mm_xor_si128(std::get<0x9>(s), m9);
        std::get<0xA>(s) = _mm_xor_si128(std::get<0xA>(s), mA);
        std::get<0xB>(s) = _mm_xor_si128(std::get<0xB>(s), mB);
        std::get<0xC>(s) = _mm_xor_si128(std::get<0xC>(s), mC);
        std::get<0xD>(s) = _mm_xor_si128(std::get<0xD>(s), mD);
        std::get<0xE>(s) = _mm_xor_si128(std::get<0xE>(s), mE);
        std::get<0xF>(s) = _mm_xor_si128(std::get<0xF>(s), mF);

        permute<N>(s);
    }

    template <std::size_t N>
    static void permute(std::array<__m128i, 16> &s)
    {
        permute(s, std::integral_constant<bool, (N > 0 && N < Rounds)>());
    }

    static void permute(std::array<__m128i, 16> &, std::false_type) {}

    static void permute(std::array<__m128i, 16> &s, std::true_type)
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

    static void permute_first(std::array<__m128i, 16> &) {}

    static void permute(std::array<__m128i, 16> &s)
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
        std::get<0x8>(s) = _mm_shuffle_epi32(std::get<0x8>(s), 0xB1);
        std::get<0x9>(s) = _mm_shuffle_epi32(std::get<0x9>(s), 0xB1);
        std::get<0xA>(s) = _mm_shuffle_epi32(std::get<0xA>(s), 0xB1);
        std::get<0xB>(s) = _mm_shuffle_epi32(std::get<0xB>(s), 0xB1);
        std::get<0xC>(s) = _mm_shuffle_epi32(std::get<0xC>(s), 0xB1);
        std::get<0xD>(s) = _mm_shuffle_epi32(std::get<0xD>(s), 0xB1);
        std::get<0xE>(s) = _mm_shuffle_epi32(std::get<0xE>(s), 0xB1);
        std::get<0xF>(s) = _mm_shuffle_epi32(std::get<0xF>(s), 0xB1);
    }

    static void permute_last(std::array<__m128i, 16> &s)
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
        std::get<0x8>(s) = _mm_shuffle_epi32(std::get<0x8>(s), 0xB1);
        std::get<0x9>(s) = _mm_shuffle_epi32(std::get<0x9>(s), 0xB1);
        std::get<0xA>(s) = _mm_shuffle_epi32(std::get<0xA>(s), 0xB1);
        std::get<0xB>(s) = _mm_shuffle_epi32(std::get<0xB>(s), 0xB1);
        std::get<0xC>(s) = _mm_shuffle_epi32(std::get<0xC>(s), 0xB1);
        std::get<0xD>(s) = _mm_shuffle_epi32(std::get<0xD>(s), 0xB1);
        std::get<0xE>(s) = _mm_shuffle_epi32(std::get<0xE>(s), 0xB1);
        std::get<0xF>(s) = _mm_shuffle_epi32(std::get<0xF>(s), 0xB1);
    }
}; // class PhiloxGeneratorImpl

template <typename T, std::size_t Rounds, typename Constants>
class PhiloxGeneratorImpl<T, 4, Rounds, Constants, 32>
    : public PhiloxGeneratorImplSSE2_32<T, 4, Rounds, Constants,
          PhiloxGeneratorImpl<T, 4, Rounds, Constants>>
{
    friend PhiloxGeneratorImplSSE2_32<T, 4, Rounds, Constants,
        PhiloxGeneratorImpl<T, 4, Rounds, Constants>>;

    static void permute_first(std::array<__m128i, 16> &s)
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
        std::get<0x8>(s) = _mm_shuffle_epi32(std::get<0x8>(s), 0xC6);
        std::get<0x9>(s) = _mm_shuffle_epi32(std::get<0x9>(s), 0xC6);
        std::get<0xA>(s) = _mm_shuffle_epi32(std::get<0xA>(s), 0xC6);
        std::get<0xB>(s) = _mm_shuffle_epi32(std::get<0xB>(s), 0xC6);
        std::get<0xC>(s) = _mm_shuffle_epi32(std::get<0xC>(s), 0xC6);
        std::get<0xD>(s) = _mm_shuffle_epi32(std::get<0xD>(s), 0xC6);
        std::get<0xE>(s) = _mm_shuffle_epi32(std::get<0xE>(s), 0xC6);
        std::get<0xF>(s) = _mm_shuffle_epi32(std::get<0xF>(s), 0xC6);
    }

    static void permute(std::array<__m128i, 16> &s)
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
        std::get<0x8>(s) = _mm_shuffle_epi32(std::get<0x8>(s), 0x93);
        std::get<0x9>(s) = _mm_shuffle_epi32(std::get<0x9>(s), 0x93);
        std::get<0xA>(s) = _mm_shuffle_epi32(std::get<0xA>(s), 0x93);
        std::get<0xB>(s) = _mm_shuffle_epi32(std::get<0xB>(s), 0x93);
        std::get<0xC>(s) = _mm_shuffle_epi32(std::get<0xC>(s), 0x93);
        std::get<0xD>(s) = _mm_shuffle_epi32(std::get<0xD>(s), 0x93);
        std::get<0xE>(s) = _mm_shuffle_epi32(std::get<0xE>(s), 0x93);
        std::get<0xF>(s) = _mm_shuffle_epi32(std::get<0xF>(s), 0x93);
    }

    static void permute_last(std::array<__m128i, 16> &s)
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
        std::get<0x8>(s) = _mm_shuffle_epi32(std::get<0x8>(s), 0xB1);
        std::get<0x9>(s) = _mm_shuffle_epi32(std::get<0x9>(s), 0xB1);
        std::get<0xA>(s) = _mm_shuffle_epi32(std::get<0xA>(s), 0xB1);
        std::get<0xB>(s) = _mm_shuffle_epi32(std::get<0xB>(s), 0xB1);
        std::get<0xC>(s) = _mm_shuffle_epi32(std::get<0xC>(s), 0xB1);
        std::get<0xD>(s) = _mm_shuffle_epi32(std::get<0xD>(s), 0xB1);
        std::get<0xE>(s) = _mm_shuffle_epi32(std::get<0xE>(s), 0xB1);
        std::get<0xF>(s) = _mm_shuffle_epi32(std::get<0xF>(s), 0xB1);
    }
}; // class PhiloxGeneratorImpl
