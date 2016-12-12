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

template <typename IntType>
inline void backend_std_range(
    IntType N, mckl::Vector<IntType> &first, mckl::Vector<IntType> &last)
{
    first.clear();
    last.clear();
    const IntType np =
        static_cast<IntType>(std::max(1U, BackendSTD::instance().np()));
    if (np == 1) {
        first.push_back(0);
        last.push_back(N);
        return;
    }

    const IntType m = N / np;
    const IntType r = N % np;
    IntType b = 0;
    for (IntType id = 0; id != np; ++id) {
        const IntType n = m + (id < r ? 1 : 0);
        first.push_back(b);
        last.push_back(b + n);
        b += n;
    }
}

} // namespace mckl::internal

/// \brief Sampler<T>::eval_type subtype using the standard library
/// \ingroup STD
template <typename T, typename Derived>
class SamplerEvalSMP<T, Derived, BackendSTD>
    : public SamplerEvalBase<T, Derived>
{
    public:
    void operator()(std::size_t iter, Particle<T> &particle)
    {
        run(iter, particle);
    }

    protected:
    MCKL_DEFINE_SMP_BACKEND_SPECIAL(STD, SamplerEval)

    void run(std::size_t iter, Particle<T> &particle)
    {
        run(iter, particle, 1);
    }

    template <typename... Args>
    void run(std::size_t iter, Particle<T> &particle, std::size_t, Args &&...)
    {
        using size_type = typename Particle<T>::size_type;

        this->eval_first(iter, particle);
        mckl::Vector<size_type> first;
        mckl::Vector<size_type> last;
        internal::backend_std_range(particle.size(), first, last);
        mckl::Vector<std::future<void>> task_group;
        for (std::size_t i = 0; i != first.size(); ++i) {
            const size_type b = first[i];
            const size_type e = last[i];
            task_group.push_back(std::async(
                std::launch::async, [this, iter, &particle, b, e]() {
                    this->eval_range(iter, particle.range(b, e));
                }));
        }
        for (auto &task : task_group)
            task.wait();
        this->eval_last(iter, particle);
    }
}; // class SamplerEvalSMP

/// \brief Monitor<T>::eval_type subtype using the standard library
/// \ingroup STD
template <typename T, typename Derived>
class MonitorEvalSMP<T, Derived, BackendSTD>
    : public MonitorEvalBase<T, Derived>
{
    public:
    void operator()(
        std::size_t iter, std::size_t dim, Particle<T> &particle, double *r)
    {
        run(iter, dim, particle, r);
    }

    protected:
    MCKL_DEFINE_SMP_BACKEND_SPECIAL(STD, MonitorEval)

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
        mckl::Vector<size_type> first;
        mckl::Vector<size_type> last;
        internal::backend_std_range(particle.size(), first, last);
        mckl::Vector<std::future<void>> task_group;
        for (std::size_t i = 0; i != first.size(); ++i) {
            const size_type b = first[i];
            const size_type e = last[i];
            task_group.push_back(std::async(
                std::launch::async, [this, iter, dim, &particle, r, b, e]() {
                    this->eval_range(iter, dim, particle.range(b, e),
                        r + static_cast<std::size_t>(b) * dim);
                }));
        }
        for (auto &task : task_group)
            task.wait();
        this->eval_last(iter, particle);
    }
}; // class MonitorEvalSMP

/// \brief Sampler<T>::eval_type subtype using the standard library
/// \ingroup STD
template <typename T, typename Derived>
using SamplerEvalSTD = SamplerEvalSMP<T, Derived, BackendSTD>;

/// \brief Monitor<T>::eval_type subtype using the standard library
/// \ingroup STD
template <typename T, typename Derived>
using MonitorEvalSTD = MonitorEvalSMP<T, Derived, BackendSTD>;

} // namespace mckl

#endif // MCKL_SMP_BACKEND_STD_HPP
