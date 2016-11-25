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

#define MCKL_EXAMPLE_DEFINE_MATH_ASM(T, U, func, vfunc, wl, wu, lb, ub)       \
    inline void math_asm_##vfunc(std::size_t N, std::size_t M, int nwid)      \
    {                                                                         \
        mckl::UniformRealDistribution<T> unif(wl, wu);                        \
        mckl::UniformIntDistribution<std::size_t> rsize(N / 2, N);            \
        mckl::RNG rng;                                                        \
                                                                              \
        mckl::Vector<T> a(N);                                                 \
        mckl::Vector<T> r1(N);                                                \
        mckl::Vector<T> r2(N);                                                \
        mckl::Vector<T> r3(N);                                                \
        mckl::Vector<long double> al(N);                                      \
        mckl::Vector<long double> rl(N);                                      \
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
                                                                              \
                mckl::func<T>(K, a.data(), r1.data());                        \
                watch1.start();                                               \
                mckl::func<T>(K, a.data(), r1.data());                        \
                watch1.stop();                                                \
                                                                              \
                mckl_##vfunc(K, a.data(), r2.data());                         \
                watch2.start();                                               \
                mckl_##vfunc(K, a.data(), r2.data());                         \
                watch2.stop();                                                \
                                                                              \
                mckl::func(K, a.data(), r3.data());                           \
                watch3.start();                                               \
                mckl::func(K, a.data(), r3.data());                           \
                watch3.stop();                                                \
                                                                              \
                std::copy_n(a.data(), K, al.data());                          \
                mckl::func(K, al.data(), rl.data());                          \
                math_error(K, r1.data(), rl.data(), e1);                      \
                math_error(K, r2.data(), rl.data(), e2);                      \
                math_error(K, r3.data(), rl.data(), e3);                      \
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
        T vf[] = {lbf, ubf, std::nextafter(lbf, -mckl::const_inf<T>()),       \
            std::nextafter(ubf, mckl::const_inf<T>()), static_cast<T>(-0.0),  \
            static_cast<T>(0.0), -mckl::const_inf<T>(), mckl::const_inf<T>(), \
            mckl::const_nan<T>()};                                            \
        const std::size_t vn = sizeof(vf) / sizeof(T);                        \
        long double vl[vn];                                                   \
        std::copy_n(vf, vn, vl);                                              \
        mckl_##vfunc(vn, vf, vf);                                             \
        mckl::func(vn, vl, vl);                                               \
        T el = 0;                                                             \
        T eu = 0;                                                             \
        math_error(1, vf + 0, vl + 0, el);                                    \
        math_error(1, vf + 1, vl + 1, eu);                                    \
                                                                              \
        std::cout << std::fixed << std::setprecision(2);                      \
        std::cout << std::setw(nwid) << std::left << #func;                   \
        std::cout << std::setw(nwid) << std::right << c1;                     \
        std::cout << std::setw(nwid) << std::right << c2;                     \
        std::cout << std::setw(nwid) << std::right << c3;                     \
        std::cout.unsetf(std::ios_base::floatfield);                          \
        std::cout << std::setprecision(2);                                    \
        std::cout << std::setw(nwid) << std::right << e1;                     \
        std::cout << std::setw(nwid) << std::right << e2;                     \
        std::cout << std::setw(nwid) << std::right << e3;                     \
        std::cout << std::setw(nwid) << std::right << el;                     \
        std::cout << std::setw(nwid) << std::right << eu;                     \
        for (int j = 2; j != vn; ++j) {                                       \
            union {                                                           \
                T x;                                                          \
                U y;                                                          \
            };                                                                \
            x = std::abs(vf[j]);                                              \
            bool positive = x > 0;                                            \
            bool denormal = x < std::numeric_limits<T>::min();                \
            bool negazero = mckl::internal::is_zero(x) && y != 0;             \
            std::stringstream ss;                                             \
            ss << std::setprecision(2) << (positive && denormal ? '*' : ' ')  \
               << (negazero ? '-' : ' ') << vf[j];                            \
            std::cout << std::setw(nwid) << std::right << ss.str();           \
        }                                                                     \
        std::cout << std::endl;                                               \
    }

MCKL_EXAMPLE_DEFINE_MATH_ASM(double, std::uint64_t, sqrt, vd_sqrt, 0.1, 1e4,
    0x0000000000000001, 0x7FEFFFFFFFFFFFFF)

MCKL_EXAMPLE_DEFINE_MATH_ASM(double, std::uint64_t, exp, vd_exp, -707, 707,
    0xC086232BDD7ABCD2, 0x40862B7D369A5AA7)
