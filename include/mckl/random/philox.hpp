//============================================================================
// MCKL/include/mckl/random/philox.hpp
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

#ifndef MCKL_RANDOM_PHILOX_HPP
#define MCKL_RANDOM_PHILOX_HPP

#include <mckl/random/internal/common.hpp>
#include <mckl/random/counter.hpp>
#include <mckl/random/threefry.hpp>

#ifdef MCKL_MSVC
#include <intrin.h>
#else
#if MCKL_HAS_SSE2
#include <emmintrin.h>
#endif
#if MCKL_HAS_AVX2
#include <immintrin.h>
#endif
#endif // MCKL_MSVC

/// \brief PhiloxGenerator default rounds
/// \ingroup Config
#ifndef MCKL_PHILOX_ROUNDS
#define MCKL_PHILOX_ROUNDS 10
#endif

namespace mckl
{

namespace internal
{

template <typename, std::size_t, int>
class PhiloxConstantsImpl;

template <typename T>
class PhiloxConstantsImpl<T, 2, 32>
{
    public:
    static constexpr T weyl[1] = {UINT32_C(0x9E3779B9)};

    static constexpr T multiplier[1] = {UINT32_C(0xD256D193)};
}; // class PhiloxConstantsImpl

template <typename T>
class PhiloxConstantsImpl<T, 4, 32>
{
    public:
    static constexpr T weyl[2] = {UINT32_C(0x9E3779B9), UINT32_C(0xBB67AE85)};

    static constexpr T multiplier[2] = {
        UINT32_C(0xCD9E8D57), UINT32_C(0xD2511F53)};
}; // class PhiloxConstantsImpl

template <typename T>
class PhiloxConstantsImpl<T, 2, 64>
{
    public:
    static constexpr T weyl[1] = {UINT64_C(0x9E3779B97F4A7C15)};

    static constexpr T multiplier[1] = {UINT64_C(0xD2B74407B1CE6E93)};
}; // class PhiloxConstantsImpl

template <typename T>
class PhiloxConstantsImpl<T, 4, 64>
{
    public:
    static constexpr T weyl[2] = {
        UINT64_C(0x9E3779B97F4A7C15), UINT64_C(0xBB67AE8584CAA73B)};

    static constexpr T multiplier[2] = {
        UINT64_C(0xCA5A826395121157), UINT64_C(0xD2E7470EE14C6C93)};
}; // class PhiloxConstantsImpl

} // namespace mckl::internal

/// \brief Default Philox constants
/// \ingroup Philox
template <typename T, std::size_t K>
using PhiloxConstants =
    internal::PhiloxConstantsImpl<T, K, std::numeric_limits<T>::digits>;

namespace internal
{

template <typename T, int = std::numeric_limits<T>::digits>
class PhiloxHiLo;

template <typename T>
class PhiloxHiLo<T, 32>
{
    public:
    static void eval(T a, T b, T &hi, T &lo)
    {
        union {
            std::uint64_t prod;
            std::array<T, 2> result;
        } buf;

        buf.prod =
            static_cast<std::uint64_t>(a) * static_cast<std::uint64_t>(b);
#if MCKL_HAS_X86 // little endian
        hi = std::get<1>(buf.result);
        lo = std::get<0>(buf.result);
#else  // MCKL_HAS_X86
        hi = static_cast<T>(buf.prod >> 32);
        lo = static_cast<T>(buf.prod);
#endif // MCKL_HAS_X86
    }
}; // class PhiloxHiLo

template <typename T>
class PhiloxHiLo<T, 64>
{
    public:
    static void eval(T a, T b, T &hi, T &lo)
    {
#if MCKL_HAS_INT128

#ifdef MCKL_GCC
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpedantic"
#endif // MCKL_GCC
        union {
            unsigned MCKL_INT128 prod;
            std::array<T, 2> result;
        } buf;

        buf.prod = static_cast<unsigned MCKL_INT128>(a) *
            static_cast<unsigned MCKL_INT128>(b);
#ifdef MCKL_GCC
#pragma GCC diagnostic pop
#endif // MCKL_GCC

#if MCKL_HAS_X86 // littel endia
        hi = std::get<1>(buf.result);
        lo = std::get<0>(buf.result);
#else  // MCKL_HAS_X86
        hi = static_cast<T>(buf.prod >> 64);
        lo = static_cast<T>(buf.prod);
#endif // MCKL_HAS_X86

#elif defined(MCKL_MSVC)

        unsigned __int64 Multiplier =
            static_cast<unsigned __int64>(multiplier::value);
        unsigned __int64 Multiplicand = static_cast<unsigned __int64>(b);
        unsigned __int64 hi_tmp = 0;
        lo = static_cast<T>(_umul128(Multiplier, Multiplicand, &hi_tmp));
        hi = static_cast<T>(hi_tmp);

#else // MCKL_HAS_INT128

        const T a = multiplier::value;
        const T lomask = (const_one<T>() << 32) - 1;
        const T ahi = a >> 32;
        const T alo = a & lomask;
        const T bhi = b >> 32;
        const T blo = b & lomask;
        const T ahbl = ahi * blo;
        const T albh = alo * bhi;
        const T ahbl_albh = ((ahbl & lomask) + (albh & lomask));

        lo = a * b;
        hi = ahi * bhi + (ahbl >> 32) + (albh >> 32);
        hi += ahbl_albh >> 32;
        hi += ((lo >> 32) < (ahbl_albh & lomask));
        std::array<T, 2> hilo = {{lo, hi}};

        return hilo;

#endif // MCKL_HAS_INT128
    }
}; // class PhiloxHiLo

template <typename T, std::size_t K, std::size_t N, typename Constants,
    bool = (N > 1)>
class PhiloxKBox
{
    public:
    static void eval(std::array<T, K / 2> &) {}
}; // class PhiloxKBox

template <typename T, std::size_t K, std::size_t N, typename Constants>
class PhiloxKBox<T, K, N, Constants, true>
{
    public:
    static void eval(std::array<T, K / 2> &par)
    {
        eval<0>(par, std::integral_constant<bool, 0 < K / 2>());
    }

    private:
    template <std::size_t>
    static void eval(std::array<T, K / 2> &, std::false_type)
    {
    }

    template <std::size_t I>
    static void eval(std::array<T, K / 2> &par, std::true_type)
    {
        static constexpr T w = Constants::weyl[I];

        std::get<I>(par) += w;
        eval<I + 1>(par, std::integral_constant<bool, I + 1 < K / 2>());
    }
}; // class PhiloxKBox

template <typename T, std::size_t K, std::size_t N, typename, bool = (N > 0)>
class PhiloxSBox
{
    public:
    static void eval(std::array<T, K> &, const std::array<T, K / 2> &) {}
}; // class PhiloxSBox

template <typename T, std::size_t K, std::size_t N, typename Constants>
class PhiloxSBox<T, K, N, Constants, true>
{
    public:
    static void eval(std::array<T, K> &state, const std::array<T, K / 2> &par)
    {
        eval<0>(state, par, std::integral_constant<bool, 1 < K>());
    }

