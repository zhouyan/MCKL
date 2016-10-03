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

template <typename RNGType>
inline void random_seed(std::size_t N, const std::string &name)
{
    using result_type = typename mckl::Seed<RNGType>::result_type;

    class id0;
    class id1;
    class id2;
    class id3;

    auto &s0 = mckl::Seed<RNGType, id0>::instance();
    auto &s1 = mckl::Seed<RNGType, id1>::instance();
    auto &s2 = mckl::Seed<RNGType, id2>::instance();
    auto &s3 = mckl::Seed<RNGType, id3>::instance();

    s0.partition(1, 0);
    s1.partition(1, 0);
    s2.partition(1, 0);
    s3.partition(1, 0);
    std::set<result_type> set1;
    for (std::size_t i = 0; i != N; ++i) {
        set1.insert(s0.get());
        set1.insert(s1.get());
        set1.insert(s2.get());
        set1.insert(s3.get());
    }
    bool pass1 = set1.size() == N;

    std::stringstream ss0;
    std::stringstream ss1;
    std::stringstream ss2;
    std::stringstream ss3;

    ss0 << s0;
    ss1 << s1;
    ss2 << s2;
    ss3 << s3;

    ss0 >> s0;
    ss1 >> s1;
    ss2 >> s2;
    ss3 >> s3;

    s0.partition(4, 0);
    s1.partition(4, 1);
    s2.partition(4, 2);
    s3.partition(4, 3);
    std::set<result_type> set4;
    for (std::size_t i = 0; i != N; ++i) {
        set4.insert(s0.get());
        set4.insert(s1.get());
        set4.insert(s2.get());
        set4.insert(s3.get());
    }
    bool pass4 = set4.size() == N * 4;

    std::cout << std::string(40, '=') << std::endl;
    std::cout << std::setw(20) << std::left << "RNGType";
    std::cout << std::setw(10) << std::right << "np = 1";
    std::cout << std::setw(10) << std::right << "np = 4";
    std::cout << std::endl;
    std::cout << std::string(40, '-') << std::endl;
    std::cout << std::setw(20) << std::left << name;
    std::cout << std::setw(10) << std::right << (pass1 ? "Passed" : "Failed");
    std::cout << std::setw(10) << std::right << (pass4 ? "Passed" : "Failed");
    std::cout << std::endl;
    std::cout << std::string(40, '-') << std::endl;
}

#endif // MCKL_EXAMPLE_RANDOM_SEED_HPP
