//============================================================================
// MCKL/include/mckl/algorithm/smc.hpp
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

#ifndef MCKL_ALGORITHM_SMC_HPP
#define MCKL_ALGORITHM_SMC_HPP

#include <mckl/internal/common.hpp>
#include <mckl/algorithm/resample.hpp>
#include <mckl/core/estimator.hpp>
#include <mckl/core/particle.hpp>
#include <mckl/core/sampler.hpp>
#include <mckl/core/state_matrix.hpp>

namespace mckl
{

/// \brief SMC estimator
/// \ingroup SMC
template <typename T, typename U = double>
class SMCEstimator
    : public Estimator<U, std::size_t, std::size_t, Particle<T> &, U *>
{
    static_assert(std::is_convertible<U, double>::value,
        "**SMCEsimator** used with estimate type U not convertible to double");

    public:
    SMCEstimator() : layout_(RowMajor), record_only_(false) {}

    SMCEstimator(std::size_t dim)
        : Estimator<U, std::size_t, std::size_t, Particle<T> &, U *>(dim)
    {
    }

    template <typename Eval>
    SMCEstimator(std::size_t dim, Eval &&eval, MatrixLayout layout = RowMajor,
        bool record_only = false)
        : Estimator<U, std::size_t, std::size_t, Particle<T> &, U *>(
              dim, std::forward<Eval>(eval))
        , layout_(layout)
        , record_only_(record_only)
    {
    }

    /// \brief If this is a record only estimator
    bool record_only() const { return record_only_; }

    /// \brief Set a new evaluation object
    template <typename Eval>
    void estimate(
        Eval &&eval, MatrixLayout layout = RowMajor, bool record_only = false)
    {
        Estimator<U, std::size_t, std::size_t, Particle<T> &, U *>::estimate(
            std::forward<Eval>(eval));
        layout_ = layout;
        record_only_ = record_only;
    }

    /// \brief Perform the evaluation given the iteration number and the
    /// particle system
    void estimate(std::size_t iter, Particle<T> &particle)
    {
        result_.resize(this->dim());
        if (record_only_) {
            this->eval(iter, this->dim(), particle, this->insert_estimate());
            return;
        }

        const std::size_t n = static_cast<std::size_t>(particle.size());
        const std::size_t d = this->dim();
        u_.resize(n * d);
        this->eval(iter, d, particle, u_.data());

        const double *w = particle.weight().data();
        double *r = rptr(std::is_same<U, double>());
        internal::size_check<MCKL_BLAS_INT>(n, "SMCEstimator::estimate");
        internal::size_check<MCKL_BLAS_INT>(d, "SMCEstimator::estimate");
        if (layout_ == RowMajor) {
            internal::cblas_dgemv(internal::CblasColMajor,
                internal::CblasNoTrans, static_cast<MCKL_BLAS_INT>(d),
                static_cast<MCKL_BLAS_INT>(n), 1.0, r,
                static_cast<MCKL_BLAS_INT>(d), w, 1, 0.0, result_.data(), 1);
        } else {
            internal::cblas_dgemv(internal::CblasColMajor,
                internal::CblasTrans, static_cast<MCKL_BLAS_INT>(n),
                static_cast<MCKL_BLAS_INT>(d), 1.0, r,
                static_cast<MCKL_BLAS_INT>(n), w, 1, 0.0, result_.data(), 1);
        }
        this->insert_estimate(result_.data());
    }

    private:
    Vector<U> u_;
    Vector<double> r_;
    Vector<double> result_;
    MatrixLayout layout_;
    bool record_only_;

    double *rptr(std::true_type) { return u_.data(); }

    double *rptr(std::false_type)
    {
        r_.resize(u_.size());
        std::copy(u_.begin(), u_.end(), r_.begin());

        return r_.data();
    }
}; // class SMCEstimator

template <typename, typename = double>
class SMCSampler;

template <typename T, typename U>
class SamplerTrait<SMCSampler<T, U>>
{
    public:
    using eval_type = std::function<void(std::size_t, Particle<T> &)>;
    using estimator_type = SMCEstimator<T, U>;
}; // class SamplerTrait

/// \brief SMC sampler
/// \ingroup SMC
template <typename T, typename U>
class SMCSampler : public Sampler<SMCSampler<T, U>>
{
    public:
    using state_type = T;
    using size_type = typename Particle<T>::size_type;
    using eval_type = typename Sampler<SMCSampler<T, U>>::eval_type;
    using estimator_type = typename Sampler<SMCSampler<T, U>>::estimator_type;

