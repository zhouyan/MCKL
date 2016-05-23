//============================================================================
// MCKL/example/random/include/random_common.hpp
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

#ifndef MCKL_EXAMPLE_RANDOM_COMMON_HPP
#define MCKL_EXAMPLE_RANDOM_COMMON_HPP

#include <mckl/random/rng.hpp>
#include <mckl/utility/stop_watch.hpp>

#define MCKL_EXAMPLE_RANDOM_MAIN(prg, NVal, MVal)                             \
    int main(int argc, char **argv)                                           \
    {                                                                         \
        std::size_t N = NVal;                                                 \
        std::size_t M = MVal;                                                 \
        --argc;                                                               \
        ++argv;                                                               \
        if (argc > 0) {                                                       \
            std::size_t n = static_cast<std::size_t>(std::atoi(*argv));       \
            if (n != 0) {                                                     \
                N = n;                                                        \
                --argc;                                                       \
                ++argv;                                                       \
            }                                                                 \
        }                                                                     \
        if (argc > 0) {                                                       \
            std::size_t m = static_cast<std::size_t>(std::atoi(*argv));       \
            if (m != 0) {                                                     \
                M = m;                                                        \
                --argc;                                                       \
                ++argv;                                                       \
            }                                                                 \
        }                                                                     \
        random_##prg(N, M, argc, argv);                                       \
                                                                              \
        return 0;                                                             \
    }

template <typename>
inline std::string random_typename();

template <>
inline std::string random_typename<float>()
{
    return "float";
}

template <>
inline std::string random_typename<double>()
{
    return "double";
}

template <>
inline std::string random_typename<long double>()
{
    return "long double";
}

template <>
inline std::string random_typename<int>()
{
    return "int";
}

template <>
inline std::string random_typename<unsigned>()
{
    return "unsigned";
}

template <>
inline std::string random_typename<mckl::Closed>()
{
    return "Closed";
}

template <>
inline std::string random_typename<mckl::Open>()
{
    return "Open";
}

inline std::string random_pass(bool pass)
{
    return pass ? "Passed" : "Failed";
}

inline double random_erfcinv(double y)
{
    if (y >= 2)
        return -mckl::const_inf<double>();
    if (y <= 0)
        return mckl::const_inf<double>();

    static constexpr double a = 0.70771;
    static constexpr double b = 2.30753;
    static constexpr double c = 0.27061;
    static constexpr double d = 0.99229;
    static constexpr double e = 0.04481;
    static constexpr double f = 1.12837916709551257;

    double z = y < 1 ? y : 2 - y;
    double t = std::sqrt(-2 * std::log(0.5 * z));
    double x = -a * ((b + t * c) / (1 + t * (d + t * e)) - t);
    for (int i = 0; i != 2; ++i) {
        double err = std::erfc(x) - z;
        x += err / (f * std::exp(-(x * x)) - x * err);
    }

    return y < 1 ? x : -x;
}

inline double random_erfinv(double y) { return random_erfcinv(1 - y); }

static constexpr std::size_t random_gau_n = 18;

static constexpr double random_gau_y[] = {0.0021695375159141994,
    0.011413521097787704, 0.027972308950302116, 0.051727015600492421,
    0.082502225484340941, 0.12007019910960293, 0.16415283300752470,
    0.21442376986779355, 0.27051082840644336, 0.33199876341447887,
    0.39843234186401943, 0.46931971407375483, 0.54413605556657973,
    0.62232745288031077, 0.70331500465597174, 0.78649910768313447,
    0.87126389619061517, 0.95698180152629142};

static constexpr double random_gau_w[] = {0.0055657196642445571,
    0.012915947284065419, 0.020181515297735382, 0.027298621498568734,
    0.034213810770299537, 0.040875750923643261, 0.047235083490265582,
    0.053244713977759692, 0.058860144245324798, 0.064039797355015485,
    0.068745323835736408, 0.072941885005653087, 0.076598410645870640,
    0.079687828912071670, 0.082187266704339706, 0.084078218979661945,
    0.085346685739338721, 0.085983275670394821};

inline double random_gammap_approx(double a, double x, bool psig)
{
    double a1 = a - 1;
    double lna1 = std::log(a1);
    double sqrta1 = std::sqrt(a1);
    double xu = x > a1 ?
        std::max(a1 + 11.5 * sqrta1, x + 6 * sqrta1) :
        std::max(0.0, std::min(a1 - 7.5 * sqrta1, x - 5 * sqrta1));
    double sum = 0;
    for (std::size_t i = 0; i != random_gau_n; ++i) {
        double t = x + (xu - x) * random_gau_y[i];
        sum +=
            random_gau_w[i] * std::exp(-(t - a1) + a1 * (std::log(t) - lna1));
    }
    double ans = sum * (xu - x) * std::exp(a1 * (lna1 - 1.) - std::lgamma(a));

    return psig ? (x > a1 ? 1 - ans : -ans) : (x > a1 ? ans : 1 + ans);
}

