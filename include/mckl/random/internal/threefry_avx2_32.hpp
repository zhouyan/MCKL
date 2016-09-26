//============================================================================
// MCKL/include/mckl/random/threefry_avx2_32.hpp
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
    static constexpr bool batch()
    {
        return K != 0 && 16 % K == 0 ?
            true :
            ThreefryGeneratorGenericImpl<T, K, Rounds, Constants>::batch();
    }

    static constexpr std::size_t blocks()
    {
        return K != 0 && 16 % K == 0 ?
            128 / K :
            ThreefryGeneratorGenericImpl<T, K, Rounds, Constants>::blocks();
    }

    static void eval(std::array<T, K> &state, const std::array<T, K + 1> &par)
    {
        ThreefryGeneratorGenericImpl<T, K, Rounds, Constants>::eval(
            state, par);
    }

    static void eval(std::array<std::array<T, K>, blocks()> &state,
        const std::array<T, K + 1> &par)
    {
        eval(state, par,
            std::integral_constant<bool, (K != 0 && 16 % K == 0)>());
    }

    private:
    static constexpr std::size_t M_ = 16 / K;

    static void eval(std::array<std::array<T, K>, blocks()> &state,
        const std::array<T, K + 1> &par, std::false_type)
    {
        ThreefryGeneratorGenericImpl<T, K, Rounds, Constants>::eval(
            state, par);
    }

    static void eval(std::array<std::array<T, K>, blocks()> &state,
        const std::array<T, K + 1> &par, std::true_type)
    {
        std::array<__m256i, 16> s;
        pack(state, s);

        // clang-format off
        sbox<0x00>(s); pbox<0x00>(s); kbox<0x00>(s, par);
        sbox<0x01>(s); pbox<0x01>(s); kbox<0x01>(s, par);
        sbox<0x02>(s); pbox<0x02>(s); kbox<0x02>(s, par);
        sbox<0x03>(s); pbox<0x03>(s); kbox<0x03>(s, par);
        sbox<0x04>(s); pbox<0x04>(s); kbox<0x04>(s, par);
        sbox<0x05>(s); pbox<0x05>(s); kbox<0x05>(s, par);
        sbox<0x06>(s); pbox<0x06>(s); kbox<0x06>(s, par);
        sbox<0x07>(s); pbox<0x07>(s); kbox<0x07>(s, par);
        sbox<0x08>(s); pbox<0x08>(s); kbox<0x08>(s, par);
        sbox<0x09>(s); pbox<0x09>(s); kbox<0x09>(s, par);
        sbox<0x0A>(s); pbox<0x0A>(s); kbox<0x0A>(s, par);
        sbox<0x0B>(s); pbox<0x0B>(s); kbox<0x0B>(s, par);
        sbox<0x0C>(s); pbox<0x0C>(s); kbox<0x0C>(s, par);
        sbox<0x0D>(s); pbox<0x0D>(s); kbox<0x0D>(s, par);
        sbox<0x0E>(s); pbox<0x0E>(s); kbox<0x0E>(s, par);
        sbox<0x0F>(s); pbox<0x0F>(s); kbox<0x0F>(s, par);
        sbox<0x10>(s); pbox<0x10>(s); kbox<0x10>(s, par);
        sbox<0x11>(s); pbox<0x11>(s); kbox<0x11>(s, par);
        sbox<0x12>(s); pbox<0x12>(s); kbox<0x12>(s, par);
        sbox<0x13>(s); pbox<0x13>(s); kbox<0x13>(s, par);
        sbox<0x14>(s); pbox<0x14>(s); kbox<0x14>(s, par);
        sbox<0x15>(s); pbox<0x15>(s); kbox<0x15>(s, par);
        sbox<0x16>(s); pbox<0x16>(s); kbox<0x16>(s, par);
        sbox<0x17>(s); pbox<0x17>(s); kbox<0x17>(s, par);
        sbox<0x18>(s); pbox<0x18>(s); kbox<0x18>(s, par);
        sbox<0x19>(s); pbox<0x19>(s); kbox<0x19>(s, par);
        sbox<0x1A>(s); pbox<0x1A>(s); kbox<0x1A>(s, par);
        sbox<0x1B>(s); pbox<0x1B>(s); kbox<0x1B>(s, par);
        sbox<0x1C>(s); pbox<0x1C>(s); kbox<0x1C>(s, par);
        sbox<0x1D>(s); pbox<0x1D>(s); kbox<0x1D>(s, par);
        sbox<0x1E>(s); pbox<0x1E>(s); kbox<0x1E>(s, par);
        sbox<0x1F>(s); pbox<0x1F>(s); kbox<0x1F>(s, par);
        // clang-format on

        eval<0x20>(s, par, std::integral_constant<bool, 0x20 <= Rounds>());
        unpack(state, s);
    }

    template <std::size_t>
    static void eval(std::array<__m256i, 16> &, const std::array<T, K + 1> &,
        std::false_type)
    {
    }

    template <std::size_t N>
    static void eval(std::array<__m256i, 16> &s,
        const std::array<T, K + 1> &par, std::true_type)
    {
        sbox<N>(s);
        pbox<N>(s);
        kbox<N>(s, par);
        eval<N + 1>(s, par, std::integral_constant<bool, N + 1 <= Rounds>());
    }

    static void pack(std::array<std::array<T, K>, blocks()> &state,
        std::array<__m256i, 16> &s)
    {
        const __m256i *sptr = reinterpret_cast<const __m256i *>(state.data());
        std::get<0x0>(s) = _mm256_load_si256(sptr++);
        std::get<0x8>(s) = _mm256_load_si256(sptr++);
        std::get<0x1>(s) = _mm256_load_si256(sptr++);
        std::get<0x9>(s) = _mm256_load_si256(sptr++);
        std::get<0x2>(s) = _mm256_load_si256(sptr++);
        std::get<0xA>(s) = _mm256_load_si256(sptr++);
        std::get<0x3>(s) = _mm256_load_si256(sptr++);
        std::get<0xB>(s) = _mm256_load_si256(sptr++);
        std::get<0x4>(s) = _mm256_load_si256(sptr++);
        std::get<0xC>(s) = _mm256_load_si256(sptr++);
        std::get<0x5>(s) = _mm256_load_si256(sptr++);
        std::get<0xD>(s) = _mm256_load_si256(sptr++);
        std::get<0x6>(s) = _mm256_load_si256(sptr++);
        std::get<0xE>(s) = _mm256_load_si256(sptr++);
        std::get<0x7>(s) = _mm256_load_si256(sptr++);
        std::get<0xF>(s) = _mm256_load_si256(sptr++);

        transpose8x32_si256<0x0, 0x1, 0x2, 0x3, 0x4, 0x5, 0x6, 0x7>(s);
        transpose8x32_si256<0x8, 0x9, 0xA, 0xB, 0xC, 0xD, 0xE, 0xF>(s);
    }

    static void unpack(std::array<std::array<T, K>, blocks()> &state,
        std::array<__m256i, 16> &s)
    {
        transpose8x32_si256<0x0, 0x1, 0x2, 0x3, 0x4, 0x5, 0x6, 0x7>(s);
        transpose8x32_si256<0x8, 0x9, 0xA, 0xB, 0xC, 0xD, 0xE, 0xF>(s);

        __m256i *sptr = reinterpret_cast<__m256i *>(state.data());
        _mm256_store_si256(sptr++, std::get<0x0>(s));
        _mm256_store_si256(sptr++, std::get<0x8>(s));
        _mm256_store_si256(sptr++, std::get<0x1>(s));
        _mm256_store_si256(sptr++, std::get<0x9>(s));
        _mm256_store_si256(sptr++, std::get<0x2>(s));
        _mm256_store_si256(sptr++, std::get<0xA>(s));
        _mm256_store_si256(sptr++, std::get<0x3>(s));
        _mm256_store_si256(sptr++, std::get<0xB>(s));
        _mm256_store_si256(sptr++, std::get<0x4>(s));
        _mm256_store_si256(sptr++, std::get<0xC>(s));
        _mm256_store_si256(sptr++, std::get<0x5>(s));
        _mm256_store_si256(sptr++, std::get<0xD>(s));
        _mm256_store_si256(sptr++, std::get<0x6>(s));
        _mm256_store_si256(sptr++, std::get<0xE>(s));
        _mm256_store_si256(sptr++, std::get<0x7>(s));
        _mm256_store_si256(sptr++, std::get<0xF>(s));
    }

    template <std::size_t N>
    static void kbox(
        std::array<__m256i, 16> &s, const std::array<T, K + 1> &par)
    {
        kbox<N>(s, par,
            std::integral_constant<bool, (N % 4 == 0 && N <= Rounds)>());
    }

    template <std::size_t>
    static void kbox(std::array<__m256i, 16> &, const std::array<T, K + 1> &,
        std::false_type)
    {
    }

    template <std::size_t N>
    static void kbox(std::array<__m256i, 16> &s,
        const std::array<T, K + 1> &par, std::true_type)
    {
        std::array<__m256i, K> k;
        set_key<N, 0>(k, par, std::integral_constant<bool, 0 < K>());

        std::get<0x0>(s) =
            _mm256_add_epi32(std::get<0x0>(s), std::get<0x0 % K>(k));
        std::get<0x1>(s) =
            _mm256_add_epi32(std::get<0x1>(s), std::get<0x1 % K>(k));
        std::get<0x2>(s) =
            _mm256_add_epi32(std::get<0x2>(s), std::get<0x2 % K>(k));
        std::get<0x3>(s) =
            _mm256_add_epi32(std::get<0x3>(s), std::get<0x3 % K>(k));
        std::get<0x4>(s) =
            _mm256_add_epi32(std::get<0x4>(s), std::get<0x4 % K>(k));
        std::get<0x5>(s) =
            _mm256_add_epi32(std::get<0x5>(s), std::get<0x5 % K>(k));
        std::get<0x6>(s) =
            _mm256_add_epi32(std::get<0x6>(s), std::get<0x6 % K>(k));
        std::get<0x7>(s) =
            _mm256_add_epi32(std::get<0x7>(s), std::get<0x7 % K>(k));
        std::get<0x8>(s) =
            _mm256_add_epi32(std::get<0x8>(s), std::get<0x8 % K>(k));
        std::get<0x9>(s) =
            _mm256_add_epi32(std::get<0x9>(s), std::get<0x9 % K>(k));
        std::get<0xA>(s) =
            _mm256_add_epi32(std::get<0xA>(s), std::get<0xA % K>(k));
        std::get<0xB>(s) =
            _mm256_add_epi32(std::get<0xB>(s), std::get<0xB % K>(k));
        std::get<0xC>(s) =
            _mm256_add_epi32(std::get<0xC>(s), std::get<0xC % K>(k));
        std::get<0xD>(s) =
            _mm256_add_epi32(std::get<0xD>(s), std::get<0xD % K>(k));
        std::get<0xE>(s) =
            _mm256_add_epi32(std::get<0xE>(s), std::get<0xE % K>(k));
        std::get<0xF>(s) =
            _mm256_add_epi32(std::get<0xF>(s), std::get<0xF % K>(k));
    }

    template <std::size_t N>
    static void sbox(std::array<__m256i, 16> &s)
    {
        sbox<N>(s, std::integral_constant<bool, (N > 0 && N <= Rounds)>());
    }

    template <std::size_t>
    static void sbox(std::array<__m256i, 16> &, std::false_type)
    {
    }

    template <std::size_t N>
    static void sbox(std::array<__m256i, 16> &s, std::true_type)
    {
        static constexpr int L0 = Constants::rotate[0 % (K / 2)][(N - 1) % 8];
        static constexpr int L1 = Constants::rotate[1 % (K / 2)][(N - 1) % 8];
        static constexpr int L2 = Constants::rotate[2 % (K / 2)][(N - 1) % 8];
        static constexpr int L3 = Constants::rotate[3 % (K / 2)][(N - 1) % 8];
        static constexpr int L4 = Constants::rotate[4 % (K / 2)][(N - 1) % 8];
        static constexpr int L5 = Constants::rotate[5 % (K / 2)][(N - 1) % 8];
        static constexpr int L6 = Constants::rotate[6 % (K / 2)][(N - 1) % 8];
        static constexpr int L7 = Constants::rotate[7 % (K / 2)][(N - 1) % 8];
        static constexpr int R0 = 32 - L0;
        static constexpr int R1 = 32 - L1;
        static constexpr int R2 = 32 - L2;
        static constexpr int R3 = 32 - L3;
        static constexpr int R4 = 32 - L4;
        static constexpr int R5 = 32 - L5;
        static constexpr int R6 = 32 - L6;
        static constexpr int R7 = 32 - L7;

        std::get<0x0>(s) =
            _mm256_add_epi32(std::get<0x0>(s), std::get<0x1>(s));
        std::get<0x2>(s) =
            _mm256_add_epi32(std::get<0x2>(s), std::get<0x3>(s));
        std::get<0x4>(s) =
            _mm256_add_epi32(std::get<0x4>(s), std::get<0x5>(s));
        std::get<0x6>(s) =
            _mm256_add_epi32(std::get<0x6>(s), std::get<0x7>(s));
        std::get<0x8>(s) =
            _mm256_add_epi32(std::get<0x8>(s), std::get<0x9>(s));
        std::get<0xA>(s) =
            _mm256_add_epi32(std::get<0xA>(s), std::get<0xB>(s));
        std::get<0xC>(s) =
            _mm256_add_epi32(std::get<0xC>(s), std::get<0xD>(s));
        std::get<0xE>(s) =
            _mm256_add_epi32(std::get<0xE>(s), std::get<0xF>(s));

        __m256i l0 = _mm256_slli_epi32(std::get<0x1>(s), L0);
        __m256i l1 = _mm256_slli_epi32(std::get<0x3>(s), L1);
        __m256i l2 = _mm256_slli_epi32(std::get<0x5>(s), L2);
        __m256i l3 = _mm256_slli_epi32(std::get<0x7>(s), L3);
        __m256i l4 = _mm256_slli_epi32(std::get<0x9>(s), L4);
        __m256i l5 = _mm256_slli_epi32(std::get<0xB>(s), L5);
        __m256i l6 = _mm256_slli_epi32(std::get<0xD>(s), L6);
        __m256i l7 = _mm256_slli_epi32(std::get<0xF>(s), L7);

        __m256i r0 = _mm256_srli_epi32(std::get<0x1>(s), R0);
        __m256i r1 = _mm256_srli_epi32(std::get<0x3>(s), R1);
        __m256i r2 = _mm256_srli_epi32(std::get<0x5>(s), R2);
        __m256i r3 = _mm256_srli_epi32(std::get<0x7>(s), R3);
        __m256i r4 = _mm256_srli_epi32(std::get<0x9>(s), R4);
        __m256i r5 = _mm256_srli_epi32(std::get<0xB>(s), R5);
        __m256i r6 = _mm256_srli_epi32(std::get<0xD>(s), R6);
        __m256i r7 = _mm256_srli_epi32(std::get<0xF>(s), R7);

        std::get<0x1>(s) = _mm256_or_si256(l0, r0);
        std::get<0x3>(s) = _mm256_or_si256(l1, r1);
        std::get<0x5>(s) = _mm256_or_si256(l2, r2);
        std::get<0x7>(s) = _mm256_or_si256(l3, r3);
        std::get<0x9>(s) = _mm256_or_si256(l4, r4);
        std::get<0xB>(s) = _mm256_or_si256(l5, r5);
        std::get<0xD>(s) = _mm256_or_si256(l6, r6);
        std::get<0xF>(s) = _mm256_or_si256(l7, r7);

        std::get<0x1>(s) =
            _mm256_xor_si256(std::get<0x0>(s), std::get<0x1>(s));
        std::get<0x3>(s) =
            _mm256_xor_si256(std::get<0x2>(s), std::get<0x3>(s));
        std::get<0x5>(s) =
            _mm256_xor_si256(std::get<0x4>(s), std::get<0x5>(s));
        std::get<0x7>(s) =
            _mm256_xor_si256(std::get<0x6>(s), std::get<0x7>(s));
        std::get<0x9>(s) =
            _mm256_xor_si256(std::get<0x8>(s), std::get<0x9>(s));
        std::get<0xB>(s) =
            _mm256_xor_si256(std::get<0xA>(s), std::get<0xB>(s));
        std::get<0xD>(s) =
            _mm256_xor_si256(std::get<0xC>(s), std::get<0xD>(s));
        std::get<0xF>(s) =
            _mm256_xor_si256(std::get<0xE>(s), std::get<0xF>(s));
    }

    template <std::size_t N>
    static void pbox(std::array<__m256i, 16> &s)
    {
        pbox<N>(s, std::integral_constant<bool, (N > 0 && N <= Rounds)>());
    }

    template <std::size_t>
    static void pbox(std::array<__m256i, 16> &, std::false_type)
    {
    }

    template <std::size_t N>
    static void pbox(std::array<__m256i, 16> &s, std::true_type)
    {
        permute<N, 0>(s, std::integral_constant<bool, 0 < M_>());
    }

    template <std::size_t, std::size_t>
    static void set_key(std::array<__m256i, K> &, const std::array<T, K + 1> &,
        std::false_type)
    {
    }

    template <std::size_t N, std::size_t I>
    static void set_key(std::array<__m256i, K> &k,
        const std::array<T, K + 1> &par, std::true_type)
    {
        static constexpr std::size_t S = N / 4;
        static constexpr std::size_t J = (S + I) % (K + 1);
        static constexpr T p = I == K - 1 ? S : 0;

        std::get<I>(k) =
            _mm256_set1_epi32(static_cast<int>(std::get<J>(par) + p));
        set_key<N, I + 1>(k, par, std::integral_constant<bool, I + 1 < K>());
    }

    template <std::size_t, std::size_t>
    static void permute(std::array<__m256i, 16> &, std::false_type)
    {
    }

    template <std::size_t N, std::size_t I>
    static void permute(std::array<__m256i, 16> &s, std::true_type)
    {
        ThreefryPBox<__m256i, K, N, Constants>::eval(s.data() + I * K);
        permute<N, I + 1>(s, std::integral_constant<bool, I + 1 < M_>());
    }
}; // class ThreefryGeneratorImpl
