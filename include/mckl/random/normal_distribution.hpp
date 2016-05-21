//============================================================================
// MCKL/include/mckl/random/normal_distribution.hpp
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

#ifndef MCKL_RANDOM_NORMAL_DISTRIBUTION_HPP
#define MCKL_RANDOM_NORMAL_DISTRIBUTION_HPP

#include <mckl/random/internal/common.hpp>
#include <mckl/random/u01_distribution.hpp>
#include <mckl/random/uniform_real_distribution.hpp>

namespace mckl
{

namespace internal
{

template <typename RealType>
inline bool normal_distribution_check_param(RealType, RealType stddev)
{
    return stddev > 0;
}

template <std::size_t K, typename RealType, typename RNGType>
inline void normal_distribution_impl(
    RNGType &rng, std::size_t n, RealType *r, RealType mean, RealType stddev)
{
    Array<RealType, K / 2> s;
    const std::size_t nu = n / 2;
    RealType *const u1 = r;
    RealType *const u2 = r + nu;
    u01_oc_distribution(rng, n, r);
    log(nu, u1, s.data());
    mul(nu, static_cast<RealType>(-2), s.data(), s.data());
    sqrt(nu, s.data(), s.data());
    mul(nu, const_pi_2<RealType>(), u2, u2);
    sincos(nu, u2, u1, u2);
    if (!is_one(stddev))
        mul(nu, stddev, s.data(), s.data());
    if (!is_zero(mean)) {
        fma(nu, s.data(), u1, mean, u1);
        fma(nu, s.data(), u2, mean, u2);
    } else {
        mul(nu, s.data(), u1, u1);
        mul(nu, s.data(), u2, u2);
    }
}

template <typename RealType, typename RNGType>
inline void normal_distribution(
    RNGType &rng, std::size_t n, RealType *r, RealType mean, RealType stddev)
{
    const std::size_t k = BufferSize<RealType>::value;
    const std::size_t m = n / k;
    const std::size_t l = n % k;
    for (std::size_t i = 0; i != m; ++i, r += k)
        normal_distribution_impl<k>(rng, k, r, mean, stddev);
    normal_distribution_impl<k>(rng, l, r, mean, stddev);
    if (n % 2 != 0) {
        U01OCDistribution<RealType> u01;
        RealType u = u01(rng);
        RealType v = u01(rng);
        r[l - 1] = mean +
            stddev * std::sqrt(-2 * std::log(u)) *
                std::cos(const_pi_2<RealType>() * v);
    }
}

template <typename RealType, typename RNGType>
inline void normal_distribution(RNGType &rng, std::size_t n, RealType *r,
    const typename NormalDistribution<RealType>::param_type &param)
{
    normal_distribution(rng, n, r, param.mean(), param.stddev());
}

} // namespace mckl::internal

/// \brief Normal distribution
/// \ingroup Distribution
template <typename RealType>
class NormalDistribution
{
    MCKL_DEFINE_RANDOM_DISTRIBUTION_ASSERT_REAL_TYPE(Normal)
    MCKL_DEFINE_RANDOM_DISTRIBUTION_2(
        Normal, normal, RealType, result_type, mean, 0, result_type, stddev, 1)
    MCKL_DEFINE_RANDOM_DISTRIBUTION_MEMBER_2(result_type, v_, bool, saved_)

    public:
    result_type min() const
    {
        return std::numeric_limits<result_type>::lowest();
    }

    result_type max() const { return std::numeric_limits<result_type>::max(); }

    void reset()
    {
        v_ = 0;
        saved_ = false;
    }

    private:
    template <typename RNGType>
    result_type generate(RNGType &rng, const param_type &param)
    {
        result_type z = 0;
        if (saved_) {
            z = v_;
            saved_ = false;
        } else {
            U01OCDistribution<RealType> u01;
            result_type u1 = std::sqrt(-2 * std::log(u01(rng)));
            result_type u2 = const_pi_2<result_type>() * u01(rng);
            z = u1 * std::cos(u2);
            v_ = u1 * std::sin(u2);
            saved_ = true;
        }

        return param.mean() + param.stddev() * z;
    }
}; // class NormalDistribution

MCKL_DEFINE_RANDOM_DISTRIBUTION_RAND(Normal, RealType)

} // namespace mckl

#endif // MCKL_RANDOM_NORMAL_DISTRIBUTION_HPP
