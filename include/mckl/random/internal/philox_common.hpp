//============================================================================
// MCKL/include/mckl/random/internal/philox_common.hpp
//----------------------------------------------------------------------------
// MCKL: Monte Carlo Kernel Library
//----------------------------------------------------------------------------
// Copyright (c) 2013-2017, Yan Zhou
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

#ifndef MCKL_RANDOM_INTERNAL_PHILOX_COMMON_HPP
#define MCKL_RANDOM_INTERNAL_PHILOX_COMMON_HPP

#include <mckl/random/internal/common.hpp>
#include <mckl/random/internal/philox_constants.hpp>
#include <mckl/random/internal/threefry_constants.hpp>

namespace mckl {

namespace internal {

template <typename T, int = std::numeric_limits<T>::digits>
class PhiloxHiLo;

template <typename T>
class PhiloxHiLo<T, 32>
{
  public:
    static T eval(T a, T b, T &h)
    {
        union {
            std::uint64_t p;
            std::array<T, 2> r;
        } buf;

        buf.p = static_cast<std::uint64_t>(a) * static_cast<std::uint64_t>(b);

#if MCKL_HAS_LITTLE_ENDIAN
        h = std::get<1>(buf.r);

        return std::get<0>(buf.r);
#elif MCKL_HAS_BIG_ENDIAN
        h = std::get<0>(buf.r);

        return std::get<1>(buf.r);
#else
        h = static_cast<T>(buf.p >> 32);

        return static_cast<T>(buf.p);
#endif
    }
}; // class PhiloxHiLo

template <typename T>
class PhiloxHiLo<T, 64>
{
  public:
    static T eval(T a, T b, T &h)
    {
#if MCKL_USE_BMI2 && MCKL_HAS_X86_64
        return eval_bmi2(a, b, h);
#elif MCKL_HAS_INT128
        return eval_int128(a, b, h);
#else
        return eval_generic(a, b, h);
#endif
    }

  private:
    static T eval_generic(T a, T b, T &h)
    {
        const T mask = (const_one<T>() << 32) - 1;
        const T ah = a >> 32;
        const T al = a & mask;
        const T bh = b >> 32;
        const T bl = b & mask;
        const T ahbl = ah * bl;
        const T albh = al * bh;
        const T ahbl_albh = ((ahbl & mask) + (albh & mask));
        const T l = a * b;
        h = ah * bh + (ahbl >> 32) + (albh >> 32);
        h += ahbl_albh >> 32;
        h += (l >> 32) < (ahbl_albh & mask) ? 1 : 0;

        return l;
    }

#if MCKL_USE_BMI2 && MCKL_HAS_X86_64
    static T eval_bmi2(T a, T b, T &h)
    {
        unsigned MCKL_INT64 t = 0;
        unsigned MCKL_INT64 l =
            static_cast<T>(_mulx_u64(static_cast<unsigned MCKL_INT64>(a),
                static_cast<unsigned MCKL_INT64>(b), &t));
        h = static_cast<T>(t);

        return l;
    }
#endif

#if MCKL_HAS_INT128
    static T eval_int128(T a, T b, T &h)
    {
#ifdef MCKL_GCC
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpedantic"
#endif
        union {
            unsigned MCKL_INT128 p;
            std::array<T, 2> r;
        } buf;

        buf.p = static_cast<unsigned MCKL_INT128>(a) *
            static_cast<unsigned MCKL_INT128>(b);
#ifdef MCKL_GCC
#pragma GCC diagnostic pop
#endif
#if MCKL_HAS_LITTLE_ENDIAN
        h = std::get<1>(buf.r);

        return std::get<0>(buf.r);
#elif MCKL_HAS_BIG_ENDIAN
        h = std::get<0>(buf.r);

        return std::get<1>(buf.r);
#else
        h = static_cast<T>(buf.p >> 64);

        return static_cast<T>(buf.p);
#endif
    }
#endif
}; // class PhiloxHiLo

#if MCKL_USE_BMI2 && MCKL_HAS_X86_64

template <>
class PhiloxHiLo<unsigned MCKL_INT64, 64>
{
  public:
    static unsigned MCKL_INT64 eval(
        unsigned MCKL_INT64 a, unsigned MCKL_INT64 b, unsigned MCKL_INT64 &h)
    {
        return _mulx_u64(a, b, &h);
    }
}; // class PhiloxHiLo

#endif // MCKL_USE_BMI2 && MCKL_HAS_X86_64

template <typename T, std::size_t K, std::size_t N, typename Constants>
class PhiloxRBox
{
  public:
    static void eval(std::array<T, K> &s, const std::array<T, K / 2> &k)
    {
        std::array<T, K> t;
        pbox<0>(s, t, std::integral_constant<bool, 0 < K>());
        sbox<0>(s, t, k, std::integral_constant<bool, 0 < K / 2>());
    }

