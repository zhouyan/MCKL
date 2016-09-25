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

// Packing scheme
// s: E 6 C 4 A 2 8 0
// t: F 7 D 5 B 3 9 1
template <typename T, std::size_t K, std::size_t Rounds, typename Constants,
    typename Derived>
class ThreefryGeneratorAVX2Impl<T, K, Rounds, Constants, Derived, 32>
{
    public:
    static constexpr std::size_t blocks()
    {
        return K != 0 && 128 % K == 0 ?
            128 / K :
            ThreefryGeneratorGenericImpl<T, K, Rounds, Constants>::blocks();
    }

    static void eval(std::array<T, K> &state, const std::array<T, K + 1> &par)
    {
        eval(state, par,
            std::integral_constant<bool, (K != 0 && K % 16 == 0)>());
    }

    static void eval(std::array<std::array<T, K>, blocks()> &state,
        const std::array<T, K + 1> &par)
    {
        eval(state, par,
            std::integral_constant<bool, (K != 0 && 128 % K == 0)>());
    }

    private:
    static constexpr std::size_t M_ = K / 16 + (K % 16 == 0 ? 0 : 1);

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

        // 3 1 2 0
        std::get<0x0>(p) = _mm256_shuffle_epi32(std::get<0x0>(p), 0xD8);
        std::get<0x1>(p) = _mm256_shuffle_epi32(std::get<0x1>(p), 0xD8);
        std::get<0x2>(p) = _mm256_shuffle_epi32(std::get<0x2>(p), 0xD8);
        std::get<0x3>(p) = _mm256_shuffle_epi32(std::get<0x3>(p), 0xD8);
        std::get<0x4>(p) = _mm256_shuffle_epi32(std::get<0x4>(p), 0xD8);
        std::get<0x5>(p) = _mm256_shuffle_epi32(std::get<0x5>(p), 0xD8);
        std::get<0x6>(p) = _mm256_shuffle_epi32(std::get<0x6>(p), 0xD8);
        std::get<0x7>(p) = _mm256_shuffle_epi32(std::get<0x7>(p), 0xD8);
        std::get<0x8>(p) = _mm256_shuffle_epi32(std::get<0x8>(p), 0xD8);
        std::get<0x9>(p) = _mm256_shuffle_epi32(std::get<0x9>(p), 0xD8);
        std::get<0xA>(p) = _mm256_shuffle_epi32(std::get<0xA>(p), 0xD8);
        std::get<0xB>(p) = _mm256_shuffle_epi32(std::get<0xB>(p), 0xD8);
        std::get<0xC>(p) = _mm256_shuffle_epi32(std::get<0xC>(p), 0xD8);
        std::get<0xD>(p) = _mm256_shuffle_epi32(std::get<0xD>(p), 0xD8);
        std::get<0xE>(p) = _mm256_shuffle_epi32(std::get<0xE>(p), 0xD8);
        std::get<0xF>(p) = _mm256_shuffle_epi32(std::get<0xF>(p), 0xD8);

        std::get<0>(s) =
            _mm256_unpacklo_epi32(std::get<0x0>(p), std::get<0x1>(p));
        std::get<1>(s) =
            _mm256_unpacklo_epi32(std::get<0x2>(p), std::get<0x3>(p));
        std::get<2>(s) =
            _mm256_unpacklo_epi32(std::get<0x4>(p), std::get<0x5>(p));
        std::get<3>(s) =
            _mm256_unpacklo_epi32(std::get<0x6>(p), std::get<0x7>(p));
        std::get<4>(s) =
            _mm256_unpacklo_epi32(std::get<0x8>(p), std::get<0x9>(p));
        std::get<5>(s) =
            _mm256_unpacklo_epi32(std::get<0xA>(p), std::get<0xB>(p));
        std::get<6>(s) =
            _mm256_unpacklo_epi32(std::get<0xC>(p), std::get<0xD>(p));
        std::get<7>(s) =
            _mm256_unpacklo_epi32(std::get<0xE>(p), std::get<0xF>(p));