    SMCSampler() : iter_(0), resample_threshold_(resample_threshold_never()) {}

    /// \brief Construct a SMC sampler
    ///
    /// \details
    /// All arguments are passed to the constructor of Particle
    template <typename... Args>
    explicit SMCSampler(size_type N, Args &&... args)
        : Sampler<SMCSampler<T, U>>(3)
        , particle_(N, std::forward<Args>(args)...)
        , iter_(0)
        , resample_threshold_(resample_threshold_never())
    {
    }

    /// \brief Clone the SMC sampler except the RNG engines
    SMCSampler<T, U> clone() const
    {
        SMCSampler<T, U> sampler(*this);
        sampler.particle().rng_set().reset();
        sampler.particle().rng().seed(
            Seed<typename Particle<T>::rng_type>::instance().get());

        return sampler;
    }

    /// \brief `this->particle().size()`
    size_type size() const { return particle_.size(); }

    /// \brief The number of iterations already performed
    std::size_t num_iter() const { return size_history_.size(); }

    /// \brief Reserve space for a specified number of iterations
    void reserve(std::size_t n)
    {
        Sampler<SMCSampler<T, U>>::reserve(n);
        n += num_iter();
        size_history_.reserve(n);
        ess_history_.reserve(n);
    }

    /// \brief Reset the sampler by clear all history, evaluation objects, and
    /// estimators
    void reset()
    {
        Sampler<SMCSampler<T, U>>::reset();
        clear();
    }

    /// \brief Clear all history
    void clear()
    {
        Sampler<SMCSampler<T, U>>::clear();
        iter_ = 0;
        size_history_.clear();
        ess_history_.clear();
    }

    /// \brief Get resampling threshold
    double resample_threshold() const { return resample_threshold_; }

    /// \brief Set resampling threshold
    void resample_threshold(double threshold)
    {
        resample_threshold_ = threshold;
    }

    /// \brief Special value of resampling threshold that indicates no
    /// resampling will be ever performed
    static double resample_threshold_never() { return -const_inf<double>(); }

    /// \brief Special value of resampling threshold that indicates resampling
    /// will always be performed
    static double resample_threshold_always() { return const_inf<double>(); }

    /// \brief Add a new evaluation object for the selection step
    template <typename Eval>
    std::size_t selection(Eval &&eval,
        std::enable_if_t<!std::is_integral<Eval>::value> * = nullptr)
    {
        return this->eval(0, std::forward<Eval>(eval));
    }

    eval_type &selection(std::size_t k) { return this->eval(0, k); }

    const eval_type &selection(std::size_t k) const
    {
        return this->eval(0, k);
    }

    /// \brief Add a new evaluation object for the selection step
    template <typename Eval>
    std::size_t resample(Eval &&eval,
        std::enable_if_t<!std::is_integral<Eval>::value> * = nullptr)
    {
        return this->eval(1, std::forward<Eval>(eval));
    }

    /// \brief Add a new evaluation object for the resample step by a built-in
    /// resample scheme
    std::size_t resample(ResampleScheme scheme)
    {
        switch (scheme) {
            case Multinomial:
                return resample(ResampleEval<T>(ResampleMultinomial()));
            case Residual:
                return resample(ResampleEval<T>(ResampleResidual()));
            case Stratified:
                return resample(ResampleEval<T>(ResampleStratified()));
            case Systematic:
                return resample(ResampleEval<T>(ResampleSystematic()));
            case ResidualStratified:
                return resample(ResampleEval<T>(ResampleResidualStratified()));
            case ResidualSystematic:
                return resample(ResampleEval<T>(ResampleResidualSystematic()));
        }

        runtime_assert(false,
            "**SMCSampler::resample** used with unknown resample "
            "scheme");

        return resample(ResampleEval<T>(ResampleMultinomial()));
    }

