//============================================================================
// MCKL/example/math/include/math_fpclassify.hpp
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
inline T math_random_nan(RNGType &rng)
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

#define MCKL_EXAMPLE_DEFINE_MATH_FPCLASSIFY(func, FPCLASS)                    \
    template <typename T>                                                     \
    inline std::pair<bool, bool> math_##func(                                 \
        std::size_t N, std::size_t M, mckl::Vector<MathPerf> &perf)           \
    {                                                                         \
        mckl::Vector<T> r(N);                                                 \
        mckl::RNG rng;                                                        \
        mckl::UniformIntDistribution<int> fpclass(0, 6);                      \
        mckl::UniformIntDistribution<std::size_t> rsize(N / 2, N);            \
        mckl::UniformRealDistribution<T> runif(-1e4, 1e4);                    \
        mckl::UniformRealDistribution<T> rnormal(1, 1e4);                     \
        mckl::U01Distribution<T> u01;                                         \
                                                                              \
        bool has_cycles = mckl::StopWatch::has_cycles();                      \
                                                                              \
        double c1 = has_cycles ? std::numeric_limits<double>::max() : 0;      \
        double c2 = has_cycles ? std::numeric_limits<double>::max() : 0;      \
        double c3 = has_cycles ? std::numeric_limits<double>::max() : 0;      \
        double c4 = has_cycles ? std::numeric_limits<double>::max() : 0;      \
                                                                              \
        bool pass1 = true;                                                    \
        bool pass2 = true;                                                    \
        for (std::size_t k = 0; k != 10; ++k) {                               \
            std::size_t n = 0;                                                \
            std::size_t m = 0;                                                \
            mckl::StopWatch watch1;                                           \
            mckl::StopWatch watch2;                                           \
            for (std::size_t i = 0; i != M; ++i) {                            \
                std::size_t K = rsize(rng);                                   \
                n += K;                                                       \
                                                                              \
                std::size_t index[6];                                         \
                std::size_t count[6];                                         \
                std::fill_n(index, 6, K);                                     \
                std::fill_n(count, 6, 0);                                     \
                index[0] = 0;                                                 \
                index[5] = 0;                                                 \
                count[0] = K / 2;                                             \
                count[5] = K / 2;                                             \
                mckl::rand(rng, rnormal, K / 2, r.data());                    \
                for (std::size_t j = K / 2; j != K; ++j) {                    \
                    switch (fpclass(rng)) {                                   \
                        case 1:                                               \
                            r[j] = std::numeric_limits<T>::min() * u01(rng);  \
                            index[1] = std::min(index[1], j);                 \
                            index[5] = std::min(index[5], j);                 \
                            ++count[1];                                       \
                            ++count[5];                                       \
                            break;                                            \
                        case 2:                                               \
                            r[j] = mckl::const_zero<T>();                     \
                            index[2] = std::min(index[2], j);                 \
                            index[5] = std::min(index[5], j);                 \
                            ++count[2];                                       \
                            ++count[5];                                       \
                            break;                                            \
                        case 3:                                               \
                            r[j] = -mckl::const_zero<T>();                    \
                            index[2] = std::min(index[2], j);                 \
                            index[5] = std::min(index[5], j);                 \
                            ++count[2];                                       \
                            ++count[5];                                       \
                            break;                                            \
                        case 4:                                               \
                            r[j] = mckl::const_inf<T>();                      \
                            index[3] = std::min(index[3], j);                 \
                            ++count[3];                                       \
                            break;                                            \
                        case 5:                                               \
                            r[j] = -mckl::const_inf<T>();                     \
                            index[3] = std::min(index[3], j);                 \
                            ++count[3];                                       \
                            break;                                            \
                        case 6:                                               \
                            r[j] = math_random_nan<T>(rng);                   \
                            index[4] = std::min(index[4], j);                 \
                            ++count[4];                                       \
                            break;                                            \
                        default:                                              \
                            r[j] = runif(rng);                                \
                            if (r[j] > 0)                                     \
                                r[j] += 2 * std::numeric_limits<T>::min();    \
                            else                                              \
                                r[j] -= 2 * std::numeric_limits<T>::min();    \
                            index[0] = std::min(index[0], j);                 \
                            index[5] = std::min(index[5], j);                 \
                            ++count[0];                                       \
                            ++count[5];                                       \
                            break;                                            \
                    }                                                         \
                }                                                             \
                                                                              \
                watch1.start();                                               \
                std::size_t n1 = mckl::func<T>(K, r.data());                  \
                watch1.stop();                                                \
                                                                              \
                watch2.start();                                               \
                std::size_t n2 = mckl::func(K, r.data());                     \
                watch2.stop();                                                \
                                                                              \
                pass1 =                                                       \
                    pass1 && (n1 == index[FPCLASS] || n1 == count[FPCLASS]);  \
                pass2 =                                                       \
                    pass2 && (n2 == index[FPCLASS] || n2 == count[FPCLASS]);  \
                                                                              \
                m += (n1 + 1);                                                \
            }                                                                 \
            if (has_cycles) {                                                 \
                c1 = std::min(c1, 1.0 * watch1.cycles() / m);                 \
                c2 = std::min(c2, 1.0 * watch2.cycles() / m);                 \
                c3 = std::min(c3, 1.0 * watch1.cycles() / n);                 \
                c4 = std::min(c4, 1.0 * watch2.cycles() / n);                 \
            } else {                                                          \
                c1 = std::max(c1, m / watch1.seconds() * 1e-6);               \
                c2 = std::max(c2, m / watch2.seconds() * 1e-6);               \
                c3 = std::max(c3, n / watch1.seconds() * 1e-6);               \
                c4 = std::max(c4, n / watch2.seconds() * 1e-6);               \
            }                                                                 \
        }                                                                     \
                                                                              \
        MathPerf result;                                                      \
        result.name = #func;                                                  \
        result.c1 = c1;                                                       \
        result.c2 = c2;                                                       \
        result.c3 = c3;                                                       \
        result.c4 = c4;                                                       \
        perf.push_back(result);                                               \
                                                                              \
        return std::make_pair(pass1, pass2);                                  \
    }

