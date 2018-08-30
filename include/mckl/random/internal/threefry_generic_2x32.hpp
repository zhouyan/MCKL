//============================================================================
// MCKL/include/mckl/random/internal/threefry_generic_2x32.hpp
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

#ifndef MCKL_RANDOM_INTERNAL_THREEFRY_GENERIC_2X32_HPP
#define MCKL_RANDOM_INTERNAL_THREEFRY_GENERIC_2X32_HPP

#include <mckl/random/internal/common.hpp>
#include <mckl/random/internal/threefry_common.hpp>
#include <mckl/random/internal/threefry_constants.hpp>
#include <mckl/random/increment.hpp>

#define MCKL_RANDOM_INTERNAL_THREEFRY_GENERIC_2X32_KBOX(N)                    \
    s0 += ThreefryKBox<T, K, N>::template key<0>(par);                        \
    s1 += ThreefryKBox<T, K, N>::template key<1>(par);

#define MCKL_RANDOM_INTERNAL_THREEFRY_GENERIC_2X32_RBOX(N)                    \
    {                                                                         \
        constexpr int L0 = Constants::rotate::value[0][(N - 1) % 8];          \
        constexpr int R0 = 32 - L0;                                           \
        s0 += s1;                                                             \
        s1 = (s1 << L0) | (s1 >> R0);                                         \
        s1 ^= s0;                                                             \
    }

#define MCKL_RANDOM_INTERNAL_THREEFRY_GENERIC_2X32_CYCLE_4(N)                 \
    MCKL_RANDOM_INTERNAL_THREEFRY_GENERIC_2X32_RBOX(N * 8 + 1);               \
    MCKL_RANDOM_INTERNAL_THREEFRY_GENERIC_2X32_RBOX(N * 8 + 2);               \
    MCKL_RANDOM_INTERNAL_THREEFRY_GENERIC_2X32_RBOX(N * 8 + 3);               \
    MCKL_RANDOM_INTERNAL_THREEFRY_GENERIC_2X32_RBOX(N * 8 + 4);               \
    MCKL_RANDOM_INTERNAL_THREEFRY_GENERIC_2X32_KBOX(N * 8 + 4);

#define MCKL_RANDOM_INTERNAL_THREEFRY_GENERIC_2X32_CYCLE_8(N)                 \
    MCKL_RANDOM_INTERNAL_THREEFRY_GENERIC_2X32_RBOX(N * 8 + 1);               \
    MCKL_RANDOM_INTERNAL_THREEFRY_GENERIC_2X32_RBOX(N * 8 + 2);               \
    MCKL_RANDOM_INTERNAL_THREEFRY_GENERIC_2X32_RBOX(N * 8 + 3);               \
    MCKL_RANDOM_INTERNAL_THREEFRY_GENERIC_2X32_RBOX(N * 8 + 4);               \
    MCKL_RANDOM_INTERNAL_THREEFRY_GENERIC_2X32_KBOX(N * 8 + 4);               \
    MCKL_RANDOM_INTERNAL_THREEFRY_GENERIC_2X32_RBOX(N * 8 + 5);               \
    MCKL_RANDOM_INTERNAL_THREEFRY_GENERIC_2X32_RBOX(N * 8 + 6);               \
    MCKL_RANDOM_INTERNAL_THREEFRY_GENERIC_2X32_RBOX(N * 8 + 7);               \
    MCKL_RANDOM_INTERNAL_THREEFRY_GENERIC_2X32_RBOX(N * 8 + 8);               \
    MCKL_RANDOM_INTERNAL_THREEFRY_GENERIC_2X32_KBOX(N * 8 + 8);

#define MCKL_RANDOM_INTERNAL_THREEFRY_GENERIC_2X32_ROUND_20                   \
    T s0 = std::get<0>(buf.s);                                                \
    T s1 = std::get<1>(buf.s);                                                \
    MCKL_RANDOM_INTERNAL_THREEFRY_GENERIC_2X32_KBOX(0)                        \
    MCKL_RANDOM_INTERNAL_THREEFRY_GENERIC_2X32_CYCLE_8(0)                     \
    MCKL_RANDOM_INTERNAL_THREEFRY_GENERIC_2X32_CYCLE_8(1)                     \
    MCKL_RANDOM_INTERNAL_THREEFRY_GENERIC_2X32_CYCLE_4(2)                     \
    std::get<0>(buf.s) = s0;                                                  \
    std::get<1>(buf.s) = s1;

namespace mckl {

namespace internal {

template <typename T>
class Threefry2x32GeneratorGenericImpl
{
    static_assert(std::numeric_limits<T>::digits == 32,
        "**Threefry2x32GeneratorGenericImpl** used with T other than a "
        "32-bit unsigned integers");

    static constexpr std::size_t K = 2;
    static constexpr std::size_t Rounds = 20;
    using Constants = ThreefryConstants<T, 2>;

  public:
    static void eval(
        const void *plain, void *cipher, const std::array<T, K + 4> &par)
    {
        alignas(MCKL_ALIGNMENT) union {
            std::array<T, K> s;
            std::array<char, sizeof(T) * K> r;
        } buf;

        std::memcpy(buf.s.data(), plain, sizeof(T) * K);
        union_le<char>(buf.s);
        MCKL_RANDOM_INTERNAL_THREEFRY_GENERIC_2X32_ROUND_20
        union_le<T>(buf.r);
        std::memcpy(cipher, buf.s.data(), sizeof(T) * K);
    }

    template <typename ResultType>
    static void eval(
        Counter<T, K> &ctr, ResultType *r, const std::array<T, K + 4> &par)
    {
        alignas(MCKL_ALIGNMENT) union {
            std::array<T, K> s;
            Counter<T, K> c;
            std::array<ResultType, sizeof(T) * K / sizeof(ResultType)> r;
        } buf;

        MCKL_INLINE_CALL increment(ctr);
        buf.c = ctr;
#if MCKL_REQUIRE_ENDIANNESS_NEUTURAL
        union_le<typename Counter<T, K>::value_type>(buf.s);
#endif
        MCKL_RANDOM_INTERNAL_THREEFRY_GENERIC_2X32_ROUND_20
#if MCKL_REQUIRE_ENDIANNESS_NEUTURAL
        union_le<T>(buf.r);
#endif
        std::memcpy(r, buf.r.data(), sizeof(T) * K);
    }

    template <typename ResultType>
    static void eval(Counter<T, K> &ctr, std::size_t n, ResultType *r,
        const std::array<T, K + 4> &par)
    {
        constexpr std::size_t R = sizeof(T) * K / sizeof(ResultType);

        for (std::size_t i = 0; i != n; ++i, r += R) {
            eval(ctr, r, par);
        }
    }
}; // class Threefry2x32GeneratorGenericImpl

} // namespace mckl::internal

} // namespace mckl

#endif // MCKL_RANDOM_INTERNAL_THREEFRY_GENERIC_2X32_HPP
