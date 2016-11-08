//============================================================================
// MCKL/include/mckl/random/internal/philox_generic_2x.hpp
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

#ifndef MCKL_RANDOM_INTERNAL_PHILOX_GENERIC_2X_HPP
#define MCKL_RANDOM_INTERNAL_PHILOX_GENERIC_2X_HPP

#include <mckl/random/internal/common.hpp>
#include <mckl/random/internal/philox_common.hpp>
#include <mckl/random/internal/philox_constants.hpp>
#include <mckl/random/increment.hpp>

#define MCKL_RANDOM_INTERNAL_PHILOX_GENERIC_2X_RBOX(N)                        \
    {                                                                         \
        constexpr T w0 = Constants::weyl::value[0] * static_cast<T>(N - 1);   \
        constexpr T m0 = Constants::multiplier::value[0];                     \
                                                                              \
        t1 = s1;                                                              \
        s1 = PhiloxHiLo<T>::eval(s0, m0, t0);                                 \
        s0 = (k0 + w0) ^ (t1 ^ t0);                                           \
    }

#define MCKL_RANDOM_INTERNAL_PHILOX_GENERIC_2X_ROUND_10                       \
    T k0 = std::get<0>(key);                                                  \
    T s0 = std::get<0>(buf.s);                                                \
    T s1 = std::get<1>(buf.s);                                                \
    T t0;                                                                     \
    T t1;                                                                     \
    MCKL_RANDOM_INTERNAL_PHILOX_GENERIC_2X_RBOX(0x1)                          \
    MCKL_RANDOM_INTERNAL_PHILOX_GENERIC_2X_RBOX(0x2)                          \
    MCKL_RANDOM_INTERNAL_PHILOX_GENERIC_2X_RBOX(0x3)                          \
    MCKL_RANDOM_INTERNAL_PHILOX_GENERIC_2X_RBOX(0x4)                          \
    MCKL_RANDOM_INTERNAL_PHILOX_GENERIC_2X_RBOX(0x5)                          \
    MCKL_RANDOM_INTERNAL_PHILOX_GENERIC_2X_RBOX(0x6)                          \
    MCKL_RANDOM_INTERNAL_PHILOX_GENERIC_2X_RBOX(0x7)                          \
    MCKL_RANDOM_INTERNAL_PHILOX_GENERIC_2X_RBOX(0x8)                          \
    MCKL_RANDOM_INTERNAL_PHILOX_GENERIC_2X_RBOX(0x9)                          \
    MCKL_RANDOM_INTERNAL_PHILOX_GENERIC_2X_RBOX(0xA)                          \
    std::get<0>(buf.s) = s0;                                                  \
    std::get<1>(buf.s) = s1;

namespace mckl
{

namespace internal
{

template <typename T, typename Constants>
class Philox2xGeneratorGenericImpl
{
    static constexpr std::size_t K = 2;
    static constexpr std::size_t Rounds = 10;

    public:
    static void eval(
        const void *plain, void *cipher, const std::array<T, K / 2> &key)
    {
        alignas(32) union {
            std::array<T, K> s;
            std::array<char, sizeof(T) * K> r;
        } buf;

        std::memcpy(buf.s.data(), plain, sizeof(T) * K);
        union_le<char>(buf.s);
        MCKL_RANDOM_INTERNAL_PHILOX_GENERIC_2X_ROUND_10
        union_le<T>(buf.r);
        std::memcpy(cipher, buf.s.data(), sizeof(T) * K);
    }

    template <typename ResultType>
    static void eval(
        Counter<T, K> &ctr, ResultType *r, const std::array<T, K / 2> &key)
    {
        alignas(32) union {
            std::array<T, K> s;
            Counter<T, K> c;
            std::array<ResultType, sizeof(T) * K / sizeof(ResultType)> r;
        } buf;

        MCKL_INLINE_CALL increment(ctr);
        buf.c = ctr;
#if MCKL_REQUIRE_ENDIANNESS_NEUTURAL
        union_le<typename Counter<T, K>::value_type>(buf.s);
#endif
        MCKL_RANDOM_INTERNAL_PHILOX_GENERIC_2X_ROUND_10
#if MCKL_REQUIRE_ENDIANNESS_NEUTURAL
        union_le<T>(buf.r);
#endif
        std::memcpy(r, buf.r.data(), sizeof(T) * K);
    }

    template <typename ResultType>
    MCKL_NOINLINE static void eval(Counter<T, K> &ctr, std::size_t n,
        ResultType *r, const std::array<T, K / 2> &key)
    {
        constexpr std::size_t R = sizeof(T) * K / sizeof(ResultType);

        for (std::size_t i = 0; i != n; ++i, r += R)
            eval(ctr, r, key);
    }
}; // class Philox2xGeneratorGenericImpl

} // namespace mckl::internal

} // namespace mckl

#endif // MCKL_RANDOM_INTERNAL_PHILOX_GENERIC_2X_HPP
