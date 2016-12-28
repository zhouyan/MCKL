//============================================================================
// MCKL/include/mckl/random/dirichlet_distribution.hpp
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

#ifndef MCKL_RANDOM_DIRICHLET_DISTRIBUTION_HPP
#define MCKL_RANDOM_DIRICHLET_DISTRIBUTION_HPP

#include <mckl/random/internal/common.hpp>
#include <mckl/random/gamma_distribution.hpp>

namespace mckl
{

namespace internal
{

template <typename RealType>
inline bool dirichlet_distribution_check_param(
    std::size_t dim, RealType *alpha)
{
    for (std::size_t i = 0; i != dim; ++i)
        if (alpha <= 0)
            return false;
    return true;
}

template <typename RealType>
inline void dirichlet_distribution_avg(
    std::size_t n, std::size_t dim, RealType *r)
{
    for (std::size_t i = 0; i != n; ++i, r += dim) {
        RealType s = std::accumulate(r, r + dim, const_zero<RealType>());
        mul<RealType>(dim, 1 / s, r, r);
    }
}

} // namespace mckl::internal

template <typename RealType, typename RNGType>
inline void dirichlet_distribution(RNGType &rng, std::size_t n, RealType *r,
    std::size_t dim, const RealType alpha)
{
    if (n * dim == 0)
        return;

    gamma_distribution(rng, n * dim, r, alpha, const_one<RealType>());
    internal::dirichlet_distribution_avg(n, dim, r);
}

template <typename RealType, typename RNGType>
inline void dirichlet_distribution(RNGType &rng, std::size_t n, RealType *r,
    std::size_t dim, const RealType *alpha)
{
    if (n * dim == 0)
        return;

    Vector<RealType> s(n);
    for (std::size_t i = 0; i != dim; ++i) {
        gamma_distribution(rng, n, s.data(), alpha[i], const_one<RealType>());
        RealType *t = r + i;
        for (std::size_t j = 0; j != n; ++j, t += dim)
            *t = s[j];
    }
    internal::dirichlet_distribution_avg(n, dim, r);
}

template <typename RealType>
class DirichletDistribution
{
    MCKL_DEFINE_RANDOM_DISTRIBUTION_ASSERT_REAL_TYPE(Dirichlet)

    public:
    using result_type = RealType;
    using distribution_type = DirichletDistribution<RealType>;

    class param_type
    {
        public:
        using result_type = RealType;
        using distribution_type = DirichletDistribution<RealType>;

        explicit param_type(std::size_t dim = 1)
            : alpha_(dim, 1), is_scalar_(true)
        {
            runtime_assert(internal::dirichlet_distribution_check_param(
                               this->dim(), this->alpha()),
                "**DirichletDistribution** constructed with invalid "
                "arguments");
        }

        param_type(std::size_t dim, result_type alpha)
            : alpha_(dim, alpha), is_scalar_(true)
        {
            runtime_assert(internal::dirichlet_distribution_check_param(
                               this->dim(), this->alpha()),
                "**DirichletDistribution** constructed with invalid "
                "arguments");
        }

        param_type(std::size_t dim, const result_type *alpha)
            : alpha_(alpha, alpha + dim), is_scalar_(false)
        {
            runtime_assert(internal::dirichlet_distribution_check_param(
                               this->dim(), this->alpha()),
                "**DirichletDistribution** constructed with invalid "
                "arguments");
        }

        std::size_t dim() const { return alpha_.size(); }

        const result_type *alpha() const { return alpha_.data(); }

        friend bool operator==(
            const param_type &param1, const param_type &param2)
        {
            if (param1.alpha_ != param2.alpha_)
                return false;
            if (param1.is_scalar_ != param2.is_scalar_)
                return false;
            return true;
        }

        friend bool operator!=(
            const param_type &param1, const param_type &param2)
        {
            return !(param1 == param2);
        }

        template <typename CharT, typename Traits>
        friend std::basic_ostream<CharT, Traits> &operator<<(
            std::basic_ostream<CharT, Traits> &os, const param_type &param)
        {
            if (!os)
                return os;

            os << param.alpha_ << ' ';
            os << param.is_scalar_;

            return os;
        }

        template <typename CharT, typename Traits>
        friend std::basic_istream<CharT, Traits> &operator>>(
            std::basic_istream<CharT, Traits> &is, param_type &param)
        {
            if (!is)
                return is;

            param_type tmp;
            is >> std::ws >> tmp.alpha_;
            is >> std::ws >> tmp.is_scalar_;

            if (is)
                param = std::move(tmp);
            else
                is.setstate(std::ios_base::failbit);

            return is;
        }

