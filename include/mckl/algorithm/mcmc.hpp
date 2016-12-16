//============================================================================
// MCKL/include/mckl/algorithm/mcmc.hpp
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

#ifndef MCKL_ALGORITHM_MCMC_HPP
#define MCKL_ALGORITHM_MCMC_HPP

#include <mckl/internal/common.hpp>
#include <mckl/algorithm/mh.hpp>
#include <mckl/random/rng.hpp>

namespace mckl
{

/// \brief MCMC monitor of estimates
/// \ingroup MCMC
///
/// \tparam T The state type
///
/// \details
/// The record of estimates is represented by an \f$T\f$ by \f$D\f$ matrix
/// \f$R\f$, where \f$T\f$ is the number of iterations recorded and \f$D\f$ is
/// the dimesion of the monitor. If \f$D = 0\f$, then \f$T\f$ is always equal
/// to zero.
template <typename T>
class MCMCMonitor
{
    public:
    using state_type = T;
    using eval_type =
        std::function<void(std::size_t, std::size_t, state_type &, double *)>;

    template <typename Eval>
    MCMCMonitor(std::size_t dim, Eval &&eval)
        : dim_(dim), eval_(std::forward<Eval>(eval))
    {
    }

    /// \brief The dimension \f$D\f$
    std::size_t dim() const { return dim_; }

    /// \brief The number of iterations \f$T\f$
    std::size_t num_iter() const
    {
        return dim_ == 0 ? 0 : record_.size() / dim_;
    }

    /// \brief Reserve space for a specified number of iterations
    void reserve(std::size_t n) { record_.reserve(dim_ * n); }

    /// \brief If the evaluation object is valid
    bool empty() const { return !eval_; }

    /// \brief The value of \f$R_{T,i}\f$
    double record(std::size_t i) const
    {
        runtime_assert(
            i < dim(), "**MCMCMonitor::record** index out of range");
        runtime_assert(
            num_iter() > 0, "**MCMCMonitor::record** no iteration reocorded");

        return record_[(num_iter() - 1) * dim_ + i];
    }

    /// \brief The value of \f$R_{t,i}\f$
    double record(std::size_t i, std::size_t t) const
    {
        runtime_assert(
            i < dim(), "**MCMCMonitor::record** index out of range");
        runtime_assert(t < num_iter(),
            "**MCMCMonitor::record** iteration number out of range");

        return record_[t * dim_ + i];
    }

    /// \brief Read the values of \f$R_{1:T,i}\f$
    template <typename OutputIter>
    OutputIter read_record(std::size_t i, OutputIter first) const
    {
        runtime_assert(
            i < dim(), "**MCMCMonitor::read_record** index out of range");

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
            "**MCMCMonitor::read_record_matrix** invalid layout parameter");

        if (layout == RowMajor)
            return std::copy(record_.begin(), record_.end(), first);

        for (std::size_t d = 0; d != dim_; ++d)
            for (std::size_t i = 0; i != num_iter(); ++i)
                *first++ = record(d, i);

        return first;
    }

    /// \brief Set a new evaluation object
    template <typename Eval>
    void estimate(Eval &&eval)
    {
        eval_ = std::forward<Eval>(eval);
    }

    /// \brief Perform the evaluation given the iteration number and the state
    void operator()(std::size_t iter, state_type &state)
    {
        if (empty()) {
            result_.resize(dim_);
            std::fill(result_.begin(), result_.end(), const_nan<double>());
            record_.insert(record_.end(), result_.begin(), result_.end());
            return;
        }

        result_.resize(dim_);
        eval_(iter, dim_, state, result_.data());
        record_.insert(record_.end(), result_.begin(), result_.end());
    }

    /// \brief Get the average of estimates after `cut` iterations using every
    /// `thin` elements
    template <typename OutputIter>
    OutputIter result(
        OutputIter first, std::size_t cut = 0, std::size_t thin = 1)
    {
        const std::size_t N = num_iter();

        if (cut >= N)
            return std::fill_n(first, dim_, 0);

        thin = thin < 1 ? 1 : thin;
        if (N - cut < thin)
            return std::copy_n(record_.data() + cut * dim_, dim_, first);

        if (thin < 1)
            thin = 1;
        const std::size_t M = (N - cut) / thin;
        const double *r = record_.data() + cut * dim_;
        if (thin > 1) {
            v_.resize(M * dim_);
            double *v = v_.data();
            for (std::size_t i = 0; i != M; ++i, r += dim_ * thin, v += dim_)
                std::copy_n(r, dim_, v);
            r = v_.data();
        }

#if MCKL_HAS_BLAS
        w_.resize(M);
        result_.resize(dim_);
        std::fill(w_.begin(), w_.end(), 1.0 / M);
        internal::cblas_dgemv(internal::CblasColMajor, internal::CblasNoTrans,
            static_cast<MCKL_BLAS_INT>(dim_), static_cast<MCKL_BLAS_INT>(M),
            1.0, r, static_cast<MCKL_BLAS_INT>(dim_), w_.data(), 1, 0.0,
            result_.data(), 1);
#else  // MCKL_HAS_BLAS
// FIXME non-blas impl
#endif // MCKL_HAS_BLAS

        return std::copy(result_.begin(), result_.end(), first);
    }

