//============================================================================
// MCKL/example/random/include/random_distribution_perf.hpp
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

#include "random_distribution.hpp"

template <typename MCKLDistType, typename ParamType, std::size_t ParamNum>
inline void random_distribution_test_perf(std::size_t N, std::size_t M,
    const std::array<ParamType, ParamNum> &param, int nwid, int twid)
{
    using result_type = typename MCKLDistType::result_type;
    using std_type = typename RandomDistributionTrait<MCKLDistType>::std_type;

    mckl::RNGFast rng;
    RNG01<mckl::RNGFast> rng01;
#if MCKL_HAS_MKL
    mckl::MKL_MT2203 random_mkl;
#endif
    std::uniform_int_distribution<std::size_t> rsize(N / 2, N);
    MCKLDistType dist_mckl(random_distribution_init<MCKLDistType>(param));
    std_type dist_std(random_distribution_init<std_type>(param));
    bool pass = true;

    mckl::Vector<result_type> r(N);
    mckl::rand(rng01, dist_mckl, N, r.data());
    for (std::size_t i = 0; i != N; ++i) {
        pass = pass && std::isfinite(r[i]);
        pass = pass && std::isfinite(dist_mckl(rng01));
    }

    mckl::Vector<result_type> r1;
    mckl::Vector<result_type> r2;
    mckl::Vector<result_type> r3;
#if MCKL_HAS_MKL
    mckl::Vector<result_type> r4;
#endif
    r1.reserve(N);
    r2.reserve(N);
    r3.reserve(N);
#if MCKL_HAS_MKL
    r4.reserve(N);
#endif

    double c1 = std::numeric_limits<double>::max();
    double c2 = std::numeric_limits<double>::max();
    double c3 = std::numeric_limits<double>::max();
#if MCKL_HAS_MKL
    double c4 = std::numeric_limits<double>::max();
#endif
    for (std::size_t k = 0; k != 10; ++k) {
        std::size_t num = 0;
        mckl::StopWatch watch1;
        mckl::StopWatch watch2;
        mckl::StopWatch watch3;
#if MCKL_HAS_MKL
        mckl::StopWatch watch4;
#endif
        for (std::size_t i = 0; i != M; ++i) {
            std::size_t K = rsize(rng);
            num += K;
            r1.resize(K);
            r2.resize(K);
            r3.resize(K);
#if MCKL_HAS_MKL
            r4.resize(K);
#endif

            watch1.start();
            for (std::size_t j = 0; j != K; ++j)
                r1[j] = dist_std(rng);
            watch1.stop();

            watch2.start();
            for (std::size_t j = 0; j != K; ++j)
                r2[j] = dist_mckl(rng);
            watch2.stop();
            pass = pass && r1 != r2;

            watch3.start();
            mckl::rand(rng, dist_mckl, K, r3.data());
            watch3.stop();
            pass = pass && r1 != r3;

#if MCKL_HAS_MKL
            watch4.start();
            mckl::rand(random_mkl, dist_mckl, K, r4.data());
            watch4.stop();
            pass = pass && r1 != r4;
#endif

            mckl::RNGFast rng1(rng);
            mckl::RNGFast rng2(rng);
            std::stringstream ss;
            ss.precision(20);
            ss << dist_mckl;
            for (std::size_t j = 0; j != K; ++j)
                r1[j] = dist_mckl(rng1);
            ss >> dist_mckl;
            for (std::size_t j = 0; j != K; ++j)
                r2[j] = dist_mckl(rng2);
            pass = pass && r1 == r2;
        }
        c1 = std::min(c1, 1.0 * watch1.cycles() / num);
        c2 = std::min(c2, 1.0 * watch2.cycles() / num);
        c3 = std::min(c3, 1.0 * watch3.cycles() / num);
#if MCKL_HAS_MKL
        c4 = std::min(c4, 1.0 * watch4.cycles() / num);
#endif
    }

    RandomDistributionTrait<MCKLDistType> trait;
    std::cout << std::setw(nwid) << std::left << trait.name(param);
    std::cout << std::setw(twid) << std::right << c1;
    std::cout << std::setw(twid) << std::right << c2;
    std::cout << std::setw(twid) << std::right << c3;
#if MCKL_HAS_MKL
    std::cout << std::setw(twid) << std::right << c4;
#endif
    std::cout << std::setw(twid + 3) << std::right << random_pass(pass);
    std::cout << std::endl;
}

template <typename MCKLDistType>
inline void random_distribution_test_perf(
    std::size_t N, std::size_t M, int nwid, int twid)
{
    RandomDistributionTrait<MCKLDistType> trait;
    mckl::Vector<std::string> names;
    auto params = trait.params();
    for (const auto &param : params)
        random_distribution_test_perf<MCKLDistType>(N, M, param, nwid, twid);
}

template <template <typename> class DistributionType>
inline void random_distribution_perf(
    std::size_t N, std::size_t M, int nwid, int twid, std::true_type)
{
    random_distribution_test_perf<DistributionType<float>>(N, M, nwid, twid);
    random_distribution_test_perf<DistributionType<double>>(N, M, nwid, twid);
}

template <template <typename> class DistributionType>
inline void random_distribution_perf(
    std::size_t N, std::size_t M, int nwid, int twid, std::false_type)
{
    random_distribution_test_perf<DistributionType<std::int32_t>>(
        N, M, nwid, twid);
    random_distribution_test_perf<DistributionType<std::uint32_t>>(
        N, M, nwid, twid);
    random_distribution_test_perf<DistributionType<std::int64_t>>(
        N, M, nwid, twid);
    random_distribution_test_perf<DistributionType<std::uint64_t>>(
        N, M, nwid, twid);
}

template <template <typename> class DistributionType, typename ResultType>
inline void random_distribution_perf(std::size_t N, std::size_t M)
{
    N = std::max(N, static_cast<std::size_t>(10000));
    M = std::max(M, static_cast<std::size_t>(10));

    int nwid = 30;
    int twid = 12;

    std::size_t lwid =
        static_cast<std::size_t>(nwid + twid * (4 + MCKL_HAS_MKL) + 3);

    std::cout << std::string(lwid, '=') << std::endl;
    std::cout << std::setw(nwid) << std::left << "Distribution";
    std::cout << std::setw(twid) << std::right << "STD";
    std::cout << std::setw(twid) << std::right << "MCKL";
    std::cout << std::setw(twid) << std::right << "Batch";
#if MCKL_HAS_MKL
    std::cout << std::setw(twid) << std::right << "MKL";
#endif
    std::cout << std::setw(twid + 3) << std::right << "Deterministics";
    std::cout << std::endl;
    std::cout << std::string(lwid, '-') << std::endl;
    random_distribution_perf<DistributionType>(
        N, M, nwid, twid, std::is_floating_point<ResultType>());
    std::cout << std::string(lwid, '-') << std::endl;
}
