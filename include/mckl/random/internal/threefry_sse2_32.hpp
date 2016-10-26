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

#ifndef MCKL_RANDOM_INTERNAL_THREEFRY_SSE2_32_HPP
#define MCKL_RANDOM_INTERNAL_THREEFRY_SSE2_32_HPP

#include <mckl/random/internal/common.hpp>
#include <mckl/random/internal/threefry_generic.hpp>
#include <mckl/random/increment.hpp>

#ifdef MCKL_GCC
#if MCKL_GCC_VERSION >= 60000
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wignored-attributes"
#endif
#endif

namespace mckl
{

namespace internal
{

template <typename T, std::size_t K, std::size_t Rounds, typename Constants>
class ThreefryGeneratorSSE2Impl32
{
    public:
    static constexpr bool batch()
    {
        return std::numeric_limits<T>::digits == 32 && K != 0 && 16 % K == 0;
    }

    static void eval(std::array<T, K> &state, const std::array<T, K + 4> &par)
    {
        ThreefryGeneratorGenericImpl<T, K, Rounds, Constants>::eval(
            state, par);
    }

    template <typename ResultType>
    static void eval(Counter<T, K> &ctr, const std::array<T, K + 4> &par,
        std::size_t n, ResultType *r)
    {
        constexpr std::size_t S = K <= 8 ? 8 : K;
        constexpr std::size_t cstride = sizeof(__m128i) * S;
        constexpr std::size_t nstride = cstride / (sizeof(T) * K);
        constexpr std::size_t rstride = cstride / sizeof(ResultType);

        std::array<__m128i, S> s;
        while (n >= nstride) {
            increment_si128(ctr, s);

            transpose4x32_load_si128(s);

            rbox<0x00>(s);
            kbox<0x00>(s, par);
            rbox<0x01>(s);
            kbox<0x01>(s, par);
            rbox<0x02>(s);
            kbox<0x02>(s, par);
            rbox<0x03>(s);
            kbox<0x03>(s, par);
            rbox<0x04>(s);
            kbox<0x04>(s, par);
            rbox<0x05>(s);
            kbox<0x05>(s, par);
            rbox<0x06>(s);
            kbox<0x06>(s, par);
            rbox<0x07>(s);
            kbox<0x07>(s, par);
            rbox<0x08>(s);
            kbox<0x08>(s, par);
            rbox<0x09>(s);
            kbox<0x09>(s, par);
            rbox<0x0A>(s);
            kbox<0x0A>(s, par);
            rbox<0x0B>(s);
            kbox<0x0B>(s, par);
            rbox<0x0C>(s);
            kbox<0x0C>(s, par);
            rbox<0x0D>(s);
            kbox<0x0D>(s, par);
            rbox<0x0E>(s);
            kbox<0x0E>(s, par);
            rbox<0x0F>(s);
            kbox<0x0F>(s, par);
            rbox<0x10>(s);
            kbox<0x10>(s, par);
            rbox<0x11>(s);
            kbox<0x11>(s, par);
            rbox<0x12>(s);
            kbox<0x12>(s, par);
            rbox<0x13>(s);
            kbox<0x13>(s, par);
            rbox<0x14>(s);
            kbox<0x14>(s, par);
            rbox<0x15>(s);
            kbox<0x15>(s, par);
            rbox<0x16>(s);
            kbox<0x16>(s, par);
            rbox<0x17>(s);
            kbox<0x17>(s, par);
            rbox<0x18>(s);
            kbox<0x18>(s, par);
            rbox<0x19>(s);
            kbox<0x19>(s, par);
            rbox<0x1A>(s);
            kbox<0x1A>(s, par);
            rbox<0x1B>(s);
            kbox<0x1B>(s, par);
            rbox<0x1C>(s);
            kbox<0x1C>(s, par);
            rbox<0x1D>(s);
            kbox<0x1D>(s, par);
            rbox<0x1E>(s);
            kbox<0x1E>(s, par);
            rbox<0x1F>(s);
            kbox<0x1F>(s, par);

            round<0x20>(
                s, par, std::integral_constant<bool, 0x20 <= Rounds>());

            transpose4x32_store_si128(s);

            std::memcpy(r, s.data(), cstride);
            n -= nstride;
            r += rstride;
        }

        alignas(32) union {
            std::array<std::array<T, K>, nstride> state;
            std::array<Counter<T, K>, nstride> ctr;
        } buf;
        for (std::size_t i = 0; i != n; ++i) {
            increment(ctr);
            buf.ctr[i] = ctr;
            eval(buf.state[i], par);
        }
        std::memcpy(r, buf.state.data(), sizeof(T) * K * n);
    }

