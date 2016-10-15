//============================================================================
// MCKL/example/random/include/random_rng.hpp
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

#ifndef MCKL_EXAMPLE_RANDOM_RNG_HPP
#define MCKL_EXAMPLE_RANDOM_RNG_HPP

#ifndef MCKL_EXAMPLE_RANDOM_STD_RNG
#define MCKL_EXAMPLE_RANDOM_STD_RNG 0
#endif

#ifndef MCKL_EXAMPLE_RANDOM_AES_RNG
#define MCKL_EXAMPLE_RANDOM_AES_RNG 0
#endif

#ifndef MCKL_EXAMPLE_RANDOM_PHILOX_RNG
#define MCKL_EXAMPLE_RANDOM_PHILOX_RNG 0
#endif

#ifndef MCKL_EXAMPLE_RANDOM_THREEFRY_RNG
#define MCKL_EXAMPLE_RANDOM_THREEFRY_RNG 0
#endif

#ifndef MCKL_EXAMPLE_RANDOM_MKL_RNG
#define MCKL_EXAMPLE_RANDOM_MKL_RNG 0
#endif

#ifndef MCKL_EXAMPLE_RANDOM_RDRAND_RNG
#define MCKL_EXAMPLE_RANDOM_RDRAND_RNG 0
#endif

#include <mckl/random/uniform_bits_distribution.hpp>
#include "random_common.hpp"

#if MCKL_EXAMPLE_RANDOM_STD_RNG || MCKL_EXAMPLE_RANDOM_MKL_RNG ||             \
    MCKL_EXAMPLE_RANDOM_RDRAND_RNG

template <typename RNGType>
inline bool random_rng_k(const RNGType &)
{
    return true;
}

#else // MCKL_EXAMPLE_RANDOM_STD_RNG || MCKL_EXAMPLE_RANDOM_MKL_RNG ||
// MCKL_EXAMPLE_RANDOM_RDRAND_RNG

template <typename RNGType>
inline bool random_rng_k(const RNGType &, const std::string &filename)
{
    using result_type = typename RNGType::result_type;

    std::ifstream kat(filename);
    mckl::Vector<result_type> k;
    result_type x;
    while (kat >> x)
        k.push_back(x);
    kat.close();

    RNGType rng;
    mckl::Vector<result_type> r(k.size());
    mckl::rand(rng, k.size(), r.data());

    return k == r;
}

#endif // MCKL_EXAMPLE_RANDOM_STD_RNG || MCKL_EXAMPLE_RANDOM_MKL_RNG ||
       // MCKL_EXAMPLE_RANDOM_RDRAND_RNG

#if MCKL_HAS_AESNI && MCKL_EXAMPLE_RANDOM_AES_RNG

template <typename ResultType, std::size_t Rounds>
inline bool random_rng_k(const mckl::AES128Engine<ResultType, Rounds> &rng)
{
    std::string filename("random_ase128");
    if (std::numeric_limits<ResultType>::digits == 64)
        filename += "_64";
    filename += ".txt";

    return random_rng_k(rng, filename);
}

template <typename ResultType, std::size_t Rounds>
inline bool random_rng_k(const mckl::AES192Engine<ResultType, Rounds> &rng)
{
    std::string filename("random_ase192");
    if (std::numeric_limits<ResultType>::digits == 64)
        filename += "_64";
    filename += ".txt";

    return random_rng_k(rng, filename);
}

template <typename ResultType, std::size_t Rounds>
inline bool random_rng_k(const mckl::AES256Engine<ResultType, Rounds> &rng)
{
    std::string filename("random_ase256");
    if (std::numeric_limits<ResultType>::digits == 64)
        filename += "_64";
    filename += ".txt";

    return random_rng_k(rng, filename);
}

template <typename ResultType, std::size_t Rounds>
inline bool random_rng_k(const mckl::ARSEngine<ResultType, Rounds> &rng)
{
    std::string filename("random_ars");
    if (std::numeric_limits<ResultType>::digits == 64)
        filename += "_64";
    filename += ".txt";

    return random_rng_k(rng, filename);
}

