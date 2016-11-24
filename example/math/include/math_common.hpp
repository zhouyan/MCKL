//============================================================================
// MCKL/example/math/include/math_common.hpp
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

#ifndef MCKL_EXAMPLE_MATH_COMMON_HPP
#define MCKL_EXAMPLE_MATH_COMMON_HPP

#include <mckl/math/vmath.hpp>
#include <mckl/random/rng.hpp>
#include <mckl/random/uniform_int_distribution.hpp>
#include <mckl/random/uniform_real_distribution.hpp>
#include <mckl/utility/stop_watch.hpp>

#if MCKL_HAS_BOOST
#define BOOST_MATH_DOMAIN_ERROR_POLICY ignore_error
#include <boost/math/special_functions/next.hpp>
#endif

class MathPerf
{
    public:
    std::string name;
    float e1;
    float e2;
    double e3;
    double e4;
    double c1;
    double c2;
    double c3;
    double c4;
    double c5;
    double c6;
    float vs[12];
    double vd[12];
}; // class MathPerf

template <typename T, typename U>
inline void math_error(std::size_t n, const T *r1, const U *r2, T &e1, T &e2)
{
    T f1 = 0;
    for (std::size_t i = 0; i != n; ++i)
        f1 = std::max(f1, std::abs(r1[i] - static_cast<T>(r2[i])));

    T f2 = 0;
#if MCKL_HAS_BOOST
    for (std::size_t i = 0; i != n; ++i) {
        f2 = std::max(
            f2, boost::math::float_distance(r1[i], static_cast<T>(r2[i])));
    }
#else
    for (std::size_t i = 0; i != n; ++i) {
        if (!mckl::internal::is_zero(static_cast<T>(r2[i]))) {
            f2 = std::max(f2, std::abs((r1[i] - static_cast<T>(r2[i])) /
                                  static_cast<T>(r2[i])));
        } else if (!mckl::internal::is_zero(r1[i])) {
            f2 = std::max(
                f2, std::abs((r1[i] - static_cast<T>(r2[i])) / r1[i]));
        }
    }
#endif

    e1 = std::max(e1, f1);
#if MCKL_HAS_BOOST
    e2 = std::max(e2, f2);
#else
    e2 = std::max(e2, f2);
#endif
}

inline void math_summary_sv(mckl::Vector<MathPerf> perf)
{
    const int nwid = 10;
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
    std::cout << std::setw(ewid) << std::right << "Err.Abs (S)";
#if MCKL_HAS_BOOST
    std::cout << std::setw(ewid) << std::right << "Err.ULP (S)";
#else
    std::cout << std::setw(ewid) << std::right << "Err.Rel (S)";
#endif
    std::cout << std::setw(ewid) << std::right << "Err.Abs (D)";
#if MCKL_HAS_BOOST
    std::cout << std::setw(ewid) << std::right << "Err.ULP (D)";
#else
    std::cout << std::setw(ewid) << std::right << "Err.Rel (D)";
#endif
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

#endif // MCKL_EXAMPLE_MATH_COMMON_HPP