    private:
    template <std::size_t>
    static void eval(
        std::array<T, K> &, const std::array<T, K / 2> &, std::false_type)
    {
    }

    template <std::size_t I>
    static void eval(std::array<T, K> &state, const std::array<T, K / 2> &par,
        std::true_type)
    {
        static constexpr T m = Constants::multiplier[I / 2];

        T x = std::get<I + 1>(state) ^ std::get<I / 2>(par);
        PhiloxHiLo<T>::eval(
            std::get<I>(state), m, std::get<I>(state), std::get<I + 1>(state));
        std::get<I>(state) ^= x;
        eval<I + 2>(state, par, std::integral_constant<bool, I + 3 < K>());
    }
}; // class PhiloxSBox

template <typename T, std::size_t K, std::size_t N, typename, bool = (N > 0)>
class PhiloxPBox
{
    public:
    static void eval(std::array<T, K> &) {}
}; // class PhiloxPBox

template <typename T, std::size_t K, std::size_t N, typename Constants>
class PhiloxPBox<T, K, N, Constants, true>
{
    public:
    static void eval(std::array<T, K> &state)
    {
        std::array<T, K> tmp;
        eval<0>(state, tmp, std::integral_constant<bool, 0 < K>());
        state = tmp;
    }

    private:
    template <std::size_t>
    static void eval(
        const std::array<T, K> &, std::array<T, K> &, std::false_type)
    {
    }

    template <std::size_t I>
    static void eval(
        const std::array<T, K> &state, std::array<T, K> &tmp, std::true_type)
    {
        static constexpr std::size_t P =
            ThreefryConstants<T, K>::permute[K - I - 1];
        static constexpr std::size_t J = K - P - 1;

        std::get<I>(tmp) = std::get<J>(state);
        eval<I + 1>(state, tmp, std::integral_constant<bool, I + 1 < K>());
    }
}; // class PhiloxPBox

template <typename T, std::size_t N>
class PhiloxPBox<T, 2, N, PhiloxConstants<T, 2>, true>
{
    public:
    static void eval(std::array<T, 2> &) {}
}; // class PhiloxPBox

template <typename T, std::size_t N>
class PhiloxPBox<T, 4, N, PhiloxConstants<T, 4>, true>
{
    public:
    static void eval(std::array<T, 4> &state)
    {
        std::swap(std::get<0>(state), std::get<2>(state));
    }
}; // class PhiloxPBox

template <typename T, std::size_t K, std::size_t Rounds, typename Constants>
class PhiloxGeneratorGenericImpl
{
    public:
    static constexpr std::size_t blocks = 8;

    static void eval(std::array<T, K> &state, const std::array<T, K / 2> &key)
    {
        std::array<T, K / 2> par = key;
        eval<0>(state, par, std::integral_constant<bool, 0 <= Rounds>());
    }

    static void eval(std::array<std::array<T, K>, blocks> &state,
        const std::array<T, K / 2> &key)
    {
        std::array<T, K / 2> par = key;
        eval<0>(state, par, std::integral_constant<bool, 0 <= Rounds>());
    }

    private:
    template <std::size_t>
    static void eval(
        std::array<T, K> &, std::array<T, K / 2> &, std::false_type)
    {
    }

    template <std::size_t N>
    static void eval(
        std::array<T, K> &state, std::array<T, K / 2> &par, std::true_type)
    {
        PhiloxKBox<T, K, N, Constants>::eval(par);
        PhiloxPBox<T, K, N, Constants>::eval(state);
        PhiloxSBox<T, K, N, Constants>::eval(state, par);
        eval<N + 1>(
            state, par, std::integral_constant<bool, N + 1 <= Rounds>());
    }

    template <std::size_t>
    static void eval(std::array<std::array<T, K>, blocks> &,
        std::array<T, K / 2> &, std::false_type)
    {
    }

    template <std::size_t N>
    static void eval(std::array<std::array<T, K>, blocks> &state,
        std::array<T, K / 2> &par, std::true_type)
    {
        PhiloxKBox<T, K, N, Constants>::eval(par);
        pbox<N, 0>(state, std::integral_constant<bool, 0 < blocks>());
        sbox<N, 0>(state, par, std::integral_constant<bool, 0 < blocks>());
        eval<N + 1>(
            state, par, std::integral_constant<bool, N + 1 <= Rounds>());
    }

    template <std::size_t, std::size_t>
    static void sbox(std::array<std::array<T, K>, blocks> &,
        std::array<T, K / 2> &, std::false_type)
    {
    }

    template <std::size_t N, std::size_t I>
    static void sbox(std::array<std::array<T, K>, blocks> &state,
        std::array<T, K / 2> &par, std::true_type)
    {
        PhiloxSBox<T, K, N, Constants>::eval(std::get<I>(state), par);
        sbox<N, I + 1>(
            state, par, std::integral_constant<bool, I + 1 < blocks>());
    }

    template <std::size_t, std::size_t>
    static void pbox(std::array<std::array<T, K>, blocks> &, std::false_type)
    {
    }

    template <std::size_t N, std::size_t I>
    static void pbox(
        std::array<std::array<T, K>, blocks> &state, std::true_type)
    {
        PhiloxPBox<T, K, N, Constants>::eval(std::get<I>(state));
        pbox<N, I + 1>(state, std::integral_constant<bool, I + 1 < blocks>());
    }
}; // class PhiloxGeneratorGenericImpl

template <typename T, std::size_t K, std::size_t Rounds, typename Constants>
class PhiloxGeneratorImpl
    : public PhiloxGeneratorGenericImpl<T, K, Rounds, Constants>
{
}; // class PhiloxGeneratorImpl

#if MCKL_HAS_SSE2

template <typename T, std::size_t K, std::size_t Rounds, typename Constants,
    typename Derived, int = std::numeric_limits<T>::digits,
    bool = 128 % (sizeof(T) * K) == 0 && (K % 8 == 0 || K < 8)>
class PhiloxGeneratorSSE2Impl
    : public PhiloxGeneratorGenericImpl<T, K, Rounds, Constants>
{
}; // class PhiloxGeneratorSSE2Impl

template <typename T, std::size_t K, std::size_t Rounds, typename Constants,
    typename Derived>
class PhiloxGeneratorSSE2Impl<T, K, Rounds, Constants, Derived, 32, true>
{
    public:
    static constexpr std::size_t blocks = 128 / (sizeof(T) * K);

    static void eval(std::array<T, K> &state, const std::array<T, K / 2> &key)
    {
        PhiloxGeneratorGenericImpl<T, K, Rounds, Constants>::eval(state, key);
    }

