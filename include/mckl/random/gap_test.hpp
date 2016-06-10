//============================================================================
// MCKL/include/mckl/random/gap_test.hpp
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

#ifndef MCKL_RANDOM_GAP_TEST_HPP
#define MCKL_RANDOM_GAP_TEST_HPP

#include <mckl/random/internal/common.hpp>
#include <mckl/random/chi_squared_test.hpp>

namespace mckl
{

/// \brief Gap test
/// \ingroup RandomTest
///
/// \tparam NTrialMax The maximum number of trials
template <std::size_t NTrialMax = std::numeric_limits<std::size_t>::max()>
class GapTest : public ChiSquaredTest<GapTest<NTrialMax>>
{
    public:
    /// \brief Construct a Gap test
    ///
    /// \param n The number of gaps if `Cyclic` is false, otherwise the number
    /// of standard uniform random numbers
    /// \param alpha The lower bound of the gap
    /// \param beta The upper bound of the gap
    ///
    /// \details
    /// The Gap test may not terminate if the sequence is sufficient
    /// non-random. The test will be terminated if a gap with length larger
    /// than \f$n\f$ is found. In this case, zero will be returned as the test
    /// statistic.
    GapTest(std::size_t n, double alpha, double beta)
        : n_(n), alpha_(alpha), beta_(beta)
    {
        runtime_assert(
            alpha >= 0, "GapTest used with alpha smaller than zero");

        runtime_assert(beta <= 1, "GapTest used with beta larger than one");

        runtime_assert(
            alpha < beta, "GapTest used with alpha not less than beta");

        double p = beta - alpha;
        double q = 1 - p;
        double a = p;
        double b = q;
        double c = 5.0 / n;
        np_.push_back(a);
        while (true) {
            a *= q;
            b *= q;
            if (a < c || b < c)
                break;
            np_.push_back(a);
        }
        mul(np_.size(), static_cast<double>(n), np_.data(), np_.data());
        np_.push_back(n - std::accumulate(np_.begin(), np_.end(), 0.0));
    }

    template <typename RNGType, typename U01DistributionType>
    double operator()(RNGType &rng, U01DistributionType &u01)
    {
        using result_type = typename U01DistributionType::result_type;

        const std::size_t t = np_.size() - 1;
        count_.resize(np_.size());
        std::fill(count_.begin(), count_.end(), 0);

        std::size_t s = 0;
        while (s < n_) {
            std::size_t r = 0;
            while (true) {
                if (r >= NTrialMax)
                    return 0;
                result_type u = u01(rng);
                if (u >= alpha_ && u < beta_) {
                    count_[std::min(r, t)] += 1;
                    ++s;
                    break;
                }
                ++r;
            }
        }

        return this->stat(t + 1, count_.data(), np_.data());
    }

    double degree_of_freedom() const
    {
        return static_cast<double>(np_.size() - 1);
    }

    private:
    std::size_t n_;
    double alpha_;
    double beta_;
    Vector<double> np_;
    Vector<double> count_;
}; // class GapTest

} // namespace mckl

#endif // MCKL_RANDOM_GAP_TEST_HPP
