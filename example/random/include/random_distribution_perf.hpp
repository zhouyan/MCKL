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

#include <mckl/random/rng_set.hpp>
#include "random_distribution.hpp"

struct RandomDistributionPerf {
    std::string name;
    bool pass;
    double c1;
    double c2;
    double c3;
    double c4;
};

template <typename MCKLDistType, typename ParamType, std::size_t ParamNum>
inline void random_distribution_perf_d(std::size_t N, std::size_t M,
    const std::array<ParamType, ParamNum> &param, mckl::Vector<bool> &perf)
{
    using result_type = typename MCKLDistType::result_type;

    RNG01<mckl::RNGFast> rng01;
    mckl::RNGFast rng1;
    mckl::RNGFast rng2;
    mckl::RNGFast rng;

    std::uniform_int_distribution<std::size_t> rsize(N / 2, N);
    MCKLDistType dist(random_distribution_init<MCKLDistType>(param));
    bool pass = true;

    mckl::Vector<result_type> r(N);
    mckl::Vector<result_type> r1(N);
    mckl::Vector<result_type> r2(N);
    for (std::size_t i = 0; i != M; ++i) {
        std::size_t K = rsize(rng);
        r.resize(K);
        r1.resize(K);
        r2.resize(K);

        mckl::rand(rng01, dist, K, r.data());
        for (std::size_t j = 0; j != K; ++j) {
            pass = pass && std::isfinite(r[j]);
            pass = pass && std::isfinite(dist(rng01));
        }

        std::stringstream ss1;
        ss1.precision(20);
        ss1 << dist;
        for (std::size_t j = 0; j != K; ++j)
            r1[j] = dist(rng1);
        ss1 >> dist;
        for (std::size_t j = 0; j != K; ++j)
            r2[j] = dist(rng2);
        pass = pass && r1 == r2;

        std::stringstream ssb;
        ssb << dist;
        mckl::rand(rng1, dist, K, r1.data());
        ssb >> dist;
        mckl::rand(rng2, dist, K, r2.data());
        pass = pass && r1 == r2;
    }
    perf.push_back(pass);
}

template <typename MCKLDistType>
inline void random_distribution_perf_d(
    std::size_t N, std::size_t M, mckl::Vector<bool> &perf)
{
    RandomDistributionTrait<MCKLDistType> trait;
    auto params = trait.params();
    for (const auto &param : params)
        random_distribution_perf_d<MCKLDistType>(N, M, param, perf);
}

template <template <typename> class DistributionType>
inline void random_distribution_perf_d(
    std::size_t N, std::size_t M, mckl::Vector<bool> &perf, std::true_type)
{
    random_distribution_perf_d<DistributionType<float>>(N, M, perf);
    random_distribution_perf_d<DistributionType<double>>(N, M, perf);
}

template <template <typename> class DistributionType>
inline void random_distribution_perf_d(
    std::size_t N, std::size_t M, mckl::Vector<bool> &perf, std::false_type)
{
    random_distribution_perf_d<DistributionType<std::int32_t>>(N, M, perf);
    random_distribution_perf_d<DistributionType<std::uint32_t>>(N, M, perf);
    random_distribution_perf_d<DistributionType<std::int64_t>>(N, M, perf);
    random_distribution_perf_d<DistributionType<std::uint64_t>>(N, M, perf);
}

