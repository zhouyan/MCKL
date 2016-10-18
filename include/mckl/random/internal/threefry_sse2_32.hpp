//============================================================================
// MCKL/include/mckl/random/internal/threefry_sse2_32.hpp
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

template <typename T, std::size_t K, std::size_t Rounds, typename Constants>
class ThreefryGeneratorImpl<T, K, Rounds, Constants, 32>
{
    public:
    static constexpr bool batch() { return K != 0 && 8 % K == 0; }

    static constexpr std::size_t blocks() { return 32 / K; }

    static void eval(std::array<T, K> &state, const std::array<T, K + 4> &par)
    {
        ThreefryGeneratorGenericImpl<T, K, Rounds, Constants>::eval(
            state, par);
    }

    static void eval(std::array<std::array<T, K>, blocks()> &state,
        const std::array<T, K + 4> &par)
    {
        std::array<__m128i, 8> s;
        __m128i *sptr = nullptr;

        sptr = reinterpret_cast<__m128i *>(state.data());
        std::get<0>(s) = _mm_load_si128(sptr++);
        std::get<2>(s) = _mm_load_si128(sptr++);
        std::get<4>(s) = _mm_load_si128(sptr++);
        std::get<6>(s) = _mm_load_si128(sptr++);
        std::get<1>(s) = _mm_load_si128(sptr++);
        std::get<3>(s) = _mm_load_si128(sptr++);
        std::get<5>(s) = _mm_load_si128(sptr++);
        std::get<7>(s) = _mm_load_si128(sptr++);

        MCKL_FLATTEN_CALL transpose4x32_si128<0, 1, 2, 3>(s);
        MCKL_FLATTEN_CALL transpose4x32_si128<4, 5, 6, 7>(s);

        MCKL_FLATTEN_CALL sbox<0x00>(s);
        MCKL_FLATTEN_CALL pbox<0x00>(s);
        MCKL_FLATTEN_CALL kbox<0x00>(s, par);
        MCKL_FLATTEN_CALL sbox<0x01>(s);
        MCKL_FLATTEN_CALL pbox<0x01>(s);
        MCKL_FLATTEN_CALL kbox<0x01>(s, par);
        MCKL_FLATTEN_CALL sbox<0x02>(s);
        MCKL_FLATTEN_CALL pbox<0x02>(s);
        MCKL_FLATTEN_CALL kbox<0x02>(s, par);
        MCKL_FLATTEN_CALL sbox<0x03>(s);
        MCKL_FLATTEN_CALL pbox<0x03>(s);
        MCKL_FLATTEN_CALL kbox<0x03>(s, par);
        MCKL_FLATTEN_CALL sbox<0x04>(s);
        MCKL_FLATTEN_CALL pbox<0x04>(s);
        MCKL_FLATTEN_CALL kbox<0x04>(s, par);
        MCKL_FLATTEN_CALL sbox<0x05>(s);
        MCKL_FLATTEN_CALL pbox<0x05>(s);
        MCKL_FLATTEN_CALL kbox<0x05>(s, par);
        MCKL_FLATTEN_CALL sbox<0x06>(s);
        MCKL_FLATTEN_CALL pbox<0x06>(s);
        MCKL_FLATTEN_CALL kbox<0x06>(s, par);
        MCKL_FLATTEN_CALL sbox<0x07>(s);
        MCKL_FLATTEN_CALL pbox<0x07>(s);
        MCKL_FLATTEN_CALL kbox<0x07>(s, par);
        MCKL_FLATTEN_CALL sbox<0x08>(s);
        MCKL_FLATTEN_CALL pbox<0x08>(s);
        MCKL_FLATTEN_CALL kbox<0x08>(s, par);
        MCKL_FLATTEN_CALL sbox<0x09>(s);
        MCKL_FLATTEN_CALL pbox<0x09>(s);
        MCKL_FLATTEN_CALL kbox<0x09>(s, par);
        MCKL_FLATTEN_CALL sbox<0x0A>(s);
        MCKL_FLATTEN_CALL pbox<0x0A>(s);
        MCKL_FLATTEN_CALL kbox<0x0A>(s, par);
        MCKL_FLATTEN_CALL sbox<0x0B>(s);
        MCKL_FLATTEN_CALL pbox<0x0B>(s);
        MCKL_FLATTEN_CALL kbox<0x0B>(s, par);
        MCKL_FLATTEN_CALL sbox<0x0C>(s);
        MCKL_FLATTEN_CALL pbox<0x0C>(s);
        MCKL_FLATTEN_CALL kbox<0x0C>(s, par);
        MCKL_FLATTEN_CALL sbox<0x0D>(s);
        MCKL_FLATTEN_CALL pbox<0x0D>(s);
        MCKL_FLATTEN_CALL kbox<0x0D>(s, par);
        MCKL_FLATTEN_CALL sbox<0x0E>(s);
        MCKL_FLATTEN_CALL pbox<0x0E>(s);
        MCKL_FLATTEN_CALL kbox<0x0E>(s, par);
        MCKL_FLATTEN_CALL sbox<0x0F>(s);
        MCKL_FLATTEN_CALL pbox<0x0F>(s);
        MCKL_FLATTEN_CALL kbox<0x0F>(s, par);
        MCKL_FLATTEN_CALL sbox<0x10>(s);
        MCKL_FLATTEN_CALL pbox<0x10>(s);
        MCKL_FLATTEN_CALL kbox<0x10>(s, par);
        MCKL_FLATTEN_CALL sbox<0x11>(s);
        MCKL_FLATTEN_CALL pbox<0x11>(s);
        MCKL_FLATTEN_CALL kbox<0x11>(s, par);
        MCKL_FLATTEN_CALL sbox<0x12>(s);
        MCKL_FLATTEN_CALL pbox<0x12>(s);
        MCKL_FLATTEN_CALL kbox<0x12>(s, par);
        MCKL_FLATTEN_CALL sbox<0x13>(s);
        MCKL_FLATTEN_CALL pbox<0x13>(s);
        MCKL_FLATTEN_CALL kbox<0x13>(s, par);
        MCKL_FLATTEN_CALL sbox<0x14>(s);
        MCKL_FLATTEN_CALL pbox<0x14>(s);
        MCKL_FLATTEN_CALL kbox<0x14>(s, par);
        MCKL_FLATTEN_CALL sbox<0x15>(s);
        MCKL_FLATTEN_CALL pbox<0x15>(s);
        MCKL_FLATTEN_CALL kbox<0x15>(s, par);
        MCKL_FLATTEN_CALL sbox<0x16>(s);
        MCKL_FLATTEN_CALL pbox<0x16>(s);
        MCKL_FLATTEN_CALL kbox<0x16>(s, par);
        MCKL_FLATTEN_CALL sbox<0x17>(s);
        MCKL_FLATTEN_CALL pbox<0x17>(s);
        MCKL_FLATTEN_CALL kbox<0x17>(s, par);
        MCKL_FLATTEN_CALL sbox<0x18>(s);
        MCKL_FLATTEN_CALL pbox<0x18>(s);
        MCKL_FLATTEN_CALL kbox<0x18>(s, par);
        MCKL_FLATTEN_CALL sbox<0x19>(s);
        MCKL_FLATTEN_CALL pbox<0x19>(s);
        MCKL_FLATTEN_CALL kbox<0x19>(s, par);
        MCKL_FLATTEN_CALL sbox<0x1A>(s);
        MCKL_FLATTEN_CALL pbox<0x1A>(s);
        MCKL_FLATTEN_CALL kbox<0x1A>(s, par);
        MCKL_FLATTEN_CALL sbox<0x1B>(s);
        MCKL_FLATTEN_CALL pbox<0x1B>(s);
        MCKL_FLATTEN_CALL kbox<0x1B>(s, par);
        MCKL_FLATTEN_CALL sbox<0x1C>(s);
        MCKL_FLATTEN_CALL pbox<0x1C>(s);
        MCKL_FLATTEN_CALL kbox<0x1C>(s, par);
        MCKL_FLATTEN_CALL sbox<0x1D>(s);
        MCKL_FLATTEN_CALL pbox<0x1D>(s);
        MCKL_FLATTEN_CALL kbox<0x1D>(s, par);
        MCKL_FLATTEN_CALL sbox<0x1E>(s);
        MCKL_FLATTEN_CALL pbox<0x1E>(s);
        MCKL_FLATTEN_CALL kbox<0x1E>(s, par);
        MCKL_FLATTEN_CALL sbox<0x1F>(s);
        MCKL_FLATTEN_CALL pbox<0x1F>(s);
        MCKL_FLATTEN_CALL kbox<0x1F>(s, par);

        round<0x20>(s, par, std::integral_constant<bool, 0x20 <= Rounds>());

        MCKL_FLATTEN_CALL transpose4x32_si128<0, 1, 2, 3>(s);
        MCKL_FLATTEN_CALL transpose4x32_si128<4, 5, 6, 7>(s);

        sptr = reinterpret_cast<__m128i *>(state.data());
        _mm_store_si128(sptr++, std::get<0>(s));
        _mm_store_si128(sptr++, std::get<2>(s));
        _mm_store_si128(sptr++, std::get<4>(s));
        _mm_store_si128(sptr++, std::get<6>(s));
        _mm_store_si128(sptr++, std::get<1>(s));
        _mm_store_si128(sptr++, std::get<3>(s));
        _mm_store_si128(sptr++, std::get<5>(s));
        _mm_store_si128(sptr++, std::get<7>(s));
    }

