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

#ifndef MCKL_CORE_SMC_HPP
#define MCKL_CORE_SMC_HPP

#include <mckl/internal/common.hpp>
#include <mckl/core/particle.hpp>
#include <mckl/core/state_matrix.hpp>
#include <mckl/resample.hpp>

namespace mckl
{

/// \brief SMC monitor of Monte Carlo integrations
/// \ingroup SMC
///
/// \tparam T The state type
///
/// \details
/// The record of estimates is represented by an \f$T\f$ by \f$D\f$ matrix
/// \f$R\f$, where \f$T\f$ is the number of iterations recorded and \f$D\f$ is
/// the dimesion of the monitor. If \f$D = 0\f$, then \f$T\f$ is always equal
/// to zero.
template <typename T>
class SMCMonitor
{
    public:
    using eval_type =
        std::function<void(std::size_t, std::size_t, Particle<T> &, double *)>;

    SMCMonitor(std::size_t dim, const eval_type &eval, MatrixLayout layout,
        bool record_only = false)
        : dim_(dim), eval_(eval), layout_(layout), record_only_(record_only)
    {
        runtime_assert(layout == RowMajor || layout == ColMajor,
            "**SMCMonitor::SMCMonitor** invalid layout parameter");
    }

    /// \brief The dimension \f$D\f$
    std::size_t dim() const { return dim_; }

    /// \brief The number of iterations \f$T\f$
    std::size_t num_iter() const
    {
        return dim_ == 0 ? 0 : record_.size() / dim_;
    }

    /// \brief If this is a record only monitor
    bool record_only() const { return record_only_; }

    /// \brief Reserve space for a specified number of iterations
    void reserve(std::size_t n) { record_.reserve(dim_ * n); }

    /// \brief If the evaluation object is valid
    bool empty() const { return !eval_; }

    /// \brief The value of \f$R_{T,i}\f$
    double record(std::size_t i) const
    {
        runtime_assert(i < dim(), "**SMCMonitor::record** index out of range");
        runtime_assert(
            num_iter() > 0, "**SMCMonitor::record** no iteration reocorded");

        return record_[(num_iter() - 1) * dim_ + i];
    }

    /// \brief The value of \f$R_{t,i}\f$
    double record(std::size_t i, std::size_t t) const
    {
        runtime_assert(i < dim(), "**SMCMonitor::record** index out of range");
        runtime_assert(t < num_iter(),
            "**SMCMonitor::record** iteration number out of range");

        return record_[t * dim_ + i];
    }

    /// \brief Read the values of \f$R_{1:T,i}\f$
    template <typename OutputIter>
    OutputIter read_record(std::size_t i, OutputIter first) const
    {
        runtime_assert(
            i < dim(), "**Monitor::read_record** index out of range");

        const std::size_t t = num_iter();
        const double *riter = record_.data() + i;
        for (std::size_t j = 0; j != t; ++j, ++first, riter += dim_)
            *first = *riter;

        return first;
    }

    /// \brief Read the values of \f$R\f$
    template <typename OutputIter>
    OutputIter read_record_matrix(MatrixLayout layout, OutputIter first) const
    {
        runtime_assert(layout == RowMajor || layout == ColMajor,
            "**SMCMonitor::read_record_matrix** invalid layout parameter");

        if (layout == RowMajor)
            return std::copy(record_.begin(), record_.end(), first);

        if (layout == ColMajor) {
            for (std::size_t d = 0; d != dim_; ++d)
                for (std::size_t i = 0; i != num_iter(); ++i)
                    *first++ = record(d, i);
        }

        return first;
    }

    /// \brief Set a new evaluation object
    void eval(const eval_type &new_eval, MatrixLayout layout,
        bool record_only = false)
    {
        runtime_assert(layout == RowMajor || layout == ColMajor,
            "**SMCMonitor::eval** invalid layout parameter");

        eval_ = new_eval;
        layout_ = layout;
        record_only_ = record_only;
    }

