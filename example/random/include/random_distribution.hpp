//============================================================================
// MCKL/example/random/include/random_distribution.hpp
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

#ifndef MCKL_EXAMPLE_RANDOM_DISTRIBUTION_HPP
#define MCKL_EXAMPLE_RANDOM_DISTRIBUTION_HPP

#ifndef MCKL_EXAMPLE_RANDOM_FISHER_F_DISTRIBUTION
#define MCKL_EXAMPLE_RANDOM_FISHER_F_DISTRIBUTION 0
#endif

#ifndef MCKL_EXAMPLE_RANDOM_STUDENT_T_DISTRIBUTION
#define MCKL_EXAMPLE_RANDOM_STUDENT_T_DISTRIBUTION 0
#endif

#ifndef MCKL_EXAMPLE_RANDOM_CHI_SQUARED_DISTRIBUTION
#define MCKL_EXAMPLE_RANDOM_CHI_SQUARED_DISTRIBUTION                          \
    (MCKL_EXAMPLE_RANDOM_STUDENT_T_DISTRIBUTION ||                            \
        MCKL_EXAMPLE_RANDOM_FISHER_F_DISTRIBUTION)
#endif

#ifndef MCKL_EXAMPLE_RANDOM_GAMMA_DISTRIBUTION
#define MCKL_EXAMPLE_RANDOM_GAMMA_DISTRIBUTION                                \
    MCKL_EXAMPLE_RANDOM_CHI_SQUARED_DISTRIBUTION
#endif

#ifndef MCKL_EXAMPLE_RANDOM_STABLE_DISTRIBUTION
#define MCKL_EXAMPLE_RANDOM_STABLE_DISTRIBUTION 0
#endif

#ifndef MCKL_EXAMPLE_RANDOM_CAUCHY_DISTRIBUTION
#define MCKL_EXAMPLE_RANDOM_CAUCHY_DISTRIBUTION                               \
    MCKL_EXAMPLE_RANDOM_STABLE_DISTRIBUTION
#endif

#ifndef MCKL_EXAMPLE_RANDOM_LEVY_DISTRIBUTION
#define MCKL_EXAMPLE_RANDOM_LEVY_DISTRIBUTION                                 \
    MCKL_EXAMPLE_RANDOM_STABLE_DISTRIBUTION
#endif

#ifndef MCKL_EXAMPLE_RANDOM_NORMAL_DISTRIBUTION
#define MCKL_EXAMPLE_RANDOM_NORMAL_DISTRIBUTION                               \
    MCKL_EXAMPLE_RANDOM_STABLE_DISTRIBUTION
#endif

#ifndef MCKL_EXAMPLE_RANDOM_ARCSINE_DISTRIBUTION
#define MCKL_EXAMPLE_RANDOM_ARCSINE_DISTRIBUTION 0
#endif

#ifndef MCKL_EXAMPLE_RANDOM_BETA_DISTRIBUTION
#define MCKL_EXAMPLE_RANDOM_BETA_DISTRIBUTION 0
#endif

#ifndef MCKL_EXAMPLE_RANDOM_EXPONENTIAL_DISTRIBUTION
#define MCKL_EXAMPLE_RANDOM_EXPONENTIAL_DISTRIBUTION 0
#endif

#ifndef MCKL_EXAMPLE_RANDOM_EXTREME_VALUE_DISTRIBUTION
#define MCKL_EXAMPLE_RANDOM_EXTREME_VALUE_DISTRIBUTION 0
#endif

#ifndef MCKL_EXAMPLE_RANDOM_LAPLACE_DISTRIBUTION
#define MCKL_EXAMPLE_RANDOM_LAPLACE_DISTRIBUTION 0
#endif

#ifndef MCKL_EXAMPLE_RANDOM_LOGISTIC_DISTRIBUTION
#define MCKL_EXAMPLE_RANDOM_LOGISTIC_DISTRIBUTION 0
#endif

#ifndef MCKL_EXAMPLE_RANDOM_LOGNORMAL_DISTRIBUTION
#define MCKL_EXAMPLE_RANDOM_LOGNORMAL_DISTRIBUTION 0
#endif

#ifndef MCKL_EXAMPLE_RANDOM_PARETO_DISTRIBUTION
#define MCKL_EXAMPLE_RANDOM_PARETO_DISTRIBUTION 0
#endif

#ifndef MCKL_EXAMPLE_RANDOM_RAYLEIGH_DISTRIBUTION
#define MCKL_EXAMPLE_RANDOM_RAYLEIGH_DISTRIBUTION 0
#endif

#ifndef MCKL_EXAMPLE_RANDOM_U01_DISTRIBUTION
#define MCKL_EXAMPLE_RANDOM_U01_DISTRIBUTION 0
#endif

#ifndef MCKL_EXAMPLE_RANDOM_U01CC_DISTRIBUTION
#define MCKL_EXAMPLE_RANDOM_U01CC_DISTRIBUTION 0
#endif

#ifndef MCKL_EXAMPLE_RANDOM_U01CO_DISTRIBUTION
#define MCKL_EXAMPLE_RANDOM_U01CO_DISTRIBUTION 0
#endif

#ifndef MCKL_EXAMPLE_RANDOM_U01OC_DISTRIBUTION
#define MCKL_EXAMPLE_RANDOM_U01OC_DISTRIBUTION 0
#endif

#ifndef MCKL_EXAMPLE_RANDOM_U01OO_DISTRIBUTION
#define MCKL_EXAMPLE_RANDOM_U01OO_DISTRIBUTION 0
#endif

#ifndef MCKL_EXAMPLE_RANDOM_UNIFORM_REAL_DISTRIBUTION
#define MCKL_EXAMPLE_RANDOM_UNIFORM_REAL_DISTRIBUTION 0
#endif

#ifndef MCKL_EXAMPLE_RANDOM_WEIBULL_DISTRIBUTION
#define MCKL_EXAMPLE_RANDOM_WEIBULL_DISTRIBUTION 0
#endif

#ifndef MCKL_EXAMPLE_RANDOM_GEOMETRIC_DISTRIBUTION
#define MCKL_EXAMPLE_RANDOM_GEOMETRIC_DISTRIBUTION 0
#endif

#ifndef MCKL_EXAMPLE_RANDOM_UNIFORM_INT_DISTRIBUTION
#define MCKL_EXAMPLE_RANDOM_UNIFORM_INT_DISTRIBUTION 0
#endif

#include <mckl/math/beta.hpp>
#include <mckl/math/erf.hpp>
#include <mckl/math/gamma.hpp>
#include "random_common.hpp"

#if MCKL_EXAMPLE_RANDOM_STABLE_DISTRIBUTION
#include <mckl/random/cauchy_distribution.hpp>
#include <mckl/random/levy_distribution.hpp>
#include <mckl/random/normal_distribution.hpp>
#endif

#if MCKL_HAS_AESNI && !defined(MCKL_EXAMPLE_RANDOM_NO_ARS)
#include <mckl/random/aes.hpp>
using MCKLRNGType = mckl::ARS;
#else
#include <mckl/random/philox.hpp>
using MCKLRNGType = mckl::Philox4x32;
#endif

#if MCKL_HAS_MKL
#include <mckl/random/mkl.hpp>
#if MCKL_HAS_AESNI && !defined(MCKL_EXAMPLE_RANDOM_NO_ARS)
using MKLRNGType = mckl::MKL_ARS5;
#else
using MKLRNGType = mckl::MKL_PHILOX4X32X10;
#endif
#endif

template <typename RNGType>
class RNG01 : public RNGType
{
    public:
    using result_type = typename RNGType::result_type;

    result_type operator()()
    {
        result_type u = RNGType::operator()();
        result_type r = u % 3;
        if (r == 0)
            return 0;
        if (r == 1)
            return std::numeric_limits<result_type>::max();
        return u;
    }

    void operator()(std::size_t n, result_type *r)
    {
        for (std::size_t i = 0; i != n; ++i)
            r[i] = operator()();
    }
}; // class RNG01

template <typename ResultType, std::size_t ParamNum>
class RandomDistributionTraitBase
{
    public:
    RandomDistributionTraitBase() = default;
    RandomDistributionTraitBase(
        const RandomDistributionTraitBase<ResultType, ParamNum> &) = default;
    RandomDistributionTraitBase<ResultType, ParamNum> &operator=(
        const RandomDistributionTraitBase<ResultType, ParamNum> &) = default;
    virtual ~RandomDistributionTraitBase() {}

    static constexpr bool invariant() { return false; }

    virtual std::string distname() const = 0;

    template <typename ParamType>
    std::string name(const std::array<ParamType, ParamNum> &param) const
    {
        return name_dispatch(distname(), param);
    }