#endif // MCKL_HAS_AESNI && MCKL_EXAMPLE_RANDOM_AES_RNG

#if MCKL_EXAMPLE_RANDOM_PHILOX_RNG

template <typename ResultType, typename T, std::size_t K>
inline bool random_rng_k(const mckl::PhiloxEngine<ResultType, T, K> &rng)
{
    std::string filename("random_philox");
    filename += std::to_string(K) + "x";
    filename += std::to_string(std::numeric_limits<T>::digits);
    if (std::numeric_limits<ResultType>::digits == 64)
        filename += "_64";
    filename += ".txt";

    return random_rng_k(rng, filename);
}

#endif // MCKL_EXAMPLE_RANDOM_PHILOX_RNG

#if MCKL_EXAMPLE_RANDOM_THREEFRY_RNG

template <typename ResultType, typename T, std::size_t K>
inline bool random_rng_k(const mckl::ThreefryEngine<ResultType, T, K> &rng)
{
    std::string filename("random_threefry");
    filename += std::to_string(K) + "x";
    filename += std::to_string(std::numeric_limits<T>::digits);
    if (std::numeric_limits<ResultType>::digits == 64)
        filename += "_64";
    filename += ".txt";

    return random_rng_k(rng, filename);
}

template <typename ResultType>
inline bool random_rng_k(const mckl::Threefish256Engine<ResultType> &rng)
{
    std::string filename("random_threefish256");
    if (std::numeric_limits<ResultType>::digits == 64)
        filename += "_64";
    filename += ".txt";

    return random_rng_k(rng, filename);
}

template <typename ResultType>
inline bool random_rng_k(const mckl::Threefish512Engine<ResultType> &rng)
{
    std::string filename("random_threefish512");
    if (std::numeric_limits<ResultType>::digits == 64)
        filename += "_64";
    filename += ".txt";

    return random_rng_k(rng, filename);
}

template <typename ResultType>
inline bool random_rng_k(const mckl::Threefish1024Engine<ResultType> &rng)
{
    std::string filename("random_threefish1024");
    if (std::numeric_limits<ResultType>::digits == 64)
        filename += "_64";
    filename += ".txt";

    return random_rng_k(rng, filename);
}

#endif // MCKL_EXAMPLE_RANDOM_THREEFRY_RNG

struct RandomRNGPerf {
    bool pass;
    double c1;
    double c2;
};