template <typename MCKLDistType, typename ParamType, std::size_t ParamNum>
inline void random_distribution_perf_s(std::size_t N, std::size_t M,
    const std::array<ParamType, ParamNum> &param,
    mckl::Vector<RandomDistributionPerf> &perf)
{
    using result_type = typename MCKLDistType::result_type;
    using std_type = typename RandomDistributionTrait<MCKLDistType>::std_type;

#if MCKL_HAS_AESNI
    mckl::ARS rng;
#else
    mckl::Philox4x32 rng;
#endif

#if MCKL_HAS_MKL
#if MCKL_HAS_AESNI
    mckl::MKL_ARS5 rng_mkl;
#else
    mckl::MKL_PHILOX4X32X10 rng_mkl;
#endif
#endif

    std::uniform_int_distribution<std::size_t> rsize(N / 2, N);
    MCKLDistType dist_mckl(random_distribution_init<MCKLDistType>(param));
    std_type dist_std(random_distribution_init<std_type>(param));
    bool pass = true;

    mckl::Vector<result_type> r1(N);
    mckl::Vector<result_type> r2(N);
    mckl::Vector<result_type> r3(N);
#if MCKL_HAS_MKL
    mckl::Vector<result_type> r4(N);
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
                r1[j] = mckl::rand(rng, dist_std);
            watch1.stop();

            watch2.start();
            for (std::size_t j = 0; j != K; ++j)
                r2[j] = mckl::rand(rng, dist_mckl);
            watch2.stop();

            watch3.start();
            mckl::rand(rng, dist_mckl, K, r3.data());
            watch3.stop();

#if MCKL_HAS_MKL
            watch4.start();
            mckl::rand(rng_mkl, dist_mckl, K, r4.data());
            watch4.stop();
#endif

            pass = pass && r1 != r2;
            pass = pass && r1 != r3;
#if MCKL_HAS_MKL
            pass = pass && r1 != r4;
#endif
        }
        c1 = std::min(c1, 1.0 * watch1.cycles() / num);
        c2 = std::min(c2, 1.0 * watch2.cycles() / num);
        c3 = std::min(c3, 1.0 * watch3.cycles() / num);
#if MCKL_HAS_MKL
        c4 = std::min(c4, 1.0 * watch4.cycles() / num);
#endif
    }

    RandomDistributionTrait<MCKLDistType> trait;
    RandomDistributionPerf result;
    result.name = trait.name(param);
    result.pass = pass;
    result.c1 = c1;
    result.c2 = c2;
    result.c3 = c3;
#if MCKL_HAS_MKL
    result.c4 = c4;
#endif

    perf.push_back(result);
}

template <typename MCKLDistType>
inline void random_distribution_perf_s(
    std::size_t N, std::size_t M, mckl::Vector<RandomDistributionPerf> &perf)
{
    RandomDistributionTrait<MCKLDistType> trait;
    auto params = trait.params();
    for (const auto &param : params)
        random_distribution_perf_s<MCKLDistType>(N, M, param, perf);
}

template <template <typename> class DistributionType>
inline void random_distribution_perf_s(std::size_t N, std::size_t M,
    mckl::Vector<RandomDistributionPerf> &perf, std::true_type)
{
    random_distribution_perf_s<DistributionType<float>>(N, M, perf);
    random_distribution_perf_s<DistributionType<double>>(N, M, perf);
}

template <template <typename> class DistributionType>
inline void random_distribution_perf_s(std::size_t N, std::size_t M,
    mckl::Vector<RandomDistributionPerf> &perf, std::false_type)
{
    random_distribution_perf_s<DistributionType<std::int32_t>>(N, M, perf);
    random_distribution_perf_s<DistributionType<std::uint32_t>>(N, M, perf);
    random_distribution_perf_s<DistributionType<std::int64_t>>(N, M, perf);
    random_distribution_perf_s<DistributionType<std::uint64_t>>(N, M, perf);
}

