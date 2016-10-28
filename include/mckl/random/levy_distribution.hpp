//============================================================================
// MCKL/include/mckl/random/levy_distribution.hpp
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

#ifndef MCKL_RANDOM_LEVY_DISTRIBUTION_HPP
#define MCKL_RANDOM_LEVY_DISTRIBUTION_HPP

#include <mckl/random/internal/common.hpp>
#include <mckl/random/normal_distribution.hpp>

namespace mckl
{

namespace internal
{

template <typename RealType>
inline bool levy_distribution_check_param(RealType, RealType b)
{
    return b > 0;
}

template <std::size_t, typename RealType, typename RNGType>
inline void levy_distribution_impl(
    RNGType &rng, std::size_t n, RealType *r, RealType a, RealType b)
{
    normal_distribution(
        rng, n, r, const_zero<RealType>(), const_one<RealType>());
    sqr(n, r, r);
    inv(n, r, r);
    fma(n, r, b, a, r);

    LevyDistribution<RealType> dist;
    for (std::size_t i = 0; i != n; ++i)
        if (!std::isfinite(r[i]))
            r[i] = dist(rng);
}

} // namespace mckl::internal

MCKL_DEFINE_RANDOM_DISTRIBUTION_BATCH_2(
    Levy, levy, RealType, RealType, a, RealType, b)

/// \brief Levy distribution
/// \ingroup Distribution
template <typename RealType>
class LevyDistribution
{
    MCKL_DEFINE_RANDOM_DISTRIBUTION_ASSERT_REAL_TYPE(Levy)
    MCKL_DEFINE_RANDOM_DISTRIBUTION_2(
        Levy, levy, RealType, result_type, a, 0, result_type, b, 1)
    MCKL_DEFINE_RANDOM_DISTRIBUTION_MEMBER_1(
        NormalDistribution<RealType>, normal_)

    public:
    result_type min() const { return a(); }

    result_type max() const { return std::numeric_limits<result_type>::max(); }

    void reset() { normal_ = NormalDistribution<RealType>(0, 1); }

    private:
    template <typename RNGType>
    result_type generate(RNGType &rng, const param_type &param)
    {
        result_type r = 0;
        while (internal::is_zero(r))
            r = normal_(rng);

        return param.a() + param.b() / (r * r);
    }
}; // class LevyDistribution

MCKL_DEFINE_RANDOM_DISTRIBUTION_RAND(Levy, RealType)

} // namespace mckl

#endif // MCKL_RANDOM_LEVY_DISTRIBUTION_HPP