        std::get<0>(t) =
            _mm256_unpackhi_epi32(std::get<0x0>(p), std::get<0x1>(p));
        std::get<1>(t) =
            _mm256_unpackhi_epi32(std::get<0x2>(p), std::get<0x3>(p));
        std::get<2>(t) =
            _mm256_unpackhi_epi32(std::get<0x4>(p), std::get<0x5>(p));
        std::get<3>(t) =
            _mm256_unpackhi_epi32(std::get<0x6>(p), std::get<0x7>(p));
        std::get<4>(t) =
            _mm256_unpackhi_epi32(std::get<0x8>(p), std::get<0x9>(p));
        std::get<5>(t) =
            _mm256_unpackhi_epi32(std::get<0xA>(p), std::get<0xB>(p));
        std::get<6>(t) =
            _mm256_unpackhi_epi32(std::get<0xC>(p), std::get<0xD>(p));
        std::get<7>(t) =
            _mm256_unpackhi_epi32(std::get<0xE>(p), std::get<0xF>(p));
    }

    static void unpack(std::array<std::array<T, K>, blocks()> &state,
        std::array<__m256i, 8> &s, std::array<__m256i, 8> &t)
    {
        std::array<__m256i, 16> p;

        // 3 1 2 0
        std::get<0>(s) = _mm256_shuffle_epi32(std::get<0>(s), 0xD8);
        std::get<1>(s) = _mm256_shuffle_epi32(std::get<1>(s), 0xD8);
        std::get<2>(s) = _mm256_shuffle_epi32(std::get<2>(s), 0xD8);
        std::get<3>(s) = _mm256_shuffle_epi32(std::get<3>(s), 0xD8);
        std::get<4>(s) = _mm256_shuffle_epi32(std::get<4>(s), 0xD8);
        std::get<5>(s) = _mm256_shuffle_epi32(std::get<5>(s), 0xD8);
        std::get<6>(s) = _mm256_shuffle_epi32(std::get<6>(s), 0xD8);
        std::get<7>(s) = _mm256_shuffle_epi32(std::get<7>(s), 0xD8);

        // 3 1 2 0
        std::get<0>(t) = _mm256_shuffle_epi32(std::get<0>(t), 0xD8);
        std::get<1>(t) = _mm256_shuffle_epi32(std::get<1>(t), 0xD8);
        std::get<2>(t) = _mm256_shuffle_epi32(std::get<2>(t), 0xD8);
        std::get<3>(t) = _mm256_shuffle_epi32(std::get<3>(t), 0xD8);
        std::get<4>(t) = _mm256_shuffle_epi32(std::get<4>(t), 0xD8);
        std::get<5>(t) = _mm256_shuffle_epi32(std::get<5>(t), 0xD8);
        std::get<6>(t) = _mm256_shuffle_epi32(std::get<6>(t), 0xD8);
        std::get<7>(t) = _mm256_shuffle_epi32(std::get<7>(t), 0xD8);

        std::get<0x0>(p) =
            _mm256_unpacklo_epi32(std::get<0>(s), std::get<0>(t));
        std::get<0x2>(p) =
            _mm256_unpacklo_epi32(std::get<1>(s), std::get<1>(t));
        std::get<0x4>(p) =
            _mm256_unpacklo_epi32(std::get<2>(s), std::get<2>(t));
        std::get<0x6>(p) =
            _mm256_unpacklo_epi32(std::get<3>(s), std::get<3>(t));
        std::get<0x8>(p) =
            _mm256_unpacklo_epi32(std::get<4>(s), std::get<4>(t));
        std::get<0xA>(p) =
            _mm256_unpacklo_epi32(std::get<5>(s), std::get<5>(t));
        std::get<0xC>(p) =
            _mm256_unpacklo_epi32(std::get<6>(s), std::get<6>(t));
        std::get<0xE>(p) =
            _mm256_unpacklo_epi32(std::get<7>(s), std::get<7>(t));

        std::get<0x1>(p) =
            _mm256_unpackhi_epi32(std::get<0>(s), std::get<0>(t));
        std::get<0x3>(p) =
            _mm256_unpackhi_epi32(std::get<1>(s), std::get<1>(t));
        std::get<0x5>(p) =
            _mm256_unpackhi_epi32(std::get<2>(s), std::get<2>(t));
        std::get<0x7>(p) =
            _mm256_unpackhi_epi32(std::get<3>(s), std::get<3>(t));
        std::get<0x9>(p) =
            _mm256_unpackhi_epi32(std::get<4>(s), std::get<4>(t));
        std::get<0xB>(p) =
            _mm256_unpackhi_epi32(std::get<5>(s), std::get<5>(t));
        std::get<0xD>(p) =
            _mm256_unpackhi_epi32(std::get<6>(s), std::get<6>(t));
        std::get<0xF>(p) =
            _mm256_unpackhi_epi32(std::get<7>(s), std::get<7>(t));

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

        static constexpr std::size_t I0 = (I * 16 + 0x0) % K;
        static constexpr std::size_t I1 = (I * 16 + 0x8) % K;
        static constexpr std::size_t I2 = (I * 16 + 0x2) % K;
        static constexpr std::size_t I3 = (I * 16 + 0xA) % K;
        static constexpr std::size_t I4 = (I * 16 + 0x4) % K;
        static constexpr std::size_t I5 = (I * 16 + 0xC) % K;
        static constexpr std::size_t I6 = (I * 16 + 0x6) % K;
        static constexpr std::size_t I7 = (I * 16 + 0xE) % K;

        static constexpr std::size_t J0 = (I * 16 + 0x1) % K;
        static constexpr std::size_t J1 = (I * 16 + 0x9) % K;
        static constexpr std::size_t J2 = (I * 16 + 0x3) % K;
        static constexpr std::size_t J3 = (I * 16 + 0xB) % K;
        static constexpr std::size_t J4 = (I * 16 + 0x5) % K;
        static constexpr std::size_t J5 = (I * 16 + 0xD) % K;
        static constexpr std::size_t J6 = (I * 16 + 0x7) % K;
        static constexpr std::size_t J7 = (I * 16 + 0xF) % K;

        static constexpr std::size_t i0 = (S + I0) % (K + 1);
        static constexpr std::size_t i1 = (S + I1) % (K + 1);
        static constexpr std::size_t i2 = (S + I2) % (K + 1);
        static constexpr std::size_t i3 = (S + I3) % (K + 1);
        static constexpr std::size_t i4 = (S + I4) % (K + 1);
        static constexpr std::size_t i5 = (S + I5) % (K + 1);
        static constexpr std::size_t i6 = (S + I6) % (K + 1);
        static constexpr std::size_t i7 = (S + I7) % (K + 1);

        static constexpr std::size_t j0 = (S + J0) % (K + 1);
        static constexpr std::size_t j1 = (S + J1) % (K + 1);
        static constexpr std::size_t j2 = (S + J2) % (K + 1);
        static constexpr std::size_t j3 = (S + J3) % (K + 1);
        static constexpr std::size_t j4 = (S + J4) % (K + 1);
        static constexpr std::size_t j5 = (S + J5) % (K + 1);
        static constexpr std::size_t j6 = (S + J6) % (K + 1);
        static constexpr std::size_t j7 = (S + J7) % (K + 1);

        static constexpr T p0 = I0 == K - 1 ? S : 0;
        static constexpr T p1 = I1 == K - 1 ? S : 0;
        static constexpr T p2 = I2 == K - 1 ? S : 0;
        static constexpr T p3 = I3 == K - 1 ? S : 0;
        static constexpr T p4 = I4 == K - 1 ? S : 0;
        static constexpr T p5 = I5 == K - 1 ? S : 0;
        static constexpr T p6 = I6 == K - 1 ? S : 0;
        static constexpr T p7 = I7 == K - 1 ? S : 0;

        static constexpr T q0 = J0 == K - 1 ? S : 0;
        static constexpr T q1 = J1 == K - 1 ? S : 0;
        static constexpr T q2 = J2 == K - 1 ? S : 0;
        static constexpr T q3 = J3 == K - 1 ? S : 0;
        static constexpr T q4 = J4 == K - 1 ? S : 0;
        static constexpr T q5 = J5 == K - 1 ? S : 0;
        static constexpr T q6 = J6 == K - 1 ? S : 0;
        static constexpr T q7 = J7 == K - 1 ? S : 0;

        const int u0 = static_cast<int>(std::get<i0>(par) + p0);
        const int u1 = static_cast<int>(std::get<i1>(par) + p1);
        const int u2 = static_cast<int>(std::get<i2>(par) + p2);
        const int u3 = static_cast<int>(std::get<i3>(par) + p3);
        const int u4 = static_cast<int>(std::get<i4>(par) + p4);
        const int u5 = static_cast<int>(std::get<i5>(par) + p5);
        const int u6 = static_cast<int>(std::get<i6>(par) + p6);
        const int u7 = static_cast<int>(std::get<i7>(par) + p7);

        const int v0 = static_cast<int>(std::get<j0>(par) + q0);
        const int v1 = static_cast<int>(std::get<j1>(par) + q1);
        const int v2 = static_cast<int>(std::get<j2>(par) + q2);
        const int v3 = static_cast<int>(std::get<j3>(par) + q3);
        const int v4 = static_cast<int>(std::get<j4>(par) + q4);
        const int v5 = static_cast<int>(std::get<j5>(par) + q5);
        const int v6 = static_cast<int>(std::get<j6>(par) + q6);
        const int v7 = static_cast<int>(std::get<j7>(par) + q7);

        std::get<I>(u) = _mm256_set_epi32(u7, u6, u5, u4, u3, u2, u1, u0);
        std::get<I>(v) = _mm256_set_epi32(v7, v6, v5, v4, v3, v2, v1, v0);

        set_key<N, I + 1>(
            u, v, par, std::integral_constant<bool, I + 1 < M_>());
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

        std::get<0>(s) = _mm256_add_epi32(std::get<0>(s), std::get<0 % M_>(u));
        std::get<1>(s) = _mm256_add_epi32(std::get<1>(s), std::get<1 % M_>(u));
        std::get<2>(s) = _mm256_add_epi32(std::get<2>(s), std::get<2 % M_>(u));
        std::get<3>(s) = _mm256_add_epi32(std::get<3>(s), std::get<3 % M_>(u));
        std::get<4>(s) = _mm256_add_epi32(std::get<4>(s), std::get<4 % M_>(u));
        std::get<5>(s) = _mm256_add_epi32(std::get<5>(s), std::get<5 % M_>(u));
        std::get<6>(s) = _mm256_add_epi32(std::get<6>(s), std::get<6 % M_>(u));
        std::get<7>(s) = _mm256_add_epi32(std::get<7>(s), std::get<7 % M_>(u));

        std::get<0>(t) = _mm256_add_epi32(std::get<0>(t), std::get<0 % M_>(v));
        std::get<1>(t) = _mm256_add_epi32(std::get<1>(t), std::get<1 % M_>(v));
        std::get<2>(t) = _mm256_add_epi32(std::get<2>(t), std::get<2 % M_>(v));
        std::get<3>(t) = _mm256_add_epi32(std::get<3>(t), std::get<3 % M_>(v));
        std::get<4>(t) = _mm256_add_epi32(std::get<4>(t), std::get<4 % M_>(v));
        std::get<5>(t) = _mm256_add_epi32(std::get<5>(t), std::get<5 % M_>(v));
        std::get<6>(t) = _mm256_add_epi32(std::get<6>(t), std::get<6 % M_>(v));
        std::get<7>(t) = _mm256_add_epi32(std::get<7>(t), std::get<7 % M_>(v));
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
        std::get<0>(s) = _mm256_add_epi32(std::get<0>(s), std::get<0>(t));
        std::get<1>(s) = _mm256_add_epi32(std::get<1>(s), std::get<1>(t));
        std::get<2>(s) = _mm256_add_epi32(std::get<2>(s), std::get<2>(t));
        std::get<3>(s) = _mm256_add_epi32(std::get<3>(s), std::get<3>(t));
        std::get<4>(s) = _mm256_add_epi32(std::get<4>(s), std::get<4>(t));
        std::get<5>(s) = _mm256_add_epi32(std::get<5>(s), std::get<5>(t));
        std::get<6>(s) = _mm256_add_epi32(std::get<6>(s), std::get<6>(t));
        std::get<7>(s) = _mm256_add_epi32(std::get<7>(s), std::get<7>(t));

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
        pbox(s, t, std::integral_constant<bool, (N > 0 && N <= Rounds)>());
    }

    static void pbox(
        std::array<__m256i, 8> &, std::array<__m256i, 8> &, std::false_type)
    {
    }

    static void pbox(
        std::array<__m256i, 8> &s, std::array<__m256i, 8> &t, std::true_type)
    {
        Derived::pbox(s, t);
    }
}; // class ThreefryGeneratorAVX2Impl