        private:
        Vector<result_type> alpha_;
        bool is_scalar_;

        friend distribution_type;
    }; // class param_type

    /// \brief Construct a distribution with scalar shape
    DirichletDistribution(std::size_t dim = 1) : param_(dim) {}

    /// \brief Construct a distribution with scalar shape
    DirichletDistribution(std::size_t dim, result_type alpha)
        : param_(dim, alpha)
    {
        reset();
    }

    /// \brief Construct a distribution with vector shapes
    DirichletDistribution(std::size_t dim, const result_type *alpha)
        : param_(dim, alpha)
    {
        reset();
    }

    explicit DirichletDistribution(const param_type &param) : param_(param)
    {
        reset();
    }

    explicit DirichletDistribution(param_type &&param)
        : param_(std::move(param))
    {
        reset();
    }

    template <typename OutputIter>
    OutputIter min(OutputIter first) const
    {
        return std::fill_n(first, dim(), 0);
    }

    template <typename OutputIter>
    OutputIter max(OutputIter first) const
    {
        return std::fill_n(first, dim(), 1);
    }

    void reset() {}

    std::size_t dim() const { return param_.dim(); }

    const result_type *alpha() const { return param_.alpha(); }

    const param_type &param() const { return param_; }

    void param(const param_type &param)
    {
        param_ = param;
        reset();
    }

    void param(param_type &&param)
    {
        param_ = std::move(param);
        reset();
    }

    template <typename RNGType>
    void operator()(RNGType &rng, result_type *r)
    {
        operator()(rng, r, param_);
    }

    template <typename RNGType>
    void operator()(RNGType &rng, result_type *r, const param_type &param)
    {
        generate(rng, r, param);
    }

    template <typename RNGType>
    void operator()(RNGType &rng, std::size_t n, result_type *r)
    {
        operator()(rng, n, r, param_);
    }

    template <typename RNGType>
    void operator()(
        RNGType &rng, std::size_t n, result_type *r, const param_type &param)
    {
        if (param.is_scalar_)
            dirichlet_distribution(rng, n, r, param.dim(), param.alpha()[0]);
        else
            dirichlet_distribution(rng, n, r, param.dim(), param.alpha());
    }

    friend bool operator==(
        const distribution_type &dist1, const distribution_type &dist2)
    {
        if (dist1.param_ != dist2.param_)
            return false;
        return true;
    }

    friend bool operator!=(
        const distribution_type &dist1, const distribution_type &dist2)
    {
        return !(dist1 == dist2);
    }

    template <typename CharT, typename Traits>
    friend std::basic_ostream<CharT, Traits> &operator<<(
        std::basic_ostream<CharT, Traits> &os, const distribution_type &dist)
    {
        if (!os)
            return os;

        os << dist.param_;

        return os;
    }

    template <typename CharT, typename Traits>
    friend std::basic_istream<CharT, Traits> &operator>>(
        std::basic_istream<CharT, Traits> &is, distribution_type &dist)
    {
        if (!is)
            return is;

        param_type param;
        is >> std::ws >> param;
        if (is)
            dist.param_ = std::move(param);

        return is;
    }

    private:
    param_type param_;

    template <typename RNGType>
    void generate(RNGType &rng, result_type *r, const param_type &param)
    {
        const std::size_t d = param.dim();
        if (param.is_scalar_) {
            GammaDistribution<result_type> gamma(param.alpha()[0]);
            gamma(rng, d, r);
        } else {
            for (std::size_t i = 0; i != d; ++i) {
                GammaDistribution<result_type> gamma(param.alpha()[i]);
                r[i] = gamma(rng);
            }
        }
        result_type s = std::accumulate(r, r + d, const_zero<result_type>());
        mul<result_type>(d, 1 / s, r, r);
    }
}; // class DirichletDistribution

template <typename RealType, typename RNGType>
inline void rand(
    RNGType &rng, DirichletDistribution<RealType> &distribution, RealType *r)
{
    distribution(rng, r);
}

template <typename RealType, typename RNGType>
inline void rand(RNGType &rng, DirichletDistribution<RealType> &distribution,
    std::size_t n, RealType *r)
{
    distribution(rng, n, r);
}

} // namespace mckl

#endif // MCKL_RANDOM_DIRICHLET_DISTRIBUTION_HPP
