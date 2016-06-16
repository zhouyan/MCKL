//============================================================================
// MCKL/include/mckl/internal/common.hpp
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

#ifndef MCKL_INTERNAL_COMMON_HPP
#define MCKL_INTERNAL_COMMON_HPP

#include <mckl/internal/config.h>

#include <mckl/internal/assert.hpp>
#include <mckl/internal/cblas.hpp>
#include <mckl/internal/defines.hpp>
#include <mckl/internal/forward.hpp>
#include <mckl/internal/traits.hpp>

#include <mckl/math.hpp>
#include <mckl/utility/aligned_memory.hpp>

#include <algorithm>
#include <array>
#include <atomic>
#include <bitset>
#include <cassert>
#include <cmath>
#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <exception>
#include <fstream>
#include <functional>
#include <future>
#include <initializer_list>
#include <iomanip>
#include <iostream>
#include <iterator>
#include <limits>
#include <list>
#include <map>
#include <memory>
#include <mutex>
#include <new>
#include <numeric>
#include <random>
#include <set>
#include <sstream>
#include <stdexcept>
#include <string>
#include <thread>
#include <type_traits>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>

namespace mckl
{

namespace internal
{

template <typename T, std::size_t N>
using StaticVector =
    typename std::conditional<N == Dynamic, Vector<T>, std::array<T, N>>::type;

class StirlingMatrix2
{
    public:
    StirlingMatrix2(std::size_t n, std::size_t m)
        : ncol_(m + 1), data_((n + 1) * (m + 1))
    {
        std::fill(data_.begin(), data_.end(), 0);
        get(0, 0) = 1;
        for (std::size_t j = 1; j <= m; ++j) {
            get(j, j) = 1;
            for (std::size_t i = j + 1; i <= n; ++i)
                get(i, j) = j * get(i - 1, j) + get(i - 1, j - 1);
        }
    }

    double operator()(std::size_t i, std::size_t j) const
    {
        return data_[i * ncol_ + j];
    }

    private:
    std::size_t ncol_;
    Vector<double> data_;

    double &get(std::size_t i, std::size_t j) { return data_[i * ncol_ + j]; }
}; // class StirlingMatrix

template <unsigned long long U,
    int N = std::numeric_limits<unsigned long long>::digits - 1>
class Log2L
{
    static constexpr unsigned long long M = 1ULL << N;

    public:
    static constexpr int value = M <= U ? N : Log2L<U, N - 1>::value;
}; // class Log2L

template <unsigned long long U>
class Log2L<U, 0>
{
    public:
    static constexpr int value = 0;
}; // class Log2L

template <int N>
class Log2L<0, N>
{
    public:
    static constexpr int value = N + 1;
}; // class Log2L

template <typename UIntType, UIntType U>
class Log2 : public Log2L<U, std::numeric_limits<UIntType>::digits - 1>
{
}; // class Log2

template <unsigned long long B, unsigned N, bool = N % 2 == 0>
class PowL
{
    static constexpr unsigned long long b = PowL<B, N / 2>::value;

