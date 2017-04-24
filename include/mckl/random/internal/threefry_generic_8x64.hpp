//============================================================================
// MCKL/include/mckl/random/internal/threefry_generic_8x64.hpp
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

#ifndef MCKL_RANDOM_INTERNAL_THREEFRY_GENERIC_8X64_HPP
#define MCKL_RANDOM_INTERNAL_THREEFRY_GENERIC_8X64_HPP

#include <mckl/random/internal/common.hpp>
#include <mckl/random/internal/threefry_common.hpp>
#include <mckl/random/internal/threefry_constants.hpp>
#include <mckl/random/increment.hpp>

#define MCKL_RANDOM_INTERNAL_THREEFRY_GENERIC_8X64_KBOX(N)                    \
    s0 += ThreefryKBox<T, K, N>::template key<0>(par);                        \
    s1 += ThreefryKBox<T, K, N>::template key<1>(par);                        \
    s2 += ThreefryKBox<T, K, N>::template key<2>(par);                        \
    s3 += ThreefryKBox<T, K, N>::template key<3>(par);                        \
    s4 += ThreefryKBox<T, K, N>::template key<4>(par);                        \
    s5 += ThreefryKBox<T, K, N>::template key<5>(par);                        \
    s6 += ThreefryKBox<T, K, N>::template key<6>(par);                        \
    s7 += ThreefryKBox<T, K, N>::template key<7>(par);

#define MCKL_RANDOM_INTERNAL_THREEFRY_GENERIC_8X64_RBOX(N)                    \
    {                                                                         \
        constexpr int L0 = Constants::rotate::value[0][(N - 1) % 8];          \
        constexpr int L1 = Constants::rotate::value[1][(N - 1) % 8];          \
        constexpr int L2 = Constants::rotate::value[2][(N - 1) % 8];          \
        constexpr int L3 = Constants::rotate::value[3][(N - 1) % 8];          \
        constexpr int R0 = 64 - L0;                                           \
        constexpr int R1 = 64 - L1;                                           \
        constexpr int R2 = 64 - L2;                                           \
        constexpr int R3 = 64 - L3;                                           \
        t0 = s0 + s1;                                                         \
        t2 = s2 + s3;                                                         \
        t4 = s4 + s5;                                                         \
        t6 = s6 + s7;                                                         \
        t1 = (s1 << L0) | (s1 >> R0);                                         \
        t3 = (s3 << L1) | (s3 >> R1);                                         \
        t5 = (s5 << L2) | (s5 >> R2);                                         \
        t7 = (s7 << L3) | (s7 >> R3);                                         \
        s0 = t2;                                                              \
        s2 = t4;                                                              \
        s4 = t6;                                                              \
        s6 = t0;                                                              \
        s1 = t0 ^ t1;                                                         \
        s3 = t6 ^ t7;                                                         \
        s5 = t4 ^ t5;                                                         \
        s7 = t2 ^ t3;                                                         \
    }

#define MCKL_RANDOM_INTERNAL_THREEFRY_GENERIC_8X64_CYCLE_4(N)                 \
    MCKL_RANDOM_INTERNAL_THREEFRY_GENERIC_8X64_RBOX(N * 8 + 1);               \
    MCKL_RANDOM_INTERNAL_THREEFRY_GENERIC_8X64_RBOX(N * 8 + 2);               \
    MCKL_RANDOM_INTERNAL_THREEFRY_GENERIC_8X64_RBOX(N * 8 + 3);               \
    MCKL_RANDOM_INTERNAL_THREEFRY_GENERIC_8X64_RBOX(N * 8 + 4);               \
    MCKL_RANDOM_INTERNAL_THREEFRY_GENERIC_8X64_KBOX(N * 8 + 4);

#define MCKL_RANDOM_INTERNAL_THREEFRY_GENERIC_8X64_CYCLE_8(N)                 \
    MCKL_RANDOM_INTERNAL_THREEFRY_GENERIC_8X64_RBOX(N * 8 + 1);               \
    MCKL_RANDOM_INTERNAL_THREEFRY_GENERIC_8X64_RBOX(N * 8 + 2);               \
    MCKL_RANDOM_INTERNAL_THREEFRY_GENERIC_8X64_RBOX(N * 8 + 3);               \
    MCKL_RANDOM_INTERNAL_THREEFRY_GENERIC_8X64_RBOX(N * 8 + 4);               \
    MCKL_RANDOM_INTERNAL_THREEFRY_GENERIC_8X64_KBOX(N * 8 + 4);               \
    MCKL_RANDOM_INTERNAL_THREEFRY_GENERIC_8X64_RBOX(N * 8 + 5);               \
    MCKL_RANDOM_INTERNAL_THREEFRY_GENERIC_8X64_RBOX(N * 8 + 6);               \
    MCKL_RANDOM_INTERNAL_THREEFRY_GENERIC_8X64_RBOX(N * 8 + 7);               \
    MCKL_RANDOM_INTERNAL_THREEFRY_GENERIC_8X64_RBOX(N * 8 + 8);               \
    MCKL_RANDOM_INTERNAL_THREEFRY_GENERIC_8X64_KBOX(N * 8 + 8);

#define MCKL_RANDOM_INTERNAL_THREEFRY_GENERIC_8X64_ROUND_20                   \
    MCKL_RANDOM_INTERNAL_THREEFRY_GENERIC_8X64_KBOX(0)                        \
    MCKL_RANDOM_INTERNAL_THREEFRY_GENERIC_8X64_CYCLE_8(0)                     \
    MCKL_RANDOM_INTERNAL_THREEFRY_GENERIC_8X64_CYCLE_8(1)                     \
    MCKL_RANDOM_INTERNAL_THREEFRY_GENERIC_8X64_CYCLE_4(2)

