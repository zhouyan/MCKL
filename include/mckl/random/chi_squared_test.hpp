//============================================================================
// MCKL/include/mckl/random/chi_squared_test.hpp
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

#ifndef MCKL_RANDOM_CHI_SQUARED_TEST_HPP
#define MCKL_RANDOM_CHI_SQUARED_TEST_HPP

#include <mckl/random/internal/common.hpp>

namespace mckl {

/// \brief Tests based on the \f$\chi^2\f$-distribution
/// \ingroup RandomTest
template <typename Derived>
class ChiSquaredTest
{
  public:
    using result_type = double;

    bool pass(double alpha, result_type s) const
    {
        double p = cdf(s);

        return std::min(p, 1 - p) > 0.5 * alpha;
    }

    double pdf(result_type s) const
    {
        double k =
            0.5 * static_cast<const Derived *>(this)->degree_of_freedom();
        double lpdf = -k * const_ln_2<double>() - std::lgamma(k) +
            (k - 1) * std::log(s) - 0.5 * s;

        return std::exp(lpdf);
    }

    double cdf(result_type s) const
    {
        return gammap(
            0.5 * static_cast<const Derived *>(this)->degree_of_freedom(),
            0.5 * s);
    }

  protected:
    double stat(std::size_t m, const double *count, const double *np) const
    {
        return stat_dispatch(m, count, np);
    }

    double stat(std::size_t m, const double *count, double np) const
    {
        return stat_dispatch(m, count, np);
    }

  private:
    template <typename NPType>
    double stat_dispatch(std::size_t m, const double *count, NPType np) const
    {
        Vector<double> tmp(m);
        sub(m, count, np, tmp.data());
        sqr(m, tmp.data(), tmp.data());
        div(m, tmp.data(), np, tmp.data());

        return std::accumulate(tmp.begin(), tmp.end(), 0.0);
    }
}; // class ChiSquaredTest

} // namespace mckl

#endif // MCKL_RANDOM_CHI_SQUARED_TEST_HPP
