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

#define MCKL_EXAMPLE_MATH_ASM_RUN_A1R1(func, vfunc)                           \
    mckl_##vfunc(K, a.data(), y1.data());                                     \
    watch1.start();                                                           \
    mckl_##vfunc(K, a.data(), y1.data());                                     \
    watch1.stop();                                                            \
                                                                              \
    mckl::func(K, a.data(), y2.data());                                       \
    watch2.start();                                                           \
    mckl::func(K, a.data(), y2.data());                                       \
    watch2.stop();                                                            \
                                                                              \
    std::copy_n(a.data(), K, al.data());                                      \
    mckl::func(K, al.data(), yl.data());                                      \
    math_error(K, y1.data(), yl.data(), e1);                                  \
    math_error(K, y2.data(), yl.data(), e2);

#define MCKL_EXAMPLE_MATH_ASM_RUN_A1R2(func, vfunc)                           \
    mckl_##vfunc(K, a.data(), y1.data(), z1.data());                          \
    watch1.start();                                                           \
    mckl_##vfunc(K, a.data(), y1.data(), z1.data());                          \
    watch1.stop();                                                            \
                                                                              \
    mckl::func(K, a.data(), y2.data(), z2.data());                            \
    watch2.start();                                                           \
    mckl::func(K, a.data(), y2.data(), z2.data());                            \
    watch2.stop();                                                            \
                                                                              \
    std::copy_n(a.data(), K, al.data());                                      \
    mckl::func(K, al.data(), yl.data(), zl.data());                           \
    math_error(K, y1.data(), yl.data(), e1);                                  \
    math_error(K, z1.data(), zl.data(), e1);                                  \
    math_error(K, y2.data(), yl.data(), e2);                                  \
    math_error(K, z2.data(), zl.data(), e2);

#define MCKL_EXAMPLE_DEFINE_MATH_ASM(AR, T, func, vfunc)                      \
    inline void math_asm_##vfunc(                                             \
        std::size_t N, std::size_t M, int nwid, int twid, T lb, T ub)         \
    {                                                                         \
        mckl::UniformRealDistribution<T> unif(lb, ub);                        \
        mckl::UniformIntDistribution<std::size_t> rsize(N / 2, N);            \
        mckl::RNG_64 rng;                                                     \
                                                                              \
        mckl::Vector<T> a(N);                                                 \
        mckl::Vector<T> y1(N);                                                \
        mckl::Vector<T> y2(N);                                                \
        mckl::Vector<T> z1(N);                                                \
        mckl::Vector<T> z2(N);                                                \
        mckl::Vector<long double> al(N);                                      \
        mckl::Vector<long double> yl(N);                                      \
        mckl::Vector<long double> zl(N);                                      \
                                                                              \
        bool has_cycles = mckl::StopWatch::has_cycles();                      \
                                                                              \
        T e1 = 0;                                                             \
        T e2 = 0;                                                             \
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
                MCKL_EXAMPLE_MATH_ASM_RUN_##AR(func, vfunc);                  \
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
        std::cout.unsetf(std::ios_base::floatfield);                          \
        std::cout << std::setprecision(5);                                    \
        std::cout << std::setw(nwid) << std::left << #func;                   \
        std::cout << std::setw(nwid) << std::right;                           \
        math_val(lb, std::cout);                                              \
        std::cout << std::setw(nwid) << std::right;                           \
        math_val(ub, std::cout);                                              \
        std::cout.unsetf(std::ios_base::floatfield);                          \
        std::cout << std::fixed << std::setprecision(2);                      \
        std::cout << std::setw(twid) << std::right << c1;                     \
        std::cout << std::setw(twid) << std::right << c2;                     \
        std::cout.unsetf(std::ios_base::floatfield);                          \
        std::cout << std::setprecision(2);                                    \
        std::cout << std::setw(twid) << std::right << e1;                     \
        std::cout << std::setw(twid) << std::right << e2;                     \
        std::cout << std::endl;                                               \
    }

