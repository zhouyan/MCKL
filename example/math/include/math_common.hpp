//============================================================================
// MCKL/example/math/include/math_common.hpp
//----------------------------------------------------------------------------
// MCKL: Monte Carlo Kernel Library
//----------------------------------------------------------------------------
// Copyright (c) 2013-2017, Yan Zhou
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

#include <mckl/math.hpp>
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
}; // class MathPerf

template <typename T, typename CharT, typename Traits>
inline void math_val(T x, std::basic_ostream<CharT, Traits> &os)
{
    std::string prefix;
    if (std::is_same<T, float>::value)
        prefix = "FLT_";
    if (std::is_same<T, double>::value)
        prefix = "DBL_";
    if (std::is_same<T, long double>::value)
        prefix = "LDBL_";

    if (!std::isfinite(x)) {
        os << x;
        return;
    }

    if (x == 0) {
        os << x;
        return;
    }

    constexpr T fmin = std::numeric_limits<T>::min();
    constexpr T fmax = std::numeric_limits<T>::max();
    constexpr T feps = std::numeric_limits<T>::epsilon();

    if (x == -fmin) {
        os << "-" + prefix + "MIN";
        return;
    }

    if (x == fmin) {
        os << prefix + "MIN";
        return;
    }

    if (x == -fmax) {
        os << "-" + prefix + "MAX";
        return;
    }

    if (x == fmax) {
        os << prefix + "MAX";
        return;
    }

    if (x == -feps) {
        os << "-" + prefix + "EPS";
        return;
    }

    if (x == feps) {
        os << prefix + "EPS";
        return;
    }

    const T nmin = x / fmin;
    const T nmax = x / fmax;
    const T neps = x / feps;

    std::stringstream ss;
    ss << std::setprecision(2);
    if (std::abs(nmin) > static_cast<T>(0.1) && std::abs(nmin) < 10) {
        ss << nmin << ' ' << prefix << "MIN";
        os << ss.str();
        return;
    }

    if (std::abs(nmax) > static_cast<T>(0.1)) {
        ss << nmax << ' ' << prefix << "max";
        os << ss.str();
        return;
    }

    if (std::abs(neps) > static_cast<T>(0.1) && std::abs(neps) < 10) {
        ss << neps << ' ' << prefix << "eps";
        os << ss.str();
        return;
    }

    os << x;
}

inline std::string math_error()
{
#if MCKL_HAS_BOOST
    return "ULP";
#else
    return "Err.Rel";
#endif
}

template <typename T, typename U>
inline T math_error(T a, U r)
{
    T b = static_cast<T>(r);

    if (std::isfinite(a) && std::isfinite(b)) {
#if MCKL_HAS_BOOST
        return std::abs(boost::math::float_distance(a, b));
#else
        T err_abs = std::abs(a - b);
        T err_rel = std::abs(err_abs / b);

        return std::min(err_abs, err_rel);
#endif
    }

    if (std::isnan(a) && std::isnan(b))
        return 0;
    if (std::isnan(a))
        return std::abs(a);
    if (std::isnan(b))
        return std::abs(b);

    if (!std::isfinite(a) && !std::isfinite(b)) {
        if (a < 0 && b < 0)
            return 0;
        if (a > 0 && b > 0)
            return 0;
        return std::abs(a);
    }
    if (!std::isfinite(a))
        return std::abs(a);
    if (!std::isfinite(b))
        return std::abs(b);

    return 0;
}

template <typename T, typename U>
inline void math_error(std::size_t n, const T *r1, const U *r2, T &e)
{
    mckl::Vector<T> s(n);
    std::copy_n(r2, n, s.data());
    T f = 0;
    for (std::size_t i = 0; i != n; ++i)
        f = std::max(f, math_error(r1[i], s[i]));
    e = std::max(e, f);
}

template <typename T, typename U>
inline void math_error(std::size_t n, const T *r1, const U *r2, T &e1, T &e2)
{
    T f1 = 0;
    T f2 = 0;
    mckl::Vector<T> s(n);
    std::copy_n(r2, n, s.data());
    for (std::size_t i = 0; i != n; ++i)
        f1 = std::max(f1, std::abs(r1[i] - static_cast<T>(r2[i])));
    for (std::size_t i = 0; i != n; ++i)
        f2 = std::max(f2, math_error(r1[i], s[i]));
    e1 = std::max(e1, f1);
    e2 = std::max(e2, f2);
}

#endif // MCKL_EXAMPLE_MATH_COMMON_HPP
