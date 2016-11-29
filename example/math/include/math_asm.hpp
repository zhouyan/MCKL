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
    watch2.stop();

#define MCKL_EXAMPLE_MATH_ASM_RUN_A1R2(func, vfunc)                           \
    mckl_##vfunc(K, a.data(), y1.data(), z1.data());                          \
    watch1.start();                                                           \
    mckl_##vfunc(K, a.data(), y1.data(), z1.data());                          \
    watch1.stop();                                                            \
                                                                              \
    mckl::func(K, a.data(), y2.data(), z2.data());                            \
    watch2.start();                                                           \
    mckl::func(K, a.data(), y2.data(), z2.data());                            \
    watch2.stop();

#define MCKL_EXAMPLE_MATH_ASM_RUNL_A1R1(func)                                 \
    mckl::func(K, al.data(), yl.data());

#define MCKL_EXAMPLE_MATH_ASM_RUNL_A1R2(func)                                 \
    mckl::func(K, al.data(), yl.data(), zl.data());

#define MCKL_EXAMPLE_DEFINE_MATH_ASM(AR, T, U, func, vfunc, wl, wu, lb, ub)   \
    inline void math_asm_##vfunc(std::size_t N, std::size_t M, int nwid)      \
    {                                                                         \
        mckl::UniformRealDistribution<T> unif(wl, wu);                        \
        mckl::UniformIntDistribution<std::size_t> rsize(N / 2, N);            \
        mckl::RNG_64 rng;                                                     \
                                                                              \
        mckl::Vector<T> a(N);                                                 \
        mckl::Vector<T> y1(N);                                                \
        mckl::Vector<T> y2(N);                                                \
        mckl::Vector<T> z1(N);                                                \
        mckl::Vector<T> z2(N);                                                \
        std::fill(y1.begin(), y1.end(), 0);                                   \
        std::fill(y2.begin(), y2.end(), 0);                                   \
        std::fill(z1.begin(), z1.end(), 0);                                   \
        std::fill(z2.begin(), z2.end(), 0);                                   \
                                                                              \
        bool has_cycles = mckl::StopWatch::has_cycles();                      \
                                                                              \
        T e = 0;                                                              \
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
                math_error(K, y1.data(), y2.data(), e);                       \
                math_error(K, z1.data(), z2.data(), e);                       \
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
        union {                                                               \
            T lbf;                                                            \
            U lbi;                                                            \
        };                                                                    \
        union {                                                               \
            T ubf;                                                            \
            U ubi;                                                            \
        };                                                                    \
        lbi = lb;                                                             \
        ubi = ub;                                                             \
        mckl::StopWatch watch1;                                               \
        mckl::StopWatch watch2;                                               \
        std::size_t K = 0;                                                    \
        a[K++] = lbf;                                                         \
        a[K++] = ubf;                                                         \
        a[K++] = std::nextafter(lbf, -mckl::const_inf<T>());                  \
        a[K++] = std::nextafter(ubf, mckl::const_inf<T>());                   \
        a[K++] = -0.0;                                                        \
        a[K++] = 0.0;                                                         \
        a[K++] = -mckl::const_inf<T>();                                       \
        a[K++] = mckl::const_inf<T>();                                        \
        a[K++] = mckl::const_nan<T>();                                        \
        MCKL_EXAMPLE_MATH_ASM_RUN_##AR(func, vfunc);                          \
                                                                              \
        std::cout << std::fixed << std::setprecision(2);                      \
        std::cout << std::setw(nwid) << std::left << #func;                   \
        std::cout << std::setw(nwid) << std::right << c1;                     \
        std::cout << std::setw(nwid) << std::right << c2;                     \
        std::cout.unsetf(std::ios_base::floatfield);                          \
        std::cout << std::setprecision(2);                                    \
        std::cout << std::setw(nwid) << std::right << e;                      \
        for (std::size_t j = 0; j != K; ++j) {                                \
            union {                                                           \
                T t;                                                          \
                U u;                                                          \
            };                                                                \
            t = std::abs(y1[j]);                                              \
            bool positive = t > 0;                                            \
            bool denormal = t < std::numeric_limits<T>::min();                \
            std::stringstream ss;                                             \
            ss << std::setprecision(2);                                       \
            ss << (positive && denormal ? '*' : ' ');                         \
            ss << y1[j];                                                      \
            std::cout << std::setw(nwid) << std::right << ss.str();           \
        }                                                                     \
        std::cout << std::endl;                                               \
    }

