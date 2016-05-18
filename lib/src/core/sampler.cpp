//============================================================================
// MCKL/lib/src/core/sampler.cpp
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
// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
// SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
// CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
// POSSIBILITY OF SUCH DAMAGE.
//============================================================================

#include <mckl/mckl.h>
#include "libmckl.hpp"

namespace mckl
{

inline SamplerC::eval_type cast(mckl_sampler_eval_type fptr)
{
    return [fptr](std::size_t iter, ParticleC &particle) {
        mckl_particle particle_c = {&particle};
        return fptr(iter, particle_c);
    };
}

} // namespace mckl

extern "C" {

mckl_sampler mckl_sampler_new(size_t n, size_t dim)
{
    return {new ::mckl::SamplerC(n, dim)};
}

void mckl_sampler_delete(mckl_sampler *sampler_ptr)
{
    delete ::mckl::cast(sampler_ptr);
    sampler_ptr->ptr = nullptr;
}

void mckl_sampler_assign(mckl_sampler sampler, mckl_sampler other)
{
    ::mckl::cast(sampler) = ::mckl::cast(other);
}

mckl_sampler mckl_sampler_clone(mckl_sampler sampler)
{
    mckl_sampler clone = mckl_sampler_new(0, 0);
    ::mckl::cast(clone) = ::mckl::cast(sampler).clone();

    return clone;
}

size_t mckl_sampler_size(mckl_sampler sampler)
{
    return ::mckl::cast(sampler).size();
}

void mckl_sampler_reserve(mckl_sampler sampler, size_t num)
{
    ::mckl::cast(sampler).reserve(num);
}

size_t mckl_sampler_iter_size(mckl_sampler sampler)
{
    return ::mckl::cast(sampler).iter_size();
}

size_t mckl_sampler_iter_num(mckl_sampler sampler)
{
    return ::mckl::cast(sampler).iter_num();
}

void mckl_sampler_reset(mckl_sampler sampler)
{
    ::mckl::cast(sampler).reset();
}

void mckl_sampler_clear(mckl_sampler sampler)
{
    ::mckl::cast(sampler).clear();
}

void mckl_sampler_eval_clear(mckl_sampler sampler)
{
    ::mckl::cast(sampler).eval_clear();
}

void mckl_sampler_eval(mckl_sampler sampler, mckl_sampler_eval_type new_eval,
    MCKLSamplerStage stage)
{
    ::mckl::cast(sampler).eval(
        ::mckl::cast(new_eval), static_cast<::mckl::SamplerStage>(stage));
}

void mckl_sampler_resample_scheme(
    mckl_sampler sampler, MCKLResampleScheme scheme, double threshold)
{
    ::mckl::cast(sampler).resample_method(
        static_cast<::mckl::ResampleScheme>(scheme), threshold);
}

void mckl_sampler_resample_eval(
    mckl_sampler sampler, mckl_sampler_eval_type res_eval, double threshold)
{
    ::mckl::cast(sampler).resample_method(::mckl::cast(res_eval), threshold);
}

double mckl_sampler_get_threshold(mckl_sampler sampler)
{
    return ::mckl::cast(sampler).resample_threshold();
}

void mckl_sampler_set_threshold(mckl_sampler sampler, double threshold)
{
    ::mckl::cast(sampler).resample_threshold(threshold);
}

double mckl_sampler_resample_threshold_never()
{
    return ::mckl::SamplerC::resample_threshold_never();
}

double mckl_sampler_resample_threshold_always()
{
    return ::mckl::SamplerC::resample_threshold_always();
}

void mckl_sampler_set_monitor(
    mckl_sampler sampler, const char *name, mckl_monitor mon)
{
    ::mckl::cast(sampler).monitor(name, ::mckl::cast(mon));
}

mckl_monitor mckl_sampler_get_monitor(mckl_sampler sampler, const char *name)
{
    mckl_monitor monitor = {&::mckl::cast(sampler).monitor(name)};

    return monitor;
}

void mckl_sampler_monitor_clear(mckl_sampler sampler, const char *name)
{
    ::mckl::cast(sampler).monitor_clear(name);
}

void mckl_sampler_monitor_clear_all(mckl_sampler sampler)
{
    ::mckl::cast(sampler).monitor_clear();
}

void mckl_sampler_initialize(mckl_sampler sampler)
{
    ::mckl::cast(sampler).initialize();
}

void mckl_sampler_iterate(mckl_sampler sampler, size_t num)
{
    ::mckl::cast(sampler).iterate(num);
}

mckl_particle mckl_sampler_particle(mckl_sampler sampler)
{
    mckl_particle particle = {&::mckl::cast(sampler).particle()};

    return particle;
}

size_t mckl_sampler_size_history(mckl_sampler sampler, size_t iter)
{
    return ::mckl::cast(sampler).size_history(iter);
}

void mckl_sampler_read_size_history(mckl_sampler sampler, size_t *first)
{
    ::mckl::cast(sampler).read_size_history(first);
}

double mckl_sampler_ess_history(mckl_sampler sampler, size_t iter)
{
    return ::mckl::cast(sampler).ess_history(static_cast<std::size_t>(iter));
}

void mckl_sampler_read_ess_history(mckl_sampler sampler, double *first)
{
    ::mckl::cast(sampler).read_ess_history(first);
}

int mckl_sampler_resampled_history(mckl_sampler sampler, size_t iter)
{
    return ::mckl::cast(sampler).resampled_history(iter);
}

void mckl_sampler_read_resampled_history(mckl_sampler sampler, int *first)
{
    ::mckl::cast(sampler).read_resampled_history(first);
}

size_t mckl_sampler_print(mckl_sampler sampler, char *buf, char sepchar)
{
    std::stringstream ss;
    ::mckl::cast(sampler).print(ss, sepchar);
    std::string str(ss.str());
    std::size_t size = (str.size() + 1) * sizeof(char);
    if (buf != nullptr)
        std::memcpy(buf, str.c_str(), size);

    return size;
}

void mckl_sampler_print_f(
    mckl_sampler sampler, const char *filename, char sepchar)
{
    std::ofstream os(filename);
    ::mckl::cast(sampler).print(os, sepchar);
    os.close();
}

} // extern "C"
