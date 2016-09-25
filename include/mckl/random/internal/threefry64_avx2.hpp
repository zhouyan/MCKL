//============================================================================
// MCKL/include/mckl/random/threefry64_avx2.hpp
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

// Packing scheme:
// s: 6 2 4 0
// t: 7 3 5 1
template <typename T, std::size_t K, std::size_t Rounds, typename Constants,
    typename Derived>
class ThreefryGeneratorAVX2Impl<T, K, Rounds, Constants, Derived, 64>
{
    public:
    static constexpr bool batch()
    {
        return K != 0 && 64 % K == 0 ?
            true :
            ThreefryGeneratorGenericImpl<T, K, Rounds, Constants>::batch();
    }

    static constexpr std::size_t blocks()
    {
        return K != 0 && 64 % K == 0 ?
            64 / K :
            ThreefryGeneratorGenericImpl<T, K, Rounds, Constants>::blocks();
    }

    static void eval(std::array<T, K> &state, const std::array<T, K + 1> &par)
    {
        eval(state, par,
            std::integral_constant<bool, (K != 0 && K % 8 == 0)>());
    }

    static void eval(std::array<std::array<T, K>, blocks()> &state,
        const std::array<T, K + 1> &par)
    {
        eval(state, par,
            std::integral_constant<bool, (K != 0 && 64 % K == 0)>());
    }

    private:
    static constexpr std::size_t M_ = K / 8 + (K % 8 == 0 ? 0 : 1);

    static void eval(std::array<T, K> &state, const std::array<T, K + 1> &par,
        std::false_type)
    {
        ThreefryGeneratorGenericImpl<T, K, Rounds, Constants>::eval(
            state, par);
    }

    static void eval(std::array<T, K> &state, const std::array<T, K + 1> &par,
        std::true_type)
    {
        ThreefryGeneratorGenericImpl<T, K, Rounds, Constants>::eval(
            state, par);
    }

    static void eval(std::array<std::array<T, K>, blocks()> &state,
        const std::array<T, K + 1> &par, std::false_type)
    {
        ThreefryGeneratorGenericImpl<T, K, Rounds, Constants>::eval(
            state, par);
    }

    static void eval(std::array<std::array<T, K>, blocks()> &state,
        const std::array<T, K + 1> &par, std::true_type)
    {
        std::array<__m256i, 8> s;
        std::array<__m256i, 8> t;

        pack(state, s, t);

        // clang-format off
        sbox<0x00>(s, t); pbox<0x00>(s, t); kbox<0x00>(s, t, par);
        sbox<0x01>(s, t); pbox<0x01>(s, t); kbox<0x01>(s, t, par);
        sbox<0x02>(s, t); pbox<0x02>(s, t); kbox<0x02>(s, t, par);
        sbox<0x03>(s, t); pbox<0x03>(s, t); kbox<0x03>(s, t, par);
        sbox<0x04>(s, t); pbox<0x04>(s, t); kbox<0x04>(s, t, par);
        sbox<0x05>(s, t); pbox<0x05>(s, t); kbox<0x05>(s, t, par);
        sbox<0x06>(s, t); pbox<0x06>(s, t); kbox<0x06>(s, t, par);
        sbox<0x07>(s, t); pbox<0x07>(s, t); kbox<0x07>(s, t, par);
        sbox<0x08>(s, t); pbox<0x08>(s, t); kbox<0x08>(s, t, par);
        sbox<0x09>(s, t); pbox<0x09>(s, t); kbox<0x09>(s, t, par);
        sbox<0x0A>(s, t); pbox<0x0A>(s, t); kbox<0x0A>(s, t, par);
        sbox<0x0B>(s, t); pbox<0x0B>(s, t); kbox<0x0B>(s, t, par);
        sbox<0x0C>(s, t); pbox<0x0C>(s, t); kbox<0x0C>(s, t, par);
        sbox<0x0D>(s, t); pbox<0x0D>(s, t); kbox<0x0D>(s, t, par);
        sbox<0x0E>(s, t); pbox<0x0E>(s, t); kbox<0x0E>(s, t, par);
        sbox<0x0F>(s, t); pbox<0x0F>(s, t); kbox<0x0F>(s, t, par);
        sbox<0x10>(s, t); pbox<0x10>(s, t); kbox<0x10>(s, t, par);
        sbox<0x11>(s, t); pbox<0x11>(s, t); kbox<0x11>(s, t, par);
        sbox<0x12>(s, t); pbox<0x12>(s, t); kbox<0x12>(s, t, par);
        sbox<0x13>(s, t); pbox<0x13>(s, t); kbox<0x13>(s, t, par);
        sbox<0x14>(s, t); pbox<0x14>(s, t); kbox<0x14>(s, t, par);
        sbox<0x15>(s, t); pbox<0x15>(s, t); kbox<0x15>(s, t, par);
        sbox<0x16>(s, t); pbox<0x16>(s, t); kbox<0x16>(s, t, par);
        sbox<0x17>(s, t); pbox<0x17>(s, t); kbox<0x17>(s, t, par);
        sbox<0x18>(s, t); pbox<0x18>(s, t); kbox<0x18>(s, t, par);
        sbox<0x19>(s, t); pbox<0x19>(s, t); kbox<0x19>(s, t, par);
        sbox<0x1A>(s, t); pbox<0x1A>(s, t); kbox<0x1A>(s, t, par);
        sbox<0x1B>(s, t); pbox<0x1B>(s, t); kbox<0x1B>(s, t, par);
        sbox<0x1C>(s, t); pbox<0x1C>(s, t); kbox<0x1C>(s, t, par);
        sbox<0x1D>(s, t); pbox<0x1D>(s, t); kbox<0x1D>(s, t, par);
        sbox<0x1E>(s, t); pbox<0x1E>(s, t); kbox<0x1E>(s, t, par);
        sbox<0x1F>(s, t); pbox<0x1F>(s, t); kbox<0x1F>(s, t, par);
        // clang-format on

        eval<0x20>(s, t, par, std::integral_constant<bool, 0x20 <= Rounds>());

        unpack(state, s, t);
    }