    /// \brief Perform the evaluation given the iteration number and the
    /// particle system
    void operator()(std::size_t iter, Particle<T> &particle)
    {
        runtime_assert(static_cast<bool>(eval_),
            "**SMCMonitor::operator()** invalid evaluation object");

        result_.resize(dim_);
        if (record_only_) {
            eval_(iter, dim_, particle, result_.data());
            record_.insert(record_.end(), result_.begin(), result_.end());
            return;
        }

        const std::size_t N = static_cast<std::size_t>(particle.size());
        r_.resize(N * dim_);
        eval_(iter, dim_, particle, r_.data());
#if !MCKL_HAS_BLAS
        internal::size_check<MCKL_BLAS_INT>(particle.size(), "Monitor::eval");
        if (layout_ == RowMajor) {
            internal::cblas_dgemv(internal::CblasColMajor,
                internal::CblasNoTrans, static_cast<MCKL_BLAS_INT>(dim_),
                static_cast<MCKL_BLAS_INT>(N), 1.0, r_.data(),
                static_cast<MCKL_BLAS_INT>(dim_), particle.weight().data(), 1,
                0.0, result_.data(), 1);
        }
        if (layout_ == ColMajor) {
            internal::cblas_dgemv(internal::CblasColMajor,
                internal::CblasTrans, static_cast<MCKL_BLAS_INT>(N),
                static_cast<MCKL_BLAS_INT>(dim_), 1.0, r_.data(),
                static_cast<MCKL_BLAS_INT>(N), particle.weight().data(), 1,
                0.0, result_.data(), 1);
        }
#else  // MCKL_HAS_BLAS
        const double *w = particle.weight().data();
        double *r = r_.data();
        if (layout_ == RowMajor) {
            std::fill(result_.begin(), result_.end(), 0);
            for (std::size_t i = 0; i != N; ++i, ++w)
                for (std::size_t d = 0; d != dim_; ++d, ++r)
                    result_[d] += (*w) * (*r);
        }
        if (layout_ == ColMajor) {
            for (std::size_t d = 0; d != dim_; ++d, r += N) {
                mul(N, r, w, r);
                result_[d] = std::accumulate(r, r + N, 0.0);
            }
        }
#endif // MCKL_HAS_BLAS
        record_.insert(record_.end(), result_.begin(), result_.end());
    }

    /// \brief Clear all records
    void clear() { record_.clear(); }

    private:
    std::size_t dim_;
    eval_type eval_;
    MatrixLayout layout_;
    bool record_only_;
    Vector<double> record_;
    Vector<double> result_;
    Vector<double> r_;
}; // class Monitor

/// \brief SMC sampler
/// \ingroup SMC
template <typename T>
class SMCSampler
{
    public:
    using size_type = typename Particle<T>::size_type;
    using eval_type = std::function<void(std::size_t, Particle<T> &)>;

    /// \brief Construct a SMC sampler
    ///
    /// \details
    /// All arguments are passed to the constructor of Particle
    template <typename... Args>
    explicit SMCSampler(Args &&... args)
        : particle_(std::forward<Args>(args)...)
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
        n += num_iter();
        for (auto &m : monitor_s_)
            m.second.reserve(n);
        for (auto &m : monitor_r_)
            m.second.reserve(n);
        for (auto &m : monitor_m_)
            m.second.reserve(n);
        size_history_.reserve(n);
        ess_history_.reserve(n);
    }

    /// \brief Reset the sampler by clear all history, evaluation objects, and
    /// monitors
    void reset()
    {
        eval_s_.clear();
        eval_r_.clear();
        eval_m_.clear();
        monitor_s_.clear();
        monitor_r_.clear();
        monitor_m_.clear();
        clear();
    }

    /// \brief Clear all history
    void clear()
    {
        iter_ = 0;
        particle_.weight().set_equal();
        for (auto &m : monitor_s_)
            m.second.clear();
        for (auto &m : monitor_r_)
            m.second.clear();
        for (auto &m : monitor_m_)
            m.second.clear();
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
            "**SMCSampler::selection** called after first iteration");