    private:
    static constexpr std::size_t M_ = 8 / K;

    template <std::size_t>
    static void round(std::array<__m128i, 8> &, const std::array<T, K + 4> &,
        std::false_type)
    {
    }

    template <std::size_t N>
    static void round(std::array<__m128i, 8> &s,
        const std::array<T, K + 4> &par, std::true_type)
    {
        MCKL_FLATTEN_CALL sbox<N>(s);
        MCKL_FLATTEN_CALL pbox<N>(s);
        MCKL_FLATTEN_CALL kbox<N>(s, par);
        round<N + 1>(s, par, std::integral_constant<bool, N + 1 <= Rounds>());
    }

    template <std::size_t N>
    static void kbox(
        std::array<__m128i, 8> &s, const std::array<T, K + 4> &par)
    {
        kbox<N>(s, par,
            std::integral_constant<bool, (N % 4 == 0 && N <= Rounds)>());
    }

    template <std::size_t>
    static void kbox(std::array<__m128i, 8> &, const std::array<T, K + 4> &,
        std::false_type)
    {
    }

    template <std::size_t N>
    static void kbox(std::array<__m128i, 8> &s,
        const std::array<T, K + 4> &par, std::true_type)
    {
        std::array<__m128i, K> k;
        set_key<N, 0>(k, par, std::integral_constant<bool, 0 < K>());

        std::get<0>(s) = _mm_add_epi32(std::get<0>(s), std::get<0 % K>(k));
        std::get<1>(s) = _mm_add_epi32(std::get<1>(s), std::get<1 % K>(k));
        std::get<2>(s) = _mm_add_epi32(std::get<2>(s), std::get<2 % K>(k));
        std::get<3>(s) = _mm_add_epi32(std::get<3>(s), std::get<3 % K>(k));
        std::get<4>(s) = _mm_add_epi32(std::get<4>(s), std::get<4 % K>(k));
        std::get<5>(s) = _mm_add_epi32(std::get<5>(s), std::get<5 % K>(k));
        std::get<6>(s) = _mm_add_epi32(std::get<6>(s), std::get<6 % K>(k));
        std::get<7>(s) = _mm_add_epi32(std::get<7>(s), std::get<7 % K>(k));
    }

