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

#define MCKL_EXAMPLE_MATH_ASM_RUN_A1R1(T, func, vfunc)                        \
    std::copy_n(a.data(), K, y1.data());                                      \
    watch1.start();                                                           \
    mckl::func<T>(K, y1.data(), y1.data());                                   \
    watch1.stop();                                                            \
                                                                              \
    std::copy_n(a.data(), K, y2.data());                                      \
    watch2.start();                                                           \
    mckl::func(K, y2.data(), y2.data());                                      \
    watch2.stop();                                                            \
                                                                              \
    std::copy_n(a.data(), K, y3.data());                                      \
    watch3.start();                                                           \
    mckl_##vfunc(K, y3.data(), y3.data());                                    \
    watch3.stop();                                                            \
                                                                              \
    std::copy_n(a.data(), K, yl.data());                                      \
    mckl::func(K, yl.data(), yl.data());                                      \
    math_error(K, y1.data(), yl.data(), e1);                                  \
    math_error(K, y2.data(), yl.data(), e2);                                  \
    math_error(K, y3.data(), yl.data(), e3);

#define MCKL_EXAMPLE_MATH_ASM_RUN_A1R2(T, func, vfunc)                        \
    std::copy_n(a.data(), K, y1.data());                                      \
    watch1.start();                                                           \
    mckl::func<T>(K, y1.data(), y1.data(), z1.data());                        \
    watch1.stop();                                                            \
                                                                              \
    std::copy_n(a.data(), K, y2.data());                                      \
    watch2.start();                                                           \
    mckl::func(K, y2.data(), y2.data(), z2.data());                           \
    watch2.stop();                                                            \
                                                                              \
    std::copy_n(a.data(), K, y3.data());                                      \
    watch3.start();                                                           \
    mckl_##vfunc(K, y3.data(), y3.data(), z3.data());                         \
    watch3.stop();                                                            \
                                                                              \
    std::copy_n(a.data(), K, yl.data());                                      \
    mckl::func(K, yl.data(), yl.data(), zl.data());                           \
    math_error(K, y1.data(), yl.data(), e1);                                  \
    math_error(K, z1.data(), zl.data(), e1);                                  \
    math_error(K, y2.data(), yl.data(), e2);                                  \
    math_error(K, z2.data(), zl.data(), e2);                                  \
    math_error(K, y3.data(), yl.data(), e3);                                  \
    math_error(K, z3.data(), zl.data(), e3);