#define MCKL_RANDOM_INTERNAL_THREEFRY_GENERIC_8X64_ROUND_72                   \
    MCKL_RANDOM_INTERNAL_THREEFRY_GENERIC_8X64_KBOX(0)                        \
    MCKL_RANDOM_INTERNAL_THREEFRY_GENERIC_8X64_CYCLE_8(0)                     \
    MCKL_RANDOM_INTERNAL_THREEFRY_GENERIC_8X64_CYCLE_8(1)                     \
    MCKL_RANDOM_INTERNAL_THREEFRY_GENERIC_8X64_CYCLE_8(2)                     \
    MCKL_RANDOM_INTERNAL_THREEFRY_GENERIC_8X64_CYCLE_8(3)                     \
    MCKL_RANDOM_INTERNAL_THREEFRY_GENERIC_8X64_CYCLE_8(4)                     \
    MCKL_RANDOM_INTERNAL_THREEFRY_GENERIC_8X64_CYCLE_8(5)                     \
    MCKL_RANDOM_INTERNAL_THREEFRY_GENERIC_8X64_CYCLE_8(6)                     \
    MCKL_RANDOM_INTERNAL_THREEFRY_GENERIC_8X64_CYCLE_8(7)                     \
    MCKL_RANDOM_INTERNAL_THREEFRY_GENERIC_8X64_CYCLE_8(8)

#define MCKL_RANDOM_INTERNAL_THREEFRY_GENERIC_8X64_ROUND(L)                   \
    T s0 = std::get<0>(buf.s);                                                \
    T s1 = std::get<1>(buf.s);                                                \
    T s2 = std::get<2>(buf.s);                                                \
    T s3 = std::get<3>(buf.s);                                                \
    T s4 = std::get<4>(buf.s);                                                \
    T s5 = std::get<5>(buf.s);                                                \
    T s6 = std::get<6>(buf.s);                                                \
    T s7 = std::get<7>(buf.s);                                                \
    T t0;                                                                     \
    T t1;                                                                     \
    T t2;                                                                     \
    T t3;                                                                     \
    T t4;                                                                     \
    T t5;                                                                     \
    T t6;                                                                     \
    T t7;                                                                     \
    MCKL_RANDOM_INTERNAL_THREEFRY_GENERIC_8X64_ROUND_##L;                     \
    std::get<0>(buf.s) = s0;                                                  \
    std::get<1>(buf.s) = s1;                                                  \
    std::get<2>(buf.s) = s2;                                                  \
    std::get<3>(buf.s) = s3;                                                  \
    std::get<4>(buf.s) = s4;                                                  \
    std::get<5>(buf.s) = s5;                                                  \
    std::get<6>(buf.s) = s6;                                                  \
    std::get<7>(buf.s) = s7;

namespace mckl
{

namespace internal
{

template <typename T>
class Threefry8x64GeneratorGenericImpl
{
    static_assert(std::numeric_limits<T>::digits == 64,
        "**Threefry8x64GeneratorGenericImpl** used with T other than a "
        "64-bit unsigned integers");

    static constexpr std::size_t K = 8;
    static constexpr std::size_t Rounds = 20;
    using Constants = ThreefryConstants<T, 8>;

  public:
    static void eval(
        const void *plain, void *cipher, const std::array<T, K + 4> &par)
    {
        alignas(32) union {
            std::array<T, K> s;
            std::array<char, sizeof(T) * K> r;
        } buf;

        std::memcpy(buf.s.data(), plain, sizeof(T) * K);
        union_le<char>(buf.s);
        MCKL_RANDOM_INTERNAL_THREEFRY_GENERIC_8X64_ROUND(20)
        union_le<T>(buf.r);
        std::memcpy(cipher, buf.s.data(), sizeof(T) * K);
    }

    template <typename ResultType>
    static void eval(
        Counter<T, K> &ctr, ResultType *r, const std::array<T, K + 4> &par)
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
        MCKL_RANDOM_INTERNAL_THREEFRY_GENERIC_8X64_ROUND(20)
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

        for (std::size_t i = 0; i != n; ++i, r += R)
            eval(ctr, r, par);
    }
}; // class Threefry8x64GeneratorGenericImpl

template <typename T>
class Threefish512GeneratorGenericImpl
{
    static_assert(std::numeric_limits<T>::digits == 64,
        "**Threefish512GeneratorGenericImpl** used with T other than a "
        "64-bit unsigned integers");

    static constexpr std::size_t K = 8;
    static constexpr std::size_t Rounds = 72;
    using Constants = ThreefryConstants<T, 8>;

  public:
    static void eval(
        const void *plain, void *cipher, const std::array<T, K + 4> &par)
    {
        alignas(32) union {
            std::array<T, K> s;
            std::array<char, sizeof(T) * K> r;
        } buf;

        std::memcpy(buf.s.data(), plain, sizeof(T) * K);
        union_le<char>(buf.s);
        MCKL_RANDOM_INTERNAL_THREEFRY_GENERIC_8X64_ROUND(72)
        union_le<T>(buf.r);
        std::memcpy(cipher, buf.s.data(), sizeof(T) * K);
    }

    template <typename ResultType>
    static void eval(
        Counter<T, K> &ctr, ResultType *r, const std::array<T, K + 4> &par)
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
        MCKL_RANDOM_INTERNAL_THREEFRY_GENERIC_8X64_ROUND(72)
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

        for (std::size_t i = 0; i != n; ++i, r += R)
            eval(ctr, r, par);
    }
}; // class Threefish512GeneratorGenericImpl

} // namespace mckl::internal

} // namespace mckl

#endif // MCKL_RANDOM_INTERNAL_THREEFRY_GENERIC_8X64_HPP
