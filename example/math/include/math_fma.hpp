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

#define MCKL_EXAMPLE_DEFINE_MATH_FMA_PARAMS(func, ffunc, par, pa, pb, pc)     \
    inline void math_##func##_##par(                                          \
        std::size_t N, std::size_t M, mckl::Vector<MathPerf> &perf)           \
    {                                                                         \
        mckl::UniformRealDistribution<float> unifs(-1e4, 1e4);                \
        mckl::UniformRealDistribution<double> unifd(-1e4, 1e4);               \
        mckl::UniformIntDistribution<std::size_t> rsize(N / 2, N);            \
        mckl::RNG_64 rng;                                                     \
                                                                              \
        mckl::Vector<float> as(N);                                            \
        mckl::Vector<float> bs(N);                                            \
        mckl::Vector<float> cs(N);                                            \
        mckl::Vector<double> ad(N);                                           \
        mckl::Vector<double> bd(N);                                           \
        mckl::Vector<double> cd(N);                                           \
        mckl::Vector<float> rs1(N);                                           \
        mckl::Vector<float> rs2(N);                                           \
        mckl::Vector<float> rs3(N);                                           \
        mckl::Vector<double> rd1(N);                                          \
        mckl::Vector<double> rd2(N);                                          \
        mckl::Vector<double> rd3(N);                                          \
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
                watch1.start();                                               \
                mckl::func<float>(K, as pa, bs pb, cs pc, rs1.data());        \
                watch1.stop();                                                \
                                                                              \
                watch2.start();                                               \
                mckl::func(K, as pa, bs pb, cs pc, rs2.data());               \
                watch2.stop();                                                \
                                                                              \
                watch3.start();                                               \
                mckl::func<double>(K, ad pa, bd pb, cd pc, rd1.data());       \
                watch3.stop();                                                \
                                                                              \
                watch4.start();                                               \
                mckl::func(K, ad pa, bd pb, cd pc, rd2.data());               \
                watch4.stop();                                                \
                                                                              \
                mckl::ffunc<float>(K, as pa, bs pb, cs pc, rs3.data());       \
                math_error(K, rs1.data(), rs3.data(), e1);                    \
                math_error(K, rs2.data(), rs3.data(), e2);                    \
                                                                              \
                mckl::ffunc<double>(K, ad pa, bd pb, cd pc, rd3.data());      \
                math_error(K, rd1.data(), rd3.data(), e3);                    \
                math_error(K, rd2.data(), rd3.data(), e4);                    \
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
        result.name = #func "(" #par ")";                                     \
        result.e1 = e1;                                                       \
        result.e2 = e2;                                                       \
        result.e3 = e3;                                                       \
        result.e4 = e4;                                                       \
        result.c1 = c1;                                                       \
        result.c2 = c2;                                                       \
        result.c3 = c3;                                                       \
        result.c4 = c4;                                                       \
                                                                              \
        perf.push_back(result);                                               \
    }

#define MCKL_EXAMPLE_DEFINE_MATH_FMA(func, ffunc)                             \
    MCKL_EXAMPLE_DEFINE_MATH_FMA_PARAMS(                                      \
        func, ffunc, vvv, .data(), .data(), .data())                          \
    MCKL_EXAMPLE_DEFINE_MATH_FMA_PARAMS(                                      \
        func, ffunc, vvs, .data(), .data(), [0])                              \
    MCKL_EXAMPLE_DEFINE_MATH_FMA_PARAMS(                                      \
        func, ffunc, vsv, .data(), [0], .data())                              \
    MCKL_EXAMPLE_DEFINE_MATH_FMA_PARAMS(                                      \
        func, ffunc, svv, [0], .data(), .data())                              \
    MCKL_EXAMPLE_DEFINE_MATH_FMA_PARAMS(func, ffunc, ssv, [0], [0], .data())  \
    MCKL_EXAMPLE_DEFINE_MATH_FMA_PARAMS(func, ffunc, svs, [0], .data(), [0])  \
    MCKL_EXAMPLE_DEFINE_MATH_FMA_PARAMS(func, ffunc, vss, .data(), [0], [0])