template <typename MCKLDistType, typename ParamType, std::size_t ParamNum>
inline void random_distribution_perf_p(std::size_t N, std::size_t M,
    const std::array<ParamType, ParamNum> &param,
    mckl::Vector<RandomDistributionPerf> &perf)
{
    using result_type = typename MCKLDistType::result_type;
    using std_type = typename RandomDistributionTrait<MCKLDistType>::std_type;

    std::size_t P = std::thread::hardware_concurrency();

#if MCKL_HAS_AESNI
    mckl::RNGSetVector<mckl::ARS> rs(P);
#else
    mckl::RNGSetVector<mckl::Philox4x32> rs(P);
#endif

#if MCKL_HAS_MKL
#if MCKL_HAS_AESNI
    mckl::RNGSetVector<mckl::MKL_ARS5> rs_mkl(P);
#else
    mckl::RNGSetVector<mckl::MKL_PHILOX4X32X10> rs_mkl(P);
#endif
#endif

    mckl::Vector<MCKLDistType> dist_mckl(P,
        random_distribution_init<MCKLDistType>(param));
    mckl::Vector<std_type> dist_std(P,
        random_distribution_init<std_type>(param));
    bool pass = true;

    mckl::Vector<result_type> r1(N * P);
    mckl::Vector<result_type> r2(N * P);
    mckl::Vector<result_type> r3(N * P);
#if MCKL_HAS_MKL
    mckl::Vector<result_type> r4(N * P);
#endif

    mckl::Vector<std::size_t> n1(M * P);
    mckl::Vector<std::size_t> n2(M * P);
    mckl::Vector<std::size_t> n3(M * P);
#if MCKL_HAS_MKL
    mckl::Vector<std::size_t> n4(M * P);
#endif

    auto worker1 = [N, M, &rs, &dist_std, &r1, &n1](std::size_t p) {
        std::uniform_int_distribution<std::size_t> rsize(N / 2, N);
        auto rng = std::move(rs[p]);
        std_type dist = std::move(dist_std[p]);
        result_type *const r = r1.data() + N * p;
        for (std::size_t i = 0; i != M; ++i) {
            std::size_t K = rsize(rng);
            for (std::size_t j = 0; j != K; ++j)
                r[j] = mckl::rand(rng, dist);
            n1[M * p + i] = K;
        }
        rs[p] = std::move(rng);
        dist_std[p] = std::move(dist);
    };

    auto worker2 = [N, M, &rs, &dist_mckl, &r2, &n2](std::size_t p) {
        std::uniform_int_distribution<std::size_t> rsize(N / 2, N);
        auto rng = std::move(rs[p]);
        MCKLDistType dist = std::move(dist_mckl[p]);
        result_type *const r = r2.data() + N * p;
        for (std::size_t i = 0; i != M; ++i) {
            std::size_t K = rsize(rng);
            for (std::size_t j = 0; j != K; ++j)
                r[j] = mckl::rand(rng, dist);
            n2[M * p + i] = K;
        }
        rs[p] = std::move(rng);
        dist_mckl[p] = std::move(dist);
    };

    auto worker3 = [N, M, &rs, &dist_mckl, &r3, &n3](std::size_t p) {
        std::uniform_int_distribution<std::size_t> rsize(N / 2, N);
        auto rng = std::move(rs[p]);
        MCKLDistType dist = std::move(dist_mckl[p]);
        result_type *const r = r3.data() + N * p;
        for (std::size_t i = 0; i != M; ++i) {
            std::size_t K = rsize(rng);
            mckl::rand(rng, dist, K, r);
            n3[M * p + i] = K;
        }
        rs[p] = std::move(rng);
        dist_mckl[p] = std::move(dist);
    };

#if MCKL_HAS_MKL
    auto worker4 = [N, M, &rs_mkl, &dist_mckl, &r4, &n4](std::size_t p) {
        std::uniform_int_distribution<std::size_t> rsize(N / 2, N);
        auto rng = std::move(rs_mkl[p]);
        MCKLDistType dist = std::move(dist_mckl[p]);
        result_type *const r = r4.data() + N * p;
        for (std::size_t i = 0; i != M; ++i) {
            std::size_t K = rsize(rng);
            mckl::rand(rng, dist, K, r);
            n4[M * p + i] = K;
        }
        rs_mkl[p] = std::move(rng);
        dist_mckl[p] = std::move(dist);
    };
#endif

    double c1 = std::numeric_limits<double>::max();
    double c2 = std::numeric_limits<double>::max();
    double c3 = std::numeric_limits<double>::max();
#if MCKL_HAS_MKL
    double c4 = std::numeric_limits<double>::max();
#endif
    for (std::size_t k = 0; k != 10; ++k) {
        mckl::StopWatch watch1;
        mckl::StopWatch watch2;
        mckl::StopWatch watch3;
#if MCKL_HAS_MKL
        mckl::StopWatch watch4;
#endif

        mckl::Vector<std::future<void>> task1;
        task1.reserve(P);
        watch1.start();
        for (std::size_t p = 0; p != P; ++p)
            task1.push_back(std::async(std::launch::async, worker1, p));
        for (std::size_t p = 0; p != P; ++p)
            task1[p].wait();
        watch1.stop();

        mckl::Vector<std::future<void>> task2;
        task2.reserve(P);
        watch2.start();
        for (std::size_t p = 0; p != P; ++p)
            task2.push_back(std::async(std::launch::async, worker2, p));
        for (std::size_t p = 0; p != P; ++p)
            task2[p].wait();
        watch2.stop();

        mckl::Vector<std::future<void>> task3;
        task3.reserve(P);
        watch3.start();
        for (std::size_t p = 0; p != P; ++p)
            task3.push_back(std::async(std::launch::async, worker3, p));
        for (std::size_t p = 0; p != P; ++p)
            task3[p].wait();
        watch3.stop();

#if MCKL_HAS_MKL
        mckl::Vector<std::future<void>> task4;
        task4.reserve(P);
        watch4.start();
        for (std::size_t p = 0; p != P; ++p)
            task4.push_back(std::async(std::launch::async, worker4, p));
        for (std::size_t p = 0; p != P; ++p)
            task4[p].wait();
        watch4.stop();
#endif

        pass = pass && r1 != r2;
        pass = pass && r1 != r3;
#if MCKL_HAS_MKL
        pass = pass && r1 != r4;
#endif

        std::size_t num1 =
            std::accumulate(n1.begin(), n1.end(), static_cast<std::size_t>(0));
        std::size_t num2 =
            std::accumulate(n2.begin(), n2.end(), static_cast<std::size_t>(0));
        std::size_t num3 =
            std::accumulate(n3.begin(), n3.end(), static_cast<std::size_t>(0));
#if MCKL_HAS_MKL
        std::size_t num4 =
            std::accumulate(n4.begin(), n4.end(), static_cast<std::size_t>(0));
#endif
        c1 = std::min(c1, 1.0 * watch1.cycles() / num1);
        c2 = std::min(c2, 1.0 * watch2.cycles() / num2);
        c3 = std::min(c3, 1.0 * watch3.cycles() / num3);
#if MCKL_HAS_MKL
        c4 = std::min(c4, 1.0 * watch4.cycles() / num4);
#endif
    }

    RandomDistributionTrait<MCKLDistType> trait;
    RandomDistributionPerf result;
    result.name = trait.name(param);
    result.pass = pass;
    result.c1 = c1;
    result.c2 = c2;
    result.c3 = c3;
#if MCKL_HAS_MKL
    result.c4 = c4;
#endif

    perf.push_back(result);
}