MCKL_EXAMPLE_DEFINE_MATH_ASM(A1R1, double, std::uint64_t, sqrt, vd_sqrt, 0.1,
    1e4, 0x0000000000000001, 0x7FEFFFFFFFFFFFFF)

MCKL_EXAMPLE_DEFINE_MATH_ASM(A1R1, double, std::uint64_t, exp, vd_exp, -707,
    707, 0xC086232BDD7ABCD2, 0x40862B7D369A5AA7)
MCKL_EXAMPLE_DEFINE_MATH_ASM(A1R1, double, std::uint64_t, exp2, vd_exp2, -1022,
    1022, 0xC08FF00000000000, 0x408FF80000000000)
MCKL_EXAMPLE_DEFINE_MATH_ASM(A1R1, double, std::uint64_t, expm1, vd_expm1,
    -500, 500, 0xC086232BDD7ABCD2, 0x40862B7D369A5AA7)
MCKL_EXAMPLE_DEFINE_MATH_ASM(A1R1, double, std::uint64_t, log, vd_log, 0.1,
    1e4, 0x0010000000000000, 0x7FEFFFFFFFFFFFFF)
MCKL_EXAMPLE_DEFINE_MATH_ASM(A1R1, double, std::uint64_t, log2, vd_log2, 0.1,
    1e4, 0x0010000000000000, 0x7FEFFFFFFFFFFFFF)
MCKL_EXAMPLE_DEFINE_MATH_ASM(A1R1, double, std::uint64_t, log10, vd_log10, 0.1,
    1e4, 0x0010000000000000, 0x7FEFFFFFFFFFFFFF)
MCKL_EXAMPLE_DEFINE_MATH_ASM(A1R1, double, std::uint64_t, log1p, vd_log1p, 1.0,
    1e4, 0xBFF0000000000000, 0x7FEFFFFFFFFFFFFF)

MCKL_EXAMPLE_DEFINE_MATH_ASM(A1R1, double, std::uint64_t, sin, vd_sin, -1e4,
    1e4, 0xFFEFFFFFFFFFFFFF, 0x7FEFFFFFFFFFFFFF)
MCKL_EXAMPLE_DEFINE_MATH_ASM(A1R1, double, std::uint64_t, cos, vd_cos, -1e4,
    1e4, 0xFFEFFFFFFFFFFFFF, 0x7FEFFFFFFFFFFFFF)
MCKL_EXAMPLE_DEFINE_MATH_ASM(A1R2, double, std::uint64_t, sincos, vd_sincos,
    -1e4, 1e4, 0xFFEFFFFFFFFFFFFF, 0x7FEFFFFFFFFFFFFF)
MCKL_EXAMPLE_DEFINE_MATH_ASM(A1R1, double, std::uint64_t, tan, vd_tan, -1e4,
    1e4, 0xFFEFFFFFFFFFFFFF, 0x7FEFFFFFFFFFFFFF)

inline void math_asm(std::size_t N, std::size_t M)
{
    const int nwid = 9;
    const std::size_t lwid = nwid * 13;

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
    std::cout << std::setw(nwid) << std::right << "ULP";
#else
    std::cout << std::setw(nwid) << std::right << "Err";
#endif
    std::cout << std::setw(nwid) << std::right << "LB";
    std::cout << std::setw(nwid) << std::right << "UB";
    std::cout << std::setw(nwid) << std::right << "< LB";
    std::cout << std::setw(nwid) << std::right << "> UB";
    std::cout << std::setw(nwid) << std::right << "-0.0";
    std::cout << std::setw(nwid) << std::right << "0.0";
    std::cout << std::setw(nwid) << std::right << "-Inf";
    std::cout << std::setw(nwid) << std::right << "Inf";
    std::cout << std::setw(nwid) << std::right << "NaN";
    std::cout << std::endl;

    std::cout << std::string(lwid, '-') << std::endl;

    math_asm_vd_sqrt(N, M, nwid);

    math_asm_vd_exp(N, M, nwid);
    math_asm_vd_exp2(N, M, nwid);
    math_asm_vd_expm1(N, M, nwid);
    math_asm_vd_log(N, M, nwid);
    math_asm_vd_log2(N, M, nwid);
    math_asm_vd_log10(N, M, nwid);
    math_asm_vd_log1p(N, M, nwid);

    math_asm_vd_sin(N, M, nwid);
    math_asm_vd_cos(N, M, nwid);
    math_asm_vd_sincos(N, M, nwid);
    math_asm_vd_tan(N, M, nwid);

    std::cout << std::string(lwid, '-') << std::endl;
}

#endif // MCKL_EXAMPLE_MATH_ASM_HPP
