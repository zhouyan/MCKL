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

#define MCKL_EXAMPLE_DEFINE_MATH_ASM(T, func, vfunc, wl, wu, lb, ub)          \
    inline void math_asm_##vfunc(std::size_t N, std::size_t M, int nwid)      \
    {                                                                         \
        mckl::UniformRealDistribution<T> unif(wl, wu);                        \
        mckl::UniformIntDistribution<std::size_t> rsize(N / 2, N);            \
        mckl::RNG rng;                                                        \
                                                                              \
        mckl::Vector<T> a(N);                                                 \
        mckl::Vector<T> r1(N);                                                \
        mckl::Vector<T> r2(N);                                                \
        mckl::Vector<long double> al(N);                                      \
        mckl::Vector<long double> rl(N);                                      \
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
                a[0] = wl;                                                    \
                a[1] = wu;                                                    \
                                                                              \
                mckl_##vfunc(K, a.data(), r1.data());                         \
                watch1.start();                                               \
                mckl_##vfunc(K, a.data(), r1.data());                         \
                watch1.stop();                                                \
                                                                              \
                mckl::func(K, a.data(), r2.data());                           \
                watch2.start();                                               \
                mckl::func(K, a.data(), r2.data());                           \
                watch2.stop();                                                \
                                                                              \
                std::copy_n(a.data(), K, al.data());                          \
                mckl::func(K, al.data(), rl.data());                          \
                math_error(K, r1.data(), rl.data(), e1);                      \
                math_error(K, r2.data(), rl.data(), e2);                      \
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
        T v[] = {lb, ub, std::nextafter(lb, -mckl::const_inf<T>()),           \
            std::nextafter(ub, mckl::const_inf<T>()), static_cast<T>(-0.0),   \
            static_cast<T>(0.0), -mckl::const_inf<T>(), mckl::const_inf<T>(), \
            mckl::const_nan<T>()};                                            \
        mckl_##vfunc(sizeof(v) / sizeof(T), v, v);                            \
                                                                              \
        std::cout << std::fixed << std::setprecision(2);                      \
        std::cout << std::setw(nwid) << std::left << #func;                   \
        std::cout << std::setw(nwid) << std::right << c1;                     \
        std::cout << std::setw(nwid) << std::right << c2;                     \
        std::cout.unsetf(std::ios_base::floatfield);                          \
        std::cout << std::setprecision(2);                                    \
        std::cout << std::setw(nwid) << std::right << e1;                     \
        std::cout << std::setw(nwid) << std::right << e2;                     \
        std::cout << std::setprecision(6);                                    \
        std::cout << std::setw(nwid) << std::right << lb;                     \
        std::cout << std::setw(nwid) << std::right << ub;                     \
        for (int j = 2; j != 11; ++j) {                                       \
            T x = std::abs(v[j]);                                             \
            bool positive = x > 0;                                            \
            bool denormal = x < std::numeric_limits<T>::min();                \
            std::stringstream ss;                                             \
            ss << std::setprecision(2) << (positive && denormal ? '*' : ' ')  \
               << v[j];                                                       \
            std::cout << std::setw(nwid) << std::right << ss.str();           \
        }                                                                     \
        std::cout << std::endl;                                               \
    }

MCKL_EXAMPLE_DEFINE_MATH_ASM(double, exp, vd_exp, -707, 707, -708.4, 709.8)
MCKL_EXAMPLE_DEFINE_MATH_ASM(double, exp2, vd_exp2, -1022, 1022, -1022, 1022)
MCKL_EXAMPLE_DEFINE_MATH_ASM(double, expm1, vd_expm1, -500, 500, -708.4, 709.8)
// MCKL_EXAMPLE_DEFINE_MATH_ASM(log, 1.0, 1.1, 1.0, 1.1)

inline void math_asm(std::size_t N, std::size_t M)
{
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

    math_asm_vd_exp(N, M, nwid);
    math_asm_vd_exp2(N, M, nwid);
    math_asm_vd_expm1(N, M, nwid);
    // math_asm_log(N, M, nwid);

    std::cout << std::string(lwid, '-') << std::endl;
}

#endif // MCKL_EXAMPLE_MATH_ASM_HPP
