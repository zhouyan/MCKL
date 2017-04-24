//============================================================================
// MCKL/include/mckl/math/gamma.hpp
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

#ifndef MCKL_MATH_GAMMA_HPP
#define MCKL_MATH_GAMMA_HPP

#include <mckl/internal/config.h>
#include <mckl/math/constants.hpp>

#include <algorithm>
#include <cmath>

namespace mckl {

namespace internal {

static constexpr std::size_t gammap_approx_n = 18;

static constexpr double gammap_approx_y[] = {0.0021695375159141994,
    0.011413521097787704, 0.027972308950302116, 0.051727015600492421,
    0.082502225484340941, 0.12007019910960293, 0.16415283300752470,
    0.21442376986779355, 0.27051082840644336, 0.33199876341447887,
    0.39843234186401943, 0.46931971407375483, 0.54413605556657973,
    0.62232745288031077, 0.70331500465597174, 0.78649910768313447,
    0.87126389619061517, 0.95698180152629142};

static constexpr double gammap_approx_w[] = {0.0055657196642445571,
    0.012915947284065419, 0.020181515297735382, 0.027298621498568734,
    0.034213810770299537, 0.040875750923643261, 0.047235083490265582,
    0.053244713977759692, 0.058860144245324798, 0.064039797355015485,
    0.068745323835736408, 0.072941885005653087, 0.076598410645870640,
    0.079687828912071670, 0.082187266704339706, 0.084078218979661945,
    0.085346685739338721, 0.085983275670394821};

inline double gammap_approx(double a, double x, bool psig)
{
    double a1 = a - 1;
    double lna1 = std::log(a1);
    double sqrta1 = std::sqrt(a1);
    double xu = x > a1 ?
        std::max(a1 + 11.5 * sqrta1, x + 6 * sqrta1) :
        std::max(0.0, std::min(a1 - 7.5 * sqrta1, x - 5 * sqrta1));
    double sum = 0;
    for (std::size_t i = 0; i != gammap_approx_n; ++i) {
        double t = x + (xu - x) * gammap_approx_y[i];
        sum += gammap_approx_w[i] *
            std::exp(-(t - a1) + a1 * (std::log(t) - lna1));
    }
    double ans = sum * (xu - x) * std::exp(a1 * (lna1 - 1.) - std::lgamma(a));

    return psig ? (x > a1 ? 1 - ans : -ans) : (x > a1 ? ans : 1 + ans);
}

inline double gammap_gser(double a, double x)
{
    constexpr double eps = std::numeric_limits<double>::epsilon();

    double ap = a;
    double del = 1 / a;
    double sum = del;
    while (true) {
        ap += 1;
        del *= x / ap;
        sum += del;
        if (std::abs(del) < std::abs(sum) * eps)
            return sum * std::exp(-x + a * std::log(x) - std::lgamma(a));
    }
}

inline double gammap_gcf(double a, double x)
{
    constexpr double eps = std::numeric_limits<double>::epsilon();
    constexpr double fpmin = std::numeric_limits<double>::min() / eps;

    double b = x + 1 - a;
    double c = 1.0 / fpmin;
    double d = 1 / b;
    double h = d;
    double n = 0;
    while (true) {
        n += 1;
        double an = -n * (n - a);
        b += 2;
        d = an * d + b;
        if (std::abs(d) < fpmin)
            d = fpmin;
        c = b + an / c;
        if (std::abs(c) < fpmin)
            c = fpmin;
        d = 1 / d;
        double del = d * c;
        h *= del;
        if (std::abs(del - 1) <= eps)
            break;
    }

    return std::exp(-x + a * std::log(x) - std::lgamma(a)) * h;
}

} // namespace mckl::internal

/// \brief Regularized lower incomplete Gamma function
/// \ingroup Special
inline double gammap(double a, double x)
{
    if (x < 0 || a <= 0)
        return const_nan<double>();
    if (!(x > 0 || x < 0))
        return 0;
    if (a > 100)
        return internal::gammap_approx(a, x, true);
    if (x < a + 1)
        return internal::gammap_gser(a, x);
    return 1 - internal::gammap_gcf(a, x);
}

/// \brief Inverse regularized lower incomplete Gamma function
/// \ingroup Special
inline double gammapinv(double a, double y)
{
    constexpr double eps = 1e-8;

    if (a <= 0)
        return const_nan<double>();
    if (y >= 1)
        return const_inf<double>();
    if (y <= 0)
        return 0;

    const double a1 = a - 1;
    const double gln = std::lgamma(a);

    double x = 0;
    double lna1 = 0;
    double afrac = 0;
    if (a > 1) {
        lna1 = std::log(a1);
        afrac = std::exp(a1 * (lna1 - 1) - gln);
        double z = y < 0.5 ? y : 1 - y;
        double t = std::sqrt(-2 * std::log(z));
        x = (2.30753 + t * 0.27061) / (1 + t * (0.99229 + t * 0.04481)) - t;
        if (y < 0.5)
            x = -x;
        x = std::max(
            1e-3, a * std::pow(1 - 1 / (9 * a) - x / (3 * std::sqrt(a)), 3));
    } else {
        double t = 1 - a * (0.253 + a * 0.12);
        x = y < t ? std::pow(y / t, 1 / a) :
                    1 - std::log(1 - (y - t) / (1 - t));
    }

    for (int i = 0; i != 12; ++i) {
        if (x <= 0)
            return 0;

        double err = gammap(a, x) - y;
        double t = a > 1 ?
            afrac * std::exp(-(x - a1) + a1 * (std::log(x) - lna1)) :
            std::exp(-x + a1 * std::log(x) - gln);
        double u = err / t;
        t = u / (1 - 0.5 * std::min(1.0, u * (a1 / x - 1)));
        x -= t;
        if (x <= 0)
            x = 0.5 * (x + t);

        if (std::abs(t) < eps * x)
            return x;
    }

    return x;
}

} // namespace mckl

#endif // MCKL_MATH_GAMMA_HPP