    protected:
    template <typename QuantileType, typename DistType>
    mckl::Vector<ResultType> partition_quantile(
        std::size_t n, QuantileType &&quantile, const DistType &dist) const
    {
        const std::size_t k = n < 2000 ? n / 100 : 20; // The number of cells
        const double p = 1.0 / k;
        mckl::Vector<ResultType> partition(k - 1);
        for (std::size_t i = 0; i != k - 1; ++i)
            partition[i] = quantile(p * (i + 1));
        partition.push_back(dist.max());

        return partition;
    }

    mckl::Vector<double> probability_quantile(std::size_t n) const
    {
        const std::size_t k = n < 2000 ? n / 100 : 20; // The number of cells
        double p = 1.0 / k;
        mckl::Vector<double> probability(k, p);

        return probability;
    }

    template <typename ParamType>
    static void add_param(mckl::Vector<std::array<ParamType, 0>> &params)
    {
        std::array<ParamType, 0> tmp;
        params.push_back(tmp);
    }

    template <typename ParamType, typename P1>
    static void add_param(mckl::Vector<std::array<ParamType, 1>> &param, P1 p1)
    {
        std::array<ParamType, 1> tmp;
        tmp[0] = static_cast<ParamType>(p1);
        param.push_back(tmp);
    }

    template <typename ParamType, typename P1, typename P2>
    static void add_param(
        mckl::Vector<std::array<ParamType, 2>> &param, P1 p1, P2 p2)
    {
        std::array<ParamType, 2> tmp;
        tmp[0] = static_cast<ParamType>(p1);
        tmp[1] = static_cast<ParamType>(p2);
        param.push_back(tmp);
    }

    template <typename ParamType, typename P1, typename P2, typename P3,
        typename P4>
    static void add_param(mckl::Vector<std::array<ParamType, 4>> &param, P1 p1,
        P2 p2, P3 p3, P4 p4)
    {
        std::array<ParamType, 4> tmp;
        tmp[0] = static_cast<ParamType>(p1);
        tmp[1] = static_cast<ParamType>(p2);
        tmp[2] = static_cast<ParamType>(p3);
        tmp[3] = static_cast<ParamType>(p4);
        param.push_back(tmp);
    }

    private:
    template <typename ParamType>
    static std::string name_dispatch(
        const std::string &distname, const std::array<ParamType, 0> &)
    {
        std::stringstream ss;
        ss << distname << '<' << random_typename<ResultType>() << '>';

        return ss.str();
    }

    template <typename ParamType>
    static std::string name_dispatch(
        const std::string &distname, const std::array<ParamType, 1> &param)
    {
        std::stringstream ss;
        ss << distname << '<' << random_typename<ResultType>() << ">("
           << param[0] << ')';

        return ss.str();
    }

    template <typename ParamType>
    static std::string name_dispatch(
        const std::string &distname, const std::array<ParamType, 2> &param)
    {
        std::stringstream ss;
        ss << distname << '<' << random_typename<ResultType>() << ">("
           << param[0] << ',' << param[1] << ')';

        return ss.str();
    }

    template <typename ParamType>
    static std::string name_dispatch(
        const std::string &distname, const std::array<ParamType, 4> &param)
    {
        std::stringstream ss;
        ss << distname << '<' << random_typename<ResultType>() << ">("
           << param[0] << ',' << param[1] << ',' << param[2] << ',' << param[3]
           << ')';

        return ss.str();
    }
}; // class RandomDistributionTraitBase

template <typename DistType>
class RandomDistributionTrait;

#if MCKL_EXAMPLE_RANDOM_ARCSINE_DISTRIBUTION

template <typename RealType>
class RandomDistributionTrait<mckl::ArcsineDistribution<RealType>>
    : public RandomDistributionTraitBase<RealType, 2>
{
    public:
    using dist_type = mckl::ArcsineDistribution<RealType>;
    using std_type = dist_type;

    std::string distname() const { return "Arcsine"; }

    mckl::Vector<RealType> partition(
        std::size_t n, const dist_type &dist) const
    {
        return this->partition_quantile(n,
            [&](double p) {
                double r = std::sin(mckl::const_pi_by2<double>() * p);
                return dist.a() +
                    (dist.b() - dist.a()) * static_cast<RealType>(r * r);
            },
            dist);
    }

    mckl::Vector<double> probability(std::size_t n, const dist_type &) const
    {
        return this->probability_quantile(n);
    }

    mckl::Vector<std::array<RealType, 2>> params() const
    {
        mckl::Vector<std::array<RealType, 2>> params;
        this->add_param(params, 0, 1);

        return params;
    }
}; // class RandomDistributionTrait

#endif // MCKL_EXAMPLE_RANDOM_ARCSINE_DISTRIBUTION

#if MCKL_EXAMPLE_RANDOM_BETA_DISTRIBUTION

template <typename RealType>
class RandomDistributionTrait<mckl::BetaDistribution<RealType>>
    : public RandomDistributionTraitBase<RealType, 2>
{
    public:
    using dist_type = mckl::BetaDistribution<RealType>;
    using std_type = dist_type;

    std::string distname() const { return "Beta"; }

    mckl::Vector<RealType> partition(
        std::size_t n, const dist_type &dist) const
    {
        return this->partition_quantile(n,
            [&](double p) {
                return static_cast<RealType>(
                    mckl::betaiinv(static_cast<double>(dist.alpha()),
                        static_cast<double>(dist.beta()), p));
            },
            dist);
    }

    mckl::Vector<double> probability(std::size_t n, const dist_type &) const
    {
        return this->probability_quantile(n);
    }

    mckl::Vector<std::array<RealType, 2>> params() const
    {
        mckl::Vector<std::array<RealType, 2>> params;
        this->add_param(params, 0.5, 0.5);
        this->add_param(params, 1, 1);
        this->add_param(params, 1, 0.5);
        this->add_param(params, 1, 1.5);
        this->add_param(params, 0.5, 1);
        this->add_param(params, 1.5, 1);
        this->add_param(params, 1.5, 1.5);
        this->add_param(params, 0.3, 0.3);
        this->add_param(params, 0.9, 0.9);
        this->add_param(params, 1.5, 0.5);
        this->add_param(params, 0.5, 1.5);

        return params;
    }
}; // class RandomDistributionTrait

#endif // MCKL_EXAMPLE_RANDOM_BETA_DISTRIBUTION

#if MCKL_EXAMPLE_RANDOM_CAUCHY_DISTRIBUTION

template <typename RealType>
class RandomDistributionTrait<mckl::CauchyDistribution<RealType>>
    : public RandomDistributionTraitBase<RealType, 2>
{
    public:
    using dist_type = mckl::CauchyDistribution<RealType>;
    using std_type = std::cauchy_distribution<RealType>;

    std::string distname() const { return "Cauchy"; }

    mckl::Vector<RealType> partition(
        std::size_t n, const dist_type &dist) const
    {
        return this->partition_quantile(n,
            [&](double p) {
                return dist.a() +
                    dist.b() * static_cast<RealType>(std::tan(
                                   mckl::const_pi<double>() * (p - 0.5)));
            },
            dist);
    }

    mckl::Vector<double> probability(std::size_t n, const dist_type &) const
    {
        return this->probability_quantile(n);
    }

    mckl::Vector<std::array<RealType, 2>> params() const
    {
        mckl::Vector<std::array<RealType, 2>> params;
        this->add_param(params, 0, 1);

        return params;
    }
}; // class RandomDistributionTrait

#endif // MCKL_EXAMPLE_RANDOM_CAUCHY_DISTRIBUTION

#if MCKL_EXAMPLE_RANDOM_GAMMA_DISTRIBUTION

template <typename RealType>
class RandomDistributionTrait<mckl::GammaDistribution<RealType>>
    : public RandomDistributionTraitBase<RealType, 2>
{
    public:
    using dist_type = mckl::GammaDistribution<RealType>;
    using std_type = std::gamma_distribution<RealType>;

    std::string distname() const { return "Gamma"; }

    mckl::Vector<RealType> partition(std::size_t n, const dist_type &dist)
    {
        return this->partition_quantile(n,
            [&](double p) {
                return static_cast<RealType>(
                    mckl::gammapinv(static_cast<double>(dist.alpha()), p) *
                    static_cast<double>(dist.beta()));
            },
            dist);
    }

    mckl::Vector<double> probability(std::size_t n, const dist_type &) const
    {
        return this->probability_quantile(n);
    }

    mckl::Vector<std::array<RealType, 2>> params() const
    {
        mckl::Vector<std::array<RealType, 2>> params;
        this->add_param(params, 1, 1);
        this->add_param(params, 0.1, 1);
        this->add_param(params, 0.5, 1);
        this->add_param(params, 0.7, 1);
        this->add_param(params, 0.9, 1);
        this->add_param(params, 1.5, 1);
        this->add_param(params, 15, 1);

        return params;
    }
}; // class RandomDistributionTrait

