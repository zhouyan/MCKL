//============================================================================
// MCKL/include/mckl/random/internal/threefry_common.hpp
//----------------------------------------------------------------------------
// MCKL: Monte Carlo Kernel Library
//----------------------------------------------------------------------------
// Copyright (c) 2013-2018, Yan Zhou
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

#ifndef MCKL_RANDOM_INTERNAL_THREEFRY_COMMON_HPP
#define MCKL_RANDOM_INTERNAL_THREEFRY_COMMON_HPP

#include <mckl/random/internal/common.hpp>
#include <mckl/random/internal/threefry_constants.hpp>

namespace mckl {

namespace internal {

template <typename T, std::size_t K, typename Constants>
class ThreefryPBox
{
  public:
    static void eval(std::array<T, K> &s)
    {
        std::array<T, K> tmp;
        eval<0>(s, tmp, std::integral_constant<bool, 0 < K>());
        MCKL_MEMCPY(s.data(), tmp.data(), sizeof(T) * K);
    }

    static void eval(T *s)
    {
        std::array<T, K> tmp;
        eval<0>(s, tmp, std::integral_constant<bool, 0 < K>());
        MCKL_MEMCPY(s, tmp.data(), sizeof(T) * K);
    }

  private:
    template <std::size_t>
    static void eval(
        const std::array<T, K> &, std::array<T, K> &, std::false_type)
    {
    }

    template <std::size_t I>
    static void eval(
        const std::array<T, K> &s, std::array<T, K> &tmp, std::true_type)
    {
        constexpr std::size_t P = Constants::permute::value[I];

        std::get<I>(tmp) = std::get<P>(s);
        eval<I + 1>(s, tmp, std::integral_constant<bool, I + 1 < K>());
    }

    template <std::size_t>
    static void eval(const T *, std::array<T, K> &, std::false_type)
    {
    }

    template <std::size_t I>
    static void eval(const T *s, std::array<T, K> &tmp, std::true_type)
    {
        constexpr std::size_t P = Constants::permute::value[I];

        std::get<I>(tmp) = s[P];
        eval<I + 1>(s, tmp, std::integral_constant<bool, I + 1 < K>());
    }
}; // class ThreefryPBox

template <typename T, typename U>
class ThreefryPBox<T, 2, ThreefryConstants<U, 2>>
{
  public:
    static void eval(std::array<T, 2> &) {}

    static void eval(T *) {}
}; // class ThreefryPBox

template <typename T, typename U>
class ThreefryPBox<T, 4, ThreefryConstants<U, 4>>
{
  public:
    static void eval(std::array<T, 4> &s)
    {
        T s1 = std::get<1>(s);
        std::get<1>(s) = std::get<3>(s);
        std::get<3>(s) = s1;
    }

    static void eval(T *s)
    {
        T s1 = s[1];
        s[1] = s[3];
        s[3] = s1;
    }
}; // class ThreefryPBox

template <typename T, typename U>
class ThreefryPBox<T, 8, ThreefryConstants<U, 8>>
{
  public:
    static void eval(std::array<T, 8> &s)
    {
        T s0 = std::get<0>(s);
        T s3 = std::get<3>(s);

        std::get<0>(s) = std::get<2>(s);
        std::get<3>(s) = std::get<7>(s);

        std::get<2>(s) = std::get<4>(s);
        std::get<4>(s) = std::get<6>(s);

        std::get<6>(s) = s0;
        std::get<7>(s) = s3;
    }

