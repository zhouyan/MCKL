//============================================================================
// MCKL/include/mckl/random/internal/threefry_generic_16x64.hpp
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

#ifndef MCKL_RANDOM_INTERNAL_THREEFRY_GENERIC_16X64_HPP
#define MCKL_RANDOM_INTERNAL_THREEFRY_GENERIC_16X64_HPP

#include <mckl/random/internal/common.hpp>
#include <mckl/random/internal/threefry_common.hpp>
#include <mckl/random/internal/threefry_constants.hpp>
#include <mckl/random/increment.hpp>

#define MCKL_RANDOM_INTERNAL_THREEFRY_GENERIC_16X64_KBOX(N)                   \
    s0 += ThreefryKBox<T, K, N, Constants>::template key<0x0>(par);           \
    s1 += ThreefryKBox<T, K, N, Constants>::template key<0x1>(par);           \
    s2 += ThreefryKBox<T, K, N, Constants>::template key<0x2>(par);           \
    s3 += ThreefryKBox<T, K, N, Constants>::template key<0x3>(par);           \
    s4 += ThreefryKBox<T, K, N, Constants>::template key<0x4>(par);           \
    s5 += ThreefryKBox<T, K, N, Constants>::template key<0x5>(par);           \
    s6 += ThreefryKBox<T, K, N, Constants>::template key<0x6>(par);           \
    s7 += ThreefryKBox<T, K, N, Constants>::template key<0x7>(par);           \
    s8 += ThreefryKBox<T, K, N, Constants>::template key<0x8>(par);           \
    s9 += ThreefryKBox<T, K, N, Constants>::template key<0x9>(par);           \
    sA += ThreefryKBox<T, K, N, Constants>::template key<0xA>(par);           \
    sB += ThreefryKBox<T, K, N, Constants>::template key<0xB>(par);           \
    sC += ThreefryKBox<T, K, N, Constants>::template key<0xC>(par);           \
    sD += ThreefryKBox<T, K, N, Constants>::template key<0xD>(par);           \
    sE += ThreefryKBox<T, K, N, Constants>::template key<0xE>(par);           \
    sF += ThreefryKBox<T, K, N, Constants>::template key<0xF>(par);

#define MCKL_RANDOM_INTERNAL_THREEFRY_GENERIC_16X64_RBOX(N)                   \
    {                                                                         \
        constexpr int L0 = Constants::rotate::value[0][(N - 1) % 8];          \
        constexpr int L1 = Constants::rotate::value[1][(N - 1) % 8];          \
        constexpr int L2 = Constants::rotate::value[2][(N - 1) % 8];          \
        constexpr int L3 = Constants::rotate::value[3][(N - 1) % 8];          \
        constexpr int L4 = Constants::rotate::value[4][(N - 1) % 8];          \
        constexpr int L5 = Constants::rotate::value[5][(N - 1) % 8];          \
        constexpr int L6 = Constants::rotate::value[6][(N - 1) % 8];          \
        constexpr int L7 = Constants::rotate::value[7][(N - 1) % 8];          \
        constexpr int R0 = 64 - L0;                                           \
        constexpr int R1 = 64 - L1;                                           \
        constexpr int R2 = 64 - L2;                                           \
        constexpr int R3 = 64 - L3;                                           \
        constexpr int R4 = 64 - L4;                                           \
        constexpr int R5 = 64 - L5;                                           \
        constexpr int R6 = 64 - L6;                                           \
        constexpr int R7 = 64 - L7;                                           \
        t0 = s0 + s1;                                                         \
        t2 = s2 + s3;                                                         \
        t4 = s4 + s5;                                                         \
        t6 = s6 + s7;                                                         \
        t8 = s8 + s9;                                                         \
        tA = sA + sB;                                                         \
        tC = sC + sD;                                                         \
        tE = sE + sF;                                                         \
        t1 = (s1 << L0) | (s1 >> R0);                                         \
        t3 = (s3 << L1) | (s3 >> R1);                                         \
        t5 = (s5 << L2) | (s5 >> R2);                                         \
        t7 = (s7 << L3) | (s7 >> R3);                                         \
        t9 = (s9 << L4) | (s9 >> R4);                                         \
        tB = (sB << L5) | (sB >> R5);                                         \
        tD = (sD << L6) | (sD >> R6);                                         \
        tF = (sF << L7) | (sF >> R7);                                         \
        s0 = t0;                                                              \
        s2 = t2;                                                              \
        s4 = t6;                                                              \
        s6 = t4;                                                              \
        s8 = tA;                                                              \
        sA = tC;                                                              \
        sC = tE;                                                              \
        sE = t8;                                                              \
        s1 = t8 ^ t9;                                                         \
        s3 = tC ^ tD;                                                         \
        s5 = tA ^ tB;                                                         \
        s7 = tE ^ tF;                                                         \
        s9 = t6 ^ t7;                                                         \
        sB = t2 ^ t3;                                                         \
        sD = t4 ^ t5;                                                         \
        sF = t0 ^ t1;                                                         \
    }