#endif // MCKL_EXAMPLE_RANDOM_GAMMA_DISTRIBUTION

#if MCKL_EXAMPLE_RANDOM_CHI_SQUARED_DISTRIBUTION

template <typename RealType>
class RandomDistributionTrait<mckl::ChiSquaredDistribution<RealType>>
    : public RandomDistributionTraitBase<RealType, 1>
{
    public:
    using dist_type = mckl::ChiSquaredDistribution<RealType>;
    using std_type = std::chi_squared_distribution<RealType>;

    std::string distname() const { return "ChiSquared"; }

    mckl::Vector<RealType> partition(std::size_t n, const dist_type &dist)
    {
        return this->partition_quantile(n,
            [&](double p) {
                return static_cast<RealType>(
                    mckl::gammapinv(static_cast<double>(dist.n()) / 2, p) * 2);
            },
            dist);
    }

    mckl::Vector<double> probability(std::size_t n, const dist_type &) const
    {
        return this->probability_quantile(n);
    }

    mckl::Vector<std::array<RealType, 1>> params() const
    {
        RandomDistributionTrait<mckl::GammaDistribution<RealType>> gamma_trait;
        mckl::Vector<std::array<RealType, 2>> pgamma = gamma_trait.params();
        mckl::Vector<std::array<RealType, 1>> params;
        for (std::size_t i = 0; i != pgamma.size(); ++i)
            this->add_param(params, pgamma[i][0] * 2);

        return params;
    }
}; // class RandomDistributionTrait

#endif // MCKL_EXAMPLE_RANDOM_CHI_SQUARED_DISTRIBUTION

#if MCKL_EXAMPLE_RANDOM_EXPONENTIAL_DISTRIBUTION

template <typename RealType>
class RandomDistributionTrait<mckl::ExponentialDistribution<RealType>>
    : public RandomDistributionTraitBase<RealType, 1>
{
    public:
    using dist_type = mckl::ExponentialDistribution<RealType>;
    using std_type = std::exponential_distribution<RealType>;

    std::string distname() const { return "Exponential"; }

    mckl::Vector<RealType> partition(std::size_t n, const dist_type &dist)
    {
        return this->partition_quantile(n,
            [&](double p) {
                return -static_cast<RealType>(std::log(1 - p)) / dist.lambda();
            },
            dist);
    }

    mckl::Vector<double> probability(std::size_t n, const dist_type &) const
    {
        return this->probability_quantile(n);
    }

    mckl::Vector<std::array<RealType, 1>> params() const
    {
        mckl::Vector<std::array<RealType, 1>> params;
        this->add_param(params, 1);

        return params;
    }
}; // class RandomDistributionTrait

#endif // MCKL_EXAMPLE_RANDOM_EXPONENTIAL_DISTRIBUTION

#if MCKL_EXAMPLE_RANDOM_EXTREME_VALUE_DISTRIBUTION

template <typename RealType>
class RandomDistributionTrait<mckl::ExtremeValueDistribution<RealType>>
    : public RandomDistributionTraitBase<RealType, 2>
{
    public:
    using dist_type = mckl::ExtremeValueDistribution<RealType>;
    using std_type = std::extreme_value_distribution<RealType>;

    std::string distname() const { return "ExtremeValue"; }

    mckl::Vector<RealType> partition(std::size_t n, const dist_type &dist)
    {
        return this->partition_quantile(n,
            [&](double p) {
                return dist.a() -
                    dist.b() * static_cast<RealType>(std::log(-std::log(p)));
            },
            dist);
    }

    mckl::Vector<double> probability(std::size_t n, const dist_type &) const
    {
        return this->probability_quantile(n);
    }

    mckl::Vector<std::array<RealType, 2>> params() const
    {
        mckl::Vector<std::array<RealType, 2>> params;
        this->add_param(params, 0, 1);

        return params;
    }
}; // class RandomDistributionTrait

#endif // MCKL_EXAMPLE_RANDOM_EXTREME_VALUE_DISTRIBUTION

#if MCKL_EXAMPLE_RANDOM_FISHER_F_DISTRIBUTION

template <typename RealType>
class RandomDistributionTrait<mckl::FisherFDistribution<RealType>>
    : public RandomDistributionTraitBase<RealType, 2>
{
    public:
    using dist_type = mckl::FisherFDistribution<RealType>;
    using std_type = std::fisher_f_distribution<RealType>;

    std::string distname() const { return "FisherF"; }

    mckl::Vector<RealType> partition(std::size_t n, const dist_type &dist)
    {
        return this->partition_quantile(n,
            [&](double p) {
                RealType q = static_cast<RealType>(
                    mckl::betaiinv(static_cast<double>(dist.m()) / 2,
                        static_cast<double>(dist.n()) / 2, p));
                return dist.n() * q / (dist.m() - dist.m() * q);
            },
            dist);
    }

    mckl::Vector<double> probability(std::size_t n, const dist_type &) const
    {
        return this->probability_quantile(n);
    }

    mckl::Vector<std::array<RealType, 2>> params() const
    {
        std::array<double, 5> df = {{1, 0.5, 1.5, 3, 30}};
        mckl::Vector<std::array<RealType, 2>> params;
        for (std::size_t i = 0; i != df.size(); ++i)
            for (std::size_t j = 0; j != df.size(); ++j)
                this->add_param(params, df[i], df[j]);

        return params;
    }
}; // class RandomDistributionTrait

#endif // MCKL_EXAMPLE_RANDOM_FISHER_F_DISTRIBUTION

#if MCKL_EXAMPLE_RANDOM_LAPLACE_DISTRIBUTION

template <typename RealType>
class RandomDistributionTrait<mckl::LaplaceDistribution<RealType>>
    : public RandomDistributionTraitBase<RealType, 2>
{
    public:
    using dist_type = mckl::LaplaceDistribution<RealType>;
    using std_type = dist_type;

    std::string distname() const { return "Laplace"; }

    mckl::Vector<RealType> partition(std::size_t n, const dist_type &dist)
    {
        return this->partition_quantile(n,
            [&](double p) {
                double q = p - 0.5;
                return q > 0 ?
                    dist.a() -
                        dist.b() * static_cast<RealType>(std::log(1 - 2 * q)) :
                    dist.a() +
                        dist.b() * static_cast<RealType>(std::log(1 + 2 * q));
            },
            dist);
    }

    mckl::Vector<double> probability(std::size_t n, const dist_type &) const
    {
        return this->probability_quantile(n);
    }

    mckl::Vector<std::array<RealType, 2>> params() const
    {
        mckl::Vector<std::array<RealType, 2>> params;
        this->add_param(params, 0, 1);

        return params;
    }
}; // class RandomDistributionTrait

#endif // MCKL_EXAMPLE_RANDOM_LAPLACE_DISTRIBUTION

#if MCKL_EXAMPLE_RANDOM_LEVY_DISTRIBUTION

template <typename RealType>
class RandomDistributionTrait<mckl::LevyDistribution<RealType>>
    : public RandomDistributionTraitBase<RealType, 2>
{
    public:
    using dist_type = mckl::LevyDistribution<RealType>;
    using std_type = dist_type;

    std::string distname() const { return "Levy"; }

    mckl::Vector<RealType> partition(std::size_t n, const dist_type &dist)
    {
        return this->partition_quantile(n,
            [&](double p) {
                double q = mckl::const_sqrt_2<double>() * mckl::erfinv(1 - p);
                return dist.a() + dist.b() / static_cast<RealType>(q * q);
            },
            dist);
    }

    mckl::Vector<double> probability(std::size_t n, const dist_type &) const
    {
        return this->probability_quantile(n);
    }

    mckl::Vector<std::array<RealType, 2>> params() const
    {
        mckl::Vector<std::array<RealType, 2>> params;
        this->add_param(params, 0, 1);

        return params;
    }
}; // class RandomDistributionTrait

#endif // MCKL_EXAMPLE_RANDOM_LEVY_DISTRIBUTION

#if MCKL_EXAMPLE_RANDOM_LOGISTIC_DISTRIBUTION

template <typename RealType>
class RandomDistributionTrait<mckl::LogisticDistribution<RealType>>
    : public RandomDistributionTraitBase<RealType, 2>
{
    public:
    using dist_type = mckl::LogisticDistribution<RealType>;
    using std_type = dist_type;

    std::string distname() const { return "Logistic"; }

    mckl::Vector<RealType> partition(std::size_t n, const dist_type &dist)
    {
        return this->partition_quantile(n,
            [&](double p) {
                return dist.a() +
                    dist.b() * static_cast<RealType>(std::log(p / (1 - p)));
            },
            dist);
    }

    mckl::Vector<double> probability(std::size_t n, const dist_type &) const
    {
        return this->probability_quantile(n);
    }

    mckl::Vector<std::array<RealType, 2>> params() const
    {
        mckl::Vector<std::array<RealType, 2>> params;
        this->add_param(params, 0, 1);

        return params;
    }
}; // class RandomDistributionTrait

