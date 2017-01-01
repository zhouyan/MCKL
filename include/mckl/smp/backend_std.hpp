//============================================================================
// MCKL/include/mckl/smp/backend_std.hpp
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

#ifndef MCKL_SMP_BACKEND_STD_HPP
#define MCKL_SMP_BACKEND_STD_HPP

#include <mckl/core/iterator.hpp>
#include <mckl/smp/backend_base.hpp>
#include <future>
#include <thread>

namespace mckl
{

class BackendSTD
{
    public:
    BackendSTD(const BackendSTD &) = delete;
    BackendSTD &operator=(const BackendSTD &) = delete;

    static BackendSTD &instance()
    {
        static BackendSTD backend;

        return backend;
    }

    void reset() { np_ = std::thread::hardware_concurrency(); }

    unsigned np() const { return np_; }

    void np(unsigned n) { np_ = n; }

    private:
    unsigned np_;

    BackendSTD() : np_(std::thread::hardware_concurrency()) {}
}; // class BackendSTD

namespace internal
{

template <typename T>
inline mckl::Vector<ParticleRange<T>> backend_std_range(Particle<T> &particle)
{
    const std::size_t N = static_cast<std::size_t>(particle.size());
    const std::size_t n =
        static_cast<std::size_t>(std::max(1U, BackendSTD::instance().np()));
    const std::size_t m = N / n;
    const std::size_t r = N % n;

    ParticleIndex<T> b = particle.begin();
    mckl::Vector<ParticleRange<T>> range;
    range.reserve(n);
    for (std::size_t i = 0; i != n; ++i) {
        const std::size_t l = m + (i < r ? 1 : 0);
        range.emplace_back(b, b + l);
        b += l;
    }

    return range;
}

} // namespace mckl::internal

/// \brief SMCSampler<T>::eval_type subtype using the standard library
/// \ingroup STD
template <typename T, typename Derived>
class SMCSamplerEvalSMP<T, Derived, BackendSTD>
    : public SMCSamplerEvalBase<T, Derived>
{
    public:
    void operator()(std::size_t iter, Particle<T> &particle)
    {
        run(iter, particle);
    }

    protected:
    MCKL_DEFINE_SMP_BACKEND_SPECIAL(STD, SMCSamplerEval)

    void run(std::size_t iter, Particle<T> &particle)
    {
        run(iter, particle, 1);
    }

    template <typename... Args>
    void run(std::size_t iter, Particle<T> &particle, std::size_t, Args &&...)
    {
        this->eval_first(iter, particle);
        mckl::Vector<std::future<void>> task_group;
        for (auto range : internal::backend_std_range(particle)) {
            task_group.push_back(std::async(
                std::launch::async, [this, iter, &particle, range]() {
                    this->eval_range(iter, range);
                }));
        }
        for (auto &task : task_group)
            task.wait();
        this->eval_last(iter, particle);
    }
}; // class SMCSamplerEvalSMP

/// \brief SMCEstimator<T>::eval_type subtype using the standard library
/// \ingroup STD
template <typename T, typename Derived>
class SMCEstimatorEvalSMP<T, Derived, BackendSTD>
    : public SMCEstimatorEvalBase<T, Derived>
{
    public:
    void operator()(
        std::size_t iter, std::size_t dim, Particle<T> &particle, double *r)
    {
        run(iter, dim, particle, r);
    }

    protected:
    MCKL_DEFINE_SMP_BACKEND_SPECIAL(STD, SMCEstimatorEval)

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
        mckl::Vector<std::future<void>> task_group;
        for (auto range : internal::backend_std_range(particle)) {
            task_group.push_back(std::async(
                std::launch::async, [this, iter, dim, &particle, r, range]() {
                    this->eval_range(iter, dim, range,
                        r + static_cast<std::size_t>(range.ibegin()) * dim);
                }));
        }
        for (auto &task : task_group)
            task.wait();
        this->eval_last(iter, particle);
    }
}; // class SMCEstimatorEvalSMP

/// \brief SMCSampler<T>::eval_type subtype using the standard library
/// \ingroup STD
template <typename T, typename Derived>
using SMCSamplerEvalSTD = SMCSamplerEvalSMP<T, Derived, BackendSTD>;

/// \brief SMCEstimator<T>::eval_type subtype using the standard library
/// \ingroup STD
template <typename T, typename Derived>
using SMCEstimatorEvalSTD = SMCEstimatorEvalSMP<T, Derived, BackendSTD>;

} // namespace mckl

#endif // MCKL_SMP_BACKEND_STD_HPP