    template <std::size_t N>
    static void sbox(std::array<__m128i, 8> &s)
    {
        sbox<N>(s, std::integral_constant<bool, (N > 0 && N <= Rounds)>());
    }

    template <std::size_t>
    static void sbox(std::array<__m128i, 8> &, std::false_type)
    {
    }

    template <std::size_t N>
    static void sbox(std::array<__m128i, 8> &s, std::true_type)
    {
        constexpr int L0 = Constants::rotate::value[0 % (K / 2)][(N - 1) % 8];
        constexpr int L1 = Constants::rotate::value[1 % (K / 2)][(N - 1) % 8];
        constexpr int L2 = Constants::rotate::value[2 % (K / 2)][(N - 1) % 8];
        constexpr int L3 = Constants::rotate::value[3 % (K / 2)][(N - 1) % 8];
        constexpr int R0 = 32 - L0;
        constexpr int R1 = 32 - L1;
        constexpr int R2 = 32 - L2;
        constexpr int R3 = 32 - L3;

        std::get<0>(s) = _mm_add_epi32(std::get<0>(s), std::get<1>(s));
        std::get<2>(s) = _mm_add_epi32(std::get<2>(s), std::get<3>(s));
        std::get<4>(s) = _mm_add_epi32(std::get<4>(s), std::get<5>(s));
        std::get<6>(s) = _mm_add_epi32(std::get<6>(s), std::get<7>(s));

        __m128i l0 = _mm_slli_epi32(std::get<1>(s), L0);
        __m128i l1 = _mm_slli_epi32(std::get<3>(s), L1);
        __m128i l2 = _mm_slli_epi32(std::get<5>(s), L2);
        __m128i l3 = _mm_slli_epi32(std::get<7>(s), L3);

        __m128i r0 = _mm_srli_epi32(std::get<1>(s), R0);
        __m128i r1 = _mm_srli_epi32(std::get<3>(s), R1);
        __m128i r2 = _mm_srli_epi32(std::get<5>(s), R2);
        __m128i r3 = _mm_srli_epi32(std::get<7>(s), R3);

        std::get<1>(s) = _mm_or_si128(l0, r0);
        std::get<3>(s) = _mm_or_si128(l1, r1);
        std::get<5>(s) = _mm_or_si128(l2, r2);
        std::get<7>(s) = _mm_or_si128(l3, r3);

        std::get<1>(s) = _mm_xor_si128(std::get<0>(s), std::get<1>(s));
        std::get<3>(s) = _mm_xor_si128(std::get<2>(s), std::get<3>(s));
        std::get<5>(s) = _mm_xor_si128(std::get<4>(s), std::get<5>(s));
        std::get<7>(s) = _mm_xor_si128(std::get<6>(s), std::get<7>(s));
    }

