//============================================================================
// MCKL/include/mckl/core/estimator.hpp
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

#ifndef MCKL_CORE_ESTIMATOR_HPP
#define MCKL_CORE_ESTIMATOR_HPP

#include <mckl/internal/common.hpp>
#include <mckl/core/estimate_matrix.hpp>

MCKL_PUSH_CLANG_WARNING("-Wpadded")

namespace mckl {

/// \brief Estimator for iterative Monte Carlo algorithms
/// \ingroup Core
template <typename T, typename... Args>
class Estimator : public EstimateMatrix<T>
{
  public:
    Estimator() = default;

    Estimator(std::size_t dim) : EstimateMatrix<T>(0, dim) {}

    template <typename Eval>
    Estimator(std::size_t dim, Eval &&eval)
        : EstimateMatrix<T>(dim), eval_(std::forward<Eval>(eval))
    {
    }

    using EstimateMatrix<T>::estimate;

    /// \brief Set a new estimate evaluation method
    template <typename Eval>
    std::enable_if_t<!std::is_convertible<Eval, int>::value> estimate(
        Eval &&eval)
    {
        eval_ = std::forward<Eval>(eval);
    }

    /// \brief Get the average of estimates after `cut` iterations using every
    /// `thin` elements
    template <typename OutputIter>
    OutputIter average(
        OutputIter first, std::size_t cut = 0, std::size_t thin = 1) const
    {
        const std::size_t n = this->num_iter();
        const std::size_t d = this->dim();

        if (cut >= n) {
            return first;
        }

        thin = thin < 1 ? 1 : thin;
        if (n - cut < thin) {
            return this->read_estimate(cut, first);
        }

        if (thin < 1) {
            thin = 1;
        }
        Vector<T> sum(d, T());
        std::size_t k = 0;
        while (cut < n) {
            add(d, this->row_data(cut), sum.data(), sum.data());
            cut += thin;
            ++k;
        }
        mul(d, static_cast<T>(1.0l / n), sum.data(), sum.data());

        return std::copy(sum.begin(), sum.end(), first);
    }

  protected:
    template <typename... CallArgs>
    void eval(CallArgs &&... args)
    {
        runtime_assert(static_cast<bool>(eval_),
            "**Estimator::eval** used with an invalid evaluation object");

        eval_(std::forward<CallArgs>(args)...);
    }

  private:
    std::function<void(Args...)> eval_;
}; // class Estimator

} // namespace mckl

MCKL_POP_CLANG_WARNING

#endif // MCKL_CORE_ESTIMATOR_HPP
