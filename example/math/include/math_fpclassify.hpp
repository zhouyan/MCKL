//============================================================================
// MCKL/example/math/include/math_fpclassify.hpp
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

#ifndef MCKL_EXAMPLE_MATH_FPCLASSIFY_HPP
#define MCKL_EXAMPLE_MATH_FPCLASSIFY_HPP

#include <mckl/random/bernoulli_distribution.hpp>
#include <mckl/random/uniform_int_distribution.hpp>
#include <mckl/random/uniform_real_distribution.hpp>
#include "math_common.hpp"

template <typename T>
class MathFPUIntType;

template <>
class MathFPUIntType<float>
{
  public:
    using type = std::uint32_t;
}; // class MathFPUIntType

template <>
class MathFPUIntType<double>
{
  public:
    using type = std::uint64_t;
}; // class MathFPUIntType

template <typename T, typename RNGType>
inline T math_rand_normal(RNGType &rng)
{
    mckl::UniformRealDistribution<T> runif(
        static_cast<T>(-1e4), static_cast<T>(1e4));

    T f = runif(rng);
    if (f > 0)
        f += std::numeric_limits<T>::min();
    else
        f -= std::numeric_limits<T>::min();

    return f;
}

template <typename T, typename RNGType>
inline T math_rand_subnormal(RNGType &rng)
{
    mckl::U01OODistribution<T> r01;

    return r01(rng) * std::numeric_limits<T>::min();
}

template <typename T, typename RNGType>
inline T math_rand_zero(RNGType &rng)
{
    mckl::BernoulliDistribution<bool> rsign;

    return rsign(rng) ? mckl::const_zero<T>() : -mckl::const_zero<T>();
}

template <typename T, typename RNGType>
inline T math_rand_inf(RNGType &rng)
{
    mckl::BernoulliDistribution<bool> rsign;

    return rsign(rng) ? mckl::const_inf<T>() : -mckl::const_inf<T>();
}

template <typename T, typename RNGType>
inline T math_rand_nan(RNGType &rng)
{
    using U = typename MathFPUIntType<T>::type;

    mckl::UniformIntDistribution<U> rbit(
        0, static_cast<U>(1) << (std::numeric_limits<T>::digits - 4));
    mckl::BernoulliDistribution<bool> rquiet;

    union {
        T f;
        U u;
    };

    f = rquiet(rng) ? std::numeric_limits<T>::quiet_NaN() :
                      std::numeric_limits<T>::signaling_NaN();

    u |= rbit(rng);

    return f;
}

template <typename T, typename RNGType>
inline T math_rand_finite(RNGType &rng)
{
    mckl::UniformIntDistribution<int> rint(0, 2);
    T f = 0;
    int c = rint(rng);
    if (c == 0)
        f = math_rand_normal<T>(rng);
    if (c == 1)
        f = math_rand_subnormal<T>(rng);
    if (c == 2)
        f = math_rand_zero<T>(rng);

    return f;
}

template <typename T, typename RNGType>
inline T math_rand_not_normal(RNGType &rng)
{
    mckl::UniformIntDistribution<int> rint(0, 3);
    T f = 0;
    int c = rint(rng);
    if (c == 0)
        f = math_rand_subnormal<T>(rng);
    if (c == 1)
        f = math_rand_zero<T>(rng);
    if (c == 2)
        f = math_rand_inf<T>(rng);
    if (c == 3)
        f = math_rand_nan<T>(rng);

    return f;
}

template <typename T, typename RNGType>
inline T math_rand_not_subnormal(RNGType &rng)
{
    mckl::UniformIntDistribution<int> rint(0, 3);
    T f = 0;
    int c = rint(rng);
    if (c == 0)
        f = math_rand_normal<T>(rng);
    if (c == 1)
        f = math_rand_zero<T>(rng);
    if (c == 2)
        f = math_rand_inf<T>(rng);
    if (c == 3)
        f = math_rand_nan<T>(rng);

    return f;
}

