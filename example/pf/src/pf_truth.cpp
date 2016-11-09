//============================================================================
// MCKL/example/pf/src/pf_truth.cpp
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

#include <mckl/core.hpp>
#include <mckl/random/normal_distribution.hpp>
#include <future>

using namespace mckl;

using PFBase = StateMatrix<ColMajor, 6, double>;

template <typename T>
using PFIndexBase = PFBase::particle_index_type<T>;

class PF : public PFBase
{
    public:
    PF(std::size_t N) : PFBase(N)
    {
        double x = 0;
        double y = 0;
        std::ifstream data("pf.data");
        while (data >> x >> y) {
            obs_x_.push_back(x);
            obs_y_.push_back(y);
        }
        data.close();
    }

    std::size_t n() const { return obs_x_.size(); }

    double obs_x(std::size_t k) { return obs_x_[k]; }
    double obs_y(std::size_t k) { return obs_y_[k]; }

    double *pos_x() { return col_data(0); }
    double *pos_y() { return col_data(1); }
    double *vel_x() { return col_data(2); }
    double *vel_y() { return col_data(3); }
    double *tempa() { return col_data(4); }
    double *tempb() { return col_data(5); }

    const double *pos_x() const { return col_data(0); }
    const double *pos_y() const { return col_data(1); }
    const double *vel_x() const { return col_data(2); }
    const double *vel_y() const { return col_data(3); }
    const double *tempa() const { return col_data(4); }
    const double *tempb() const { return col_data(5); }

    private:
    Vector<double> obs_x_;
    Vector<double> obs_y_;
}; // class PF

inline void PFInit(std::size_t, Particle<PF> &particle)
{
    auto &rng = particle.rng();
    const auto N = particle.size();

    mckl::NormalDistribution<double> rpos(0, 2);
    mckl::NormalDistribution<double> rvel(0, 1);

    mckl::rand(rng, rpos, N, particle.state().pos_x());
    mckl::rand(rng, rpos, N, particle.state().pos_y());
    mckl::rand(rng, rvel, N, particle.state().vel_x());
    mckl::rand(rng, rvel, N, particle.state().vel_y());
}

inline void PFMove(std::size_t, Particle<PF> &particle)
{
    auto &rng = particle.rng();
    const auto N = particle.size();
    const double delta = 0.1;

    mckl::NormalDistribution<double> rpos(0, std::sqrt(0.02));
    mckl::NormalDistribution<double> rvel(0, std::sqrt(0.001));

    const auto pos_x = particle.state().pos_x();
    const auto pos_y = particle.state().pos_y();
    const auto vel_x = particle.state().vel_x();
    const auto vel_y = particle.state().vel_y();
    const auto tempa = particle.state().tempa();

    mckl::rand(rng, rpos, N, tempa);
    mckl::fma(N, delta, vel_x, tempa, tempa);
    mckl::add(N, pos_x, tempa, pos_x);

    mckl::rand(rng, rpos, N, tempa);
    mckl::fma(N, delta, vel_y, tempa, tempa);
    mckl::add(N, pos_y, tempa, pos_y);

    mckl::rand(rng, rvel, N, particle.state().tempa());
    mckl::add(N, vel_x, tempa, vel_x);

    mckl::rand(rng, rvel, N, particle.state().tempa());
    mckl::add(N, vel_y, tempa, vel_y);
}

inline void PFWeight(std::size_t iter, Particle<PF> &particle)
{
    const auto N = particle.size();
    const double scale = 10;
    const double nu = 10;
    const double nuinv = 0.1;

    const auto pos_x = particle.state().pos_x();
    const auto pos_y = particle.state().pos_y();
    const auto tempa = particle.state().tempa();
    const auto tempb = particle.state().tempb();

    const auto x = particle.state().obs_x(iter);
    const auto y = particle.state().obs_y(iter);

    mckl::sub(N, pos_x, x, tempa);
    mckl::sub(N, pos_y, y, tempb);
    mckl::mul(N, scale, tempa, tempa);
    mckl::mul(N, scale, tempb, tempb);
    mckl::sqr(N, tempa, tempa);
    mckl::sqr(N, tempb, tempb);
    mckl::mul(N, tempa, nuinv, tempa);
    mckl::mul(N, tempb, nuinv, tempb);
    mckl::log1p(N, tempa, tempa);
    mckl::log1p(N, tempa, tempa);
    mckl::add(N, tempa, tempb, tempa);
    mckl::mul(N, -0.5 * (nu + 1), tempa, tempa);

    particle.weight().add_log(tempa);
}

inline void PFEstimate(
    std::size_t, std::size_t, Particle<PF> &particle, double *r)
{
    const auto n = static_cast<MCKL_BLAS_INT>(particle.size());
    const auto pos_x = particle.state().pos_x();
    const auto pos_y = particle.state().pos_y();
    const auto weight = particle.weight().data();

    r[0] = mckl::internal::cblas_ddot(n, pos_x, 1, weight, 1);
    r[1] = mckl::internal::cblas_ddot(n, pos_y, 1, weight, 1);
}

inline mckl::Vector<double> PFRun(std::size_t N, std::size_t M)
{
    Sampler<PF> sampler(N);
    sampler.resample_method(Stratified, 0.5)
        .eval(PFInit, SamplerInit)
        .eval(PFMove, SamplerMove)
        .eval(PFWeight, SamplerInit | SamplerMove)
        .monitor("pos", Monitor<PF>(2, PFEstimate, true));

    const std::size_t n = sampler.particle().state().n();
    mckl::Vector<double> r(n * 2);
    mckl::Vector<double> s(n * 2);
    std::fill(s.begin(), s.end(), 0);
    for (std::size_t i = 0; i != M; ++i) {
        sampler.initialize().iterate(n - 1);
        sampler.monitor("pos").read_record_matrix(mckl::ColMajor, r.data());
        mckl::add(n * 2, r.data(), s.data(), s.data());
    }

    return s;
}

int main(int argc, char **argv)
{
    std::size_t N = 1000000;
    if (argc > 1)
        N = static_cast<std::size_t>(std::atoi(argv[1]));

    std::size_t M = 25;
    if (argc > 2)
        M = static_cast<std::size_t>(std::atoi(argv[2]));

    std::size_t P = std::thread::hardware_concurrency();
    if (argc > 3)
        P = static_cast<std::size_t>(std::atoi(argv[3]));

    mckl::Vector<std::future<mckl::Vector<double>>> jobs;
    for (std::size_t p = 0; p != P; ++p)
        jobs.push_back(std::async(std::launch::async, PFRun, N, M));

    mckl::Vector<double> s = jobs[0].get();
    mckl::Vector<double> r;
    for (std::size_t p = 1; p < P; ++p) {
        r = jobs[p].get();
        mckl::add(r.size(), r.data(), s.data(), s.data());
    }
    mckl::mul(s.size(), 1.0 / (P * M), s.data(), s.data());

    const std::size_t n = s.size() / 2;
    const double *const x = s.data();
    const double *const y = s.data() + n;
    std::ofstream txtfile("pf_truth.txt");
    txtfile.precision(16);
    for (std::size_t i = 0; i != n; ++i)
        txtfile << x[i] << '\t' << y[i] << '\n';
    txtfile.close();

    return 0;
}
