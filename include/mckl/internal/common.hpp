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
#include <climits>
#include <cmath>
#include <complex>
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

inline bool is_big_endian()
{
    union {
        char c[sizeof(int)];
        int i;
    } buf;

    buf.i = 0x01 << (sizeof(int) * CHAR_BIT - CHAR_BIT);

    return buf.c[0] == 0x01;
}

template <int, int, typename U>
inline U swap_bytes(U, std::false_type)
{
    return 0;
}

template <int Bytes, int N, typename U>
inline U swap_bytes(U u, std::true_type)
{
    static constexpr int bits = sizeof(U) * CHAR_BIT;
    static constexpr int p = Bytes * CHAR_BIT;
    static constexpr int r = p * N;
    static constexpr int l = bits - r - p;
    static constexpr U mask = (~const_zero<U>()) >> (bits - p);

    return ((u & mask) << l) +
        swap_bytes<Bytes, N + 1>(
            u >> p, std::integral_constant<bool, (r + p < bits)>());
}

template <int Bytes, typename T>
inline T swap_bytes(T x)
{
    static_assert(sizeof(T) % Bytes == 0,
        "**swap_bytes** sizeof(T) is not divisible by Bytes");

    using U = typename std::make_unsigned<T>::type;

    return static_cast<T>(swap_bytes<Bytes, 0>(
        static_cast<U>(x), std::integral_constant < bool, Bytes<sizeof(U)>()));
}

#if MCKL_HAS_LITTLE_ENDIAN

template <typename, typename T, std::size_t K>
inline void union_le(std::array<T, K> &)
{
}

#else // MCKL_HAS_LITTLE_ENDIAN

// Let in the following union
// union {
//    U u[sizeof(T) * sizeof(K) / sizeof(U);
//    T t[K]
// } buf;
// buf.u is given the exact value
// transform buf.t such that its value will be as if we are using little endian
template <typename U, typename T, std::size_t K>
inline void union_le(std::array<T, K> &, std::false_type, std::false_type)
{
}

template <typename U, typename T, std::size_t K>
inline void union_le(std::array<T, K> &buf, std::false_type, std::true_type)
{
    static constexpr std::size_t m = sizeof(U) / sizeof(T);
    static constexpr std::size_t l = K / m;

    T *b = buf.data();
    for (std::size_t i = 0; i != l; ++i, b += m)
        std::reverse(b, b + m);
}

template <typename U, typename T, std::size_t K>
inline void union_le(std::array<T, K> &buf, std::true_type, std::false_type)
{
    for (std::size_t i = 0; i != K; ++i)
        buf[i] = swap_bytes<sizeof(U)>(buf[i]);
}

template <typename U, typename T, std::size_t K>
inline void union_le(std::array<T, K> &buf)
{
    static_assert(sizeof(U) % sizeof(T) == 0 || sizeof(T) % sizeof(U) == 0,
        "**union_le** called with sizeof(U) and sizeof(T) with neither of "
        "them a multiple of the other");

#if MCKL_HAS_BIG_ENDIAN
    union_le<U>(buf, std::integral_constant<bool, (sizeof(U) < sizeof(T))>(),
        std::integral_constant<bool, (sizeof(U) > sizeof(T))>());
#else
    if (is_big_endian()) {
        union_le<U>(buf,
            std::integral_constant<bool, (sizeof(U) < sizeof(T))>(),
            std::integral_constant<bool, (sizeof(U) > sizeof(T))>());
    }
#endif
}

#endif // MCKL_HAS_LITTLE_ENDIAN

template <typename CharT, typename Traits, typename T, std::size_t N>
inline std::basic_ostream<CharT, Traits> &ostream(
    std::basic_ostream<CharT, Traits> &os, const std::array<T, N> &ary)
{
    if (!os)
        return os;

    os << N;
    if (!os)
        return os;

    for (const auto &v : ary)
        os << ' ' << v;

    return os;
}

