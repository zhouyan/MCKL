//============================================================================
// MCKL/example/math/include/math_vmf.hpp
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

#ifndef MCKL_EXAMPLE_MATH_VMF_HPP
#define MCKL_EXAMPLE_MATH_VMF_HPP

#include "math_common.hpp"

#define MCKL_EXAMPLE_MATH_VMF_RUN_A1R1(func)                                  \
    mckl::func<float>(K, as.data(), rs1.data());                              \
    watch1.start();                                                           \
    mckl::func<float>(K, as.data(), rs1.data());                              \
    watch1.stop();                                                            \
                                                                              \
    mckl::func(K, as.data(), rs2.data());                                     \
    watch2.start();                                                           \
    mckl::func(K, as.data(), rs2.data());                                     \
    watch2.stop();                                                            \
                                                                              \
    mckl::func<double>(K, ad.data(), rd1.data());                             \
    watch3.start();                                                           \
    mckl::func<double>(K, ad.data(), rd1.data());                             \
    watch3.stop();                                                            \
                                                                              \
    mckl::func(K, ad.data(), rd2.data());                                     \
    watch4.start();                                                           \
    mckl::func(K, ad.data(), rd2.data());                                     \
    watch4.stop();

#define MCKL_EXAMPLE_MATH_VMF_RUN_A2R1(func)                                  \
    mckl::func<float>(K, as.data(), bs.data(), rs1.data());                   \
    watch1.start();                                                           \
    mckl::func<float>(K, as.data(), bs.data(), rs1.data());                   \
    watch1.stop();                                                            \
                                                                              \
    mckl::func(K, as.data(), bs.data(), rs2.data());                          \
    watch2.start();                                                           \
    mckl::func(K, as.data(), bs.data(), rs2.data());                          \
    watch2.stop();                                                            \
                                                                              \
    mckl::func<double>(K, ad.data(), bd.data(), rd1.data());                  \
    watch3.start();                                                           \
    mckl::func<double>(K, ad.data(), bd.data(), rd1.data());                  \
    watch3.stop();                                                            \
                                                                              \
    mckl::func(K, ad.data(), bd.data(), rd2.data());                          \
    watch4.start();                                                           \
    mckl::func(K, ad.data(), bd.data(), rd2.data());                          \
    watch4.stop();

#define MCKL_EXAMPLE_MATH_VMF_RUN_A3R1(func)                                  \
    mckl::func<float>(K, as.data(), bs.data(), cs.data(), rs1.data());        \
    watch1.start();                                                           \
    mckl::func<float>(K, as.data(), bs.data(), cs.data(), rs1.data());        \
    watch1.stop();                                                            \
                                                                              \
    mckl::func(K, as.data(), bs.data(), cs.data(), rs2.data());               \
    watch2.start();                                                           \
    mckl::func(K, as.data(), bs.data(), cs.data(), rs2.data());               \
    watch2.stop();                                                            \
                                                                              \
    mckl::func<double>(K, ad.data(), bd.data(), cd.data(), rd1.data());       \
    watch3.start();                                                           \
    mckl::func<double>(K, ad.data(), bd.data(), cd.data(), rd1.data());       \
    watch3.stop();                                                            \
                                                                              \
    mckl::func(K, ad.data(), bd.data(), cd.data(), rd2.data());               \
    watch4.start();                                                           \
    mckl::func(K, ad.data(), bd.data(), cd.data(), rd2.data());               \
    watch4.stop();

#define MCKL_EXAMPLE_MATH_VMF_RUN_A1R2(func)                                  \
    mckl::func<float>(K, as.data(), rs1.data(), rs3.data());                  \
    watch1.start();                                                           \
    mckl::func<float>(K, as.data(), rs1.data(), rs3.data());                  \
    watch1.stop();                                                            \
                                                                              \
    mckl::func(K, as.data(), rs2.data(), rs4.data());                         \
    watch2.start();                                                           \
    mckl::func(K, as.data(), rs2.data(), rs4.data());                         \
    watch2.stop();                                                            \
                                                                              \
    mckl::func<double>(K, ad.data(), rd1.data(), rd3.data());                 \
    watch3.start();                                                           \
    mckl::func<double>(K, ad.data(), rd1.data(), rd3.data());                 \
    watch3.stop();                                                            \
                                                                              \
    mckl::func(K, ad.data(), rd2.data(), rd4.data());                         \
    watch4.start();                                                           \
    mckl::func(K, ad.data(), rd2.data(), rd4.data());                         \
    watch4.stop();

