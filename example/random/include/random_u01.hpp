//============================================================================
// MCKL/example/random/include/random_u01.hpp
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

#ifndef MCKL_EXAMPLE_RANDOM_U01_HPP
#define MCKL_EXAMPLE_RANDOM_U01_HPP

#include <mckl/random/philox.hpp>
#include <mckl/random/u01.hpp>
#include <mckl/randomc/u01.h>
#include "random_common.hpp"

#define MCKL_EXAMPLE_DEFINE_RANDOMC_U01_TEST(                                 \
    ubits, fsuffix, lr, Lower, Upper, RealType)                               \
    template <>                                                               \
    inline RealType random_u01_c<std::uint##ubits##_t, RealType, mckl::Lower, \
        mckl::Upper>(std::uint##ubits##_t u)                                  \
    {                                                                         \
        return mckl_u01_##lr##_u##ubits##fsuffix(u);                          \
    }

template <typename UIntType, typename RealType, typename, typename>
inline RealType random_u01_c(UIntType);

MCKL_EXAMPLE_DEFINE_RANDOMC_U01_TEST(32, f, cc, Closed, Closed, float)
MCKL_EXAMPLE_DEFINE_RANDOMC_U01_TEST(64, f, cc, Closed, Closed, float)
MCKL_EXAMPLE_DEFINE_RANDOMC_U01_TEST(32, d, cc, Closed, Closed, double)
MCKL_EXAMPLE_DEFINE_RANDOMC_U01_TEST(64, d, cc, Closed, Closed, double)

MCKL_EXAMPLE_DEFINE_RANDOMC_U01_TEST(32, f, co, Closed, Open, float)
MCKL_EXAMPLE_DEFINE_RANDOMC_U01_TEST(64, f, co, Closed, Open, float)
MCKL_EXAMPLE_DEFINE_RANDOMC_U01_TEST(32, d, co, Closed, Open, double)
MCKL_EXAMPLE_DEFINE_RANDOMC_U01_TEST(64, d, co, Closed, Open, double)

MCKL_EXAMPLE_DEFINE_RANDOMC_U01_TEST(32, f, oc, Open, Closed, float)
MCKL_EXAMPLE_DEFINE_RANDOMC_U01_TEST(64, f, oc, Open, Closed, float)
MCKL_EXAMPLE_DEFINE_RANDOMC_U01_TEST(32, d, oc, Open, Closed, double)
MCKL_EXAMPLE_DEFINE_RANDOMC_U01_TEST(64, d, oc, Open, Closed, double)

MCKL_EXAMPLE_DEFINE_RANDOMC_U01_TEST(32, f, oo, Open, Open, float)
MCKL_EXAMPLE_DEFINE_RANDOMC_U01_TEST(64, f, oo, Open, Open, float)
MCKL_EXAMPLE_DEFINE_RANDOMC_U01_TEST(32, d, oo, Open, Open, double)
MCKL_EXAMPLE_DEFINE_RANDOMC_U01_TEST(64, d, oo, Open, Open, double)

template <typename UIntType, typename RealType, typename Lower, typename Upper>
inline std::string random_u01_function()
{
    std::stringstream ss;
    ss << "u01<uint";
    ss << std::numeric_limits<UIntType>::digits << "_t, ";
    ss << random_typename<RealType>() << ", ";
    ss << random_typename<Lower>() << ", ";
    ss << random_typename<Upper>() << ">";

    return ss.str();
}

template <typename RealType>
inline void random_u01_lb(RealType x, std::string &minimum, std::string &lower)
{
    std::stringstream ss;
    if (mckl::internal::is_zero(x))
        ss << 0;
    else
        ss << "2^" << std::log2(x);
    minimum = ss.str();

    if (x < static_cast<RealType>(0))
        lower = "< 0";
    else if (x > static_cast<RealType>(0))
        lower = "Open";
    else
        lower = "Closed";
}

template <typename RealType>
inline void random_u01_rb(RealType x, std::string &maximum, std::string &upper)
{
    std::stringstream ss;
    if (mckl::internal::is_equal(x, static_cast<RealType>(1)))
        ss << 1;
    else
        ss << "1 - 2^" << std::log2(static_cast<RealType>(1) - x);
    maximum = ss.str();
    if (x > static_cast<RealType>(1))
        upper = "> 1";
    else if (x < static_cast<RealType>(1))
        upper = "Open";
    else
        upper = "Closed";
}

struct RandomU01Perf {
    bool pass_b;
    bool pass_c;
    std::string function;
    std::string minimum;
    std::string maximum;
    std::string lower;
    std::string upper;
    double c1;
    double c2;
    double c3;
}; // struct RandomU01Perf

template <typename UIntType, typename RealType, typename Lower, typename Upper>
inline void random_u01(
    std::size_t N, std::size_t M, mckl::Vector<RandomU01Perf> &perf)
{
    mckl::PhiloxEngine<UIntType, UIntType, 4> rng;
    std::uniform_int_distribution<std::size_t> rsize(N / 2, N);
    bool pass_b = true;
    bool pass_c = true;

    mckl::Vector<UIntType> ru(N);
    mckl::Vector<RealType> r1(N);
    mckl::Vector<RealType> r2(N);
    mckl::Vector<RealType> r3(N);

    bool has_cycles = mckl::StopWatch::has_cycles();

    double c1 = has_cycles ? std::numeric_limits<double>::max() : 0.0;
    double c2 = has_cycles ? std::numeric_limits<double>::max() : 0.0;
    double c3 = has_cycles ? std::numeric_limits<double>::max() : 0.0;
    for (std::size_t k = 0; k != 10; ++k) {
        std::size_t n = 0;
        mckl::StopWatch watch1;
        mckl::StopWatch watch2;
        mckl::StopWatch watch3;
        for (std::size_t i = 0; i != M; ++i) {
            std::size_t K = rsize(rng);
            n += K;
            ru.resize(K);
            r1.resize(K);
            r2.resize(K);
            r3.resize(K);

            mckl::rand(rng, K, ru.data());

            watch1.start();
            for (std::size_t j = 0; j != K; ++j)
                r1[j] = mckl::u01<UIntType, RealType, Lower, Upper>(ru[j]);
            watch1.stop();

            watch2.start();
            mckl::u01<UIntType, RealType, Lower, Upper>(
                K, ru.data(), r2.data());
            watch2.stop();

            watch3.start();
            for (std::size_t j = 0; j != K; ++j)
                r3[j] = random_u01_c<UIntType, RealType, Lower, Upper>(ru[j]);
            watch3.stop();

            pass_b = pass_b && r2 == r1;
            pass_c = pass_c && r3 == r1;
        }
        if (has_cycles) {
            c1 = std::min(c1, 1.0 * watch1.cycles() / n);
            c2 = std::min(c2, 1.0 * watch2.cycles() / n);
            c3 = std::min(c3, 1.0 * watch3.cycles() / n);
        } else {
            c1 = std::max(c1, n / watch1.seconds() * 1e-6);
            c2 = std::max(c2, n / watch2.seconds() * 1e-6);
            c3 = std::max(c3, n / watch3.seconds() * 1e-6);
        }
    }

    RandomU01Perf result;
    result.pass_b = pass_b;
    result.pass_c = pass_c;
    result.function = random_u01_function<UIntType, RealType, Lower, Upper>();
    random_u01_lb(mckl::u01<UIntType, RealType, Lower, Upper>(
                      std::numeric_limits<UIntType>::min()),
        result.minimum, result.lower);
    random_u01_rb(mckl::u01<UIntType, RealType, Lower, Upper>(
                      std::numeric_limits<UIntType>::max()),
        result.maximum, result.upper);
    result.c1 = c1;
    result.c2 = c2;
    result.c3 = c3;
    perf.push_back(result);
}

template <typename Lower, typename Upper>
inline void random_u01(
    std::size_t N, std::size_t M, int nwid, int swid, int twid)
{
    mckl::Vector<RandomU01Perf> perf;
    random_u01<std::uint32_t, float, Lower, Upper>(N, M, perf);
    random_u01<std::uint64_t, float, Lower, Upper>(N, M, perf);
    random_u01<std::uint32_t, double, Lower, Upper>(N, M, perf);
    random_u01<std::uint64_t, double, Lower, Upper>(N, M, perf);
    for (std::size_t i = 0; i != perf.size(); ++i) {
        std::cout << std::setw(nwid) << std::left << perf[i].function;
        std::cout << std::setw(swid) << std::right << perf[i].minimum;
        std::cout << std::setw(swid) << std::right << perf[i].maximum;
        std::cout << std::setw(twid) << std::right << perf[i].lower;
        std::cout << std::setw(twid) << std::right << perf[i].upper;
        std::cout << std::setw(twid) << std::right << perf[i].c1;
        std::cout << std::setw(twid) << std::right << perf[i].c2;
        std::cout << std::setw(twid) << std::right << perf[i].c3;
        std::string pass;
        pass += perf[i].pass_b ? "-" : "*";
        pass += perf[i].pass_c ? "-" : "*";
        pass += random_pass(perf[i].pass_b && perf[i].pass_c);
        std::cout << std::setw(15) << std::right << pass;
        std::cout << std::endl;
    }
}

inline void random_u01(std::size_t N, std::size_t M)
{
    const int nwid = 45;
    const int swid = 15;
    const int twid = 10;
    const std::size_t lwid = nwid + swid * 2 + twid * 5 + 15;

    std::cout << std::fixed << std::setprecision(2);

    std::cout << std::string(lwid, '=') << std::endl;

    std::cout << std::setw(nwid) << std::left << "Function";
    std::cout << std::setw(swid) << std::right << "Mininum";
    std::cout << std::setw(swid) << std::right << "Maximum";
    std::cout << std::setw(twid) << std::right << "Lower";
    std::cout << std::setw(twid) << std::right << "Upper";
    if (mckl::StopWatch::has_cycles()) {
        std::cout << std::setw(twid) << std::right << "cpE (S)";
        std::cout << std::setw(twid) << std::right << "cpE (B)";
        std::cout << std::setw(twid) << std::right << "cpE (C)";
    } else {
        std::cout << std::setw(twid) << std::right << "ME/s (S)";
        std::cout << std::setw(twid) << std::right << "ME/s (B)";
        std::cout << std::setw(twid) << std::right << "ME/s (C)";
    }
    std::cout << std::setw(15) << std::right << "Deterministics";
    std::cout << std::endl;

    std::cout << std::string(lwid, '-') << std::endl;
    random_u01<mckl::Closed, mckl::Closed>(N, M, nwid, swid, twid);
    std::cout << std::string(lwid, '-') << std::endl;
    random_u01<mckl::Closed, mckl::Open>(N, M, nwid, swid, twid);
    std::cout << std::string(lwid, '-') << std::endl;
    random_u01<mckl::Open, mckl::Closed>(N, M, nwid, swid, twid);
    std::cout << std::string(lwid, '-') << std::endl;
    random_u01<mckl::Open, mckl::Open>(N, M, nwid, swid, twid);
    std::cout << std::string(lwid, '-') << std::endl;
}

#endif // MCKL_EXAMPLE_RANDOM_U01_HPP
