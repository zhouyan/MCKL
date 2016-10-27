//============================================================================
// MCKL/include/mckl/random/internal/philox_generic.hpp
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

#ifndef MCKL_RANDOM_INTERNAL_PHILOX_GENERIC_HPP
#define MCKL_RANDOM_INTERNAL_PHILOX_GENERIC_HPP

#include <mckl/random/internal/common.hpp>
#include <mckl/random/internal/threefry_constants.hpp>

namespace mckl
{

namespace internal
{

template <typename T, int = std::numeric_limits<T>::digits>
class PhiloxHiLo;

template <typename T>
class PhiloxHiLo<T, 32>
{
    public:
    static T eval(T a, T b, T &hi)
    {
        union {
            std::uint64_t prod;
            std::array<T, 2> result;
        } buf;

        buf.prod =
            static_cast<std::uint64_t>(a) * static_cast<std::uint64_t>(b);

#if MCKL_HAS_LITTLE_ENDIAN
        hi = std::get<1>(buf.result);

        return std::get<0>(buf.result);
#elif MCKL_HAS_BIG_ENDIAN
        hi = std::get<0>(buf.result);

        return std::get<1>(buf.result);
#else
        a = static_cast<T>(buf.prod >> 32);

        return static_cast<T>(buf.prod);
#endif
    }
}; // class PhiloxHiLo

template <typename T>
class PhiloxHiLo<T, 64>
{
    public:
    static T eval(T a, T b, T &hi)
    {
#if MCKL_USE_BMI2 && MCKL_HAS_X86_64
        return eval_bmi2(a, b, hi);
#elif MCKL_HAS_INT128
        return eval_int128(a, b, hi);
#elif defined(MCKL_MSVC)
        return eval_msvc(a, b, hi);
#else
        return eval_generic(a, b, hi);
#endif
    }

    private:
    static T eval_generic(T a, T b, T &hi)
    {
        const T mask = (const_one<T>() << 32) - 1;
        const T ahi = a >> 32;
        const T alo = a & mask;
        const T bhi = b >> 32;
        const T blo = b & mask;
        const T ahbl = ahi * blo;
        const T albh = alo * bhi;
        const T ahbl_albh = ((ahbl & mask) + (albh & mask));
        const T lo = a * b;
        hi = ahi * bhi + (ahbl >> 32) + (albh >> 32);
        hi += ahbl_albh >> 32;
        hi += ((lo >> 32) < (ahbl_albh & mask));

        return lo;
    }

#if MCKL_USE_BMI2 && MCKL_HAS_X86_64
    static T eval_bmi2(T a, T b, T &hi)
    {
        unsigned MCKL_INT64 h = 0;
        unsigned MCKL_INT64 l =
            static_cast<T>(_mulx_u64(static_cast<unsigned MCKL_INT64>(a),
                static_cast<unsigned MCKL_INT64>(b), &h));
        hi = static_cast<T>(h);

        return l;
    }
#endif

#if MCKL_HAS_INT128
    static T eval_int128(T a, T b, T &hi)
    {
#ifdef MCKL_GCC
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpedantic"
#endif
        union {
            unsigned MCKL_INT128 prod;
            std::array<T, 2> result;
        } buf;

        buf.prod = static_cast<unsigned MCKL_INT128>(a) *
            static_cast<unsigned MCKL_INT128>(b);
#ifdef MCKL_GCC
#pragma GCC diagnostic pop
#endif
#if MCKL_HAS_LITTLE_ENDIAN
        hi = std::get<1>(buf.result);

        return std::get<0>(buf.result);
#elif MCKL_HAS_BIG_ENDIAN
        hi = std::get<0>(buf.result);

        return std::get<1>(buf.result);
#else
        hi = static_cast<T>(buf.prod >> 64);

        return static_cast<T>(buf.prod);
#endif
    }
#endif

#ifdef MCKL_MSVC
    static T eval_msvc(T a, T b, T &hi)
    {
        unsigned __int64 Multiplier = static_cast<unsigned __int64>(a);
        unsigned __int64 Multiplicand = static_cast<unsigned __int64>(b);
        unsigned __int64 h = 0;
        unsigned __int64 l = _umul128(Multiplier, Multiplicand, &h);
        hi = static_cast<T>(h);

        return static_cast<T>(l);
    }
#endif
}; // class PhiloxHiLo

#if MCKL_USE_BMI2 && MCKL_HAS_X86_64

template <>
class PhiloxHiLo<unsigned MCKL_INT64, 64>
{
    public:
    static unsigned MCKL_INT64 eval(
        unsigned MCKL_INT64 a, unsigned MCKL_INT64 b, unsigned MCKL_INT64 &hi)
    {
        return _mulx_u64(a, b, &hi);
    }
}; // class PhiloxHiLo

#endif // MCKL_USE_BMI2 && MCKL_HAS_X86_64

template <typename T, std::size_t K, std::size_t N, typename Constants>
class PhiloxKBox
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
        constexpr T w = Constants::weyl::value[I];

        std::get<I>(par) += w;
        eval<I + 1>(par, std::integral_constant<bool, I + 1 < K / 2>());
    }
}; // class PhiloxKBox

template <typename T, std::size_t N, typename Constants>
class PhiloxKBox<T, 2, N, Constants>
{
    public:
    static void eval(std::array<T, 1> &par)
    {
        constexpr T w0 = Constants::weyl::value[0];

        std::get<0>(par) += w0;
    }
}; // class PhiloxKBox

