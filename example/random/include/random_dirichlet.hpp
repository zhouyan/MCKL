//============================================================================
// MCKL/example/random/include/random_dirichlet.hpp
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

#ifndef MCKL_EXAMPLE_RANDOM_DIRICHLET_HPP
#define MCKL_EXAMPLE_RANDOM_DIRICHLET_HPP

#include <mckl/core/matrix.hpp>
#include <mckl/random/dirichlet_distribution.hpp>
#include "random_distribution.hpp"

template <typename RealType>
inline void random_dirichlet(
    std::size_t N, std::size_t M, const RealType *alpha, bool scalar)
{
    using param_type =
        typename mckl::DirichletDistribution<RealType>::param_type;

    MCKLRNGType rng;
    MCKLRNGType rng1;
    MCKLRNGType rng2;
#if MCKL_HAS_MKL
    MKLRNGType rng_mkl;
#endif

    mckl::UniformIntDistribution<std::size_t> rsize(N / 2, N);
    mckl::DirichletDistribution<RealType> dist;
    if (scalar)
        dist.param(param_type(2, alpha[0]));
    else
        dist.param(param_type(2, alpha));

    bool pass = true;

    mckl::Matrix<mckl::RowMajor, RealType> r1;
    mckl::Matrix<mckl::RowMajor, RealType> r2;
#if MCKL_HAS_MKL
    mckl::Matrix<mckl::RowMajor, RealType> r3;
#endif
    for (std::size_t i = 0; i != M; ++i) {
        std::size_t K = rsize(rng);
        r1.resize(K, 2);
        r2.resize(K, 2);

        std::stringstream ss1;
        ss1.precision(20);
        ss1 << dist;
        for (std::size_t j = 0; j != K; ++j)
            dist(rng1, r1.data() + j * 2);
        ss1 >> dist;
        for (std::size_t j = 0; j != K; ++j)
            dist(rng2, r2.data() + j * 2);
        pass = pass && r1 == r2;

        std::stringstream ssb;
        ssb << dist;
        mckl::rand(rng1, dist, K, r1.data());
        ssb >> dist;
        mckl::rand(rng2, dist, K, r2.data());
        pass = pass && r1 == r2;
    }

    bool has_cycles = mckl::StopWatch::has_cycles();
    double c1 = has_cycles ? std::numeric_limits<double>::max() : 0.0;
    double c2 = has_cycles ? std::numeric_limits<double>::max() : 0.0;
#if MCKL_HAS_MKL
    double c3 = has_cycles ? std::numeric_limits<double>::max() : 0.0;
#endif
    for (std::size_t k = 0; k != 10; ++k) {
        std::size_t num = 0;
        mckl::StopWatch watch1;
        mckl::StopWatch watch2;
#if MCKL_HAS_MKL
        mckl::StopWatch watch3;
#endif
        for (std::size_t i = 0; i != M; ++i) {
            std::size_t K = rsize(rng);
            num += K * 2;
            r1.resize(K, 2);
            r2.resize(K, 2);
#if MCKL_HAS_MKL
            r3.resize(K, 2);
#endif

            watch1.start();
            RealType *p = r1.data();
            for (std::size_t j = 0; j != K; ++j, p += 2)
                dist(rng, p);
            watch1.stop();

            watch2.start();
            mckl::rand(rng, dist, K, r2.data());
            watch2.stop();

#if MCKL_HAS_MKL
            watch3.start();
            mckl::rand(rng_mkl, dist, K, r3.data());
            watch3.stop();
#endif

            pass = pass && r1 != r2;
#if MCKL_HAS_MKL
            pass = pass && r1 != r3;
#endif
        }
        if (has_cycles) {
            c1 = std::min(c1, 1.0 * watch1.cycles() / num);
            c2 = std::min(c2, 1.0 * watch2.cycles() / num);
#if MCKL_HAS_MKL
            c3 = std::min(c3, 1.0 * watch3.cycles() / num);
#endif
        } else {
            c1 = std::max(c1, num / watch1.seconds() * 1e-6);
            c2 = std::max(c2, num / watch2.seconds() * 1e-6);
#if MCKL_HAS_MKL
            c3 = std::max(c3, num / watch3.seconds() * 1e-6);
#endif
        }
    }

    std::stringstream ss;
    ss << "Dirichlet" << '<' << random_typename<RealType>() << ">(";
    const RealType *a = dist.alpha();
    ss << '{' << a[0] << ", " << a[1] << "})";

    std::cout << std::setw(40) << std::left << ss.str();
    std::cout << std::setw(12) << std::right << c1;
    std::cout << std::setw(12) << std::right << c2;
#if MCKL_HAS_MKL
    std::cout << std::setw(12) << std::right << c3;
#endif
    std::cout << std::setw(15) << std::right << random_pass(pass);
    std::cout << std::endl;

    std::ofstream txt("random_dirichlet.txt", std::ios_base::app);
    for (std::size_t i = 0; i != r1.nrow(); ++i) {
        txt << r1(i, 0) << '\t' << r1(i, 1) << '\t';
        txt << '"' << ss.str() << '"' << '\t' << "Single" << '\n';
    }
    for (std::size_t i = 0; i != r2.nrow(); ++i) {
        txt << r2(i, 0) << '\t' << r2(i, 1) << '\t';
        txt << '"' << ss.str() << '"' << '\t' << "Batch" << '\n';
    }
#if MCKL_HAS_MKL
    for (std::size_t i = 0; i != r3.nrow(); ++i) {
        txt << r3(i, 0) << '\t' << r3(i, 1) << '\t';
        txt << '"' << ss.str() << '"' << '\t' << "VSL" << '\n';
    }
#endif
    txt.close();
}

template <typename RealType>
inline void random_dirichlet(std::size_t N, std::size_t M)
{
    std::array<RealType, 2> alpha = {{1, 2}};

    random_dirichlet(N, M, alpha.data(), false);
    random_dirichlet(N, M, alpha.data(), true);
}

inline void random_dirichlet(std::size_t N, std::size_t M)
{
    std::ofstream txt("random_dirichlet.txt");
    txt << "V1\tV2\tDistribution\tImplementation\n";
    txt.close();

    constexpr std::size_t lwid = 40 + 12 * (2 + MCKL_HAS_MKL) + 15;

    std::cout << std::string(lwid, '=') << std::endl;
    std::cout << std::setw(40) << std::left << "Distribution";
    if (mckl::StopWatch::has_cycles()) {
        std::cout << std::setw(12) << std::right << "cpE (S)";
        std::cout << std::setw(12) << std::right << "cpE (B)";
#if MCKL_HAS_MKL
        std::cout << std::setw(12) << std::right << "cpE (V)";
#endif
    } else {
        std::cout << std::setw(12) << std::right << "ME/s (S)";
        std::cout << std::setw(12) << std::right << "ME/s (B)";
#if MCKL_HAS_MKL
        std::cout << std::setw(12) << std::right << "ME/s (V)";
#endif
    }
    std::cout << std::setw(15) << std::right << "Deterministics";
    std::cout << std::endl;
    std::cout << std::string(lwid, '-') << std::endl;
    random_dirichlet<float>(N, M);
    std::cout << std::string(lwid, '-') << std::endl;
    random_dirichlet<double>(N, M);
    std::cout << std::string(lwid, '-') << std::endl;
}

#endif // MCKL_EXAMPLE_RANDOM_DIRICHLET_HPP
