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

#include <mckl/random/distribution.hpp>
#include <boost/math/distributions.hpp>
#include <boost/random.hpp>
#include "random_common.hpp"

#define MCKL_DEFINE_EXAMPLE_RANDOM_DISTRIBUTION_TEST_REAL(test)               \
    random_distribution_test_##test<mckl::ArcsineDistribution<RealType>>(     \
        N, M, nwid, twid);                                                    \
    random_distribution_test_##test<mckl::BetaDistribution<RealType>>(        \
        N, M, nwid, twid);                                                    \
    random_distribution_test_##test<mckl::CauchyDistribution<RealType>>(      \
        N, M, nwid, twid);                                                    \
    random_distribution_test_##test<mckl::ChiSquaredDistribution<RealType>>(  \
        N, M, nwid, twid);                                                    \
    random_distribution_test_##test<mckl::ExponentialDistribution<RealType>>( \
        N, M, nwid, twid);                                                    \
    random_distribution_test_##test<                                          \
        mckl::ExtremeValueDistribution<RealType>>(N, M, nwid, twid);          \
    random_distribution_test_##test<mckl::FisherFDistribution<RealType>>(     \
        N, M, nwid, twid);                                                    \
    random_distribution_test_##test<mckl::GammaDistribution<RealType>>(       \
        N, M, nwid, twid);                                                    \
    random_distribution_test_##test<mckl::LaplaceDistribution<RealType>>(     \
        N, M, nwid, twid);                                                    \
    random_distribution_test_##test<mckl::LevyDistribution<RealType>>(        \
        N, M, nwid, twid);                                                    \
    random_distribution_test_##test<mckl::LogisticDistribution<RealType>>(    \
        N, M, nwid, twid);                                                    \
    random_distribution_test_##test<mckl::LognormalDistribution<RealType>>(   \
        N, M, nwid, twid);                                                    \
    random_distribution_test_##test<mckl::NormalDistribution<RealType>>(      \
        N, M, nwid, twid);                                                    \
    random_distribution_test_##test<mckl::ParetoDistribution<RealType>>(      \
        N, M, nwid, twid);                                                    \
    random_distribution_test_##test<mckl::RayleighDistribution<RealType>>(    \
        N, M, nwid, twid);                                                    \
    random_distribution_test_##test<mckl::StudentTDistribution<RealType>>(    \
        N, M, nwid, twid);                                                    \
    random_distribution_test_##test<mckl::U01Distribution<RealType>>(         \
        N, M, nwid, twid);                                                    \
    random_distribution_test_##test<mckl::UniformRealDistribution<RealType>>( \
        N, M, nwid, twid);                                                    \
    random_distribution_test_##test<mckl::WeibullDistribution<RealType>>(     \
        N, M, nwid, twid);

