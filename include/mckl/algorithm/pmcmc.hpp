//============================================================================
// MCKL/include/mckl/algorithm/pmcmc.hpp
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

#ifndef MCKL_ALGORITHM_PMCMC_HPP
#define MCKL_ALGORITHM_PMCMC_HPP

#include <mckl/internal/common.hpp>
#include <mckl/algorithm/mcmc.hpp>
#include <mckl/algorithm/smc.hpp>
#include <mckl/core/state_matrix.hpp>
#include <mckl/random/rng.hpp>

namespace mckl
{

template <MatrixLayout Layout, std::size_t Dim, typename T, typename Param>
class PMCMCStateMatrix : public StateMatrix<Layout, Dim, T>
{
    public:
    using size_type = typename Matrix<Layout, T>::size_type;
    using value_type = typename Matrix<Layout, T>::value_type;
    using param_type = Param;

    explicit PMCMCStateMatrix(size_type N)
        : StateMatrix<Layout, Dim, T>(N), log_nc_(0)
    {
    }

    PMCMCStateMatrix(size_type N, size_type dim)
        : StateMatrix<Layout, Dim, T>(N, dim), log_nc_(0)
    {
    }

    double log_nc() const { return log_nc_; }

    void set_log_nc(double nc) { log_nc_ = nc; }

    void add_log_nc(double nc) { log_nc_ += nc; }

    const param_type &param() { return param_; }

    void param(const param_type &p) { return param_ = p; }

    private:
    double log_nc_;
    param_type param_;
}; // class PMCMCStateMatrix

/// \brief Particle Markov chain Monte Carlo mutation
/// \ingroup PMCMC
template <typename State, typename T>
class PMCMCMutation
{
    public:
    using state_type = State;
    using eval_type =
        std::function<double(typename Particle<T>::rng_type &, state_type &)>;
    using prior_type = std::function<double(const state_type &)>;

    template <typename... Args>
    PMCMCMutation(const prior_type &prior, Args &&... args)
        : prior_(prior), pf_(std::forward<Args>(args)...)
    {
    }

    SMCSampler<T> &pf() { return pf_; }

    SMCSampler<T> &pf() const { return pf_; }

    std::size_t operator()(std::size_t iter, state_type &state)
    {
        if (iter == 0) {
            pf_.clear();
            pf_.particle().state().reset(state);
            pf_.iterate(pf_.particle().state().n());
            return 0;
        }

        state_type s(state);
        double l = pf_.particle().state().log_nc();
        double p = -l;
        for (auto &eval : eval_)
            p += eval(pf_.particle().rng(), s);

        pf_.clear();
        pf_.particle().state().reset(s);
        pf_.iterate(pf_.particle().state().n());
        p += pf_.particle().state().log_nc();
        p += prior_(s) - prior_(state);

        mckl::U01Distribution<double> u01;
        double u = std::log(u01(pf_.particle().rng()));

        if (u < p) {
            state = s;
            return 1;
        }
        pf_.particle().state().log_nc(l);
        return 0;
    }

    void reset() { eval_.clear(); }

    template <typename Prior>
    void prior(Prior &&prior)
    {
        prior_ = std::forward<Prior>(prior);
    }

    /// \brief Add a new evaluation object for the mutation step
    template <typename Eval>
    void update(Eval &&eval)
    {
        eval_.emplace_back(std::forward<Eval>(eval));
    }

    private:
    prior_type prior_;
    SMCSampler<T> pf_;
    Vector<eval_type> eval_;
}; // class PMCMCMutation

} // namespace mckl

#endif // MCKL_ALGORITHM_PMCMC_HPP