MCKL_EXAMPLE_DEFINE_MATH_ASM(A1R1, double, sqrt, vd_sqrt)
MCKL_EXAMPLE_DEFINE_MATH_ASM(A1R1, double, exp, vd_exp)
MCKL_EXAMPLE_DEFINE_MATH_ASM(A1R1, double, exp2, vd_exp2)
MCKL_EXAMPLE_DEFINE_MATH_ASM(A1R1, double, expm1, vd_expm1)
MCKL_EXAMPLE_DEFINE_MATH_ASM(A1R1, double, log, vd_log)
MCKL_EXAMPLE_DEFINE_MATH_ASM(A1R1, double, log2, vd_log2)
MCKL_EXAMPLE_DEFINE_MATH_ASM(A1R1, double, log10, vd_log10)
MCKL_EXAMPLE_DEFINE_MATH_ASM(A1R1, double, log1p, vd_log1p)
MCKL_EXAMPLE_DEFINE_MATH_ASM(A1R1, double, sin, vd_sin)
MCKL_EXAMPLE_DEFINE_MATH_ASM(A1R1, double, cos, vd_cos)
MCKL_EXAMPLE_DEFINE_MATH_ASM(A1R2, double, sincos, vd_sincos)
MCKL_EXAMPLE_DEFINE_MATH_ASM(A1R1, double, tan, vd_tan)

