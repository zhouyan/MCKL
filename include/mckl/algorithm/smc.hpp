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
#include <mckl/core/state_matrix.hpp>

namespace mckl
{

/// \brief SMC estimator
/// \ingroup SMC
template <typename T, typename U>
class SMCEstimator
    : public Estimator<U, std::size_t, std::size_t, Particle<T> &, U *>
{
    static_assert(std::is_convertible<U, double>::value,
        "**SMCEsimator** used with estimate type U not convertible to double");

    public:
    SMCEstimator() = default;

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
        const double *r = rptr(std::is_same<U, double>());

#if MCKL_HAS_BLAS
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
#else  // MCKL_HAS_BLAS
        if (layout_ == RowMajor) {
            std::fill(result_.begin(), result_.end(), 0);
            for (std::size_t i = 0; i != n; ++i, ++w)
                for (std::size_t d = 0; d != d; ++d, ++r)
                    result_[d] += (*w) * (*r);
        } else {
            for (std::size_t d = 0; d != d; ++d, r += n) {
                mul(n, r, w, r);
                result_[d] = std::accumulate(r, r + n, 0.0);
            }
        }
#endif // MCKL_HAS_BLAS
        this->insert_estimate(result_.data());
    }

    private:
    Vector<U> u_;
    Vector<double> r_;
    Vector<double> result_;
    MatrixLayout layout_;
    bool record_only_;

    const double *rptr(std::true_type) { return u_.data(); }

    const double *rptr(std::false_type)
    {
        r_.resize(u_.size());
        std::copy(u_.begin(), u_.end(), r_.begin());

        return r_.data();
    }
}; // class SMCEstimator

/// \brief SMC sampler
/// \ingroup SMC
template <typename T, typename U = double>
class SMCSampler
{
    public:
    using size_type = typename Particle<T>::size_type;
    using eval_type = std::function<void(std::size_t, Particle<T> &)>;
    using estimator_type = SMCEstimator<T, U>;

    SMCSampler() : iter_(0), resample_threshold_(resample_threshold_never()) {}

    /// \brief Construct a SMC sampler
    ///
    /// \details
    /// All arguments are passed to the constructor of Particle
    template <typename... Args>
    explicit SMCSampler(size_type N, Args &&... args)
        : particle_(N, std::forward<Args>(args)...)
        , iter_(0)
        , resample_threshold_(resample_threshold_never())
    {
    }

    /// \brief Clone the SMC sampler except the RNG engines
    SMCSampler<T> clone() const
    {
        SMCSampler<T> sampler(*this);
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
        for (auto &e : estimator_s_)
            e.second.reserve(n);
        for (auto &e : estimator_r_)
            e.second.reserve(n);
        for (auto &e : estimator_m_)
            e.second.reserve(n);
        n += num_iter();
        size_history_.reserve(n);
        ess_history_.reserve(n);
    }

    /// \brief Reset the sampler by clear all history, evaluation objects, and
    /// estimators
    void reset()
    {
        eval_s_.clear();
        eval_r_.clear();
        eval_m_.clear();
        estimator_s_.clear();
        estimator_r_.clear();
        estimator_m_.clear();
        clear();
    }