#define MCKL_EXAMPLE_DEFINE_MATH_VMF(AR, func, ls, us, ld, ud)                \
    inline void math_vmf_##func(                                              \
        std::size_t N, std::size_t M, mckl::Vector<MathPerf> &perf)           \
    {                                                                         \
        mckl::UniformRealDistribution<float> unifs(ls, us);                   \
        mckl::UniformRealDistribution<double> unifd(ld, ud);                  \
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
        mckl::Vector<float> rs3(N);                                           \
        mckl::Vector<float> rs4(N);                                           \
        mckl::Vector<double> rd1(N);                                          \
        mckl::Vector<double> rd2(N);                                          \
        mckl::Vector<double> rd3(N);                                          \
        mckl::Vector<double> rd4(N);                                          \
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
        for (std::size_t k = 0; k != 10; ++k) {                               \
            std::size_t n = 0;                                                \
            mckl::StopWatch watch1;                                           \
            mckl::StopWatch watch2;                                           \
            mckl::StopWatch watch3;                                           \
            mckl::StopWatch watch4;                                           \
            for (std::size_t i = 0; i != M; ++i) {                            \
                std::size_t K = rsize(rng);                                   \
                n += K;                                                       \
                mckl::rand(rng, unifs, K, as.data());                         \
                mckl::rand(rng, unifs, K, bs.data());                         \
                mckl::rand(rng, unifs, K, cs.data());                         \
                mckl::rand(rng, unifd, K, ad.data());                         \
                mckl::rand(rng, unifd, K, bd.data());                         \
                mckl::rand(rng, unifd, K, cd.data());                         \
                MCKL_EXAMPLE_MATH_VMF_RUN_##AR(func);                         \
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
        result.name = #func;                                                  \
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

MCKL_EXAMPLE_DEFINE_MATH_VMF(A1R1, acos, -1.0f, 1.0f, -1.0, 1.0)
MCKL_EXAMPLE_DEFINE_MATH_VMF(A1R1, asin, -1.0f, 1.0f, -1.0, 1.0)
MCKL_EXAMPLE_DEFINE_MATH_VMF(A1R1, atan, -5.0f, 5.0f, -5.0, 5.0)
MCKL_EXAMPLE_DEFINE_MATH_VMF(A2R1, atan2, -1e3f, 1e3f, -1e3, 1e3)
MCKL_EXAMPLE_DEFINE_MATH_VMF(A1R1, cos, -1e4f, 1e4f, -1e4, 1e4)
MCKL_EXAMPLE_DEFINE_MATH_VMF(A1R1, sin, -1e4f, 1e4f, -1e4, 1e4)
MCKL_EXAMPLE_DEFINE_MATH_VMF(A1R2, sincos, -1e4f, 1e4f, -1e4, 1e4)
MCKL_EXAMPLE_DEFINE_MATH_VMF(A1R1, tan, -1e3f, 1e3f, -1e3, 1e3)

MCKL_EXAMPLE_DEFINE_MATH_VMF(A1R1, acosh, 4.0f, 1e19f, 1.001, 1e154)
MCKL_EXAMPLE_DEFINE_MATH_VMF(A1R1, asinh, 4.0f, 1e19f, 0.002, 1e154)
MCKL_EXAMPLE_DEFINE_MATH_VMF(A1R1, atanh, 0.77f, 0.99999f, 0.005, 0.99999)
MCKL_EXAMPLE_DEFINE_MATH_VMF(A1R1, cosh, -87.0f, 87.0f, -675.0, 675.0)
MCKL_EXAMPLE_DEFINE_MATH_VMF(A1R1, sinh, -87.0f, 87.0f, -675.0, 675.0)
MCKL_EXAMPLE_DEFINE_MATH_VMF(A1R1, tanh, -9.0f, 9.0f, -19.0, 19.0)

MCKL_EXAMPLE_DEFINE_MATH_VMF(A1R1, cbrt, -1e4f, 1e4f, -1e4, 1e4)
MCKL_EXAMPLE_DEFINE_MATH_VMF(A2R1, hypot, -1e4f, 1e4f, -1e4, 1e4)
MCKL_EXAMPLE_DEFINE_MATH_VMF(A1R1, inv, 1e-3f, 1e4f, 1e-3, 1e4)
MCKL_EXAMPLE_DEFINE_MATH_VMF(A1R1, invcbrt, 0.1f, 1e4f, 0.1, 1e4)
MCKL_EXAMPLE_DEFINE_MATH_VMF(A1R1, invsqrt, 0.1f, 1e4f, 0.1, 1e4)
MCKL_EXAMPLE_DEFINE_MATH_VMF(A2R1, pow, 0.1f, 10.0f, 5.0, 100.0)
MCKL_EXAMPLE_DEFINE_MATH_VMF(A1R1, pow2o3, 0.1f, 1e4f, 0.1, 1e4)
MCKL_EXAMPLE_DEFINE_MATH_VMF(A1R1, pow3o2, 0.1f, 1e4f, 0.1, 1e4)
MCKL_EXAMPLE_DEFINE_MATH_VMF(A1R1, sqrt, 0.0f, 1e4f, 0.0, 1e4)

MCKL_EXAMPLE_DEFINE_MATH_VMF(A1R1, exp, -87.0f, 87.0f, -707.0, 707.0)
MCKL_EXAMPLE_DEFINE_MATH_VMF(A1R1, expm1, -70.0f, 70.0f, -500.0, 500.0)
MCKL_EXAMPLE_DEFINE_MATH_VMF(A1R1, log, 0.1f, 1e4, 0.1, 1e4)
MCKL_EXAMPLE_DEFINE_MATH_VMF(A1R1, log10, 0.1f, 1e4, 0.1, 1e4)
MCKL_EXAMPLE_DEFINE_MATH_VMF(A1R1, log1p, 1.0f, 999.0f, 1.0, 999.0)

MCKL_EXAMPLE_DEFINE_MATH_VMF(A1R1, abs, -1e4f, 1e4f, -1e4, 1e4)
MCKL_EXAMPLE_DEFINE_MATH_VMF(A2R1, add, -1e4f, 1e4f, -1e4, 1e4)
MCKL_EXAMPLE_DEFINE_MATH_VMF(A2R1, div, 1e-4f, 1e4f, 1e-4, 1e4)
MCKL_EXAMPLE_DEFINE_MATH_VMF(A2R1, mul, -1e4f, 1e4f, -1e4, 1e4)
MCKL_EXAMPLE_DEFINE_MATH_VMF(A1R1, sqr, -1e4f, 1e4f, -1e4, 1e4)
MCKL_EXAMPLE_DEFINE_MATH_VMF(A2R1, sub, -1e4f, 1e4f, -1e4, 1e4)
MCKL_EXAMPLE_DEFINE_MATH_VMF(A3R1, fma, -1e4f, 1e4f, -1e4, 1e4)

MCKL_EXAMPLE_DEFINE_MATH_VMF(A1R1, ceil, -1e4f, 1e4f, -1e4, 1e4)
MCKL_EXAMPLE_DEFINE_MATH_VMF(A1R1, floor, -1e4f, 1e4f, -1e4, 1e4)
MCKL_EXAMPLE_DEFINE_MATH_VMF(A1R2, modf, -1e4f, 1e4f, -1e4, 1e4)
MCKL_EXAMPLE_DEFINE_MATH_VMF(A1R1, nearbyint, -1e4f, 1e4f, -1e4, 1e4)
MCKL_EXAMPLE_DEFINE_MATH_VMF(A1R1, rint, -1e4f, 1e4f, -1e4, 1e4)
MCKL_EXAMPLE_DEFINE_MATH_VMF(A1R1, round, -1e4f, 1e4f, -1e4, 1e4)
MCKL_EXAMPLE_DEFINE_MATH_VMF(A1R1, trunc, -1e4f, 1e4f, -1e4, 1e4)

MCKL_EXAMPLE_DEFINE_MATH_VMF(A1R1, cdfnorm, -8.0f, 5.0f, -32.0, 8.0)
MCKL_EXAMPLE_DEFINE_MATH_VMF(A1R1, cdfnorminv, 1e-5f, 0.99999f, 1e-5, 0.99999)
MCKL_EXAMPLE_DEFINE_MATH_VMF(A1R1, erf, 1.0f, 4.0f, 0.5, 5.9)
MCKL_EXAMPLE_DEFINE_MATH_VMF(A1R1, erfc, -3.0f, 9.0f, -5.0, 25.0)
MCKL_EXAMPLE_DEFINE_MATH_VMF(
    A1R1, erfinv, -0.99999f, 0.99999f, -0.99999, 0.99999)
MCKL_EXAMPLE_DEFINE_MATH_VMF(A1R1, erfcinv, 1e-5f, 1.99999f, 1e-5, 1.99999)
MCKL_EXAMPLE_DEFINE_MATH_VMF(A1R1, lgamma, 1e-5f, 1e3f, 1e-5, 1e3)
MCKL_EXAMPLE_DEFINE_MATH_VMF(A1R1, tgamma, 1e-4f, 35.0f, 1e-4, 171.0)

inline void math_vmf(std::size_t N, std::size_t M)
{
    mckl::Vector<MathPerf> perf;

    math_vmf_add(N, M, perf);
    math_vmf_sub(N, M, perf);
    math_vmf_sqr(N, M, perf);
    math_vmf_mul(N, M, perf);
    math_vmf_fma(N, M, perf);
    math_vmf_abs(N, M, perf);

    math_vmf_inv(N, M, perf);
    math_vmf_div(N, M, perf);
    math_vmf_sqrt(N, M, perf);
    math_vmf_invsqrt(N, M, perf);
    math_vmf_cbrt(N, M, perf);
    math_vmf_invcbrt(N, M, perf);
    math_vmf_pow2o3(N, M, perf);
    math_vmf_pow3o2(N, M, perf);
    math_vmf_pow(N, M, perf);
    math_vmf_hypot(N, M, perf);

    math_vmf_exp(N, M, perf);
    math_vmf_expm1(N, M, perf);
    math_vmf_log(N, M, perf);
    math_vmf_log10(N, M, perf);
    math_vmf_log1p(N, M, perf);

    math_vmf_cos(N, M, perf);
    math_vmf_sin(N, M, perf);
    math_vmf_sincos(N, M, perf);
    math_vmf_tan(N, M, perf);
    math_vmf_acos(N, M, perf);
    math_vmf_asin(N, M, perf);
    math_vmf_atan(N, M, perf);
    math_vmf_atan2(N, M, perf);

    math_vmf_cosh(N, M, perf);
    math_vmf_sinh(N, M, perf);
    math_vmf_tanh(N, M, perf);
    math_vmf_acosh(N, M, perf);
    math_vmf_asinh(N, M, perf);
    math_vmf_atanh(N, M, perf);

    math_vmf_erf(N, M, perf);
    math_vmf_erfc(N, M, perf);
    math_vmf_cdfnorm(N, M, perf);
    math_vmf_erfinv(N, M, perf);
    math_vmf_erfcinv(N, M, perf);
    math_vmf_cdfnorminv(N, M, perf);
    math_vmf_lgamma(N, M, perf);
    math_vmf_tgamma(N, M, perf);

    math_vmf_floor(N, M, perf);
    math_vmf_ceil(N, M, perf);
    math_vmf_trunc(N, M, perf);
    math_vmf_round(N, M, perf);
    math_vmf_nearbyint(N, M, perf);
    math_vmf_rint(N, M, perf);

    math_summary_sv(perf);
}

#endif // MCKL_EXAMPLE_MATH_VMF_HPP
