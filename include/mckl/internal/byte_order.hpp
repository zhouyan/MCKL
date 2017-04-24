//============================================================================
// MCKL/include/mckl/internal/byte_order.hpp
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

#ifndef MCKL_INTERNAL_BYTE_ORDER_HPP
#define MCKL_INTERNAL_BYTE_ORDER_HPP

#include <mckl/internal/config.h>
#include <mckl/math/constants.hpp>
#include <array>
#include <climits>

namespace mckl {

namespace internal {

inline bool is_little_endian()
{
    union {
        char c[sizeof(int)];
        int i;
    } buf;

    buf.i = 0x01;

    return buf.c[0] == 0x01;
}

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
    constexpr int bits = sizeof(U) * CHAR_BIT;
    constexpr int p = Bytes * CHAR_BIT;
    constexpr int q = p < bits ? p : 0;
    constexpr int r = p * N;
    constexpr int l = bits - r - p;
    constexpr U mask = (~const_zero<U>()) >> (bits - p);

    return ((u & mask) << l) |
        swap_bytes<Bytes, N + 1>(
            u >> q, std::integral_constant<bool, (r + p < bits)>());
}

template <int Bytes, typename T>
inline T swap_bytes(T x)
{
    static_assert(sizeof(T) % Bytes == 0,
        "**swap_bytes** sizeof(T) is not divisible by Bytes");

    using U = std::make_unsigned_t<T>;

    return static_cast<T>(swap_bytes<Bytes, 0>(static_cast<U>(x),
        std::integral_constant<bool, Bytes <= sizeof(U)>()));
}

#if MCKL_HAS_LITTLE_ENDIAN

// Let in the following union
// union {
//    U v[sizeof(T) * sizeof(K) / sizeof(U);
//    T r[K]
// } buf;
// buf.v is given the exact value
// transform buf.r such that its value will be as if we are using little endian
template <typename, typename T>
inline void union_le(std::size_t, T *)
{
}

template <typename, typename T>
inline void union_le(T &)
{
}

#else // MCKL_HAS_LITTLE_ENDIAN

template <typename, typename T>
inline void union_le(std::size_t, T *, std::false_type, std::false_type)
{
}

template <typename U, typename T>
inline void union_le(std::size_t n, T *r, std::false_type, std::true_type)
{
    const std::size_t m = sizeof(U) / sizeof(T);
    const std::size_t l = n / m;

    for (std::size_t i = 0; i != l; ++i, r += m)
        std::reverse(r, r + m);
}

template <typename U, typename T>
inline void union_le(std::size_t n, T *r, std::true_type, std::false_type)
{
    for (std::size_t i = 0; i != n; ++i)
        r[i] = swap_bytes<sizeof(U)>(r[i]);
}

template <typename U, typename T>
inline void union_le(std::size_t n, T *r)
{
    static_assert(sizeof(U) % sizeof(T) == 0 || sizeof(T) % sizeof(U) == 0,
        "**union_le** used with sizeof(U) and sizeof(T) with neither of them "
        "a multiple of the other");

#if MCKL_HAS_BIG_ENDIAN
    union_le<U>(n, r, std::integral_constant<bool, (sizeof(U) < sizeof(T))>(),
        std::integral_constant<bool, (sizeof(U) > sizeof(T))>());
#else
    if (is_little_endian())
        return;
    union_le<U>(n, r, std::integral_constant<bool, (sizeof(U) < sizeof(T))>(),
        std::integral_constant<bool, (sizeof(U) > sizeof(T))>());
#endif
}

template <typename U, typename T>
class UnionLE
{
  public:
    static void eval(T &r) { union_le<U>(1, &r); }
}; // class UnionLE

template <typename U, typename T, std::size_t K>
class UnionLE<U, std::array<T, K>>
{
  public:
    static void eval(std::array<T, K> &r) { union_le<U>(K, r.data()); }
}; // class UnionLE

template <typename U, typename T>
inline void union_le(T &r)
{
    UnionLE<U, T>::eval(r);
}

#endif // MCKL_HAS_LITTLE_ENDIAN

} // namespace mckl::internal

} // namespace mckl

#endif // MCKL_INTERNAL_BYTE_ORDER_HPP
