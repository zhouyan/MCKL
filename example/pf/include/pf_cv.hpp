//============================================================================
// MCKL/example/pf/include/pf_cv.hpp
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

#ifndef MCKL_EXAMPLE_PF_CV_HPP
#define MCKL_EXAMPLE_PF_CV_HPP

#include <mckl/core.hpp>
#include <mckl/smp.hpp>
#include "pf.hpp"

#if MCKL_HAS_HDF5
#include <mckl/utility/hdf5.hpp>
#endif

template <typename>
std::string pf_cv_backend_name();

template <>
std::string pf_cv_backend_name<mckl::BackendSEQ>()
{
    return "BackendSEQ";
}

template <>
std::string pf_cv_backend_name<mckl::BackendSTD>()
{
    return "BackendSTD";
}

#if MCKL_HAS_OMP
template <>
std::string pf_cv_backend_name<mckl::BackendOMP>()
{
    return "BackendOMP";
}
#endif

#if MCKL_HAS_TBB
template <>
std::string pf_cv_backend_name<mckl::BackendTBB>()
{
    return "BackendTBB";
}
#endif

template <mckl::ResampleScheme>
std::string pf_cv_scheme_name();

template <>
std::string pf_cv_scheme_name<mckl::Multinomial>()
{
    return "Multinomial";
}

template <>
std::string pf_cv_scheme_name<mckl::Residual>()
{
    return "Residual";
}

template <>
std::string pf_cv_scheme_name<mckl::ResidualStratified>()
{
    return "ResidualStratified";
}

template <>
std::string pf_cv_scheme_name<mckl::ResidualSystematic>()
{
    return "ResidualSystematic";
}

template <>
std::string pf_cv_scheme_name<mckl::Stratified>()
{
    return "Stratified";
}

template <>
std::string pf_cv_scheme_name<mckl::Systematic>()
{
    return "Systematic";
}

template <mckl::MatrixLayout>
std::string pf_cv_layout_name();

template <>
std::string pf_cv_layout_name<mckl::RowMajor>()
{
    return "RowMajor";
}

template <>
std::string pf_cv_layout_name<mckl::ColMajor>()
{
    return "ColMajor";
}

template <typename>
std::string pf_cv_rng_set_name();

template <>
std::string pf_cv_rng_set_name<mckl::RNGSetVector<mckl::RNG>>()
{
    return "RNGSetVector";
}

#if MCKL_HAS_TBB
template <>
std::string pf_cv_rng_set_name<mckl::RNGSetTBB<mckl::RNG>>()
{
    return "RNGSetTBB";
}

template <>
std::string pf_cv_rng_set_name<mckl::RNGSetTBBKPI<mckl::RNG>>()
{
    return "RNGSetTBBKPI";
}
#endif

template <mckl::MatrixLayout Layout>
using PFCVBase = mckl::StateMatrix<Layout, 4, double>;

template <mckl::MatrixLayout Layout, typename RNGSetType>
class PFCV : public PFCVBase<Layout>
{
    public:
    using rng_set_type = RNGSetType;

    using base = PFCVBase<Layout>;

    template <typename T>
    using base_idx = typename base::template particle_index_type<T>;

    template <typename T>
    class particle_index_type : public base_idx<T>
    {
        public:
        particle_index_type(std::size_t id, mckl::Particle<T> *pptr)
            : base_idx<T>(id, pptr)
        {
        }

        double &pos_x() { return this->at(0); }
        double &pos_y() { return this->at(1); }
        double &vel_x() { return this->at(2); }
        double &vel_y() { return this->at(3); }

        double log_likelihood(std::size_t iter)
        {
            return pf_log_likelihood(pos_x(), pos_y(),
                this->particle().state().obs_x_[iter],
                this->particle().state().obs_x_[iter]);
        }
    }; // class particle_index_type

    PFCV(std::size_t N) : PFCVBase<Layout>(N)
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

