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

#ifndef MCKL_EXAMPLE_RANDOM_R123_RNG
#define MCKL_EXAMPLE_RANDOM_R123_RNG 0
#endif

#ifndef MCKL_EXAMPLE_RANDOM_MKL_RNG
#define MCKL_EXAMPLE_RANDOM_MKL_RNG 0
#endif

#ifndef MCKL_EXAMPLE_RANDOM_RDRAND_RNG
#define MCKL_EXAMPLE_RANDOM_RDRAND_RNG 0
#endif

#include <mckl/random/u01_distribution.hpp>
#include <mckl/random/uniform_bits_distribution.hpp>
#include <mckl/random/uniform_int_distribution.hpp>
#include "random_common.hpp"

#if MCKL_EXAMPLE_RANDOM_STD_RNG || MCKL_EXAMPLE_RANDOM_R123_RNG ||            \
    MCKL_EXAMPLE_RANDOM_MKL_RNG || MCKL_EXAMPLE_RANDOM_RDRAND_RNG

template <typename RNGType>
inline bool random_rng_k(const RNGType &)
{
    return true;
}

#else // MCKL_EXAMPLE_RANDOM_STD_RNG || MCKL_EXAMPLE_RANDOM_R123_RNG ||
// MCKL_EXAMPLE_RANDOM_MKL_RNG || MCKL_EXAMPLE_RANDOM_RDRAND_RNG

template <typename RNGType>
inline bool random_rng_k(const RNGType &, const std::string &filename)
{
    using result_type = typename RNGType::result_type;
    using key_type = typename RNGType::key_type;

    std::ifstream kat(filename);
    mckl::Vector<result_type> k;
    result_type x;
    while (kat >> x)
        k.push_back(x);
    kat.close();

    RNGType rng;
    mckl::Vector<result_type> r(k.size());
    mckl::rand(rng, k.size(), r.data());

    key_type kk;
    for (std::size_t i = 0; i != kk.size(); ++i)
        kk[i] = static_cast<typename key_type::value_type>(rng());
    rng.seed(kk);
    key_type rk = rng.key();

    return k.size() != 0 && k == r && kk == rk;
}

#endif // MCKL_EXAMPLE_RANDOM_STD_RNG || MCKL_EXAMPLE_RANDOM_MKL_RNG ||
       // MCKL_EXAMPLE_RANDOM_RDRAND_RNG

#if MCKL_EXAMPLE_RANDOM_AES_RNG

template <typename ResultType, std::size_t Rounds>
inline bool random_rng_k(const mckl::AES128Engine<ResultType, Rounds> &rng)
{
    std::string filename("random_aes128");
    if (std::numeric_limits<ResultType>::digits == 64)
        filename += "_64";
    filename += ".txt";

    return random_rng_k(rng, filename);
}

template <typename ResultType, std::size_t Rounds>
inline bool random_rng_k(const mckl::AES192Engine<ResultType, Rounds> &rng)
{
    std::string filename("random_aes192");
    if (std::numeric_limits<ResultType>::digits == 64)
        filename += "_64";
    filename += ".txt";

    return random_rng_k(rng, filename);
}

