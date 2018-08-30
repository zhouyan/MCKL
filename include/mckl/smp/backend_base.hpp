//============================================================================
// MCKL/include/mckl/smp/backend_base.hpp
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

#ifndef MCKL_SMP_BACKEND_BASE_HPP
#define MCKL_SMP_BACKEND_BASE_HPP

#include <mckl/internal/common.hpp>
#include <mckl/core/particle.hpp>

MCKL_PUSH_CLANG_WARNING("-Wweak-vtables")

/// \brief Default SMP backend
/// \ingroup Config
#ifndef MCKL_SMP_BACKEND
#if MCKL_HAS_TBB
#define MCKL_SMP_BACKEND ::mckl::BackendTBB
#elif MCKL_HAS_OMP
#define MCKL_SMP_BACKEND ::mckl::BackendOMP
#else
#define MCKL_SMP_BACKEND ::mckl::BackendSTD
#endif
#endif

#define MCKL_DEFINE_SMP_BACKEND_BASE_SPECIAL(Name)                            \
    Name##Base() = default;                                                   \
    Name##Base(const Name##Base<T, Derived> &) = default;                     \
    Name##Base<T, Derived> &operator=(const Name##Base<T, Derived> &) =       \
        default;                                                              \
    Name##Base(Name##Base<T, Derived> &&) = default;                          \
    Name##Base<T, Derived> &operator=(Name##Base<T, Derived> &&) = default;

#define MCKL_DEFINE_SMP_BACKEND_BASE_SPECIAL_VIRTUAL(Name)                    \
    Name##Base() = default;                                                   \
    Name##Base(const Name##Base<T, Virtual> &) = default;                     \
    Name##Base<T, Virtual> &operator=(const Name##Base<T, Virtual> &) =       \
        default;                                                              \
    Name##Base(Name##Base<T, Virtual> &&) = default;                          \
    Name##Base<T, Virtual> &operator=(Name##Base<T, Virtual> &&) = default;   \
    virtual ~Name##Base() = default;

#define MCKL_DEFINE_SMP_BACKEND_SPECIAL(Impl, Name)                           \
    Name##SMP() = default;                                                    \
    Name##SMP(const Name##SMP<T, Derived, Backend##Impl> &) = default;        \
    Name##SMP<T, Derived, Backend##Impl> &operator=(                          \
        const Name##SMP<T, Derived, Backend##Impl> &) = default;              \
    Name##SMP(Name##SMP<T, Derived, Backend##Impl> &&) = default;             \
    Name##SMP<T, Derived, Backend##Impl> &operator=(                          \
        Name##SMP<T, Derived, Backend##Impl> &&) = default;

namespace mckl {

/// \brief SMP implementation ID for sequential
/// \ingroup SMP
class BackendSEQ;

/// \brief SMP implementation ID for the standard library
/// \ingroup SMP
class BackendSTD;

/// \brief SMP implementation ID for OpenMP
/// \ingroup SMP
class BackendOMP;

/// \brief SMP implementation ID for Intel Threading Building Blocks
/// \ingroup SMP
class BackendTBB;

/// \brief SMP default implementation ID
/// \ingroup SMP
using BackendSMP = MCKL_SMP_BACKEND;

/// \brief SMCSMCSampler<T>::eval_type
/// \ingroup SMP
template <typename T, typename = Virtual, typename = BackendSMP>
class SMCSamplerEvalSMP;

/// \brief SMCEstimator<T>::eval_type
/// \ingroup SMP
template <typename T, typename = Virtual, typename = BackendSMP>
class SMCEstimatorEvalSMP;

/// \brief SMCSampler evaluation base dispatch class
/// \ingroup SMP
template <typename T, typename Derived>
class SMCSamplerEvalBase
{
  protected:
    MCKL_DEFINE_SMP_BACKEND_BASE_SPECIAL(SMCSamplerEval)

    void eval_each(std::size_t iter, ParticleIndex<T> idx)
    {
        eval_dispatch(iter, idx, &Derived::eval_each);
    }

    void eval_range(std::size_t iter, const ParticleRange<T> &range)
    {
        eval_range_dispatch(iter, range, &Derived::eval_range);
    }

    void eval_first(std::size_t iter, Particle<T> &particle)
    {
        eval_first_dispatch(iter, particle, &Derived::eval_first);
    }

    void eval_last(std::size_t iter, Particle<T> &particle)
    {
        eval_last_dispatch(iter, particle, &Derived::eval_last);
    }

  private:
    // non-static non-const

    template <typename D>
    void eval_dispatch(std::size_t iter, ParticleIndex<T> idx,
        void (D::*)(std::size_t, ParticleIndex<T>))
    {
        static_cast<Derived *>(this)->eval_each(iter, idx);
    }

    template <typename D>
    void eval_range_dispatch(std::size_t iter, const ParticleRange<T> &range,
        void (D::*)(std::size_t, const ParticleRange<T> &))
    {
        static_cast<Derived *>(this)->eval_range(iter, range);
    }

