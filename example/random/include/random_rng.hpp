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

#include <mckl/random/uniform_bits_distribution.hpp>
#include "random_common.hpp"

template <typename T>
inline mckl::Vector<T> random_rng_kat(const std::string &filename)
{
    std::ifstream kat(filename);
    mckl::Vector<T> k;
    T x;
    while (kat >> x)
        k.push_back(x);
    kat.close();

    return k;
}

template <typename RNGType>
inline bool random_rng_kat(const RNGType &)
{
    return true;
}

template <typename ResultType, typename T, std::size_t K, std::size_t Rounds>
inline bool random_rng_kat(mckl::PhiloxEngine<ResultType, T, K, Rounds> &rng)
{
    std::string filename("random_Philox");
    filename += std::to_string(K) + "x";
    filename += std::to_string(std::numeric_limits<T>::digits) + ".txt";
    mckl::Vector<T> k = random_rng_kat<T>(filename);
    mckl::Vector<ResultType> r(256);
    rng(256, r.data());

    return std::memcmp(k.data(), r.data(), sizeof(ResultType) * 256) == 0;
}

template <typename ResultType, typename T, std::size_t K, std::size_t Rounds>
inline bool random_rng_kat(mckl::ThreefryEngine<ResultType, T, K, Rounds> &rng)
{
    std::string filename("random_Threefry");
    filename += std::to_string(K) + "x";
    filename += std::to_string(std::numeric_limits<T>::digits) + ".txt";
    mckl::Vector<T> k = random_rng_kat<T>(filename);
    mckl::Vector<ResultType> r(256);
    rng(256, r.data());

    return std::memcmp(k.data(), r.data(), sizeof(ResultType) * 256) == 0;
}

#if MCKL_HAS_AESNI

template <typename ResultType, std::size_t Blocks>
inline bool random_rng_kat(mckl::AES128Engine<ResultType, Blocks> &rng)
{
    mckl::Vector<std::uint64_t> k =
        random_rng_kat<std::uint64_t>("random_AES128.txt");
    mckl::Vector<ResultType> r(256);
    rng(256, r.data());

    return std::memcmp(k.data(), r.data(), sizeof(ResultType) * 256) == 0;
}

template <typename ResultType, std::size_t Blocks>
inline bool random_rng_kat(mckl::AES192Engine<ResultType, Blocks> &rng)
{
    mckl::Vector<std::uint64_t> k =
        random_rng_kat<std::uint64_t>("random_AES192.txt");
    mckl::Vector<ResultType> r(256);
    rng(256, r.data());

    return std::memcmp(k.data(), r.data(), sizeof(ResultType) * 256) == 0;
}

template <typename ResultType, std::size_t Blocks>
inline bool random_rng_kat(mckl::AES256Engine<ResultType, Blocks> &rng)
{
    mckl::Vector<std::uint64_t> k =
        random_rng_kat<std::uint64_t>("random_AES256.txt");
    mckl::Vector<ResultType> r(256);
    rng(256, r.data());

    return std::memcmp(k.data(), r.data(), sizeof(ResultType) * 256) == 0;
}

template <typename ResultType, std::size_t Rounds, std::size_t Blocks>
inline bool random_rng_kat(mckl::ARSEngine<ResultType, Rounds, Blocks> &rng)
{
    mckl::Vector<std::uint64_t> k =
        random_rng_kat<std::uint64_t>("random_ARS.txt");
    mckl::Vector<ResultType> r(256);
    rng(256, r.data());

    return std::memcmp(k.data(), r.data(), sizeof(ResultType) * 256) == 0;
}

#endif // MCKL_HAS_AESNI

template <typename RNGType>
inline std::string random_rng_size(const RNGType &)
{
    return std::to_string(sizeof(RNGType));
}

template <typename ResultType, typename Generator>
inline std::string random_rng_size(
    const mckl::CounterEngine<ResultType, Generator> &)
{
    using RNGType = mckl::CounterEngine<ResultType, Generator>;

    std::size_t align = std::max(alignof(ResultType), alignof(Generator));
    std::size_t size = sizeof(Generator) +
        sizeof(typename Generator::ctr_type) + Generator::size() + 1;
    if (size % align != 0)
        size += align - size % align;

    return std::to_string(size) + "/" + std::to_string(sizeof(RNGType));
}