MCKL_EXAMPLE_DEFINE_MATH_ASM(double, std::uint64_t, exp2, vd_exp2, -1022, 1022,
    0xC08FF00000000000, 0x408FF80000000000)
MCKL_EXAMPLE_DEFINE_MATH_ASM(double, std::uint64_t, expm1, vd_expm1, -500, 500,
    0xC086232BDD7ABCD2, 0x40862B7D369A5AA7)
MCKL_EXAMPLE_DEFINE_MATH_ASM(double, std::uint64_t, log, vd_log, 0.1, 1e4,
    0x0010000000000000, 0x7FEFFFFFFFFFFFFF)
MCKL_EXAMPLE_DEFINE_MATH_ASM(double, std::uint64_t, log2, vd_log2, 0.1, 1e4,
    0x0010000000000000, 0x7FEFFFFFFFFFFFFF)
MCKL_EXAMPLE_DEFINE_MATH_ASM(double, std::uint64_t, log10, vd_log10, 0.1, 1e4,
    0x0010000000000000, 0x7FEFFFFFFFFFFFFF)
MCKL_EXAMPLE_DEFINE_MATH_ASM(double, std::uint64_t, log1p, vd_log1p, 1.0, 1e4,
    0xBFEFFFFFFFFFFFFF, 0x7FEFFFFFFFFFFFFF)

MCKL_EXAMPLE_DEFINE_MATH_ASM(double, std::uint64_t, sin, vd_sin, -1e4, 1e4,
    0xC1D0000000000000, 0x41D0000000000000)
MCKL_EXAMPLE_DEFINE_MATH_ASM(double, std::uint64_t, cos, vd_cos, -1e4, 1e4,
    0xC1D0000000000000, 0x41D0000000000000)

inline void math_asm_vd_sincos(std::size_t N, std::size_t M, int nwid)
{
    mckl::UniformRealDistribution<double> unif(-1e4, 1e4);
    mckl::UniformIntDistribution<std::size_t> rsize(N / 2, N);
    mckl::RNG rng;

    mckl::Vector<double> a(N);
    mckl::Vector<double> r1(N);
    mckl::Vector<double> r2(N);
    mckl::Vector<double> r3(N);
    mckl::Vector<double> s1(N);
    mckl::Vector<double> s2(N);
    mckl::Vector<double> s3(N);
    mckl::Vector<long double> al(N);
    mckl::Vector<long double> rl(N);
    mckl::Vector<long double> sl(N);

    bool has_cycles = mckl::StopWatch::has_cycles();

    double e1 = 0;
    double e2 = 0;
    double e3 = 0;
    double c1 = has_cycles ? std::numeric_limits<double>::max() : 0.0;
    double c2 = has_cycles ? std::numeric_limits<double>::max() : 0.0;
    double c3 = has_cycles ? std::numeric_limits<double>::max() : 0.0;
    for (std::size_t k = 0; k != 10; ++k) {
        std::size_t n = 0;
        mckl::StopWatch watch1;
        mckl::StopWatch watch2;
        mckl::StopWatch watch3;
        for (std::size_t i = 0; i != M; ++i) {
            std::size_t K = rsize(rng);
            n += K;
            mckl::rand(rng, unif, K, a.data());

            mckl::sincos<double>(K, a.data(), r1.data(), s1.data());
            watch1.start();
            mckl::sincos<double>(K, a.data(), r1.data(), s1.data());
            watch1.stop();

            mckl_vd_sincos(K, a.data(), r2.data(), s2.data());
            watch2.start();
            mckl_vd_sincos(K, a.data(), r2.data(), s2.data());
            watch2.stop();

            mckl::sincos(K, a.data(), r3.data(), s3.data());
            watch3.start();
            mckl::sincos(K, a.data(), r3.data(), s3.data());
            watch3.stop();

            std::copy_n(a.data(), K, al.data());
            mckl::sincos(K, al.data(), rl.data(), sl.data());
            math_error(K, r1.data(), rl.data(), e1);
            math_error(K, s1.data(), sl.data(), e1);
            math_error(K, r2.data(), rl.data(), e2);
            math_error(K, s2.data(), sl.data(), e2);
            math_error(K, r3.data(), rl.data(), e3);
            math_error(K, s3.data(), sl.data(), e3);
        }
        if (has_cycles) {
            c1 = std::min(c1, 1.0 * watch1.cycles() / n);
            c2 = std::min(c2, 1.0 * watch2.cycles() / n);
            c3 = std::min(c3, 1.0 * watch3.cycles() / n);
        } else {
            c1 = std::max(c1, n / watch1.seconds() * 1e-6);
            c2 = std::max(c2, n / watch2.seconds() * 1e-6);
            c3 = std::max(c3, n / watch3.seconds() * 1e-6);
        }
    }

    union {
        double lbf;
        std::uint64_t lbi;
    };
    union {
        double ubf;
        std::uint64_t ubi;
    };
    lbi = 0xC1D0000000000000;
    ubi = 0x41D0000000000000;
    double vf[] = {lbf, ubf, std::nextafter(lbf, -mckl::const_inf<double>()),
        std::nextafter(ubf, mckl::const_inf<double>()), -0.0, 0.0,
        -mckl::const_inf<double>(), mckl::const_inf<double>(),
        mckl::const_nan<double>()};
    const std::size_t vn = sizeof(vf) / sizeof(double);
    double wf[vn];
    long double vl[vn];
    long double wl[vn];
    std::copy_n(vf, vn, vl);
    mckl_vd_sincos(vn, vf, vf, wf);
    mckl::sincos(vn, vl, vl, wl);
    double el = 0;
    double eu = 0;
    math_error(1, vf + 0, vl + 0, el);
    math_error(1, wf + 0, wl + 0, el);
    math_error(1, vf + 1, vl + 1, eu);
    math_error(1, wf + 1, wl + 1, eu);

    std::cout << std::fixed << std::setprecision(2);
    std::cout << std::setw(nwid) << std::left << "sincos";
    std::cout << std::setw(nwid) << std::right << c1;
    std::cout << std::setw(nwid) << std::right << c2;
    std::cout << std::setw(nwid) << std::right << c3;
    std::cout.unsetf(std::ios_base::floatfield);
    std::cout << std::setprecision(2);
    std::cout << std::setw(nwid) << std::right << e1;
    std::cout << std::setw(nwid) << std::right << e2;
    std::cout << std::setw(nwid) << std::right << e3;
    std::cout << std::setw(nwid) << std::right << el;
    std::cout << std::setw(nwid) << std::right << eu;
    for (int j = 2; j != vn; ++j) {
        union {
            double x;
            std::uint64_t y;
        };
        x = std::abs(vf[j]);
        bool positive = x > 0;
        bool denormal = x < std::numeric_limits<double>::min();
        bool negazero = mckl::internal::is_zero(x) && y != 0;
        std::stringstream ss;
        ss << std::setprecision(2) << (positive && denormal ? '*' : ' ')
           << (negazero ? '-' : ' ') << vf[j];
        std::cout << std::setw(nwid) << std::right << ss.str();
    }
    std::cout << std::endl;
}