#define MCKL_EXAMPLE_DEFINE_MATH_ASM(AR, T, func, vfunc)                      \
    template <typename U>                                                     \
    inline void math_asm_##vfunc##_check(U lb, U ub)                          \
    {                                                                         \
        union {                                                               \
            T x;                                                              \
            U u;                                                              \
        };                                                                    \
                                                                              \
        union {                                                               \
            T y;                                                              \
            U v;                                                              \
        };                                                                    \
                                                                              \
        u = lb;                                                               \
        v = ub;                                                               \
                                                                              \
        mckl::Vector<T> a;                                                    \
        a.push_back(std::nextafter(x, -mckl::const_inf<T>()));                \
        a.push_back(x);                                                       \
        a.push_back(std::nextafter(x, mckl::const_inf<T>()));                 \
        a.push_back(std::nextafter(y, -mckl::const_inf<T>()));                \
        a.push_back(y);                                                       \
        a.push_back(std::nextafter(y, mckl::const_inf<T>()));                 \
        a.push_back(-mckl::const_zero<T>());                                  \
        a.push_back(mckl::const_zero<T>());                                   \
        a.push_back(-mckl::const_inf<T>());                                   \
        a.push_back(mckl::const_inf<T>());                                    \
        a.push_back(mckl::const_nan<T>());                                    \
                                                                              \
        mckl::Vector<std::string> s;                                          \
        s.push_back("a < Lower bound");                                       \
        s.push_back("a = Lower bound");                                       \
        s.push_back("a > Lower bound");                                       \
        s.push_back("a < Upper bound");                                       \
        s.push_back("a = Upper bound");                                       \
        s.push_back("a > Upper bound");                                       \
        s.push_back("a = -0.0");                                              \
        s.push_back("a = +0.0");                                              \
        s.push_back("a = -inf");                                              \
        s.push_back("a = +inf");                                              \
        s.push_back("a =  nan");                                              \
                                                                              \
        const std::size_t K = a.size();                                       \
                                                                              \
        mckl::Vector<T> y1(K);                                                \
        mckl::Vector<T> y2(K);                                                \
        mckl::Vector<T> y3(K);                                                \
        mckl::Vector<T> z1(K);                                                \
        mckl::Vector<T> z2(K);                                                \
        mckl::Vector<T> z3(K);                                                \
        mckl::Vector<long double> al(K);                                      \
        mckl::Vector<long double> yl(K);                                      \
        mckl::Vector<long double> zl(K);                                      \
                                                                              \
        mckl::StopWatch watch1;                                               \
        mckl::StopWatch watch2;                                               \
        mckl::StopWatch watch3;                                               \
        T e1 = 0;                                                             \
        T e2 = 0;                                                             \
        T e3 = 0;                                                             \
                                                                              \
        MCKL_EXAMPLE_MATH_ASM_RUN_##AR(T, func, vfunc);                       \
                                                                              \
        const int nwid = 15;                                                  \
        const int twid = 12;                                                  \
        const std::size_t lwid = nwid + twid * 6;                             \
        std::cout << std::string(lwid, '=') << std::endl;                     \
        std::cout << std::setw(nwid) << std::left << #func;                   \
        std::cout << std::setw(twid) << std::right << "Val (S)";              \
        std::cout << std::setw(twid) << std::right << "Val (V)";              \
        std::cout << std::setw(twid) << std::right << "Val (A)";              \
        std::cout << std::setw(twid) << std::right << "ULP (S)";              \
        std::cout << std::setw(twid) << std::right << "ULP (V)";              \
        std::cout << std::setw(twid) << std::right << "ULP (A)";              \
        std::cout << std::endl;                                               \
        std::cout << std::string(lwid, '-') << std::endl;                     \
        for (std::size_t i = 0; i != K; ++i) {                                \
            std::cout.unsetf(std::ios_base::floatfield);                      \
            std::cout << std::setprecision(3);                                \
            std::cout << std::setw(nwid) << std::left << s[i];                \
            std::cout << std::setw(twid) << std::right;                       \
            math_val(y1[i], std::cout);                                       \
            std::cout << std::setw(twid) << std::right;                       \
            math_val(y2[i], std::cout);                                       \
            std::cout << std::setw(twid) << std::right;                       \
            math_val(y3[i], std::cout);                                       \
            std::cout.unsetf(std::ios_base::floatfield);                      \
            std::cout << std::setprecision(2);                                \
            std::cout << std::setw(twid) << std::right                        \
                      << math_error(y1[i], yl[i]);                            \
            std::cout << std::setw(twid) << std::right                        \
                      << math_error(y2[i], yl[i]);                            \
            std::cout << std::setw(twid) << std::right                        \
                      << math_error(y3[i], yl[i]);                            \
            std::cout << std::endl;                                           \
        }                                                                     \
        std::cout << std::string(lwid, '-') << std::endl;                     \
    }                                                                         \
                                                                              \
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
        mckl::Vector<T> y3(N);                                                \
        mckl::Vector<T> z1(N);                                                \
        mckl::Vector<T> z2(N);                                                \
        mckl::Vector<T> z3(N);                                                \
        mckl::Vector<long double> al(N);                                      \
        mckl::Vector<long double> yl(N);                                      \
        mckl::Vector<long double> zl(N);                                      \
                                                                              \
        bool has_cycles = mckl::StopWatch::has_cycles();                      \
                                                                              \
        T e1 = 0;                                                             \
        T e2 = 0;                                                             \
        T e3 = 0;                                                             \
        double c1 = has_cycles ? std::numeric_limits<double>::max() : 0.0;    \
        double c2 = has_cycles ? std::numeric_limits<double>::max() : 0.0;    \
        double c3 = has_cycles ? std::numeric_limits<double>::max() : 0.0;    \
        for (std::size_t k = 0; k != 10; ++k) {                               \
            std::size_t n = 0;                                                \
            mckl::StopWatch watch1;                                           \
            mckl::StopWatch watch2;                                           \
            mckl::StopWatch watch3;                                           \
            for (std::size_t i = 0; i != M; ++i) {                            \
                std::size_t K = rsize(rng);                                   \
                n += K;                                                       \
                mckl::rand(rng, unif, K, a.data());                           \
                MCKL_EXAMPLE_MATH_ASM_RUN_##AR(T, func, vfunc);               \
            }                                                                 \
            if (has_cycles) {                                                 \
                c1 = std::min(c1, 1.0 * watch1.cycles() / n);                 \
                c2 = std::min(c2, 1.0 * watch2.cycles() / n);                 \
                c3 = std::min(c3, 1.0 * watch3.cycles() / n);                 \
            } else {                                                          \
                c1 = std::max(c1, n / watch1.seconds() * 1e-6);               \
                c2 = std::max(c2, n / watch2.seconds() * 1e-6);               \
                c3 = std::max(c3, n / watch3.seconds() * 1e-6);               \
            }                                                                 \
        }                                                                     \
                                                                              \
        std::cout.unsetf(std::ios_base::floatfield);                          \
        std::cout << std::setprecision(5);                                    \
        std::cout << std::setw(nwid) << std::left << #vfunc;                  \
        std::cout << std::setw(nwid) << std::right;                           \
        math_val(lb, std::cout);                                              \
        std::cout << std::setw(nwid) << std::right;                           \
        math_val(ub, std::cout);                                              \
        std::cout.unsetf(std::ios_base::floatfield);                          \
        std::cout << std::fixed << std::setprecision(2);                      \
        std::cout << std::setw(twid) << std::right << c1;                     \
        std::cout << std::setw(twid) << std::right << c2;                     \
        std::cout << std::setw(twid) << std::right << c3;                     \
        std::cout.unsetf(std::ios_base::floatfield);                          \
        std::cout << std::setprecision(2);                                    \
        std::cout << std::setw(twid) << std::right << e1;                     \
        std::cout << std::setw(twid) << std::right << e2;                     \
        std::cout << std::setw(twid) << std::right << e3;                     \
        std::cout << std::endl;                                               \
    }