inline double random_gammap_gser(double a, double x)
{
    static constexpr double eps = std::numeric_limits<double>::epsilon();

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

inline double random_gammap_gcf(double a, double x)
{
    static constexpr double eps = std::numeric_limits<double>::epsilon();
    static constexpr double fpmin = std::numeric_limits<double>::min() / eps;

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

inline double random_gammap(double a, double x)
{
    if (x < 0 || a <= 0)
        return mckl::const_nan<double>();
    if (!(x > 0 || x < 0))
        return 0;
    if (a > 100)
        return random_gammap_approx(a, x, true);
    if (x < a + 1)
        return random_gammap_gser(a, x);
    return 1 - random_gammap_gcf(a, x);
}

inline double random_gammapinv(double a, double y)
{
    static constexpr double eps = 1e-8;

    if (a <= 0)
        return mckl::const_nan<double>();
    if (y >= 1)
        return mckl::const_inf<double>();
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

        double err = random_gammap(a, x) - y;
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

inline double random_betai_acf(double a, double b, double x)
{
    static constexpr double eps = std::numeric_limits<double>::epsilon();
    static constexpr double fpmin = std::numeric_limits<double>::min() / eps;

    double qab = a + b;
    double qap = a + 1;
    double qam = a - 1;
    double c = 1;
    double d = 1 - qab * x / qap;
    if (std::abs(d) < fpmin)
        d = fpmin;
    d = 1 / d;
    double h = d;
    double m = 0;
    for (int i = 0; i != 10000; ++i) {
        m += 1;
        double m2 = 2 * m;
        double aa = m * (b - m) * x / ((qam + m2) * (a + m2));
        d = 1 + aa * d;
        if (std::abs(d) < fpmin)
            d = fpmin;
        c = 1 + aa / c;
        if (std::abs(c) < fpmin)
            c = fpmin;
        d = 1 / d;
        h *= d * c;
        aa = -(a + m) * (qab + m) * x / ((a + m2) * (qap + m2));
        d = 1 + aa * d;
        if (std::abs(d) < fpmin)
            d = fpmin;
        c = 1 + aa / c;
        if (std::abs(c) < fpmin)
            c = fpmin;
        d = 1 / d;
        double del = d * c;
        h *= del;
        if (std::abs(del - 1) <= eps)
            break;
    }

    return h;
}

inline double random_betai_approx(double a, double b, double x)
{
    double a1 = a - 1;
    double b1 = b - 1;
    double mu = a / (a + b);
    double lnmu = std::log(mu);
    double lnmuc = std::log(1 - mu);
    double t = std::sqrt(a * b / ((a + b) * (a + b) * (a + b + 1)));
    double xu = 0;
    if (x > a / (a + b)) {
        if (x >= 1)
            return 1;
        xu = std::min(1.0, std::max(mu + 10 * t, x + 5 * t));
    } else {
        if (x <= 0)
            return 0;
        xu = std::max(0.0, std::min(mu - 10 * t, x - 5 * t));
    }
    double sum = 0;
    for (std::size_t i = 0; i != random_gau_n; ++i) {
        t = x + (xu - x) * random_gau_y[i];
        sum += random_gau_w[i] * std::exp(a1 * (std::log(t) - lnmu) +
                                     b1 * (std::log(1 - t) - lnmuc));
    }
    double ans =
        sum * (xu - x) * std::exp(a1 * lnmu - std::lgamma(a) + b1 * lnmuc -
                             std::lgamma(b) + std::lgamma(a + b));

    return ans > 0 ? 1 - ans : -ans;
}

inline double random_betai(double a, double b, double x)
{
    if (a <= 0.0 || b <= 0.0)
        return mckl::const_nan<double>();
    if (x < 0.0 || x > 1.0)
        return mckl::const_nan<double>();
    if (!(x > 0 || x < 0))
        return x;
    if (!(x > 1 || x < 1))
        return x;
    if (a > 3000 && b > 3000)
        return random_betai_approx(a, b, x);

    double bt = std::exp(std::lgamma(a + b) - std::lgamma(a) - std::lgamma(b) +
        a * std::log(x) + b * std::log(1 - x));

    return x < (a + 1) / (a + b + 2) ?
        (bt * random_betai_acf(a, b, x) / a) :
        (1 - bt * random_betai_acf(b, a, 1 - x) / b);
}

inline double random_betaiinv(double a, double b, double y)
{
    if (y <= 0)
        return 0;
    if (y >= 1)
        return 1;

    double eps = 1e-8;
    double a1 = a - 1;
    double b1 = b - 1;
    double x = 0;
    if (a >= 1 && b >= 1) {
        double z = (y < 0.5) ? y : 1 - y;
        double t = std::sqrt(-2 * std::log(z));
        x = (2.30753 + t * 0.27061) / (1 + t * (0.99229 + t * 0.04481)) - t;
        if (y < 0.5)
            x = -x;
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
                        1 - pow(b * w * (1 - y), 1 / b);
    }
    double afac = -std::lgamma(a) - std::lgamma(b) + std::lgamma(a + b);
    for (int i = 0; i != 10; ++i) {
        if (!(x > 0 || x < 0))
            return x;
        if (!(x > 1 || x < 1))
            return x;
        double err = random_betai(a, b, x) - y;
        double t = std::exp(a1 * std::log(x) + b1 * std::log(1 - x) + afac);
        double u = err / t;
        x -= (t = u / (1 - 0.5 * std::min(1.0, u * (a1 / x - b1 / (1 - x)))));
        if (x <= 0)
            x = 0.5 * (x + t);
        if (x >= 1)
            x = 0.5 * (x + t + 1);
        if (std::abs(t) < eps * x && i > 0)
            break;
    }

    return x;
}

#endif // MCKL_EXAMPLE_RANDOM_COMMON_HPP