template <typename T, std::size_t Rounds>
class ThreefryGeneratorImpl<T, 2, Rounds, ThreefryConstants<T, 2>, 32>
    : public ThreefryGeneratorAVX2Impl<T, 2, Rounds, ThreefryConstants<T, 2>,
          ThreefryGeneratorImpl<T, 2, Rounds, ThreefryConstants<T, 2>>>
{
    friend ThreefryGeneratorAVX2Impl<T, 2, Rounds, ThreefryConstants<T, 2>,
        ThreefryGeneratorImpl<T, 2, Rounds, ThreefryConstants<T, 2>>>;

    static void pbox(std::array<__m256i, 8> &, std::array<__m256i, 8> &) {}

    template <std::size_t N>
    static void rotate(std::array<__m256i, 8> &t)
    {
        using constants = ThreefryConstants<T, 2>;

        static constexpr int L = constants::rotate[0][(N - 1) % 8];
        static constexpr int R = 32 - L;

        __m256i l0 = _mm256_slli_epi32(std::get<0>(t), L);
        __m256i l1 = _mm256_slli_epi32(std::get<1>(t), L);
        __m256i l2 = _mm256_slli_epi32(std::get<2>(t), L);
        __m256i l3 = _mm256_slli_epi32(std::get<3>(t), L);
        __m256i l4 = _mm256_slli_epi32(std::get<4>(t), L);
        __m256i l5 = _mm256_slli_epi32(std::get<5>(t), L);
        __m256i l6 = _mm256_slli_epi32(std::get<6>(t), L);
        __m256i l7 = _mm256_slli_epi32(std::get<7>(t), L);

        __m256i r0 = _mm256_srli_epi32(std::get<0>(t), R);
        __m256i r1 = _mm256_srli_epi32(std::get<1>(t), R);
        __m256i r2 = _mm256_srli_epi32(std::get<2>(t), R);
        __m256i r3 = _mm256_srli_epi32(std::get<3>(t), R);
        __m256i r4 = _mm256_srli_epi32(std::get<4>(t), R);
        __m256i r5 = _mm256_srli_epi32(std::get<5>(t), R);
        __m256i r6 = _mm256_srli_epi32(std::get<6>(t), R);
        __m256i r7 = _mm256_srli_epi32(std::get<7>(t), R);

        std::get<0>(t) = _mm256_or_si256(l0, r0);
        std::get<1>(t) = _mm256_or_si256(l1, r1);
        std::get<2>(t) = _mm256_or_si256(l2, r2);
        std::get<3>(t) = _mm256_or_si256(l3, r3);
        std::get<4>(t) = _mm256_or_si256(l4, r4);
        std::get<5>(t) = _mm256_or_si256(l5, r5);
        std::get<6>(t) = _mm256_or_si256(l6, r6);
        std::get<7>(t) = _mm256_or_si256(l7, r7);
    }
}; // class ThreefryGeneratorImpl