    template <std::size_t>
    static void eval(std::array<__m256i, 8> &, std::array<__m256i, 8> &,
        const std::array<T, K + 1> &, std::false_type)
    {
    }

    template <std::size_t N>
    static void eval(std::array<__m256i, 8> &s, std::array<__m256i, 8> &t,
        const std::array<T, K + 1> &par, std::true_type)
    {
        sbox<N>(s, t);
        pbox<N>(s, t);
        kbox<N>(s, t, par);
        eval<N + 1>(
            s, t, par, std::integral_constant<bool, N + 1 <= Rounds>());
    }

    static void pack(std::array<std::array<T, K>, blocks()> &state,
        std::array<__m256i, 8> &s, std::array<__m256i, 8> &t)
    {
        std::array<__m256i, 16> p;

        const __m256i *sptr = reinterpret_cast<const __m256i *>(state.data());
        std::get<0x0>(p) = _mm256_load_si256(sptr++);
        std::get<0x1>(p) = _mm256_load_si256(sptr++);
        std::get<0x2>(p) = _mm256_load_si256(sptr++);
        std::get<0x3>(p) = _mm256_load_si256(sptr++);
        std::get<0x4>(p) = _mm256_load_si256(sptr++);
        std::get<0x5>(p) = _mm256_load_si256(sptr++);
        std::get<0x6>(p) = _mm256_load_si256(sptr++);
        std::get<0x7>(p) = _mm256_load_si256(sptr++);
        std::get<0x8>(p) = _mm256_load_si256(sptr++);
        std::get<0x9>(p) = _mm256_load_si256(sptr++);
        std::get<0xA>(p) = _mm256_load_si256(sptr++);
        std::get<0xB>(p) = _mm256_load_si256(sptr++);
        std::get<0xC>(p) = _mm256_load_si256(sptr++);
        std::get<0xD>(p) = _mm256_load_si256(sptr++);
        std::get<0xE>(p) = _mm256_load_si256(sptr++);
        std::get<0xF>(p) = _mm256_load_si256(sptr++);

        std::get<0>(s) =
            _mm256_unpacklo_epi64(std::get<0x0>(p), std::get<0x1>(p));
        std::get<1>(s) =
            _mm256_unpacklo_epi64(std::get<0x2>(p), std::get<0x3>(p));
        std::get<2>(s) =
            _mm256_unpacklo_epi64(std::get<0x4>(p), std::get<0x5>(p));
        std::get<3>(s) =
            _mm256_unpacklo_epi64(std::get<0x6>(p), std::get<0x7>(p));
        std::get<4>(s) =
            _mm256_unpacklo_epi64(std::get<0x8>(p), std::get<0x9>(p));
        std::get<5>(s) =
            _mm256_unpacklo_epi64(std::get<0xA>(p), std::get<0xB>(p));
        std::get<6>(s) =
            _mm256_unpacklo_epi64(std::get<0xC>(p), std::get<0xD>(p));
        std::get<7>(s) =
            _mm256_unpacklo_epi64(std::get<0xE>(p), std::get<0xF>(p));

        std::get<0>(t) =
            _mm256_unpackhi_epi64(std::get<0x0>(p), std::get<0x1>(p));
        std::get<1>(t) =
            _mm256_unpackhi_epi64(std::get<0x2>(p), std::get<0x3>(p));
        std::get<2>(t) =
            _mm256_unpackhi_epi64(std::get<0x4>(p), std::get<0x5>(p));
        std::get<3>(t) =
            _mm256_unpackhi_epi64(std::get<0x6>(p), std::get<0x7>(p));
        std::get<4>(t) =
            _mm256_unpackhi_epi64(std::get<0x8>(p), std::get<0x9>(p));
        std::get<5>(t) =
            _mm256_unpackhi_epi64(std::get<0xA>(p), std::get<0xB>(p));
        std::get<6>(t) =
            _mm256_unpackhi_epi64(std::get<0xC>(p), std::get<0xD>(p));
        std::get<7>(t) =
            _mm256_unpackhi_epi64(std::get<0xE>(p), std::get<0xF>(p));
    }

