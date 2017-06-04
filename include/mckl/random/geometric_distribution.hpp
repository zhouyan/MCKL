//============================================================================
// MCKL/include/mckl/random/geometric_distribution.hpp
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

#ifndef MCKL_RANDOM_GEOMETRIC_DISTRIBUTION_HPP
#define MCKL_RANDOM_GEOMETRIC_DISTRIBUTION_HPP

#include <mckl/random/internal/common.hpp>
#include <mckl/random/u01_distribution.hpp>

namespace mckl {

namespace internal {

inline bool geometric_distribution_check_param(double p)
{
    return p > 0 && p <= 1;
}

template <std::size_t K, typename IntType, typename RNGType>
inline void geometric_distribution_impl(
    RNGType &rng, std::size_t n, IntType *r, double p)
{
    alignas(32) std::array<double, K> s;
    u01_oc_distribution(rng, n, s.data());
    log(n, s.data(), s.data());
    mul(n, 1 / std::log(1 - p), s.data(), s.data());
    floor(n, s.data(), s.data());
    for (std::size_t i = 0; i != n; ++i) {
        r[i] = ftoi<IntType>(s[i]);
    }
}

} // namespace mckl::internal

MCKL_DEFINE_RANDOM_DISTRIBUTION_BATCH_1(
    Geometric, geometric, InType, double, p)

/// \brief Geometric distribution
/// \ingroup Distribution
template <typename IntType>
class GeometricDistribution
{
    MCKL_DEFINE_RANDOM_DISTRIBUTION_ASSERT_INT_TYPE(Geometric, 16)
    MCKL_DEFINE_RANDOM_DISTRIBUTION_1(
        Geometric, geometric, IntType, double, p, 0.5)
    MCKL_DEFINE_RANDOM_DISTRIBUTION_MEMBER_1(double, lnpinv_)

  public:
    result_type min() const { return 0; }

    result_type max() const { return std::numeric_limits<IntType>::max(); }

    void reset() { lnpinv_ = 1 / std::log(1 - p()); }

  private:
    template <typename RNGType>
    result_type generate(RNGType &rng, const param_type &param)
    {
        U01OCDistribution<double> u01;

        const double lnpinv =
            param == param_ ? lnpinv_ : 1 / std::log(1 - param.p());
        const double r = std::floor(std::log(u01(rng)) * lnpinv);

        return internal::ftoi<IntType>(r);
    }
}; // class GeometricDistribution

MCKL_DEFINE_RANDOM_DISTRIBUTION_RAND(Geometric, IntType)

} // namespace mckl

#endif // MCKL_RANDOM_GEOMETRIC_DISTRIBUTION_HPP