        eval_s_.emplace_back(std::forward<Eval>(eval));
    }

    /// \brief Add a new evaluation object for the selection step
    template <typename Eval>
    void resample(Eval &&eval,
        std::enable_if_t<!std::is_convertible<Eval, ResampleScheme>::value> * =
            nullptr)
    {
        runtime_assert(num_iter() == 0,
            "**SMCSampler::resample** called after first iteration");

        eval_r_.emplace_back(std::forward<Eval>(eval));
    }

    /// \brief Add a new evaluation object for the resample step by a built-in
    /// resample scheme
    void resample(ResampleScheme scheme)
    {
        runtime_assert(num_iter() == 0,
            "**SMCSampler::resample** called after first iteration");

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
            "**SMCSampler::mutation** called after first iteration");

        eval_m_.emplace_back(std::forward<Eval>(eval));
    }

    /// \brief Attach a new monitor and return a reference to it
    const std::pair<std::string, SMCMonitor<T>> &monitor_selection(
        const SMCMonitor<T> &monitor, const std::string &name = std::string())
    {
        runtime_assert(num_iter() == 0,
            "**SMCSampler::monitor_selection** called after first iteration");

        return add_monitor(monitor_s_, monitor, name);
    }

    /// \brief Attach a new monitor and return a reference to it
    const std::pair<std::string, SMCMonitor<T>> &monitor_resample(
        const SMCMonitor<T> &monitor, const std::string &name = std::string())
    {
        runtime_assert(num_iter() == 0,
            "**SMCSampler::monitor_resample** called after first iteration");

        return add_monitor(monitor_r_, monitor, name);
    }

    /// \brief Attach a new monitor and return a reference to it
    const std::pair<std::string, SMCMonitor<T>> &monitor_mutation(
        const SMCMonitor<T> &monitor, const std::string &name = std::string())
    {
        runtime_assert(num_iter() == 0,
            "**SMCSampler::monitor_mutation** called after first iteration");

        return add_monitor(monitor_m_, monitor, name);
    }

    /// \brief Iteration
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

        for (const auto &m : monitor_s_) {
            for (std::size_t i = 0; i != m.second.dim(); ++i) {
                m.second.read_record(i, data.begin());
                df[m.first + "." + std::to_string(i)] = data;
            }
        }

        for (const auto &m : monitor_r_) {
            for (std::size_t i = 0; i != m.second.dim(); ++i) {
                m.second.read_record(i, data.begin());
                df[m.first + "." + std::to_string(i)] = data;
            }
        }

        for (const auto &m : monitor_m_) {
            for (std::size_t i = 0; i != m.second.dim(); ++i) {
                m.second.read_record(i, data.begin());
                df[m.first + "." + std::to_string(i)] = data;
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
    eval_type resample_eval_;
    Vector<eval_type> eval_s_;
    Vector<eval_type> eval_r_;
    Vector<eval_type> eval_m_;
    Vector<std::pair<std::string, SMCMonitor<T>>> monitor_s_;
    Vector<std::pair<std::string, SMCMonitor<T>>> monitor_r_;
    Vector<std::pair<std::string, SMCMonitor<T>>> monitor_m_;
    Vector<size_type> size_history_;
    Vector<double> ess_history_;

    const std::pair<std::string, SMCMonitor<T>> &add_monitor(
        Vector<std::pair<std::string, SMCMonitor<T>>> &vector,
        const SMCMonitor<T> &monitor, const std::string &name)
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
        vector.emplace_back(vname, monitor);

        return vector.back();
    }

    void do_iterate()
    {
        do_eval(eval_s_);
        do_monitor(monitor_s_);

        size_history_.push_back(size());
        ess_history_.push_back(particle_.weight().ess());

        if (ess_history_.back() < size() * resample_threshold_)
            do_eval(eval_r_);
        do_monitor(monitor_r_);

        do_eval(eval_m_);
        do_monitor(monitor_m_);

        ++iter_;
    }

    void do_eval(Vector<eval_type> &vector)
    {
        for (auto &eval : vector) {
            runtime_assert(static_cast<bool>(eval),
                "**SMCSampler** empty evaluation object");
            eval(iter_, particle_);
        }
    }

    void do_monitor(Vector<std::pair<std::string, SMCMonitor<T>>> &vector)
    {
        for (auto &m : vector) {
            runtime_assert(
                !m.second.empty(), "**SMCSampler** empty monitor object");
            m.second(iter_, particle_);
        }
    }
}; // class SMCSampler

template <typename CharT, typename Traits, typename T>
inline std::basic_ostream<CharT, Traits> &operator<<(
    std::basic_ostream<CharT, Traits> &os, const SMCSampler<T> &sampler)
{
    return sampler.print(os);
}

} // namespace mckl

#endif // MCKL_CORE_SAMPLER_HPP