template <typename ResultType, std::size_t Rounds>
inline bool random_rng_k(const mckl::AES256Engine<ResultType, Rounds> &rng)
{
    std::string filename("random_aes256");
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

#endif // MCKL_EXAMPLE_RANDOM_AES_RNG

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

class RandomRNGPerf
{
    public:
    bool pass;
    double c1;
    double c2;
}; // class RandomRNGPerf

template <typename RNGType>
inline bool random_rng_d(std::size_t N, std::size_t M)
{
    using result_type = typename RNGType::result_type;

    mckl::UniformIntDistribution<std::size_t> rsize(0, N);
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

template <typename RNGType, typename DistributionType>
inline RandomRNGPerf random_rng_p(std::size_t N, std::size_t M)
{
    using result_type = typename DistributionType::result_type;

    RNGType rng;
    DistributionType dist;
    mckl::UniformIntDistribution<std::size_t> rsize(N / 2, N);
    bool pass = true;

    RNGType rng1;
    RNGType rng2;

    mckl::Vector<result_type> r1(N);
    mckl::Vector<result_type> r2(N);

    bool has_cycles = mckl::StopWatch::has_cycles();

    double c1 = has_cycles ? std::numeric_limits<double>::max() : 0.0;
    double c2 = has_cycles ? std::numeric_limits<double>::max() : 0.0;
    for (std::size_t k = 0; k != 10; ++k) {
        std::size_t n = 0;
        mckl::StopWatch watch1;
        mckl::StopWatch watch2;
        for (std::size_t i = 0; i != M; ++i) {
            std::size_t K = rsize(rng);
            r1.resize(K);
            r2.resize(K);
            n += K;

            watch1.start();
            for (std::size_t j = 0; j != K; ++j)
                r1[j] = mckl::rand(rng1, dist);
            watch1.stop();

            watch2.start();
            mckl::rand(rng2, dist, K, r2.data());
            watch2.stop();

            pass = pass && (r1 == r2 || rng != rng);
        }
        if (std::is_integral<result_type>::value) {
            std::size_t bytes = sizeof(result_type) * n;
            if (has_cycles) {
                c1 = std::min(c1, 1.0 * watch1.cycles() / bytes);
                c2 = std::min(c2, 1.0 * watch2.cycles() / bytes);
            } else {
                c1 = std::max(c1, bytes / watch1.seconds() * 1e-9);
                c2 = std::max(c2, bytes / watch2.seconds() * 1e-9);
            }
        } else {
            if (has_cycles) {
                c1 = std::min(c1, 1.0 * watch1.cycles() / n);
                c2 = std::min(c2, 1.0 * watch2.cycles() / n);
            } else {
                c1 = std::max(c1, n / watch1.seconds() * 1e-6);
                c2 = std::max(c2, n / watch2.seconds() * 1e-6);
            }
        }
    }

    return {pass, c1, c2};
}

template <typename RNGType>
inline void random_rng(std::size_t N, std::size_t M, const std::string &name)
{
    const int nwid = 33;
    const int swid = 6;
    const int twid = 10;
    const std::size_t lwid = nwid + swid * 2 + twid * 2 + 15;

    bool pass_k = random_rng_k(RNGType());
    bool pass_d = random_rng_d<RNGType>(N, M);

    using ubits = mckl::UniformBitsDistribution<typename RNGType::result_type>;
    RandomRNGPerf perf_ubits(random_rng_p<RNGType, ubits>(N, M));

    std::cout << std::fixed << std::setprecision(2);

    std::cout << std::string(lwid, '=') << std::endl;

    std::cout << std::setw(nwid) << std::left << "RNG";
    std::cout << std::setw(swid) << std::right << "Size";
    std::cout << std::setw(swid) << std::right << "Align";
    if (mckl::StopWatch::has_cycles()) {
        std::cout << std::setw(twid) << std::right << "cpB (S)";
        std::cout << std::setw(twid) << std::right << "cpB (B)";
    } else {
        std::cout << std::setw(twid) << std::right << "GB/s (S)";
        std::cout << std::setw(twid) << std::right << "GB/s (B)";
    }
    std::cout << std::setw(15) << std::right << "Deterministics";
    std::cout << std::endl;

    std::cout << std::string(lwid, '-') << std::endl;

    std::cout << std::setw(nwid) << std::left << name;
    std::cout << std::setw(swid) << std::right << sizeof(RNGType);
    std::cout << std::setw(swid) << std::right << alignof(RNGType);
    std::cout << std::setw(twid) << std::right << perf_ubits.c1;
    std::cout << std::setw(twid) << std::right << perf_ubits.c2;
    std::string pass;
    pass += pass_k ? "-" : "*";
    pass += pass_d ? "-" : "*";
    pass += perf_ubits.pass ? "-" : "*";
    pass += random_pass(pass_k && pass_d && perf_ubits.pass);
    std::cout << std::setw(15) << std::right << pass;
    std::cout << std::endl;

    std::cout << std::string(lwid, '-') << std::endl;
}

#endif // MCKL_EXAMPLE_RANDOM_RNG_HPP