template <typename MCKLDistType>
inline void random_distribution_perf_p(
    std::size_t N, std::size_t M, mckl::Vector<RandomDistributionPerf> &perf)
{
    RandomDistributionTrait<MCKLDistType> trait;
    auto params = trait.params();
    for (const auto &param : params)
        random_distribution_perf_p<MCKLDistType>(N, M, param, perf);
}

template <template <typename> class DistributionType>
inline void random_distribution_perf_p(std::size_t N, std::size_t M,
    mckl::Vector<RandomDistributionPerf> &perf, std::true_type)
{
    random_distribution_perf_p<DistributionType<float>>(N, M, perf);
    random_distribution_perf_p<DistributionType<double>>(N, M, perf);
}

template <template <typename> class DistributionType>
inline void random_distribution_perf_p(std::size_t N, std::size_t M,
    mckl::Vector<RandomDistributionPerf> &perf, std::false_type)
{
    random_distribution_perf_p<DistributionType<std::int32_t>>(N, M, perf);
    random_distribution_perf_p<DistributionType<std::uint32_t>>(N, M, perf);
    random_distribution_perf_p<DistributionType<std::int64_t>>(N, M, perf);
    random_distribution_perf_p<DistributionType<std::uint64_t>>(N, M, perf);
}

template <template <typename> class DistributionType, typename ResultType>
inline void random_distribution_perf(std::size_t N, std::size_t M)
{
    N = std::max(N, static_cast<std::size_t>(10000));
    M = std::max(M, static_cast<std::size_t>(10));

    mckl::Vector<bool> perf_d;
    random_distribution_perf_d<DistributionType>(
        N, M, perf_d, std::is_floating_point<ResultType>());

    mckl::Vector<RandomDistributionPerf> perf_s;
    random_distribution_perf_s<DistributionType>(
        N, M, perf_s, std::is_floating_point<ResultType>());

    mckl::Vector<RandomDistributionPerf> perf_p;
    random_distribution_perf_p<DistributionType>(
        N, M, perf_p, std::is_floating_point<ResultType>());

    const int nwid = 30;
    const int twid = 12;
    std::size_t lwid = nwid + twid * (3 + MCKL_HAS_MKL) + 20;

    std::cout << std::fixed << std::setprecision(2);

    std::cout << std::string(lwid, '=') << std::endl;

    std::cout << std::setw(nwid) << std::left << "Distribution";
    std::cout << std::setw(twid) << std::right << "STD";
    std::cout << std::setw(twid) << std::right << "MCKL";
    std::cout << std::setw(twid) << std::right << "Batch";
#if MCKL_HAS_MKL
    std::cout << std::setw(twid) << std::right << "MKL";
#endif
    std::cout << std::setw(5) << std::right << "Mode";
    std::cout << std::setw(15) << std::right << "Deterministics";
    std::cout << std::endl;

    std::cout << std::string(lwid, '-') << std::endl;

    for (std::size_t i = 0; i != perf_s.size(); ++i) {
        std::cout << std::setw(nwid) << std::left << perf_s[i].name;
        std::cout << std::setw(twid) << std::right << perf_s[i].c1;
        std::cout << std::setw(twid) << std::right << perf_s[i].c2;
        std::cout << std::setw(twid) << std::right << perf_s[i].c3;
#if MCKL_HAS_MKL
        std::cout << std::setw(twid) << std::right << perf_s[i].c4;
#endif
        std::cout << std::setw(5) << std::right << "S";

        std::string pass;
        pass += perf_d[i] ? "-" : "*";
        pass += perf_s[i].pass ? "-" : "*";
        pass += random_pass(perf_d[i] && perf_s[i].pass);
        std::cout << std::setw(15) << std::right << pass;
        std::cout << std::endl;
    }

    std::cout << std::string(lwid, '-') << std::endl;

    for (std::size_t i = 0; i != perf_p.size(); ++i) {
        std::cout << std::setw(nwid) << std::left << perf_p[i].name;
        std::cout << std::setw(twid) << std::right << perf_p[i].c1;
        std::cout << std::setw(twid) << std::right << perf_p[i].c2;
        std::cout << std::setw(twid) << std::right << perf_p[i].c3;
#if MCKL_HAS_MKL
        std::cout << std::setw(twid) << std::right << perf_p[i].c4;
#endif
        std::cout << std::setw(5) << std::right << "P";

        std::string pass;
        pass += perf_d[i] ? "-" : "*";
        pass += perf_p[i].pass ? "-" : "*";
        pass += random_pass(perf_d[i] && perf_p[i].pass);
        std::cout << std::setw(15) << std::right << pass;
        std::cout << std::endl;
    }

    std::cout << std::string(lwid, '-') << std::endl;
}