    static void eval(std::array<std::array<T, K>, blocks> &state,
        const std::array<T, K / 2> &key)
    {
        std::array<__m128i, M_> k;
        std::array<__m128i, M_> w;
        std::array<__m128i, M_> m;
        std::array<__m128i, 8> s;
        std::array<__m128i, 8> t;

        std::memcpy(s.data(), state.data(), 128);
        init_k<0>(k, key, std::integral_constant<bool, 0 < M_>());
        init_w<0>(w, std::integral_constant<bool, 0 < M_>());
        init_m<0>(m, std::integral_constant<bool, 0 < M_>());

        std::get<0>(t) = _mm_srli_epi64(std::get<0>(s), 32);
        std::get<1>(t) = _mm_srli_epi64(std::get<1>(s), 32);
        std::get<2>(t) = _mm_srli_epi64(std::get<2>(s), 32);
        std::get<3>(t) = _mm_srli_epi64(std::get<3>(s), 32);
        std::get<4>(t) = _mm_srli_epi64(std::get<4>(s), 32);
        std::get<5>(t) = _mm_srli_epi64(std::get<5>(s), 32);
        std::get<6>(t) = _mm_srli_epi64(std::get<6>(s), 32);
        std::get<7>(t) = _mm_srli_epi64(std::get<7>(s), 32);

        Derived::pbox(s, t); // Round 1
        sbox(s, t, k, m);    // Round 1
        kbox(k, w);          // Round 2
        Derived::pbox(s, t); // Round 2
        sbox(s, t, k, m);    // Round 2
        kbox(k, w);          // Round 3
        Derived::pbox(s, t); // Round 3
        sbox(s, t, k, m);    // Round 3
        kbox(k, w);          // Round 4
        Derived::pbox(s, t); // Round 4
        sbox(s, t, k, m);    // Round 4
        kbox(k, w);          // Round 5
        Derived::pbox(s, t); // Round 5
        sbox(s, t, k, m);    // Round 5
        kbox(k, w);          // Round 6
        Derived::pbox(s, t); // Round 6
        sbox(s, t, k, m);    // Round 6
        kbox(k, w);          // Round 7
        Derived::pbox(s, t); // Round 7
        sbox(s, t, k, m);    // Round 7
        kbox(k, w);          // Round 8
        Derived::pbox(s, t); // Round 8
        sbox(s, t, k, m);    // Round 8
        kbox(k, w);          // Round 9
        Derived::pbox(s, t); // Round 9
        sbox(s, t, k, m);    // Round 9
        kbox(k, w);          // Round 10
        Derived::pbox(s, t); // Round 10
        sbox(s, t, k, m);    // Round 10

        eval<11>(s, t, k, w, m, std::integral_constant<bool, 11 <= Rounds>());

        std::get<0>(s) = _mm_shuffle_epi32(std::get<0>(s), 0x87); // 2 0 1 3
        std::get<1>(s) = _mm_shuffle_epi32(std::get<1>(s), 0x87); // 2 0 1 3
        std::get<2>(s) = _mm_shuffle_epi32(std::get<2>(s), 0x87); // 2 0 1 3
        std::get<3>(s) = _mm_shuffle_epi32(std::get<3>(s), 0x87); // 2 0 1 3
        std::get<4>(s) = _mm_shuffle_epi32(std::get<4>(s), 0x87); // 2 0 1 3
        std::get<5>(s) = _mm_shuffle_epi32(std::get<5>(s), 0x87); // 2 0 1 3
        std::get<6>(s) = _mm_shuffle_epi32(std::get<6>(s), 0x87); // 2 0 1 3
        std::get<7>(s) = _mm_shuffle_epi32(std::get<7>(s), 0x87); // 2 0 1 3

        std::get<0>(t) = _mm_shuffle_epi32(std::get<0>(t), 0x87); // 2 0 1 3
        std::get<1>(t) = _mm_shuffle_epi32(std::get<1>(t), 0x87); // 2 0 1 3
        std::get<2>(t) = _mm_shuffle_epi32(std::get<2>(t), 0x87); // 2 0 1 3
        std::get<3>(t) = _mm_shuffle_epi32(std::get<3>(t), 0x87); // 2 0 1 3
        std::get<4>(t) = _mm_shuffle_epi32(std::get<4>(t), 0x87); // 2 0 1 3
        std::get<5>(t) = _mm_shuffle_epi32(std::get<5>(t), 0x87); // 2 0 1 3
        std::get<6>(t) = _mm_shuffle_epi32(std::get<6>(t), 0x87); // 2 0 1 3
        std::get<7>(t) = _mm_shuffle_epi32(std::get<7>(t), 0x87); // 2 0 1 3

        std::get<0>(s) = _mm_unpackhi_epi32(std::get<0>(s), std::get<0>(t));
        std::get<1>(s) = _mm_unpackhi_epi32(std::get<1>(s), std::get<1>(t));
        std::get<2>(s) = _mm_unpackhi_epi32(std::get<2>(s), std::get<2>(t));
        std::get<3>(s) = _mm_unpackhi_epi32(std::get<3>(s), std::get<3>(t));
        std::get<4>(s) = _mm_unpackhi_epi32(std::get<4>(s), std::get<4>(t));
        std::get<5>(s) = _mm_unpackhi_epi32(std::get<5>(s), std::get<5>(t));
        std::get<6>(s) = _mm_unpackhi_epi32(std::get<6>(s), std::get<6>(t));
        std::get<7>(s) = _mm_unpackhi_epi32(std::get<7>(s), std::get<7>(t));

        std::memcpy(state.data(), s.data(), 128);
    }

    private:
    static constexpr std::size_t M_ = K < 8 ? 1 : K / 8;

    template <std::size_t>
    static void eval(std::array<__m128i, 8> &, std::array<__m128i, 8> &,
        std::array<__m128i, M_> &, const std::array<__m128i, M_> &,
        const std::array<__m128i, M_> &, std::false_type)
    {
    }

    template <std::size_t N>
    static void eval(std::array<__m128i, 8> &s, std::array<__m128i, 8> &t,
        std::array<__m128i, M_> &k, const std::array<__m128i, M_> &w,
        const std::array<__m128i, M_> &m, std::true_type)
    {
        kbox(k, w);
        Derived::pbox(s, t);
        sbox(s, t, k, m);
        eval<N + 1>(
            s, t, k, w, m, std::integral_constant<bool, N + 1 <= Rounds>());
    }

    template <std::size_t>
    static void init_k(std::array<__m128i, M_> &, const std::array<T, K / 2> &,
        std::false_type)
    {
    }

