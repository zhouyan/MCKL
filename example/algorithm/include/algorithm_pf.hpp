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
#include <mckl/smp.hpp>
#include <mckl/utility/stop_watch.hpp>

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

template <typename Backend>
class AlgorithmPFSelection : public mckl::SMCSamplerEvalSMP<AlgorithmPF,
                                 AlgorithmPFSelection<Backend>, Backend>
{
    public:
    void operator()(std::size_t iter, mckl::Particle<AlgorithmPF> &particle)
    {
        this->run(iter, particle, 1000);
    }

    void eval_first(std::size_t, mckl::Particle<AlgorithmPF> &particle)
    {
        w_.resize(particle.size());
        v_.resize(particle.size());
    }

    void eval_last(std::size_t, mckl::Particle<AlgorithmPF> &particle)
    {
        particle.weight().add_log(w_.data());
    }

    void eval_range(
        std::size_t iter, const mckl::ParticleRange<AlgorithmPF> &range)
    {
        constexpr double scale = 10;
        constexpr double nuinv = 0.1;
        constexpr double coeff = -5.5;
        constexpr double delta = 0.1;

        auto rng = range.begin().rng();

        const std::size_t N = range.size();
        double *const w = w_.data() + range.ibegin();
        double *const v = v_.data() + range.ibegin();
        double *const p =
            range.particle().state().col_data(0) + range.ibegin();
        double *const q =
            range.particle().state().col_data(1) + range.ibegin();
        double *const s =
            range.particle().state().col_data(2) + range.ibegin();
        double *const t =
            range.particle().state().col_data(3) + range.ibegin();

        if (iter == 0) {
            mckl::NormalDistribution<double> rp(0, 2);
            mckl::NormalDistribution<double> rv(0, 1);

            mckl::rand(rng, rp, N, p);
            mckl::rand(rng, rp, N, q);
            mckl::rand(rng, rv, N, s);
            mckl::rand(rng, rv, N, t);
        } else {
            mckl::NormalDistribution<double> rp(0, std::sqrt(0.02));
            mckl::NormalDistribution<double> rv(0, std::sqrt(0.001));

            mckl::rand(rng, rp, N, w);
            mckl::muladd(N, delta, s, w, w);
            mckl::add(N, w, p, p);
            mckl::rand(rng, rv, N, w);
            mckl::add(N, w, s, s);

            mckl::rand(rng, rp, N, w);
            mckl::muladd(N, delta, t, w, w);
            mckl::add(N, w, q, q);
            mckl::rand(rng, rv, N, w);
            mckl::add(N, w, t, t);
        }

        mckl::sub(N, p, range.particle().state().x(iter), w);
        mckl::mul(N, scale, w, w);
        mckl::sqr(N, w, w);
        mckl::mul(N, w, nuinv, w);
        mckl::log1p(N, w, w);

        mckl::sub(N, q, range.particle().state().y(iter), v);
        mckl::mul(N, scale, v, v);
        mckl::sqr(N, v, v);
        mckl::mul(N, v, nuinv, v);
        mckl::log1p(N, v, v);

        mckl::add(N, w, v, w);
        mckl::mul(N, coeff, w, w);

        range.begin().rng() = rng;
    }

    private:
    mckl::Vector<double> w_;
    mckl::Vector<double> v_;
}; // AlgorithmPFSelection

template <typename Backend>
class AlgorithmPFPos : public mckl::SMCEstimatorEvalSMP<AlgorithmPF,
                           AlgorithmPFPos<Backend>, Backend>
{
    public:
    void eval_each(std::size_t, std::size_t,
        mckl::ParticleIndex<AlgorithmPF> idx, double *r)
    {
        r[0] = idx(0);
        r[1] = idx(1);
    }
}; // class AlgorithmPFPos

template <typename Backend>
inline std::string algorithm_pf_name();

template <>
inline std::string algorithm_pf_name<mckl::BackendSEQ>()
{
    return "SEQ";
}

template <>
inline std::string algorithm_pf_name<mckl::BackendSTD>()
{
    return "STD";
}

template <>
inline std::string algorithm_pf_name<mckl::BackendOMP>()
{
    return "OMP";
}

template <>
inline std::string algorithm_pf_name<mckl::BackendTBB>()
{
    return "TBB";
}

template <typename Backend>
inline void algorithm_pf(std::size_t N)
{
    mckl::SMCSampler<AlgorithmPF> sampler(N);
    sampler.selection(AlgorithmPFSelection<Backend>());
    sampler.resample(mckl::Stratified);
    sampler.resample_threshold(0.5);
    sampler.selection_estimator(
        mckl::SMCEstimator<AlgorithmPF>(2, AlgorithmPFPos<Backend>()));
    sampler.resample_estimator(
        mckl::SMCEstimator<AlgorithmPF>(2, AlgorithmPFPos<Backend>()));
    sampler.mutation_estimator(
        mckl::SMCEstimator<AlgorithmPF>(2, AlgorithmPFPos<Backend>()));

    sampler.iterate(sampler.particle().state().n());
    sampler.clear();

    mckl::StopWatch watch;
    watch.start();
    sampler.iterate(sampler.particle().state().n());
    watch.stop();

    const std::string name = algorithm_pf_name<Backend>();
    std::cout << name << ": " << watch.seconds() << 's' << std::endl;

    std::ofstream save("algorithm_pf_" + name + ".save");
    save << sampler << std::endl;
    save.close();
}

inline void algorithm_pf(std::size_t N)
{
    std::cout << std::fixed << std::setprecision(3);

    algorithm_pf<mckl::BackendSEQ>(N);
    algorithm_pf<mckl::BackendSTD>(N);
    algorithm_pf<mckl::BackendOMP>(N);
#if MCKL_HAS_TBB
    algorithm_pf<mckl::BackendTBB>(N);
#endif
}

#endif // MCKL_EXAMPLE_ALGORITHM_PF_HPP