#define MCKL_RANDOM_INTERNAL_THREEFRY_GENERIC_16X64_CYCLE_4(N)                \
    MCKL_RANDOM_INTERNAL_THREEFRY_GENERIC_16X64_RBOX(N * 8 + 1);              \
    MCKL_RANDOM_INTERNAL_THREEFRY_GENERIC_16X64_RBOX(N * 8 + 2);              \
    MCKL_RANDOM_INTERNAL_THREEFRY_GENERIC_16X64_RBOX(N * 8 + 3);              \
    MCKL_RANDOM_INTERNAL_THREEFRY_GENERIC_16X64_RBOX(N * 8 + 4);              \
    MCKL_RANDOM_INTERNAL_THREEFRY_GENERIC_16X64_KBOX(N * 8 + 4);

#define MCKL_RANDOM_INTERNAL_THREEFRY_GENERIC_16X64_CYCLE_8(N)                \
    MCKL_RANDOM_INTERNAL_THREEFRY_GENERIC_16X64_RBOX(N * 8 + 1);              \
    MCKL_RANDOM_INTERNAL_THREEFRY_GENERIC_16X64_RBOX(N * 8 + 2);              \
    MCKL_RANDOM_INTERNAL_THREEFRY_GENERIC_16X64_RBOX(N * 8 + 3);              \
    MCKL_RANDOM_INTERNAL_THREEFRY_GENERIC_16X64_RBOX(N * 8 + 4);              \
    MCKL_RANDOM_INTERNAL_THREEFRY_GENERIC_16X64_KBOX(N * 8 + 4);              \
    MCKL_RANDOM_INTERNAL_THREEFRY_GENERIC_16X64_RBOX(N * 8 + 5);              \
    MCKL_RANDOM_INTERNAL_THREEFRY_GENERIC_16X64_RBOX(N * 8 + 6);              \
    MCKL_RANDOM_INTERNAL_THREEFRY_GENERIC_16X64_RBOX(N * 8 + 7);              \
    MCKL_RANDOM_INTERNAL_THREEFRY_GENERIC_16X64_RBOX(N * 8 + 8);              \
    MCKL_RANDOM_INTERNAL_THREEFRY_GENERIC_16X64_KBOX(N * 8 + 8);

#define MCKL_RANDOM_INTERNAL_THREEFRY_GENERIC_16X64_ROUND_20                  \
    MCKL_RANDOM_INTERNAL_THREEFRY_GENERIC_16X64_KBOX(0)                       \
    MCKL_RANDOM_INTERNAL_THREEFRY_GENERIC_16X64_CYCLE_8(0)                    \
    MCKL_RANDOM_INTERNAL_THREEFRY_GENERIC_16X64_CYCLE_8(1)                    \
    MCKL_RANDOM_INTERNAL_THREEFRY_GENERIC_16X64_CYCLE_4(2)