#define MCKL_DEFINE_EXAMPLE_RANDOM_DISTRIBUTION_TEST_INT(test)                \
    random_distribution_test_##test<mckl::GeometricDistribution<IntType>>(    \
        N, M, nwid, twid);                                                    \
    random_distribution_test_##test<mckl::UniformIntDistribution<IntType>>(   \
        N, M, nwid, twid);

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

    static const std::size_t param_num = ParamNum;

    virtual std::string dist_name() const = 0;

    template <typename ParamType>
    std::string name(const std::array<ParamType, ParamNum> &param) const
    {
        return name_dispatch(dist_name(), param);
    }

    protected:
    template <typename QuantileType>
    mckl::Vector<ResultType> partition_quantile(
        std::size_t n, QuantileType &&quantile) const
    {
        const std::size_t k = n < 2000 ? n / 100 : 20; // The number of cells
        const double p = 1.0 / k;
        mckl::Vector<ResultType> partition(k - 1);
        for (std::size_t i = 0; i != k - 1; ++i)
            partition[i] = quantile(p * (i + 1));

        return partition;
    }

    template <typename BoostDistType>
    mckl::Vector<ResultType> partition_boost(
        std::size_t n, BoostDistType &&dist) const
    {
        return partition_quantile<ResultType>(n, [&](double p) {
            return boost::math::quantile(
                std::forward<BoostDistType>(dist), static_cast<ResultType>(p));
        });
    }

    template <typename ParamType>
    static void add_param(mckl::Vector<std::array<ParamType, 0>> &params)
    {
        std::array<ParamType, 0> tmp;
        params.push_back(tmp);
    }

    template <typename ParamType, typename ValueType1>
    static void add_param(
        mckl::Vector<std::array<ParamType, 1>> &param, ValueType1 p1)
    {
        std::array<ParamType, 1> tmp;
        tmp[0] = static_cast<ParamType>(p1);
        param.push_back(tmp);
    }

    template <typename ParamType, typename ValueType1, typename ValueType2>
    static void add_param(mckl::Vector<std::array<ParamType, 2>> &param,
        ValueType1 p1, ValueType2 p2)
    {
        std::array<ParamType, 2> tmp;
        tmp[0] = static_cast<ParamType>(p1);
        tmp[1] = static_cast<ParamType>(p2);
        param.push_back(tmp);
    }

    private:
    template <typename ParamType>
    static std::string name_dispatch(
        const std::string &dist_name, const std::array<ParamType, 0> &)
    {
        std::stringstream ss;
        ss << dist_name << "<" << random_typename<ResultType>() << ">";

        return ss.str();
    }

    template <typename ParamType>
    static std::string name_dispatch(
        const std::string &dist_name, const std::array<ParamType, 1> &param)
    {
        std::stringstream ss;
        ss << dist_name << "<" << random_typename<ResultType>() << ">("
           << param[0] << ")";

        return ss.str();
    }

    template <typename ParamType>
    static std::string name_dispatch(
        const std::string &dist_name, const std::array<ParamType, 2> &param)
    {
        std::stringstream ss;
        ss << dist_name << "<" << random_typename<ResultType>() << ">("
           << param[0] << "," << param[1] << ")";

        return ss.str();
    }
};

template <typename DistType>
class RandomDistributionTrait;

template <typename RealType>
class RandomDistributionTrait<mckl::ArcsineDistribution<RealType>>
    : public RandomDistributionTraitBase<RealType, 2>
{
    public:
    using dist_type = mckl::ArcsineDistribution<RealType>;
    using std_type = dist_type;

    std::string dist_name() const { return "Arcsine"; }

    mckl::Vector<RealType> partition(
        std::size_t n, const dist_type &dist) const
    {
        return this->partition_quantile(n, [&](double p) {
            double r = std::sin(mckl::const_pi_by2<double>() * p);
            return dist.a() +
                (dist.b() - dist.a()) * static_cast<RealType>(r * r);
        });
    }

    mckl::Vector<std::array<RealType, 2>> params() const
    {
        mckl::Vector<std::array<RealType, 2>> params;
        this->add_param(params, 0, 1);

        return params;
    }
};

