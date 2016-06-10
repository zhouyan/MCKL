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

#include <mckl/random/rng.hpp>
#include <mckl/utility/stop_watch.hpp>

#define MCKL_EXAMPLE_RANDOM_MAIN(prg, NVal, MVal)                             \
    int main(int argc, char **argv)                                           \
    {                                                                         \
        --argc;                                                               \
        ++argv;                                                               \
                                                                              \
        std::size_t N = NVal;                                                 \
        if (argc > 0) {                                                       \
            std::size_t n = static_cast<std::size_t>(std::atoi(*argv));       \
            if (n != 0) {                                                     \
                N = n;                                                        \
                --argc;                                                       \
                ++argv;                                                       \
            }                                                                 \
        }                                                                     \
                                                                              \
        std::size_t M = MVal;                                                 \
        if (argc > 0) {                                                       \
            std::size_t m = static_cast<std::size_t>(std::atoi(*argv));       \
            if (m != 0) {                                                     \
                M = m;                                                        \
                --argc;                                                       \
                ++argv;                                                       \
            }                                                                 \
        }                                                                     \
                                                                              \
        random_##prg(N, M, argc, argv);                                       \
                                                                              \
        return 0;                                                             \
    }

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
inline std::string random_typename<int>()
{
    return "int";
}

template <>
inline std::string random_typename<unsigned>()
{
    return "unsigned";
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