#define MCKL_RANDOM_INTERNAL_THREEFRY_GENERIC_16X64_ROUND_80                  \
    MCKL_RANDOM_INTERNAL_THREEFRY_GENERIC_16X64_KBOX(0)                       \
    MCKL_RANDOM_INTERNAL_THREEFRY_GENERIC_16X64_CYCLE_8(0)                    \
    MCKL_RANDOM_INTERNAL_THREEFRY_GENERIC_16X64_CYCLE_8(1)                    \
    MCKL_RANDOM_INTERNAL_THREEFRY_GENERIC_16X64_CYCLE_8(2)                    \
    MCKL_RANDOM_INTERNAL_THREEFRY_GENERIC_16X64_CYCLE_8(3)                    \
    MCKL_RANDOM_INTERNAL_THREEFRY_GENERIC_16X64_CYCLE_8(4)                    \
    MCKL_RANDOM_INTERNAL_THREEFRY_GENERIC_16X64_CYCLE_8(5)                    \
    MCKL_RANDOM_INTERNAL_THREEFRY_GENERIC_16X64_CYCLE_8(6)                    \
    MCKL_RANDOM_INTERNAL_THREEFRY_GENERIC_16X64_CYCLE_8(7)                    \
    MCKL_RANDOM_INTERNAL_THREEFRY_GENERIC_16X64_CYCLE_8(8)                    \
    MCKL_RANDOM_INTERNAL_THREEFRY_GENERIC_16X64_CYCLE_8(9)

#define MCKL_RANDOM_INTERNAL_THREEFRY_GENERIC_16X64_ROUND(L)                  \
    std::uint64_t s0 = std::get<0x0>(ctr);                                    \
    std::uint64_t s1 = std::get<0x1>(ctr);                                    \
    std::uint64_t s2 = std::get<0x2>(ctr);                                    \
    std::uint64_t s3 = std::get<0x3>(ctr);                                    \
    std::uint64_t s4 = std::get<0x4>(ctr);                                    \
    std::uint64_t s5 = std::get<0x5>(ctr);                                    \
    std::uint64_t s6 = std::get<0x6>(ctr);                                    \
    std::uint64_t s7 = std::get<0x7>(ctr);                                    \
    std::uint64_t s8 = std::get<0x8>(ctr);                                    \
    std::uint64_t s9 = std::get<0x9>(ctr);                                    \
    std::uint64_t sA = std::get<0xA>(ctr);                                    \
    std::uint64_t sB = std::get<0xB>(ctr);                                    \
    std::uint64_t sC = std::get<0xC>(ctr);                                    \
    std::uint64_t sD = std::get<0xD>(ctr);                                    \
    std::uint64_t sE = std::get<0xE>(ctr);                                    \
    std::uint64_t sF = std::get<0xF>(ctr);                                    \
    std::uint64_t t0;                                                         \
    std::uint64_t t1;                                                         \
    std::uint64_t t2;                                                         \
    std::uint64_t t3;                                                         \
    std::uint64_t t4;                                                         \
    std::uint64_t t5;                                                         \
    std::uint64_t t6;                                                         \
    std::uint64_t t7;                                                         \
    std::uint64_t t8;                                                         \
    std::uint64_t t9;                                                         \
    std::uint64_t tA;                                                         \
    std::uint64_t tB;                                                         \
    std::uint64_t tC;                                                         \
    std::uint64_t tD;                                                         \
    std::uint64_t tE;                                                         \
    std::uint64_t tF;                                                         \
    MCKL_RANDOM_INTERNAL_THREEFRY_GENERIC_16X64_ROUND_##L;                    \
    std::array<std::uint64_t, 16> res;                                        \
    std::get<0x0>(res) = s0;                                                  \
    std::get<0x1>(res) = s1;                                                  \
    std::get<0x2>(res) = s2;                                                  \
    std::get<0x3>(res) = s3;                                                  \
    std::get<0x4>(res) = s4;                                                  \
    std::get<0x5>(res) = s5;                                                  \
    std::get<0x6>(res) = s6;                                                  \
    std::get<0x7>(res) = s7;                                                  \
    std::get<0x8>(res) = s8;                                                  \
    std::get<0x9>(res) = s9;                                                  \
    std::get<0xA>(res) = sA;                                                  \
    std::get<0xB>(res) = sB;                                                  \
    std::get<0xC>(res) = sC;                                                  \
    std::get<0xD>(res) = sD;                                                  \
    std::get<0xE>(res) = sE;                                                  \
    std::get<0xF>(res) = sF;