MCKL_EXAMPLE_DEFINE_MATH_FPCLASSIFY(find_normal, 0)
MCKL_EXAMPLE_DEFINE_MATH_FPCLASSIFY(find_subnormal, 1)
MCKL_EXAMPLE_DEFINE_MATH_FPCLASSIFY(find_zero, 2)
MCKL_EXAMPLE_DEFINE_MATH_FPCLASSIFY(find_inf, 3)
MCKL_EXAMPLE_DEFINE_MATH_FPCLASSIFY(find_nan, 4)
MCKL_EXAMPLE_DEFINE_MATH_FPCLASSIFY(find_finite, 5)
MCKL_EXAMPLE_DEFINE_MATH_FPCLASSIFY(count_normal, 0)
MCKL_EXAMPLE_DEFINE_MATH_FPCLASSIFY(count_subnormal, 1)
MCKL_EXAMPLE_DEFINE_MATH_FPCLASSIFY(count_zero, 2)
MCKL_EXAMPLE_DEFINE_MATH_FPCLASSIFY(count_inf, 3)
MCKL_EXAMPLE_DEFINE_MATH_FPCLASSIFY(count_nan, 4)
MCKL_EXAMPLE_DEFINE_MATH_FPCLASSIFY(count_finite, 5)

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

    const int nwid = 15;
    const int twid = 10;
    const int ewid = 12;
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
        if (i <= 5) {
            std::cout << std::setw(twid) << std::right << perf[i * 2].c1;
            std::cout << std::setw(twid) << std::right << perf[i * 2].c2;
            std::cout << std::setw(twid) << std::right << perf[i * 2 + 1].c1;
            std::cout << std::setw(twid) << std::right << perf[i * 2 + 1].c2;
        } else {
            std::cout << std::setw(twid) << std::right << perf[i * 2].c3;
            std::cout << std::setw(twid) << std::right << perf[i * 2].c4;
            std::cout << std::setw(twid) << std::right << perf[i * 2 + 1].c3;
            std::cout << std::setw(twid) << std::right << perf[i * 2 + 1].c4;
        }
        std::cout << std::setw(ewid) << std::right << ss1.str();
        std::cout << std::setw(ewid) << std::right << ss2.str();
        std::cout << std::setw(ewid) << std::right << ss3.str();
        std::cout << std::setw(ewid) << std::right << ss4.str();
        std::cout << std::endl;
    }

    std::cout << std::string(lwid, '-') << std::endl;
}

#endif // MCKL_EXAMPLE_MATH_FPCLASSIFY_HPP