template <typename RNGType>
inline bool random_rng_d(std::size_t N, std::size_t M)
{
    using result_type = typename RNGType::result_type;

    std::uniform_int_distribution<std::size_t> rsize(0, N);
    mckl::UniformBitsDistribution<std::uint16_t> ubits16;
    mckl::UniformBitsDistribution<std::uint32_t> ubits32;
    mckl::UniformBitsDistribution<std::uint64_t> ubits64;
    RNGType rng;
    bool pass = true;

    RNGType rng1;
    RNGType rng2;

    mckl::Vector<result_type> r1;
    mckl::Vector<result_type> r2;
    mckl::Vector<std::uint16_t> u16_1;
    mckl::Vector<std::uint16_t> u16_2;
    mckl::Vector<std::uint32_t> u32_1;
    mckl::Vector<std::uint32_t> u32_2;
    mckl::Vector<std::uint64_t> u64_1;
    mckl::Vector<std::uint64_t> u64_2;
    r1.reserve(N);
    r2.reserve(N);
    u16_1.reserve(N);
    u16_2.reserve(N);
    u32_1.reserve(N);
    u32_2.reserve(N);
    u64_1.reserve(N);
    u64_2.reserve(N);

    for (std::size_t i = 0; i != M; ++i) {
        std::size_t K = rsize(rng);
        r1.resize(K);
        r2.resize(K);
        u16_1.resize(K);
        u16_2.resize(K);
        u32_1.resize(K);
        u32_2.resize(K);
        u64_1.resize(K);
        u64_2.resize(K);

        for (std::size_t j = 0; j != K; ++j)
            r1[j] = rng1();
        mckl::rand(rng2, K, r2.data());
        pass = pass && (r1 == r2 || rng != rng);

        for (std::size_t j = 0; j != K; ++j)
            u16_1[j] = mckl::rand(rng1, ubits16);
        mckl::rand(rng2, ubits16, K, u16_2.data());
        pass = pass && (u16_1 == u16_2 || rng != rng);

        for (std::size_t j = 0; j != K; ++j)
            u32_1[j] = mckl::rand(rng1, ubits32);
        mckl::rand(rng2, ubits32, K, u32_2.data());
        pass = pass && (u32_1 == u32_2 || rng != rng);

        for (std::size_t j = 0; j != K; ++j)
            u64_1[j] = mckl::rand(rng1, ubits64);
        mckl::rand(rng2, ubits64, K, u64_2.data());
        pass = pass && (u64_1 == u64_2 || rng != rng);

        rng1.discard(static_cast<unsigned>(K));
        typename RNGType::result_type next = rng1();
        for (std::size_t j = 0; j != K; ++j)
            rng2();
        bool find = false;
        for (std::size_t j = 0; j != 2; ++j)
            find = find || rng2() == next;
        pass = pass && (find || rng != rng);

        std::stringstream ss;
        ss << rng;
        mckl::rand(rng, K, r1.data());
        ss >> rng;
        mckl::rand(rng, K, r2.data());
        pass = pass && (r1 == r2 || rng != rng);

        RNGType tmp(rng);
        mckl::rand(rng, K, r1.data());
        mckl::rand(tmp, K, r2.data());
        pass = pass && (r1 == r2 || rng != rng);

        tmp = rng;
        mckl::rand(rng, K, r1.data());
        mckl::rand(tmp, K, r2.data());
        pass = pass && (r1 == r2 || rng != rng);
    }

    return pass;
}

template <typename RNGType>
inline RandomRNGPerf random_rng_s(std::size_t N, std::size_t M)
{
    mckl::UniformBitsDistribution<std::uint64_t> ubits;
    std::uniform_int_distribution<std::size_t> rsize(N / 2, N);
    RNGType rng;
    bool pass = true;

    RNGType rng1;
    RNGType rng2;

    mckl::Vector<std::uint64_t> r1(N);
    mckl::Vector<std::uint64_t> r2(N);

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

            watch1.start();
            for (std::size_t j = 0; j != K; ++j)
                r1[j] = mckl::rand(rng1, ubits);
            watch1.stop();

            watch2.start();
            mckl::rand(rng2, ubits, K, r2.data());
            watch2.stop();

            std::uniform_int_distribution<std::size_t> ridx(0, K - 1);
            std::size_t idx = ridx(rng);
            pass = pass && (r1[idx] == r2[idx] || rng != rng);
        }
        std::size_t bytes = sizeof(std::uint64_t) * n;
        if (has_cycles) {
            c1 = std::min(c1, 1.0 * watch1.cycles() / bytes);
            c2 = std::min(c2, 1.0 * watch2.cycles() / bytes);
        } else {
            c1 = std::max(c1, bytes / watch1.seconds() * 1e-9);
            c2 = std::max(c2, bytes / watch2.seconds() * 1e-9);
        }
    }

    return {pass, c1, c2};
}

#if !MCKL_CROSS_COMPILING