namespace mckl
{

namespace internal
{

template <typename T>
class Threefry16x64GeneratorGenericImpl
{
    static_assert(std::numeric_limits<T>::digits == 64,
        "**Threefry16x64GeneratorGenericImpl** used with T other than a "
        "64-bit unsigned integers");

    static constexpr std::size_t K = 16;
    static constexpr std::size_t Rounds = 20;
    using Constants = ThreefryConstants<T, 16>;

    public:
    static void eval(
        const void *plain, void *cipher, const std::array<T, K + 4> &par)
    {
        std::array<std::uint64_t, 16> ctr;
        std::memcpy(ctr.data(), plain, sizeof(T) * K);
        MCKL_RANDOM_INTERNAL_THREEFRY_GENERIC_16X64_ROUND(20)
        std::memcpy(cipher, res.data(), sizeof(T) * K);
    }

    template <typename ResultType>
    static void eval(std::array<std::uint64_t, 16> &ctr, ResultType *r,
        const std::array<T, K + 4> &par)
    {
        MCKL_INLINE_CALL increment(ctr);
        MCKL_RANDOM_INTERNAL_THREEFRY_GENERIC_16X64_ROUND(20)
        std::memcpy(r, res.data(), sizeof(T) * K);
    }

    template <typename ResultType>
    MCKL_NOINLINE static void eval(std::array<std::uint64_t, 16> &ctr,
        std::size_t n, ResultType *r, const std::array<T, K + 4> &par)
    {
        constexpr std::size_t R = sizeof(T) * K / sizeof(ResultType);

        for (std::size_t i = 0; i != n; ++i, r += R)
            eval(ctr, r, par);
    }
}; // class Threefry16x64GeneratorGenericImpl

template <typename T>
class Threefish1024GeneratorGenericImpl
{
    static_assert(std::numeric_limits<T>::digits == 64,
        "**Threefish1024GeneratorGenericImpl** used with T other than a "
        "64-bit unsigned integers");

    static constexpr std::size_t K = 16;
    static constexpr std::size_t Rounds = 80;
    using Constants = ThreefryConstants<T, 16>;

    public:
    static void eval(
        const void *plain, void *cipher, const std::array<T, K + 4> &par)
    {
        std::array<std::uint64_t, 16> ctr;
        std::memcpy(ctr.data(), plain, sizeof(T) * K);
        MCKL_RANDOM_INTERNAL_THREEFRY_GENERIC_16X64_ROUND(80)
        std::memcpy(cipher, res.data(), sizeof(T) * K);
    }

    template <typename ResultType>
    static void eval(std::array<std::uint64_t, 16> &ctr, ResultType *r,
        const std::array<T, K + 4> &par)
    {
        MCKL_INLINE_CALL increment(ctr);
        MCKL_RANDOM_INTERNAL_THREEFRY_GENERIC_16X64_ROUND(80)
        std::memcpy(r, res.data(), sizeof(T) * K);
    }

    template <typename ResultType>
    MCKL_NOINLINE static void eval(std::array<std::uint64_t, 16> &ctr,
        std::size_t n, ResultType *r, const std::array<T, K + 4> &par)
    {
        constexpr std::size_t R = sizeof(T) * K / sizeof(ResultType);

        for (std::size_t i = 0; i != n; ++i, r += R)
            eval(ctr, r, par);
    }
}; // class Threefish1024GeneratorGenericImpl

} // namespace mckl::internal

} // namespace mckl

#endif // MCKL_RANDOM_INTERNAL_THREEFRY_GENERIC_16X64_HPP