#endif // MCKL_EXAMPLE_RANDOM_LOGISTIC_DISTRIBUTION

#if MCKL_EXAMPLE_RANDOM_LOGNORMAL_DISTRIBUTION

template <typename RealType>
class RandomDistributionTrait<mckl::LognormalDistribution<RealType>>
    : public RandomDistributionTraitBase<RealType, 2>
{
    public:
    using dist_type = mckl::LognormalDistribution<RealType>;
    using std_type = std::lognormal_distribution<RealType>;

    std::string distname() const { return "Lognormal"; }

    mckl::Vector<RealType> partition(std::size_t n, const dist_type &dist)
    {
        return this->partition_quantile(n,
            [&](double p) {
                double q =
                    mckl::const_sqrt_2<double>() * mckl::erfinv(2 * p - 1);
                return std::exp(
                    dist.m() + dist.s() * static_cast<RealType>(q));
            },
            dist);
    }

    mckl::Vector<double> probability(std::size_t n, const dist_type &) const
    {
        return this->probability_quantile(n);
    }

    mckl::Vector<std::array<RealType, 2>> params() const
    {
        mckl::Vector<std::array<RealType, 2>> params;
        this->add_param(params, 0, 1);

        return params;
    }
}; // class RandomDistributionTrait

#endif // MCKL_EXAMPLE_RANDOM_LOGNORMAL_DISTRIBUTION

#if MCKL_EXAMPLE_RANDOM_NORMAL_DISTRIBUTION

template <typename RealType>
class RandomDistributionTrait<mckl::NormalDistribution<RealType>>
    : public RandomDistributionTraitBase<RealType, 2>
{
    public:
    using dist_type = mckl::NormalDistribution<RealType>;
    using std_type = std::normal_distribution<RealType>;

    std::string distname() const { return "Normal"; }

    mckl::Vector<RealType> partition(std::size_t n, const dist_type &dist)
    {
        return this->partition_quantile(n,
            [&](double p) {
                double q =
                    mckl::const_sqrt_2<double>() * mckl::erfinv(2 * p - 1);
                return dist.mean() + dist.stddev() * static_cast<RealType>(q);
            },
            dist);
    }

    mckl::Vector<double> probability(std::size_t n, const dist_type &) const
    {
        return this->probability_quantile(n);
    }

    mckl::Vector<std::array<RealType, 2>> params() const
    {
        mckl::Vector<std::array<RealType, 2>> params;
        this->add_param(params, 0, 1);

        return params;
    }
}; // class RandomDistributionTrait

#endif // MCKL_EXAMPLE_RANDOM_NORMAL_DISTRIBUTION

#if MCKL_EXAMPLE_RANDOM_PARETO_DISTRIBUTION

template <typename RealType>
class RandomDistributionTrait<mckl::ParetoDistribution<RealType>>
    : public RandomDistributionTraitBase<RealType, 2>
{
    public:
    using dist_type = mckl::ParetoDistribution<RealType>;
    using std_type = dist_type;

    std::string distname() const { return "Pareto"; }

    mckl::Vector<RealType> partition(std::size_t n, const dist_type &dist)
    {
        return this->partition_quantile(n,
            [&](double p) {
                return dist.b() /
                    std::exp(
                        static_cast<RealType>(std::log(1 - p)) / dist.a());
            },
            dist);
    }

    mckl::Vector<double> probability(std::size_t n, const dist_type &) const
    {
        return this->probability_quantile(n);
    }

    mckl::Vector<std::array<RealType, 2>> params() const
    {
        mckl::Vector<std::array<RealType, 2>> params;
        this->add_param(params, 1, 1);

        return params;
    }
}; // class RandomDistributionTrait

#endif // MCKL_EXAMPLE_RANDOM_PARETO_DISTRIBUTION

#if MCKL_EXAMPLE_RANDOM_RAYLEIGH_DISTRIBUTION

template <typename RealType>
class RandomDistributionTrait<mckl::RayleighDistribution<RealType>>
    : public RandomDistributionTraitBase<RealType, 1>
{
    public:
    using dist_type = mckl::RayleighDistribution<RealType>;
    using std_type = dist_type;

    std::string distname() const { return "Rayleigh"; }

    mckl::Vector<RealType> partition(std::size_t n, const dist_type &dist)
    {
        return this->partition_quantile(n,
            [&](double p) {
                return std::sqrt(-2 * static_cast<RealType>(std::log(1 - p)) *
                    dist.sigma() * dist.sigma());
            },
            dist);
    }

    mckl::Vector<double> probability(std::size_t n, const dist_type &) const
    {
        return this->probability_quantile(n);
    }

    mckl::Vector<std::array<RealType, 1>> params() const
    {
        mckl::Vector<std::array<RealType, 1>> params;
        this->add_param(params, 1);

        return params;
    }
}; // class RandomDistributionTrait

#endif // MCKL_EXAMPLE_RANDOM_RAYLEIGH_DISTRIBUTION

#if MCKL_EXAMPLE_RANDOM_STABLE_DISTRIBUTION

template <typename RealType>
class RandomDistributionTrait<mckl::StableDistribution<RealType>>
    : public RandomDistributionTraitBase<RealType, 4>
{
    public:
    using dist_type = mckl::StableDistribution<RealType>;
    using std_type = dist_type;

    std::string distname() const { return "Stable"; }

    mckl::Vector<RealType> partition(std::size_t n, const dist_type &dist)
    {
        if (mckl::internal::is_equal(dist.alpha(), static_cast<RealType>(2))) {
            RandomDistributionTrait<mckl::NormalDistribution<RealType>> trait;
            return trait.partition(
                n, mckl::NormalDistribution<RealType>(
                       0, std::sqrt(static_cast<RealType>(2))));
        } else if (mckl::internal::is_one(dist.alpha())) {
            RandomDistributionTrait<mckl::CauchyDistribution<RealType>> trait;
            return trait.partition(
                n, mckl::CauchyDistribution<RealType>(0, 1));
        } else {
            RandomDistributionTrait<mckl::LevyDistribution<RealType>> trait;
            return trait.partition(n, mckl::LevyDistribution<RealType>(0, 1));
        }
    }

    mckl::Vector<double> probability(std::size_t n, const dist_type &) const
    {
        return this->probability_quantile(n);
    }

    mckl::Vector<std::array<RealType, 4>> params() const
    {
        mckl::Vector<std::array<RealType, 4>> params;
        this->add_param(params, 2, 0, 0, 1);
        this->add_param(params, 1, 0, 0, 1);
        this->add_param(params, 0.5, 1, 0, 1);

        return params;
    }
}; // class RandomDistributionTrait

#endif // MCKL_EXAMPLE_RANDOM_STABLE_DISTRIBUTION

#if MCKL_EXAMPLE_RANDOM_STUDENT_T_DISTRIBUTION

template <typename RealType>
class RandomDistributionTrait<mckl::StudentTDistribution<RealType>>
    : public RandomDistributionTraitBase<RealType, 1>
{
    public:
    using dist_type = mckl::StudentTDistribution<RealType>;
    using std_type = std::student_t_distribution<RealType>;

    std::string distname() const { return "StudentT"; }

    mckl::Vector<RealType> partition(std::size_t n, const dist_type &dist)
    {
        return this->partition_quantile(n,
            [&](double p) {
                RealType q = static_cast<RealType>(
                    mckl::betaiinv(static_cast<double>(dist.n()) * 0.5, 0.5,
                        2 * std::min(p, 1 - p)));
                RealType t = std::sqrt(dist.n() * (1 - q) / q);

                return p > 0.5 ? t : -t;
            },
            dist);
    }

    mckl::Vector<double> probability(std::size_t n, const dist_type &) const
    {
        return this->probability_quantile(n);
    }

    mckl::Vector<std::array<RealType, 1>> params() const
    {
        RandomDistributionTrait<mckl::ChiSquaredDistribution<RealType>>
            chi_trait;

        return chi_trait.params();
    }
}; // class RandomDistributionTrait

#endif // MCKL_EXAMPLE_RANDOM_STUDENT_T_DISTRIBUTION

#if MCKL_EXAMPLE_RANDOM_U01_DISTRIBUTION

