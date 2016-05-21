//============================================================================
// MCKL/include/mckl/random/chi_squared_distribution.hpp
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

#ifndef MCKL_RANDOM_CHI_SQUARED_DISTRIBUTION_HPP
#define MCKL_RANDOM_CHI_SQUARED_DISTRIBUTION_HPP

#include <mckl/random/internal/common.hpp>
#include <mckl/random/gamma_distribution.hpp>

namespace mckl
{

namespace internal
{

template <typename RealType>
inline bool chi_squared_distribution_check_param(RealType n)
{
    return n > 0;
}

template <typename RealType, typename RNGType>
inline void chi_squared_distribution(
    RNGType &rng, std::size_t n, RealType *r, RealType df)
{
    gamma_distribution(rng, n, r, df / 2, static_cast<RealType>(2));
}

template <typename RealType, typename RNGType>
inline void chi_squared_distribution(RNGType &rng, std::size_t n, RealType *r,
    const typename ChiSquaredDistribution<RealType>::param_type &param)
{
    chi_squared_distribution(rng, n, r, param.n());
}

} // namespace mckl::internal

/// \brief The \f$\chi^2\f$ distribution
/// \ingroup Distribution
template <typename RealType>
class ChiSquaredDistribution
{
    MCKL_DEFINE_RANDOM_DISTRIBUTION_ASSERT_REAL_TYPE(ChiSquared)
    MCKL_DEFINE_RANDOM_DISTRIBUTION_1(
        ChiSquared, chi_squared, RealType, result_type, n, 1)
    MCKL_DEFINE_RANDOM_DISTRIBUTION_MEMBER_1(
        GammaDistribution<RealType>, gamma_)

    public:
    result_type min() const { return 0; }

    result_type max() const { return std::numeric_limits<result_type>::max(); }

    void reset() { gamma_ = GammaDistribution<RealType>(n() / 2, 2); }

    private:
    template <typename RNGType>
    result_type generate(RNGType &rng, const param_type &param)
    {
        if (param == param_)
            return gamma_(rng);

        GammaDistribution<RealType> gamma(param.n() / 2, 2);

        return gamma(rng);
    }
}; // class ChiSquaredDistribution

MCKL_DEFINE_RANDOM_DISTRIBUTION_RAND(ChiSquared, RealType)

} // namespace mckl

#endif // MCKL_RANDOM_CHI_SQUARED_DISTRIBUTION_HPP
