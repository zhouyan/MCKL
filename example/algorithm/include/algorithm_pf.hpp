//============================================================================
// MCKL/example/algorithm/include/algorithm_pf.hpp
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

#ifndef MCKL_EXAMPLE_ALGORITHM_PF_HPP
#define MCKL_EXAMPLE_ALGORITHM_PF_HPP

#include <mckl/algorithm/smc.hpp>
#include <mckl/random/normal_distribution.hpp>

using AlgorithmPFBase = mckl::StateMatrix<mckl::ColMajor, 4, double>;

class AlgorithmPF : public AlgorithmPFBase
{
    public:
    AlgorithmPF(std::size_t N) : AlgorithmPFBase(N)
    {
        double x = 0;
        double y = 0;
        std::ifstream data("algorithm_pf.data");
        while (data >> x >> y) {
            x_.push_back(x);
            y_.push_back(y);
        }
        data.close();
    }

    std::size_t n() const { return x_.size(); }
    double x(std::size_t iter) { return x_[iter]; }
    double y(std::size_t iter) { return y_[iter]; }

    private:
    mckl::Vector<double> x_;
    mckl::Vector<double> y_;
}; // class AlgorithmPF

class AlgorithmPFSelection
{
    public:
    void operator()(std::size_t iter, mckl::Particle<AlgorithmPF> &particle)
    {
        constexpr double scale = 10;
        constexpr double nuinv = 0.1;
        constexpr double coeff = -5.5;
        constexpr double delta = 0.1;

        auto &rng = particle.rng();
        const std::size_t N = particle.size();
        const std::size_t M = N * 2;

        w_.resize(M);
        double *const w = w_.data();
        double *const p = particle.state().col_data(0);
        double *const v = particle.state().col_data(2);

        if (iter == 0) {
            mckl::NormalDistribution<double> rp(0, 2);
            mckl::rand(rng, rp, M, p);

            mckl::NormalDistribution<double> rv(0, 1);
            mckl::rand(rng, rv, M, v);
        } else {
            mckl::NormalDistribution<double> rp(0, std::sqrt(0.02));
            mckl::rand(rng, rp, M, w);
            mckl::muladd(M, delta, v, w, w);
            mckl::add(M, w, p, p);

            mckl::NormalDistribution<double> rv(0, std::sqrt(0.001));
            mckl::rand(rng, rv, M, w);
            mckl::add(M, w, v, v);
        }

        mckl::sub(N, p + 0, particle.state().x(iter), w + 0);
        mckl::sub(N, p + N, particle.state().y(iter), w + N);
        mckl::mul(M, scale, w, w);
        mckl::sqr(M, w, w);
        mckl::mul(M, w, nuinv, w);
        mckl::log1p(M, w, w);
        mckl::add(N, w, w + N, w);
        mckl::mul(N, coeff, w, w);
        particle.weight().add_log(w);
    }

    private:
    mckl::Vector<double> w_;
}; // AlgorithmPFSelection

class AlgorithmPFPos
{
    public:
    void operator()(std::size_t, std::size_t,
        mckl::Particle<AlgorithmPF> &particle, double *r)
    {
        std::memcpy(
            r, particle.state().data(), sizeof(double) * particle.size() * 2);
    }
}; // class AlgorithmPFPos

#endif // MCKL_EXAMPLE_ALGORITHM_PF_HPP
