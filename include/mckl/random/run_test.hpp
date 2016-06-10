//============================================================================
// MCKL/include/mckl/random/run_test.hpp
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

#ifndef MCKL_RANDOM_RUN_TEST_HPP
#define MCKL_RANDOM_RUN_TEST_HPP

#include <mckl/random/internal/common.hpp>
#include <mckl/random/chi_squared_test.hpp>

namespace mckl
{

namespace internal
{

template <bool>
class RunTestCheck;

template <>
class RunTestCheck<true>
{
    public:
    template <typename ResultType>
    static bool eval(ResultType u, ResultType v)
    {
        return u > v;
    }
}; // class RunTestCheck

template <>
class RunTestCheck<false>
{
    public:
    template <typename ResultType>
    static bool eval(ResultType u, ResultType v)
    {
        return u < v;
    }
}; // class RunTestCheck

template <bool, bool>
class RunTestImpl;

template <bool Up>
class RunTestImpl<false, Up> : public ChiSquaredTest<RunTestImpl<false, Up>>
{
    public:
    RunTestImpl(std::size_t n) : n_(n) {}

    template <typename RNGType, typename U01Type>
    double operator()(RNGType &rng, U01Type &u01)
    {
        using result_type = typename U01Type::result_type;

        std::array<double, 6> count;
        std::fill(count.begin(), count.end(), 0);

        const std::size_t t = 5;
        std::size_t s = 0;
        result_type v = u01(rng);
        while (s < n_) {
            std::size_t r = 0;
            while (true) {
                ++r;
                ++s;
                result_type u = v;
                v = u01(rng);
                if (RunTestCheck<Up>::eval(u, v))
                    break;
                if (s == n_)
                    break;
            }
            count[std::min(r - 1, t)] += 1;
        }

        static const std::array<double, 36> a = {
            {4529.3536459687906886, 9044.9020785000773816,
                13567.945220995269025, 18091.267213474662936,
                22614.713867746890457, 27892.158836517771651,
                9044.9020785000773816, 18097.025434456939714,
                27139.455190145025166, 36186.649289760956163,
                45233.819845609141668, 55788.831062952543501,
                13567.945220995269025, 27139.455190145025166,
                40721.332025378211539, 54281.265638700444264,
                67852.044551180596848, 83684.570464780923545,
                18091.267213474662936, 36186.649289760956163,
                54281.265638700444264, 72413.608184786274684,
                90470.078876317476369, 111580.11003176274101,
                22614.713867746890457, 45233.819845609141668,
                67852.044551180596848, 90470.078876317476369,
                113261.81500341715234, 139475.55458338270984,
                27892.158836517771651, 55788.831062952543501,
                83684.570464780923545, 111580.11003176274101,
                139475.55458338270984, 172860.17010641275243}};

        static const std::array<double, 6> b = {
            {0.16666666666666666667, 0.20833333333333333333,
                0.091666666666666666667, 0.026388888888888888889,
                0.0057539682539682539683, 0.0011904761904761904762}};

        fma(6, -static_cast<double>(n_), b.data(), count.data(), count.data());

        std::array<double, 36> c;
        std::size_t k = 0;
        for (std::size_t i = 0; i != 6; ++i)
            for (std::size_t j = 0; j != 6; ++j)
                c[k++] = count[i] * count[j];
        mul(36, a.data(), c.data(), c.data());

        return std::accumulate(c.begin(), c.end(), 0.0) / (n_ - 6);
    }

    double degree_of_freedom() const { return 6; }

    private:
    std::size_t n_;
}; // class RunTestImpl

template <bool Up>
class RunTestImpl<true, Up> : public ChiSquaredTest<RunTestImpl<true, Up>>
{
    public:
    RunTestImpl(std::size_t n) : n_(n) {}

    template <typename RNGType, typename U01Type>
    double operator()(RNGType &rng, U01Type &u01)
    {
        using result_type = typename U01Type::result_type;

        const std::size_t t = 5;
        std::array<double, 6> count;
        std::fill(count.begin(), count.end(), 0);

        double s = 0;
        result_type v = u01(rng);
        while (s < n_) {
            std::size_t r = 0;
            while (true) {
                if (r > n_)
                    return 0;
                result_type u = v;
                v = u01(rng);
                if (RunTestCheck<Up>::eval(u, v)) {
                    count[std::min(r, t)] += 1;
                    ++s;
                    v = u01(rng);
                    break;
                }
                ++r;
            }
        }

        double n = static_cast<double>(n_);
        std::array<double, 6> np = {
            {n / 2, n / 3, n / 8, n / 30, n / 144, n / 720}};

        return this->stat(t + 1, count.data(), np.data());
    }

    double degree_of_freedom() const { return 5; }

    private:
    std::size_t n_;
}; // class RunTestImpl

} // namespace mckl::internal

/// \brief Run test
/// \ingroup RandomTest
///
/// \tparam Up If the run counts shall be upward
template <bool Up = true>
using RunTest = internal::RunTestImpl<false, Up>;

/// \brief Run test with independent tuples
/// \ingroup RandomTest
///
/// \tparam Up If the run counts shall be upward
template <bool Up = true>
using RunIndepTest = internal::RunTestImpl<true, Up>;

} // namespace mckl

#endif // MCKL_RANDOM_RUN_TEST_HPP