    template <std::size_t I>
    static void init_k(std::array<__m128i, M_> &k,
        const std::array<T, K / 2> &key, std::true_type)
    {
        static constexpr std::size_t i0 = (I * 2 + 0) % (K / 2);
        static constexpr std::size_t i1 = (I * 2 + 1) % (K / 2);

        const int e0 = static_cast<int>(std::get<i0>(key));
        const int e1 = static_cast<int>(std::get<i1>(key));

        std::get<I>(k) = _mm_set_epi32(0, e1, 0, e0);
        init_k<I + 1>(k, key, std::integral_constant<bool, I + 1 < M_>());
    }

    template <std::size_t>
    static void init_w(std::array<__m128i, M_> &, std::false_type)
    {
    }

    template <std::size_t I>
    static void init_w(std::array<__m128i, M_> &w, std::true_type)
    {
        static constexpr std::size_t i0 = (I * 2 + 0) % (K / 2);
        static constexpr std::size_t i1 = (I * 2 + 1) % (K / 2);

        static constexpr int e0 = static_cast<int>(Constants::weyl[i0]);
        static constexpr int e1 = static_cast<int>(Constants::weyl[i1]);

        std::get<I>(w) = _mm_set_epi32(0, e1, 0, e0);
        init_w<I + 1>(w, std::integral_constant<bool, I + 1 < M_>());
    }

    template <std::size_t>
    static void init_m(std::array<__m128i, M_> &, std::false_type)
    {
    }

    template <std::size_t I>
    static void init_m(std::array<__m128i, M_> &m, std::true_type)
    {
        static constexpr std::size_t i0 = (I * 2 + 0) % (K / 2);
        static constexpr std::size_t i1 = (I * 2 + 1) % (K / 2);

        static constexpr int e0 = static_cast<int>(Constants::multiplier[i0]);
        static constexpr int e1 = static_cast<int>(Constants::multiplier[i1]);

        std::get<I>(m) = _mm_set_epi32(0, e1, 0, e0);
        init_m<I + 1>(m, std::integral_constant<bool, I + 1 < M_>());
    }

    static void kbox(
        std::array<__m128i, M_> &k, const std::array<__m128i, M_> &w)
    {
        kbox<0>(k, w, std::integral_constant<bool, 0 < M_>());
    }

    template <std::size_t>
    static void kbox(std::array<__m128i, M_> &,
        const std::array<__m128i, M_> &, std::false_type)
    {
    }

    template <std::size_t I>
    static void kbox(std::array<__m128i, M_> &k,
        const std::array<__m128i, M_> &w, std::true_type)
    {
        std::get<I>(k) = _mm_add_epi32(std::get<I>(k), std::get<I>(w));
        kbox<I + 1>(k, w, std::integral_constant<bool, I + 1 < M_>());
    }

    static void sbox(std::array<__m128i, 8> &s, std::array<__m128i, 8> &t,
        const std::array<__m128i, M_> &k, const std::array<__m128i, M_> &m)
    {
        __m128i x0 = _mm_xor_si128(std::get<0>(t), std::get<0 % M_>(k));
        __m128i x1 = _mm_xor_si128(std::get<1>(t), std::get<1 % M_>(k));
        __m128i x2 = _mm_xor_si128(std::get<2>(t), std::get<2 % M_>(k));
        __m128i x3 = _mm_xor_si128(std::get<3>(t), std::get<3 % M_>(k));
        __m128i x4 = _mm_xor_si128(std::get<4>(t), std::get<4 % M_>(k));
        __m128i x5 = _mm_xor_si128(std::get<5>(t), std::get<5 % M_>(k));
        __m128i x6 = _mm_xor_si128(std::get<6>(t), std::get<6 % M_>(k));
        __m128i x7 = _mm_xor_si128(std::get<7>(t), std::get<7 % M_>(k));

        std::get<0>(t) = _mm_mul_epu32(std::get<0>(s), std::get<0 % M_>(m));
        std::get<1>(t) = _mm_mul_epu32(std::get<1>(s), std::get<1 % M_>(m));
        std::get<2>(t) = _mm_mul_epu32(std::get<2>(s), std::get<2 % M_>(m));
        std::get<3>(t) = _mm_mul_epu32(std::get<3>(s), std::get<3 % M_>(m));
        std::get<4>(t) = _mm_mul_epu32(std::get<4>(s), std::get<4 % M_>(m));
        std::get<5>(t) = _mm_mul_epu32(std::get<5>(s), std::get<5 % M_>(m));
        std::get<6>(t) = _mm_mul_epu32(std::get<6>(s), std::get<6 % M_>(m));
        std::get<7>(t) = _mm_mul_epu32(std::get<7>(s), std::get<7 % M_>(m));

        std::get<0>(s) = _mm_shuffle_epi32(std::get<0>(t), 0xB1); // 2 3 0 1
        std::get<1>(s) = _mm_shuffle_epi32(std::get<1>(t), 0xB1); // 2 3 0 1
        std::get<2>(s) = _mm_shuffle_epi32(std::get<2>(t), 0xB1); // 2 3 0 1
        std::get<3>(s) = _mm_shuffle_epi32(std::get<3>(t), 0xB1); // 2 3 0 1
        std::get<4>(s) = _mm_shuffle_epi32(std::get<4>(t), 0xB1); // 2 3 0 1
        std::get<5>(s) = _mm_shuffle_epi32(std::get<5>(t), 0xB1); // 2 3 0 1
        std::get<6>(s) = _mm_shuffle_epi32(std::get<6>(t), 0xB1); // 2 3 0 1
        std::get<7>(s) = _mm_shuffle_epi32(std::get<7>(t), 0xB1); // 2 3 0 1

        std::get<0>(s) = _mm_xor_si128(std::get<0>(s), x0);
        std::get<1>(s) = _mm_xor_si128(std::get<1>(s), x1);
        std::get<2>(s) = _mm_xor_si128(std::get<2>(s), x2);
        std::get<3>(s) = _mm_xor_si128(std::get<3>(s), x3);
        std::get<4>(s) = _mm_xor_si128(std::get<4>(s), x4);
        std::get<5>(s) = _mm_xor_si128(std::get<5>(s), x5);
        std::get<6>(s) = _mm_xor_si128(std::get<6>(s), x6);
        std::get<7>(s) = _mm_xor_si128(std::get<7>(s), x7);
    }

    public:
    static void pbox(std::array<__m128i, 8> &, std::array<__m128i, 8> &) {}
}; // class PhiloxGeneratorSSE2Impl

#endif // MCKL_HAS_SSE2

#if MCKL_HAS_AVX2

template <typename T, std::size_t K, std::size_t Rounds, typename Constants,
    typename Derived, int = std::numeric_limits<T>::digits,
    bool = 256 % (sizeof(T) * K) == 0 && (K % 8 == 0 || K < 8)>
class PhiloxGeneratorAVX2Impl
    : public PhiloxGeneratorGenericImpl<T, K, Rounds, Constants>
{
}; // class PhiloxGeneratorAVX2Impl

template <typename T, std::size_t K, std::size_t Rounds, typename Constants,
    typename Derived>
class PhiloxGeneratorAVX2Impl<T, K, Rounds, Constants, Derived, 32, true>
{
    public:
    static constexpr std::size_t blocks = 256 / (sizeof(T) * K);

