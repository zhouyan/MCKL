//============================================================================
// MCKL/include/mckl/random/threefry2x64_avx2.hpp
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

template <typename T, std::size_t Rounds, typename Constants>
class ThreefryGeneratorImpl<T, 2, Rounds, Constants, 64>
{
    static constexpr std::size_t K_ = 2;
    static constexpr std::size_t M_ = 16 / K_;

    public:
    static constexpr bool batch() { return true; }

    static constexpr std::size_t blocks() { return M_ * 4; }

    static void eval(
        std::array<T, K_> &state, const std::array<T, K_ + 1> &par)
    {
        ThreefryGeneratorGenericImpl<T, K_, Rounds, Constants>::eval(
            state, par);
    }

    static void eval(std::array<std::array<T, K_>, blocks()> &state,
        const std::array<T, K_ + 1> &par)
    {
        std::array<std::array<__m256i, K_>, M_> s;

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

    private:
    template <std::size_t>
    static void eval(std::array<std::array<__m256i, K_>, M_> &,
        const std::array<T, K_ + 1> &, std::false_type)
    {
    }

    template <std::size_t N>
    static void eval(std::array<std::array<__m256i, K_>, M_> &s,
        const std::array<T, K_ + 1> &par, std::true_type)
    {
        sbox<N>(s);
        pbox<N>(s);
        kbox<N>(s, par);
        eval<N + 1>(s, par, std::integral_constant<bool, N + 1 <= Rounds>());
    }

    static void pack(std::array<std::array<T, K_>, blocks()> &state,
        std::array<std::array<__m256i, K_>, M_> &s)
    {
        const __m256i *sptr = reinterpret_cast<const __m256i *>(state.data());
        __m256i s0 = _mm256_load_si256(sptr++);
        __m256i s1 = _mm256_load_si256(sptr++);
        __m256i s2 = _mm256_load_si256(sptr++);
        __m256i s3 = _mm256_load_si256(sptr++);
        __m256i s4 = _mm256_load_si256(sptr++);
        __m256i s5 = _mm256_load_si256(sptr++);
        __m256i s6 = _mm256_load_si256(sptr++);
        __m256i s7 = _mm256_load_si256(sptr++);
        __m256i t0 = _mm256_load_si256(sptr++);
        __m256i t1 = _mm256_load_si256(sptr++);
        __m256i t2 = _mm256_load_si256(sptr++);
        __m256i t3 = _mm256_load_si256(sptr++);
        __m256i t4 = _mm256_load_si256(sptr++);
        __m256i t5 = _mm256_load_si256(sptr++);
        __m256i t6 = _mm256_load_si256(sptr++);
        __m256i t7 = _mm256_load_si256(sptr++);

        std::get<0>(std::get<0>(s)) = _mm256_unpacklo_epi64(s0, t0);
        std::get<0>(std::get<1>(s)) = _mm256_unpacklo_epi64(s1, t1);
        std::get<0>(std::get<2>(s)) = _mm256_unpacklo_epi64(s2, t2);
        std::get<0>(std::get<3>(s)) = _mm256_unpacklo_epi64(s3, t3);
        std::get<0>(std::get<4>(s)) = _mm256_unpacklo_epi64(s4, t4);
        std::get<0>(std::get<5>(s)) = _mm256_unpacklo_epi64(s5, t5);
        std::get<0>(std::get<6>(s)) = _mm256_unpacklo_epi64(s6, t6);
        std::get<0>(std::get<7>(s)) = _mm256_unpacklo_epi64(s7, t7);

        std::get<1>(std::get<0>(s)) = _mm256_unpackhi_epi64(s0, t0);
        std::get<1>(std::get<1>(s)) = _mm256_unpackhi_epi64(s1, t1);
        std::get<1>(std::get<2>(s)) = _mm256_unpackhi_epi64(s2, t2);
        std::get<1>(std::get<3>(s)) = _mm256_unpackhi_epi64(s3, t3);
        std::get<1>(std::get<4>(s)) = _mm256_unpackhi_epi64(s4, t4);
        std::get<1>(std::get<5>(s)) = _mm256_unpackhi_epi64(s5, t5);
        std::get<1>(std::get<6>(s)) = _mm256_unpackhi_epi64(s6, t6);
        std::get<1>(std::get<7>(s)) = _mm256_unpackhi_epi64(s7, t7);
    }

    static void unpack(std::array<std::array<T, K_>, blocks()> &state,
        std::array<std::array<__m256i, K_>, M_> &s)
    {
        __m256i s0 = _mm256_unpacklo_epi64(
            std::get<0>(std::get<0>(s)), std::get<1>(std::get<0>(s)));
        __m256i s1 = _mm256_unpacklo_epi64(
            std::get<0>(std::get<1>(s)), std::get<1>(std::get<1>(s)));
        __m256i s2 = _mm256_unpacklo_epi64(
            std::get<0>(std::get<2>(s)), std::get<1>(std::get<2>(s)));
        __m256i s3 = _mm256_unpacklo_epi64(
            std::get<0>(std::get<3>(s)), std::get<1>(std::get<3>(s)));
        __m256i s4 = _mm256_unpacklo_epi64(
            std::get<0>(std::get<4>(s)), std::get<1>(std::get<4>(s)));
        __m256i s5 = _mm256_unpacklo_epi64(
            std::get<0>(std::get<5>(s)), std::get<1>(std::get<5>(s)));
        __m256i s6 = _mm256_unpacklo_epi64(
            std::get<0>(std::get<6>(s)), std::get<1>(std::get<6>(s)));
        __m256i s7 = _mm256_unpacklo_epi64(
            std::get<0>(std::get<7>(s)), std::get<1>(std::get<7>(s)));

        __m256i t0 = _mm256_unpackhi_epi64(
            std::get<0>(std::get<0>(s)), std::get<1>(std::get<0>(s)));
        __m256i t1 = _mm256_unpackhi_epi64(
            std::get<0>(std::get<1>(s)), std::get<1>(std::get<1>(s)));
        __m256i t2 = _mm256_unpackhi_epi64(
            std::get<0>(std::get<2>(s)), std::get<1>(std::get<2>(s)));
        __m256i t3 = _mm256_unpackhi_epi64(
            std::get<0>(std::get<3>(s)), std::get<1>(std::get<3>(s)));
        __m256i t4 = _mm256_unpackhi_epi64(
            std::get<0>(std::get<4>(s)), std::get<1>(std::get<4>(s)));
        __m256i t5 = _mm256_unpackhi_epi64(
            std::get<0>(std::get<5>(s)), std::get<1>(std::get<5>(s)));
        __m256i t6 = _mm256_unpackhi_epi64(
            std::get<0>(std::get<6>(s)), std::get<1>(std::get<6>(s)));
        __m256i t7 = _mm256_unpackhi_epi64(
            std::get<0>(std::get<7>(s)), std::get<1>(std::get<7>(s)));

        __m256i *sptr = reinterpret_cast<__m256i *>(state.data());
        _mm256_store_si256(sptr++, s0);
        _mm256_store_si256(sptr++, s1);
        _mm256_store_si256(sptr++, s2);
        _mm256_store_si256(sptr++, s3);
        _mm256_store_si256(sptr++, s4);
        _mm256_store_si256(sptr++, s5);
        _mm256_store_si256(sptr++, s6);
        _mm256_store_si256(sptr++, s7);
        _mm256_store_si256(sptr++, t0);
        _mm256_store_si256(sptr++, t1);
        _mm256_store_si256(sptr++, t2);
        _mm256_store_si256(sptr++, t3);
        _mm256_store_si256(sptr++, t4);
        _mm256_store_si256(sptr++, t5);
        _mm256_store_si256(sptr++, t6);
        _mm256_store_si256(sptr++, t7);
    }

    template <std::size_t N>
    static void kbox(std::array<std::array<__m256i, K_>, M_> &s,
        const std::array<T, K_ + 1> &par)
    {
        kbox<N>(s, par,
            std::integral_constant<bool, (N % 4 == 0 && N <= Rounds)>());
    }

    template <std::size_t>
    static void kbox(std::array<std::array<__m256i, K_>, M_> &,
        const std::array<T, K_ + 1> &, std::false_type)
    {
    }

    template <std::size_t N>
    static void kbox(std::array<std::array<__m256i, K_>, M_> &s,
        const std::array<T, K_ + 1> &par, std::true_type)
    {
        static constexpr std::size_t S = N / 4;

        const MCKL_INT64 k0 =
            static_cast<MCKL_INT64>(std::get<(S + 0) % (K_ + 1)>(par));
        const MCKL_INT64 k1 =
            static_cast<MCKL_INT64>(std::get<(S + 1) % (K_ + 1)>(par) + S);

        const __m256i p0 = _mm256_set1_epi64x(k0);
        const __m256i p1 = _mm256_set1_epi64x(k1);

        std::get<0>(std::get<0>(s)) =
            _mm256_add_epi64(std::get<0>(std::get<0>(s)), p0);
        std::get<1>(std::get<0>(s)) =
            _mm256_add_epi64(std::get<1>(std::get<0>(s)), p1);
        std::get<0>(std::get<1>(s)) =
            _mm256_add_epi64(std::get<0>(std::get<1>(s)), p0);
        std::get<1>(std::get<1>(s)) =
            _mm256_add_epi64(std::get<1>(std::get<1>(s)), p1);
        std::get<0>(std::get<2>(s)) =
            _mm256_add_epi64(std::get<0>(std::get<2>(s)), p0);
        std::get<1>(std::get<2>(s)) =
            _mm256_add_epi64(std::get<1>(std::get<2>(s)), p1);
        std::get<0>(std::get<3>(s)) =
            _mm256_add_epi64(std::get<0>(std::get<3>(s)), p0);
        std::get<1>(std::get<3>(s)) =
            _mm256_add_epi64(std::get<1>(std::get<3>(s)), p1);
        std::get<0>(std::get<4>(s)) =
            _mm256_add_epi64(std::get<0>(std::get<4>(s)), p0);
        std::get<1>(std::get<4>(s)) =
            _mm256_add_epi64(std::get<1>(std::get<4>(s)), p1);
        std::get<0>(std::get<5>(s)) =
            _mm256_add_epi64(std::get<0>(std::get<5>(s)), p0);
        std::get<1>(std::get<5>(s)) =
            _mm256_add_epi64(std::get<1>(std::get<5>(s)), p1);
        std::get<0>(std::get<6>(s)) =
            _mm256_add_epi64(std::get<0>(std::get<6>(s)), p0);
        std::get<1>(std::get<6>(s)) =
            _mm256_add_epi64(std::get<1>(std::get<6>(s)), p1);
        std::get<0>(std::get<7>(s)) =
            _mm256_add_epi64(std::get<0>(std::get<7>(s)), p0);
        std::get<1>(std::get<7>(s)) =
            _mm256_add_epi64(std::get<1>(std::get<7>(s)), p1);
    }

    template <std::size_t N>
    static void sbox(std::array<std::array<__m256i, K_>, M_> &s)
    {
        sbox<N>(s, std::integral_constant<bool, (N > 0 && N <= Rounds)>());
    }

    template <std::size_t>
    static void sbox(
        std::array<std::array<__m256i, K_>, M_> &, std::false_type)
    {
    }

    template <std::size_t N>
    static void sbox(
        std::array<std::array<__m256i, K_>, M_> &s, std::true_type)
    {
        static constexpr int L0 = Constants::rotate[0][(N - 1) % 8];
        static constexpr int R0 = 64 - L0;

        std::get<0>(std::get<0>(s)) = _mm256_add_epi64(
            std::get<0>(std::get<0>(s)), std::get<1>(std::get<0>(s)));
        std::get<0>(std::get<1>(s)) = _mm256_add_epi64(
            std::get<0>(std::get<1>(s)), std::get<1>(std::get<1>(s)));
        std::get<0>(std::get<2>(s)) = _mm256_add_epi64(
            std::get<0>(std::get<2>(s)), std::get<1>(std::get<2>(s)));
        std::get<0>(std::get<3>(s)) = _mm256_add_epi64(
            std::get<0>(std::get<3>(s)), std::get<1>(std::get<3>(s)));
        std::get<0>(std::get<4>(s)) = _mm256_add_epi64(
            std::get<0>(std::get<4>(s)), std::get<1>(std::get<4>(s)));
        std::get<0>(std::get<5>(s)) = _mm256_add_epi64(
            std::get<0>(std::get<5>(s)), std::get<1>(std::get<5>(s)));
        std::get<0>(std::get<6>(s)) = _mm256_add_epi64(
            std::get<0>(std::get<6>(s)), std::get<1>(std::get<6>(s)));
        std::get<0>(std::get<7>(s)) = _mm256_add_epi64(
            std::get<0>(std::get<7>(s)), std::get<1>(std::get<7>(s)));

        __m256i l0 = _mm256_slli_epi64(std::get<1>(std::get<0>(s)), L0);
        __m256i l1 = _mm256_slli_epi64(std::get<1>(std::get<1>(s)), L0);
        __m256i l2 = _mm256_slli_epi64(std::get<1>(std::get<2>(s)), L0);
        __m256i l3 = _mm256_slli_epi64(std::get<1>(std::get<3>(s)), L0);
        __m256i l4 = _mm256_slli_epi64(std::get<1>(std::get<4>(s)), L0);
        __m256i l5 = _mm256_slli_epi64(std::get<1>(std::get<5>(s)), L0);
        __m256i l6 = _mm256_slli_epi64(std::get<1>(std::get<6>(s)), L0);
        __m256i l7 = _mm256_slli_epi64(std::get<1>(std::get<7>(s)), L0);

        __m256i r0 = _mm256_srli_epi64(std::get<1>(std::get<0>(s)), R0);
        __m256i r1 = _mm256_srli_epi64(std::get<1>(std::get<1>(s)), R0);
        __m256i r2 = _mm256_srli_epi64(std::get<1>(std::get<2>(s)), R0);
        __m256i r3 = _mm256_srli_epi64(std::get<1>(std::get<3>(s)), R0);
        __m256i r4 = _mm256_srli_epi64(std::get<1>(std::get<4>(s)), R0);
        __m256i r5 = _mm256_srli_epi64(std::get<1>(std::get<5>(s)), R0);
        __m256i r6 = _mm256_srli_epi64(std::get<1>(std::get<6>(s)), R0);
        __m256i r7 = _mm256_srli_epi64(std::get<1>(std::get<7>(s)), R0);

        std::get<1>(std::get<0>(s)) = _mm256_or_si256(l0, r0);
        std::get<1>(std::get<1>(s)) = _mm256_or_si256(l1, r1);
        std::get<1>(std::get<2>(s)) = _mm256_or_si256(l2, r2);
        std::get<1>(std::get<3>(s)) = _mm256_or_si256(l3, r3);
        std::get<1>(std::get<4>(s)) = _mm256_or_si256(l4, r4);
        std::get<1>(std::get<5>(s)) = _mm256_or_si256(l5, r5);
        std::get<1>(std::get<6>(s)) = _mm256_or_si256(l6, r6);
        std::get<1>(std::get<7>(s)) = _mm256_or_si256(l7, r7);

        std::get<1>(std::get<0>(s)) = _mm256_xor_si256(
            std::get<0>(std::get<0>(s)), std::get<1>(std::get<0>(s)));
        std::get<1>(std::get<1>(s)) = _mm256_xor_si256(
            std::get<0>(std::get<1>(s)), std::get<1>(std::get<1>(s)));
        std::get<1>(std::get<2>(s)) = _mm256_xor_si256(
            std::get<0>(std::get<2>(s)), std::get<1>(std::get<2>(s)));
        std::get<1>(std::get<3>(s)) = _mm256_xor_si256(
            std::get<0>(std::get<3>(s)), std::get<1>(std::get<3>(s)));
        std::get<1>(std::get<4>(s)) = _mm256_xor_si256(
            std::get<0>(std::get<4>(s)), std::get<1>(std::get<4>(s)));
        std::get<1>(std::get<5>(s)) = _mm256_xor_si256(
            std::get<0>(std::get<5>(s)), std::get<1>(std::get<5>(s)));
        std::get<1>(std::get<6>(s)) = _mm256_xor_si256(
            std::get<0>(std::get<6>(s)), std::get<1>(std::get<6>(s)));
        std::get<1>(std::get<7>(s)) = _mm256_xor_si256(
            std::get<0>(std::get<7>(s)), std::get<1>(std::get<7>(s)));
    }

    template <std::size_t N>
    static void pbox(std::array<std::array<__m256i, K_>, M_> &s)
    {
        pbox<N>(s, std::integral_constant<bool, (N > 0 && N <= Rounds)>());
    }

    template <std::size_t N>
    static void pbox(
        std::array<std::array<__m256i, K_>, M_> &, std::false_type)
    {
    }

    template <std::size_t N>
    static void pbox(
        std::array<std::array<__m256i, K_>, M_> &s, std::true_type)
    {
        ThreefryPBox<__m256i, K_, N, Constants>::eval(std::get<0>(s));
        ThreefryPBox<__m256i, K_, N, Constants>::eval(std::get<1>(s));
        ThreefryPBox<__m256i, K_, N, Constants>::eval(std::get<2>(s));
        ThreefryPBox<__m256i, K_, N, Constants>::eval(std::get<3>(s));
        ThreefryPBox<__m256i, K_, N, Constants>::eval(std::get<4>(s));
        ThreefryPBox<__m256i, K_, N, Constants>::eval(std::get<5>(s));
        ThreefryPBox<__m256i, K_, N, Constants>::eval(std::get<6>(s));
        ThreefryPBox<__m256i, K_, N, Constants>::eval(std::get<7>(s));
    }
}; // class ThreefryGeneratorImpl
