//============================================================================
// MCKL/include/mckl/internal/assert.hpp
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

#ifndef MCKL_INTERNAL_ASSERT_HPP
#define MCKL_INTERNAL_ASSERT_HPP

#include <mckl/internal/config.h>

#include <cassert>
#include <cmath>
#include <exception>
#include <iostream>
#include <limits>
#include <stdexcept>
#include <string>

#ifdef MCKL_CLANG
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wweak-vtables"
#endif

namespace mckl {

class RuntimeAssert : public std::runtime_error
{
  public:
    explicit RuntimeAssert(const char *msg) : std::runtime_error(msg) {}

    explicit RuntimeAssert(const std::string &msg) : std::runtime_error(msg) {}
}; // class RuntimeAssert

#if MCKL_NO_RUNTIME_ASSERT

inline void runtime_assert(bool, const char *, bool = false) {}

inline void runtime_assert(bool, const std::string &, bool = false) {}

#else // MCKL_NO_RUNTIME_ASSERT

inline void runtime_assert(bool cond, const char *msg, bool soft = false)
{
#if MCKL_RUNTIME_ASSERT_AS_EXCEPTION
    if (!cond)
        throw ::mckl::RuntimeAssert(msg);
#else  // MCKL_RUNTIME_ASSERT_AS_EXCEPTION
    if (!cond)
        std::cerr << "MCKL runtime assertion failed: " << msg << std::endl;
    if (!soft)
        assert(cond);
#endif // MCKL_RUNTIME_ASSERT_AS_EXCEPTION
}

inline void runtime_assert(
    bool cond, const std::string &msg, bool soft = false)
{
    runtime_assert(cond, msg.c_str(), soft);
}

#endif // MCKL_NO_RUNTIME_ASSERT

template <typename Except>
inline void runtime_assert(bool cond, const char *msg)
{
    if (!cond)
        throw Except(msg);
}

template <typename Except>
inline void runtime_assert(bool cond, const std::string &msg)
{
    runtime_assert<Except>(cond, msg.c_str());
}

namespace internal {

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
    constexpr std::uintmax_t nmax =
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
    constexpr std::uintmax_t nmax =
        static_cast<std::uintmax_t>(std::numeric_limits<IntType>::max());
    constexpr std::uintmax_t smax =
        static_cast<std::uintmax_t>(std::numeric_limits<SizeType>::max());

    size_check<IntType>(n, f, std::integral_constant<bool, (nmax < smax)>());
}

#endif // MCKL_NO_RUNTIME_ASSERT

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
    return is_nullptr(ptr,
        std::integral_constant<bool,
            (std::is_pointer<T>::value ||
                std::is_same<std::nullptr_t, std::remove_cv_t<T>>::value)>());
}

inline bool is_nullptr(std::nullptr_t) { return true; }

} // namespace mckl::internal

} // namespace mckl

#ifdef MCKL_CLANG
#pragma clang diagnostic pop
#endif

#endif // MCKL_INTERNAL_ASSERT_HPP
