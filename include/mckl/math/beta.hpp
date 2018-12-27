//============================================================================
// MCKL/include/mckl/math/beta.hpp
//----------------------------------------------------------------------------
// MCKL: Monte Carlo Kernel Library
//----------------------------------------------------------------------------
// Copyright (c) 2013-2018, Yan Zhou
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

#ifndef MCKL_MATH_BETA_HPP
#define MCKL_MATH_BETA_HPP

#include <mckl/internal/config.h>
#include <mckl/math/constants.hpp>
#include <mckl/math/gamma.hpp>

#include <algorithm>
#include <limits>

namespace mckl {

namespace internal {

inline double betai_acf(double a, double b, double x)
{
    constexpr double eps = std::numeric_limits<double>::epsilon();
    constexpr double fpmin = std::numeric_limits<double>::min() / eps;

    double qab = a + b;
    double qap = a + 1;
    double qam = a - 1;
    double c = 1;
    double d = 1 - qab * x / qap;
    if (std::abs(d) < fpmin) {
        d = fpmin;
    }
    d = 1 / d;
    double h = d;
    double m = 0;
    for (int i = 0; i != 10000; ++i) {
        m += 1;
        double m2 = 2 * m;
        double aa = m * (b - m) * x / ((qam + m2) * (a + m2));
        d = 1 + aa * d;
        if (std::abs(d) < fpmin) {
            d = fpmin;
        }
        c = 1 + aa / c;
        if (std::abs(c) < fpmin) {
            c = fpmin;
        }
        d = 1 / d;
        h *= d * c;
        aa = -(a + m) * (qab + m) * x / ((a + m2) * (qap + m2));
        d = 1 + aa * d;
        if (std::abs(d) < fpmin) {
            d = fpmin;
        }
        c = 1 + aa / c;
        if (std::abs(c) < fpmin) {
            c = fpmin;
        }
        d = 1 / d;
        double del = d * c;
        h *= del;
        if (std::abs(del - 1) <= eps) {
            break;
        }
    }

    return h;
}

inline double betai_approx(double a, double b, double x)
{
    double a1 = a - 1;
    double b1 = b - 1;
    double mu = a / (a + b);
    double lnmu = std::log(mu);
    double lnmuc = std::log(1 - mu);
    double t = std::sqrt(a * b / ((a + b) * (a + b) * (a + b + 1)));
    double xu = 0;
    if (x > a / (a + b)) {
        if (x >= 1) {
            return 1;
        }
        xu = std::min(1.0, std::max(mu + 10 * t, x + 5 * t));
    } else {
        if (x <= 0) {
            return 0;
        }
        xu = std::max(0.0, std::min(mu - 10 * t, x - 5 * t));
    }
    double sum = 0;
    for (std::size_t i = 0; i != gammap_approx_n; ++i) {
        t = x + (xu - x) * gammap_approx_y[i];
        sum += gammap_approx_w[i] *
            std::exp(
                a1 * (std::log(t) - lnmu) + b1 * (std::log(1 - t) - lnmuc));
    }
    double ans = sum * (xu - x) *
        std::exp(a1 * lnmu - std::lgamma(a) + b1 * lnmuc - std::lgamma(b) +
            std::lgamma(a + b));

    return ans > 0 ? 1 - ans : -ans;
}

} // namespace internal

/// \brief Regularized incomplete Beta function
/// \ingroup Special
inline double betai(double a, double b, double x)
{
    if (a <= 0.0 || b <= 0.0) {
        return mckl::const_nan<double>();
    }
    if (x < 0.0 || x > 1.0) {
        return mckl::const_nan<double>();
    }
    if (!(x > 0 || x < 0)) {
        return x;
    }
    if (!(x > 1 || x < 1)) {
        return x;
    }
    if (a > 3000 && b > 3000) {
        return internal::betai_approx(a, b, x);
    }

    double bt = std::exp(std::lgamma(a + b) - std::lgamma(a) - std::lgamma(b) +
        a * std::log(x) + b * std::log(1 - x));

    return x < (a + 1) / (a + b + 2) ?
        (bt * internal::betai_acf(a, b, x) / a) :
        (1 - bt * internal::betai_acf(b, a, 1 - x) / b);
}

/// \brief Inverse regularized incomplete Beta function
/// \ingroup Special
inline double betaiinv(double a, double b, double y)
{
    if (y <= 0) {
        return 0;
    }
    if (y >= 1) {
        return 1;
    }

    double eps = 1e-8;
    double a1 = a - 1;
    double b1 = b - 1;
    double x = 0;
    if (a >= 1 && b >= 1) {
        double z = (y < 0.5) ? y : 1 - y;
        double t = std::sqrt(-2 * std::log(z));
        x = (2.30753 + t * 0.27061) / (1 + t * (0.99229 + t * 0.04481)) - t;
        if (y < 0.5) {
            x = -x;
        }
        double al = (x * x - 3) / 6;
        double h = 2 / (1 / (2 * a - 1) + 1 / (2 * b - 1));
        double w = (x * std::sqrt(al + h) / h) -
            (1 / (2 * b - 1) - 1 / (2 * a - 1)) * (al + 5 / 6 - 2 / (3 * h));
        x = a / (a + b * std::exp(2 * w));
    } else {
        double lna = std::log(a / (a + b));
        double lnb = std::log(b / (a + b));
        double t = std::exp(a * lna) / a;
        double u = std::exp(b * lnb) / b;
        double w = t + u;
        x = y < t / w ? std::pow(a * w * y, 1 / a) :
                        1 - std::pow(b * w * (1 - y), 1 / b);
    }
    double afac = -std::lgamma(a) - std::lgamma(b) + std::lgamma(a + b);
    for (int i = 0; i != 10; ++i) {
        if (!(x > 0 || x < 0)) {
            return x;
        }
        if (!(x > 1 || x < 1)) {
            return x;
        }
        double err = betai(a, b, x) - y;
        double t = std::exp(a1 * std::log(x) + b1 * std::log(1 - x) + afac);
        double u = err / t;
        x -= (t = u / (1 - 0.5 * std::min(1.0, u * (a1 / x - b1 / (1 - x)))));
        if (x <= 0) {
            x = 0.5 * (x + t);
        }
        if (x >= 1) {
            x = 0.5 * (x + t + 1);
        }
        if (std::abs(t) < eps * x && i > 0) {
            break;
        }
    }

    return x;
}

} // namespace mckl

#endif // MCKL_MATH_BETA_HPP
