//============================================================================
// MCKL/example/algorithm/include/algorithm_gibbs.hpp
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

#ifndef MCKL_EXAMPLE_ALGORITHM_GIBBS_HPP
#define MCKL_EXAMPLE_ALGORITHM_GIBBS_HPP

#include <mckl/algorithm/mcmc.hpp>
#include <mckl/random/gamma_distribution.hpp>
#include <mckl/random/normal_distribution.hpp>

using AlgorithmGibbs = std::array<double, 2>;

class AlgorithmGibbsMutation
{
    public:
    AlgorithmGibbsMutation() : xbar_(0)
    {
        double x = 0;
        std::ifstream data("algorithm_gibbs.data");
        while (data >> x) {
            x_.push_back(x);
            xbar_ += x;
        }
        data.close();
        n_ = static_cast<double>(x_.size());
        xbar_ /= n_;
    }

    std::size_t operator()(std::size_t, AlgorithmGibbs &state)
    {
        constexpr double m = 0;
        constexpr double tau = 1;
        constexpr double alpha = 1;
        constexpr double beta = 1;

        double mu = std::get<0>(state);
        double lambda = std::get<1>(state);

        const double p = tau + n_ * lambda;
        const double mean = (tau * m + lambda * n_ * xbar_) / p;
        const double sd = 1 / std::sqrt(p);
        mckl::NormalDistribution<double> normal(mean, sd);
        mu = normal(rng_);

        r_.resize(x_.size());
        mckl::sub(x_.size(), x_.data(), mu, r_.data());
        mckl::sqr(r_.size(), r_.data(), r_.data());
        const double shape = alpha + 0.5 * n_;
        const double scale = 1.0 /
            (1.0 / beta + 0.5 * std::accumulate(r_.begin(), r_.end(), 0.0));
        mckl::GammaDistribution<double> gamma(shape, scale);
        lambda = gamma(rng_);

        std::get<0>(state) = mu;
        std::get<1>(state) = lambda;

        return 0;
    }

    protected:
    mckl::Vector<double> x_;
    mckl::Vector<double> r_;
    double n_;
    double xbar_;
    mckl::RNG rng_;
}; // class AlgorithmGibbsMutation

class AlgorithmGibbsEstimate
{
    public:
    void operator()(std::size_t, std::size_t, AlgorithmGibbs &state, double *r)
    {
        r[0] = std::get<0>(state);
        r[1] = std::get<1>(state);
    }
}; // class AlgorithmGibbsEstimate

#endif // MCKL_EXAMPLE_ALGORITHM_GIBBS_HPP