    private:
    mckl::Vector<double> obs_x_;
    mckl::Vector<double> obs_y_;
}; // class PFCV

template <typename Backend, mckl::MatrixLayout Layout, typename RNGSetType>
class PFCVInit : public mckl::SamplerEvalSMP<PFCV<Layout, RNGSetType>,
                     PFCVInit<Backend, Layout, RNGSetType>, Backend>
{
    public:
    using T = PFCV<Layout, RNGSetType>;

    void eval_each(std::size_t, mckl::ParticleIndex<T> idx)
    {
        pf_init_each(
            idx.rng(), idx.pos_x(), idx.pos_y(), idx.vel_x(), idx.vel_y());
    }
}; // PFCVInit

template <typename Backend, mckl::MatrixLayout Layout, typename RNGSetType>
class PFCVMove : public mckl::SamplerEvalSMP<PFCV<Layout, RNGSetType>,
                     PFCVMove<Backend, Layout, RNGSetType>, Backend>
{
    public:
    using T = PFCV<Layout, RNGSetType>;

    void eval_each(std::size_t, mckl::ParticleIndex<T> idx)
    {
        pf_move_each(
            idx.rng(), idx.pos_x(), idx.pos_y(), idx.vel_x(), idx.vel_y());
    }
}; // class PFCVMove

template <typename Backend, typename RNGSetType>
class PFCVMove<Backend, mckl::ColMajor, RNGSetType>
    : public mckl::SamplerEvalSMP<PFCV<mckl::ColMajor, RNGSetType>,
          PFCVMove<Backend, mckl::ColMajor, RNGSetType>, Backend>
{
    public:
    using T = PFCV<mckl::ColMajor, RNGSetType>;

    void operator()(std::size_t iter, mckl::Particle<T> &particle)
    {
        this->run(iter, particle, 512);
    }

    void eval_range(std::size_t, const mckl::ParticleRange<T> &range)
    {
        pf_move_range(range.begin().rng(), range.size(),
            range.particle().state().col_data(0) + range.first(),
            range.particle().state().col_data(1) + range.first(),
            range.particle().state().col_data(2) + range.first(),
            range.particle().state().col_data(3) + range.first(),
            w_.data() + range.first(), v_.data() + range.first());
    }

    void eval_first(std::size_t, mckl::Particle<T> &particle)
    {
        w_.resize(particle.size());
        v_.resize(particle.size());
    }

    private:
    mckl::Vector<double> w_;
    mckl::Vector<double> v_;
}; // class PFCVMove

template <typename Backend, mckl::MatrixLayout Layout, typename RNGSetType>
class PFCVWeight : public mckl::SamplerEvalSMP<PFCV<Layout, RNGSetType>,
                       PFCVWeight<Backend, Layout, RNGSetType>, Backend>
{
    public:
    using T = PFCV<Layout, RNGSetType>;

    void eval_each(std::size_t iter, mckl::ParticleIndex<T> idx)
    {
        w_[idx.i()] = idx.log_likelihood(iter);
    }

    void eval_first(std::size_t, mckl::Particle<T> &particle)
    {
        w_.resize(particle.size());
    }

    void eval_last(std::size_t, mckl::Particle<T> &particle)
    {
        particle.weight().add_log(w_.data());
    }

    private:
    mckl::Vector<double> w_;
}; // class PFCVWeight

template <typename Backend, mckl::MatrixLayout Layout, typename RNGSetType>
class PFCVMEval : public mckl::MonitorEvalSMP<PFCV<Layout, RNGSetType>,
                      PFCVMEval<Backend, Layout, RNGSetType>, Backend>
{
    public:
    using T = PFCV<Layout, RNGSetType>;

    void eval_each(
        std::size_t, std::size_t, mckl::ParticleIndex<T> idx, double *res)
    {
        res[0] = idx.pos_x();
        res[1] = idx.pos_y();
    }
}; // class PFCVMEval