    static void eval(std::array<T, K> &state, const std::array<T, K / 2> &key)
    {
        PhiloxGeneratorGenericImpl<T, K, Rounds, Constants>::eval(state, key);
    }

    static void eval(std::array<std::array<T, K>, blocks> &state,
        const std::array<T, K / 2> &key)
    {
        std::array<__m256i, M_> k;
        std::array<__m256i, M_> w;
        std::array<__m256i, M_> m;
        std::array<__m256i, 8> s;
        std::array<__m256i, 8> t;

        std::memcpy(s.data(), state.data(), 256);
        init_k<0>(k, key, std::integral_constant<bool, 0 < M_>());
        init_w<0>(w, std::integral_constant<bool, 0 < M_>());
        init_m<0>(m, std::integral_constant<bool, 0 < M_>());

        std::get<0>(t) = _mm256_srli_epi64(std::get<0>(s), 32);
        std::get<1>(t) = _mm256_srli_epi64(std::get<1>(s), 32);
        std::get<2>(t) = _mm256_srli_epi64(std::get<2>(s), 32);
        std::get<3>(t) = _mm256_srli_epi64(std::get<3>(s), 32);
        std::get<4>(t) = _mm256_srli_epi64(std::get<4>(s), 32);
        std::get<5>(t) = _mm256_srli_epi64(std::get<5>(s), 32);
        std::get<6>(t) = _mm256_srli_epi64(std::get<6>(s), 32);
        std::get<7>(t) = _mm256_srli_epi64(std::get<7>(s), 32);

        Derived::pbox(s, t); // Round 1
        sbox(s, t, k, m);    // Round 1
        kbox(k, w);          // Round 2
        Derived::pbox(s, t); // Round 2
        sbox(s, t, k, m);    // Round 2
        kbox(k, w);          // Round 3
        Derived::pbox(s, t); // Round 3
        sbox(s, t, k, m);    // Round 3
        kbox(k, w);          // Round 4
        Derived::pbox(s, t); // Round 4
        sbox(s, t, k, m);    // Round 4
        kbox(k, w);          // Round 5
        Derived::pbox(s, t); // Round 5
        sbox(s, t, k, m);    // Round 5
        kbox(k, w);          // Round 6
        Derived::pbox(s, t); // Round 6
        sbox(s, t, k, m);    // Round 6
        kbox(k, w);          // Round 7
        Derived::pbox(s, t); // Round 7
        sbox(s, t, k, m);    // Round 7
        kbox(k, w);          // Round 8
        Derived::pbox(s, t); // Round 8
        sbox(s, t, k, m);    // Round 8
        kbox(k, w);          // Round 9
        Derived::pbox(s, t); // Round 9
        sbox(s, t, k, m);    // Round 9
        kbox(k, w);          // Round 10
        Derived::pbox(s, t); // Round 10
        sbox(s, t, k, m);    // Round 10

        eval<11>(s, t, k, w, m, std::integral_constant<bool, 11 <= Rounds>());

        std::get<0>(s) = _mm256_shuffle_epi32(std::get<0>(s), 0x87); // 2 0 1 3
        std::get<1>(s) = _mm256_shuffle_epi32(std::get<1>(s), 0x87); // 2 0 1 3
        std::get<2>(s) = _mm256_shuffle_epi32(std::get<2>(s), 0x87); // 2 0 1 3
        std::get<3>(s) = _mm256_shuffle_epi32(std::get<3>(s), 0x87); // 2 0 1 3
        std::get<4>(s) = _mm256_shuffle_epi32(std::get<4>(s), 0x87); // 2 0 1 3
        std::get<5>(s) = _mm256_shuffle_epi32(std::get<5>(s), 0x87); // 2 0 1 3
        std::get<6>(s) = _mm256_shuffle_epi32(std::get<6>(s), 0x87); // 2 0 1 3
        std::get<7>(s) = _mm256_shuffle_epi32(std::get<7>(s), 0x87); // 2 0 1 3

        std::get<0>(t) = _mm256_shuffle_epi32(std::get<0>(t), 0x87); // 2 0 1 3
        std::get<1>(t) = _mm256_shuffle_epi32(std::get<1>(t), 0x87); // 2 0 1 3
        std::get<2>(t) = _mm256_shuffle_epi32(std::get<2>(t), 0x87); // 2 0 1 3
        std::get<3>(t) = _mm256_shuffle_epi32(std::get<3>(t), 0x87); // 2 0 1 3
        std::get<4>(t) = _mm256_shuffle_epi32(std::get<4>(t), 0x87); // 2 0 1 3
        std::get<5>(t) = _mm256_shuffle_epi32(std::get<5>(t), 0x87); // 2 0 1 3
        std::get<6>(t) = _mm256_shuffle_epi32(std::get<6>(t), 0x87); // 2 0 1 3
        std::get<7>(t) = _mm256_shuffle_epi32(std::get<7>(t), 0x87); // 2 0 1 3

        std::get<0>(s) = _mm256_unpackhi_epi32(std::get<0>(s), std::get<0>(t));
        std::get<1>(s) = _mm256_unpackhi_epi32(std::get<1>(s), std::get<1>(t));
        std::get<2>(s) = _mm256_unpackhi_epi32(std::get<2>(s), std::get<2>(t));
        std::get<3>(s) = _mm256_unpackhi_epi32(std::get<3>(s), std::get<3>(t));
        std::get<4>(s) = _mm256_unpackhi_epi32(std::get<4>(s), std::get<4>(t));
        std::get<5>(s) = _mm256_unpackhi_epi32(std::get<5>(s), std::get<5>(t));
        std::get<6>(s) = _mm256_unpackhi_epi32(std::get<6>(s), std::get<6>(t));
        std::get<7>(s) = _mm256_unpackhi_epi32(std::get<7>(s), std::get<7>(t));

        std::memcpy(state.data(), s.data(), 256);
    }

    private:
    static constexpr std::size_t M_ = K < 8 ? 1 : K / 8;

    template <std::size_t>
    static void eval(std::array<__m256i, 8> &, std::array<__m256i, 8> &,
        std::array<__m256i, M_> &, const std::array<__m256i, M_> &,
        const std::array<__m256i, M_> &, std::false_type)
    {
    }

    template <std::size_t N>
    static void eval(std::array<__m256i, 8> &s, std::array<__m256i, 8> &t,
        std::array<__m256i, M_> &k, const std::array<__m256i, M_> &w,
        const std::array<__m256i, M_> &m, std::true_type)
    {
        kbox(k, w);
        Derived::pbox(s, t);
        sbox(s, t, k, m);
        eval<N + 1>(
            s, t, k, w, m, std::integral_constant<bool, N + 1 <= Rounds>());
    }

