//============================================================================
// MCKL/example/resample/include/resample_u01_sequence.hpp
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

#ifndef MCKL_EXAMPLE_RESAMPLE_U01_SEQUENCE_HPP
#define MCKL_EXAMPLE_RESAMPLE_U01_SEQUENCE_HPP

#include <mckl/random/rng.hpp>
#include <mckl/resample/u01_sequence.hpp>
#include <mckl/utility/stop_watch.hpp>

template <typename U01SeqType, typename RealType>
inline void resample_u01_sequence(std::size_t N, std::size_t M, std::size_t L,
    int twid, const std::string &name)
{
    mckl::RNGFast rng;
    std::size_t num = 0;
    RealType error = 0;
    std::uniform_int_distribution<std::size_t> rsize(N / 2, N);
    U01SeqType u01seq;

    mckl::RNGFast rng1;
    mckl::RNGFast rng2;

    mckl::StopWatch watch1;
    mckl::StopWatch watch2;

    mckl::Vector<RealType> r1;
    mckl::Vector<RealType> r2;
    r1.reserve(N);
    r2.reserve(N);

    for (std::size_t i = 0; i != M; ++i) {
        std::size_t K = rsize(rng);
        num += K;
        r1.resize(K);
        r2.resize(K);

        watch1.start();
        mckl::U01Distribution<RealType> dist;
        mckl::rand(rng1, dist, std::min(L, K), r1.data());
        u01seq(K, r1.data(), r1.data());
        watch1.stop();

        watch2.start();
        u01seq(rng2, K, r2.data());
        watch2.stop();

        for (std::size_t j = 0; j != K; ++j)
            error = std::max(error, std::abs(r1[j] - r2[j]));
    }
    std::stringstream ss;
    ss << error / std::numeric_limits<RealType>::epsilon() << " eps";

    double c1 = 1.0 * watch1.cycles() / num;
    double c2 = 1.0 * watch2.cycles() / num;
    if (std::is_same<RealType, float>::value)
        std::cout << std::setw(twid) << std::left << "float";
    if (std::is_same<RealType, double>::value)
        std::cout << std::setw(twid) << std::left << "double";
    std::cout << std::setw(twid) << std::left << name;
    std::cout << std::setw(twid) << std::right << c1;
    std::cout << std::setw(twid) << std::right << c2;
    std::cout << std::setw(twid) << std::right << ss.str();
    std::cout << std::endl;
}

template <typename RealType>
inline void resample_u01_sequence(std::size_t N, std::size_t M, int twid)
{
    resample_u01_sequence<mckl::U01SequenceSorted, RealType>(
        N, M, N, twid, "Sorted");
    resample_u01_sequence<mckl::U01SequenceStratified, RealType>(
        N, M, N, twid, "Stratified");
    resample_u01_sequence<mckl::U01SequenceSystematic, RealType>(
        N, M, 1, twid, "Systematic");
}

inline void resample_u01_sequence(std::size_t N, std::size_t M)
{
    const int twid = 15;
    const std::size_t lwid = twid * 5;

    std::cout << std::string(lwid, '=') << std::endl;
    std::cout << std::setw(twid) << std::left << "Precision";
    std::cout << std::setw(twid) << std::left << "Algorithm";
    std::cout << std::setw(twid) << std::right << "CPE (Transform)";
    std::cout << std::setw(twid) << std::right << "CPE (Generate)";
    std::cout << std::setw(twid) << std::right << "Error";
    std::cout << std::endl;

    std::cout << std::fixed << std::setprecision(2);
    std::cout << std::string(lwid, '-') << std::endl;
    resample_u01_sequence<float>(N, M, twid);
    std::cout << std::string(lwid, '-') << std::endl;
    resample_u01_sequence<double>(N, M, twid);
    std::cout << std::string(lwid, '-') << std::endl;
}

#endif // MCKL_EXAMPLE_RESAMPLE_U01_SEQUENCE_HPP
