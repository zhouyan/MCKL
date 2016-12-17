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
#include <mckl/core/estimate_matrix.hpp>
#include <mckl/random/rng.hpp>

namespace mckl
{

/// \brief MCMC estimator
/// \ingroup MCMC
template <typename T>
class MCMCEstimator : public EstimateMatrix<double, 0>
{
    public:
    using state_type = T;
    using eval_type =
        std::function<void(std::size_t, std::size_t, state_type &, double *)>;

    template <typename Eval>
    MCMCEstimator(std::size_t dim, Eval &&eval)
        : EstimateMatrix<double, 0>(0, dim), eval_(std::forward<Eval>(eval))
    {
        runtime_assert(static_cast<bool>(eval_),
            "**MCMCEstimator::MCMCEstimator** used with an invalid evaluation "
            "object");
    }

    /// \brief Set a new evaluation object
    template <typename Eval>
    void estimate(Eval &&eval)
    {
        eval_ = std::forward<Eval>(eval);

        runtime_assert(static_cast<bool>(eval_),
            "**MCMCEstimator::estimate** used with an invalid evaluation "
            "object");
    }

    /// \brief Perform the evaluation given the iteration number and the state
    void operator()(std::size_t iter, state_type &state)
    {
        eval_(iter, this->dim(), state, this->insert_estimate());
    }

    /// \brief Get the average of estimates after `cut` iterations using every
    /// `thin` elements
    template <typename OutputIter>
    OutputIter average(
        OutputIter first, std::size_t cut = 0, std::size_t thin = 1) const
    {
        const std::size_t N = num_iter();

        if (cut >= N)
            return std::fill_n(first, this->dim(), -const_nan<double>());

        thin = thin < 1 ? 1 : thin;
        if (N - cut < thin)
            return this->read_estimate(cut, first);

        if (thin < 1)
            thin = 1;
        Vector<double> sum(this->dim());
        std::fill(sum.begin(), sum.end(), 0);
        std::size_t n = 0;
        while (cut < num_iter()) {
            add(this->dim(), this->row_data(cut), sum.data(), sum.data());
            cut += thin;
            ++n;
        }
        mul(this->dim(), 1.0 / n, sum.data(), sum.data());

        return std::copy(sum.begin(), sum.end(), first);
    }

    private:
    eval_type eval_;
}; // class MCMCEstimator

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
        for (auto &e : estimator_)
            e.second.reserve(n);
        for (auto &a : accept_history_)
            a.reserve(num_iter() + n);
    }

    /// \brief Reset the sampler by clear all history, evaluation objects, and
    /// estimators
    void reset()
    {
        eval_.clear();
        estimator_.clear();
        clear();
    }

    /// \brief Clear all history
    void clear()
    {
        iter_ = 0;
        for (auto &e : estimator_)
            e.second.clear();
        accept_history_.clear();
    }

    /// \brief Add a new evaluation object for the mutation step
    template <typename Eval>
    void mutation(Eval &&eval)
    {
        runtime_assert(num_iter() == 0,
            "**MCMCSampler::mutation** used after first iteration");

        eval_.emplace_back(std::forward<Eval>(eval));
        accept_history_.emplace_back(0);

        runtime_assert(static_cast<bool>(eval_.back()),
            "**MCMCSampler::mutation** used with an invalid evaluation "
            "object");
    }

    std::string estimator(const MCMCEstimator<T> &estimator,
        const std::string &name = std::string())
    {
        auto find = [this](const std::string &vname) {
            return std::find_if(estimator_.begin(), estimator_.end(),
                [&vname](auto &iter) { return iter.first == vname; });
        };

        std::string vname(name);
        if (name.empty()) {
            const std::string v("V");
            int i = 0;
            while (find(v + std::to_string(i)) != estimator_.end())
                ++i;
            vname = "V" + std::to_string(i);
        } else {
            auto f = find(vname);
            if (f != estimator_.end())
                estimator_.erase(f);
        }
        estimator_.emplace_back(vname, estimator);

        return vname;
    }

    const MCMCEstimator<T> &estimator(const std::string &name) const
    {
        auto exact = std::find_if(estimator_.begin(), estimator_.end(),
            [&name](auto &iter) { return iter.first == name; });
        if (exact != estimator_.end())
            return exact->second;

        auto partial = std::find_if(
            estimator_.begin(), estimator_.end(), [&name](auto &iter) {
                return iter.first.find(name) != std::string::npos;
            });
        if (partial != estimator_.end())
            return partial->second;

        runtime_assert(
            false, "**MCMCSampler::estimator** not found with the given name");

        return estimator_.front().second;
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

        for (const auto &e : estimator_) {
            for (std::size_t i = 0; i != e.second.dim(); ++i) {
                e.second.read_variable(i, data.begin());
                df[e.first + "." + std::to_string(i)] = data;
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
    Vector<std::pair<std::string, MCMCEstimator<T>>> estimator_;
    Vector<Vector<std::size_t>> accept_history_;

    void do_iterate()
    {
        for (std::size_t i = 0; i != eval_.size(); ++i)
            accept_history_[i].push_back(eval_[i](iter_, state_));

        for (auto &e : estimator_)
            e.second(iter_, state_);

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