inline void math_asm(std::size_t N, std::size_t M)
{
    const int nwid = 10;
    const std::size_t lwid = nwid * 16;

    std::cout << std::string(lwid, '=') << std::endl;

    std::cout << std::setw(nwid) << std::left << "Function";
    if (mckl::StopWatch::has_cycles()) {
        std::cout << std::setw(nwid) << std::right << "cpE (S)";
        std::cout << std::setw(nwid) << std::right << "cpE (A)";
        std::cout << std::setw(nwid) << std::right << "cpE (V)";
    } else {
        std::cout << std::setw(nwid) << std::right << "ME/s (S)";
        std::cout << std::setw(nwid) << std::right << "ME/s (A)";
        std::cout << std::setw(nwid) << std::right << "ME/s (V)";
    }
#if MCKL_HAS_BOOST
    std::cout << std::setw(nwid) << std::right << "ULP (S)";
    std::cout << std::setw(nwid) << std::right << "ULP (A)";
    std::cout << std::setw(nwid) << std::right << "ULP (V)";
    std::cout << std::setw(nwid) << std::right << "ULP (LB)";
    std::cout << std::setw(nwid) << std::right << "ULP (UB)";
#else
    std::cout << std::setw(nwid) << std::right << "Err (S)";
    std::cout << std::setw(nwid) << std::right << "Err (A)";
    std::cout << std::setw(nwid) << std::right << "Err (V)";
    std::cout << std::setw(nwid) << std::right << "Err (LB)";
    std::cout << std::setw(nwid) << std::right << "Err (UB)";
#endif
    std::cout << std::setw(nwid) << std::right << "a < LB";
    std::cout << std::setw(nwid) << std::right << "a > UB";
    std::cout << std::setw(nwid) << std::right << "a = -0.0";
    std::cout << std::setw(nwid) << std::right << "a = 0.0";
    std::cout << std::setw(nwid) << std::right << "a = -Inf";
    std::cout << std::setw(nwid) << std::right << "a = Inf";
    std::cout << std::setw(nwid) << std::right << "a = NaN";
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

    std::cout << std::string(lwid, '-') << std::endl;
}

#endif // MCKL_EXAMPLE_MATH_ASM_HPP
