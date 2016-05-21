//============================================================================
// MCKL/include/mckl/random/bernoulli_distribution.hpp
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

#ifndef MCKL_RANDOM_BERNOULLI_DISTRIBUTION_HPP
#define MCKL_RANDOM_BERNOULLI_DISTRIBUTION_HPP

#include <mckl/random/internal/common.hpp>
#include <mckl/random/u01_distribution.hpp>

namespace mckl
{

namespace internal
{

inline bool bernoulli_distribution_check_param(double p)
{
    return p >= 0 && p <= 1;
}

template <std::size_t K, typename IntType, typename RNGType>
inline void bernoulli_distribution_impl(
    RNGType &rng, std::size_t n, IntType *r, double p)
{
    Array<double, K> s;
    u01_co_distribution(rng, n, s.data());
    std::fill_n(r, n, 0);
    for (std::size_t i = 0; i != n; ++i)
        if (s[i] < p)
            r[i] = 1;
}

MCKL_DEFINE_RANDOM_DISTRIBUTION_IMPL_1(Bernoulli, bernoulli, InType, double, p)

} // namespace mckl::internal

/// \brief Bernoulli distribution
/// \ingroup Distribution
template <typename IntType>
class BernoulliDistribution
{
    MCKL_DEFINE_RANDOM_DISTRIBUTION_ASSERT_INT_TYPE(Bernoulli, bool)
    MCKL_DEFINE_RANDOM_DISTRIBUTION_1(
        Bernoulli, bernoulli, IntType, double, p, 0.5)
    MCKL_DEFINE_RANDOM_DISTRIBUTION_MEMBER_0

    public:
    result_type min() const { return 0; }

    result_type max() const { return 1; }

    void reset() {}

    private:
    template <typename RNGType>
    result_type generate(RNGType &rng, const param_type &param)
    {
        U01CODistribution<double> u01;

        return u01(rng) < param.p() ? 1 : 0;
    }
}; // class BernoulliDistribution

MCKL_DEFINE_RANDOM_DISTRIBUTION_RAND(Bernoulli, IntType)

} // namespace mckl

#endif // MCKL_RANDOM_CAUCHY_DISTRIBUTION_HPP