template <typename T>
inline double pf_cv_error(const mckl::Sampler<T> &sampler)
{
    static mckl::Vector<double> tx;
    static mckl::Vector<double> ty;

    if (tx.size() == 0) {
        double x = 0;
        double y = 0;
        std::ifstream truth("pf.truth");
        while (truth >> x >> y) {
            tx.push_back(x);
            ty.push_back(y);
        }
    }

    const std::size_t n = tx.size();
    mckl::Vector<double> rx(n);
    mckl::Vector<double> ry(n);
    sampler.monitor("pos").read_record(0, rx.data());
    sampler.monitor("pos").read_record(1, ry.data());
    mckl::sub(n, tx.data(), rx.data(), rx.data());
    mckl::sub(n, ty.data(), ry.data(), ry.data());
    mckl::sqr(n, rx.data(), rx.data());
    mckl::sqr(n, ry.data(), ry.data());
    mckl::add(n, rx.data(), ry.data(), rx.data());
    mckl::sqrt(n, rx.data(), rx.data());

    return std::accumulate(rx.begin(), rx.end(), 0.0) / n;
}

template <typename Backend, mckl::ResampleScheme Scheme,
    mckl::MatrixLayout Layout, typename RNGSetType>
inline void pf_cv(std::size_t N)
{
    using T = PFCV<Layout, RNGSetType>;
    using init = PFCVInit<Backend, Layout, RNGSetType>;
    using move = PFCVMove<Backend, Layout, RNGSetType>;
    using weight = PFCVWeight<Backend, Layout, RNGSetType>;
    using meval = PFCVMEval<Backend, Layout, RNGSetType>;

    mckl::Sampler<T> sampler(N);
    sampler.resample_method(Scheme, 0.5);
    sampler.eval(init(), mckl::SamplerInit);
    sampler.eval(move(), mckl::SamplerMove);
    sampler.eval(weight(), mckl::SamplerInit | mckl::SamplerMove);
    sampler.monitor("pos", mckl::Monitor<T>(2, meval()));
    sampler.monitor("pos").name(0) = "pos.x";
    sampler.monitor("pos").name(1) = "pos.y";

    const std::size_t n = sampler.particle().state().n();
    const std::string smp(pf_cv_backend_name<Backend>());
    const std::string res(pf_cv_scheme_name<Scheme>());
    const std::string rc(pf_cv_layout_name<Layout>());
    const std::string rs(pf_cv_rng_set_name<RNGSetType>());
    const std::string base = "pf_cv." + smp + "." + res + "." + rc + "." + rs;

    sampler.initialize();
#if MCKL_HAS_HDF5
    std::string h5file = base + ".h5";
    mckl::hdf5store(h5file);
    mckl::hdf5store(h5file, "Particle", true);
    mckl::hdf5store(sampler.particle(), h5file, "Particle/Iter.0", true);
#endif

    for (std::size_t i = 1; i < n; ++i) {
        sampler.iterate();
#if MCKL_HAS_HDF5
        mckl::hdf5store(sampler.particle(), h5file,
            "Particle/Iter." + std::to_string(i), true);
#endif
    }
#if MCKL_HAS_HDF5
    mckl::hdf5store(sampler, h5file, "Sampler", true);
    mckl::hdf5store(sampler.monitor("pos"), h5file, "Monitor", true);
#endif

    std::ofstream txt(base + ".txt");
    txt << sampler << std::endl;
    txt.close();

    std::ofstream err;
    err.open(base + ".err.txt");
    err.precision(16);
    err << "N\t";
    err << "Backend\t";
    err << "ResampleScheme\t";
    err << "MatrixLayout\t";
    err << "RNGSetType\t";
    err << "Error\n";
    err << N << '\t';
    err << smp << '\t';
    err << res << '\t';
    err << rc << '\t';
    err << rs << '\t';
    err << pf_cv_error(sampler) << '\n';
    err.close();
}

#endif // MCKL_EXAMPLE_PF_CV_HPP