    private:
    template <std::size_t N, std::size_t S>
    static void round(std::array<__m128i, S> &, const std::array<T, K + 4> &,
        std::false_type)
    {
    }

    template <std::size_t N, std::size_t S>
    static void round(std::array<__m128i, S> &s,
        const std::array<T, K + 4> &par, std::true_type)
    {
        rbox<N>(s);
        kbox<N>(s, par);
        round<N + 1>(s, par, std::integral_constant<bool, N + 1 <= Rounds>());
    }

    template <std::size_t N, std::size_t S>
    static void kbox(
        std::array<__m128i, S> &s, const std::array<T, K + 4> &par)
    {
        kbox<N>(s, par,
            std::integral_constant<bool, (N % 4 == 0 && N <= Rounds)>());
    }

    template <std::size_t, std::size_t S>
    static void kbox(std::array<__m128i, S> &, const std::array<T, K + 4> &,
        std::false_type)
    {
    }

    template <std::size_t N>
    static void kbox(std::array<__m128i, 8> &s,
        const std::array<T, K + 4> &par, std::true_type)
    {
        std::array<__m128i, K> k;
        set_key<N>(k, par);

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
    static void kbox(std::array<__m128i, 16> &s,
        const std::array<T, K + 4> &par, std::true_type)
    {
        std::array<__m128i, K> k;
        set_key<N>(k, par);

        std::get<0x0>(s) =
            _mm_add_epi32(std::get<0x0>(s), std::get<0x0 % K>(k));
        std::get<0x1>(s) =
            _mm_add_epi32(std::get<0x1>(s), std::get<0x1 % K>(k));
        std::get<0x2>(s) =
            _mm_add_epi32(std::get<0x2>(s), std::get<0x2 % K>(k));
        std::get<0x3>(s) =
            _mm_add_epi32(std::get<0x3>(s), std::get<0x3 % K>(k));
        std::get<0x4>(s) =
            _mm_add_epi32(std::get<0x4>(s), std::get<0x4 % K>(k));
        std::get<0x5>(s) =
            _mm_add_epi32(std::get<0x5>(s), std::get<0x5 % K>(k));
        std::get<0x6>(s) =
            _mm_add_epi32(std::get<0x6>(s), std::get<0x6 % K>(k));
        std::get<0x7>(s) =
            _mm_add_epi32(std::get<0x7>(s), std::get<0x7 % K>(k));
        std::get<0x8>(s) =
            _mm_add_epi32(std::get<0x8>(s), std::get<0x8 % K>(k));
        std::get<0x9>(s) =
            _mm_add_epi32(std::get<0x9>(s), std::get<0x9 % K>(k));
        std::get<0xA>(s) =
            _mm_add_epi32(std::get<0xA>(s), std::get<0xA % K>(k));
        std::get<0xB>(s) =
            _mm_add_epi32(std::get<0xB>(s), std::get<0xB % K>(k));
        std::get<0xC>(s) =
            _mm_add_epi32(std::get<0xC>(s), std::get<0xC % K>(k));
        std::get<0xD>(s) =
            _mm_add_epi32(std::get<0xD>(s), std::get<0xD % K>(k));
        std::get<0xE>(s) =
            _mm_add_epi32(std::get<0xE>(s), std::get<0xE % K>(k));
        std::get<0xF>(s) =
            _mm_add_epi32(std::get<0xF>(s), std::get<0xF % K>(k));
    }

    template <std::size_t N, std::size_t S>
    static void rbox(std::array<__m128i, S> &s)
    {
        rbox<N>(s, std::integral_constant<bool, (N > 0 && N <= Rounds)>());
    }

    template <std::size_t, std::size_t S>
    static void rbox(std::array<__m128i, S> &, std::false_type)
    {
    }

    template <std::size_t N>
    static void rbox(std::array<__m128i, 8> &s, std::true_type)
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

        permute<N>(s);
    }