    template <std::size_t>
    static void init_k(std::array<__m256i, M_> &, const std::array<T, K / 2> &,
        std::false_type)
    {
    }

    template <std::size_t I>
    static void init_k(std::array<__m256i, M_> &k,
        const std::array<T, K / 2> &key, std::true_type)
    {
        static constexpr std::size_t i0 = (I * 4 + 0) % (K / 2);
        static constexpr std::size_t i1 = (I * 4 + 1) % (K / 2);
        static constexpr std::size_t i2 = (I * 4 + 2) % (K / 2);
        static constexpr std::size_t i3 = (I * 4 + 3) % (K / 2);

        const int e0 = static_cast<int>(std::get<i0>(key));
        const int e1 = static_cast<int>(std::get<i1>(key));
        const int e2 = static_cast<int>(std::get<i2>(key));
        const int e3 = static_cast<int>(std::get<i3>(key));

        std::get<I>(k) = _mm256_set_epi32(0, e3, 0, e2, 0, e1, 0, e0);
        init_k<I + 1>(k, key, std::integral_constant<bool, I + 1 < M_>());
    }

    template <std::size_t>
    static void init_w(std::array<__m256i, M_> &, std::false_type)
    {
    }

    template <std::size_t I>
    static void init_w(std::array<__m256i, M_> &w, std::true_type)
    {
        static constexpr std::size_t i0 = (I * 4 + 0) % (K / 2);
        static constexpr std::size_t i1 = (I * 4 + 1) % (K / 2);
        static constexpr std::size_t i2 = (I * 4 + 2) % (K / 2);
        static constexpr std::size_t i3 = (I * 4 + 3) % (K / 2);

        static constexpr int e0 = static_cast<int>(Constants::weyl[i0]);
        static constexpr int e1 = static_cast<int>(Constants::weyl[i1]);
        static constexpr int e2 = static_cast<int>(Constants::weyl[i2]);
        static constexpr int e3 = static_cast<int>(Constants::weyl[i3]);

        std::get<I>(w) = _mm256_set_epi32(0, e3, 0, e2, 0, e1, 0, e0);
        init_w<I + 1>(w, std::integral_constant<bool, I + 1 < M_>());
    }

    template <std::size_t>
    static void init_m(std::array<__m256i, M_> &, std::false_type)
    {
    }

    template <std::size_t I>
    static void init_m(std::array<__m256i, M_> &m, std::true_type)
    {
        static constexpr std::size_t i0 = (I * 4 + 0) % (K / 2);
        static constexpr std::size_t i1 = (I * 4 + 1) % (K / 2);
        static constexpr std::size_t i2 = (I * 4 + 2) % (K / 2);
        static constexpr std::size_t i3 = (I * 4 + 3) % (K / 2);

        static constexpr int e0 = static_cast<int>(Constants::multiplier[i0]);
        static constexpr int e1 = static_cast<int>(Constants::multiplier[i1]);
        static constexpr int e2 = static_cast<int>(Constants::multiplier[i2]);
        static constexpr int e3 = static_cast<int>(Constants::multiplier[i3]);

        std::get<I>(m) = _mm256_set_epi32(0, e3, 0, e2, 0, e1, 0, e0);
        init_m<I + 1>(m, std::integral_constant<bool, I + 1 < M_>());
    }

    static void kbox(
        std::array<__m256i, M_> &k, const std::array<__m256i, M_> &w)
    {
        kbox<0>(k, w, std::integral_constant<bool, 0 < M_>());
    }

    template <std::size_t>
    static void kbox(std::array<__m256i, M_> &,
        const std::array<__m256i, M_> &, std::false_type)
    {
    }

    template <std::size_t I>
    static void kbox(std::array<__m256i, M_> &k,
        const std::array<__m256i, M_> &w, std::true_type)
    {
        std::get<I>(k) = _mm256_add_epi32(std::get<I>(k), std::get<I>(w));
        kbox<I + 1>(k, w, std::integral_constant<bool, I + 1 < M_>());
    }

    static void sbox(std::array<__m256i, 8> &s, std::array<__m256i, 8> &t,
        const std::array<__m256i, M_> &k, const std::array<__m256i, M_> &m)
    {
        __m256i x0 = _mm256_xor_si256(std::get<0>(t), std::get<0 % M_>(k));
        __m256i x1 = _mm256_xor_si256(std::get<1>(t), std::get<1 % M_>(k));
        __m256i x2 = _mm256_xor_si256(std::get<2>(t), std::get<2 % M_>(k));
        __m256i x3 = _mm256_xor_si256(std::get<3>(t), std::get<3 % M_>(k));
        __m256i x4 = _mm256_xor_si256(std::get<4>(t), std::get<4 % M_>(k));
        __m256i x5 = _mm256_xor_si256(std::get<5>(t), std::get<5 % M_>(k));
        __m256i x6 = _mm256_xor_si256(std::get<6>(t), std::get<6 % M_>(k));
        __m256i x7 = _mm256_xor_si256(std::get<7>(t), std::get<7 % M_>(k));

        std::get<0>(t) = _mm256_mul_epu32(std::get<0>(s), std::get<0 % M_>(m));
        std::get<1>(t) = _mm256_mul_epu32(std::get<1>(s), std::get<1 % M_>(m));
        std::get<2>(t) = _mm256_mul_epu32(std::get<2>(s), std::get<2 % M_>(m));
        std::get<3>(t) = _mm256_mul_epu32(std::get<3>(s), std::get<3 % M_>(m));
        std::get<4>(t) = _mm256_mul_epu32(std::get<4>(s), std::get<4 % M_>(m));
        std::get<5>(t) = _mm256_mul_epu32(std::get<5>(s), std::get<5 % M_>(m));
        std::get<6>(t) = _mm256_mul_epu32(std::get<6>(s), std::get<6 % M_>(m));
        std::get<7>(t) = _mm256_mul_epu32(std::get<7>(s), std::get<7 % M_>(m));

        std::get<0>(s) = _mm256_shuffle_epi32(std::get<0>(t), 0xB1); // 2 3 0 1
        std::get<1>(s) = _mm256_shuffle_epi32(std::get<1>(t), 0xB1); // 2 3 0 1
        std::get<2>(s) = _mm256_shuffle_epi32(std::get<2>(t), 0xB1); // 2 3 0 1
        std::get<3>(s) = _mm256_shuffle_epi32(std::get<3>(t), 0xB1); // 2 3 0 1
        std::get<4>(s) = _mm256_shuffle_epi32(std::get<4>(t), 0xB1); // 2 3 0 1
        std::get<5>(s) = _mm256_shuffle_epi32(std::get<5>(t), 0xB1); // 2 3 0 1
        std::get<6>(s) = _mm256_shuffle_epi32(std::get<6>(t), 0xB1); // 2 3 0 1
        std::get<7>(s) = _mm256_shuffle_epi32(std::get<7>(t), 0xB1); // 2 3 0 1

        std::get<0>(s) = _mm256_xor_si256(std::get<0>(s), x0);
        std::get<1>(s) = _mm256_xor_si256(std::get<1>(s), x1);
        std::get<2>(s) = _mm256_xor_si256(std::get<2>(s), x2);
        std::get<3>(s) = _mm256_xor_si256(std::get<3>(s), x3);
        std::get<4>(s) = _mm256_xor_si256(std::get<4>(s), x4);
        std::get<5>(s) = _mm256_xor_si256(std::get<5>(s), x5);
        std::get<6>(s) = _mm256_xor_si256(std::get<6>(s), x6);
        std::get<7>(s) = _mm256_xor_si256(std::get<7>(s), x7);
    }