    static void unpack(std::array<std::array<T, K>, blocks()> &state,
        std::array<__m256i, 8> &s, std::array<__m256i, 8> &t)
    {
        std::array<__m256i, 16> p;

        std::get<0x0>(p) =
            _mm256_unpacklo_epi64(std::get<0>(s), std::get<0>(t));
        std::get<0x2>(p) =
            _mm256_unpacklo_epi64(std::get<1>(s), std::get<1>(t));
        std::get<0x4>(p) =
            _mm256_unpacklo_epi64(std::get<2>(s), std::get<2>(t));
        std::get<0x6>(p) =
            _mm256_unpacklo_epi64(std::get<3>(s), std::get<3>(t));
        std::get<0x8>(p) =
            _mm256_unpacklo_epi64(std::get<4>(s), std::get<4>(t));
        std::get<0xA>(p) =
            _mm256_unpacklo_epi64(std::get<5>(s), std::get<5>(t));
        std::get<0xC>(p) =
            _mm256_unpacklo_epi64(std::get<6>(s), std::get<6>(t));
        std::get<0xE>(p) =
            _mm256_unpacklo_epi64(std::get<7>(s), std::get<7>(t));

        std::get<0x1>(p) =
            _mm256_unpackhi_epi64(std::get<0>(s), std::get<0>(t));
        std::get<0x3>(p) =
            _mm256_unpackhi_epi64(std::get<1>(s), std::get<1>(t));
        std::get<0x5>(p) =
            _mm256_unpackhi_epi64(std::get<2>(s), std::get<2>(t));
        std::get<0x7>(p) =
            _mm256_unpackhi_epi64(std::get<3>(s), std::get<3>(t));
        std::get<0x9>(p) =
            _mm256_unpackhi_epi64(std::get<4>(s), std::get<4>(t));
        std::get<0xB>(p) =
            _mm256_unpackhi_epi64(std::get<5>(s), std::get<5>(t));
        std::get<0xD>(p) =
            _mm256_unpackhi_epi64(std::get<6>(s), std::get<6>(t));
        std::get<0xF>(p) =
            _mm256_unpackhi_epi64(std::get<7>(s), std::get<7>(t));

        __m256i *sptr = reinterpret_cast<__m256i *>(state.data());
        _mm256_store_si256(sptr++, std::get<0x0>(p));
        _mm256_store_si256(sptr++, std::get<0x1>(p));
        _mm256_store_si256(sptr++, std::get<0x2>(p));
        _mm256_store_si256(sptr++, std::get<0x3>(p));
        _mm256_store_si256(sptr++, std::get<0x4>(p));
        _mm256_store_si256(sptr++, std::get<0x5>(p));
        _mm256_store_si256(sptr++, std::get<0x6>(p));
        _mm256_store_si256(sptr++, std::get<0x7>(p));
        _mm256_store_si256(sptr++, std::get<0x8>(p));
        _mm256_store_si256(sptr++, std::get<0x9>(p));
        _mm256_store_si256(sptr++, std::get<0xA>(p));
        _mm256_store_si256(sptr++, std::get<0xB>(p));
        _mm256_store_si256(sptr++, std::get<0xC>(p));
        _mm256_store_si256(sptr++, std::get<0xD>(p));
        _mm256_store_si256(sptr++, std::get<0xE>(p));
        _mm256_store_si256(sptr++, std::get<0xF>(p));
    }

