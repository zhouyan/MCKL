//============================================================================
// MCKL/include/mckl/internal/iostream.hpp
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

#ifndef MCKL_INTERNAL_IOSTREAM_HPP
#define MCKL_INTERNAL_IOSTREAM_HPP

#include <mckl/internal/config.h>
#include <array>
#include <iostream>
#include <list>
#include <vector>

namespace mckl {

namespace internal {

template <typename CharT, typename Traits, typename T, std::size_t N>
inline std::basic_ostream<CharT, Traits> &ostream(
    std::basic_ostream<CharT, Traits> &os, const std::array<T, N> &ary)
{
    if (!os) {
        return os;
    }

    os << N;
    if (!os) {
        return os;
    }

    for (const auto &v : ary) {
        os << ' ' << v;
    }

    return os;
}

template <typename CharT, typename Traits, typename T, std::size_t N>
inline std::basic_istream<CharT, Traits> &istream(
    std::basic_istream<CharT, Traits> &is, std::array<T, N> &ary)
{
    if (!is) {
        return is;
    }

    std::size_t n = 0;
    is >> n;
    if (!is) {
        return is;
    }

    std::array<T, N> tmp;

    n = std::min(n, N);
    for (std::size_t i = 0; i != n; ++i) {
        is >> std::ws >> tmp[i];
    }
    if (is) {
        ary = std::move(tmp);
    }

    return is;
}

template <typename CharT, typename Traits, typename T, typename Alloc>
inline std::basic_ostream<CharT, Traits> &ostream(
    std::basic_ostream<CharT, Traits> &os, const std::vector<T, Alloc> &vec)
{
    if (!os) {
        return os;
    }

    os << vec.size();
    if (!os) {
        return os;
    }

    for (const auto &v : vec) {
        os << ' ' << v;
    }

    return os;
}

template <typename CharT, typename Traits, typename T, typename Alloc>
inline std::basic_istream<CharT, Traits> &istream(
    std::basic_istream<CharT, Traits> &is, std::vector<T, Alloc> &vec)
{
    if (!is) {
        return is;
    }

    std::size_t n = 0;
    is >> n;
    if (!is) {
        return is;
    }

    std::vector<T, Alloc> tmp(n);
    for (std::size_t i = 0; i != n; ++i) {
        is >> std::ws >> tmp[i];
    }
    if (is) {
        vec = std::move(tmp);
    }

    return is;
}

template <typename CharT, typename Traits, typename T, typename Alloc>
inline std::basic_ostream<CharT, Traits> &ostream(
    std::basic_ostream<CharT, Traits> &os, const std::list<T, Alloc> &lst)
{
    if (!os) {
        return os;
    }

    os << lst.size();
    if (!os) {
        return os;
    }

    for (const auto &v : lst) {
        os << ' ' << v;
    }

    return os;
}

template <typename CharT, typename Traits, typename T, typename Alloc>
inline std::basic_istream<CharT, Traits> &istream(
    std::basic_istream<CharT, Traits> &is, std::list<T, Alloc> &lst)
{
    if (!is) {
        return is;
    }

    std::size_t n = 0;
    is >> n;
    if (!is) {
        return is;
    }

    std::list<T, Alloc> tmp;
    T val;
    for (std::size_t i = 0; i != n; ++i) {
        is >> std::ws >> val;
        tmp.push_back(val);
    }
    if (is) {
        lst = std::move(tmp);
    }

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

template <typename CharT, typename Traits, typename T, typename Alloc>
inline std::basic_ostream<CharT, Traits> &operator<<(
    std::basic_ostream<CharT, Traits> &os, const std::list<T, Alloc> &lst)
{
    return internal::ostream(os, lst);
}

template <typename CharT, typename Traits, typename T, typename Alloc>
inline std::basic_istream<CharT, Traits> &operator>>(
    std::basic_istream<CharT, Traits> &is, std::list<T, Alloc> &lst)
{
    return internal::istream(is, lst);
}

} // namespace mckl

#endif // MCKL_INTERNAL_IOSTREAM_HPP