    public:
    static void pbox(std::array<__m256i, 8> &, std::array<__m256i, 8> &) {}
}; // class PhiloxGeneratorAVX2Impl

#endif // MCKL_HAS_AVX2

template <typename T, std::size_t Rounds, typename Constants>
class PhiloxGeneratorImpl<T, 2, Rounds, Constants>
#if MCKL_HAS_AVX2
    : public PhiloxGeneratorAVX2Impl<T, 2, Rounds, Constants,
          PhiloxGeneratorImpl<T, 2, Rounds, Constants>>
#elif MCKL_HAS_SSE2
    : public PhiloxGeneratorSSE2Impl<T, 2, Rounds, Constants,
          PhiloxGeneratorImpl<T, 2, Rounds, Constants>>
#else  // MCKL_HAS_AVX2
    : public PhiloxGeneratorGenericImpl<T, 2, Rounds, Constants>
#endif // MCKL_HAS_AVX2
{
}; // class PhiloxGeneratorImpl

template <typename T, std::size_t Rounds, typename Constants>
class PhiloxGeneratorImpl<T, 4, Rounds, Constants>
#if MCKL_HAS_AVX2
    : public PhiloxGeneratorAVX2Impl<T, 4, Rounds, Constants,
          PhiloxGeneratorImpl<T, 4, Rounds, Constants>>
#elif MCKL_HAS_SSE2
    : public PhiloxGeneratorSSE2Impl<T, 4, Rounds, Constants,
          PhiloxGeneratorImpl<T, 4, Rounds, Constants>>
#else  // MCKL_HAS_AVX2
    : public PhiloxGeneratorGenericImpl<T, 4, Rounds, Constants>
#endif // MCKL_HAS_AVX2
{
    public:
#if MCKL_HAS_SSE2
    static void pbox(std::array<__m128i, 8> &s, std::array<__m128i, 8> &)
    {
        std::get<0>(s) = _mm_shuffle_epi32(std::get<0>(s), 0xC6); // 3 0 1 2
        std::get<1>(s) = _mm_shuffle_epi32(std::get<1>(s), 0xC6); // 3 0 1 2
        std::get<2>(s) = _mm_shuffle_epi32(std::get<2>(s), 0xC6); // 3 0 1 2
        std::get<3>(s) = _mm_shuffle_epi32(std::get<3>(s), 0xC6); // 3 0 1 2
        std::get<4>(s) = _mm_shuffle_epi32(std::get<4>(s), 0xC6); // 3 0 1 2
        std::get<5>(s) = _mm_shuffle_epi32(std::get<5>(s), 0xC6); // 3 0 1 2
        std::get<6>(s) = _mm_shuffle_epi32(std::get<6>(s), 0xC6); // 3 0 1 2
        std::get<7>(s) = _mm_shuffle_epi32(std::get<7>(s), 0xC6); // 3 0 1 2
    }
#endif // MCKL_HAS_SSE2

#if MCKL_HAS_AVX2
    static void pbox(std::array<__m256i, 8> &s, std::array<__m256i, 8> &)
    {
        std::get<0>(s) = _mm256_shuffle_epi32(std::get<0>(s), 0xC6); // 3 0 1 2
        std::get<1>(s) = _mm256_shuffle_epi32(std::get<1>(s), 0xC6); // 3 0 1 2
        std::get<2>(s) = _mm256_shuffle_epi32(std::get<2>(s), 0xC6); // 3 0 1 2
        std::get<3>(s) = _mm256_shuffle_epi32(std::get<3>(s), 0xC6); // 3 0 1 2
        std::get<4>(s) = _mm256_shuffle_epi32(std::get<4>(s), 0xC6); // 3 0 1 2
        std::get<5>(s) = _mm256_shuffle_epi32(std::get<5>(s), 0xC6); // 3 0 1 2
        std::get<6>(s) = _mm256_shuffle_epi32(std::get<6>(s), 0xC6); // 3 0 1 2
        std::get<7>(s) = _mm256_shuffle_epi32(std::get<7>(s), 0xC6); // 3 0 1 2
    }
#endif // MCKL_HAS_AVX2
};     // class PhiloxGeneratorImpl

} // namespace mckl::internal

/// \brief Philox RNG generator
/// \ingroup Philox
///
/// \tparam T State type, must be 32- or 64-bit unsigned integers
/// \tparam K State vector length, must be 2 or 4 (for 32- or 64-bit states)
/// \tparam Rounds Number of SP rounds
/// \tparam Constants A trait class that defines algorithm constants, see
/// PhiloxConstants
///
/// \details
/// This generator implement the Philox algorithm in
/// [Random123](http://www.deshawresearch.com/resources_random123.html),
/// developed John K. Salmon, Mark A. Moraes, Ron O. Dror, and David E. Shaw.
template <typename T, std::size_t K, std::size_t Rounds = MCKL_PHILOX_ROUNDS,
    typename Constants = PhiloxConstants<T, K>>
class PhiloxGenerator
{
    static_assert(std::is_unsigned<T>::value,
        "**PhiloxGenerator** used with T other than unsigned integer types");

    static_assert(K != 0 && K % 2 == 0,
        "**PhiloxGenerator** used with K other than multiples of 2");

    static_assert(
        Rounds != 0, "**PhiloxGenerator** used with rounds equal to zero");

    public:
    using ctr_type =
        typename std::conditional<sizeof(T) * K % sizeof(std::uint64_t) == 0,
            std::array<std::uint64_t, sizeof(T) * K / sizeof(std::uint64_t)>,
            std::array<T, K>>::type;
    using key_type = std::array<T, K / 2>;

    static constexpr std::size_t size() { return sizeof(T) * K; }

    key_type key() const { return key_; }

    void reset(const key_type &key) { key_ = key; }

    void enc(const ctr_type &ctr, ctr_type &buffer)
    {
        union {
            std::array<T, K> state;
            ctr_type result;
        } buf;

        buf.result = ctr;
        internal::PhiloxGeneratorImpl<T, K, Rounds, Constants>::eval(
            buf.state, key_);
        buffer = buf.result;
    }