    template <std::size_t, std::size_t>
    static void set_key(std::array<__m256i, M_> &, std::array<__m256i, M_> &,
        const std::array<T, K + 1> &, std::false_type)
    {
    }

    template <std::size_t N, std::size_t I>
    static void set_key(std::array<__m256i, M_> &u, std::array<__m256i, M_> &v,
        const std::array<T, K + 1> &par, std::true_type)
    {
        static constexpr std::size_t S = N / 4;

        static constexpr std::size_t I0 = (I * 8 + 0) % K;
        static constexpr std::size_t I1 = (I * 8 + 4) % K;
        static constexpr std::size_t I2 = (I * 8 + 2) % K;
        static constexpr std::size_t I3 = (I * 8 + 6) % K;

        static constexpr std::size_t J0 = (I * 8 + 1) % K;
        static constexpr std::size_t J1 = (I * 8 + 5) % K;
        static constexpr std::size_t J2 = (I * 8 + 3) % K;
        static constexpr std::size_t J3 = (I * 8 + 7) % K;

        static constexpr std::size_t i0 = (S + I0) % (K + 1);
        static constexpr std::size_t i1 = (S + I1) % (K + 1);
        static constexpr std::size_t i2 = (S + I2) % (K + 1);
        static constexpr std::size_t i3 = (S + I3) % (K + 1);

        static constexpr std::size_t j0 = (S + J0) % (K + 1);
        static constexpr std::size_t j1 = (S + J1) % (K + 1);
        static constexpr std::size_t j2 = (S + J2) % (K + 1);
        static constexpr std::size_t j3 = (S + J3) % (K + 1);

        static constexpr T p0 = I0 == K - 1 ? S : 0;
        static constexpr T p1 = I1 == K - 1 ? S : 0;
        static constexpr T p2 = I2 == K - 1 ? S : 0;
        static constexpr T p3 = I3 == K - 1 ? S : 0;

        static constexpr T q0 = J0 == K - 1 ? S : 0;
        static constexpr T q1 = J1 == K - 1 ? S : 0;
        static constexpr T q2 = J2 == K - 1 ? S : 0;
        static constexpr T q3 = J3 == K - 1 ? S : 0;

        const MCKL_INT64 u0 = static_cast<MCKL_INT64>(std::get<i0>(par) + p0);
        const MCKL_INT64 u1 = static_cast<MCKL_INT64>(std::get<i1>(par) + p1);
        const MCKL_INT64 u2 = static_cast<MCKL_INT64>(std::get<i2>(par) + p2);
        const MCKL_INT64 u3 = static_cast<MCKL_INT64>(std::get<i3>(par) + p3);

        const MCKL_INT64 v0 = static_cast<MCKL_INT64>(std::get<j0>(par) + q0);
        const MCKL_INT64 v1 = static_cast<MCKL_INT64>(std::get<j1>(par) + q1);
        const MCKL_INT64 v2 = static_cast<MCKL_INT64>(std::get<j2>(par) + q2);
        const MCKL_INT64 v3 = static_cast<MCKL_INT64>(std::get<j3>(par) + q3);

        std::get<I>(u) = _mm256_set_epi64x(u3, u2, u1, u0);
        std::get<I>(v) = _mm256_set_epi64x(v3, v2, v1, v0);

        set_key<N, I + 1>(
            u, v, par, std::integral_constant<bool, I + 1 < M_>());
    }

    template <std::size_t, std::size_t>
    static void set_rotate(
        std::array<__m256i, M_> &, std::array<__m256i, M_> &, std::false_type)
    {
    }

    template <std::size_t N, std::size_t I>
    static void set_rotate(
        std::array<__m256i, M_> &l, std::array<__m256i, M_> &r, std::true_type)
    {
        static constexpr std::size_t I0 = (I * 8 + 0) % K;
        static constexpr std::size_t I1 = (I * 8 + 4) % K;
        static constexpr std::size_t I2 = (I * 8 + 2) % K;
        static constexpr std::size_t I3 = (I * 8 + 6) % K;

        static constexpr int L0 = Constants::rotate[I0 / 2][(N - 1) % 8];
        static constexpr int L1 = Constants::rotate[I1 / 2][(N - 1) % 8];
        static constexpr int L2 = Constants::rotate[I2 / 2][(N - 1) % 8];
        static constexpr int L3 = Constants::rotate[I3 / 2][(N - 1) % 8];

        static constexpr int R0 = 64 - L0;
        static constexpr int R1 = 64 - L1;
        static constexpr int R2 = 64 - L2;
        static constexpr int R3 = 64 - L3;

        std::get<I>(l) = _mm256_set_epi64x(L3, L2, L1, L0);
        std::get<I>(r) = _mm256_set_epi64x(R3, R2, R1, R0);

        set_rotate<N, I + 1>(l, r, std::integral_constant<bool, I + 1 < M_>());
    }