    template <std::size_t N>
    static void pbox(std::array<__m128i, 8> &s)
    {
        pbox<N>(s, std::integral_constant<bool, (N > 0 && N <= Rounds)>());
    }

    template <std::size_t>
    static void pbox(std::array<__m128i, 8> &, std::false_type)
    {
    }

    template <std::size_t N>
    static void pbox(std::array<__m128i, 8> &s, std::true_type)
    {
        permute<N, 0>(s, std::integral_constant<bool, 0 < M_>());
    }

    template <std::size_t, std::size_t>
    static void set_key(std::array<__m128i, K> &, const std::array<T, K + 4> &,
        std::false_type)
    {
    }

    template <std::size_t N, std::size_t I>
    static void set_key(std::array<__m128i, K> &k,
        const std::array<T, K + 4> &par, std::true_type)
    {
        std::get<I>(k) = _mm_set1_epi32(static_cast<int>(
            ThreefryKBox<T, K, N, Constants>::template key<I>(par)));
        set_key<N, I + 1>(k, par, std::integral_constant<bool, I + 1 < K>());
    }

    template <std::size_t, std::size_t>
    static void permute(std::array<__m128i, 8> &, std::false_type)
    {
    }

    template <std::size_t N, std::size_t I>
    static void permute(std::array<__m128i, 8> &s, std::true_type)
    {
        ThreefryPBox<__m128i, K, N, Constants>::eval(s.data() + I * K);
        permute<N, I + 1>(s, std::integral_constant<bool, I + 1 < M_>());
    }
}; // class ThreefryGeneratorImpl