    template <typename D>
    void eval_first_dispatch(std::size_t iter, Particle<T> &particle,
        void (D::*)(std::size_t, Particle<T> &))
    {
        static_cast<Derived *>(this)->eval_first(iter, particle);
    }

    template <typename D>
    void eval_last_dispatch(std::size_t iter, Particle<T> &particle,
        void (D::*)(std::size_t, Particle<T> &))
    {
        static_cast<Derived *>(this)->eval_last(iter, particle);
    }

    // non-static const

    template <typename D>
    void eval_dispatch(std::size_t iter, ParticleIndex<T> idx,
        void (D::*)(std::size_t, ParticleIndex<T>) const)
    {
        static_cast<Derived *>(this)->eval_each(iter, idx);
    }

    template <typename D>
    void eval_range_dispatch(std::size_t iter, const ParticleRange<T> &range,
        void (D::*)(std::size_t, const ParticleRange<T> &) const)
    {
        static_cast<Derived *>(this)->eval_range(iter, range);
    }

    template <typename D>
    void eval_first_dispatch(std::size_t iter, Particle<T> &particle,
        void (D::*)(std::size_t, Particle<T> &) const)
    {
        static_cast<Derived *>(this)->eval_first(iter, particle);
    }

    template <typename D>
    void eval_last_dispatch(std::size_t iter, Particle<T> &particle,
        void (D::*)(std::size_t, Particle<T> &) const)
    {
        static_cast<Derived *>(this)->eval_last(iter, particle);
    }

    // static

    void eval_dispatch(std::size_t iter, ParticleIndex<T> idx,
        void (*)(std::size_t, ParticleIndex<T>))
    {
        Derived::eval_each(iter, idx);
    }

    void eval_range_dispatch(std::size_t iter, const ParticleRange<T> &range,
        void (*)(std::size_t, const ParticleRange<T> &))
    {
        Derived::eval_range(iter, range);
    }

    void eval_first_dispatch(std::size_t iter, Particle<T> &particle,
        void (*)(std::size_t, Particle<T> &))
    {
        Derived::eval_first(iter, particle);
    }

    void eval_last_dispatch(std::size_t iter, Particle<T> &particle,
        void (*)(std::size_t, Particle<T> &))
    {
        Derived::eval_last(iter, particle);
    }

    // base

    void eval_dispatch(std::size_t, ParticleIndex<T>,
        void (SMCSamplerEvalBase::*)(std::size_t, ParticleIndex<T>))
    {
    }

    void eval_range_dispatch(std::size_t iter, const ParticleRange<T> &range,
        void (SMCSamplerEvalBase::*)(std::size_t, const ParticleRange<T> &))
    {
        for (auto idx : range) {
            eval_each(iter, idx);
        }
    }

    void eval_first_dispatch(std::size_t, Particle<T> &,
        void (SMCSamplerEvalBase::*)(std::size_t, Particle<T> &))
    {
    }

    void eval_last_dispatch(std::size_t, Particle<T> &,
        void (SMCSamplerEvalBase::*)(std::size_t, Particle<T> &))
    {
    }
}; // class SMCSamplerEvalBase

/// \brief Mampler evaluation base dispatch class
/// \ingroup SMP
template <typename T>
class SMCSamplerEvalBase<T, Virtual>
{
  protected:
    MCKL_DEFINE_SMP_BACKEND_BASE_SPECIAL_VIRTUAL(SMCSamplerEval)

    virtual void eval_each(std::size_t, ParticleIndex<T>) {}

    virtual void eval_range(std::size_t iter, const ParticleRange<T> &range)
    {
        for (auto idx : range) {
            eval_each(iter, idx);
        }
    }

    virtual void eval_first(std::size_t, Particle<T> &) {}

    virtual void eval_last(std::size_t, Particle<T> &) {}
}; // class SMCSamplerEvalBase<T, Virtual>

/// \brief SMCEstimator evalution base dispatch class
/// \ingroup SMP
template <typename T, typename Derived>
class SMCEstimatorEvalBase
{
  protected:
    MCKL_DEFINE_SMP_BACKEND_BASE_SPECIAL(SMCEstimatorEval)

    void eval_each(
        std::size_t iter, std::size_t dim, ParticleIndex<T> idx, double *r)
    {
        eval_dispatch(iter, dim, idx, r, &Derived::eval_each);
    }

    void eval_range(std::size_t iter, std::size_t dim,
        const ParticleRange<T> &range, double *r)
    {
        eval_range_dispatch(iter, dim, range, r, &Derived::eval_range);
    }

    void eval_first(std::size_t iter, Particle<T> &particle)
    {
        eval_first_dispatch(iter, particle, &Derived::eval_first);
    }

    void eval_last(std::size_t iter, Particle<T> &particle)
    {
        eval_last_dispatch(iter, particle, &Derived::eval_last);
    }

  private:
    // non-static non-const

    template <typename D>
    void eval_dispatch(std::size_t iter, std::size_t dim, ParticleIndex<T> idx,
        double *r,
        void (D::*)(std::size_t, std::size_t, ParticleIndex<T>, double *))
    {
        static_cast<Derived *>(this)->eval_each(iter, dim, idx, r);
    }