template <typename T, typename RNGType>
inline T math_rand_not_zero(RNGType &rng)
{
    mckl::UniformIntDistribution<int> rint(0, 3);
    T f = 0;
    int c = rint(rng);
    if (c == 0)
        f = math_rand_normal<T>(rng);
    if (c == 1)
        f = math_rand_subnormal<T>(rng);
    if (c == 2)
        f = math_rand_inf<T>(rng);
    if (c == 3)
        f = math_rand_nan<T>(rng);

    return f;
}

template <typename T, typename RNGType>
inline T math_rand_not_inf(RNGType &rng)
{
    mckl::UniformIntDistribution<int> rint(0, 3);
    T f = 0;
    int c = rint(rng);
    if (c == 0)
        f = math_rand_normal<T>(rng);
    if (c == 1)
        f = math_rand_subnormal<T>(rng);
    if (c == 2)
        f = math_rand_zero<T>(rng);
    if (c == 3)
        f = math_rand_nan<T>(rng);

    return f;
}

template <typename T, typename RNGType>
inline T math_rand_not_nan(RNGType &rng)
{
    mckl::UniformIntDistribution<int> rint(0, 3);
    T f = 0;
    int c = rint(rng);
    if (c == 0)
        f = math_rand_normal<T>(rng);
    if (c == 1)
        f = math_rand_subnormal<T>(rng);
    if (c == 2)
        f = math_rand_zero<T>(rng);
    if (c == 3)
        f = math_rand_inf<T>(rng);

    return f;
}

template <typename T, typename RNGType>
inline T math_rand_not_finite(RNGType &rng)
{
    mckl::UniformIntDistribution<int> rint(0, 1);
    T f = 0;
    int c = rint(rng);
    if (c == 0)
        f = math_rand_inf<T>(rng);
    if (c == 1)
        f = math_rand_nan<T>(rng);

    return f;
}

