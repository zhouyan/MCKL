//============================================================================
// MCKL/include/mckl/random/logistic_distribution.hpp
//----------------------------------------------------------------------------
//                         MCKL: Monte Carlo Kernel Library
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

#ifndef MCKL_RANDOM_LOGISTIC_DISTRIBUTION_HPP
#define MCKL_RANDOM_LOGISTIC_DISTRIBUTION_HPP

#include <mckl/random/internal/common.hpp>
#include <mckl/random/u01_distribution.hpp>

namespace mckl
{

namespace internal
{

template <typename RealType>
inline bool logistic_distribution_check_param(RealType, RealType b)
{
    return b > 0;
}

} // namespace mckl::internal

/// \brief Logistic distribution
/// \ingroup Distribution
template <typename RealType>
class LogisticDistribution
{
    MCKL_DEFINE_RANDOM_DISTRIBUTION_2(Logistic, logistic, a, 0, b, 1)
    MCKL_DEFINE_RANDOM_DISTRIBUTION_MEMBER_0

    public:
    result_type min() const
    {
        return std::numeric_limits<result_type>::lowest();
    }

    result_type max() const { return std::numeric_limits<result_type>::max(); }

    void reset() {}

    private:
    template <typename RNGType>
    result_type generate(RNGType &rng, const param_type &param)
    {
        U01OODistribution<RealType> u01;
        result_type u = u01(rng);

        return param.a() + param.b() * std::log(u / (1 - u));
    }
}; // class LogisticDistribution

namespace internal
{

template <std::size_t K, typename RealType, typename RNGType>
inline void logistic_distribution_impl(
    RNGType &rng, std::size_t n, RealType *r, RealType a, RealType b)
{
    Array<RealType, K> s;
    u01_oo_distribution(rng, n, r);
    sub(n, const_one<RealType>(), r, s.data());
    div(n, r, s.data(), r);
    log(n, r, r);
    fma(n, r, b, a, r);
}

} // namespace mckl::internal

/// \brief Generating logistic random variates
/// \ingroup Distribution
MCKL_DEFINE_RANDOM_DISTRIBUTION_IMPL_2(logistic, a, b)
MCKL_DEFINE_RANDOM_DISTRIBUTION_RAND_2(Logistic, logistic, a, b)

} // namespace mckl

#endif // MCKL_RANDOM_LOGISTIC_DISTRIBUTION_HPP