inline void math_asm(std::size_t N, std::size_t M)
{
    const int nwid = 15;
    const int twid = 10;
    const std::size_t lwid = nwid * 3 + twid * 4;

    std::cout << std::string(lwid, '=') << std::endl;

    std::cout << std::setw(nwid) << std::left << "Function";
    std::cout << std::setw(nwid) << std::right << "LB";
    std::cout << std::setw(nwid) << std::right << "UB";
    if (mckl::StopWatch::has_cycles()) {
        std::cout << std::setw(twid) << std::right << "cpE (A)";
        std::cout << std::setw(twid) << std::right << "cpE (V)";
    } else {
        std::cout << std::setw(twid) << std::right << "ME/s (A)";
        std::cout << std::setw(twid) << std::right << "ME/s (V)";
    }
#if MCKL_HAS_BOOST
    std::cout << std::setw(twid) << std::right << "ULP (A)";
    std::cout << std::setw(twid) << std::right << "ULP (V)";
#else
    std::cout << std::setw(twid) << std::right << "Err (A)";
    std::cout << std::setw(twid) << std::right << "Err (V)";
#endif
    std::cout << std::endl;

    std::cout << std::string(lwid, '-') << std::endl;
    math_asm_vd_sqrt(N, M, nwid, twid, 0.1, 1e4);
    math_asm_vd_sqrt(N, M, nwid, twid, DBL_MIN, 0.1);
    math_asm_vd_sqrt(N, M, nwid, twid, 0.1, 1e0);
    math_asm_vd_sqrt(N, M, nwid, twid, 1e0, 1e1);
    math_asm_vd_sqrt(N, M, nwid, twid, 1e1, 1e2);
    math_asm_vd_sqrt(N, M, nwid, twid, 1e2, 1e3);
    math_asm_vd_sqrt(N, M, nwid, twid, 1e3, 1e4);

    std::cout << std::string(lwid, '-') << std::endl;
    math_asm_vd_exp(N, M, nwid, twid, -707, 707);
    math_asm_vd_exp(N, M, nwid, twid, -708.39, -707);
    math_asm_vd_exp(N, M, nwid, twid, -707, -500);
    math_asm_vd_exp(N, M, nwid, twid, -500, -1);
    math_asm_vd_exp(N, M, nwid, twid, -1, -DBL_MIN);
    math_asm_vd_exp(N, M, nwid, twid, -DBL_MIN, DBL_MIN);
    math_asm_vd_exp(N, M, nwid, twid, DBL_MIN, 1);
    math_asm_vd_exp(N, M, nwid, twid, 1, 500);
    math_asm_vd_exp(N, M, nwid, twid, 500, 707);
    math_asm_vd_exp(N, M, nwid, twid, 707, 709.43);

    std::cout << std::string(lwid, '-') << std::endl;
    math_asm_vd_exp2(N, M, nwid, twid, -1022, 1022);
    math_asm_vd_exp2(N, M, nwid, twid, -1022, -1000);
    math_asm_vd_exp2(N, M, nwid, twid, -1000, -500);
    math_asm_vd_exp2(N, M, nwid, twid, -500, -1);
    math_asm_vd_exp2(N, M, nwid, twid, -1, -DBL_MIN);
    math_asm_vd_exp2(N, M, nwid, twid, -DBL_MIN, DBL_MIN);
    math_asm_vd_exp2(N, M, nwid, twid, DBL_MIN, 1);
    math_asm_vd_exp2(N, M, nwid, twid, 1, 500);
    math_asm_vd_exp2(N, M, nwid, twid, 500, 1000);
    math_asm_vd_exp2(N, M, nwid, twid, 1000, 1022);

    std::cout << std::string(lwid, '-') << std::endl;
    math_asm_vd_expm1(N, M, nwid, twid, -500, 500);
    math_asm_vd_expm1(N, M, nwid, twid, -708.39, -707);
    math_asm_vd_expm1(N, M, nwid, twid, -707, -500);
    math_asm_vd_expm1(N, M, nwid, twid, -500, -1);
    math_asm_vd_expm1(N, M, nwid, twid, -1, -DBL_MIN);
    math_asm_vd_expm1(N, M, nwid, twid, -DBL_MIN, DBL_MIN);
    math_asm_vd_expm1(N, M, nwid, twid, DBL_MIN, 1);
    math_asm_vd_expm1(N, M, nwid, twid, 1, 500);
    math_asm_vd_expm1(N, M, nwid, twid, 500, 707);
    math_asm_vd_expm1(N, M, nwid, twid, 707, 709.43);

    std::cout << std::string(lwid, '-') << std::endl;
    math_asm_vd_log(N, M, nwid, twid, 0.1, 1e4);
    math_asm_vd_log(N, M, nwid, twid, 0, DBL_MIN);
    math_asm_vd_log(N, M, nwid, twid, DBL_MIN, 0.1);
    math_asm_vd_log(N, M, nwid, twid, 0.1, 1e0);
    math_asm_vd_log(N, M, nwid, twid, 1e0, 1e1);
    math_asm_vd_log(N, M, nwid, twid, 1e1, 1e2);
    math_asm_vd_log(N, M, nwid, twid, 1e2, 1e3);
    math_asm_vd_log(N, M, nwid, twid, 1e3, 1e4);

    std::cout << std::string(lwid, '-') << std::endl;
    math_asm_vd_log2(N, M, nwid, twid, 0.1, 1e4);
    math_asm_vd_log2(N, M, nwid, twid, 0, DBL_MIN);
    math_asm_vd_log2(N, M, nwid, twid, DBL_MIN, 0.1);
    math_asm_vd_log2(N, M, nwid, twid, 0.1, 1e0);
    math_asm_vd_log2(N, M, nwid, twid, 1e0, 1e1);
    math_asm_vd_log2(N, M, nwid, twid, 1e1, 1e2);
    math_asm_vd_log2(N, M, nwid, twid, 1e2, 1e3);
    math_asm_vd_log2(N, M, nwid, twid, 1e3, 1e4);

    std::cout << std::string(lwid, '-') << std::endl;
    math_asm_vd_log10(N, M, nwid, twid, 0.1, 1e4);
    math_asm_vd_log10(N, M, nwid, twid, 0, DBL_MIN);
    math_asm_vd_log10(N, M, nwid, twid, DBL_MIN, 0.1);
    math_asm_vd_log10(N, M, nwid, twid, 0.1, 1e0);
    math_asm_vd_log10(N, M, nwid, twid, 1e0, 1e1);
    math_asm_vd_log10(N, M, nwid, twid, 1e1, 1e2);
    math_asm_vd_log10(N, M, nwid, twid, 1e2, 1e3);
    math_asm_vd_log10(N, M, nwid, twid, 1e3, 1e4);

    std::cout << std::string(lwid, '-') << std::endl;
    math_asm_vd_log1p(N, M, nwid, twid, 0.1, 1e4);
    math_asm_vd_log1p(N, M, nwid, twid, -1, -DBL_MIN);
    math_asm_vd_log1p(N, M, nwid, twid, -DBL_MIN, DBL_MIN);
    math_asm_vd_log1p(N, M, nwid, twid, DBL_MIN, 0.1);
    math_asm_vd_log1p(N, M, nwid, twid, 0.1, 1e0);
    math_asm_vd_log1p(N, M, nwid, twid, 1e0, 1e1);
    math_asm_vd_log1p(N, M, nwid, twid, 1e1, 1e2);
    math_asm_vd_log1p(N, M, nwid, twid, 1e2, 1e3);
    math_asm_vd_log1p(N, M, nwid, twid, 1e3, 1e4);

    std::cout << std::string(lwid, '-') << std::endl;
    math_asm_vd_sin(N, M, nwid, twid, -1e4, 1e4);
    math_asm_vd_sin(N, M, nwid, twid, 0, DBL_MIN);
    math_asm_vd_sin(N, M, nwid, twid, DBL_MIN, 1);
    math_asm_vd_sin(N, M, nwid, twid, 1e0, 1e1);
    math_asm_vd_sin(N, M, nwid, twid, 1e1, 1e2);
    math_asm_vd_sin(N, M, nwid, twid, 1e2, 1e3);
    math_asm_vd_sin(N, M, nwid, twid, 1e3, 1e4);

    std::cout << std::string(lwid, '-') << std::endl;
    math_asm_vd_cos(N, M, nwid, twid, -1e4, 1e4);
    math_asm_vd_cos(N, M, nwid, twid, 0, DBL_MIN);
    math_asm_vd_cos(N, M, nwid, twid, DBL_MIN, 1);
    math_asm_vd_cos(N, M, nwid, twid, 1e0, 1e1);
    math_asm_vd_cos(N, M, nwid, twid, 1e1, 1e2);
    math_asm_vd_cos(N, M, nwid, twid, 1e2, 1e3);
    math_asm_vd_cos(N, M, nwid, twid, 1e3, 1e4);

    std::cout << std::string(lwid, '-') << std::endl;
    math_asm_vd_sincos(N, M, nwid, twid, -1e4, 1e4);
    math_asm_vd_sincos(N, M, nwid, twid, 0, DBL_MIN);
    math_asm_vd_sincos(N, M, nwid, twid, DBL_MIN, 1);
    math_asm_vd_sincos(N, M, nwid, twid, 1e0, 1e1);
    math_asm_vd_sincos(N, M, nwid, twid, 1e1, 1e2);
    math_asm_vd_sincos(N, M, nwid, twid, 1e2, 1e3);
    math_asm_vd_sincos(N, M, nwid, twid, 1e3, 1e4);

    std::cout << std::string(lwid, '-') << std::endl;
    math_asm_vd_tan(N, M, nwid, twid, -1e4, 1e4);
    math_asm_vd_tan(N, M, nwid, twid, 0, DBL_MIN);
    math_asm_vd_tan(N, M, nwid, twid, DBL_MIN, 1);
    math_asm_vd_tan(N, M, nwid, twid, 1e0, 1e1);
    math_asm_vd_tan(N, M, nwid, twid, 1e1, 1e2);
    math_asm_vd_tan(N, M, nwid, twid, 1e2, 1e3);
    math_asm_vd_tan(N, M, nwid, twid, 1e3, 1e4);

    std::cout << std::string(lwid, '-') << std::endl;
}

#endif // MCKL_EXAMPLE_MATH_ASM_HPP