template <typename Test, typename T>
inline void math_asm(std::size_t N, std::size_t M, Test &&test,
    const mckl::Vector<std::pair<T, T>> &bounds)
{
    const int nwid = 15;
    const int twid = 10;
    const std::size_t lwid = nwid * 3 + twid * 6;

    std::cout << std::string(lwid, '=') << std::endl;

    std::cout << std::setw(nwid) << std::left << "Function";
    std::cout << std::setw(nwid) << std::right << "LB";
    std::cout << std::setw(nwid) << std::right << "UB";
    if (mckl::StopWatch::has_cycles()) {
        std::cout << std::setw(twid) << std::right << "cpE (S)";
        std::cout << std::setw(twid) << std::right << "cpE (V)";
        std::cout << std::setw(twid) << std::right << "cpE (A)";
    } else {
        std::cout << std::setw(twid) << std::right << "ME/s (S)";
        std::cout << std::setw(twid) << std::right << "ME/s (V)";
        std::cout << std::setw(twid) << std::right << "ME/s (A)";
    }
#if MCKL_HAS_BOOST
    std::cout << std::setw(twid) << std::right << "ULP (S)";
    std::cout << std::setw(twid) << std::right << "ULP (V)";
    std::cout << std::setw(twid) << std::right << "ULP (A)";
#else
    std::cout << std::setw(twid) << std::right << "Err (S)";
    std::cout << std::setw(twid) << std::right << "Err (V)";
    std::cout << std::setw(twid) << std::right << "Err (A)";
#endif
    std::cout << std::endl;

    std::cout << std::string(lwid, '-') << std::endl;
    for (auto &bd : bounds)
        test(N, M, nwid, twid, bd.first, bd.second);
    std::cout << std::string(lwid, '-') << std::endl;
}

template <typename Test, typename T>
inline void math_asm(int argc, char **argv, Test &&test,
    const mckl::Vector<std::pair<T, T>> &bounds)
{
    --argc;
    ++argv;

    std::size_t N = 10000;
    if (argc > 0) {
        std::size_t n = static_cast<std::size_t>(std::atoi(*argv));
        if (n != 0) {
            N = n;
            --argc;
            ++argv;
        }
    }

    std::size_t M = 100;
    if (argc > 0) {
        std::size_t m = static_cast<std::size_t>(std::atoi(*argv));
        if (m != 0) {
            M = m;
            --argc;
            ++argv;
        }
    }

    math_asm(N, M, std::forward<Test>(test), bounds);
}

#endif // MCKL_EXAMPLE_MATH_ASM_HPP