#define MCKL_EXAMPLE_DEFINE_MATH_FPCLASSIFY_FIND(func, randtrue, randfalse)   \
    template <typename T>                                                     \
    inline std::pair<bool, bool> math_find_##func(                            \
        std::size_t N, std::size_t M, mckl::Vector<MathPerf> &perf)           \
    {                                                                         \
        mckl::Vector<T> r(N);                                                 \
        mckl::RNG rng;                                                        \
        mckl::BernoulliDistribution<bool> rtrue(0.3);                         \
        mckl::UniformIntDistribution<std::size_t> rsize(N / 2, N);            \
                                                                              \
        bool has_cycles = mckl::StopWatch::has_cycles();                      \
                                                                              \
        double c1 = has_cycles ? std::numeric_limits<double>::max() : 0;      \
        double c2 = has_cycles ? std::numeric_limits<double>::max() : 0;      \
                                                                              \
        bool pass1 = true;                                                    \
        bool pass2 = true;                                                    \
        for (std::size_t k = 0; k != 10; ++k) {                               \
            std::size_t n = 0;                                                \
            mckl::StopWatch watch1;                                           \
            mckl::StopWatch watch2;                                           \
            for (std::size_t i = 0; i != M; ++i) {                            \
                std::size_t K = rsize(rng);                                   \
                std::size_t index = K;                                        \
                for (std::size_t j = 0; j != K / 2; ++j)                      \
                    r[j] = randfalse<T>(rng);                                 \
                for (std::size_t j = K / 2; j != K; ++j) {                    \
                    if (rtrue(rng)) {                                         \
                        r[j] = randtrue<T>(rng);                              \
                        index = std::min(index, j);                           \
                    } else {                                                  \
                        r[j] = randfalse<T>(rng);                             \
                    }                                                         \
                }                                                             \
                                                                              \
                watch1.start();                                               \
                std::size_t n1 = mckl::find_##func<T>(K, r.data());           \
                watch1.stop();                                                \
                                                                              \
                watch2.start();                                               \
                std::size_t n2 = mckl::find_##func(K, r.data());              \
                watch2.stop();                                                \
                                                                              \
                pass1 = pass1 && n1 == index;                                 \
                pass2 = pass2 && n2 == index;                                 \
                n += index;                                                   \
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
        MathPerf result;                                                      \
        result.name = "find_" #func;                                          \
        result.c1 = c1;                                                       \
        result.c2 = c2;                                                       \
        perf.push_back(result);                                               \
                                                                              \
        return std::make_pair(pass1, pass2);                                  \
    }

#define MCKL_EXAMPLE_DEFINE_MATH_FPCLASSIFY_COUNT(func, randtrue, randfalse)  \
    template <typename T>                                                     \
    inline std::pair<bool, bool> math_count_##func(                           \
        std::size_t N, std::size_t M, mckl::Vector<MathPerf> &perf)           \
    {                                                                         \
        mckl::Vector<T> r(N);                                                 \
        mckl::RNG rng;                                                        \
        mckl::BernoulliDistribution<bool> rtrue(0.3);                         \
        mckl::UniformIntDistribution<std::size_t> rsize(N / 2, N);            \
                                                                              \
        bool has_cycles = mckl::StopWatch::has_cycles();                      \
                                                                              \
        double c1 = has_cycles ? std::numeric_limits<double>::max() : 0;      \
        double c2 = has_cycles ? std::numeric_limits<double>::max() : 0;      \
                                                                              \
        bool pass1 = true;                                                    \
        bool pass2 = true;                                                    \
        for (std::size_t k = 0; k != 10; ++k) {                               \
            std::size_t n = 0;                                                \
            mckl::StopWatch watch1;                                           \
            mckl::StopWatch watch2;                                           \
            for (std::size_t i = 0; i != M; ++i) {                            \
                std::size_t K = rsize(rng);                                   \
                n += K;                                                       \
                                                                              \
                std::size_t count = 0;                                        \
                for (std::size_t j = 0; j != K; ++j) {                        \
                    if (rtrue(rng)) {                                         \
                        ++count;                                              \
                        r[j] = randtrue<T>(rng);                              \
                    } else {                                                  \
                        r[j] = randfalse<T>(rng);                             \
                    }                                                         \
                }                                                             \
                                                                              \
                watch1.start();                                               \
                std::size_t n1 = mckl::count_##func<T>(K, r.data());          \
                watch1.stop();                                                \
                                                                              \
                watch2.start();                                               \
                std::size_t n2 = mckl::count_##func(K, r.data());             \
                watch2.stop();                                                \
                                                                              \
                pass1 = pass1 && n1 == count;                                 \
                pass2 = pass2 && n2 == count;                                 \
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
        MathPerf result;                                                      \
        result.name = "count_" #func;                                         \
        result.c1 = c1;                                                       \
        result.c2 = c2;                                                       \
        perf.push_back(result);                                               \
                                                                              \
        return std::make_pair(pass1, pass2);                                  \
    }

MCKL_EXAMPLE_DEFINE_MATH_FPCLASSIFY_FIND(
    normal, math_rand_normal, math_rand_not_normal)
MCKL_EXAMPLE_DEFINE_MATH_FPCLASSIFY_FIND(
    subnormal, math_rand_subnormal, math_rand_not_subnormal)
MCKL_EXAMPLE_DEFINE_MATH_FPCLASSIFY_FIND(
    zero, math_rand_zero, math_rand_not_zero)
MCKL_EXAMPLE_DEFINE_MATH_FPCLASSIFY_FIND(inf, math_rand_inf, math_rand_not_inf)
MCKL_EXAMPLE_DEFINE_MATH_FPCLASSIFY_FIND(nan, math_rand_nan, math_rand_not_nan)
MCKL_EXAMPLE_DEFINE_MATH_FPCLASSIFY_FIND(
    finite, math_rand_finite, math_rand_not_finite)

MCKL_EXAMPLE_DEFINE_MATH_FPCLASSIFY_COUNT(
    normal, math_rand_normal, math_rand_not_normal)
MCKL_EXAMPLE_DEFINE_MATH_FPCLASSIFY_COUNT(
    subnormal, math_rand_subnormal, math_rand_not_subnormal)
MCKL_EXAMPLE_DEFINE_MATH_FPCLASSIFY_COUNT(
    zero, math_rand_zero, math_rand_not_zero)
MCKL_EXAMPLE_DEFINE_MATH_FPCLASSIFY_COUNT(
    inf, math_rand_inf, math_rand_not_inf)
MCKL_EXAMPLE_DEFINE_MATH_FPCLASSIFY_COUNT(
    nan, math_rand_nan, math_rand_not_nan)
MCKL_EXAMPLE_DEFINE_MATH_FPCLASSIFY_COUNT(
    finite, math_rand_finite, math_rand_not_finite)

MCKL_EXAMPLE_DEFINE_MATH_FPCLASSIFY_FIND(
    not_normal, math_rand_not_normal, math_rand_normal)
MCKL_EXAMPLE_DEFINE_MATH_FPCLASSIFY_FIND(
    not_subnormal, math_rand_not_subnormal, math_rand_subnormal)
MCKL_EXAMPLE_DEFINE_MATH_FPCLASSIFY_FIND(
    not_zero, math_rand_not_zero, math_rand_zero)
MCKL_EXAMPLE_DEFINE_MATH_FPCLASSIFY_FIND(
    not_inf, math_rand_not_inf, math_rand_inf)
MCKL_EXAMPLE_DEFINE_MATH_FPCLASSIFY_FIND(
    not_nan, math_rand_not_nan, math_rand_nan)
MCKL_EXAMPLE_DEFINE_MATH_FPCLASSIFY_FIND(
    not_finite, math_rand_not_finite, math_rand_finite)

MCKL_EXAMPLE_DEFINE_MATH_FPCLASSIFY_COUNT(
    not_normal, math_rand_not_normal, math_rand_normal)
MCKL_EXAMPLE_DEFINE_MATH_FPCLASSIFY_COUNT(
    not_subnormal, math_rand_not_subnormal, math_rand_subnormal)
MCKL_EXAMPLE_DEFINE_MATH_FPCLASSIFY_COUNT(
    not_zero, math_rand_not_zero, math_rand_zero)
MCKL_EXAMPLE_DEFINE_MATH_FPCLASSIFY_COUNT(
    not_inf, math_rand_not_inf, math_rand_inf)
MCKL_EXAMPLE_DEFINE_MATH_FPCLASSIFY_COUNT(
    not_nan, math_rand_not_nan, math_rand_nan)
MCKL_EXAMPLE_DEFINE_MATH_FPCLASSIFY_COUNT(
    not_finite, math_rand_not_finite, math_rand_finite)

inline void math_fpclassify(std::size_t N, std::size_t M)
{
    mckl::Vector<MathPerf> perf;
    mckl::Vector<std::pair<bool, bool>> pass;

    pass.push_back(math_find_normal<float>(N, M, perf));
    pass.push_back(math_find_normal<double>(N, M, perf));
    pass.push_back(math_find_subnormal<float>(N, M, perf));
    pass.push_back(math_find_subnormal<double>(N, M, perf));
    pass.push_back(math_find_zero<float>(N, M, perf));
    pass.push_back(math_find_zero<double>(N, M, perf));
    pass.push_back(math_find_inf<float>(N, M, perf));
    pass.push_back(math_find_inf<double>(N, M, perf));
    pass.push_back(math_find_nan<float>(N, M, perf));
    pass.push_back(math_find_nan<double>(N, M, perf));
    pass.push_back(math_find_finite<float>(N, M, perf));
    pass.push_back(math_find_finite<double>(N, M, perf));

    pass.push_back(math_count_normal<float>(N, M, perf));
    pass.push_back(math_count_normal<double>(N, M, perf));
    pass.push_back(math_count_subnormal<float>(N, M, perf));
    pass.push_back(math_count_subnormal<double>(N, M, perf));
    pass.push_back(math_count_zero<float>(N, M, perf));
    pass.push_back(math_count_zero<double>(N, M, perf));
    pass.push_back(math_count_inf<float>(N, M, perf));
    pass.push_back(math_count_inf<double>(N, M, perf));
    pass.push_back(math_count_nan<float>(N, M, perf));
    pass.push_back(math_count_nan<double>(N, M, perf));
    pass.push_back(math_count_finite<float>(N, M, perf));
    pass.push_back(math_count_finite<double>(N, M, perf));

    pass.push_back(math_find_not_normal<float>(N, M, perf));
    pass.push_back(math_find_not_normal<double>(N, M, perf));
    pass.push_back(math_find_not_subnormal<float>(N, M, perf));
    pass.push_back(math_find_not_subnormal<double>(N, M, perf));
    pass.push_back(math_find_not_zero<float>(N, M, perf));
    pass.push_back(math_find_not_zero<double>(N, M, perf));
    pass.push_back(math_find_not_inf<float>(N, M, perf));
    pass.push_back(math_find_not_inf<double>(N, M, perf));
    pass.push_back(math_find_not_nan<float>(N, M, perf));
    pass.push_back(math_find_not_nan<double>(N, M, perf));
    pass.push_back(math_find_not_finite<float>(N, M, perf));
    pass.push_back(math_find_not_finite<double>(N, M, perf));

    pass.push_back(math_count_not_normal<float>(N, M, perf));
    pass.push_back(math_count_not_normal<double>(N, M, perf));
    pass.push_back(math_count_not_subnormal<float>(N, M, perf));
    pass.push_back(math_count_not_subnormal<double>(N, M, perf));
    pass.push_back(math_count_not_zero<float>(N, M, perf));
    pass.push_back(math_count_not_zero<double>(N, M, perf));
    pass.push_back(math_count_not_inf<float>(N, M, perf));
    pass.push_back(math_count_not_inf<double>(N, M, perf));
    pass.push_back(math_count_not_nan<float>(N, M, perf));
    pass.push_back(math_count_not_nan<double>(N, M, perf));
    pass.push_back(math_count_not_finite<float>(N, M, perf));
    pass.push_back(math_count_not_finite<double>(N, M, perf));

    const int nwid = 20;
    const int twid = 10;
    const int ewid = 10;
    const std::size_t lwid = nwid + twid * 4 + ewid * 4;

    std::cout << std::string(lwid, '=') << std::endl;
    std::cout << std::setw(nwid) << std::left << "Function";
    if (mckl::StopWatch::has_cycles()) {
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
        std::cout << std::setw(ewid) << std::right << "Pass (S)";
        std::cout << std::setw(ewid) << std::right << "Pass (SV)";
        std::cout << std::setw(ewid) << std::right << "Pass (D)";
        std::cout << std::setw(ewid) << std::right << "Pass (DV)";
    }
    std::cout << std::endl;

    std::cout << std::string(lwid, '-') << std::endl;

    for (std::size_t i = 0; i != perf.size() / 2; ++i) {
        std::stringstream ss1;
        std::stringstream ss2;
        std::stringstream ss3;
        std::stringstream ss4;
        ss1 << std::boolalpha << pass[i * 2].first;
        ss2 << std::boolalpha << pass[i * 2].second;
        ss3 << std::boolalpha << pass[i * 2 + 1].first;
        ss4 << std::boolalpha << pass[i * 2 + 1].second;
        std::cout << std::fixed << std::setprecision(2);
        std::cout << std::setw(nwid) << std::left << perf[i * 2].name;
        std::cout << std::setw(twid) << std::right << perf[i * 2].c1;
        std::cout << std::setw(twid) << std::right << perf[i * 2].c2;
        std::cout << std::setw(twid) << std::right << perf[i * 2 + 1].c1;
        std::cout << std::setw(twid) << std::right << perf[i * 2 + 1].c2;
        std::cout << std::setw(ewid) << std::right << ss1.str();
        std::cout << std::setw(ewid) << std::right << ss2.str();
        std::cout << std::setw(ewid) << std::right << ss3.str();
        std::cout << std::setw(ewid) << std::right << ss4.str();
        std::cout << std::endl;
    }

    std::cout << std::string(lwid, '-') << std::endl;
}

#endif // MCKL_EXAMPLE_MATH_FPCLASSIFY_HPP