template <typename CharT, typename Traits, typename T, std::size_t N>
inline std::basic_istream<CharT, Traits> &istream(
    std::basic_istream<CharT, Traits> &is, std::array<T, N> &ary)
{
    if (!is)
        return is;

    std::size_t n = 0;
    is >> n;
    if (!is)
        return is;

    std::array<T, N> tmp;

    n = std::min(n, N);
    for (std::size_t i = 0; i != n; ++i)
        is >> std::ws >> tmp[i];
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

#ifdef MCKL_GCC
#if MCKL_GCC_VERSION >= 60000
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wignored-attributes"
#endif
#endif

#if MCKL_HAS_SSE2

template <std::size_t i0, std::size_t i1, std::size_t i2, std::size_t i3,
    std::size_t N>
static void transpose4x32_si128(std::array<__m128i, N> &s)
{
    __m128i s0 = _mm_unpacklo_epi32(std::get<i0>(s), std::get<i1>(s));
    __m128i s1 = _mm_unpacklo_epi32(std::get<i2>(s), std::get<i3>(s));
    __m128i t0 = _mm_unpackhi_epi32(std::get<i0>(s), std::get<i1>(s));
    __m128i t1 = _mm_unpackhi_epi32(std::get<i2>(s), std::get<i3>(s));

    std::get<i0>(s) = _mm_unpacklo_epi64(s0, s1);
    std::get<i1>(s) = _mm_unpackhi_epi64(s0, s1);
    std::get<i2>(s) = _mm_unpacklo_epi64(t0, t1);
    std::get<i3>(s) = _mm_unpackhi_epi64(t0, t1);
}

template <std::size_t i0, std::size_t i1, std::size_t N>
static void transpose2x64_si128(std::array<__m128i, N> &s)
{
    __m128i s0 = _mm_unpacklo_epi64(std::get<i0>(s), std::get<i1>(s));
    __m128i t0 = _mm_unpackhi_epi64(std::get<i0>(s), std::get<i1>(s));

    std::get<i0>(s) = s0;
    std::get<i1>(s) = t0;
}

#endif // MCKL_HAS_SSE2

#if MCKL_HAS_AVX2

template <std::size_t i0, std::size_t i1, std::size_t i2, std::size_t i3,
    std::size_t i4, std::size_t i5, std::size_t i6, std::size_t i7,
    std::size_t N>
static void transpose8x32_si256(std::array<__m256i, N> &s)
{
    __m256i s0 = _mm256_unpacklo_epi32(std::get<i0>(s), std::get<i1>(s));
    __m256i s1 = _mm256_unpacklo_epi32(std::get<i2>(s), std::get<i3>(s));
    __m256i s2 = _mm256_unpacklo_epi32(std::get<i4>(s), std::get<i5>(s));
    __m256i s3 = _mm256_unpacklo_epi32(std::get<i6>(s), std::get<i7>(s));
    __m256i t0 = _mm256_unpackhi_epi32(std::get<i0>(s), std::get<i1>(s));
    __m256i t1 = _mm256_unpackhi_epi32(std::get<i2>(s), std::get<i3>(s));
    __m256i t2 = _mm256_unpackhi_epi32(std::get<i4>(s), std::get<i5>(s));
    __m256i t3 = _mm256_unpackhi_epi32(std::get<i6>(s), std::get<i7>(s));

    __m256i u0 = _mm256_unpacklo_epi64(s0, s1);
    __m256i u1 = _mm256_unpacklo_epi64(s2, s3);
    __m256i u2 = _mm256_unpacklo_epi64(t0, t1);
    __m256i u3 = _mm256_unpacklo_epi64(t2, t3);
    __m256i v0 = _mm256_unpackhi_epi64(s0, s1);
    __m256i v1 = _mm256_unpackhi_epi64(s2, s3);
    __m256i v2 = _mm256_unpackhi_epi64(t0, t1);
    __m256i v3 = _mm256_unpackhi_epi64(t2, t3);

    std::get<i0>(s) = _mm256_permute2x128_si256(u0, u1, 0x20);
    std::get<i1>(s) = _mm256_permute2x128_si256(v0, v1, 0x20);
    std::get<i2>(s) = _mm256_permute2x128_si256(u2, u3, 0x20);
    std::get<i3>(s) = _mm256_permute2x128_si256(v2, v3, 0x20);
    std::get<i4>(s) = _mm256_permute2x128_si256(u0, u1, 0x31);
    std::get<i5>(s) = _mm256_permute2x128_si256(v0, v1, 0x31);
    std::get<i6>(s) = _mm256_permute2x128_si256(u2, u3, 0x31);
    std::get<i7>(s) = _mm256_permute2x128_si256(v2, v3, 0x31);
}

template <std::size_t i0, std::size_t i1, std::size_t i2, std::size_t i3,
    std::size_t N>
static void transpose4x64_si256(std::array<__m256i, N> &s)
{
    __m256i s0 = _mm256_unpacklo_epi64(std::get<i0>(s), std::get<i1>(s));
    __m256i s1 = _mm256_unpacklo_epi64(std::get<i2>(s), std::get<i3>(s));
    __m256i t0 = _mm256_unpackhi_epi64(std::get<i0>(s), std::get<i1>(s));
    __m256i t1 = _mm256_unpackhi_epi64(std::get<i2>(s), std::get<i3>(s));

    std::get<i0>(s) = _mm256_permute2x128_si256(s0, s1, 0x20);
    std::get<i1>(s) = _mm256_permute2x128_si256(t0, t1, 0x20);
    std::get<i2>(s) = _mm256_permute2x128_si256(s0, s1, 0x31);
    std::get<i3>(s) = _mm256_permute2x128_si256(t0, t1, 0x31);
}

#endif // MCKL_HAS_AVX2

#ifdef MCKL_GCC
#if MCKL_GCC_VERSION >= 60000
#pragma GCC diagnostic pop
#endif
#endif

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
