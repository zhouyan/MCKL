//============================================================================
// MCKL/example/algorithm/src/algorithm_pmcmc.cpp
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

#include <mckl/algorithm/pmcmc.hpp>
#include <mckl/random/lognormal_distribution.hpp>
#include <mckl/random/normal_distribution.hpp>

using AlgorithmPMCMCState = std::array<double, 3>;
using AlgorithmPMCMCBase = mckl::StateMatrix<mckl::ColMajor, 1, double>;

class AlgorithmPMCMC : public AlgorithmPMCMCBase
{
    public:
    AlgorithmPMCMC(std::size_t N)
        : AlgorithmPMCMCBase(N), theta_(1), sigma_(1), log_nc_(0)
    {
        double y = 0;
        std::ifstream data("algorithm_pmcmc.data");
        while (data >> y)
            y_.push_back(y);
        data.close();
    }

    void reset(const AlgorithmPMCMCState &state)
    {
        theta_ = std::get<0>(state);
        sigma_ = std::get<0>(state);
        log_nc_ = 0;
    }

    std::size_t n() const { return y_.size(); }

    double log_nc() const { return log_nc_; }

    void log_nc(double l) { log_nc_ = l; }

    void add_log_nc(double inc) { log_nc_ += inc; }

    double theta() const { return theta_; }
    double sigma() const { return sigma_; }
    double y(std::size_t iter) const { return y_[iter]; }

    private:
    mckl::Vector<double> y_;
    double theta_;
    double sigma_;
    double log_nc_;
}; // class AlgorithmPMCMC

class AlgorithmPMCMCSelection
{
    public:
    void operator()(std::size_t iter, mckl::Particle<AlgorithmPMCMC> &particle)
    {
        constexpr double x0 = 0;
        constexpr double c = -2.5;
        constexpr double h = 0.025;
        constexpr std::size_t k = 20;

        const double theta = particle.state().theta();
        const double sigma = particle.state().sigma();
        const double y = particle.state().y(iter);
        const std::size_t N = particle.size();
        double *const x = particle.state().col_data(0);

        w_.resize(N);
        double *const w = w_.data();

        if (iter == 0) {
            std::fill_n(x, N, x0);
            particle.weight().set_equal();
            return;
        }

        mckl::NormalDistribution<double> normal(0, 1);
        for (std::size_t i = 0; i != k; ++i) {
            mckl::rand(particle.rng(), normal, N, w);
            mckl::muladd(N, -theta * h, x, x, x);
            mckl::muladd(N, sigma * std::sqrt(h), w, x, x);
        }

        mckl::sub(N, y, x, w);
        mckl::sqr(N, w, w);
        mckl::mul(N, c, w, w);
        particle.weight().add_log(w);
        mckl::exp(N, w, w);
        mckl::mul(N, particle.weight().data(), w, w);
        particle.state().add_log_nc(std::log(std::accumulate(w, w + N, 0.0)));
    }

    private:
    mckl::Vector<double> w_;
}; // class AlgorithmPMCMCSelection

class AlgorithmPMCMCPrior
{
    public:
    double operator()(const AlgorithmPMCMCState &state)
    {
        return -std::get<0>(state) - 2 * std::get<1>(state);
    }
}; // class AlgorithmPMCMCPrior

template <std::size_t I>
class AlgorithmPMCMCUpdate
{
    public:
    template <typename RNGType>
    double operator()(RNGType &rng, AlgorithmPMCMCState &s)
    {
        mckl::LognormalDistribution<double> lognormal(0, 0.5);
        const double u = lognormal(rng);

        std::get<I>(s) *= u;

        return std::log(u);
    }
}; // class AlgorithmPMCMCUpdate

class AlgorithmPMCMCEstimate
{
    public:
    void operator()(
        std::size_t, std::size_t, AlgorithmPMCMCState &state, double *r)
    {
        r[0] = std::get<0>(state);
        r[1] = std::get<1>(state);
    }
}; // class AlgorithmPMCMCEstimate