#define MCKL_EXAMPLE_MATH_FMA(func)                                           \
    math_##func##_vvv(N, M, perf);                                            \
    math_##func##_vvs(N, M, perf);                                            \
    math_##func##_vsv(N, M, perf);                                            \
    math_##func##_svv(N, M, perf);                                            \
    math_##func##_ssv(N, M, perf);                                            \
    math_##func##_svs(N, M, perf);                                            \
    math_##func##_vss(N, M, perf);

MCKL_EXAMPLE_DEFINE_MATH_FMA(muladd, fmadd)
MCKL_EXAMPLE_DEFINE_MATH_FMA(mulsub, fmsub)
MCKL_EXAMPLE_DEFINE_MATH_FMA(nmuladd, fnmadd)
MCKL_EXAMPLE_DEFINE_MATH_FMA(nmulsub, fnmsub)
MCKL_EXAMPLE_DEFINE_MATH_FMA(fmadd, fmadd)
MCKL_EXAMPLE_DEFINE_MATH_FMA(fmsub, fmsub)
MCKL_EXAMPLE_DEFINE_MATH_FMA(fnmadd, fnmadd)
MCKL_EXAMPLE_DEFINE_MATH_FMA(fnmsub, fnmsub)

inline void math_fma(std::size_t N, std::size_t M)
{
    mckl::Vector<MathPerf> perf;

    MCKL_EXAMPLE_MATH_FMA(muladd)
    MCKL_EXAMPLE_MATH_FMA(mulsub)
    MCKL_EXAMPLE_MATH_FMA(nmuladd)
    MCKL_EXAMPLE_MATH_FMA(nmulsub)
    MCKL_EXAMPLE_MATH_FMA(fmadd)
    MCKL_EXAMPLE_MATH_FMA(fmsub)
    MCKL_EXAMPLE_MATH_FMA(fnmadd)
    MCKL_EXAMPLE_MATH_FMA(fnmsub)

    const int nwid = 15;
    const int twid = 10;
    const int ewid = 15;
    const std::size_t lwid = nwid + twid * 4 + ewid * 4;

    std::cout << std::string(lwid, '=') << std::endl;

    std::cout << std::setw(nwid) << std::left << "Function";
    if (mckl::StopWatch::has_cycles()) {
        std::cout << std::setw(twid) << std::right << "cpE (S)";
        std::cout << std::setw(twid) << std::right << "cpE (SV)";
        std::cout << std::setw(twid) << std::right << "cpE (D)";
        std::cout << std::setw(twid) << std::right << "cpE (DV)";
    } else {
        std::cout << std::setw(twid) << std::right << "ME/s (S)";
        std::cout << std::setw(twid) << std::right << "ME/s (SV)";
        std::cout << std::setw(twid) << std::right << "ME/s (D)";
        std::cout << std::setw(twid) << std::right << "ME/s (DV)";
    }
    std::cout << std::setw(ewid) << std::right << math_error() + " (S)";
    std::cout << std::setw(ewid) << std::right << math_error() + " (SV)";
    std::cout << std::setw(ewid) << std::right << math_error() + " (D)";
    std::cout << std::setw(ewid) << std::right << math_error() + " (DV)";
    std::cout << std::endl;

    std::cout << std::string(lwid, '-') << std::endl;

    for (std::size_t i = 0; i != perf.size(); ++i) {
        std::cout << std::fixed << std::setprecision(2);
        std::cout << std::setw(nwid) << std::left << perf[i].name;
        std::cout << std::setw(twid) << std::right << perf[i].c1;
        std::cout << std::setw(twid) << std::right << perf[i].c2;
        std::cout << std::setw(twid) << std::right << perf[i].c3;
        std::cout << std::setw(twid) << std::right << perf[i].c4;
        std::cout.unsetf(std::ios_base::floatfield);
        std::cout << std::setprecision(2);
        std::cout << std::setw(ewid) << std::right << perf[i].e1;
        std::cout << std::setw(ewid) << std::right << perf[i].e2;
        std::cout << std::setw(ewid) << std::right << perf[i].e3;
        std::cout << std::setw(ewid) << std::right << perf[i].e4;
        std::cout << std::endl;
    }

    std::cout << std::string(lwid, '-') << std::endl;
}

#endif // MCKL_EXAMPLE_MATH_FMA_HPP
