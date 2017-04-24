//============================================================================
// MCKL/include/mckl/random/exponential_distribution.hpp
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

#ifndef MCKL_RANDOM_EXPONENTIAL_DISTRIBUTION_HPP
#define MCKL_RANDOM_EXPONENTIAL_DISTRIBUTION_HPP

#include <mckl/random/internal/common.hpp>
#include <mckl/random/u01_distribution.hpp>

namespace mckl
{

namespace internal
{

template <typename RealType>
inline bool exponential_distribution_check_param(RealType lambda)
{
    return lambda > 0;
}

template <std::size_t, typename RealType, typename RNGType>
inline void exponential_distribution_impl(
    RNGType &rng, std::size_t n, RealType *r, RealType lambda)
{
    u01_oc_distribution(rng, n, r);
    log(n, r, r);
    mul(n, -1 / lambda, r, r);
}

} // namespace mckl::internal

MCKL_DEFINE_RANDOM_DISTRIBUTION_BATCH_1(
    Exponential, exponential, RealType, RealType, lambda)

/// \brief Exponential distribution
/// \ingroup Distribution
template <typename RealType>
class ExponentialDistribution
{
    MCKL_DEFINE_RANDOM_DISTRIBUTION_ASSERT_REAL_TYPE(Exponential)
    MCKL_DEFINE_RANDOM_DISTRIBUTION_1(
        Exponential, exponential, RealType, result_type, lambda, 1)
    MCKL_DEFINE_RANDOM_DISTRIBUTION_MEMBER_0

  public:
    result_type min() const { return 0; }

    result_type max() const { return std::numeric_limits<result_type>::max(); }

    void reset() {}

  private:
    template <typename RNGType>
    result_type generate(RNGType &rng, const param_type &param)
    {
        U01OCDistribution<RealType> u01;

        return -std::log(u01(rng)) / param.lambda();
    }
}; // class ExponentialDistribution

MCKL_DEFINE_RANDOM_DISTRIBUTION_RAND(Exponential, RealType)

} // namespace mckl

#endif // MCKL_RANDOM_EXPONENTIAL_DISTRIBUTION_HPP