template <typename T, std::size_t N, typename Constants>
class PhiloxKBox<T, 4, N, Constants>
{
    public:
    static void eval(std::array<T, 2> &par)
    {
        constexpr T w0 = Constants::weyl::value[0];
        constexpr T w1 = Constants::weyl::value[1];

        std::get<0>(par) += w0;
        std::get<1>(par) += w1;
    }
}; // class PhiloxKBox

template <typename T, std::size_t K, std::size_t N, typename Constants>
class PhiloxSBox
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
        constexpr T m = Constants::multiplier::value[I / 2];

        T x = std::get<I + 1>(state);
        T hi;
        std::get<I + 1>(state) =
            PhiloxHiLo<T>::eval(std::get<I>(state), m, hi);
        std::get<I>(state) = std::get<I / 2>(par) ^ x ^ hi;

        eval<I + 2>(state, par, std::integral_constant<bool, I + 3 < K>());
    }
}; // class PhiloxSBox

template <typename T, std::size_t N, typename Constants>
class PhiloxSBox<T, 2, N, Constants>
{
    public:
    static void eval(std::array<T, 2> &state, const std::array<T, 1> &par)
    {
        constexpr T m0 = Constants::multiplier::value[0];

        T x1 = std::get<1>(state);
        T hi0;
        std::get<1>(state) = PhiloxHiLo<T>::eval(std::get<0>(state), m0, hi0);
        std::get<0>(state) = std::get<0>(par) ^ x1 ^ hi0;
    }
}; // class PhiloxSBox

template <typename T, std::size_t N, typename Constants>
class PhiloxSBox<T, 4, N, Constants>
{
    public:
    static void eval(std::array<T, 4> &state, const std::array<T, 2> &par)
    {
        constexpr T m2 = Constants::multiplier::value[0];
        constexpr T m0 = Constants::multiplier::value[1];

        T x1 = std::get<1>(state);
        T x3 = std::get<3>(state);
        T hi0;
        T hi2;
        std::get<1>(state) = PhiloxHiLo<T>::eval(std::get<2>(state), m2, hi2);
        std::get<3>(state) = PhiloxHiLo<T>::eval(std::get<0>(state), m0, hi0);
        std::get<0>(state) = std::get<0>(par) ^ x1 ^ hi2;
        std::get<2>(state) = std::get<1>(par) ^ x3 ^ hi0;
    }
}; // class PhiloxSBox

template <typename T, std::size_t K, std::size_t N, typename Constants>
class PhiloxPBox
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
        constexpr std::size_t P =
            ThreefryConstants<T, K>::permute::value[K - I - 1];
        constexpr std::size_t J = K - P - 1;

        std::get<I>(tmp) = std::get<J>(state);
        eval<I + 1>(state, tmp, std::integral_constant<bool, I + 1 < K>());
    }
}; // class PhiloxPBox

template <typename T, std::size_t N, typename Constants>
class PhiloxPBox<T, 2, N, Constants>
{
    public:
    static void eval(std::array<T, 2> &) {}
}; // class PhiloxPBox

template <typename T, std::size_t N, typename Constants>
class PhiloxPBox<T, 4, N, Constants>
{
    public:
    static void eval(std::array<T, 4> &) {}
}; // class PhiloxPBox

template <typename T, std::size_t K, std::size_t Rounds, typename Constants>
class PhiloxGeneratorGenericImpl
{
    public:
    static constexpr bool batch() { return false; }

    static void eval(std::array<T, K> &state, const std::array<T, K / 2> &key)
    {
        std::array<T, K / 2> par = key;
        round<0>(state, par, std::integral_constant<bool, 0 <= Rounds>());
    }

    private:
    template <std::size_t>
    static void round(
        std::array<T, K> &, std::array<T, K / 2> &, std::false_type)
    {
    }

    template <std::size_t N>
    static void round(
        std::array<T, K> &state, std::array<T, K / 2> &par, std::true_type)
    {
        kbox<N>(par);
        rbox<N>(state, par);
        round<N + 1>(
            state, par, std::integral_constant<bool, N + 1 <= Rounds>());
    }

    template <std::size_t N>
    static void kbox(std::array<T, K / 2> &par)
    {
        kbox<N>(par, std::integral_constant<bool, (N > 1 && N <= Rounds)>());
    }

    template <std::size_t N>
    static void kbox(std::array<T, K / 2> &, std::false_type)
    {
    }

    template <std::size_t N>
    static void kbox(std::array<T, K / 2> &par, std::true_type)
    {
        PhiloxKBox<T, K, N, Constants>::eval(par);
    }

    template <std::size_t N>
    static void rbox(std::array<T, K> &state, const std::array<T, K / 2> &par)
    {
        rbox<N>(state, par,
            std::integral_constant<bool, (N > 0 && N <= Rounds)>());
    }

    template <std::size_t N>
    static void rbox(
        std::array<T, K> &, const std::array<T, K / 2> &, std::false_type)
    {
    }

    template <std::size_t N>
    static void rbox(std::array<T, K> &state, const std::array<T, K / 2> &par,
        std::true_type)
    {
        PhiloxPBox<T, K, N, Constants>::eval(state);
        PhiloxSBox<T, K, N, Constants>::eval(state, par);
    }
}; // class PhiloxGeneratorGenericImpl

} // namespace mckl::internal

} // namespace mckl

#endif // MCKL_RANDOM_INTERNAL_PHILOX_GENERIC_HPP