    template <std::size_t N>
    static void rbox(std::array<__m128i, 16> &s, std::true_type)
    {
        constexpr int L0 = Constants::rotate::value[0 % (K / 2)][(N - 1) % 8];
        constexpr int L1 = Constants::rotate::value[1 % (K / 2)][(N - 1) % 8];
        constexpr int L2 = Constants::rotate::value[2 % (K / 2)][(N - 1) % 8];
        constexpr int L3 = Constants::rotate::value[3 % (K / 2)][(N - 1) % 8];
        constexpr int L4 = Constants::rotate::value[4 % (K / 2)][(N - 1) % 8];
        constexpr int L5 = Constants::rotate::value[5 % (K / 2)][(N - 1) % 8];
        constexpr int L6 = Constants::rotate::value[6 % (K / 2)][(N - 1) % 8];
        constexpr int L7 = Constants::rotate::value[7 % (K / 2)][(N - 1) % 8];
        constexpr int R0 = 32 - L0;
        constexpr int R1 = 32 - L1;
        constexpr int R2 = 32 - L2;
        constexpr int R3 = 32 - L3;
        constexpr int R4 = 32 - L4;
        constexpr int R5 = 32 - L5;
        constexpr int R6 = 32 - L6;
        constexpr int R7 = 32 - L7;

        std::get<0x0>(s) = _mm_add_epi32(std::get<0x0>(s), std::get<0x1>(s));
        std::get<0x2>(s) = _mm_add_epi32(std::get<0x2>(s), std::get<0x3>(s));
        std::get<0x4>(s) = _mm_add_epi32(std::get<0x4>(s), std::get<0x5>(s));
        std::get<0x6>(s) = _mm_add_epi32(std::get<0x6>(s), std::get<0x7>(s));
        std::get<0x8>(s) = _mm_add_epi32(std::get<0x8>(s), std::get<0x9>(s));
        std::get<0xA>(s) = _mm_add_epi32(std::get<0xA>(s), std::get<0xB>(s));
        std::get<0xC>(s) = _mm_add_epi32(std::get<0xC>(s), std::get<0xD>(s));
        std::get<0xE>(s) = _mm_add_epi32(std::get<0xE>(s), std::get<0xF>(s));

        __m128i l0 = _mm_slli_epi32(std::get<0x1>(s), L0);
        __m128i l1 = _mm_slli_epi32(std::get<0x3>(s), L1);
        __m128i l2 = _mm_slli_epi32(std::get<0x5>(s), L2);
        __m128i l3 = _mm_slli_epi32(std::get<0x7>(s), L3);
        __m128i l4 = _mm_slli_epi32(std::get<0x9>(s), L4);
        __m128i l5 = _mm_slli_epi32(std::get<0xB>(s), L5);
        __m128i l6 = _mm_slli_epi32(std::get<0xD>(s), L6);
        __m128i l7 = _mm_slli_epi32(std::get<0xF>(s), L7);

        std::get<0x1>(s) = _mm_srli_epi32(std::get<0x1>(s), R0);
        std::get<0x3>(s) = _mm_srli_epi32(std::get<0x3>(s), R1);
        std::get<0x5>(s) = _mm_srli_epi32(std::get<0x5>(s), R2);
        std::get<0x7>(s) = _mm_srli_epi32(std::get<0x7>(s), R3);
        std::get<0x9>(s) = _mm_srli_epi32(std::get<0x9>(s), R4);
        std::get<0xB>(s) = _mm_srli_epi32(std::get<0xB>(s), R5);
        std::get<0xD>(s) = _mm_srli_epi32(std::get<0xD>(s), R6);
        std::get<0xF>(s) = _mm_srli_epi32(std::get<0xF>(s), R7);

        std::get<0x1>(s) = _mm_or_si128(l0, std::get<0x1>(s));
        std::get<0x3>(s) = _mm_or_si128(l1, std::get<0x3>(s));
        std::get<0x5>(s) = _mm_or_si128(l2, std::get<0x5>(s));
        std::get<0x7>(s) = _mm_or_si128(l3, std::get<0x7>(s));
        std::get<0x9>(s) = _mm_or_si128(l4, std::get<0x9>(s));
        std::get<0xB>(s) = _mm_or_si128(l5, std::get<0xB>(s));
        std::get<0xD>(s) = _mm_or_si128(l6, std::get<0xD>(s));
        std::get<0xF>(s) = _mm_or_si128(l7, std::get<0xF>(s));

        std::get<0x1>(s) = _mm_xor_si128(std::get<0x0>(s), std::get<0x1>(s));
        std::get<0x3>(s) = _mm_xor_si128(std::get<0x2>(s), std::get<0x3>(s));
        std::get<0x5>(s) = _mm_xor_si128(std::get<0x4>(s), std::get<0x5>(s));
        std::get<0x7>(s) = _mm_xor_si128(std::get<0x6>(s), std::get<0x7>(s));
        std::get<0x9>(s) = _mm_xor_si128(std::get<0x8>(s), std::get<0x9>(s));
        std::get<0xB>(s) = _mm_xor_si128(std::get<0xA>(s), std::get<0xB>(s));
        std::get<0xD>(s) = _mm_xor_si128(std::get<0xC>(s), std::get<0xD>(s));
        std::get<0xF>(s) = _mm_xor_si128(std::get<0xE>(s), std::get<0xF>(s));

        permute<N>(s);
    }

