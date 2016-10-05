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

    std::cout << std::setw(20) << std::left << name;
    std::cout << std::setw(15) << std::right << random_pass(pass1);
    std::cout << std::setw(15) << std::right << random_pass(pass4);
    std::cout << std::setw(15) << std::right << random_pass(pass1r);
    std::cout << std::setw(15) << std::right << random_pass(pass4r);
    std::cout << std::endl;
}

inline void random_seed(std::size_t N)
{
    std::cout << std::string(80, '=') << std::endl;
    std::cout << std::setw(20) << std::left << "ResultType";
    std::cout << std::setw(15) << std::right << "np = 1";
    std::cout << std::setw(15) << std::right << "np = 4";
    std::cout << std::setw(15) << std::right << "np = 1 (R)";
    std::cout << std::setw(15) << std::right << "np = 4 (R)";
    std::cout << std::endl;
    std::cout << std::string(80, '-') << std::endl;
    random_seed<std::uint32_t>(N, "uint32_t");
    random_seed<std::uint64_t>(N, "uint64_t");
    random_seed<std::array<std::uint32_t, 1>>(N, "uint32_t[1]");
    random_seed<std::array<std::uint32_t, 2>>(N, "uint32_t[2]");
    random_seed<std::array<std::uint32_t, 4>>(N, "uint32_t[4]");
    random_seed<std::array<std::uint32_t, 8>>(N, "uint32_t[8]");
    random_seed<std::array<std::uint32_t, 16>>(N, "uint32_t[16]");
    random_seed<std::array<std::uint64_t, 1>>(N, "uint64_t[1]");
    random_seed<std::array<std::uint64_t, 2>>(N, "uint64_t[2]");
    random_seed<std::array<std::uint64_t, 4>>(N, "uint64_t[4]");
    random_seed<std::array<std::uint64_t, 8>>(N, "uint64_t[8]");
    random_seed<std::array<std::uint64_t, 16>>(N, "uint64_t[16]");
    std::cout << std::string(80, '-') << std::endl;
}

#endif // MCKL_EXAMPLE_RANDOM_SEED_HPP