  private:
    template <std::size_t>
    static void sbox(std::array<T, K> &, const std::array<T, K> &,
        const std::array<T, K / 2> &, std::false_type)
    {
    }

    template <std::size_t I>
    static void sbox(std::array<T, K> &s, const std::array<T, K> &t,
        const std::array<T, K / 2> &k, std::true_type)
    {
        constexpr std::size_t I0 = I * 2;
        constexpr std::size_t I1 = I * 2 + 1;
        constexpr T w = Constants::weyl::value[I] * static_cast<T>(N - 1);
        constexpr T m = Constants::multiplier::value[I];

        T h;
        std::get<I1>(s) = PhiloxHiLo<T>::eval(std::get<I0>(t), m, h);
        std::get<I0>(s) = (std::get<I>(k) + w) ^ (std::get<I1>(t) ^ h);
        sbox<I + 1>(s, t, k, std::integral_constant<bool, I + 1 < K / 2>());
    }

    template <std::size_t>
    static void pbox(
        const std::array<T, K> &, std::array<T, K> &, std::false_type)
    {
    }

    template <std::size_t I>
    static void pbox(
        const std::array<T, K> &s, std::array<T, K> &t, std::true_type)
    {
        constexpr std::size_t P =
            ThreefryConstants<T, K>::permute::value[K - I - 1];
        constexpr std::size_t J = K - P - 1;

        std::get<I>(t) = std::get<J>(s);
        pbox<I + 1>(s, t, std::integral_constant<bool, I + 1 < K>());
    }
}; // class PhiloxRBox

template <typename T, std::size_t N, typename Constants>
class PhiloxRBox<T, 2, N, Constants>
{
  public:
    static void eval(std::array<T, 2> &s, const std::array<T, 1> &k)
    {
        constexpr T w0 = Constants::weyl::value[0] * static_cast<T>(N - 1);
        constexpr T m0 = Constants::multiplier::value[0];

        T s1 = std::get<1>(s);
        T h0;
        std::get<1>(s) = PhiloxHiLo<T>::eval(std::get<0>(s), m0, h0);
        std::get<0>(s) = (std::get<0>(k) + w0) ^ (s1 ^ h0);
    }
}; // class PhiloxRBox

template <typename T, std::size_t N, typename Constants>
class PhiloxRBox<T, 4, N, Constants>
{
  public:
    static void eval(std::array<T, 4> &s, const std::array<T, 2> &k)
    {
        constexpr T w0 = Constants::weyl::value[0] * static_cast<T>(N - 1);
        constexpr T w1 = Constants::weyl::value[1] * static_cast<T>(N - 1);
        constexpr T m2 = Constants::multiplier::value[0];
        constexpr T m0 = Constants::multiplier::value[1];

        T s1 = std::get<1>(s);
        T s3 = std::get<3>(s);
        T h2;
        T h0;
        std::get<1>(s) = PhiloxHiLo<T>::eval(std::get<2>(s), m2, h2);
        std::get<3>(s) = PhiloxHiLo<T>::eval(std::get<0>(s), m0, h0);
        std::get<0>(s) = (std::get<0>(k) + w0) ^ (s1 ^ h2);
        std::get<2>(s) = (std::get<1>(k) + w1) ^ (s3 ^ h0);
    }
}; // class PhiloxRBox

} // namespace mckl::internal

} // namespace mckl

#endif // MCKL_RANDOM_INTERNAL_PHILOX_COMMON_HPP