    template <std::size_t N>
    static void set_key(std::array<__m128i, 2> &k, const std::array<T, 6> &par)
    {
        std::get<0>(k) = _mm_set1_epi32(static_cast<int>(
            ThreefryKBox<T, 2, N, Constants>::template key<0>(par)));
        std::get<1>(k) = _mm_set1_epi32(static_cast<int>(
            ThreefryKBox<T, 2, N, Constants>::template key<1>(par)));
    }

    template <std::size_t N>
    static void set_key(std::array<__m128i, 4> &k, const std::array<T, 8> &par)
    {
        std::get<0>(k) = _mm_set1_epi32(static_cast<int>(
            ThreefryKBox<T, 4, N, Constants>::template key<0>(par)));
        std::get<1>(k) = _mm_set1_epi32(static_cast<int>(
            ThreefryKBox<T, 4, N, Constants>::template key<1>(par)));
        std::get<2>(k) = _mm_set1_epi32(static_cast<int>(
            ThreefryKBox<T, 4, N, Constants>::template key<2>(par)));
        std::get<3>(k) = _mm_set1_epi32(static_cast<int>(
            ThreefryKBox<T, 4, N, Constants>::template key<3>(par)));
    }

    template <std::size_t N>
    static void set_key(
        std::array<__m128i, 8> &k, const std::array<T, 12> &par)
    {
        std::get<0>(k) = _mm_set1_epi32(static_cast<int>(
            ThreefryKBox<T, 8, N, Constants>::template key<0>(par)));
        std::get<1>(k) = _mm_set1_epi32(static_cast<int>(
            ThreefryKBox<T, 8, N, Constants>::template key<1>(par)));
        std::get<2>(k) = _mm_set1_epi32(static_cast<int>(
            ThreefryKBox<T, 8, N, Constants>::template key<2>(par)));
        std::get<3>(k) = _mm_set1_epi32(static_cast<int>(
            ThreefryKBox<T, 8, N, Constants>::template key<3>(par)));
        std::get<4>(k) = _mm_set1_epi32(static_cast<int>(
            ThreefryKBox<T, 8, N, Constants>::template key<4>(par)));
        std::get<5>(k) = _mm_set1_epi32(static_cast<int>(
            ThreefryKBox<T, 8, N, Constants>::template key<5>(par)));
        std::get<6>(k) = _mm_set1_epi32(static_cast<int>(
            ThreefryKBox<T, 8, N, Constants>::template key<6>(par)));
        std::get<7>(k) = _mm_set1_epi32(static_cast<int>(
            ThreefryKBox<T, 8, N, Constants>::template key<7>(par)));
    }

