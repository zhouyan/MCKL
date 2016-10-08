//============================================================================
// MCKL/example/random/include/random_seed.hpp
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

#ifndef MCKL_EXAMPLE_RANDOM_SEED_HPP
#define MCKL_EXAMPLE_RANDOM_SEED_HPP

#include <mckl/random/seed.hpp>
#include "random_common.hpp"

template <typename ResultType>
inline std::uint32_t random_seed_val(const ResultType &s)
{
    static constexpr std::size_t M =
        sizeof(ResultType) / sizeof(std::uint32_t);

    union {
        std::array<std::uint32_t, M> v;
        ResultType s;
    } buf;

    buf.s = s;

    return std::get<0>(buf.v);
}

template <typename ResultType>
inline void random_seed(std::size_t N, const std::string &name)
{
    using result_type = typename mckl::SeedGenerator<ResultType>::result_type;

    class id0;
    class id1;
    class id2;
    class id3;

    auto &s0 = mckl::SeedGenerator<ResultType, id0, false>::instance();
    auto &s1 = mckl::SeedGenerator<ResultType, id1, false>::instance();
    auto &s2 = mckl::SeedGenerator<ResultType, id2, false>::instance();
    auto &s3 = mckl::SeedGenerator<ResultType, id3, false>::instance();
    auto &s0r = mckl::SeedGenerator<ResultType, id0, true>::instance();
    auto &s1r = mckl::SeedGenerator<ResultType, id1, true>::instance();
    auto &s2r = mckl::SeedGenerator<ResultType, id2, true>::instance();
    auto &s3r = mckl::SeedGenerator<ResultType, id3, true>::instance();

    mckl::StopWatch watch1;
    mckl::StopWatch watch2;

    watch1.start();
    for (std::size_t i = 0; i != N; ++i) {
        s0.get();
        s1.get();
        s2.get();
        s3.get();
    }
    watch1.stop();

    watch2.start();
    for (std::size_t i = 0; i != N; ++i) {
        s0r.get();
        s1r.get();
        s2r.get();
        s3r.get();
    }
    watch2.stop();

    s0.partition(1, 0);
    s1.partition(1, 0);
    s2.partition(1, 0);
    s3.partition(1, 0);
    s0r.partition(1, 0);
    s1r.partition(1, 0);
    s2r.partition(1, 0);
    s3r.partition(1, 0);
    std::set<result_type> set1;
    std::set<result_type> set1r;
    for (std::size_t i = 0; i != N; ++i) {
        set1.insert(s0.get());
        set1.insert(s1.get());
        set1.insert(s2.get());
        set1.insert(s3.get());
        set1r.insert(s0r.get());
        set1r.insert(s1r.get());
        set1r.insert(s2r.get());
        set1r.insert(s3r.get());
    }
    bool pass1 = set1.size() == N;
    bool pass1r = set1r.size() == N;

    std::stringstream ss0;
    std::stringstream ss1;
    std::stringstream ss2;
    std::stringstream ss3;
    std::stringstream ss0r;
    std::stringstream ss1r;
    std::stringstream ss2r;
    std::stringstream ss3r;

    ss0 << s0;
    ss1 << s1;
    ss2 << s2;
    ss3 << s3;
    ss0r << s0r;
    ss1r << s1r;
    ss2r << s2r;
    ss3r << s3r;

    ss0 >> s0;
    ss1 >> s1;
    ss2 >> s2;
    ss3 >> s3;
    ss0r >> s0r;
    ss1r >> s1r;
    ss2r >> s2r;
    ss3r >> s3r;

    s0.partition(4, 0);
    s1.partition(4, 1);
    s2.partition(4, 2);
    s3.partition(4, 3);
    s0r.partition(4, 0);
    s1r.partition(4, 1);
    s2r.partition(4, 2);
    s3r.partition(4, 3);
    std::set<result_type> set4;
    std::set<result_type> set4r;
    for (std::size_t i = 0; i != N; ++i) {
        set4.insert(s0.get());
        set4.insert(s1.get());
        set4.insert(s2.get());
        set4.insert(s3.get());
        set4r.insert(s0r.get());
        set4r.insert(s1r.get());
        set4r.insert(s2r.get());
        set4r.insert(s3r.get());
    }
    bool pass4 = set4.size() == N * 4;
    bool pass4r = set4r.size() == N * 4;

    std::size_t bytes = N * 4 * sizeof(result_type);
    double c1 = 1.0 * watch1.cycles() / bytes;
    double c2 = 1.0 * watch2.cycles() / bytes;

    std::cout << std::setw(20) << std::left << name;
    std::cout << std::setw(10) << std::right << c1;
    std::cout << std::setw(10) << std::right << c2;
    std::cout << std::setw(15) << std::right << random_pass(pass1);
    std::cout << std::setw(15) << std::right << random_pass(pass4);
    std::cout << std::setw(15) << std::right << random_pass(pass1r);
    std::cout << std::setw(15) << std::right << random_pass(pass4r);
    std::cout << std::endl;

    std::ofstream os(
        "random_seed_" + std::to_string(sizeof(result_type)) + ".txt");
    os << std::hex << std::uppercase;
    os << std::setw(10) << std::right << "s0";
    os << std::setw(10) << std::right << "s1";
    os << std::setw(10) << std::right << "s2";
    os << std::setw(10) << std::right << "s3";
    os << std::setw(10) << std::right << "s0r";
    os << std::setw(10) << std::right << "s1r";
    os << std::setw(10) << std::right << "s2r";
    os << std::setw(10) << std::right << "s3r";
    os << '\n';
    for (std::size_t i = 0; i != N; ++i) {
        os << std::setw(10) << std::right << random_seed_val(s0.get());
        os << std::setw(10) << std::right << random_seed_val(s1.get());
        os << std::setw(10) << std::right << random_seed_val(s2.get());
        os << std::setw(10) << std::right << random_seed_val(s3.get());
        os << std::setw(10) << std::right << random_seed_val(s0r.get());
        os << std::setw(10) << std::right << random_seed_val(s1r.get());
        os << std::setw(10) << std::right << random_seed_val(s2r.get());
        os << std::setw(10) << std::right << random_seed_val(s3r.get());
        os << '\n';
    }
}

