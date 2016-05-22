//============================================================================
// MCKL/include/mckl/random/uniform_int_distribution.hpp
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

#ifndef MCKL_RANDOM_UNIFORM_INT_DISTRIBUTION_HPP
#define MCKL_RANDOM_UNIFORM_INT_DISTRIBUTION_HPP

#include <mckl/random/internal/common.hpp>
#include <mckl/random/u01_distribution.hpp>

namespace mckl
{

namespace internal
{

template <typename IntType>
inline bool uniform_int_distribution_check_param(IntType a, IntType b)
{
    return a <= b;
}

template <std::size_t K, typename IntType, typename RNGType>
inline void uniform_int_distribution_impl(
    RNGType &rng, std::size_t n, IntType *r, IntType a, IntType b)
{
    if (a == b) {
        std::fill_n(r, n, a);
        return;
    }

    Array<double, K> s;
    u01_co_distribution(rng, n, s.data());
    fma(n, s.data(), static_cast<double>(b) - static_cast<double>(a) + 1.0,
        static_cast<double>(a), s.data());
    floor(n, s.data(), s.data());
    for (std::size_t i = 0; i != n; ++i)
        r[i] = static_cast<IntType>(s[i]);
}

MCKL_DEFINE_RANDOM_DISTRIBUTION_IMPL_2(
    UniformInt, uniform_int, IntType, IntType, a, IntType, b)

} // namespace mckl::internal

/// \brief Uniform integer distribution
/// \ingroup Distribution
template <typename IntType>
class UniformIntDistribution
{
    MCKL_DEFINE_RANDOM_DISTRIBUTION_ASSERT_INT_TYPE(UniformInt, short)
    MCKL_DEFINE_RANDOM_DISTRIBUTION_2(UniformInt, uniform_int, IntType,
        result_type, a, 0, result_type, b,
        std::numeric_limits<result_type>::max())
    MCKL_DEFINE_RANDOM_DISTRIBUTION_MEMBER_0

    public:
    result_type min() const { return a(); }

    result_type max() const { return b(); }

    void reset() {}

    private:
    template <typename RNGType>
    result_type generate(RNGType &rng, const param_type &param)
    {
        U01CODistribution<double> u01;
        double u = u01(rng);
        u = static_cast<double>(param.a()) +
            (static_cast<double>(param.b()) - static_cast<double>(param.a()) +
                1.0) *
                u;

        return static_cast<result_type>(std::floor(u));
    }
}; // class UniformIntDistribution

MCKL_DEFINE_RANDOM_DISTRIBUTION_RAND(UniformInt, IntType)

} // namespace mckl

#endif // MCKL_RANDOM_UNIFORM_INT_DISTRIBUTION_HPP