    template <std::size_t N>
    static void set_key(
        std::array<__m128i, 16> &k, const std::array<T, 20> &par)
    {
        std::get<0x0>(k) = _mm_set1_epi32(static_cast<int>(
            ThreefryKBox<T, 16, N, Constants>::template key<0x0>(par)));
        std::get<0x1>(k) = _mm_set1_epi32(static_cast<int>(
            ThreefryKBox<T, 16, N, Constants>::template key<0x1>(par)));
        std::get<0x2>(k) = _mm_set1_epi32(static_cast<int>(
            ThreefryKBox<T, 16, N, Constants>::template key<0x2>(par)));
        std::get<0x3>(k) = _mm_set1_epi32(static_cast<int>(
            ThreefryKBox<T, 16, N, Constants>::template key<0x3>(par)));
        std::get<0x4>(k) = _mm_set1_epi32(static_cast<int>(
            ThreefryKBox<T, 16, N, Constants>::template key<0x4>(par)));
        std::get<0x5>(k) = _mm_set1_epi32(static_cast<int>(
            ThreefryKBox<T, 16, N, Constants>::template key<0x5>(par)));
        std::get<0x6>(k) = _mm_set1_epi32(static_cast<int>(
            ThreefryKBox<T, 16, N, Constants>::template key<0x6>(par)));
        std::get<0x7>(k) = _mm_set1_epi32(static_cast<int>(
            ThreefryKBox<T, 16, N, Constants>::template key<0x7>(par)));
        std::get<0x8>(k) = _mm_set1_epi32(static_cast<int>(
            ThreefryKBox<T, 16, N, Constants>::template key<0x8>(par)));
        std::get<0x9>(k) = _mm_set1_epi32(static_cast<int>(
            ThreefryKBox<T, 16, N, Constants>::template key<0x9>(par)));
        std::get<0xA>(k) = _mm_set1_epi32(static_cast<int>(
            ThreefryKBox<T, 16, N, Constants>::template key<0xA>(par)));
        std::get<0xB>(k) = _mm_set1_epi32(static_cast<int>(
            ThreefryKBox<T, 16, N, Constants>::template key<0xB>(par)));
        std::get<0xC>(k) = _mm_set1_epi32(static_cast<int>(
            ThreefryKBox<T, 16, N, Constants>::template key<0xC>(par)));
        std::get<0xD>(k) = _mm_set1_epi32(static_cast<int>(
            ThreefryKBox<T, 16, N, Constants>::template key<0xD>(par)));
        std::get<0xE>(k) = _mm_set1_epi32(static_cast<int>(
            ThreefryKBox<T, 16, N, Constants>::template key<0xE>(par)));
        std::get<0xF>(k) = _mm_set1_epi32(static_cast<int>(
            ThreefryKBox<T, 16, N, Constants>::template key<0xF>(par)));
    }

    template <std::size_t N, std::size_t S>
    static void permute(std::array<__m128i, S> &s)
    {
        permute<N, 0>(s, std::integral_constant<bool, 0 < S / K>());
    }

    template <std::size_t, std::size_t, std::size_t S>
    static void permute(std::array<__m128i, S> &, std::false_type)
    {
    }

    template <std::size_t N, std::size_t I, std::size_t S>
    static void permute(std::array<__m128i, S> &s, std::true_type)
    {
        ThreefryPBox<__m128i, K, N, Constants>::eval(s.data() + I * K);
        permute<N, I + 1>(s, std::integral_constant<bool, I + 1 < S / K>());
    }
}; // class ThreefryGeneratorSSE2Impl32

} // namespace mckl::internal

} // namespace mckl

#ifdef MCKL_GCC
#if MCKL_GCC_VERSION >= 60000
#pragma GCC diagnostic pop
#endif
#endif

#endif // MCKL_RANDOM_INTERNAL_THREEFRY_SSE2_32_HPP