template <typename RealType>
class RandomDistributionTrait<mckl::U01Distribution<RealType>>
    : public RandomDistributionTraitBase<RealType, 0>
{
    public:
    using dist_type = mckl::U01Distribution<RealType>;
    using std_type = std::uniform_real_distribution<RealType>;

    static constexpr bool invariant() { return true; }

    std::string distname() const { return "U01"; }

    mckl::Vector<RealType> partition(std::size_t n, const dist_type &dist)
    {
        return this->partition_quantile(
            n, [&](double p) { return static_cast<RealType>(p); }, dist);
    }

    mckl::Vector<double> probability(std::size_t n, const dist_type &) const
    {
        return this->probability_quantile(n);
    }

    mckl::Vector<std::array<RealType, 0>> params() const
    {
        return mckl::Vector<std::array<RealType, 0>>(1);
    }
}; // class RandomDistributionTrait

#endif // MCKL_EXAMPLE_RANDOM_U01_DISTRIBUTION

#if MCKL_EXAMPLE_RANDOM_U01CC_DISTRIBUTION

template <typename RealType>
class RandomDistributionTrait<mckl::U01CCDistribution<RealType>>
    : public RandomDistributionTraitBase<RealType, 0>
{
    public:
    using dist_type = mckl::U01CCDistribution<RealType>;
    using std_type = std::uniform_real_distribution<RealType>;

    static constexpr bool invariant() { return true; }

    std::string distname() const { return "U01CC"; }

    mckl::Vector<RealType> partition(std::size_t n, const dist_type &dist)
    {
        return this->partition_quantile(
            n, [&](double p) { return static_cast<RealType>(p); }, dist);
    }

    mckl::Vector<double> probability(std::size_t n, const dist_type &) const
    {
        return this->probability_quantile(n);
    }

    mckl::Vector<std::array<RealType, 0>> params() const
    {
        return mckl::Vector<std::array<RealType, 0>>(1);
    }
}; // class RandomDistributionTrait

#endif // MCKL_EXAMPLE_RANDOM_U01CC_DISTRIBUTION

#if MCKL_EXAMPLE_RANDOM_U01CO_DISTRIBUTION

template <typename RealType>
class RandomDistributionTrait<mckl::U01CODistribution<RealType>>
    : public RandomDistributionTraitBase<RealType, 0>
{
    public:
    using dist_type = mckl::U01CODistribution<RealType>;
    using std_type = std::uniform_real_distribution<RealType>;

    static constexpr bool invariant() { return true; }

    std::string distname() const { return "U01CO"; }

    mckl::Vector<RealType> partition(std::size_t n, const dist_type &dist)
    {
        return this->partition_quantile(
            n, [&](double p) { return static_cast<RealType>(p); }, dist);
    }

    mckl::Vector<double> probability(std::size_t n, const dist_type &) const
    {
        return this->probability_quantile(n);
    }

    mckl::Vector<std::array<RealType, 0>> params() const
    {
        return mckl::Vector<std::array<RealType, 0>>(1);
    }
}; // class RandomDistributionTrait

#endif // MCKL_EXAMPLE_RANDOM_U01CO_DISTRIBUTION

#if MCKL_EXAMPLE_RANDOM_U01OC_DISTRIBUTION

template <typename RealType>
class RandomDistributionTrait<mckl::U01OCDistribution<RealType>>
    : public RandomDistributionTraitBase<RealType, 0>
{
    public:
    using dist_type = mckl::U01OCDistribution<RealType>;
    using std_type = std::uniform_real_distribution<RealType>;

    static constexpr bool invariant() { return true; }

    std::string distname() const { return "U01OC"; }

    mckl::Vector<RealType> partition(std::size_t n, const dist_type &dist)
    {
        return this->partition_quantile(
            n, [&](double p) { return static_cast<RealType>(p); }, dist);
    }

    mckl::Vector<double> probability(std::size_t n, const dist_type &) const
    {
        return this->probability_quantile(n);
    }

    mckl::Vector<std::array<RealType, 0>> params() const
    {
        return mckl::Vector<std::array<RealType, 0>>(1);
    }
}; // class RandomDistributionTrait

#endif // MCKL_EXAMPLE_RANDOM_U01OC_DISTRIBUTION

#if MCKL_EXAMPLE_RANDOM_U01OO_DISTRIBUTION

template <typename RealType>
class RandomDistributionTrait<mckl::U01OODistribution<RealType>>
    : public RandomDistributionTraitBase<RealType, 0>
{
    public:
    using dist_type = mckl::U01OODistribution<RealType>;
    using std_type = std::uniform_real_distribution<RealType>;

    static constexpr bool invariant() { return true; }

    std::string distname() const { return "U01OO"; }

    mckl::Vector<RealType> partition(std::size_t n, const dist_type &dist)
    {
        return this->partition_quantile(
            n, [&](double p) { return static_cast<RealType>(p); }, dist);
    }

    mckl::Vector<double> probability(std::size_t n, const dist_type &) const
    {
        return this->probability_quantile(n);
    }

    mckl::Vector<std::array<RealType, 0>> params() const
    {
        return mckl::Vector<std::array<RealType, 0>>(1);
    }
}; // class RandomDistributionTrait

#endif // MCKL_EXAMPLE_RANDOM_U01OO_DISTRIBUTION

#if MCKL_EXAMPLE_RANDOM_UNIFORM_REAL_DISTRIBUTION

template <typename RealType>
class RandomDistributionTrait<mckl::UniformRealDistribution<RealType>>
    : public RandomDistributionTraitBase<RealType, 2>
{
    public:
    using dist_type = mckl::UniformRealDistribution<RealType>;
    using std_type = std::uniform_real_distribution<RealType>;

    static constexpr bool invariant() { return true; }

    std::string distname() const { return "UniformReal"; }

    mckl::Vector<RealType> partition(std::size_t n, const dist_type &dist)
    {
        return this->partition_quantile(n,
            [&](double p) {
                return dist.a() +
                    static_cast<RealType>(p) * (dist.b() - dist.a());
            },
            dist);
    }

    mckl::Vector<double> probability(std::size_t n, const dist_type &) const
    {
        return this->probability_quantile(n);
    }

    mckl::Vector<std::array<RealType, 2>> params() const
    {
        mckl::Vector<std::array<RealType, 2>> params;
        this->add_param(params, -1.3, 1.3);

        return params;
    }
}; // class RandomDistributionTrait

#endif // MCKL_EXAMPLE_RANDOM_UNIFORM_REAL_DISTRIBUTION

#if MCKL_EXAMPLE_RANDOM_WEIBULL_DISTRIBUTION

template <typename RealType>
class RandomDistributionTrait<mckl::WeibullDistribution<RealType>>
    : public RandomDistributionTraitBase<RealType, 2>
{
    public:
    using dist_type = mckl::WeibullDistribution<RealType>;
    using std_type = std::weibull_distribution<RealType>;

    std::string distname() const { return "Weibull"; }

    mckl::Vector<RealType> partition(std::size_t n, const dist_type &dist)
    {
        return this->partition_quantile(n,
            [&](double p) {
                return dist.b() *
                    std::pow(
                        -static_cast<RealType>(std::log(1 - p)), 1 / dist.a());
            },
            dist);
    }

    mckl::Vector<double> probability(std::size_t n, const dist_type &) const
    {
        return this->probability_quantile(n);
    }

    mckl::Vector<std::array<RealType, 2>> params() const
    {
        mckl::Vector<std::array<RealType, 2>> params;
        this->add_param(params, 1, 1);

        return params;
    }
}; // class RandomDistributionTrait

#endif // MCKL_EXAMPLE_RANDOM_WEIBULL_DISTRIBUTION

#if MCKL_EXAMPLE_RANDOM_GEOMETRIC_DISTRIBUTION

template <typename IntType>
class RandomDistributionTrait<mckl::GeometricDistribution<IntType>>
    : public RandomDistributionTraitBase<IntType, 1>
{
    public:
    using dist_type = mckl::GeometricDistribution<IntType>;
    using std_type = std::geometric_distribution<IntType>;

    std::string distname() const { return "Geometric"; }

    mckl::Vector<std::array<double, 1>> params() const
    {
        mckl::Vector<std::array<double, 1>> params;
        this->add_param(params, 0.5);

        return params;
    }

    mckl::Vector<IntType> partition(std::size_t n, const dist_type &dist)
    {
        const IntType k = static_cast<IntType>(
            std::ceil((std::log(5.0 / n) - std::log(dist.p())) /
                std::log(1 - dist.p())));

        mckl::Vector<IntType> partition;
        for (IntType i = 0; i <= k; ++i)
            partition.push_back(i);
        partition.push_back(dist.max());

        return partition;
    }

    mckl::Vector<double> probability(std::size_t n, const dist_type &dist)
    {
        const IntType k = static_cast<IntType>(
            std::ceil((std::log(5.0 / n) - std::log(dist.p())) /
                std::log(1 - dist.p())));

        mckl::Vector<double> probability;
        probability.push_back(dist.p());
        for (IntType i = 1; i <= k; ++i)
            probability.push_back(probability.back() * (1 - dist.p()));
        probability.push_back(
            1 - std::accumulate(probability.begin(), probability.end(), 0.0));

        return probability;
    }
}; // class RandomDistributionTrait

