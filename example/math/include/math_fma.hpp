//============================================================================
// MCKL/example/math/include/math_fma.hpp
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

#ifndef MCKL_EXAMPLE_MATH_FMA_HPP
#define MCKL_EXAMPLE_MATH_FMA_HPP

#include "math_common.hpp"

#define MCKL_EXAMPLE_DEFINE_MATH_FMA(par, pas, pbs, pcs, pad, pbd, pcd)       \
    inline void math_fma_##par(                                               \
        std::size_t N, std::size_t M, mckl::Vector<MathPerf> &perf)           \
    {                                                                         \
        mckl::UniformRealDistribution<float> unifs(-1e4f, 1e4f);              \
        mckl::UniformRealDistribution<double> unifd(-1e4, 1e4);               \
        mckl::UniformIntDistribution<std::size_t> rsize(N / 2, N);            \
        mckl::RNG rng;                                                        \
                                                                              \
        mckl::Vector<float> as(N);                                            \
        mckl::Vector<float> bs(N);                                            \
        mckl::Vector<float> cs(N);                                            \
        mckl::Vector<double> ad(N);                                           \
        mckl::Vector<double> bd(N);                                           \
        mckl::Vector<double> cd(N);                                           \
        mckl::Vector<float> rs1(N);                                           \
        mckl::Vector<float> rs2(N);                                           \
        mckl::Vector<double> rd1(N);                                          \
        mckl::Vector<double> rd2(N);                                          \
                                                                              \
        bool has_cycles = mckl::StopWatch::has_cycles();                      \
                                                                              \
        float e1 = 0;                                                         \
        float e2 = 0;                                                         \
        double e3 = 0;                                                        \
        double e4 = 0;                                                        \
        double c1 = has_cycles ? std::numeric_limits<double>::max() : 0.0;    \
        double c2 = has_cycles ? std::numeric_limits<double>::max() : 0.0;    \
        double c3 = has_cycles ? std::numeric_limits<double>::max() : 0.0;    \
        double c4 = has_cycles ? std::numeric_limits<double>::max() : 0.0;    \
                                                                              \
        for (std::size_t k = 0; k != 10; ++k) {                               \
            std::size_t n = 0;                                                \
            mckl::StopWatch watch1;                                           \
            mckl::StopWatch watch2;                                           \
            mckl::StopWatch watch3;                                           \
            mckl::StopWatch watch4;                                           \
            for (std::size_t i = 0; i != M; ++i) {                            \
                std::size_t K = rsize(rng);                                   \
                n += K;                                                       \
                                                                              \
                mckl::rand(rng, unifs, K, as.data());                         \
                mckl::rand(rng, unifs, K, bs.data());                         \
                mckl::rand(rng, unifs, K, cs.data());                         \
                mckl::rand(rng, unifd, K, ad.data());                         \
                mckl::rand(rng, unifd, K, bd.data());                         \
                mckl::rand(rng, unifd, K, cd.data());                         \
                                                                              \
                mckl::fma<float>(K, pas, pbs, pcs, rs1.data());               \
                watch1.start();                                               \
                mckl::fma<float>(K, pas, pbs, pcs, rs1.data());               \
                watch1.stop();                                                \
                                                                              \
                mckl::fma(K, pas, pbs, pcs, rs2.data());                      \
                watch2.start();                                               \
                mckl::fma(K, pas, pbs, pcs, rs2.data());                      \
                watch2.stop();                                                \
                                                                              \
                mckl::fma<double>(K, pad, pbd, pcd, rd1.data());              \
                watch3.start();                                               \
                mckl::fma<double>(K, pad, pbd, pcd, rd1.data());              \
                watch3.stop();                                                \
                                                                              \
                mckl::fma(K, pad, pbd, pcd, rd2.data());                      \
                watch4.start();                                               \
                mckl::fma(K, pad, pbd, pcd, rd2.data());                      \
                watch4.stop();                                                \
                                                                              \
                math_error(K, rs1.data(), rs2.data(), e1, e2);                \
                math_error(K, rd1.data(), rd2.data(), e3, e4);                \
            }                                                                 \
            if (has_cycles) {                                                 \
                c1 = std::min(c1, 1.0 * watch1.cycles() / n);                 \
                c2 = std::min(c2, 1.0 * watch2.cycles() / n);                 \
                c3 = std::min(c3, 1.0 * watch3.cycles() / n);                 \
                c4 = std::min(c4, 1.0 * watch4.cycles() / n);                 \
            } else {                                                          \
                c1 = std::max(c1, n / watch1.seconds() * 1e-6);               \
                c2 = std::max(c2, n / watch2.seconds() * 1e-6);               \
                c3 = std::max(c3, n / watch3.seconds() * 1e-6);               \
                c4 = std::max(c4, n / watch4.seconds() * 1e-6);               \
            }                                                                 \
        }                                                                     \
                                                                              \
        MathPerf result;                                                      \
        result.name = "fma(" #par ")";                                        \
        result.e1 = e1 / std::numeric_limits<float>::epsilon();               \
        result.e2 = e2 / std::numeric_limits<float>::epsilon();               \
        result.e3 = e3 / std::numeric_limits<double>::epsilon();              \
        result.e4 = e4 / std::numeric_limits<double>::epsilon();              \
        result.c1 = c1;                                                       \
        result.c2 = c2;                                                       \
        result.c3 = c3;                                                       \
        result.c4 = c4;                                                       \
                                                                              \
        perf.push_back(result);                                               \
    }

MCKL_EXAMPLE_DEFINE_MATH_FMA(
    vvv, as.data(), bs.data(), cs.data(), ad.data(), bd.data(), cd.data())

MCKL_EXAMPLE_DEFINE_MATH_FMA(
    vvs, as.data(), bs.data(), *cs.data(), ad.data(), bd.data(), *cd.data())

MCKL_EXAMPLE_DEFINE_MATH_FMA(
    vsv, as.data(), *bs.data(), cs.data(), ad.data(), *bd.data(), cd.data())

MCKL_EXAMPLE_DEFINE_MATH_FMA(
    svv, *as.data(), bs.data(), cs.data(), *ad.data(), bd.data(), cd.data())

MCKL_EXAMPLE_DEFINE_MATH_FMA(
    ssv, *as.data(), *bs.data(), cs.data(), *ad.data(), *bd.data(), cd.data())

MCKL_EXAMPLE_DEFINE_MATH_FMA(
    svs, *as.data(), bs.data(), *cs.data(), *ad.data(), bd.data(), *cd.data())

MCKL_EXAMPLE_DEFINE_MATH_FMA(
    vss, as.data(), *bs.data(), *cs.data(), ad.data(), *bd.data(), *cd.data())

inline void math_fma(std::size_t N, std::size_t M)
{
    mckl::Vector<MathPerf> perf;

    math_fma_vvv(N, M, perf);
    math_fma_vvs(N, M, perf);
    math_fma_vsv(N, M, perf);
    math_fma_svv(N, M, perf);
    math_fma_ssv(N, M, perf);
    math_fma_svs(N, M, perf);
    math_fma_vss(N, M, perf);

    math_summary_sv(perf);
}

#endif // MCKL_EXAMPLE_MATH_FMA_HPP