// Packing scheme
// s: 2''' 2' 0''' 0' 2'' 2 0'' 0
// t: 3''' 3' 1''' 1' 3'' 3 1'' 1
template <typename T, std::size_t Rounds>
class ThreefryGeneratorImpl<T, 4, Rounds, ThreefryConstants<T, 4>, 32>
    : public ThreefryGeneratorAVX2Impl<T, 4, Rounds, ThreefryConstants<T, 4>,
          ThreefryGeneratorImpl<T, 4, Rounds, ThreefryConstants<T, 4>>>
{
    friend ThreefryGeneratorAVX2Impl<T, 4, Rounds, ThreefryConstants<T, 4>,
        ThreefryGeneratorImpl<T, 4, Rounds, ThreefryConstants<T, 4>>>;

    static void pbox(std::array<__m256i, 8> &, std::array<__m256i, 8> &t)
    {
        // 1 0 3 2
        std::get<0>(t) = _mm256_shuffle_epi32(std::get<0>(t), 0x4E);
        std::get<1>(t) = _mm256_shuffle_epi32(std::get<1>(t), 0x4E);
        std::get<2>(t) = _mm256_shuffle_epi32(std::get<2>(t), 0x4E);
        std::get<3>(t) = _mm256_shuffle_epi32(std::get<3>(t), 0x4E);
        std::get<4>(t) = _mm256_shuffle_epi32(std::get<4>(t), 0x4E);
        std::get<5>(t) = _mm256_shuffle_epi32(std::get<5>(t), 0x4E);
        std::get<6>(t) = _mm256_shuffle_epi32(std::get<6>(t), 0x4E);
        std::get<7>(t) = _mm256_shuffle_epi32(std::get<7>(t), 0x4E);
    }

    template <std::size_t N>
    static void rotate(std::array<__m256i, 8> &t)
    {
        using constants = ThreefryConstants<T, 4>;

        static constexpr int L0 = constants::rotate[0][(N - 1) % 8];
        static constexpr int L1 = constants::rotate[1][(N - 1) % 8];
        static constexpr int R0 = 32 - L0;
        static constexpr int R1 = 32 - L1;

        __m256i x0 = _mm256_unpacklo_epi64(std::get<0>(t), std::get<1>(t));
        __m256i x1 = _mm256_unpacklo_epi64(std::get<2>(t), std::get<3>(t));
        __m256i x2 = _mm256_unpacklo_epi64(std::get<4>(t), std::get<5>(t));
        __m256i x3 = _mm256_unpacklo_epi64(std::get<6>(t), std::get<7>(t));

        __m256i x4 = _mm256_unpackhi_epi64(std::get<0>(t), std::get<1>(t));
        __m256i x5 = _mm256_unpackhi_epi64(std::get<2>(t), std::get<3>(t));
        __m256i x6 = _mm256_unpackhi_epi64(std::get<4>(t), std::get<5>(t));
        __m256i x7 = _mm256_unpackhi_epi64(std::get<6>(t), std::get<7>(t));

        __m256i l0 = _mm256_slli_epi32(x0, L0);
        __m256i l1 = _mm256_slli_epi32(x1, L0);
        __m256i l2 = _mm256_slli_epi32(x2, L0);
        __m256i l3 = _mm256_slli_epi32(x3, L0);
        __m256i l4 = _mm256_slli_epi32(x4, L1);
        __m256i l5 = _mm256_slli_epi32(x5, L1);
        __m256i l6 = _mm256_slli_epi32(x6, L1);
        __m256i l7 = _mm256_slli_epi32(x7, L1);

        __m256i r0 = _mm256_srli_epi32(x0, R0);
        __m256i r1 = _mm256_srli_epi32(x1, R0);
        __m256i r2 = _mm256_srli_epi32(x2, R0);
        __m256i r3 = _mm256_srli_epi32(x3, R0);
        __m256i r4 = _mm256_srli_epi32(x4, R1);
        __m256i r5 = _mm256_srli_epi32(x5, R1);
        __m256i r6 = _mm256_srli_epi32(x6, R1);
        __m256i r7 = _mm256_srli_epi32(x7, R1);

        x0 = _mm256_or_si256(l0, r0);
        x1 = _mm256_or_si256(l1, r1);
        x2 = _mm256_or_si256(l2, r2);
        x3 = _mm256_or_si256(l3, r3);
        x4 = _mm256_or_si256(l4, r4);
        x5 = _mm256_or_si256(l5, r5);
        x6 = _mm256_or_si256(l6, r6);
        x7 = _mm256_or_si256(l7, r7);

        std::get<0>(t) = _mm256_unpacklo_epi64(x0, x4);
        std::get<2>(t) = _mm256_unpacklo_epi64(x1, x5);
        std::get<4>(t) = _mm256_unpacklo_epi64(x2, x6);
        std::get<6>(t) = _mm256_unpacklo_epi64(x3, x7);

        std::get<1>(t) = _mm256_unpackhi_epi64(x0, x4);
        std::get<3>(t) = _mm256_unpackhi_epi64(x1, x5);
        std::get<5>(t) = _mm256_unpackhi_epi64(x2, x6);
        std::get<7>(t) = _mm256_unpackhi_epi64(x3, x7);
    }
}; // class ThreefryGeneratorImpl