    template <std::size_t N>
    static void rotate(std::array<__m256i, 8> &t)
    {
        std::array<__m256i, M_> l;
        std::array<__m256i, M_> r;
        set_rotate<N, 0>(l, r, std::integral_constant<bool, 0 < M_>());

        __m256i l0 = _mm256_sllv_epi64(std::get<0>(t), std::get<0 % M_>(l));
        __m256i l1 = _mm256_sllv_epi64(std::get<1>(t), std::get<1 % M_>(l));
        __m256i l2 = _mm256_sllv_epi64(std::get<2>(t), std::get<2 % M_>(l));
        __m256i l3 = _mm256_sllv_epi64(std::get<3>(t), std::get<3 % M_>(l));
        __m256i l4 = _mm256_sllv_epi64(std::get<4>(t), std::get<4 % M_>(l));
        __m256i l5 = _mm256_sllv_epi64(std::get<5>(t), std::get<5 % M_>(l));
        __m256i l6 = _mm256_sllv_epi64(std::get<6>(t), std::get<6 % M_>(l));
        __m256i l7 = _mm256_sllv_epi64(std::get<7>(t), std::get<7 % M_>(l));

        __m256i r0 = _mm256_srlv_epi64(std::get<0>(t), std::get<0 % M_>(r));
        __m256i r1 = _mm256_srlv_epi64(std::get<1>(t), std::get<1 % M_>(r));
        __m256i r2 = _mm256_srlv_epi64(std::get<2>(t), std::get<2 % M_>(r));
        __m256i r3 = _mm256_srlv_epi64(std::get<3>(t), std::get<3 % M_>(r));
        __m256i r4 = _mm256_srlv_epi64(std::get<4>(t), std::get<4 % M_>(r));
        __m256i r5 = _mm256_srlv_epi64(std::get<5>(t), std::get<5 % M_>(r));
        __m256i r6 = _mm256_srlv_epi64(std::get<6>(t), std::get<6 % M_>(r));
        __m256i r7 = _mm256_srlv_epi64(std::get<7>(t), std::get<7 % M_>(r));

        std::get<0>(t) = _mm256_or_si256(l0, r0);
        std::get<1>(t) = _mm256_or_si256(l1, r1);
        std::get<2>(t) = _mm256_or_si256(l2, r2);
        std::get<3>(t) = _mm256_or_si256(l3, r3);
        std::get<4>(t) = _mm256_or_si256(l4, r4);
        std::get<5>(t) = _mm256_or_si256(l5, r5);
        std::get<6>(t) = _mm256_or_si256(l6, r6);
        std::get<7>(t) = _mm256_or_si256(l7, r7);
    }

    template <std::size_t N>
    static void kbox(std::array<__m256i, 8> &s, std::array<__m256i, 8> &t,
        const std::array<T, K + 1> &par)
    {
        kbox<N>(s, t, par,
            std::integral_constant<bool, (N % 4 == 0 && N <= Rounds)>());
    }

    template <std::size_t>
    static void kbox(std::array<__m256i, 8> &, std::array<__m256i, 8> &,
        const std::array<T, K + 1> &, std::false_type)
    {
    }

