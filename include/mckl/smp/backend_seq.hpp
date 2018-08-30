//============================================================================
// MCKL/include/mckl/smp/backend_seq.hpp
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

#ifndef MCKL_SMP_BACKEND_SEQ_HPP
#define MCKL_SMP_BACKEND_SEQ_HPP

#include <mckl/smp/backend_base.hpp>

namespace mckl {

/// \brief SMCSampler<T>::eval_type subtype
/// \ingroup SEQ
template <typename T, typename Derived>
class SMCSamplerEvalSMP<T, Derived, BackendSEQ>
    : public SMCSamplerEvalBase<T, Derived>
{
  public:
    void operator()(std::size_t iter, Particle<T> &particle)
    {
        run(iter, particle);
    }

  protected:
    MCKL_DEFINE_SMP_BACKEND_SPECIAL(SEQ, SMCSamplerEval)

    void run(std::size_t iter, Particle<T> &particle)
    {
        run(iter, particle, 1);
    }

    template <typename... Args>
    void run(std::size_t iter, Particle<T> &particle, std::size_t, Args &&...)
    {
        this->eval_first(iter, particle);
        this->eval_range(iter, particle.range());
        this->eval_last(iter, particle);
    }
}; // class SMCSamplerEvalSMP

/// \brief SMCEstimator<T>::eval_type subtype
/// \ingroup SEQ
template <typename T, typename Derived>
class SMCEstimatorEvalSMP<T, Derived, BackendSEQ>
    : public SMCEstimatorEvalBase<T, Derived>
{
  public:
    void operator()(
        std::size_t iter, std::size_t dim, Particle<T> &particle, double *r)
    {
        run(iter, dim, particle, r);
    }

  protected:
    MCKL_DEFINE_SMP_BACKEND_SPECIAL(SEQ, SMCEstimatorEval)

    void run(
        std::size_t iter, std::size_t dim, Particle<T> &particle, double *r)
    {
        run(iter, dim, particle, r, 1);
    }

    template <typename... Args>
    void run(std::size_t iter, std::size_t dim, Particle<T> &particle,
        double *r, std::size_t, Args &&...)
    {
        this->eval_first(iter, particle);
        this->eval_range(iter, dim, particle.range(), r);
        this->eval_last(iter, particle);
    }
}; // class SMCEstimatorEvalSMP

/// \brief SMCSampler<T>::eval_type subtype
/// \ingroup SEQ
template <typename T, typename Derived>
using SMCSamplerEvalSEQ = SMCSamplerEvalSMP<T, Derived, BackendSEQ>;

/// \brief SMCEstimator<T>::eval_type subtype
/// \ingroup SEQ
template <typename T, typename Derived>
using SMCEstimatorEvalSEQ = SMCEstimatorEvalSMP<T, Derived, BackendSEQ>;

} // namespace mckl

#endif // MCKL_SMP_BACKEND_SEQ_HPP