#endif // MCKL_EXAMPLE_RANDOM_GEOMETRIC_DISTRIBUTION

#if MCKL_EXAMPLE_RANDOM_UNIFORM_INT_DISTRIBUTION

template <typename IntType>
class RandomDistributionTrait<mckl::UniformIntDistribution<IntType>>
    : public RandomDistributionTraitBase<IntType, 2>
{
    public:
    using dist_type = mckl::UniformIntDistribution<IntType>;
    using std_type = std::uniform_int_distribution<IntType>;

    std::string distname() const { return "UniformInt"; }

    mckl::Vector<std::array<IntType, 2>> params() const
    {
        return params(std::is_signed<IntType>());
    }

    mckl::Vector<IntType> partition(std::size_t, const dist_type &dist)
    {
        IntType a = dist.a();
        IntType b = dist.b();
        mckl::Vector<IntType> partition;
        for (IntType i = a; i <= b; ++i)
            partition.push_back(i);

        return partition;
    }

    mckl::Vector<double> probability(std::size_t, const dist_type &dist)
    {
        IntType a = dist.a();
        IntType b = dist.b();
        const double p = 1.0 / (b - a + 1);

        return mckl::Vector<double>(static_cast<std::size_t>(b - a + 1), p);
    }

    private:
    mckl::Vector<std::array<IntType, 2>> params(std::true_type) const
    {
        mckl::Vector<std::array<IntType, 2>> params;
        this->add_param(params, -10, 10);

        return params;
    }

    mckl::Vector<std::array<IntType, 2>> params(std::false_type) const
    {
        mckl::Vector<std::array<IntType, 2>> params;
        this->add_param(params, 0, 20);

        return params;
    }
}; // class RandomDistributionTrait

#endif // MCKL_EXAMPLE_RANDOM_UNIFORM_INT_DISTRIBUTION

template <typename DistType, typename ParamType>
inline DistType random_distribution_init(const std::array<ParamType, 0> &)
{
    return DistType();
}

template <typename DistType, typename ParamType>
inline DistType random_distribution_init(const std::array<ParamType, 1> &param)
{
    return DistType(param[0]);
}

template <typename DistType, typename ParamType>
inline DistType random_distribution_init(const std::array<ParamType, 2> &param)
{
    return DistType(param[0], param[1]);
}

template <typename DistType, typename ParamType>
inline DistType random_distribution_init(const std::array<ParamType, 4> &param)
{
    return DistType(param[0], param[1], param[2], param[3]);
}

template <typename MCKLDistType>
inline double random_distribution_chi2(std::size_t n,
    const typename MCKLDistType::result_type *r, const MCKLDistType &dist)
{
    using result_type = typename MCKLDistType::result_type;

    RandomDistributionTrait<MCKLDistType> trait;
    auto partition = trait.partition(n, dist);
    auto probability = trait.probability(n, dist);
    std::size_t k = partition.size();

    mckl::Vector<result_type> rval(n);
    mckl::Vector<std::size_t> count(k);
    std::copy_n(r, n, rval.begin());
    std::sort(rval.begin(), rval.end());
    std::size_t j = 0;
    for (std::size_t i = 0; i != k; ++i) {
        std::size_t c = 0;
        result_type p = static_cast<result_type>(partition[i]);
        while (j != n && rval[j] <= p) {
            ++c;
            ++j;
        }
        count[i] = c;
    }

    double s = 0;
    for (std::size_t i = 0; i != k; ++i) {
        double c = static_cast<double>(count[i]);
        double p = static_cast<double>(probability[i]);
        s += c * c / p;
    }

    return mckl::gammap(0.5 * (k - 1), 0.5 * (s / n - n));
}

template <typename MCKLDistType>
inline double random_distribution_ksad(std::size_t n,
    const typename MCKLDistType::result_type *r, const MCKLDistType &dist)
{
    const std::size_t k = 10;
    const std::size_t m = n / k;
    mckl::Vector<double> chi2(k);
    mckl::Vector<double> head(k);
    mckl::Vector<double> tail(k);
    for (std::size_t i = 0; i != k; ++i)
        chi2[i] = random_distribution_chi2(m, r + i * m, dist);
    std::sort(chi2.begin(), chi2.end());
    mckl::log(k, chi2.data(), head.data());
    std::reverse(chi2.begin(), chi2.end());
    mckl::sub(k, 1.0, chi2.data(), chi2.data());
    mckl::log(k, chi2.data(), tail.data());
    mckl::add(k, head.data(), tail.data(), chi2.data());
    for (std::size_t i = 0; i != k; ++i)
        chi2[i] *= 2 * i + 1;
    double s = std::accumulate(chi2.begin(), chi2.end(), 0.0);

    return -(k + s / k);
}

inline void random_distribution_pval(const mckl::Vector<double> &chi2,
    const mckl::Vector<double> &ksad,
    std::array<mckl::Vector<double>, 6> &pval)
{
    std::size_t alpha1;
    std::size_t alpha2;
    std::size_t alpha3;

    alpha1 = alpha2 = alpha3 = 0;
    for (std::size_t i = 0; i != chi2.size(); ++i) {
        if (chi2[i] > 0.0125 && chi2[i] < 1 - 0.0125)
            ++alpha1;
        if (chi2[i] > 0.025 && chi2[i] < 1 - 0.025)
            ++alpha2;
        if (chi2[i] > 0.05 && chi2[i] < 1 - 0.05)
            ++alpha3;
    }
    pval[0].push_back(100.0 * alpha1 / chi2.size());
    pval[1].push_back(100.0 * alpha2 / chi2.size());
    pval[2].push_back(100.0 * alpha3 / chi2.size());

    alpha1 = alpha2 = alpha3 = 0;
    for (std::size_t i = 0; i != ksad.size(); ++i) {
        if (ksad[i] < 3.0916)
            ++alpha1;
        if (ksad[i] < 2.4986)
            ++alpha2;
        if (ksad[i] < 1.9355)
            ++alpha3;
    }
    pval[3].push_back(100.0 * alpha1 / ksad.size());
    pval[4].push_back(100.0 * alpha2 / ksad.size());
    pval[5].push_back(100.0 * alpha3 / ksad.size());
}

inline void random_distribution_summary_pval(double pval, int twid)
{
    std::stringstream ss;
    if (pval < 50)
        ss << '*';
    ss << pval << '%';
    std::cout << std::setw(twid) << std::right << ss.str();
}

inline void random_distribution_summary_pval(
    const mckl::Vector<std::string> &names,
    const std::array<mckl::Vector<double>, 6> &pval, int nwid, int twid)
{
    const std::size_t D = names.size();
    const std::size_t R = pval[0].size() / D;
    const std::size_t lwid =
        static_cast<std::size_t>(nwid + twid * (3 + MCKL_HAS_MKL));

    const double *p0 = pval[0].data();
    const double *p1 = pval[1].data();
    const double *p2 = pval[2].data();
    const double *p3 = pval[3].data();
    const double *p4 = pval[4].data();
    const double *p5 = pval[5].data();
    for (std::size_t i = 0; i != D; ++i) {
        std::cout << std::string(lwid, '=') << std::endl;
        std::cout << std::setw(nwid) << std::left << names[i];
        std::cout << std::setw(twid) << std::right << "STD";
        std::cout << std::setw(twid) << std::right << "MCKL";
        std::cout << std::setw(twid) << std::right << "Batch";
#if MCKL_HAS_MKL
        std::cout << std::setw(twid) << std::right << "MKL";
#endif
        std::cout << std::endl;
        std::cout << std::string(lwid, '-') << std::endl;

        std::cout << std::setw(nwid) << std::left << "One level test (2.5%)";
        for (std::size_t r = 0; r != R; ++r)
            random_distribution_summary_pval(*p0++, twid);
        std::cout << std::endl;

        std::cout << std::setw(nwid) << std::left << "One level test (5%)";
        for (std::size_t r = 0; r != R; ++r)
            random_distribution_summary_pval(*p1++, twid);
        std::cout << std::endl;

        std::cout << std::setw(nwid) << std::left << "One level test (10%)";
        for (std::size_t r = 0; r != R; ++r)
            random_distribution_summary_pval(*p2++, twid);
        std::cout << std::endl;

        std::cout << std::setw(nwid) << std::left << "Two level test (2.5%)";
        for (std::size_t r = 0; r != R; ++r)
            random_distribution_summary_pval(*p3++, twid);
        std::cout << std::endl;

        std::cout << std::setw(nwid) << std::left << "Two level test (5%)";
        for (std::size_t r = 0; r != R; ++r)
            random_distribution_summary_pval(*p4++, twid);
        std::cout << std::endl;

        std::cout << std::setw(nwid) << std::left << "Two level test (10%)";
        for (std::size_t r = 0; r != R; ++r)
            random_distribution_summary_pval(*p5++, twid);
        std::cout << std::endl;
    }
    std::cout << std::string(lwid, '-') << std::endl;
}