template <typename RNGType>
inline RandomRNGPerf random_rng_p(std::size_t N, std::size_t M)
{
    bool pass = true;

    std::size_t P = std::thread::hardware_concurrency();

    mckl::Vector<RNGType> rs1(P);
    mckl::Vector<RNGType> rs2(P);
    for (unsigned i = 0; i != P; ++i) {
        rs1[i].seed(i + 1);
        rs2[i].seed(i + 1);
    }

    mckl::Vector<std::uint64_t> r1(N * P);
    mckl::Vector<std::uint64_t> r2(N * P);
    mckl::Vector<std::uint64_t> s1(M * P);
    mckl::Vector<std::uint64_t> s2(M * P);
    mckl::Vector<std::size_t> n1(M * P);
    mckl::Vector<std::size_t> n2(M * P);

    auto worker1 = [N, M, &rs1, &r1, &s1, &n1](std::size_t p) {
        mckl::UniformBitsDistribution<std::uint64_t> ubits;
        std::uniform_int_distribution<std::size_t> rsize(N / 2, N);
        RNGType rng = std::move(rs1[p]);
        std::uint64_t *const r = r1.data() + N * p;
        for (std::size_t i = 0; i != M; ++i) {
            std::size_t K = rsize(rng);
            for (std::size_t j = 0; j != K; ++j)
                r[j] = mckl::rand(rng, ubits);
            std::uniform_int_distribution<std::size_t> ridx(0, K - 1);
            std::size_t idx = ridx(rng);
            s1[M * p + i] = r[idx];
            n1[M * p + i] = K;
        }
        rs1[p] = std::move(rng);
    };

    auto worker2 = [N, M, &rs2, &r2, &s2, &n2](std::size_t p) {
        mckl::UniformBitsDistribution<std::uint64_t> ubits;
        std::uniform_int_distribution<std::size_t> rsize(N / 2, N);
        RNGType rng = std::move(rs2[p]);
        std::uint64_t *const r = r2.data() + N * p;
        for (std::size_t i = 0; i != M; ++i) {
            std::size_t K = rsize(rng);
            mckl::rand(rng, ubits, K, r);
            std::uniform_int_distribution<std::size_t> ridx(0, K - 1);
            std::size_t idx = ridx(rng);
            s2[M * p + i] = r[idx];
            n2[M * p + i] = K;
        }
        rs2[p] = std::move(rng);
    };

    bool has_cycles = mckl::StopWatch::has_cycles();

    double c1 = has_cycles ? std::numeric_limits<double>::max() : 0.0;
    double c2 = has_cycles ? std::numeric_limits<double>::max() : 0.0;
    for (std::size_t k = 0; k != 10; ++k) {
        mckl::StopWatch watch1;
        mckl::StopWatch watch2;

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

        pass = pass && (s1 == s2 || rs1[0] != rs1[0]);
        pass = pass && (n1 == n2 || rs1[0] != rs1[0]);

        std::size_t bytes1 = sizeof(std::uint64_t) *
            std::accumulate(n1.begin(), n1.end(), static_cast<std::size_t>(0));
        std::size_t bytes2 = sizeof(std::uint64_t) *
            std::accumulate(n2.begin(), n2.end(), static_cast<std::size_t>(0));
        if (has_cycles) {
            c1 = std::min(c1, 1.0 * watch1.cycles() / bytes1);
            c2 = std::min(c2, 1.0 * watch2.cycles() / bytes2);
        } else {
            c1 = std::max(c1, bytes1 / watch1.seconds() * 1e-9);
            c2 = std::max(c2, bytes2 / watch2.seconds() * 1e-9);
        }
    }

    return {pass, c1, c2};
}

#endif // !MCKL_CROSS_COMPILING

template <typename RNGType>
inline double random_rng_e(const RNGType &, std::size_t, std::size_t)
{
    return 0;
}

template <typename ResultType, typename Generator>
inline double random_rng_e(
    const mckl::CounterEngine<ResultType, Generator> &rng, std::size_t N,
    std::size_t M)
{
    using ctr_type =
        typename mckl::CounterEngine<ResultType, Generator>::ctr_type;

    std::uniform_int_distribution<std::size_t> rsize(N / 2, N);
    auto rngs = rng;
    ctr_type ctr = {{0}};
    mckl::Vector<ctr_type> r(N);
    mckl::StopWatch watch;

    bool has_cycles = mckl::StopWatch::has_cycles();

    double c = has_cycles ? std::numeric_limits<double>::max() : 0.0;
    for (std::size_t i = 0; i != M; ++i) {
        std::size_t K = rsize(rngs);
        watch.start();
        for (std::size_t j = 0; j != K; ++j) {
            rng.generator().enc(ctr.data(), r[j].data());
            ++ctr.front();
        }
        watch.stop();
        std::uniform_int_distribution<std::size_t> ridx(0, K - 1);
        std::size_t idx = ridx(rngs);
        if (r[idx].front() == 0)
            continue;
        std::size_t bytes = K * sizeof(ctr_type);
        c = has_cycles ? std::min(c, 1.0 * watch.cycles() / bytes) :
                         std::max(c, bytes / watch.seconds() * 1e-9);
    }

    return c;
}