    /// \brief Clear all history
    void clear()
    {
        iter_ = 0;
        particle_.weight().set_equal();
        for (auto &e : estimator_s_)
            e.second.clear();
        for (auto &e : estimator_r_)
            e.second.clear();
        for (auto &e : estimator_m_)
            e.second.clear();
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
    void selection(Eval &&eval)
    {
        runtime_assert(num_iter() == 0,
            "**SMCSampler::selection** used after first iteration");

        eval_s_.emplace_back(std::forward<Eval>(eval));

        runtime_assert(static_cast<bool>(eval_s_.back()),
            "**SMCSampler::selection** used with an invalid evaluation "
            "object");
    }

    /// \brief Add a new evaluation object for the selection step
    template <typename Eval>
    void resample(Eval &&eval,
        std::enable_if_t<!std::is_convertible<Eval, ResampleScheme>::value> * =
            nullptr)
    {
        runtime_assert(num_iter() == 0,
            "**SMCSampler::resample** used after first iteration");

        eval_r_.emplace_back(std::forward<Eval>(eval));

        runtime_assert(static_cast<bool>(eval_r_.back()),
            "**SMCSampler::resample** used with an invalid evaluation object");
    }

    /// \brief Add a new evaluation object for the resample step by a built-in
    /// resample scheme
    void resample(ResampleScheme scheme)
    {
        runtime_assert(num_iter() == 0,
            "**SMCSampler::resample** used after first iteration");

        switch (scheme) {
            case Multinomial:
                resample(ResampleEval<T>(ResampleMultinomial()));
                break;
            case Residual:
                resample(ResampleEval<T>(ResampleResidual()));
                break;
            case Stratified:
                resample(ResampleEval<T>(ResampleStratified()));
                break;
            case Systematic:
                resample(ResampleEval<T>(ResampleSystematic()));
                break;
            case ResidualStratified:
                resample(ResampleEval<T>(ResampleResidualStratified()));
                break;
            case ResidualSystematic:
                resample(ResampleEval<T>(ResampleResidualSystematic()));
                break;
        }
    }

    /// \brief Add a new evaluation object for the mutation step
    template <typename Eval>
    void mutation(Eval &&eval)
    {
        runtime_assert(num_iter() == 0,
            "**SMCSampler::mutation** used after first iteration");

        eval_m_.emplace_back(std::forward<Eval>(eval));

        runtime_assert(static_cast<bool>(eval_m_.back()),
            "**SMCSampler::mutation** used with an invalid evaluation object");
    }

    /// \brief Attach a new estimator and return a reference to it
    std::string selection_estimator(const estimator_type &estimator,
        const std::string &name = std::string())
    {
        runtime_assert(num_iter() == 0,
            "**SMCSampler::selection_estimator** used after first iteration");

        return insert_estimator(estimator_s_, estimator, name);
    }

    /// \brief Attach a new estimator and return a reference to it
    std::string resample_estimator(const estimator_type &estimator,
        const std::string &name = std::string())
    {
        runtime_assert(num_iter() == 0,
            "**SMCSampler::resample_estimator** used after first iteration");

        return insert_estimator(estimator_r_, estimator, name);
    }

    /// \brief Attach a new estimator and return a reference to it
    std::string mutation_estimator(const estimator_type &estimator,
        const std::string &name = std::string())
    {
        runtime_assert(num_iter() == 0,
            "**SMCSampler::mutation_estimator** used after first iteration");

        return insert_estimator(estimator_m_, estimator, name);
    }

    /// \brief Get read only access to estimator in the selection step given
    /// (partial) name
    const estimator_type &estimator_selection(const std::string &name) const
    {
        return find_estimator(estimator_s_, name);
    }

    /// \brief Get read only access to estimator in the resample step
    const estimator_type &estimator_resample(const std::string &name) const
    {
        return find_estimator(estimator_r_, name);
    }

    /// \brief Get read only access to estimator in the mutation step
    const estimator_type &estimator_mutation(const std::string &name) const
    {
        return find_estimator(estimator_m_, name);
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

    /// \brief Get the sample size given an iteration number
    size_type size_history(std::size_t t) const { return size_history_[t]; }

    /// \brief Read history the sampler sizes
    template <typename OutputIter>
    OutputIter read_size_history(OutputIter first) const
    {
        return std::copy(size_history_.begin(), size_history_.end(), first);
    }

    /// \brief Get the value of ESS given an iteration
    double ess_history(std::size_t t) const { return ess_history_[t]; }

    /// \brief Read the history of the values of ESS
    template <typename OutputIter>
    OutputIter read_ess_history(OutputIter first) const
    {
        return std::copy(ess_history_.begin(), ess_history_.end(), first);
    }

    /// \brief Summary of SMC sampler history
    std::map<std::string, Vector<double>> summary() const
    {
        std::map<std::string, Vector<double>> df;
        Vector<double> data(num_iter());

        std::copy(size_history_.begin(), size_history_.end(), data.begin());
        df["Size"] = data;

        df["ESS"] = ess_history_;

        for (const auto &e : estimator_s_) {
            for (std::size_t i = 0; i != e.second.dim(); ++i) {
                e.second.read_variable(i, data.begin());
                df[e.first + "." + std::to_string(i)] = data;
            }
        }

        for (const auto &e : estimator_r_) {
            for (std::size_t i = 0; i != e.second.dim(); ++i) {
                e.second.read_variable(i, data.begin());
                df[e.first + "." + std::to_string(i)] = data;
            }
        }

        for (const auto &e : estimator_m_) {
            for (std::size_t i = 0; i != e.second.dim(); ++i) {
                e.second.read_variable(i, data.begin());
                df[e.first + "." + std::to_string(i)] = data;
            }
        }

        return df;
    }

    /// \brief Print the history of the SMC sampler
    ///
    /// \param os The ostream to which the contents are printed
    /// \param sepchar The seperator of fields
    template <typename CharT, typename Traits>
    std::basic_ostream<CharT, Traits> &print(
        std::basic_ostream<CharT, Traits> &os, char sepchar = '\t') const
    {
        if (!os || num_iter() == 0)
            return os;

        const auto df = summary();

        for (const auto &v : df)
            os << v.first << sepchar;
        os << '\n';
        for (std::size_t i = 0; i != num_iter(); ++i) {
            for (const auto &v : df)
                os << v.second[i] << sepchar;
            os << '\n';
        }

        return os;
    }

    private:
    Particle<T> particle_;
    std::size_t iter_;
    double resample_threshold_;
    Vector<eval_type> eval_s_;
    Vector<eval_type> eval_r_;
    Vector<eval_type> eval_m_;
    Vector<std::pair<std::string, estimator_type>> estimator_s_;
    Vector<std::pair<std::string, estimator_type>> estimator_r_;
    Vector<std::pair<std::string, estimator_type>> estimator_m_;
    Vector<size_type> size_history_;
    Vector<double> ess_history_;

    std::string insert_estimator(
        Vector<std::pair<std::string, estimator_type>> &vector,
        const estimator_type &estimator, const std::string &name)
    {
        auto find = [&vector](const std::string &vname) {
            return std::find_if(vector.begin(), vector.end(),
                [&vname](auto &iter) { return iter.first == vname; });
        };

        std::string vname(name);
        if (name.empty()) {
            const std::string v("V");
            int i = 0;
            while (find(v + std::to_string(i)) != vector.end())
                ++i;
            vname = "V" + std::to_string(i);
        } else {
            auto f = find(vname);
            if (f != vector.end())
                vector.erase(f);
        }
        vector.emplace_back(vname, estimator);

        return vname;
    }

    const estimator_type &find_estimator(
        Vector<std::pair<std::string, estimator_type>> &vector,
        const std::string &name) const
    {
        auto exact = std::find_if(vector.begin(), vector.end(),
            [&name](auto &iter) { return iter.first == name; });
        if (exact != vector.end())
            return exact->second;

        auto partial =
            std::find_if(vector.begin(), vector.end(), [&name](auto &iter) {
                return iter.first.find(name) != std::string::npos;
            });
        if (partial != vector.end())
            return partial->second;

        runtime_assert(
            false, "**SMCSampler::estimator** not found with the given name");

        return vector.front().second;
    }

    void do_iterate()
    {
        do_eval(eval_s_);
        do_estimate(estimator_s_);

        size_history_.push_back(size());
        ess_history_.push_back(particle_.weight().ess());

        if (ess_history_.back() < size() * resample_threshold_)
            do_eval(eval_r_);
        do_estimate(estimator_r_);

        do_eval(eval_m_);
        do_estimate(estimator_m_);

        ++iter_;
    }

    void do_eval(Vector<eval_type> &vector)
    {
        for (auto &eval : vector)
            eval(iter_, particle_);
    }

    void do_estimate(Vector<std::pair<std::string, estimator_type>> &vector)
    {
        for (auto &e : vector)
            e.second.estimate(iter_, particle_);
    }
}; // class SMCSampler

template <typename CharT, typename Traits, typename T, typename U>
inline std::basic_ostream<CharT, Traits> &operator<<(
    std::basic_ostream<CharT, Traits> &os, const SMCSampler<T, U> &sampler)
{
    return sampler.print(os);
}

} // namespace mckl

#endif // MCKL_ALGORITHM_SMC_HPP
