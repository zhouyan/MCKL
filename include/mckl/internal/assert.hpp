//============================================================================
// MCKL/include/mckl/internal/assert.hpp
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

#ifndef MCKL_INTERNAL_ASSERT_HPP
#define MCKL_INTERNAL_ASSERT_HPP

#include <mckl/internal/config.h>

#include <cassert>
#include <cstdio>
#include <limits>
#include <stdexcept>
#include <string>

#ifdef MCKL_CLANG
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wweak-vtables"
#endif

namespace mckl
{

class RuntimeAssert : public std::runtime_error
{
    public:
    explicit RuntimeAssert(const char *msg) : std::runtime_error(msg) {}

    explicit RuntimeAssert(const std::string &msg) : std::runtime_error(msg) {}
}; // class RuntimeAssert

#if MCKL_NO_RUNTIME_ASSERT

inline void runtime_assert(bool, const char *, bool) {}

#else // MCKL_NO_RUNTIME_ASSERT

inline void runtime_assert(bool cond, const char *msg, bool soft = false)
{
#if MCKL_RUNTIME_ASSERT_AS_EXCEPTION
    if (!cond) {
        throw ::mckl::RuntimeAssert(msg);
    }
#else  // MCKL_RUNTIME_ASSERT_AS_EXCEPTION
    if (!cond) {
        std::fprintf(stderr, "MCKL runtime assertion failed:%s\n", msg);
        std::fflush(stderr);
    }
    if (!soft)
        assert(cond);
#endif // MCKL_RUNTIME_ASSERT_AS_EXCEPTION
}

#endif // MCKL_NO_RUNTIME_ASSERT

inline void runtime_assert(
    bool cond, const std::string &msg, bool soft = false)
{
    runtime_assert(cond, msg.c_str(), soft);
}

namespace internal
{

#if MCKL_NO_RUNTIME_ASSERT

template <typename IntType, typename SizeType>
inline void size_check(SizeType, const char *)
{
}

#else // MCKL_NO_RUNTIME_ASSERT

template <typename IntType, typename SizeType>
inline void size_check(SizeType, const char *, std::false_type)
{
}

template <typename IntType, typename SizeType>
inline void size_check(SizeType n, const char *f, std::true_type)
{
    static constexpr std::uintmax_t nmax =
        static_cast<std::uintmax_t>(std::numeric_limits<IntType>::max());

    std::string msg;
    msg += "**";
    msg += f;
    msg += "** input size too large";

    runtime_assert((static_cast<std::uintmax_t>(n) <= nmax), msg.c_str());
}

template <typename IntType, typename SizeType>
inline void size_check(SizeType n, const char *f)
{
    static constexpr std::uintmax_t nmax =
        static_cast<std::uintmax_t>(std::numeric_limits<IntType>::max());
    static constexpr std::uintmax_t smax =
        static_cast<std::uintmax_t>(std::numeric_limits<SizeType>::max());

    size_check<IntType>(n, f, std::integral_constant<bool, (nmax < smax)>());
}

#endif // MCKL_NO_RUNTIME_ASSERT

template <typename T>
inline bool is_equal(const T &a, const T &b, std::true_type)
{
    return !(a < b || a > b);
}

template <typename T>
inline bool is_equal(const T &a, const T &b, std::false_type)
{
    return a == b;
}

template <typename T>
inline bool is_equal(const T &a, const T &b)
{
    return is_equal(a, b, std::is_floating_point<T>());
}

template <typename T>
inline bool is_zero(const T &a)
{
    return is_equal(a, static_cast<T>(0));
}

template <typename T>
inline bool is_one(const T &a)
{
    return is_equal(a, static_cast<T>(1));
}

template <typename T>
inline bool is_negative(const T &, std::true_type)
{
    return false;
}

template <typename T>
inline bool is_negative(const T &a, std::false_type)
{
    return a < 0;
}

template <typename T>
inline bool is_negative(const T &a)
{
    return is_negative(a, std::is_unsigned<T>());
}

template <typename T>
inline bool is_nullptr(T ptr, std::true_type)
{
    return ptr == nullptr;
}

template <typename T>
inline bool is_nullptr(T, std::false_type)
{
    return false;
}

template <typename T>
inline bool is_nullptr(T ptr)
{
    return is_nullptr(
        ptr, std::integral_constant<bool,
                 (std::is_pointer<T>::value ||
                     std::is_same<std::nullptr_t,
                         typename std::remove_cv<T>::type>::value)>());
}

inline bool is_nullptr(std::nullptr_t) { return true; }

} // namespace mckl::internal

} // namespace mckl

#ifdef MCKL_CLANG
#pragma clang diagnostic pop
#endif

#endif // MCKL_INTERNAL_ASSERT_HPP