template <typename RNGType>
inline void random_rng(std::size_t N, std::size_t M, int nwid, int swid,
    int twid, const std::string &name,
    const mckl::Vector<std::string> &rngname)
{
    if (!rngname.empty()) {
        auto iter = std::find_if(
            rngname.begin(), rngname.end(), [&name](const std::string &rname) {
                return name.find(rname) != std::string::npos;
            });
        if (iter == rngname.end())
            return;
    }

    RNGType rng;
    bool pass = random_rng_kat(rng);
    mckl::UniformBitsDistribution<std::uint64_t> ubits;
    std::uniform_int_distribution<std::size_t> rsize(N / 2, N);

    RNGType rng1;
    RNGType rng2;

    mckl::Vector<std::uint64_t> r1;
    mckl::Vector<std::uint64_t> r2;
    r1.reserve(N);
    r2.reserve(N);

    double g1 = 0;
    double g2 = 0;
    double c1 = std::numeric_limits<double>::max();
    double c2 = std::numeric_limits<double>::max();
    for (std::size_t k = 0; k != 10; ++k) {
        std::size_t num = 0;
        mckl::StopWatch watch1;
        mckl::StopWatch watch2;
        for (std::size_t i = 0; i != M; ++i) {
            std::size_t K = rsize(rng);
            num += K;
            r1.resize(K);
            r2.resize(K);

            watch1.start();
            for (std::size_t j = 0; j != K; ++j)
                r1[j] = mckl::rand(rng1, ubits);
            watch1.stop();

            watch2.start();
            mckl::rand(rng2, ubits, K, r2.data());
            watch2.stop();
            pass = pass && (r1 == r2 || rng != rng);

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
            mckl::rand(rng, ubits, K, r1.data());
            ss >> rng;
            mckl::rand(rng, ubits, K, r2.data());
            pass = pass && (r1 == r2 || rng != rng);
        }
        double bytes = static_cast<double>(sizeof(std::uint64_t) * num);
        g1 = std::max(g1, bytes / watch1.nanoseconds());
        g2 = std::max(g2, bytes / watch2.nanoseconds());
        c1 = std::min(c1, 1.0 * watch1.cycles() / bytes);
        c2 = std::min(c2, 1.0 * watch2.cycles() / bytes);
    }

    std::cout << std::setw(nwid) << std::left << name;
    std::cout << std::setw(swid) << std::right << random_rng_size(rng);
    std::cout << std::setw(swid) << std::right << alignof(RNGType);
    std::cout << std::setw(twid) << std::right << g1;
    std::cout << std::setw(twid) << std::right << g2;
    std::cout << std::setw(twid) << std::right << c1;
    std::cout << std::setw(twid) << std::right << c2;
    std::cout << std::setw(twid) << std::right << random_pass(pass);
    std::cout << std::endl;
}

inline void random_rng(std::size_t N, std::size_t M, int argc, char **argv)
{
    mckl::Vector<std::string> rngname;
    for (int i = 0; i != argc; ++i)
        rngname.push_back(argv[i]);

    const int nwid = 20;
    const int swid = 8;
    const int twid = 15;
    const std::size_t lwid = nwid + swid * 2 + twid * 5;

    std::cout << std::string(lwid, '=') << std::endl;
    std::cout << std::setw(nwid) << std::left << "RNGType";
    std::cout << std::setw(swid) << std::right << "Size";
    std::cout << std::setw(swid) << std::right << "Align";
    std::cout << std::setw(twid) << std::right << "GB/s (Loop)";
    std::cout << std::setw(twid) << std::right << "GB/s (Batch)";
    std::cout << std::setw(twid) << std::right << "CPB (Loop)";
    std::cout << std::setw(twid) << std::right << "CPB (Batch)";
    std::cout << std::setw(twid) << std::right << "Deterministics";
    std::cout << std::endl;

#ifdef MCKL_RNG_DEFINE_MACRO
#undef MCKL_RNG_DEFINE_MACRO
#endif

#ifdef MCKL_RNG_DEFINE_MACRO_NA
#undef MCKL_RNG_DEFINE_MACRO_NA
#endif

#define MCKL_RNG_DEFINE_MACRO(RNGType, Name, name)                            \
    random_rng<RNGType>(N, M, nwid, swid, twid, #Name, rngname);

    std::cout << std::fixed << std::setprecision(2);
    std::cout << std::string(lwid, '-') << std::endl;

#include <mckl/random/internal/rng_define_macro_std.hpp>

#include <mckl/random/internal/rng_define_macro.hpp>

#include <mckl/random/internal/rng_define_macro_mkl.hpp>

    std::cout << std::string(lwid, '-') << std::endl;
}

#endif // MCKL_EXAMPLE_RANDOM_RNG_HPP