template <typename MCKLDistType, typename ParamType, std::size_t ParamNum>
inline void random_distribution_test_pval(std::size_t N, std::size_t M,
    const std::array<ParamType, ParamNum> &param,
    mckl::Vector<std::string> &names,
    std::array<mckl::Vector<double>, 6> &pval)
{
    using result_type = typename MCKLDistType::result_type;
    using std_type = typename RandomDistributionTrait<MCKLDistType>::std_type;

    RandomDistributionTrait<MCKLDistType> trait;
    names.push_back(trait.name(param));

    MCKLRNGType rng;
    MCKLDistType dist_mckl(random_distribution_init<MCKLDistType>(param));
    std_type dist_std(random_distribution_init<std_type>(param));

    mckl::Vector<result_type> r(N);
    mckl::Vector<double> chi2(M);
    mckl::Vector<double> ksad(M);

    for (std::size_t i = 0; i != M; ++i) {
        for (std::size_t j = 0; j != N; ++j)
            r[j] = dist_std(rng);
        chi2[i] = random_distribution_chi2(N, r.data(), dist_mckl);
        ksad[i] = random_distribution_ksad(N, r.data(), dist_mckl);
    }
    random_distribution_pval(chi2, ksad, pval);

    for (std::size_t i = 0; i != M; ++i) {
        for (std::size_t j = 0; j != N; ++j)
            r[j] = dist_mckl(rng);
        chi2[i] = random_distribution_chi2(N, r.data(), dist_mckl);
        ksad[i] = random_distribution_ksad(N, r.data(), dist_mckl);
    }
    random_distribution_pval(chi2, ksad, pval);

    for (std::size_t i = 0; i != M; ++i) {
        mckl::rand(rng, dist_mckl, N, r.data());
        chi2[i] = random_distribution_chi2(N, r.data(), dist_mckl);
        ksad[i] = random_distribution_ksad(N, r.data(), dist_mckl);
    }
    random_distribution_pval(chi2, ksad, pval);

#if MCKL_HAS_MKL
    MKLRNGType random_mkl;
    for (std::size_t i = 0; i != M; ++i) {
        mckl::rand(random_mkl, dist_mckl, N, r.data());
        chi2[i] = random_distribution_chi2(N, r.data(), dist_mckl);
        ksad[i] = random_distribution_ksad(N, r.data(), dist_mckl);
    }
    random_distribution_pval(chi2, ksad, pval);
#endif
}

template <typename MCKLDistType>
inline void random_distribution_test_pval(
    std::size_t N, std::size_t M, int nwid, int twid)
{
    RandomDistributionTrait<MCKLDistType> trait;
    mckl::Vector<std::string> names;
    std::array<mckl::Vector<double>, 6> pval;
    auto params = trait.params();
    for (const auto &param : params)
        random_distribution_test_pval<MCKLDistType>(N, M, param, names, pval);
    random_distribution_summary_pval(names, pval, nwid, twid);
}

template <template <typename> class DistributionType>
inline void random_distribution_pval(
    std::size_t N, std::size_t M, int nwid, int twid, std::true_type)
{
    random_distribution_test_pval<DistributionType<float>>(N, M, nwid, twid);
    random_distribution_test_pval<DistributionType<double>>(N, M, nwid, twid);
}

template <template <typename> class DistributionType>
inline void random_distribution_pval(
    std::size_t N, std::size_t M, int nwid, int twid, std::false_type)
{
    random_distribution_test_pval<DistributionType<std::int32_t>>(
        N, M, nwid, twid);
    random_distribution_test_pval<DistributionType<std::uint32_t>>(
        N, M, nwid, twid);
    random_distribution_test_pval<DistributionType<std::int64_t>>(
        N, M, nwid, twid);
    random_distribution_test_pval<DistributionType<std::uint64_t>>(
        N, M, nwid, twid);
}

template <template <typename> class DistributionType, typename ResultType>
inline void random_distribution_pval(std::size_t N, std::size_t M)
{
    N = std::max(N, static_cast<std::size_t>(10000));
    M = std::max(M, static_cast<std::size_t>(10));

    const int nwid = 30;
    const int twid = 12;

    random_distribution_pval<DistributionType>(
        N, M, nwid, twid, std::is_floating_point<ResultType>());
}

struct RandomDistributionPerf {
    std::string name;
    bool pass;
    double e1;
    double e2;
    double c1;
    double c2;
    double c3;
    double c4;
}; // struct RandomDistributionPerf

template <typename T>
inline void random_distribution_perf_e(
    std::size_t, T *, T *, T &, T &, std::false_type)
{
    std::make_pair(mckl::const_zero<T>(), mckl::const_zero<T>());
}

template <typename T>
inline void random_distribution_perf_e(
    std::size_t n, T *r1, T *r2, T &e1, T &e2, std::true_type)
{
    mckl::sub(n, r1, r2, r1);
    mckl::div(n, r1, r2, r2);
    T f1 = 0;
    T f2 = 0;
    for (std::size_t i = 0; i != n; ++i)
        f1 = std::max(f1, r1[i]);
    for (std::size_t i = 0; i != n; ++i)
        f2 = std::max(f2, r2[i]);
    f1 /= std::numeric_limits<T>::epsilon();
    f2 /= std::numeric_limits<T>::epsilon();
    e1 = std::max(e1, f1);
    e2 = std::max(e2, f2);
}

template <typename MCKLDistType, typename ParamType, std::size_t ParamNum>
inline void random_distribution_perf_d(std::size_t N, std::size_t M,
    const std::array<ParamType, ParamNum> &param,
    mckl::Vector<RandomDistributionPerf> &perf)
{
    using result_type = typename MCKLDistType::result_type;

    RNG01<MCKLRNGType> rng01;
    MCKLRNGType rng;
    MCKLRNGType rng1;
    MCKLRNGType rng2;

    std::uniform_int_distribution<std::size_t> rsize(N / 2, N);
    MCKLDistType dist(random_distribution_init<MCKLDistType>(param));
    bool pass = true;

    result_type e1 = 0;
    result_type e2 = 0;
    mckl::Vector<result_type> r(N);
    mckl::Vector<result_type> r1(N);
    mckl::Vector<result_type> r2(N);
    for (std::size_t i = 0; i != M; ++i) {
        std::size_t K = rsize(rng);
        r.resize(K);
        r1.resize(K);
        r2.resize(K);

        mckl::rand(rng01, dist, K, r.data());
        for (std::size_t j = 0; j != K; ++j) {
            pass = pass && std::isfinite(r[j]);
            pass = pass && std::isfinite(dist(rng01));
        }

        constexpr bool invariant =
            RandomDistributionTrait<MCKLDistType>::invariant();
        if (invariant) {
            for (std::size_t j = 0; j != K; ++j)
                r1[j] = dist(rng1);
            mckl::rand(rng2, dist, K, r2.data());
            pass = pass && r1 == r2;
            random_distribution_perf_e(K, r1.data(), r2.data(), e1, e2,
                std::integral_constant<bool, invariant>());
        }

        std::stringstream ss1;
        ss1.precision(20);
        ss1 << dist;
        for (std::size_t j = 0; j != K; ++j)
            r1[j] = dist(rng1);
        ss1 >> dist;
        for (std::size_t j = 0; j != K; ++j)
            r2[j] = dist(rng2);
        pass = pass && r1 == r2;

        std::stringstream ssb;
        ssb << dist;
        mckl::rand(rng1, dist, K, r1.data());
        ssb >> dist;
        mckl::rand(rng2, dist, K, r2.data());
        pass = pass && r1 == r2;
    }

    RandomDistributionPerf result;
    result.pass = pass;
    result.e1 = static_cast<double>(e1);
    result.e2 = static_cast<double>(e2);
    perf.push_back(result);
}

template <typename MCKLDistType>
inline void random_distribution_perf_d(
    std::size_t N, std::size_t M, mckl::Vector<RandomDistributionPerf> &perf)
{
    RandomDistributionTrait<MCKLDistType> trait;
    auto params = trait.params();
    for (const auto &param : params)
        random_distribution_perf_d<MCKLDistType>(N, M, param, perf);
}

template <template <typename> class DistributionType>
inline void random_distribution_perf_d(std::size_t N, std::size_t M,
    mckl::Vector<RandomDistributionPerf> &perf, std::true_type)
{
    random_distribution_perf_d<DistributionType<float>>(N, M, perf);
    random_distribution_perf_d<DistributionType<double>>(N, M, perf);
}