    template <typename D>
    void eval_range_dispatch(std::size_t iter, std::size_t dim,
        const ParticleRange<T> &range, double *r,
        void (D::*)(
            std::size_t, std::size_t, const ParticleRange<T> &, double *))
    {
        static_cast<Derived *>(this)->eval_range(iter, dim, range, r);
    }

    template <typename D>
    void eval_first_dispatch(std::size_t iter, Particle<T> &particle,
        void (D::*)(std::size_t, Particle<T> &))
    {
        static_cast<Derived *>(this)->eval_first(iter, particle);
    }

    template <typename D>
    void eval_last_dispatch(std::size_t iter, Particle<T> &particle,
        void (D::*)(std::size_t, Particle<T> &))
    {
        static_cast<Derived *>(this)->eval_last(iter, particle);
    }

    // non-static const

    template <typename D>
    void eval_dispatch(std::size_t iter, std::size_t dim, ParticleIndex<T> idx,
        double *r, void (D::*)(std::size_t, std::size_t, ParticleIndex<T>,
                       double *) const)
    {
        static_cast<Derived *>(this)->eval_each(iter, dim, idx, r);
    }

    template <typename D>
    void eval_range_dispatch(std::size_t iter, std::size_t dim,
        const ParticleRange<T> &range, double *r,
        void (D::*)(std::size_t, std::size_t, const ParticleRange<T> &,
            double *) const)
    {
        static_cast<Derived *>(this)->eval_range(iter, dim, range, r);
    }

    template <typename D>
    void eval_first_dispatch(std::size_t iter, Particle<T> &particle,
        void (D::*)(std::size_t, Particle<T> &) const)
    {
        static_cast<Derived *>(this)->eval_first(iter, particle);
    }

    template <typename D>
    void eval_last_dispatch(std::size_t iter, Particle<T> &particle,
        void (D::*)(std::size_t, Particle<T> &) const)
    {
        static_cast<Derived *>(this)->eval_last(iter, particle);
    }

    // static

    void eval_dispatch(std::size_t iter, std::size_t dim, ParticleIndex<T> idx,
        double *r,
        void (*)(std::size_t, std::size_t, ParticleIndex<T>, double *))
    {
        Derived::eval_each(iter, dim, idx, r);
    }

    void eval_range_dispatch(std::size_t iter, std::size_t dim,
        const ParticleRange<T> &range, double *r,
        void (*)(std::size_t, std::size_t, const ParticleRange<T> &, double *))
    {
        Derived::eval_range(iter, dim, range, r);
    }

    void eval_first_dispatch(std::size_t iter, Particle<T> &particle,
        void (*)(std::size_t, Particle<T> &))
    {
        Derived::eval_first(iter, particle);
    }

    void eval_last_dispatch(std::size_t iter, Particle<T> &particle,
        void (*)(std::size_t, Particle<T> &))
    {
        Derived::eval_last(iter, particle);
    }

    // base

    void eval_dispatch(std::size_t, std::size_t, ParticleIndex<T>, double *,
        void (SMCEstimatorEvalBase::*)(
            std::size_t, std::size_t, ParticleIndex<T>, double *))
    {
    }

    void eval_range_dispatch(std::size_t iter, std::size_t dim,
        const ParticleRange<T> &range, double *r,
        void (SMCEstimatorEvalBase::*)(
            std::size_t, std::size_t, const ParticleRange<T> &, double *))
    {
        for (auto idx : range) {
            eval_each(iter, dim, idx, r);
            r += dim;
        }
    }

    void eval_first_dispatch(std::size_t, Particle<T> &,
        void (SMCEstimatorEvalBase::*)(std::size_t, Particle<T> &))
    {
    }

    void eval_last_dispatch(std::size_t, Particle<T> &,
        void (SMCEstimatorEvalBase::*)(std::size_t, Particle<T> &))
    {
    }
}; // class SMCEstimatorBase

/// \brief SMCEstimator evalution base dispatch class
/// \ingroup SMP
template <typename T>
class SMCEstimatorEvalBase<T, Virtual>
{
  protected:
    MCKL_DEFINE_SMP_BACKEND_BASE_SPECIAL_VIRTUAL(SMCEstimatorEval)

    virtual void eval_each(
        std::size_t, std::size_t, ParticleIndex<T>, double *)
    {
    }

    virtual void eval_range(std::size_t iter, std::size_t dim,
        const ParticleRange<T> &range, double *r)
    {
        for (auto idx : range) {
            eval_each(iter, dim, idx, r);
            r += dim;
        }
    }

    virtual void eval_first(std::size_t, Particle<T> &) {}

    virtual void eval_last(std::size_t, Particle<T> &) {}
}; // class SMCEstimatorEvalBase<T, Virtual>

} // namespace mckl

MCKL_POP_CLANG_WARNING

#endif // MCKL_SMP_BACKEND_BASE_HPP
