//============================================================================
// MCKL/include/mckl/random/poisson_distribution.hpp
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

#ifndef MCKL_RANDOM_POISSON_DISTRIBUTION_HPP
#define MCKL_RANDOM_POISSON_DISTRIBUTION_HPP

#include <mckl/random/internal/common.hpp>
#include <mckl/random/u01_distribution.hpp>

namespace mckl
{

namespace internal
{

inline bool poisson_distribution_check_param(double mean) { return mean > 0; }

enum PoissonDistributionAlgorithm {
    PoissonDistributionAlgorithm1,
    PoissonDistributionAlgorithmN
}; // enum PoissonDistributionAlgorithm

template <typename IntType>
class PoissonDistributionConstant
{
    public:
    PoissonDistributionConstant(double mean = 1)
    {
        if (mean < 10)
            algorithm_ = PoissonDistributionAlgorithm1;
        else
            algorithm_ = PoissonDistributionAlgorithmN;

        k0_ = 0;
        double p = std::exp(-mean);
        switch (algorithm_) {
            case PoissonDistributionAlgorithm1:
                cdf_[0] = p;
                for (std::size_t i = 1; i != cdf_.size(); ++i) {
                    p *= mean / static_cast<double>(i);
                    cdf_[i] = cdf_[i - 1] + p;
                }
                k0_ = static_cast<IntType>(std::floor(mean));
                break;
            case PoissonDistributionAlgorithmN:
                break;
        }
    }

    static constexpr IntType kend() { return 36; }

    IntType k1(double u) const
    {
        IntType k = u > 0.458 ? k0_ : 0;
        while (k != kend()) {
            if (u <= cdf_[static_cast<std::size_t>(k)])
                return k;
            ++k;
        }
        return kend();
    }

    PoissonDistributionAlgorithm algorithm() const { return algorithm_; }

    private:
    std::array<double, kend()> cdf_;
    IntType k0_;
    PoissonDistributionAlgorithm algorithm_;
}; // class PoissonDistributionConstant

template <std::size_t K, typename IntType, typename RNGType>
inline void poisson_distribution_impl_1(RNGType &rng, std::size_t n,
    IntType *r, double, const PoissonDistributionConstant<IntType> &constant)
{
    U01CODistribution<double> u01;
    Array<double, K> s;
    double *const u = s.data();
    u01_co_distribution(rng, n, u);
    for (std::size_t i = 0; i != n; ++i) {
        IntType k = constant.k1(u[i]);
        while (k == constant.kend())
            k = constant.k1(u01(rng));
        r[i] = k;
    }
}

template <std::size_t K, typename IntType, typename RNGType>
inline void poisson_distribution_impl_n(RNGType &rng, std::size_t n,
    IntType *r, double mean,
    const PoissonDistributionConstant<IntType> &constant)
{
}

template <std::size_t K, typename IntType, typename RNGType>
inline void poisson_distribution_impl(RNGType &rng, std::size_t n, IntType *r,
    double mean, const PoissonDistributionConstant<IntType> &constant)
{
    switch (constant.algorithm()) {
        case PoissonDistributionAlgorithm1:
            poisson_distribution_impl_1<K>(rng, n, r, mean, constant);
            break;
        case PoissonDistributionAlgorithmN:
            poisson_distribution_impl_n<K>(rng, n, r, mean, constant);
            break;
    }
}

template <typename IntType, typename RNGType>
inline void poisson_distribution(
    RNGType &rng, std::size_t n, IntType *r, double mean)
{
    const PoissonDistributionConstant<IntType> constant(mean);
    const std::size_t k = BufferSize<double>::value;
    const std::size_t m = n / k;
    const std::size_t l = n % k;
    for (std::size_t i = 0; i != m; ++i, r += k)
        poisson_distribution_impl<k>(rng, k, r, mean, constant);
    poisson_distribution_impl<k>(rng, l, r, mean, constant);
}

template <typename IntType, typename RNGType>
inline void poisson_distribution(RNGType &rng, std::size_t n, IntType *r,
    const typename PoissonDistribution<IntType>::param_type &param)
{
    poisson_distribution(rng, n, r, param.mean());
}

} // namespace mckl::internal

/// \brief Poisson distribution
/// \ingroup Distribution
template <typename IntType>
class PoissonDistribution
{
    MCKL_DEFINE_RANDOM_DISTRIBUTION_ASSERT_INT_TYPE(Poisson, short)
    MCKL_DEFINE_RANDOM_DISTRIBUTION_1(
        Poisson, poisson, IntType, double, mean, 1)

    public:
    result_type min() const { return 0; }

    result_type max() const { return std::numeric_limits<IntType>::max(); }

    void reset()
    {
        constant_ = internal::PoissonDistributionConstant<result_type>(mean());
    }

    private:
    internal::PoissonDistributionConstant<result_type> constant_;

    template <typename CharT, typename Traits>
    void ostream(std::basic_ostream<CharT, Traits> &) const
    {
    }

    template <typename CharT, typename Traits>
    void istream(std::basic_istream<CharT, Traits> &)
    {
        reset();
    }

    template <typename RNGType>
    result_type generate(RNGType &rng, const param_type &param)
    {
        if (param == param_)
            return generate(rng, param_, constant_);

        internal::PoissonDistributionConstant<result_type> constant(
            param.mean());

        return generate(rng, param, constant);
    }

    template <typename RNGType>
    result_type generate(RNGType &rng, const param_type &param,
        const internal::PoissonDistributionConstant<result_type> &constant)
    {
        result_type r = 0;
        switch (constant.algorithm()) {
            case internal::PoissonDistributionAlgorithm1:
                r = generate_1(rng, param, constant);
                break;
            case internal::PoissonDistributionAlgorithmN:
                r = generate_n(rng, param, constant);
                break;
        }

        return r;
    }

    template <typename RNGType>
    result_type generate_1(RNGType &rng, const param_type &param,
        const internal::PoissonDistributionConstant<result_type> &constant)
    {
        U01CODistribution<double> u01;
        result_type k = constant.k1(u01(rng));

        return k != constant.kend() ? k : generate_1(rng, param, constant);
    }

    template <typename RNGType>
    result_type generate_n(RNGType &rng, const param_type &param,
        const internal::PoissonDistributionConstant<result_type> &constant)
    {
        return 0;
    }
}; // class PoissonDistribution

MCKL_DEFINE_RANDOM_DISTRIBUTION_RAND(Poisson, IntType)

} // namespace mckl

#endif // MCKL_RANDOM_POISSON_DISTRIBUTION_HPP
