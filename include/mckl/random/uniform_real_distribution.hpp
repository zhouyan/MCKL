//============================================================================
// MCKL/include/mckl/random/uniform_real_distribution.hpp
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

#ifndef MCKL_RANDOM_UNIFORM_REAL_DISTRIBUTION_HPP
#define MCKL_RANDOM_UNIFORM_REAL_DISTRIBUTION_HPP

#include <mckl/random/internal/common.hpp>
#include <mckl/random/u01_distribution.hpp>

namespace mckl
{

namespace internal
{

template <typename RealType>
inline bool uniform_real_distribution_check_param(RealType a, RealType b)
{
    return a < b;
}

template <std::size_t, typename RealType, typename RNGType>
inline void uniform_real_distribution_impl(
    RNGType &rng, std::size_t n, RealType *r, RealType a, RealType b)
{
    u01_co_distribution(rng, n, r);
    fma(n, r, b - a, a, r);
}

} // namespace mckl::internal

MCKL_DEFINE_RANDOM_DISTRIBUTION_BATCH_2(
    UniformReal, uniform_real, RealType, RealType, a, RealType, b)

/// \brief Uniform real distribution
/// \ingroup Distribution
template <typename RealType>
class UniformRealDistribution
{
    MCKL_DEFINE_RANDOM_DISTRIBUTION_ASSERT_REAL_TYPE(UniformReal)
    MCKL_DEFINE_RANDOM_DISTRIBUTION_2(UniformReal, uniform_real, RealType,
        result_type, a, 0, result_type, b, 1)
    MCKL_DEFINE_RANDOM_DISTRIBUTION_MEMBER_0

    public:
    result_type min() const { return a(); }

    result_type max() const { return b(); }

    void reset() {}

    private:
    template <typename RNGType>
    result_type generate(RNGType &rng, const param_type &param)
    {
        U01CODistribution<RealType> u01;

#if MCKL_USE_FMA
        return std::fma(u01(rng), param.b() - param.a(), param.a());
#else
        return param.a() + (param.b() - param.a()) * u01(rng);
#endif
    }
}; // class UniformRealDistribution

MCKL_DEFINE_RANDOM_DISTRIBUTION_RAND(UniformReal, RealType)

} // namespace mckl

#endif // MCKL_RANDOM_UNIFORM_REAL_DISTRIBUTION_HPP