template <typename RealType>
class RandomDistributionTrait<mckl::BetaDistribution<RealType>>
    : public RandomDistributionTraitBase<RealType, 2>
{
    public:
    using dist_type = mckl::BetaDistribution<RealType>;
    using std_type = boost::random::beta_distribution<RealType>;

    std::string dist_name() const { return "Beta"; }

    mckl::Vector<RealType> partition(
        std::size_t n, const dist_type &dist) const
    {
        return this->partition_boost(
            n, boost::math::beta_distribution<RealType>(
                   dist.alpha(), dist.beta()));
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
};

template <typename RealType>
class RandomDistributionTrait<mckl::CauchyDistribution<RealType>>
    : public RandomDistributionTraitBase<RealType, 2>
{
    public:
    using dist_type = mckl::CauchyDistribution<RealType>;
    using std_type = std::cauchy_distribution<RealType>;

    std::string dist_name() const { return "Cauchy"; }

    mckl::Vector<RealType> partition(
        std::size_t n, const dist_type &dist) const
    {
        return this->partition_quantile(n, [&](double p) {
            return dist.a() +
                dist.b() * static_cast<RealType>(
                               std::tan(mckl::const_pi<double>() * (p - 0.5)));
        });
    }

    mckl::Vector<std::array<RealType, 2>> params() const
    {
        mckl::Vector<std::array<RealType, 2>> params;
        this->add_param(params, 0, 1);

        return params;
    }
};

template <typename RealType>
class RandomDistributionTrait<mckl::GammaDistribution<RealType>>
    : public RandomDistributionTraitBase<RealType, 2>
{
    public:
    using dist_type = mckl::GammaDistribution<RealType>;
    using std_type = std::gamma_distribution<RealType>;

    std::string dist_name() const { return "Gamma"; }

    mckl::Vector<RealType> partition(std::size_t n, const dist_type &dist)
    {
        return this->partition_boost(
            n, boost::math::gamma_distribution<RealType>(
                   dist.alpha(), dist.beta()));
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
};

template <typename RealType>
class RandomDistributionTrait<mckl::ChiSquaredDistribution<RealType>>
    : public RandomDistributionTraitBase<RealType, 1>
{
    public:
    using dist_type = mckl::ChiSquaredDistribution<RealType>;
    using std_type = std::chi_squared_distribution<RealType>;

    std::string dist_name() const { return "ChiSquared"; }

    mckl::Vector<RealType> partition(std::size_t n, const dist_type &dist)
    {
        return this->partition_boost(
            n, boost::math::chi_squared_distribution<RealType>(dist.n()));
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
};

template <typename RealType>
class RandomDistributionTrait<mckl::ExponentialDistribution<RealType>>
    : public RandomDistributionTraitBase<RealType, 1>
{
    public:
    using dist_type = mckl::ExponentialDistribution<RealType>;
    using std_type = std::exponential_distribution<RealType>;

    std::string dist_name() const { return "Exponential"; }

    mckl::Vector<RealType> partition(std::size_t n, const dist_type &dist)
    {
        return this->partition_quantile(n, [&](double p) {
            return -static_cast<RealType>(std::log(1 - p)) / dist.lambda();
        });
    }

    mckl::Vector<std::array<RealType, 1>> params() const
    {
        mckl::Vector<std::array<RealType, 1>> params;
        this->add_param(params, 1);

        return params;
    }
};

template <typename RealType>
class RandomDistributionTrait<mckl::ExtremeValueDistribution<RealType>>
    : public RandomDistributionTraitBase<RealType, 2>
{
    public:
    using dist_type = mckl::ExtremeValueDistribution<RealType>;
    using std_type = std::extreme_value_distribution<RealType>;

    std::string dist_name() const { return "ExtremeValue"; }

    mckl::Vector<RealType> partition(std::size_t n, const dist_type &dist)
    {
        return this->partition_quantile(n, [&](double p) {
            return dist.a() -
                dist.b() * static_cast<RealType>(std::log(-std::log(p)));
        });
    }

    mckl::Vector<std::array<RealType, 2>> params() const
    {
        mckl::Vector<std::array<RealType, 2>> params;
        this->add_param(params, 0, 1);

        return params;
    }
};

template <typename RealType>
class RandomDistributionTrait<mckl::FisherFDistribution<RealType>>
    : public RandomDistributionTraitBase<RealType, 2>
{
    public:
    using dist_type = mckl::FisherFDistribution<RealType>;
    using std_type = std::fisher_f_distribution<RealType>;

    std::string dist_name() const { return "FisherF"; }

    mckl::Vector<RealType> partition(std::size_t n, const dist_type &dist)
    {
        return this->partition_boost(n,
            boost::math::fisher_f_distribution<RealType>(dist.m(), dist.n()));
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
};

template <typename RealType>
class RandomDistributionTrait<mckl::LaplaceDistribution<RealType>>
    : public RandomDistributionTraitBase<RealType, 2>
{
    public:
    using dist_type = mckl::LaplaceDistribution<RealType>;
    using std_type = boost::random::laplace_distribution<RealType>;

    std::string dist_name() const { return "Laplace"; }

    mckl::Vector<RealType> partition(std::size_t n, const dist_type &dist)
    {
        return this->partition_quantile(n, [&](double p) {
            double q = p - 0.5;
            return q > 0 ?
                dist.a() -
                    dist.b() * static_cast<RealType>(std::log(1 - 2 * q)) :
                dist.a() +
                    dist.b() * static_cast<RealType>(std::log(1 + 2 * q));
        });
    }

    mckl::Vector<std::array<RealType, 2>> params() const
    {
        mckl::Vector<std::array<RealType, 2>> params;
        this->add_param(params, 0, 1);

        return params;
    }
};

template <typename RealType>
class RandomDistributionTrait<mckl::LevyDistribution<RealType>>
    : public RandomDistributionTraitBase<RealType, 2>
{
    public:
    using dist_type = mckl::LevyDistribution<RealType>;
    using std_type = dist_type;

    std::string dist_name() const { return "Levy"; }

    mckl::Vector<RealType> partition(std::size_t n, const dist_type &dist)
    {
        boost::math::normal_distribution<RealType> normal(0, 1);
        return this->partition_quantile(n, [&](double p) {
            RealType q = boost::math::quantile(
                normal, 1 - static_cast<RealType>(p) / 2);
            return dist.a() + dist.b() / static_cast<RealType>(q * q);
        });
    }

    mckl::Vector<std::array<RealType, 2>> params() const
    {
        mckl::Vector<std::array<RealType, 2>> params;
        this->add_param(params, 0, 1);

        return params;
    }
};

template <typename RealType>
class RandomDistributionTrait<mckl::LogisticDistribution<RealType>>
    : public RandomDistributionTraitBase<RealType, 2>
{
    public:
    using dist_type = mckl::LogisticDistribution<RealType>;
    using std_type = dist_type;

    std::string dist_name() const { return "Logistic"; }

    mckl::Vector<RealType> partition(std::size_t n, const dist_type &dist)
    {
        return this->partition_quantile(n, [&](double p) {
            return dist.a() +
                dist.b() * static_cast<RealType>(std::log(p / (1 - p)));
        });
    }

    mckl::Vector<std::array<RealType, 2>> params() const
    {
        mckl::Vector<std::array<RealType, 2>> params;
        this->add_param(params, 0, 1);

        return params;
    }
};

template <typename RealType>
class RandomDistributionTrait<mckl::LognormalDistribution<RealType>>
    : public RandomDistributionTraitBase<RealType, 2>
{
    public:
    using dist_type = mckl::LognormalDistribution<RealType>;
    using std_type = std::lognormal_distribution<RealType>;

    std::string dist_name() const { return "Lognormal"; }

    mckl::Vector<RealType> partition(std::size_t n, const dist_type &dist)
    {
        return this->partition_boost(n,
            boost::math::lognormal_distribution<RealType>(dist.m(), dist.s()));
    }

    mckl::Vector<std::array<RealType, 2>> params() const
    {
        mckl::Vector<std::array<RealType, 2>> params;
        this->add_param(params, 0, 1);

        return params;
    }
};

template <typename RealType>
class RandomDistributionTrait<mckl::NormalDistribution<RealType>>
    : public RandomDistributionTraitBase<RealType, 2>
{
    public:
    using dist_type = mckl::NormalDistribution<RealType>;
    using std_type = std::normal_distribution<RealType>;

    std::string dist_name() const { return "Normal"; }

    mckl::Vector<RealType> partition(std::size_t n, const dist_type &dist)
    {
        return this->partition_boost(
            n, boost::math::normal_distribution<RealType>(
                   dist.mean(), dist.stddev()));
    }

    mckl::Vector<std::array<RealType, 2>> params() const
    {
        mckl::Vector<std::array<RealType, 2>> params;
        this->add_param(params, 0, 1);

        return params;
    }
};

template <typename RealType>
class RandomDistributionTrait<mckl::ParetoDistribution<RealType>>
    : public RandomDistributionTraitBase<RealType, 2>
{
    public:
    using dist_type = mckl::ParetoDistribution<RealType>;
    using std_type = dist_type;

    std::string dist_name() const { return "Pareto"; }

    mckl::Vector<RealType> partition(std::size_t n, const dist_type &dist)
    {
        return this->partition_quantile(n, [&](double p) {
            return dist.b() /
                std::exp(static_cast<RealType>(std::log(1 - p)) / dist.a());
        });
    }

    mckl::Vector<std::array<RealType, 2>> params() const
    {
        mckl::Vector<std::array<RealType, 2>> params;
        this->add_param(params, 1, 1);

        return params;
    }
};

template <typename RealType>
class RandomDistributionTrait<mckl::RayleighDistribution<RealType>>
    : public RandomDistributionTraitBase<RealType, 1>
{
    public:
    using dist_type = mckl::RayleighDistribution<RealType>;
    using std_type = dist_type;

    std::string dist_name() const { return "Rayleigh"; }

    mckl::Vector<RealType> partition(std::size_t n, const dist_type &dist)
    {
        return this->partition_quantile(n, [&](double p) {
            return std::sqrt(-2 * static_cast<RealType>(std::log(1 - p)) *
                dist.sigma() * dist.sigma());
        });
    }

    mckl::Vector<std::array<RealType, 1>> params() const
    {
        mckl::Vector<std::array<RealType, 1>> params;
        this->add_param(params, 1);

        return params;
    }
};

template <typename RealType>
class RandomDistributionTrait<mckl::StudentTDistribution<RealType>>
    : public RandomDistributionTraitBase<RealType, 1>
{
    public:
    using dist_type = mckl::StudentTDistribution<RealType>;
    using std_type = std::student_t_distribution<RealType>;

    std::string dist_name() const { return "StudentT"; }

    mckl::Vector<RealType> partition(std::size_t n, const dist_type &dist)
    {
        return this->partition_boost(
            n, boost::math::students_t_distribution<RealType>(dist.n()));
    }

    mckl::Vector<std::array<RealType, 1>> params() const
    {
        RandomDistributionTrait<mckl::ChiSquaredDistribution<RealType>>
            chi_trait;

        return chi_trait.params();
    }
};

template <typename RealType>
class RandomDistributionTrait<mckl::U01Distribution<RealType>>
    : public RandomDistributionTraitBase<RealType, 0>
{
    public:
    using dist_type = mckl::U01Distribution<RealType>;
    using std_type = std::uniform_real_distribution<RealType>;

    std::string dist_name() const { return "U01"; }

    mckl::Vector<RealType> partition(std::size_t n, const dist_type &)
    {
        return this->partition_quantile(
            n, [&](double p) { return static_cast<RealType>(p); });
    }

    mckl::Vector<std::array<RealType, 0>> params() const
    {
        return mckl::Vector<std::array<RealType, 0>>(1);
    }
};

template <typename RealType>
class RandomDistributionTrait<mckl::UniformRealDistribution<RealType>>
    : public RandomDistributionTraitBase<RealType, 2>
{
    public:
    using dist_type = mckl::UniformRealDistribution<RealType>;
    using std_type = std::uniform_real_distribution<RealType>;

    std::string dist_name() const { return "UniformReal"; }

    mckl::Vector<RealType> partition(std::size_t n, const dist_type &dist)
    {
        return this->partition_quantile(n, [&](double p) {
            return dist.a() + static_cast<RealType>(p) * (dist.b() - dist.a());
        });
    }

    mckl::Vector<std::array<RealType, 2>> params() const
    {
        mckl::Vector<std::array<RealType, 2>> params;
        this->add_param(params, -0.5, 0.5);

        return params;
    }
};

template <typename RealType>
class RandomDistributionTrait<mckl::WeibullDistribution<RealType>>
    : public RandomDistributionTraitBase<RealType, 2>
{
    public:
    using dist_type = mckl::WeibullDistribution<RealType>;
    using std_type = std::weibull_distribution<RealType>;

    std::string dist_name() const { return "Weibull"; }

    mckl::Vector<RealType> partition(std::size_t n, const dist_type &dist)
    {
        return this->partition_quantile(n, [&](double p) {
            return dist.b() * std::pow(-static_cast<RealType>(std::log(1 - p)),
                                  1 / dist.a());
        });
    }

    mckl::Vector<std::array<RealType, 2>> params() const
    {
        mckl::Vector<std::array<RealType, 2>> params;
        this->add_param(params, 1, 1);

        return params;
    }
};

template <typename IntType>
class RandomDistributionTrait<mckl::GeometricDistribution<IntType>>
    : public RandomDistributionTraitBase<IntType, 1>
{
    public:
    using dist_type = mckl::GeometricDistribution<IntType>;
    using std_type = std::geometric_distribution<IntType>;

    std::string dist_name() const { return "Geometric"; }

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

        return partition;
    }

    mckl::Vector<double> pmf(std::size_t n, const dist_type &dist)
    {
        const IntType k = static_cast<IntType>(
            std::ceil((std::log(5.0 / n) - std::log(dist.p())) /
                std::log(1 - dist.p())));

        mckl::Vector<double> pmf;
        pmf.push_back(dist.p());
        for (IntType i = 1; i <= k; ++i)
            pmf.push_back(pmf.back() * (1 - dist.p()));

        return pmf;
    }
};

template <typename IntType>
class RandomDistributionTrait<mckl::UniformIntDistribution<IntType>>
    : public RandomDistributionTraitBase<IntType, 2>
{
    public:
    using dist_type = mckl::UniformIntDistribution<IntType>;
    using std_type = std::uniform_int_distribution<IntType>;

    std::string dist_name() const { return "UniformInt"; }

    mckl::Vector<std::array<IntType, 2>> params() const
    {
        return params(std::is_signed<IntType>());
    }

    mckl::Vector<IntType> partition(std::size_t, const dist_type &dist)
    {
        IntType a = dist.a();
        IntType b = dist.b();
        mckl::Vector<IntType> partition;
        for (IntType i = a; i < b; ++i)
            partition.push_back(i);

        return partition;
    }

    mckl::Vector<double> pmf(std::size_t, const dist_type &dist)
    {
        IntType a = dist.a();
        IntType b = dist.b();
        const double p = 1.0 / (b - a + 1);

        return mckl::Vector<double>(static_cast<std::size_t>(b - a), p);
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
};

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

template <typename MCKLDistType>
inline double random_distribution_chi2(std::size_t n,
    const typename MCKLDistType::result_type *r, const MCKLDistType &dist,
    std::true_type)
{
    using result_type = typename MCKLDistType::result_type;

    mckl::Vector<result_type> rval(r, r + n);
    std::sort(rval.begin(), rval.end());

    RandomDistributionTrait<MCKLDistType> trait;
    mckl::Vector<result_type> partition(trait.partition(n, dist));

    const std::size_t k = partition.size() + 1;
    mckl::Vector<double> count(k);
    std::size_t j = 0;
    for (std::size_t i = 0; i != k - 1; ++i) {
        std::size_t c = 0;
        while (j != rval.size() && rval[j] <= partition[i]) {
            ++c;
            ++j;
        }
        count[i] = static_cast<double>(c);
    }
    count.back() = static_cast<double>(rval.size() - j);

    const double np = static_cast<double>(n) / k;
    mckl::sub(k, count.data(), np, count.data());
    mckl::sqr(k, count.data(), count.data());
    const double s = std::accumulate(count.begin(), count.end(), 0.0) / np;
    boost::math::chi_squared_distribution<double> chi2(
        static_cast<double>(k - 1));

    return boost::math::cdf(chi2, s);
}

template <typename MCKLDistType>
inline double random_distribution_chi2(std::size_t n,
    const typename MCKLDistType::result_type *r, const MCKLDistType &dist,
    std::false_type)
{
    using result_type = typename MCKLDistType::result_type;

    mckl::Vector<result_type> rval(r, r + n);
    std::sort(rval.begin(), rval.end());

    RandomDistributionTrait<MCKLDistType> trait;
    mckl::Vector<result_type> partition(trait.partition(n, dist));
    mckl::Vector<double> pmf(trait.pmf(n, dist));

    const std::size_t k = partition.size() + 1;
    mckl::Vector<double> count(k);
    std::size_t j = 0;
    for (std::size_t i = 0; i != k - 1; ++i) {
        std::size_t c = 0;
        while (j != rval.size() && rval[j] == partition[i]) {
            ++c;
            ++j;
        }
        count[i] = static_cast<double>(c);
    }
    count.back() = static_cast<double>(rval.size() - j);
    pmf.push_back(1 - std::accumulate(pmf.begin(), pmf.end(), 0.0));

    mckl::mul(k, static_cast<double>(n), pmf.data(), pmf.data());
    mckl::sub(k, count.data(), pmf.data(), count.data());
    mckl::sqr(k, count.data(), count.data());
    mckl::div(k, count.data(), pmf.data(), count.data());
    const double s = std::accumulate(count.begin(), count.end(), 0.0);
    boost::math::chi_squared_distribution<double> chi2(
        static_cast<double>(k - 1));

    return boost::math::cdf(chi2, s);
}

template <typename MCKLDistType>
inline double random_distribution_chi2(std::size_t n,
    const typename MCKLDistType::result_type *r, const MCKLDistType &dist)
{
    return random_distribution_chi2(n, r, dist,
        std::is_floating_point<typename MCKLDistType::result_type>());
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
        ss << "*";
    ss << pval << "%";
    std::cout << std::setw(twid) << std::right << ss.str();
}

inline void random_distribution_summary_pval(
    const mckl::Vector<std::string> &names,
    const std::array<mckl::Vector<double>, 6> &pval, int nwid, int twid)
{
    std::size_t D = names.size();
    std::size_t R = pval[0].size() / D;
    std::size_t lwid =
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

    mckl::RNG rng;
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
    mckl::MKL_SFMT19937 random_mkl;
    for (std::size_t i = 0; i != M; ++i) {
        mckl::rand(random_mkl, dist_mckl, N, r.data());
        chi2[i] = random_distribution_chi2(N, r.data(), dist_mckl);
        ksad[i] = random_distribution_ksad(N, r.data(), dist_mckl);
    }
    random_distribution_pval(chi2, ksad, pval);
#endif // MCKL_HAS_MKL
}

template <typename MCKLDistType>
inline void random_distribution_test_pval(
    std::size_t N, std::size_t M, int nwid, int twid)
{
    RandomDistributionTrait<MCKLDistType> trait;
    auto params = trait.params();

    mckl::Vector<std::string> names;
    std::array<mckl::Vector<double>, 6> pval;
    for (const auto &param : params)
        random_distribution_test_pval<MCKLDistType>(N, M, param, names, pval);
    random_distribution_summary_pval(names, pval, nwid, twid);
}

template <typename RealType>
inline void random_distribution_pval_real(
    std::size_t N, std::size_t M, int nwid, int twid)
{
    MCKL_DEFINE_EXAMPLE_RANDOM_DISTRIBUTION_TEST_REAL(pval);
}

template <typename IntType>
inline void random_distribution_pval_int(
    std::size_t N, std::size_t M, int nwid, int twid)
{
    MCKL_DEFINE_EXAMPLE_RANDOM_DISTRIBUTION_TEST_INT(pval);
}

inline void random_distribution(std::size_t N, std::size_t M)
{
    N = std::max(N, static_cast<std::size_t>(10000));
    M = std::max(M, static_cast<std::size_t>(10));

    int nwid = 30;
    int twid = 12;

    // random_distribution_pval_real<float>(N, M, nwid, twid);
    // random_distribution_pval_real<double>(N, M, nwid, twid);
    random_distribution_pval_int<int>(N, M, nwid, twid);
    random_distribution_pval_int<unsigned>(N, M, nwid, twid);
}

#endif // MCKL_EXAMPLE_RANDOM_DISTRIBUTION_HPP
