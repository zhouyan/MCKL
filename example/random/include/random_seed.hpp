//============================================================================
// MCKL/example/random/include/random_seed.hpp
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

#ifndef MCKL_EXAMPLE_RANDOM_SEED_HPP
#define MCKL_EXAMPLE_RANDOM_SEED_HPP

#include <mckl/random/seed.hpp>
#include "random_common.hpp"

template <typename T, std::size_t K>
class RandomSeedResultType
{
    public:
    using type = std::array<T, K>;
}; // class RandomSeedResultType

template <typename T>
class RandomSeedResultType<T, 1>
{
    public:
    using type = T;
}; // class RandomSeedResultType

template <typename T>
inline bool random_seed_unique(mckl::Vector<T> &set)
{
    std::sort(set.begin(), set.end());
    for (std::size_t i = 1; i < set.size(); ++i)
        if (set[i] == set[i - 1])
            return false;
    return true;
}

template <typename T, std::size_t K>
inline void random_seed(std::size_t N)
{
    using result_type = typename RandomSeedResultType<T, K>::type;

    static constexpr int bits = sizeof(T) * CHAR_BIT;

    class id0;
    class id1;
    class id2;
    class id3;

    auto &s0 = mckl::SeedGenerator<result_type, id0, false, false>::instance();
    auto &s1 = mckl::SeedGenerator<result_type, id1, false, false>::instance();
    auto &s2 = mckl::SeedGenerator<result_type, id2, false, false>::instance();
    auto &s3 = mckl::SeedGenerator<result_type, id3, false, false>::instance();
    auto &s0r = mckl::SeedGenerator<result_type, id0, true, false>::instance();
    auto &s1r = mckl::SeedGenerator<result_type, id1, true, false>::instance();
    auto &s2r = mckl::SeedGenerator<result_type, id2, true, false>::instance();
    auto &s3r = mckl::SeedGenerator<result_type, id3, true, false>::instance();

    const std::string name(std::to_string(K) + "x" + std::to_string(bits));

    mckl::Vector<T> k;
    mckl::Vector<T> kr;
    T x;
    T xr;
    std::ifstream kat("random_seed_" + name + ".txt");
    while (kat >> x >> xr) {
        k.push_back(x);
        kr.push_back(xr);
    }
    kat.close();

    mckl::Vector<T> r;
    mckl::Vector<T> rr;
    for (std::size_t i = 0; i != k.size() / K; ++i) {
        union {
            std::array<T, K> a;
            result_type s;
        } buf;
        buf.s = s0.get();
        for (std::size_t j = 0; j != K; ++j)
            r.push_back(buf.a[j]);
    }
    for (std::size_t i = 0; i != k.size() / K; ++i) {
        union {
            std::array<T, K> a;
            result_type s;
        } buf;
        buf.s = s0r.get();
        for (std::size_t j = 0; j != K; ++j)
            rr.push_back(buf.a[j]);
    }

    bool pass1 = k == r;
    bool pass1r = kr == rr;

    mckl::StopWatch watch1;
    mckl::StopWatch watch2;

    watch1.start();
    for (std::size_t i = 0; i != N; ++i)
        s0.get();
    watch1.stop();

    watch2.start();
    for (std::size_t i = 0; i != N; ++i)
        s0r.get();
    watch2.stop();

    s0.partition(1, 0);
    s0r.partition(1, 0);
    mckl::Vector<result_type> set1;
    mckl::Vector<result_type> set1r;
    set1.reserve(N);
    set1r.reserve(N);
    for (std::size_t i = 0; i != N; ++i) {
        set1.push_back(s0.get());
        set1r.push_back(s0r.get());
    }
    pass1 = pass1 && random_seed_unique(set1);
    pass1r = pass1r && random_seed_unique(set1r);

    std::stringstream ss0;
    ss0 << s0;
    ss0 >> s0;

    std::stringstream ss0r;
    ss0r << s0r;
    ss0r >> s0r;

    s0.partition(4, 0);
    s1.partition(4, 1);
    s2.partition(4, 2);
    s3.partition(4, 3);
    s0r.partition(4, 0);
    s1r.partition(4, 1);
    s2r.partition(4, 2);
    s3r.partition(4, 3);
    mckl::Vector<result_type> set4;
    mckl::Vector<result_type> set4r;
    set4.reserve(N * 4);
    set4r.reserve(N * 4);
    for (std::size_t i = 0; i != N; ++i) {
        set4.push_back(s0.get());
        set4.push_back(s1.get());
        set4.push_back(s2.get());
        set4.push_back(s3.get());
        set4r.push_back(s0r.get());
        set4r.push_back(s1r.get());
        set4r.push_back(s2r.get());
        set4r.push_back(s3r.get());
    }
    bool pass4 = random_seed_unique(set4);
    bool pass4r = random_seed_unique(set4r);

    std::size_t bytes = sizeof(result_type) * N;
    double c1 = mckl::StopWatch::has_cycles() ?
        1.0 * watch1.cycles() / bytes :
        bytes / watch1.seconds() * 1e-9;
    double c2 = mckl::StopWatch::has_cycles() ?
        1.0 * watch2.cycles() / bytes :
        bytes / watch2.seconds() * 1e-9;

    std::cout << std::setw(20) << std::left << name;
    std::cout << std::setw(10) << std::right << c1;
    std::cout << std::setw(10) << std::right << c2;
    std::cout << std::setw(15) << std::right << random_pass(pass1);
    std::cout << std::setw(15) << std::right << random_pass(pass4);
    std::cout << std::setw(15) << std::right << random_pass(pass1r);
    std::cout << std::setw(15) << std::right << random_pass(pass4r);
    std::cout << std::endl;
}

template <typename T>
inline void random_seed(std::size_t N)
{
    std::string type =
        "uint" + std::to_string(std::numeric_limits<T>::digits) + "_t";

    random_seed<T, 0x01>(N);
    random_seed<T, 0x02>(N);
    random_seed<T, 0x03>(N);
    random_seed<T, 0x04>(N);
    random_seed<T, 0x05>(N);
    random_seed<T, 0x06>(N);
    random_seed<T, 0x07>(N);
    random_seed<T, 0x08>(N);
    random_seed<T, 0x09>(N);
    random_seed<T, 0x0A>(N);
    random_seed<T, 0x0B>(N);
    random_seed<T, 0x0C>(N);
    random_seed<T, 0x0D>(N);
    random_seed<T, 0x0E>(N);
    random_seed<T, 0x0F>(N);
    random_seed<T, 0x10>(N);
}

inline void random_seed(std::size_t N)
{
    std::cout << std::fixed << std::setprecision(2);
    std::cout << std::string(100, '=') << std::endl;
    std::cout << std::setw(20) << std::left << "ResultType";
    if (mckl::StopWatch::has_cycles()) {
        std::cout << std::setw(10) << std::right << "CPB";
        std::cout << std::setw(10) << std::right << "CPB (R)";
    } else {
        std::cout << std::setw(10) << std::right << "GB/s";
        std::cout << std::setw(10) << std::right << "GB/s (R)";
    }
    std::cout << std::setw(15) << std::right << "np = 1";
    std::cout << std::setw(15) << std::right << "np = 4";
    std::cout << std::setw(15) << std::right << "np = 1 (R)";
    std::cout << std::setw(15) << std::right << "np = 4 (R)";
    std::cout << std::endl;
    std::cout << std::string(100, '-') << std::endl;
    random_seed<std::uint32_t>(N);
    random_seed<std::uint64_t>(N);
    std::cout << std::string(100, '-') << std::endl;
}

#endif // MCKL_EXAMPLE_RANDOM_SEED_HPP
