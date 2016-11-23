//============================================================================
// MCKL/example/math/include/math_asm.hpp
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

#ifndef MCKL_EXAMPLE_MATH_ASM_HPP
#define MCKL_EXAMPLE_MATH_ASM_HPP

#include "math_common.hpp"

#define MCKL_EXAMPLE_MATH_ASM_RUN_A1R1(func)                                  \
    mckl_##func##_vv_pd(K, a.data(), r1.data());                              \
    watch1.start();                                                           \
    mckl_##func##_vv_pd(K, a.data(), r1.data());                              \
    watch1.stop();                                                            \
                                                                              \
    mckl::func(K, a.data(), r2.data());                                       \
    watch2.start();                                                           \
    mckl::func(K, a.data(), r2.data());                                       \
    watch2.stop();

#define MCKL_EXAMPLE_DEFINE_MATH_ASM(AR, func, lb, ub)                        \
    inline void math_asm_##func(                                              \
        std::size_t N, std::size_t M, mckl::Vector<MathPerf> &perf)           \
    {                                                                         \
        mckl::UniformRealDistribution<double> unif(lb, ub);                   \
        mckl::UniformIntDistribution<std::size_t> rsize(N / 2, N);            \
        mckl::RNG rng;                                                        \
                                                                              \
        mckl::Vector<double> a(N);                                            \
        mckl::Vector<double> b(N);                                            \
        mckl::Vector<double> c(N);                                            \
        mckl::Vector<double> r1(N);                                           \
        mckl::Vector<double> r2(N);                                           \
        mckl::Vector<double> r3(N);                                           \
        mckl::Vector<double> r4(N);                                           \
                                                                              \
        bool has_cycles = mckl::StopWatch::has_cycles();                      \
                                                                              \
        double e3 = 0;                                                        \
        double e4 = 0;                                                        \
        double c1 = has_cycles ? std::numeric_limits<double>::max() : 0.0;    \
        double c2 = has_cycles ? std::numeric_limits<double>::max() : 0.0;    \
        for (std::size_t k = 0; k != 10; ++k) {                               \
            std::size_t n = 0;                                                \
            mckl::StopWatch watch1;                                           \
            mckl::StopWatch watch2;                                           \
            for (std::size_t i = 0; i != M; ++i) {                            \
                std::size_t K = rsize(rng);                                   \
                n += K;                                                       \
                mckl::rand(rng, unif, K, a.data());                           \
                mckl::rand(rng, unif, K, b.data());                           \
                mckl::rand(rng, unif, K, c.data());                           \
                MCKL_EXAMPLE_MATH_ASM_RUN_##AR(func);                         \
                math_error(K, r1.data(), r2.data(), e3, e4);                  \
            }                                                                 \
            if (has_cycles) {                                                 \
                c1 = std::min(c1, 1.0 * watch1.cycles() / n);                 \
                c2 = std::min(c2, 1.0 * watch2.cycles() / n);                 \
            } else {                                                          \
                c1 = std::max(c1, n / watch1.seconds() * 1e-6);               \
                c2 = std::max(c2, n / watch2.seconds() * 1e-6);               \
            }                                                                 \
        }                                                                     \
                                                                              \
        MathPerf result;                                                      \
        result.name = #func;                                                  \
        result.e3 = e3;                                                       \
        result.e4 = e4;                                                       \
        result.c1 = c1;                                                       \
        result.c2 = c2;                                                       \
                                                                              \
        perf.push_back(result);                                               \
    }

MCKL_EXAMPLE_DEFINE_MATH_ASM(A1R1, exp, -707.0, 707.0)
MCKL_EXAMPLE_DEFINE_MATH_ASM(A1R1, exp2, -1e3, 1e3)
MCKL_EXAMPLE_DEFINE_MATH_ASM(A1R1, expm1, -500.0, 500.0)

inline void math_asm(std::size_t N, std::size_t M)
{
    mckl::Vector<MathPerf> perf;

    math_asm_exp(N, M, perf);
    math_asm_exp2(N, M, perf);
    math_asm_expm1(N, M, perf);

    math_summary_av(perf);
}

#endif // MCKL_EXAMPLE_MATH_ASM_HPP