    /// \brief Clear all records
    void clear() { record_.clear(); }

    private:
    std::size_t dim_;
    eval_type eval_;
    Vector<double> record_;
    Vector<double> result_;
    Vector<double> w_;
    Vector<double> v_;
}; // class MCMCMonitor

/// \brief MCMC sampler
/// \ingroup MCMC
template <typename T>
class MCMCSampler
{
    public:
    using state_type = T;
    using eval_type = std::function<std::size_t(std::size_t, state_type &)>;

    template <typename... Args>
    explicit MCMCSampler(Args &&... args)
        : state_(std::forward<Args>(args)...), iter_(0)
    {
    }

    /// \brief The number of iterations already performed
    std::size_t num_iter() const
    {
        return accept_history_.size() == 0 ? 0 :
                                             accept_history_.front().size();
    }

    /// \brief Reserve space for a specified number of iterations
    void reserve(std::size_t n)
    {
        n += num_iter();
        for (auto &m : monitor_)
            m.second.reserve(n);
        for (auto &a : accept_history_)
            a.reserve(n);
    }

    /// \brief Reset the sampler by clear all history, evaluation objects, and
    /// monitors
    void reset()
    {
        eval_.clear();
        monitor_.clear();
        clear();
    }

    /// \brief Clear all history
    void clear()
    {
        iter_ = 0;
        for (auto &m : monitor_)
            m.second.clear();
        accept_history_.clear();
    }

    /// \brief Add a new evaluation object for the mutation step
    template <typename Eval>
    void mutation(Eval &&eval)
    {
        runtime_assert(num_iter() == 0,
            "**MCMCSampler::mutation** called after first iteration");

        eval_.emplace_back(std::forward<Eval>(eval));
        accept_history_.emplace_back(0);
    }

    std::pair<std::string, MCMCMonitor<T>> &monitor(
        const MCMCMonitor<T> &monitor, const std::string &name = std::string())
    {
        auto find = [this](const std::string &vname) {
            return std::find_if(monitor_.begin(), monitor_.end(),
                [&vname](auto &iter) { return iter.first == vname; });
        };

        std::string vname(name);
        if (name.empty()) {
            const std::string v("V");
            int i = 0;
            while (find(v + std::to_string(i)) != monitor_.end())
                ++i;
            vname = "V" + std::to_string(i);
        } else {
            auto f = find(vname);
            if (f != monitor_.end())
                monitor_.erase(f);
        }
        monitor_.emplace_back(vname, monitor);

        return monitor_.back();
    }

    /// \brief Iterate the sampler
    void iterate(std::size_t n = 1)
    {
        if (n > 1)
            reserve(n);
        for (std::size_t i = 0; i != n; ++i)
            do_iterate();
    }

    /// \brief Read and write access to the state object
    state_type &state() { return state_; }

    /// \brief Read only access to the state object
    const state_type &state() const { return state_; }

    std::map<std::string, Vector<double>> summary() const
    {
        std::map<std::string, Vector<double>> df;
        Vector<double> data(num_iter());

        for (std::size_t i = 0; i != accept_history_.size(); ++i) {
            std::copy(accept_history_[i].begin(), accept_history_[i].end(),
                data.begin());
            df["Accept." + std::to_string(i)] = data;
        }

        for (const auto &m : monitor_) {
            for (std::size_t i = 0; i != m.second.dim(); ++i) {
                m.second.read_record(i, data.begin());
                df[m.first + "." + std::to_string(i)] = data;
            }
        }

        return df;
    }

    /// \brief Print the history of the MCMC sampler
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
    state_type state_;
    std::size_t iter_;
    Vector<eval_type> eval_;
    Vector<std::pair<std::string, MCMCMonitor<T>>> monitor_;
    Vector<Vector<std::size_t>> accept_history_;

    void do_iterate()
    {
        for (std::size_t i = 0; i != eval_.size(); ++i) {
            runtime_assert(static_cast<bool>(eval_[i]),
                "**MCMCSampler** empty evaluation object");
            accept_history_[i].push_back(eval_[i](iter_, state_));
        }

        for (auto &m : monitor_) {
            runtime_assert(
                !m.second.empty(), "**MCMCSampler** empty monitor object");
            m.second(iter_, state_);
        }

        ++iter_;
    }
}; // class MCMCSampler

template <typename CharT, typename Traits, typename T>
inline std::basic_ostream<CharT, Traits> &operator<<(
    std::basic_ostream<CharT, Traits> &os, const MCMCSampler<T> &sampler)
{
    return sampler.print(os);
}

} // namespace mckl

#endif // MCKL_ALGORITHM_MCMC_HPP