template <typename RNGType>
inline void random_rng(std::size_t N, std::size_t M, const std::string &name)
{
    const int nwid = 20;
    const int swid = 8;
    const int twid = 7;
#if MCKL_CROSS_COMPILING
    const std::size_t lwid = nwid + swid * 2 + twid * 4 + 15;
#else
    const std::size_t lwid = nwid + swid * 2 + twid * 9 + 15;
#endif

    bool pass_k = random_rng_k(RNGType());
    bool pass_d = random_rng_d<RNGType>(N, M);
    RandomRNGPerf perf_s(random_rng_s<RNGType>(N, M));
#if !MCKL_CROSS_COMPILING
    RandomRNGPerf perf_p = random_rng_p<RNGType>(N, M);
#endif
    double cpb = random_rng_e(RNGType(), N, M);

    std::cout << std::fixed << std::setprecision(2);

    std::cout << std::string(lwid, '=') << std::endl;

    std::cout << std::setw(nwid) << std::left << "RNG";
    std::cout << std::setw(swid) << std::right << "Size";
    std::cout << std::setw(swid) << std::right << "Align";
    std::cout << std::setw(twid) << std::right << "S";
    std::cout << std::setw(twid) << std::right << "B";
#if !MCKL_CROSS_COMPILING
    std::cout << std::setw(twid) << std::right << "SP";
    std::cout << std::setw(twid) << std::right << "BP";
#endif
    std::cout << std::setw(twid) << std::right << "S/B";
#if !MCKL_CROSS_COMPILING
    std::cout << std::setw(twid) << std::right << "SP/BP";
    std::cout << std::setw(twid) << std::right << "S/SP";
    std::cout << std::setw(twid) << std::right << "B/BP";
#endif
    std::cout << std::setw(twid) << std::right << "E";
    std::cout << std::setw(15) << std::right << "Deterministics";
    std::cout << std::endl;

    std::cout << std::string(lwid, '-') << std::endl;

    std::cout << std::setw(nwid) << std::left << name;
    std::cout << std::setw(swid) << std::right << sizeof(RNGType);
    std::cout << std::setw(swid) << std::right << alignof(RNGType);
    std::cout << std::setw(twid) << std::right << perf_s.c1;
    std::cout << std::setw(twid) << std::right << perf_s.c2;
#if !MCKL_CROSS_COMPILING
    std::cout << std::setw(twid) << std::right << perf_p.c1;
    std::cout << std::setw(twid) << std::right << perf_p.c2;
#endif
    std::cout << std::setw(twid) << std::right << perf_s.c1 / perf_s.c2;
#if !MCKL_CROSS_COMPILING
    std::cout << std::setw(twid) << std::right << perf_p.c1 / perf_p.c2;
    std::cout << std::setw(twid) << std::right << perf_s.c1 / perf_p.c1;
    std::cout << std::setw(twid) << std::right << perf_s.c2 / perf_p.c2;
#endif
    if (cpb > 0)
        std::cout << std::setw(twid) << std::right << cpb;
    else
        std::cout << std::setw(twid) << std::right << "---";

    std::string pass;
    pass += pass_k ? "-" : "*";
    pass += pass_d ? "-" : "*";
    pass += perf_s.pass ? "-" : "*";
#if MCKL_CROSS_COMPILING
    pass += random_pass(pass_k && pass_d && perf_s.pass);
#else
    pass += perf_p.pass ? "-" : "*";
    pass += random_pass(pass_k && pass_d && perf_s.pass && perf_p.pass);
#endif
    std::cout << std::setw(15) << std::right << pass;
    std::cout << std::endl;

    std::cout << std::string(lwid, '-') << std::endl;
}

#endif // MCKL_EXAMPLE_RANDOM_RNG_HPP