    template <std::size_t N>
    static void kbox(std::array<__m256i, 8> &s, std::array<__m256i, 8> &t,
        const std::array<T, K + 1> &par, std::true_type)
    {
        std::array<__m256i, M_> u;
        std::array<__m256i, M_> v;
        set_key<N, 0>(u, v, par, std::integral_constant<bool, 0 < M_>());

        std::get<0>(s) = _mm256_add_epi64(std::get<0>(s), std::get<0 % M_>(u));
        std::get<1>(s) = _mm256_add_epi64(std::get<1>(s), std::get<1 % M_>(u));
        std::get<2>(s) = _mm256_add_epi64(std::get<2>(s), std::get<2 % M_>(u));
        std::get<3>(s) = _mm256_add_epi64(std::get<3>(s), std::get<3 % M_>(u));
        std::get<4>(s) = _mm256_add_epi64(std::get<4>(s), std::get<4 % M_>(u));
        std::get<5>(s) = _mm256_add_epi64(std::get<5>(s), std::get<5 % M_>(u));
        std::get<6>(s) = _mm256_add_epi64(std::get<6>(s), std::get<6 % M_>(u));
        std::get<7>(s) = _mm256_add_epi64(std::get<7>(s), std::get<7 % M_>(u));

        std::get<0>(t) = _mm256_add_epi64(std::get<0>(t), std::get<0 % M_>(v));
        std::get<1>(t) = _mm256_add_epi64(std::get<1>(t), std::get<1 % M_>(v));
        std::get<2>(t) = _mm256_add_epi64(std::get<2>(t), std::get<2 % M_>(v));
        std::get<3>(t) = _mm256_add_epi64(std::get<3>(t), std::get<3 % M_>(v));
        std::get<4>(t) = _mm256_add_epi64(std::get<4>(t), std::get<4 % M_>(v));
        std::get<5>(t) = _mm256_add_epi64(std::get<5>(t), std::get<5 % M_>(v));
        std::get<6>(t) = _mm256_add_epi64(std::get<6>(t), std::get<6 % M_>(v));
        std::get<7>(t) = _mm256_add_epi64(std::get<7>(t), std::get<7 % M_>(v));
    }

    template <std::size_t N>
    static void sbox(std::array<__m256i, 8> &s, std::array<__m256i, 8> &t)
    {
        sbox<N>(s, t, std::integral_constant<bool, (N > 0 && N <= Rounds)>());
    }

    template <std::size_t>
    static void sbox(
        std::array<__m256i, 8> &, std::array<__m256i, 8> &, std::false_type)
    {
    }

    template <std::size_t N>
    static void sbox(
        std::array<__m256i, 8> &s, std::array<__m256i, 8> &t, std::true_type)
    {
        std::get<0>(s) = _mm256_add_epi64(std::get<0>(s), std::get<0>(t));
        std::get<1>(s) = _mm256_add_epi64(std::get<1>(s), std::get<1>(t));
        std::get<2>(s) = _mm256_add_epi64(std::get<2>(s), std::get<2>(t));
        std::get<3>(s) = _mm256_add_epi64(std::get<3>(s), std::get<3>(t));
        std::get<4>(s) = _mm256_add_epi64(std::get<4>(s), std::get<4>(t));
        std::get<5>(s) = _mm256_add_epi64(std::get<5>(s), std::get<5>(t));
        std::get<6>(s) = _mm256_add_epi64(std::get<6>(s), std::get<6>(t));
        std::get<7>(s) = _mm256_add_epi64(std::get<7>(s), std::get<7>(t));

        Derived::template rotate<N>(t);

        std::get<0>(t) = _mm256_xor_si256(std::get<0>(t), std::get<0>(s));
        std::get<1>(t) = _mm256_xor_si256(std::get<1>(t), std::get<1>(s));
        std::get<2>(t) = _mm256_xor_si256(std::get<2>(t), std::get<2>(s));
        std::get<3>(t) = _mm256_xor_si256(std::get<3>(t), std::get<3>(s));
        std::get<4>(t) = _mm256_xor_si256(std::get<4>(t), std::get<4>(s));
        std::get<5>(t) = _mm256_xor_si256(std::get<5>(t), std::get<5>(s));
        std::get<6>(t) = _mm256_xor_si256(std::get<6>(t), std::get<6>(s));
        std::get<7>(t) = _mm256_xor_si256(std::get<7>(t), std::get<7>(s));
    }

    template <std::size_t N>
    static void pbox(std::array<__m256i, 8> &s, std::array<__m256i, 8> &t)
    {
        pbox<N>(s, t, std::integral_constant<bool, (N > 0 && N <= Rounds)>());
    }

    template <std::size_t>
    static void pbox(
        std::array<__m256i, 8> &, std::array<__m256i, 8> &, std::false_type)
    {
    }

    template <std::size_t N>
    static void pbox(
        std::array<__m256i, 8> &s, std::array<__m256i, 8> &t, std::true_type)
    {
        Derived::template permute<N>(s, t);
    }
}; // class ThreefryGeneratorAVX2Impl

#include <mckl/random/internal/threefry2x64_avx2.hpp>
#include <mckl/random/internal/threefry4x64_avx2.hpp>
#include <mckl/random/internal/threefry8x64_avx2.hpp>
#include <mckl/random/internal/threefry16x64_avx2.hpp>
