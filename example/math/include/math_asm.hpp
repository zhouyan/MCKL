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

#ifdef __x86_64__
#define MCKL_USE_RDPMC 1
#endif

#include "math_common.hpp"

#define MCKL_EXAMPLE_DEFINE_MATH_ASM(func, wl, wu, lb, ub)                    \
    inline void math_asm_##func(                                              \
        std::size_t N, std::size_t M, mckl::Vector<MathPerf> &perf)           \
    {                                                                         \
        mckl::UniformRealDistribution<double> unif(wl, wu);                   \
        mckl::UniformIntDistribution<std::size_t> rsize(N / 2, N);            \
        mckl::RNG rng;                                                        \
                                                                              \
        mckl::Vector<double> ad(N);                                           \
        mckl::Vector<double> rd(N);                                           \
        mckl::Vector<long double> al(N);                                      \
        mckl::Vector<long double> rl(N);                                      \
                                                                              \
        bool has_cycles = mckl::StopWatch::has_cycles();                      \
                                                                              \
        double e1 = 0;                                                        \
        double e2 = 0;                                                        \
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
                mckl::rand(rng, unif, K, ad.data());                          \
                ad[0] = wl;                                                   \
                ad[0] = ub;                                                   \
                std::copy_n(ad.data(), K, al.data());                         \
                mckl::func(K, al.data(), rl.data());                          \
                                                                              \
                mckl_##func##_vv_pd(K, ad.data(), rd.data());                 \
                watch1.start();                                               \
                mckl_##func##_vv_pd(K, ad.data(), rd.data());                 \
                watch1.stop();                                                \
                math_error(K, rd.data(), rl.data(), e1, e2);                  \
                                                                              \
                mckl::func(K, ad.data(), rd.data());                          \
                watch2.start();                                               \
                mckl::func(K, ad.data(), rd.data());                          \
                watch2.stop();                                                \
                math_error(K, rd.data(), rl.data(), e3, e4);                  \
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
        result.e3 = e2;                                                       \
        result.e4 = e4;                                                       \
        result.c1 = c1;                                                       \
        result.c2 = c2;                                                       \
        result.vd[0] = lb;                                                    \
        result.vd[1] = ub;                                                    \
        const double v[] = {lb, ub,                                           \
            std::nextafter(lb, -mckl::const_inf<double>()),                   \
            std::nextafter(ub, mckl::const_inf<double>()), -0.0, 0.0,         \
            -mckl::const_inf<double>(), mckl::const_inf<double>(),            \
            mckl::const_nan<double>()};                                       \
        mckl_##func##_vv_pd(sizeof(v) / sizeof(double), v, result.vd + 2);    \
                                                                              \
        perf.push_back(result);                                               \
    }

MCKL_EXAMPLE_DEFINE_MATH_ASM(exp, -707, 707, -708.4, 709.8)
MCKL_EXAMPLE_DEFINE_MATH_ASM(exp2, -1022, 1022, -1022, 1022)
MCKL_EXAMPLE_DEFINE_MATH_ASM(expm1, -500, 500, -708.4, 709.8)

inline void math_asm(std::size_t N, std::size_t M)
{
    mckl::Vector<MathPerf> perf;

    math_asm_exp(N, M, perf);
    math_asm_exp2(N, M, perf);
    math_asm_expm1(N, M, perf);

    const int nwid = 10;
    const std::size_t lwid = nwid * 16;

    std::cout << std::string(lwid, '=') << std::endl;

    std::cout << std::setw(nwid) << std::left << "Function";
    if (mckl::StopWatch::has_cycles()) {
        std::cout << std::setw(nwid) << std::right << "cpE (A)";
        std::cout << std::setw(nwid) << std::right << "cpE (V)";
    } else {
        std::cout << std::setw(nwid) << std::right << "ME/s (A)";
        std::cout << std::setw(nwid) << std::right << "ME/s (V)";
    }
#if MCKL_HAS_BOOST
    std::cout << std::setw(nwid) << std::right << "ULP (A)";
    std::cout << std::setw(nwid) << std::right << "ULP (V)";
#else
    std::cout << std::setw(nwid) << std::right << "Err (A)";
    std::cout << std::setw(nwid) << std::right << "Err (A)";
#endif
    std::cout << std::setw(nwid) << std::right << "LB";
    std::cout << std::setw(nwid) << std::right << "UB";
    std::cout << std::setw(nwid) << std::right << "a = LB";
    std::cout << std::setw(nwid) << std::right << "a = UB";
    std::cout << std::setw(nwid) << std::right << "a < LB";
    std::cout << std::setw(nwid) << std::right << "a > UB";
    std::cout << std::setw(nwid) << std::right << "a = -0.0";
    std::cout << std::setw(nwid) << std::right << "a = 0.0";
    std::cout << std::setw(nwid) << std::right << "a = -Inf";
    std::cout << std::setw(nwid) << std::right << "a = Inf";
    std::cout << std::setw(nwid) << std::right << "a = NaN";
    std::cout << std::endl;

    std::cout << std::string(lwid, '-') << std::endl;

    for (std::size_t i = 0; i != perf.size(); ++i) {
        std::cout << std::fixed << std::setprecision(2);
        std::cout << std::setw(nwid) << std::left << perf[i].name;
        std::cout << std::setw(nwid) << std::right << perf[i].c1;
        std::cout << std::setw(nwid) << std::right << perf[i].c2;
        std::cout.unsetf(std::ios_base::floatfield);
        std::cout << std::setprecision(2);
        std::cout << std::setw(nwid) << std::right << perf[i].e3;
        std::cout << std::setw(nwid) << std::right << perf[i].e4;
        std::cout << std::setprecision(6);
        std::cout << std::setw(nwid) << std::right << perf[i].vd[0];
        std::cout << std::setw(nwid) << std::right << perf[i].vd[1];
        for (int j = 2; j != 11; ++j) {
            double v = std::abs(perf[i].vd[j]);
            bool positive = v > 0;
            bool denormal = v < std::numeric_limits<double>::min();
            std::stringstream ss;
            ss << std::setprecision(2) << (positive && denormal ? '*' : ' ')
               << perf[i].vd[j];
            std::cout << std::setw(nwid) << std::right << ss.str();
        }
        std::cout << std::endl;
    }

    std::cout << std::string(lwid, '-') << std::endl;
}

#endif // MCKL_EXAMPLE_MATH_ASM_HPP