    public:
    static constexpr unsigned long long value = b * b;
}; // class PowL

template <unsigned long long B, unsigned N>
class PowL<B, N, false>
{
    public:
    static constexpr unsigned long long value = B * PowL<B, N - 1>::value;
}; // class PowL

template <unsigned long long B>
class PowL<B, 0, true>
{
    public:
    static constexpr unsigned long long value = 1;
}; // class PowL

template <typename UIntType, unsigned long long B, unsigned N>
class Pow
{
    public:
    static constexpr UIntType value = static_cast<UIntType>(PowL<B, N>::value);
}; // class Pow

template <typename UIntType, unsigned N>
class Factorial
{
    public:
    static constexpr UIntType value = N * Factorial<UIntType, N - 1>::value;
}; // class Factorial

template <typename UIntType>
class Factorial<UIntType, 1>
{
    public:
    static constexpr UIntType value = 1;
}; // class Factorial

template <typename UIntType>
class Factorial<UIntType, 0>
{
    public:
    static constexpr UIntType value = 1;
}; // class Factorial

template <typename T, std::size_t K = 1>
class BufferSize
    : public std::integral_constant<std::size_t,
          8192 / (sizeof(T) * K) == 0 ? 1 : 8192 / (sizeof(T) * K)>
{
}; // class BufferSize;

template <std::size_t, typename CharT, typename Traits, typename T,
    std::size_t N>
inline void ostream_ary_space(std::basic_ostream<CharT, Traits> &,
    const std::array<T, N> &, std::false_type)
{
}

template <std::size_t, typename CharT, typename Traits, typename T,
    std::size_t N>
inline void ostream_ary_space(std::basic_ostream<CharT, Traits> &os,
    const std::array<T, N> &, std::true_type)
{
    os << ' ';
}

template <std::size_t, typename CharT, typename Traits, typename T,
    std::size_t N>
inline void ostream_ary(std::basic_ostream<CharT, Traits> &,
    const std::array<T, N> &, std::false_type)
{
}

template <std::size_t K, typename CharT, typename Traits, typename T,
    std::size_t N>
inline void ostream_ary(std::basic_ostream<CharT, Traits> &os,
    const std::array<T, N> &ary, std::true_type)
{
    os << std::get<K>(ary);
    ostream_ary_space<K>(os, ary, std::integral_constant<bool, K + 1 != N>());
    ostream_ary<K + 1>(os, ary, std::integral_constant<bool, K + 1 < N>());
}

template <typename CharT, typename Traits, typename T, std::size_t N>
inline std::basic_ostream<CharT, Traits> &ostream(
    std::basic_ostream<CharT, Traits> &os, const std::array<T, N> &ary)
{
    if (!os)
        return os;

    ostream_ary<0>(os, ary, std::integral_constant<bool, 0 < N>());

    return os;
}

template <std::size_t, typename CharT, typename Traits, typename T,
    std::size_t N>
inline void istream_ary(
    std::basic_istream<CharT, Traits> &, std::array<T, N> &, std::false_type)
{
}

template <std::size_t K, typename CharT, typename Traits, typename T,
    std::size_t N>
inline void istream_ary(std::basic_istream<CharT, Traits> &is,
    std::array<T, N> &ary, std::true_type)
{
    is >> std::ws >> std::get<K>(ary);
    istream_ary<K + 1>(is, ary, std::integral_constant<bool, K + 1 < N>());
}

template <typename CharT, typename Traits, typename T, std::size_t N>
inline std::basic_istream<CharT, Traits> &istream(
    std::basic_istream<CharT, Traits> &is, std::array<T, N> &ary)
{
    if (!is)
        return is;

    std::array<T, N> tmp;
    istream_ary<0>(is, tmp, std::integral_constant<bool, 0 < N>());
    if (is)
        ary = std::move(tmp);

    return is;
}

template <typename CharT, typename Traits, typename T, typename Alloc>
inline std::basic_ostream<CharT, Traits> &ostream(
    std::basic_ostream<CharT, Traits> &os, const std::vector<T, Alloc> &vec)
{
    if (!os)
        return os;

    os << vec.size();
    if (!os)
        return os;

    for (const auto &v : vec)
        os << ' ' << v;

    return os;
}

template <typename CharT, typename Traits, typename T, typename Alloc>
inline std::basic_istream<CharT, Traits> &istream(
    std::basic_istream<CharT, Traits> &is, std::vector<T, Alloc> &vec)
{
    if (!is)
        return is;

    std::size_t n = 0;
    is >> n;
    if (!is)
        return is;

    std::vector<T, Alloc> tmp(n);
    for (std::size_t i = 0; i != n; ++i)
        is >> std::ws >> tmp[i];
    if (is)
        vec = std::move(tmp);

    return is;
}

} // namespace mckl::internal

template <typename CharT, typename Traits, typename T, std::size_t N>
inline std::basic_ostream<CharT, Traits> &operator<<(
    std::basic_ostream<CharT, Traits> &os, const std::array<T, N> &ary)
{
    return internal::ostream(os, ary);
}

template <typename CharT, typename Traits, typename T, std::size_t N>
inline std::basic_istream<CharT, Traits> &operator>>(
    std::basic_istream<CharT, Traits> &is, std::array<T, N> &ary)
{
    return internal::istream(is, ary);
}

template <typename CharT, typename Traits, typename T, typename Alloc>
inline std::basic_ostream<CharT, Traits> &operator<<(
    std::basic_ostream<CharT, Traits> &os, const std::vector<T, Alloc> &vec)
{
    return internal::ostream(os, vec);
}

template <typename CharT, typename Traits, typename T, typename Alloc>
inline std::basic_istream<CharT, Traits> &operator>>(
    std::basic_istream<CharT, Traits> &is, std::vector<T, Alloc> &vec)
{
    return internal::istream(is, vec);
}

} // namespace mckl

#endif // MCKL_INTERNAL_COMMON_HPP