    static void eval(T *s)
    {
        T s0 = s[0];
        T s3 = s[3];

        s[0] = s[2];
        s[3] = s[7];

        s[2] = s[4];
        s[4] = s[6];

        s[6] = s0;
        s[7] = s3;
    }
}; // class ThreefryPBox

template <typename T, typename U>
class ThreefryPBox<T, 16, ThreefryConstants<U, 16>>
{
  public:
    static void eval(std::array<T, 16> &s)
    {
        T s1 = std::get<0x1>(s);
        T s3 = std::get<0x3>(s);
        T s4 = std::get<0x4>(s);
        T s8 = std::get<0x8>(s);

        std::get<0x1>(s) = std::get<0x9>(s);
        std::get<0x3>(s) = std::get<0xD>(s);
        std::get<0x4>(s) = std::get<0x6>(s);
        std::get<0x8>(s) = std::get<0xA>(s);

        std::get<0x9>(s) = std::get<0x7>(s);
        std::get<0xD>(s) = std::get<0x5>(s);
        std::get<0xA>(s) = std::get<0xC>(s);

        std::get<0x7>(s) = std::get<0xF>(s);
        std::get<0x5>(s) = std::get<0xB>(s);
        std::get<0xC>(s) = std::get<0xE>(s);

        std::get<0xF>(s) = s1;
        std::get<0xB>(s) = s3;
        std::get<0x6>(s) = s4;
        std::get<0xE>(s) = s8;
    }

    static void eval(T *s)
    {
        T s1 = s[0x1];
        T s3 = s[0x3];
        T s4 = s[0x4];
        T s8 = s[0x8];

        s[0x1] = s[0x9];
        s[0x3] = s[0xD];
        s[0x4] = s[0x6];
        s[0x8] = s[0xA];

        s[0x9] = s[0x7];
        s[0xD] = s[0x5];
        s[0xA] = s[0xC];

        s[0x7] = s[0xF];
        s[0x5] = s[0xB];
        s[0xC] = s[0xE];

        s[0xF] = s1;
        s[0xB] = s3;
        s[0x6] = s4;
        s[0xE] = s8;
    }
}; // class ThreefryPBox

template <typename T, std::size_t K, std::size_t N>
class ThreefryKBox
{
  public:
    template <std::size_t I>
    static T key(const std::array<T, K + 4> &par)
    {
        constexpr std::size_t S = N / 4;

        const T p = (I + 1 == K ? S : 0) +
            (I + 2 == K ? std::get<K + 1 + (S + 1) % 3>(par) : 0) +
            (I + 3 == K ? std::get<K + 1 + (S + 0) % 3>(par) : 0);

        return std::get<(S + I) % (K + 1)>(par) + p;
    }

    static void eval(std::array<T, K> &s, const std::array<T, K + 4> &par)
    {
        eval<0>(s, par, std::integral_constant<bool, 0 < K>());
    }

  private:
    template <std::size_t>
    static void eval(
        std::array<T, K> &, const std::array<T, K + 4> &, std::false_type)
    {
    }

    template <std::size_t I>
    static void eval(
        std::array<T, K> &s, const std::array<T, K + 4> &par, std::true_type)
    {
        std::get<I>(s) += key<I>(par);
        eval<I + 1>(s, par, std::integral_constant<bool, I + 1 < K>());
    }
}; // class ThreefryKBox

template <typename T, std::size_t K, std::size_t N, typename Constants>
class ThreefryRBox
{
  public:
    static void eval(std::array<T, K> &s)
    {
        sbox<0>(s, std::integral_constant<bool, 0 < K / 2>());
        ThreefryPBox<T, K, Constants>::eval(s);
    }

  private:
    template <std::size_t>
    static void sbox(std::array<T, K> &, std::false_type)
    {
    }

    template <std::size_t I>
    static void sbox(std::array<T, K> &s, std::true_type)
    {
        constexpr std::size_t I0 = I * 2;
        constexpr std::size_t I1 = I * 2 + 1;
        constexpr int L = Constants::rotate::value[I][(N - 1) % 8];
        constexpr int R = std::numeric_limits<T>::digits - L;

        T x = std::get<I1>(s);
        std::get<I0>(s) += x;
        x = (x << L) | (x >> R);
        x ^= std::get<I0>(s);
        std::get<I1>(s) = x;
        sbox<I + 1>(s, std::integral_constant<bool, I + 1 < K / 2>());
    }
}; // class ThreefryRBox

} // namespace internal

} // namespace mckl

#endif // MCKL_RANDOM_INTERNAL_THREEFRY_COMMON_HPP