    eval_type &resample(std::size_t k) { return this->eval(1, k); }

    const eval_type &resample(std::size_t k) const { return this->eval(1, k); }

    /// \brief Add a new evaluation object for the mutation step
    template <typename Eval>
    std::size_t mutation(Eval &&eval,
        std::enable_if_t<!std::is_integral<Eval>::value> * = nullptr)
    {
        return this->eval(2, std::forward<Eval>(eval));
    }

    eval_type &mutation(std::size_t k) { return this->eval(2, k); }

    const eval_type &mutation(std::size_t k) const { return this->eval(2, k); }

    template <typename Estimator>
    std::size_t selection_estimator(Estimator &&estimator,
        std::enable_if_t<!std::is_integral<Estimator>::value> * = nullptr)
    {
        return this->estimator(0, std::forward<Estimator>(estimator));
    }

    estimator_type &selection_estimator(std::size_t k)
    {
        return this->estimator(0, k);
    }

    const estimator_type &selection_estimator(std::size_t k) const
    {
        return this->estimator(0, k);
    }

    template <typename Estimator>
    std::size_t resample_estimator(Estimator &&estimator,
        std::enable_if_t<!std::is_integral<Estimator>::value> * = nullptr)
    {
        return this->estimator(1, std::forward<Estimator>(estimator));
    }

    estimator_type &resample_estimator(std::size_t k)
    {
        return this->estimator(1, k);
    }

    const estimator_type &resample_estimator(std::size_t k) const
    {
        return this->estimator(1, k);
    }

    template <typename Estimator>
    std::size_t mutation_estimator(Estimator &&estimator,
        std::enable_if_t<!std::is_integral<Estimator>::value> * = nullptr)
    {
        return this->estimator(2, std::forward<Estimator>(estimator));
    }

    estimator_type &mutation_estimator(std::size_t k)
    {
        return this->estimator(2, k);
    }

    const estimator_type &mutation_estimator(std::size_t k) const
    {
        return this->estimator(2, k);
    }

    /// \brief Iterate the sampler
    void iterate(std::size_t n = 1)
    {
        if (n > 1)
            reserve(n);
        for (std::size_t i = 0; i != n; ++i)
            do_iterate();
    }

    /// \brief Read and write access to the Particle<T> object
    Particle<T> &particle() { return particle_; }

    /// \brief Read only access to the Particle<T> object
    const Particle<T> &particle() const { return particle_; }

    /// \brief Read history the sampler sizes
    template <typename OutputIter>
    OutputIter read_size_history(OutputIter first) const
    {
        return std::copy(size_history_.begin(), size_history_.end(), first);
    }

    /// \brief Read the history of the values of ESS
    template <typename OutputIter>
    OutputIter read_ess_history(OutputIter first) const
    {
        return std::copy(ess_history_.begin(), ess_history_.end(), first);
    }

    private:
    Particle<T> particle_;
    std::size_t iter_;
    double resample_threshold_;
    Vector<size_type> size_history_;
    Vector<double> ess_history_;

    void do_iterate()
    {
        do_eval(0);
        do_estimate(0);

        size_history_.push_back(size());
        ess_history_.push_back(particle_.weight().ess());

        if (ess_history_.back() < size() * resample_threshold_)
            do_eval(1);
        do_estimate(1);

        do_eval(2);
        do_estimate(2);

        ++iter_;
    }

    void do_eval(std::size_t step)
    {
        for (auto &eval : this->eval(step))
            eval(iter_, particle_);
    }

    void do_estimate(std::size_t step)
    {
        for (auto &est : this->estimator(step))
            est.estimate(iter_, particle_);
    }
}; // class SMCSampler

} // namespace mckl

#endif // MCKL_ALGORITHM_SMC_HPP
