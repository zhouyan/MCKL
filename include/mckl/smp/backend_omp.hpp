//============================================================================
// MCKL/include/mckl/smp/backend_omp.hpp
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

#ifndef MCKL_SMP_BACKEND_OMP_HPP
#define MCKL_SMP_BACKEND_OMP_HPP

#include <mckl/smp/backend_base.hpp>

#if MCKL_HAS_OMP
#include <omp.h>
#endif

namespace mckl
{

namespace internal
{

template <typename IntType>
inline void backend_omp_range(IntType N, IntType &ibegin, IntType &iend)
{
#if MCKL_HAS_OMP
    const IntType np = static_cast<IntType>(::omp_get_num_threads());
    const IntType id = static_cast<IntType>(::omp_get_thread_num());
#else
    const IntType np = 1;
    const IntType id = 0;
#endif
    const IntType m = N / np;
    const IntType r = N % np;
    const IntType n = m + (id < r ? 1 : 0);
    ibegin = id < r ? n * id : (n + 1) * r + n * (id - r);
    iend = ibegin + n;
}

} // namespace mckl::internal

/// \brief Sampler<T>::eval_type subtype using OpenMP
/// \ingroup OMP
template <typename T, typename Derived>
class SamplerEvalSMP<T, Derived, BackendOMP>
    : public SamplerEvalBase<T, Derived>
{
    public:
    void operator()(std::size_t iter, Particle<T> &particle)
    {
        run(iter, particle);
    }

    protected:
    MCKL_DEFINE_SMP_BACKEND_SPECIAL(OMP, SamplerEval)

    void run(std::size_t iter, Particle<T> &particle)
    {
        run(iter, particle, 1);
    }

    template <typename... Args>
    void run(std::size_t iter, Particle<T> &particle, std::size_t, Args &&...)
    {
        using size_type = typename Particle<T>::size_type;

        this->eval_first(iter, particle);
        Particle<T> *pptr = &particle;
#if MCKL_HAS_OMP
#pragma omp parallel default(none) firstprivate(pptr, iter)
#endif
        {
            size_type ibegin = 0;
            size_type iend = 0;
            internal::backend_omp_range(pptr->size(), ibegin, iend);
            this->eval_range(iter, pptr->range(ibegin, iend));
        }
        this->eval_last(iter, particle);
    }
}; // class SamplerEvalSMP

/// \brief Monitor<T>::eval_type subtype using OpenMP
/// \ingroup OMP
template <typename T, typename Derived>
class MonitorEvalSMP<T, Derived, BackendOMP>
    : public MonitorEvalBase<T, Derived>
{
    public:
    void operator()(
        std::size_t iter, std::size_t dim, Particle<T> &particle, double *r)
    {
        run(iter, dim, particle, r);
    }

    protected:
    MCKL_DEFINE_SMP_BACKEND_SPECIAL(OMP, MonitorEval)

    void run(
        std::size_t iter, std::size_t dim, Particle<T> &particle, double *r)
    {
        run(iter, dim, particle, r, 1);
    }

    template <typename... Args>
    void run(std::size_t iter, std::size_t dim, Particle<T> &particle,
        double *r, std::size_t, Args &&...)
    {
        using size_type = typename Particle<T>::size_type;

        this->eval_first(iter, particle);
        Particle<T> *pptr = &particle;
#if MCKL_HAS_OMP
#pragma omp parallel default(none) firstprivate(pptr, iter, dim, r)
#endif
        {
            size_type ibegin = 0;
            size_type iend = 0;
            internal::backend_omp_range(pptr->size(), ibegin, iend);
            this->eval_range(iter, dim, pptr->range(ibegin, iend),
                r + static_cast<std::size_t>(ibegin) * dim);
        }
        this->eval_last(iter, particle);
    }
}; // class MonitorEvalSMP

/// \brief Sampler<T>::eval_type subtype using OpenMP
/// \ingroup OMP
template <typename T, typename Derived>
using SamplerEvalOMP = SamplerEvalSMP<T, Derived, BackendOMP>;

/// \brief Monitor<T>::eval_type subtype using OpenMP
/// \ingroup OMP
template <typename T, typename Derived>
using MonitorEvalOMP = MonitorEvalSMP<T, Derived, BackendOMP>;

} // namespace mckl

#endif // MCKL_SMP_BACKEND_OMP_HPP
