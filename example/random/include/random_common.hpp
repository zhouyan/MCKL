//============================================================================
// MCKL/example/random/include/random_common.hpp
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

#ifndef MCKL_EXAMPLE_RANDOM_COMMON_HPP
#define MCKL_EXAMPLE_RANDOM_COMMON_HPP

#include <mckl/random/internal/common.hpp>
#include <mckl/utility/stop_watch.hpp>

template <typename>
inline std::string random_typename();

template <>
inline std::string random_typename<float>()
{
    return "float";
}

template <>
inline std::string random_typename<double>()
{
    return "double";
}

template <>
inline std::string random_typename<long double>()
{
    return "long double";
}

template <>
inline std::string random_typename<std::int32_t>()
{
    return "int32_t";
}

template <>
inline std::string random_typename<std::uint32_t>()
{
    return "uint32_t";
}

template <>
inline std::string random_typename<std::int64_t>()
{
    return "int64_t";
}

template <>
inline std::string random_typename<std::uint64_t>()
{
    return "uint64_t";
}

template <>
inline std::string random_typename<mckl::Closed>()
{
    return "Closed";
}

template <>
inline std::string random_typename<mckl::Open>()
{
    return "Open";
}

inline std::string random_pass(bool pass)
{
    return pass ? "Passed" : "Failed";
}

#endif // MCKL_EXAMPLE_RANDOM_COMMON_HPP
