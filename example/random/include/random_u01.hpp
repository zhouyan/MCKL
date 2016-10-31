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
#include <mckl/random/u01_distribution.hpp>
#include <mckl/random/uniform_real_distribution.hpp>
#include "random_common.hpp"
#include "random_rng.hpp"

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
    bool pass_l;
    bool pass_u;
    std::string function;
    std::string minimum;
    std::string maximum;
    std::string lower;
    std::string upper;
}; // struct RandomU01Perf

template <typename RNGType, typename UIntType, typename RealType,
    typename Lower, typename Upper>
inline RandomU01Perf random_u01_d(std::size_t N, std::size_t M)
{
    RNGType rng;
    mckl::UniformBitsDistribution<UIntType> ubits;
    std::uniform_int_distribution<std::size_t> rsize(N / 2, N);

    mckl::Vector<RealType> r1(N);
    mckl::Vector<RealType> r2(N);
    mckl::Vector<UIntType> ru(N);

    RealType lb = mckl::u01<UIntType, RealType, Lower, Upper>(
        std::numeric_limits<UIntType>::min());

    RealType ub = mckl::u01<UIntType, RealType, Lower, Upper>(
        std::numeric_limits<UIntType>::max());

    bool pass_b = true;
    bool pass_l = random_u01_check_lb<Lower>(lb);
    bool pass_u = random_u01_check_ub<Upper>(ub);

    for (std::size_t i = 0; i != M; ++i) {
        std::size_t K = rsize(rng);
        r1.resize(K);
        r2.resize(K);
        ru.resize(K);

        for (std::size_t j = 0; j != K; ++j) {
            UIntType u = ubits(rng);
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

        RealType lbi = *std::min_element(r1.begin(), r1.end());
        RealType ubi = *std::max_element(r1.begin(), r1.end());

        pass_b = pass_b && r2 == r1;
        pass_l = pass_l && mckl::internal::is_equal(lb, lbi);
        pass_u = pass_u && mckl::internal::is_equal(ub, ubi);
    }

    std::stringstream ss;
    ss << "u01<";
    ss << std::setw(10) << std::left << random_typename<UIntType>() + ",";
    ss << std::setw(13) << std::left << random_typename<RealType>() + ",";
    ss << std::setw(8) << std::left << random_typename<Lower>() + ",";
    ss << std::setw(8) << std::left << random_typename<Upper>() + ">";

    RandomU01Perf result;
    result.pass_b = pass_b;
    result.pass_l = pass_l;
    result.pass_u = pass_u;
    result.function = ss.str();
    result.minimum = random_u01_minimum(lb);
    result.maximum = random_u01_maximum(ub);

    return result;
}

template <typename RNGType, typename Lower, typename Upper>
inline void random_u01_d(std::size_t N, std::size_t M, int nwid, int twid)
{
    mckl::Vector<RandomU01Perf> perf;
    perf.push_back(
        random_u01_d<RNGType, std::uint32_t, float, Lower, Upper>(N, M));
    perf.push_back(
        random_u01_d<RNGType, std::uint64_t, float, Lower, Upper>(N, M));
    perf.push_back(
        random_u01_d<RNGType, std::uint32_t, double, Lower, Upper>(N, M));
    perf.push_back(
        random_u01_d<RNGType, std::uint64_t, double, Lower, Upper>(N, M));
    perf.push_back(
        random_u01_d<RNGType, std::uint32_t, long double, Lower, Upper>(N, M));
    perf.push_back(
        random_u01_d<RNGType, std::uint64_t, long double, Lower, Upper>(N, M));

    for (std::size_t i = 0; i != perf.size(); ++i) {
        std::cout << std::setw(nwid) << std::left << perf[i].function;
        std::cout << std::setw(twid) << std::right << perf[i].minimum;
        std::cout << std::setw(twid) << std::right << perf[i].maximum;
        std::string pass;
        pass += perf[i].pass_b ? "-" : "*";
        pass += perf[i].pass_l ? "-" : "*";
        pass += perf[i].pass_u ? "-" : "*";
        pass +=
            random_pass(perf[i].pass_b && perf[i].pass_l && perf[i].pass_u);
        std::cout << std::setw(15) << std::right << pass;
        std::cout << std::endl;
    }
}

template <typename RNGType>
inline void random_u01_d(std::size_t N, std::size_t M)
{
    const int nwid = 50;
    const int twid = 15;
    const std::size_t lwid = nwid + twid * 3;

    std::cout << std::fixed << std::setprecision(2);

    std::cout << std::string(lwid, '=') << std::endl;

    std::cout << std::setw(nwid) << std::left << "Function";
    std::cout << std::setw(twid) << std::right << "Mininum";
    std::cout << std::setw(twid) << std::right << "Maximum";
    std::cout << std::setw(twid) << std::right << "Deterministics";
    std::cout << std::endl;

    std::cout << std::string(lwid, '-') << std::endl;
    random_u01_d<RNGType, mckl::Closed, mckl::Closed>(N, M, nwid, twid);
    std::cout << std::string(lwid, '-') << std::endl;
    random_u01_d<RNGType, mckl::Closed, mckl::Open>(N, M, nwid, twid);
    std::cout << std::string(lwid, '-') << std::endl;
    random_u01_d<RNGType, mckl::Open, mckl::Closed>(N, M, nwid, twid);
    std::cout << std::string(lwid, '-') << std::endl;
    random_u01_d<RNGType, mckl::Open, mckl::Open>(N, M, nwid, twid);
    std::cout << std::string(lwid, '-') << std::endl;
}

template <typename RNGType>
inline void random_u01_p(std::size_t N, std::size_t M)
{
    using f01cc = mckl::U01CCDistribution<float>;
    using f01co = mckl::U01CODistribution<float>;
    using f01oc = mckl::U01OCDistribution<float>;
    using f01oo = mckl::U01OODistribution<float>;
    using funif = mckl::UniformRealDistribution<float>;

    using d01cc = mckl::U01CCDistribution<double>;
    using d01co = mckl::U01CODistribution<double>;
    using d01oc = mckl::U01OCDistribution<double>;
    using d01oo = mckl::U01OODistribution<double>;
    using dunif = mckl::UniformRealDistribution<double>;

    using l01cc = mckl::U01CCDistribution<long double>;
    using l01co = mckl::U01CODistribution<long double>;
    using l01oc = mckl::U01OCDistribution<long double>;
    using l01oo = mckl::U01OODistribution<long double>;
    using lunif = mckl::UniformRealDistribution<long double>;

    RandomRNGPerf perf_f01cc(random_rng_p<RNGType, f01cc>(N, M));
    RandomRNGPerf perf_f01co(random_rng_p<RNGType, f01co>(N, M));
    RandomRNGPerf perf_f01oc(random_rng_p<RNGType, f01oc>(N, M));
    RandomRNGPerf perf_f01oo(random_rng_p<RNGType, f01oo>(N, M));
    RandomRNGPerf perf_funif(random_rng_p<RNGType, funif>(N, M));

    RandomRNGPerf perf_d01cc(random_rng_p<RNGType, d01cc>(N, M));
    RandomRNGPerf perf_d01co(random_rng_p<RNGType, d01co>(N, M));
    RandomRNGPerf perf_d01oc(random_rng_p<RNGType, d01oc>(N, M));
    RandomRNGPerf perf_d01oo(random_rng_p<RNGType, d01oo>(N, M));
    RandomRNGPerf perf_dunif(random_rng_p<RNGType, dunif>(N, M));

    RandomRNGPerf perf_l01cc(random_rng_p<RNGType, l01cc>(N, M));
    RandomRNGPerf perf_l01co(random_rng_p<RNGType, l01co>(N, M));
    RandomRNGPerf perf_l01oc(random_rng_p<RNGType, l01oc>(N, M));
    RandomRNGPerf perf_l01oo(random_rng_p<RNGType, l01oo>(N, M));
    RandomRNGPerf perf_lunif(random_rng_p<RNGType, lunif>(N, M));

    const int nwid = 50;
    const int twid = 15;
    const std::size_t lwid = nwid + twid * 3;

    std::cout << std::fixed << std::setprecision(2);

    std::cout << std::string(lwid, '=') << std::endl;

    std::cout << std::setw(nwid) << std::left << "Distribution";
    if (mckl::StopWatch::has_cycles()) {
        std::cout << std::setw(twid) << std::right << "cpE (S)";
        std::cout << std::setw(twid) << std::right << "cpE (B)";
    } else {
        std::cout << std::setw(twid) << std::right << "ME/s (S)";
        std::cout << std::setw(twid) << std::right << "ME/s (B)";
    }
    std::cout << std::setw(twid) << std::right << "Deterministics";
    std::cout << std::endl;

    std::cout << std::string(lwid, '-') << std::endl;

    std::cout << std::setw(nwid) << std::left << "U01CCDistribution<float>";
    std::cout << std::setw(twid) << std::right << perf_f01cc.c1;
    std::cout << std::setw(twid) << std::right << perf_f01cc.c2;
    std::cout << std::setw(twid) << std::right << random_pass(perf_f01cc.pass);
    std::cout << std::endl;

    std::cout << std::setw(nwid) << std::left << "U01CODistribution<float>";
    std::cout << std::setw(twid) << std::right << perf_f01co.c1;
    std::cout << std::setw(twid) << std::right << perf_f01co.c2;
    std::cout << std::setw(twid) << std::right << random_pass(perf_f01co.pass);
    std::cout << std::endl;

    std::cout << std::setw(nwid) << std::left << "U01OCDistribution<float>";
    std::cout << std::setw(twid) << std::right << perf_f01oc.c1;
    std::cout << std::setw(twid) << std::right << perf_f01oc.c2;
    std::cout << std::setw(twid) << std::right << random_pass(perf_f01oc.pass);
    std::cout << std::endl;

    std::cout << std::setw(nwid) << std::left << "U01OODistribution<float>";
    std::cout << std::setw(twid) << std::right << perf_f01oo.c1;
    std::cout << std::setw(twid) << std::right << perf_f01oo.c2;
    std::cout << std::setw(twid) << std::right << random_pass(perf_f01oo.pass);
    std::cout << std::endl;

    std::cout << std::setw(nwid) << std::left
              << "UniformRealDistribution<float>";
    std::cout << std::setw(twid) << std::right << perf_funif.c1;
    std::cout << std::setw(twid) << std::right << perf_funif.c2;
    std::cout << std::setw(twid) << std::right << random_pass(perf_funif.pass);
    std::cout << std::endl;

    std::cout << std::setw(nwid) << std::left << "U01CCDistribution<double>";
    std::cout << std::setw(twid) << std::right << perf_d01cc.c1;
    std::cout << std::setw(twid) << std::right << perf_d01cc.c2;
    std::cout << std::setw(twid) << std::right << random_pass(perf_d01cc.pass);
    std::cout << std::endl;

    std::cout << std::setw(nwid) << std::left << "U01CODistribution<double>";
    std::cout << std::setw(twid) << std::right << perf_d01co.c1;
    std::cout << std::setw(twid) << std::right << perf_d01co.c2;
    std::cout << std::setw(twid) << std::right << random_pass(perf_d01co.pass);
    std::cout << std::endl;

    std::cout << std::setw(nwid) << std::left << "U01OCDistribution<double>";
    std::cout << std::setw(twid) << std::right << perf_d01oc.c1;
    std::cout << std::setw(twid) << std::right << perf_d01oc.c2;
    std::cout << std::setw(twid) << std::right << random_pass(perf_d01oc.pass);
    std::cout << std::endl;

    std::cout << std::setw(nwid) << std::left << "U01OODistribution<double>";
    std::cout << std::setw(twid) << std::right << perf_d01oo.c1;
    std::cout << std::setw(twid) << std::right << perf_d01oo.c2;
    std::cout << std::setw(twid) << std::right << random_pass(perf_d01oo.pass);
    std::cout << std::endl;

    std::cout << std::setw(nwid) << std::left
              << "UniformRealDistribution<double>";
    std::cout << std::setw(twid) << std::right << perf_dunif.c1;
    std::cout << std::setw(twid) << std::right << perf_dunif.c2;
    std::cout << std::setw(twid) << std::right << random_pass(perf_dunif.pass);
    std::cout << std::endl;

    std::cout << std::setw(nwid) << std::left
              << "U01CCDistribution<long double>";
    std::cout << std::setw(twid) << std::right << perf_l01cc.c1;
    std::cout << std::setw(twid) << std::right << perf_l01cc.c2;
    std::cout << std::setw(twid) << std::right << random_pass(perf_l01cc.pass);
    std::cout << std::endl;

    std::cout << std::setw(nwid) << std::left
              << "U01CODistribution<long double>";
    std::cout << std::setw(twid) << std::right << perf_l01co.c1;
    std::cout << std::setw(twid) << std::right << perf_l01co.c2;
    std::cout << std::setw(twid) << std::right << random_pass(perf_l01co.pass);
    std::cout << std::endl;

    std::cout << std::setw(nwid) << std::left
              << "U01OCDistribution<long double>";
    std::cout << std::setw(twid) << std::right << perf_l01oc.c1;
    std::cout << std::setw(twid) << std::right << perf_l01oc.c2;
    std::cout << std::setw(twid) << std::right << random_pass(perf_l01oc.pass);
    std::cout << std::endl;

    std::cout << std::setw(nwid) << std::left
              << "U01OODistribution<long double>";
    std::cout << std::setw(twid) << std::right << perf_l01oo.c1;
    std::cout << std::setw(twid) << std::right << perf_l01oo.c2;
    std::cout << std::setw(twid) << std::right << random_pass(perf_l01oo.pass);
    std::cout << std::endl;

    std::cout << std::setw(nwid) << std::left
              << "UniformRealDistribution<long double>";
    std::cout << std::setw(twid) << std::right << perf_lunif.c1;
    std::cout << std::setw(twid) << std::right << perf_lunif.c2;
    std::cout << std::setw(twid) << std::right << random_pass(perf_lunif.pass);
    std::cout << std::endl;

    std::cout << std::string(lwid, '-') << std::endl;
}

template <typename RNGType>
inline void random_u01(std::size_t N, std::size_t M, const std::string &name)
{
    std::cout << std::string(95, '=') << std::endl;
    std::cout << std::setw(40) << std::left << "RNGType:" << name << std::endl;
    std::cout << std::setw(40) << std::left << "result_type:"
              << random_typename<typename RNGType::result_type>() << std::endl;
    std::cout << std::setw(40) << std::left
              << "Number of random bits:" << mckl::RNGTraits<RNGType>::bits
              << std::endl;
    std::cout << std::string(95, '-') << std::endl;

    random_u01_d<RNGType>(N, M);
    random_u01_p<RNGType>(N, M);
}

#endif // MCKL_EXAMPLE_RANDOM_U01_HPP
