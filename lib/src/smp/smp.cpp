//============================================================================
// MCKL/lib/src/smp/smp.cpp
//----------------------------------------------------------------------------
//                         MCKL: Monte Carlo Kernel Library
//----------------------------------------------------------------------------
// Copyright (c) 2013-2016, Yan Zhou
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
// CONOMPUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
// SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
// CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
// POSSIBILITY OF SUCH DAMAGE.
//============================================================================

#include <mckl/smp/smp.h>
#include <mckl/smp/smp.hpp>
#include "libmckl.hpp"

namespace mckl
{

template <typename Backend>
class SamplerEvalSMPC
    : public SamplerEvalSMP<StateMatrixC, SamplerEvalSMPC<Backend>, Backend>
{
    public:
    SamplerEvalSMPC(const mckl_sampler_eval_smp_type &work) : work_(work) {}

    void eval_each(std::size_t iter, ParticleIndexC idx)
    {
        if (work_.eval_each == nullptr)
            return;

        mckl_particle_index idx_c = {&idx(0), idx.i()};
        work_.eval_each(iter, idx_c);
    }

    void eval_first(std::size_t iter, ParticleC &particle)
    {
        if (work_.eval_first == nullptr)
            return;

        mckl_particle particle_c = {&particle};
        work_.eval_first(iter, particle_c);
    }

    void eval_last(std::size_t iter, ParticleC &particle)
    {
        if (work_.eval_last == nullptr)
            return;

        mckl_particle particle_c = {&particle};
        work_.eval_last(iter, particle_c);
    }

    private:
    mckl_sampler_eval_smp_type work_;
}; // class SamplerEvalSMPC

template <typename Backend>
class MonitorEvalSMPC
    : public MonitorEvalSMP<StateMatrixC, MonitorEvalSMPC<Backend>, Backend>
{
    public:
    MonitorEvalSMPC(const mckl_monitor_eval_smp_type &work) : work_(work) {}

    void eval_each(std::size_t iter, size_t dim, ParticleIndexC idx, double *r)
    {
        if (work_.eval_each == nullptr)
            return;

        mckl_particle_index idx_c = {&idx(0), idx.i()};

        work_.eval_each(iter, dim, idx_c, r);
    }

    void eval_first(std::size_t iter, ParticleC &particle)
    {
        if (work_.eval_first == nullptr)
            return;

        mckl_particle particle_c = {&particle};
        work_.eval_first(iter, particle_c);
    }

    void eval_last(std::size_t iter, ParticleC &particle)
    {
        if (work_.eval_last == nullptr)
            return;

        mckl_particle particle_c = {&particle};
        work_.eval_last(iter, particle_c);
    }

    private:
    mckl_monitor_eval_smp_type work_;
}; // class MonitorEvalSMPC

} // namespace mckl

#define MCKL_DEFINE_LIB_SMP(Name, name)                                       \
    inline void mckl_sampler_eval_##name(mckl_sampler sampler,                \
        mckl_sampler_eval_smp_type eval, MCKLSamplerStage stage)              \
    {                                                                         \
        ::mckl::cast(sampler).eval(                                           \
            ::mckl::SamplerEvalSMPC<::mckl::Backend##Name>(eval),             \
            static_cast<::mckl::SamplerStage>(stage));                        \
    }                                                                         \
                                                                              \
    inline mckl_monitor mckl_monitor_new_##name(size_t dim,                   \
        mckl_monitor_eval_smp_type eval, int record_only,                     \
        MCKLMonitorStage stage)                                               \
    {                                                                         \
        return {new ::mckl::MonitorC(dim,                                     \
            ::mckl::MonitorEvalSMPC<::mckl::Backend##Name>(eval),             \
            record_only != 0, static_cast<::mckl::MonitorStage>(stage))};     \
    }

extern "C" {

MCKL_DEFINE_LIB_SMP(SEQ, seq)
MCKL_DEFINE_LIB_SMP(STD, std)
#if MCKL_HAS_OMP
MCKL_DEFINE_LIB_SMP(OMP, omp)
#endif
#if MCKL_HAS_TBB
MCKL_DEFINE_LIB_SMP(TBB, tbb)
#endif

static int mckl_backend_smp_table[] = {1, 1,
#if MCKL_HAS_OMP
    1,
#else
    0,
#endif
#if MCKL_HAS_TBB
    1,
#else
    0,
#endif
    0}; // mckl_backend_smp

int mckl_backend_smp_max()
{
    return static_cast<int>(sizeof(mckl_backend_smp_table) / sizeof(int)) - 1;
}

int mckl_backend_smp_check(MCKLBackendSMP backend)
{
    if (static_cast<int>(backend) < 0)
        return 0;

    if (static_cast<int>(backend) > mckl_backend_smp_max())
        return 0;

    return mckl_backend_smp_table[static_cast<std::size_t>(backend)];
}

using mckl_sampler_eval_smp_dispatch_type = void (*)(
    mckl_sampler, mckl_sampler_eval_smp_type, MCKLSamplerStage);

static mckl_sampler_eval_smp_dispatch_type mckl_sampler_eval_smp_dispatch[] = {
    mckl_sampler_eval_seq, mckl_sampler_eval_std,
#if MCKL_HAS_OMP
    mckl_sampler_eval_omp,
#else
    nullptr,
#endif
#if MCKL_HAS_TBB
    mckl_sampler_eval_tbb,
#else
    nullptr,
#endif
    nullptr}; // mckl_sampler_eval_smp_dispatch

void mckl_sampler_eval_smp(MCKLBackendSMP backend, mckl_sampler sampler,
    mckl_sampler_eval_smp_type eval, MCKLSamplerStage stage)
{
    mckl_sampler_eval_smp_dispatch[static_cast<std::size_t>(backend)](
        sampler, eval, stage);
}

using mckl_monitor_new_smp_dispatch_type = mckl_monitor (*)(
    size_t, mckl_monitor_eval_smp_type, int, MCKLMonitorStage);

static mckl_monitor_new_smp_dispatch_type mckl_monitor_new_smp_dispatch[] = {
    mckl_monitor_new_seq, mckl_monitor_new_std,
#if MCKL_HAS_OMP
    mckl_monitor_new_omp,
#else
    nullptr,
#endif
#if MCKL_HAS_TBB
    mckl_monitor_new_tbb,
#else
    nullptr,
#endif
    nullptr}; // mckl_monitor_new_smp_dispatch

mckl_monitor mckl_monitor_new_smp(MCKLBackendSMP backend, size_t dim,
    mckl_monitor_eval_smp_type eval, int record_only, MCKLMonitorStage stage)
{
    return mckl_monitor_new_smp_dispatch[static_cast<std::size_t>(backend)](
        dim, eval, record_only, stage);
}

} // extern "C"
