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

MCKL_PUSH_CLANG_WARNING("-Wpadded")

namespace mckl {

template <typename Param, MatrixLayout Layout, typename T, std::size_t Dim = 0>
class PMCMCStateMatrix : public StateMatrix<Layout, T, Dim>
{
  public:
    using param_type = Param;

    using StateMatrix<Layout, T, Dim>::StateMatrix;

    double log_nc() const { return log_nc_; }

    void add_log_nc(double nc) { log_nc_ += nc; }

    const param_type &param() { return param_; }

    void reset(const param_type &p, double nc = 0)
    {
        param_ = p;
        log_nc_ = nc;
    }

  private:
    param_type param_;
    double log_nc_;
}; // class PMCMCStateMatrix

/// \brief Particle Markov chain Monte Carlo mutation
/// \ingroup PMCMC
template <typename Param, typename T, typename U = double>
class PMCMCMutation
{
  public:
    using param_type = Param;
    using state_type = T;
    using size_type = typename Particle<T>::size_type;
    using eval_type =
        std::function<double(typename Particle<T>::rng_type &, param_type &)>;
    using prior_type = std::function<double(const param_type &)>;
    using pf_type = SMCSampler<T, U>;

    template <typename Prior, typename... Args>
    PMCMCMutation(std::size_t N, std::size_t M, Prior &&prior, Args &&... args)
        : M_(M), prior_(prior), pf_(N, std::forward<Args>(args)...)
    {
    }

    void reset()
    {
        eval_.clear();
        pf_.reset();
    }

    pf_type &pf() { return pf_; }

    pf_type &pf() const { return pf_; }

    PMCMCMutation<Param, T, U> clone() const
    {
        PMCMCMutation<Param, T, U> mutation(*this);
        mutation.pf_.particle().rng_set().reset();
        mutation.pf_.particle().rng().seed(
            Seed<typename Particle<T>::rng_type>::instance().get());

        return mutation;
    }

    template <typename Prior>
    void prior(Prior &&prior)
    {
        prior_ = std::forward<Prior>(prior);
    }

    /// \brief Add a new evaluation object for the update step
    template <typename Eval>
    std::size_t update(Eval &&eval)
    {
        eval_.push_back(std::forward<Eval>(eval));

        return eval_.size() - 1;
    }

    std::size_t operator()(std::size_t iter, param_type &param)
    {
        if (iter == 0) {
            pf_.clear();
            pf_.particle().state().reset(param, 0);
            pf_.iterate(M_);
            return 0;
        }

        const double lnc = pf_.particle().state().log_nc();

        double prob = -prior_(param) - lnc;
        param_type p(param);
        for (auto &eval : eval_) {
            prob += eval(pf_.particle().rng(), p);
        }
        prob += prior_(p);

        pf_.clear();
        pf_.particle().state().reset(p, 0);
        pf_.iterate(M_);
        prob += pf_.particle().state().log_nc();

        mckl::U01Distribution<double> u01;
        double u = std::log(u01(pf_.particle().rng()));

        if (u < prob) {
            param = std::move(p);
        } else {
            pf_.particle().state().reset(param, lnc);
        }

        return u < prob ? 1 : 0;
    }

  private:
    std::size_t M_;
    prior_type prior_;
    pf_type pf_;
    Vector<eval_type> eval_;
}; // class PMCMCMutation

} // namespace mckl

MCKL_POP_CLANG_WARNING

#endif // MCKL_ALGORITHM_PMCMC_HPP
