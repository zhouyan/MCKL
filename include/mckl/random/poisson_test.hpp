//============================================================================
// MCKL/include/mckl/random/poisson_test.hpp
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

#ifndef MCKL_RANDOM_POISSON_TEST_HPP
#define MCKL_RANDOM_POISSON_TEST_HPP

#include <mckl/random/internal/common.hpp>

namespace mckl {

/// \brief Tests based on the Poisson distribution
/// \ingroup RandomTest
template <typename Derived>
class PoissonTest
{
  public:
    using result_type = std::size_t;

    bool pass(double alpha, result_type s) const
    {
        double l = cdf(s);
        double r = s > 0 ? 1 - cdf(s - 1) : 1;
        double p = 0;
        if (r < l)
            p = r;
        else if (l < 0.5)
            p = 1 - l;
        else
            p = 0.5;

        return std::min(p, 1 - p) > 0.5 * alpha;
    }

    double pdf(result_type s) const
    {
        double mean = static_cast<const Derived *>(this)->mean();
        double lpdf = s * std::log(mean) - mean -
            std::lgamma(static_cast<double>(s + 1));

        return std::exp(lpdf);
    }

    double cdf(result_type s) const
    {
        double mean = static_cast<const Derived *>(this)->mean();

        return 1 - gammap(static_cast<double>(s + 1), mean);
    }
}; // class PoissonTest

} // namespace mckl

#endif // MCKL_RANDOM_POISSON_TEST_HPP