template <template <typename> class DistributionType>
inline void random_distribution_perf_d(std::size_t N, std::size_t M,
    mckl::Vector<RandomDistributionPerf> &perf, std::false_type)
{
    random_distribution_perf_d<DistributionType<std::int32_t>>(N, M, perf);
    random_distribution_perf_d<DistributionType<std::uint32_t>>(N, M, perf);
    random_distribution_perf_d<DistributionType<std::int64_t>>(N, M, perf);
    random_distribution_perf_d<DistributionType<std::uint64_t>>(N, M, perf);
}

template <typename MCKLDistType, typename ParamType, std::size_t ParamNum>
inline void random_distribution_perf_p(std::size_t N, std::size_t M,
    const std::array<ParamType, ParamNum> &param,
    mckl::Vector<RandomDistributionPerf> &perf)
{
    using result_type = typename MCKLDistType::result_type;
    using std_type = typename RandomDistributionTrait<MCKLDistType>::std_type;

    MCKLRNGType rng;
#if MCKL_HAS_MKL
    MKLRNGType rng_mkl;
#endif

    std::uniform_int_distribution<std::size_t> rsize(N / 2, N);
    MCKLDistType dist_mckl(random_distribution_init<MCKLDistType>(param));
    std_type dist_std(random_distribution_init<std_type>(param));
    bool pass = true;

    mckl::Vector<result_type> r1(N);
    mckl::Vector<result_type> r2(N);
    mckl::Vector<result_type> r3(N);
#if MCKL_HAS_MKL
    mckl::Vector<result_type> r4(N);
#endif

    bool has_cycles = mckl::StopWatch::has_cycles();

    double c1 = has_cycles ? std::numeric_limits<double>::max() : 0.0;
    double c2 = has_cycles ? std::numeric_limits<double>::max() : 0.0;
    double c3 = has_cycles ? std::numeric_limits<double>::max() : 0.0;
#if MCKL_HAS_MKL
    double c4 = has_cycles ? std::numeric_limits<double>::max() : 0.0;
#endif
    for (std::size_t k = 0; k != 10; ++k) {
        std::size_t num = 0;
        mckl::StopWatch watch1;
        mckl::StopWatch watch2;
        mckl::StopWatch watch3;
#if MCKL_HAS_MKL
        mckl::StopWatch watch4;
#endif
        for (std::size_t i = 0; i != M; ++i) {
            std::size_t K = rsize(rng);
            num += K;
            r1.resize(K);
            r2.resize(K);
            r3.resize(K);
#if MCKL_HAS_MKL
            r4.resize(K);
#endif

            watch1.start();
            for (std::size_t j = 0; j != K; ++j)
                r1[j] = mckl::rand(rng, dist_std);
            watch1.stop();

            watch2.start();
            for (std::size_t j = 0; j != K; ++j)
                r2[j] = mckl::rand(rng, dist_mckl);
            watch2.stop();

            watch3.start();
            mckl::rand(rng, dist_mckl, K, r3.data());
            watch3.stop();

#if MCKL_HAS_MKL
            watch4.start();
            mckl::rand(rng_mkl, dist_mckl, K, r4.data());
            watch4.stop();
#endif

            pass = pass && r1 != r2;
            pass = pass && r1 != r3;
#if MCKL_HAS_MKL
            pass = pass && r1 != r4;
#endif
        }
        if (has_cycles) {
            c1 = std::min(c1, 1.0 * watch1.cycles() / num);
            c2 = std::min(c2, 1.0 * watch2.cycles() / num);
            c3 = std::min(c3, 1.0 * watch3.cycles() / num);
#if MCKL_HAS_MKL
            c4 = std::min(c4, 1.0 * watch4.cycles() / num);
#endif
        } else {
            c1 = std::max(c1, num / watch1.seconds() * 1e-6);
            c2 = std::max(c2, num / watch2.seconds() * 1e-6);
            c3 = std::max(c3, num / watch3.seconds() * 1e-6);
#if MCKL_HAS_MKL
            c4 = std::max(c4, num / watch4.seconds() * 1e-6);
#endif
        }
    }

    RandomDistributionTrait<MCKLDistType> trait;
    RandomDistributionPerf result;
    result.name = trait.name(param);
    result.pass = pass;
    result.c1 = c1;
    result.c2 = c2;
    result.c3 = c3;
#if MCKL_HAS_MKL
    result.c4 = c4;
#endif

    perf.push_back(result);
}

template <typename MCKLDistType>
inline void random_distribution_perf_p(
    std::size_t N, std::size_t M, mckl::Vector<RandomDistributionPerf> &perf)
{
    RandomDistributionTrait<MCKLDistType> trait;
    auto params = trait.params();
    for (const auto &param : params)
        random_distribution_perf_p<MCKLDistType>(N, M, param, perf);
}

template <template <typename> class DistributionType>
inline void random_distribution_perf_p(std::size_t N, std::size_t M,
    mckl::Vector<RandomDistributionPerf> &perf, std::true_type)
{
    random_distribution_perf_p<DistributionType<float>>(N, M, perf);
    random_distribution_perf_p<DistributionType<double>>(N, M, perf);
}

template <template <typename> class DistributionType>
inline void random_distribution_perf_p(std::size_t N, std::size_t M,
    mckl::Vector<RandomDistributionPerf> &perf, std::false_type)
{
    random_distribution_perf_p<DistributionType<std::int32_t>>(N, M, perf);
    random_distribution_perf_p<DistributionType<std::uint32_t>>(N, M, perf);
    random_distribution_perf_p<DistributionType<std::int64_t>>(N, M, perf);
    random_distribution_perf_p<DistributionType<std::uint64_t>>(N, M, perf);
}

template <template <typename> class DistributionType, typename ResultType>
inline void random_distribution_perf(std::size_t N, std::size_t M)
{
    N = std::max(N, static_cast<std::size_t>(2000));
    M = std::max(M, static_cast<std::size_t>(10));

    mckl::Vector<RandomDistributionPerf> perf_d;
    random_distribution_perf_d<DistributionType>(
        N, M, perf_d, std::is_floating_point<ResultType>());

    mckl::Vector<RandomDistributionPerf> perf_p;
    random_distribution_perf_p<DistributionType>(
        N, M, perf_p, std::is_floating_point<ResultType>());

    const int nwid = 30;
    const int twid = 12;
#if MCKL_HAS_MKL
    const std::size_t lwid = nwid + twid * 4 + 25;
#else
    const std::size_t lwid = nwid + twid * 3 + 25;
#endif

    std::cout << std::fixed << std::setprecision(2);

    std::cout << std::string(lwid, '=') << std::endl;

    if (mckl::StopWatch::has_cycles())
        std::cout << std::setw(nwid) << std::left << "Distribution (cpE)";
    else
        std::cout << std::setw(nwid) << std::left << "Distribution (ME/s)";
    std::cout << std::setw(twid) << std::right << "STD";
    std::cout << std::setw(twid) << std::right << "MCKL";
    std::cout << std::setw(twid) << std::right << "Batch";
#if MCKL_HAS_MKL
    std::cout << std::setw(twid) << std::right << "MKL";
#endif
    std::cout << std::setw(10) << std::right << "vMath";
    std::cout << std::setw(15) << std::right << "Deterministics";
    std::cout << std::endl;

    std::cout << std::string(lwid, '-') << std::endl;

#if MCKL_USE_MKL_VML
    std::string vmath = "VML";
#else
    std::string vmath = "C++";
#endif

    for (std::size_t i = 0; i != perf_p.size(); ++i) {
        std::cout << std::setw(nwid) << std::left << perf_p[i].name;
        std::cout << std::setw(twid) << std::right << perf_p[i].c1;
        std::cout << std::setw(twid) << std::right << perf_p[i].c2;
        std::cout << std::setw(twid) << std::right << perf_p[i].c3;
#if MCKL_HAS_MKL
        std::cout << std::setw(twid) << std::right << perf_p[i].c4;
#endif
        std::cout << std::setw(10) << std::right << vmath;

        std::stringstream ss;
        ss << "(";
        ss << std::min(perf_d[i].e1, perf_d[i].e2);
        ss << ")";

        std::string pass;
        pass += ss.str();
        pass += perf_d[i].pass ? "-" : "*";
        pass += perf_p[i].pass ? "-" : "*";
        pass += random_pass(perf_d[i].pass && perf_p[i].pass);
        std::cout << std::setw(15) << std::right << pass;
        std::cout << std::endl;
    }

    std::cout << std::string(lwid, '-') << std::endl;
}

template <template <typename> class DistributionType, typename ResultType>
inline void random_distribution(std::size_t N, std::size_t M)
{
    random_distribution_pval<DistributionType, ResultType>(N, M);
    random_distribution_perf<DistributionType, ResultType>(N, M);
}

#endif // MCKL_EXAMPLE_RANDOM_DISTRIBUTION_HPP
