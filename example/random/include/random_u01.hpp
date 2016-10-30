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

#if MCKL_HAS_AESNI && !defined(MCKL_EXAMPLE_RANDOM_NO_ARS)
#include <mckl/random/aes.hpp>
using MCKLRNG32Type = mckl::ARS;
using MCKLRNG64Type = mckl::ARS_64;
#else
#include <mckl/random/philox.hpp>
using MCKLRNG32Type = mckl::Philox4x32;
using MCKLRNG64Type = mckl::Philox4x32_64;
#endif

template <typename UIntType, int = std::numeric_limits<UIntType>::digits>
class MCKLRNGType;

template <typename UIntType>
class MCKLRNGType<UIntType, 32>
{
    public:
    using type = MCKLRNG32Type;
}; // class MCKLRNGType

template <typename UIntType>
class MCKLRNGType<UIntType, 64>
{
    public:
    using type = MCKLRNG64Type;
}; // class MCKLRNGType

template <typename, typename, typename>
class MCKLU01DistributionType;

template <typename RealType>
class MCKLU01DistributionType<RealType, mckl::Closed, mckl::Closed>
{
    public:
    using type = mckl::U01CCDistribution<RealType>;
}; // class MCKLU01DistributionType

template <typename RealType>
class MCKLU01DistributionType<RealType, mckl::Closed, mckl::Open>
{
    public:
    using type = mckl::U01CODistribution<RealType>;
}; // class MCKLU01DistributionType

template <typename RealType>
class MCKLU01DistributionType<RealType, mckl::Open, mckl::Closed>
{
    public:
    using type = mckl::U01OCDistribution<RealType>;
}; // class MCKLU01DistributionType

template <typename RealType>
class MCKLU01DistributionType<RealType, mckl::Open, mckl::Open>
{
    public:
    using type = mckl::U01OODistribution<RealType>;
}; // class MCKLU01DistributionType

template <typename>
inline std::string random_u01_typename();

template <>
inline std::string random_u01_typename<mckl::Closed>()
{
    return "C";
}

template <>
inline std::string random_u01_typename<mckl::Open>()
{
    return "O";
}

template <typename UIntType, typename RealType, typename Lower, typename Upper>
inline std::string random_u01_function()
{
    return "U01" + random_u01_typename<Lower>() +
        random_u01_typename<Upper>() + " (" +
        std::to_string(std::numeric_limits<UIntType>::digits) + ", " +
        random_typename<RealType>() + ")";
}

template <typename Lower, typename RealType>
inline bool random_u01_check_lb(RealType x)
{
    return std::is_same<Lower, mckl::Closed>::value ?
        mckl::internal::is_zero(x) :
        !mckl::internal::is_zero(x);
}

template <typename Upper, typename RealType>
inline bool random_u01_check_ub(RealType x)
{
    return std::is_same<Upper, mckl::Closed>::value ?
        mckl::internal::is_one(x) :
        !mckl::internal::is_one(x);
}

template <typename RealType>
inline std::string random_u01_minimum(RealType x)
{
    std::stringstream ss;
    if (mckl::internal::is_zero(x))
        ss << 0;
    else
        ss << "2^" << std::log2(x);

    return ss.str();
}

template <typename RealType>
inline std::string random_u01_maximum(RealType x)
{
    std::stringstream ss;
    if (mckl::internal::is_equal(x, static_cast<RealType>(1)))
        ss << 1;
    else
        ss << "1 - 2^" << std::log2(static_cast<RealType>(1) - x);

    return ss.str();
}

struct RandomU01Perf {
    bool pass_b;
    bool pass_c;
    bool pass_l;
    bool pass_u;
    std::string function;
    std::string minimum;
    std::string maximum;
    std::string lower;
    std::string upper;
    double c1;
    double c2;
}; // struct RandomU01Perf

template <typename UIntType, typename RealType, typename Lower, typename Upper>
inline void random_u01_d(
    std::size_t N, std::size_t M, mckl::Vector<RandomU01Perf> &perf)
{
    typename MCKLRNGType<UIntType>::type rng;

    std::uniform_int_distribution<std::size_t> rsize(N / 2, N);

    mckl::Vector<RealType> r1(N);
    mckl::Vector<RealType> r2(N);
    mckl::Vector<RealType> r3(N);
    mckl::Vector<UIntType> ru(N);

    RealType lb = mckl::u01<UIntType, RealType, Lower, Upper>(
        std::numeric_limits<UIntType>::min());

    RealType ub = mckl::u01<UIntType, RealType, Lower, Upper>(
        std::numeric_limits<UIntType>::max());

    bool pass_b = true;
    bool pass_c = true;
    bool pass_l = random_u01_check_lb<Lower>(lb);
    bool pass_u = random_u01_check_ub<Upper>(ub);

    for (std::size_t i = 0; i != M; ++i) {
        std::size_t K = rsize(rng);
        r1.resize(K);
        r2.resize(K);
        r3.resize(K);
        ru.resize(K);

        for (std::size_t j = 0; j != K; ++j) {
            UIntType u = rng();
            switch (u % 3) {
                case 0:
                    ru[j] = 0;
                    break;
                case 1:
                    ru[j] = std::numeric_limits<UIntType>::max();
                    break;
                default:
                    ru[j] = u;
            }
        }

        for (std::size_t j = 0; j != K; ++j)
            r1[j] = mckl::u01<UIntType, RealType, Lower, Upper>(ru[j]);

        mckl::u01<UIntType, RealType, Lower, Upper>(K, ru.data(), r2.data());

        for (std::size_t j = 0; j != K; ++j)
            r3[j] = random_u01_c<UIntType, RealType, Lower, Upper>(ru[j]);

        RealType lbi = *std::min_element(r1.begin(), r1.end());
        RealType ubi = *std::max_element(r1.begin(), r1.end());

        pass_b = pass_b && r2 == r1;
        pass_c = pass_c && r3 == r1;
        pass_l = pass_l && mckl::internal::is_equal(lb, lbi);
        pass_u = pass_u && mckl::internal::is_equal(ub, ubi);
    }

    RandomU01Perf result;
    result.pass_b = pass_b;
    result.pass_c = pass_c;
    result.pass_l = pass_l;
    result.pass_u = pass_u;
    result.function = random_u01_function<UIntType, RealType, Lower, Upper>();
    result.minimum = random_u01_minimum(lb);
    result.maximum = random_u01_maximum(ub);
    perf.push_back(result);
}