template <typename T>
inline void random_seed(std::size_t N)
{
    std::string type =
        "uint" + std::to_string(std::numeric_limits<T>::digits) + "_t";

    random_seed<T>(N, type);
    random_seed<std::array<T, 0x01>>(N, type + "[0x01]");
    random_seed<std::array<T, 0x02>>(N, type + "[0x02]");
    random_seed<std::array<T, 0x03>>(N, type + "[0x03]");
    random_seed<std::array<T, 0x04>>(N, type + "[0x04]");
    random_seed<std::array<T, 0x05>>(N, type + "[0x05]");
    random_seed<std::array<T, 0x06>>(N, type + "[0x06]");
    random_seed<std::array<T, 0x07>>(N, type + "[0x07]");
    random_seed<std::array<T, 0x08>>(N, type + "[0x08]");
    random_seed<std::array<T, 0x09>>(N, type + "[0x09]");
    random_seed<std::array<T, 0x0A>>(N, type + "[0x0A]");
    random_seed<std::array<T, 0x0B>>(N, type + "[0x0B]");
    random_seed<std::array<T, 0x0C>>(N, type + "[0x0C]");
    random_seed<std::array<T, 0x0D>>(N, type + "[0x0D]");
    random_seed<std::array<T, 0x0E>>(N, type + "[0x0E]");
    random_seed<std::array<T, 0x0F>>(N, type + "[0x0F]");
    random_seed<std::array<T, 0x10>>(N, type + "[0x10]");
}

inline void random_seed(std::size_t N)
{
    std::cout << std::fixed << std::setprecision(2);
    std::cout << std::string(100, '=') << std::endl;
    std::cout << std::setw(20) << std::left << "ResultType";
    std::cout << std::setw(10) << std::right << "CPB";
    std::cout << std::setw(10) << std::right << "CPB (R)";
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