    template <typename ResultType>
    void operator()(ctr_type &ctr,
        std::array<ResultType, size() / sizeof(ResultType)> &buffer) const
    {
        generate(ctr, buffer);
    }

    template <typename ResultType>
    void operator()(ctr_type &ctr, std::size_t n,
        std::array<ResultType, size() / sizeof(ResultType)> *buffer) const
    {
        generate(ctr, n, buffer);
    }

    friend bool operator==(const PhiloxGenerator<T, K, Rounds> &gen1,
        const PhiloxGenerator<T, K, Rounds> &gen2)
    {
        return gen1.key_ == gen2.key_;
    }

    friend bool operator!=(const PhiloxGenerator<T, K, Rounds> &gen1,
        const PhiloxGenerator<T, K, Rounds> &gen2)
    {
        return !(gen1 == gen2);
    }

    template <typename CharT, typename Traits>
    friend std::basic_ostream<CharT, Traits> &operator<<(
        std::basic_ostream<CharT, Traits> &os,
        const PhiloxGenerator<T, K, Rounds> &gen)
    {
        if (!os)
            return os;

        os << gen.key_;

        return os;
    }

    template <typename CharT, typename Traits>
    friend std::basic_istream<CharT, Traits> &operator>>(
        std::basic_istream<CharT, Traits> &is,
        PhiloxGenerator<T, K, Rounds> &gen)
    {
        if (!is)
            return is;

        key_type key = {{0}};
        is >> std::ws >> key;

        if (is)
            gen.key_ = std::move(key);

        return is;
    }

    private:
    key_type key_;

    void generate(std::array<T, K> &ctr, std::array<T, K> &buffer) const
    {
        increment(ctr);
        buffer = ctr;
        internal::PhiloxGeneratorImpl<T, K, Rounds, Constants>::eval(
            buffer, key_);
    }

    template <typename ResultType>
    void generate(ctr_type &ctr,
        std::array<ResultType, size() / sizeof(ResultType)> &buffer) const
    {
        union {
            std::array<T, K> state;
            ctr_type ctr;
            std::array<ResultType, size() / sizeof(ResultType)> result;
        } buf;

        increment(ctr);
        buf.ctr = ctr;
        internal::PhiloxGeneratorImpl<T, K, Rounds, Constants>::eval(
            buf.state, key_);
        buffer = buf.result;
    }

    void generate(
        std::array<T, K> &ctr, std::size_t n, std::array<T, K> *buffer) const
    {
        static constexpr std::size_t blocks =
            internal::PhiloxGeneratorImpl<T, K, Rounds, Constants>::blocks;

        using state_type = std::array<std::array<T, K>, blocks>;

        const std::size_t m = n / blocks;
        const std::size_t l = n % blocks;
        for (std::size_t i = 0; i != m; ++i, buffer += blocks) {
            state_type &state = *reinterpret_cast<state_type *>(buffer);
            increment(ctr, state);
            internal::PhiloxGeneratorImpl<T, K, Rounds, Constants>::eval(
                state, key_);
        }
        for (std::size_t i = 0; i != l; ++i)
            generate(ctr, buffer[i]);
    }

    template <typename ResultType>
    void generate(ctr_type &ctr, std::size_t n,
        std::array<ResultType, size() / sizeof(ResultType)> *buffer) const
    {
        static constexpr std::size_t blocks =
            internal::PhiloxGeneratorImpl<T, K, Rounds, Constants>::blocks;

        union {
            std::array<std::array<T, K>, blocks> state;
            std::array<ctr_type, blocks> ctr_block;
            std::array<std::array<ResultType, size() / sizeof(ResultType)>,
                blocks>
                result;
        } buf;

        const std::size_t m = n / blocks;
        const std::size_t l = n % blocks;
        for (std::size_t i = 0; i != m; ++i, buffer += blocks) {
            increment(ctr, buf.ctr_block);
            internal::PhiloxGeneratorImpl<T, K, Rounds, Constants>::eval(
                buf.state, key_);
            std::copy_n(buf.result.data(), blocks, buffer);
        }
        for (std::size_t i = 0; i != l; ++i)
            generate(ctr, buffer[i]);
    }
}; // class PhiloxGenerator

/// \brief Philox RNG engine
/// \ingroup Philox
template <typename ResultType, typename T, std::size_t K,
    std::size_t Rounds = MCKL_PHILOX_ROUNDS,
    typename Constants = PhiloxConstants<T, K>>
using PhiloxEngine =
    CounterEngine<ResultType, PhiloxGenerator<T, K, Rounds, Constants>>;

/// \brief Philox2x32 RNG engine
/// \ingroup Philox
template <typename ResultType>
using Philox2x32Engine = PhiloxEngine<ResultType, std::uint32_t, 2>;

/// \brief Philox4x32 RNG engine
/// \ingroup Philox
template <typename ResultType>
using Philox4x32Engine = PhiloxEngine<ResultType, std::uint32_t, 4>;

/// \brief Philox2x64 RNG engine
/// \ingroup Philox
template <typename ResultType>
using Philox2x64Engine = PhiloxEngine<ResultType, std::uint64_t, 2>;

/// \brief Philox4x64 RNG engine
/// \ingroup Philox
template <typename ResultType>
using Philox4x64Engine = PhiloxEngine<ResultType, std::uint64_t, 4>;

/// \brief Philox2x32 RNG engine with 32-bit integer output
/// \ingroup Philox
using Philox2x32 = Philox2x32Engine<std::uint32_t>;

/// \brief Philox4x32 RNG engine with 32-bit integer output
/// \ingroup Philox
using Philox4x32 = Philox4x32Engine<std::uint32_t>;

/// \brief Philox2x64 RNG engine with 32-bit integer output
/// \ingroup Philox
using Philox2x64 = Philox2x64Engine<std::uint32_t>;

/// \brief Philox4x64 RNG engine with 32-bit integer output
/// \ingroup Philox
using Philox4x64 = Philox4x64Engine<std::uint32_t>;

/// \brief Philox2x32 RNG engine with 64-bit integer output
/// \ingroup Philox
using Philox2x32_64 = Philox2x32Engine<std::uint64_t>;

/// \brief Philox4x32 RNG engine with 64-bit integer output
/// \ingroup Philox
using Philox4x32_64 = Philox4x32Engine<std::uint64_t>;

/// \brief Philox2x64 RNG engine with 64-bit integer output
/// \ingroup Philox
using Philox2x64_64 = Philox2x64Engine<std::uint64_t>;

/// \brief Philox4x64 RNG engine with 64-bit integer output
/// \ingroup Philox
using Philox4x64_64 = Philox4x64Engine<std::uint64_t>;

} // namespace mckl

#endif // MCKL_RANDOM_PHILOX_HPP