template <typename UIntType, typename RealType, typename Lower, typename Upper>
inline void random_u01_p(
    std::size_t N, std::size_t M, mckl::Vector<RandomU01Perf> &perf)
{
    typename MCKLU01DistributionType<RealType, Lower, Upper>::type ru01;
    typename MCKLRNGType<UIntType>::type rng;
    typename MCKLRNGType<UIntType>::type rng1;
    typename MCKLRNGType<UIntType>::type rng2;
    std::uniform_int_distribution<std::size_t> rsize(N / 2, N);
    bool pass_b = true;

    mckl::Vector<RealType> r1(N);
    mckl::Vector<RealType> r2(N);

    bool has_cycles = mckl::StopWatch::has_cycles();

    double c1 = has_cycles ? std::numeric_limits<double>::max() : 0.0;
    double c2 = has_cycles ? std::numeric_limits<double>::max() : 0.0;
    for (std::size_t k = 0; k != 10; ++k) {
        std::size_t n = 0;
        mckl::StopWatch watch1;
        mckl::StopWatch watch2;
        for (std::size_t i = 0; i != M; ++i) {
            std::size_t K = rsize(rng);
            n += K;
            r1.resize(K);
            r2.resize(K);

            watch1.start();
            for (std::size_t j = 0; j != K; ++j)
                r1[j] = mckl::rand(rng1, ru01);
            watch1.stop();

            watch2.start();
            mckl::rand(rng2, ru01, K, r2.data());
            watch2.stop();

            pass_b = pass_b && r2 == r1;
        }
        if (has_cycles) {
            c1 = std::min(c1, 1.0 * watch1.cycles() / n);
            c2 = std::min(c2, 1.0 * watch2.cycles() / n);
        } else {
            c1 = std::max(c1, n / watch1.seconds() * 1e-6);
            c2 = std::max(c2, n / watch2.seconds() * 1e-6);
        }
    }

    RandomU01Perf result;
    result.pass_b = pass_b;
    result.c1 = c1;
    result.c2 = c2;
    perf.push_back(result);
}

template <typename Lower, typename Upper>
inline void random_u01(
    std::size_t N, std::size_t M, int nwid, int swid, int twid)
{
    mckl::Vector<RandomU01Perf> perf_d;
    random_u01_d<std::uint32_t, float, Lower, Upper>(N, M, perf_d);
    random_u01_d<std::uint64_t, float, Lower, Upper>(N, M, perf_d);
    random_u01_d<std::uint32_t, double, Lower, Upper>(N, M, perf_d);
    random_u01_d<std::uint64_t, double, Lower, Upper>(N, M, perf_d);

    mckl::Vector<RandomU01Perf> perf_p;
    random_u01_p<std::uint32_t, float, Lower, Upper>(N, M, perf_p);
    random_u01_p<std::uint64_t, float, Lower, Upper>(N, M, perf_p);
    random_u01_p<std::uint32_t, double, Lower, Upper>(N, M, perf_p);
    random_u01_p<std::uint64_t, double, Lower, Upper>(N, M, perf_p);

    for (std::size_t i = 0; i != perf_d.size(); ++i) {
        std::cout << std::setw(nwid) << std::left << perf_d[i].function;
        std::cout << std::setw(swid) << std::right << perf_d[i].minimum;
        std::cout << std::setw(swid) << std::right << perf_d[i].maximum;
        std::cout << std::setw(twid) << std::right << perf_p[i].c1;
        std::cout << std::setw(twid) << std::right << perf_p[i].c2;
        std::string pass;
        pass += perf_d[i].pass_b ? "-" : "*";
        pass += perf_d[i].pass_c ? "-" : "*";
        pass += perf_d[i].pass_l ? "-" : "*";
        pass += perf_d[i].pass_u ? "-" : "*";
        pass += perf_p[i].pass_b ? "-" : "*";
        pass += random_pass(perf_d[i].pass_b && perf_d[i].pass_c &&
            perf_d[i].pass_l && perf_d[i].pass_u && perf_p[i].pass_b);
        std::cout << std::setw(15) << std::right << pass;
        std::cout << std::endl;
    }
}

inline void random_u01(std::size_t N, std::size_t M)
{
    const int nwid = 20;
    const int swid = 12;
    const int twid = 8;
    const std::size_t lwid = nwid + swid * 2 + twid * 2 + 15;

    std::cout << std::fixed << std::setprecision(2);

    std::cout << std::string(lwid, '=') << std::endl;

    std::cout << std::setw(nwid) << std::left << "Function";
    std::cout << std::setw(swid) << std::right << "Mininum";
    std::cout << std::setw(swid) << std::right << "Maximum";
    if (mckl::StopWatch::has_cycles()) {
        std::cout << std::setw(twid) << std::right << "cpE (S)";
        std::cout << std::setw(twid) << std::right << "cpE (B)";
    } else {
        std::cout << std::setw(twid) << std::right << "ME/s (S)";
        std::cout << std::setw(twid) << std::right << "ME/s (B)";
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
