//============================================================================
// MCKL/include/mckl/algorithm/mcmc.hpp
//----------------------------------------------------------------------------
// MCKL: Monte Carlo Kernel Library
//----------------------------------------------------------------------------
// Copyright (c) 2013-2018, Yan Zhou
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
#include <mckl/core/estimator.hpp>
#include <mckl/core/sampler.hpp>
#include <mckl/random/rng.hpp>

namespace mckl {

/// \brief MCMC estimator
/// \ingroup MCMC
template <typename T, typename U = double>
class MCMCEstimator : public Estimator<U, std::size_t, std::size_t, T &, U *>
{
  public:
    using Estimator<U, std::size_t, std::size_t, T &, U *>::Estimator;
    using Estimator<U, std::size_t, std::size_t, T &, U *>::estimate;

    void estimate(std::size_t iter, T &state)
    {
        this->eval(iter, this->dim(), state, this->insert_estimate());
    }
}; // class MCMCEstimator

template <typename, typename = double>
class MCMCSampler;

template <typename T, typename U>
class SamplerTrait<MCMCSampler<T, U>>
{
  public:
    using eval_type = std::function<std::size_t(std::size_t, T &)>;
    using estimator_type = MCMCEstimator<T, U>;
}; // class SamplerTrait

/// \brief MCMC sampler
/// \ingroup MCMC
template <typename T, typename U>
class MCMCSampler : public Sampler<MCMCSampler<T, U>>
{
  public:
    using state_type = T;
    using eval_type = typename Sampler<MCMCSampler<T, U>>::eval_type;
    using estimator_type = typename Sampler<MCMCSampler<T, U>>::estimator_type;

    template <typename... Args>
    explicit MCMCSampler(Args &&... args)
        : Sampler<MCMCSampler<T, U>>(1)
        , state_(std::forward<Args>(args)...)
        , iter_(0)
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
        Sampler<MCMCSampler<T, U>>::reserve(n);
        for (auto &a : accept_history_) {
            a.reserve(num_iter() + n);
        }
    }

    /// \brief Reset the sampler by clear all history, evaluation objects, and
    /// estimators
    void reset()
    {
        Sampler<MCMCSampler<T, U>>::reset();
        clear();
    }

    /// \brief Clear all history
    void clear()
    {
        Sampler<MCMCSampler<T, U>>::clear();
        iter_ = 0;
        accept_history_.clear();
    }

    /// \brief Add a new evaluation object for the mutation step
    template <typename Eval>
    std::size_t mutation(Eval &&eval,
        std::enable_if_t<!std::is_integral<Eval>::value> * = nullptr)
    {
        return this->eval(0, std::forward<Eval>(eval));
    }

    eval_type &mutation(std::size_t k) { return this->eval(0, k); }

    const eval_type &mutation(std::size_t k) const { return this->eval(0, k); }

    template <typename Estimator>
    std::size_t estimator(Estimator &&estimator)
    {
        return Sampler<MCMCSampler<T, U>>::estimator(
            0, std::forward<Estimator>(estimator));
    }

    estimator_type &estimator(std::size_t k)
    {
        return Sampler<MCMCSampler<T, U>>::estimator(0, k);
    }

    const estimator_type &estimator(std::size_t k) const
    {
        return Sampler<MCMCSampler<T, U>>::estimator(0, k);
    }

    /// \brief Iterate the sampler
    void iterate(std::size_t n = 1)
    {
        if (n > 1) {
            reserve(n);
        }
        for (std::size_t i = 0; i != n; ++i) {
            do_iterate();
        }
    }

    /// \brief Read and write access to the state object
    state_type &state() { return state_; }

    /// \brief Read only access to the state object
    const state_type &state() const { return state_; }

    /// \brief Read accept count history given mutation step index
    template <typename OutputIter>
    OutputIter read_accept_history(std::size_t i, OutputIter first) const
    {
        runtime_assert(i < accept_history_.size(),
            "**MCMCSampler::read_accept_history** index out of range");

        return std::copy(
            accept_history_[i].begin(), accept_history_[i].end(), first);
    }

    /// \brief Read all accept count history into a matrix
    template <typename OutputIter>
    OutputIter read_accept_history(MatrixLayout layout, OutputIter first) const
    {
        if (layout == RowMajor) {
            if (accept_history_.size() > 0) {
                const std::size_t n = accept_history_.front().size();
                const std::size_t d = accept_history_.size();
                for (std::size_t i = 0; i != n; ++i) {
                    for (std::size_t j = 0; j != d; ++j) {
                        *first++ = accept_history_[j][i];
                    }
                }
            }
        } else {
            for (std::size_t i = 0; i != accept_history_.size(); ++i) {
                first = read_accept_history(i, first);
            }
        }

        return first;
    }

  private:
    state_type state_;
    std::size_t iter_;
    Vector<Vector<std::size_t>> accept_history_;

    void do_iterate()
    {
        accept_history_.resize(this->eval(0).size());
        for (std::size_t i = 0; i != this->eval(0).size(); ++i) {
            accept_history_[i].push_back(this->eval(0)[i](iter_, state_));
        }

        for (auto &e : Sampler<MCMCSampler<T, U>>::estimator(0)) {
            e.estimate(iter_, state_);
        }

        ++iter_;
    }
}; // class MCMCSampler

} // namespace mckl

#endif // MCKL_ALGORITHM_MCMC_HPP
