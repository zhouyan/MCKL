//============================================================================
// MCKL/lib/src/core/particle.cpp
//----------------------------------------------------------------------------
// MCKL: Monte Carlo Kernel Library
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

extern "C" {

mckl_particle mckl_particle_new(size_t n, size_t dim)
{
    return {new ::mckl::ParticleC(n, dim)};
}

void mckl_particle_delete(mckl_particle *particle_ptr)
{
    delete ::mckl::cast(particle_ptr);
    particle_ptr->ptr = nullptr;
}

void mckl_particle_assign(mckl_particle particle, mckl_particle other)
{
    ::mckl::cast(particle) = ::mckl::cast(other);
}

mckl_particle mckl_particle_clone(mckl_particle particle)
{
    mckl_particle clone = mckl_particle_new(0, 0);
    ::mckl::cast(clone) = ::mckl::cast(particle).clone();

    return clone;
}

size_t mckl_particle_size(mckl_particle particle)
{
    return ::mckl::cast(particle).size();
}

void mckl_particle_resize_by_index(
    mckl_particle particle, size_t n, const size_t *index)
{
    ::mckl::cast(particle).resize_by_index(n, index);
}

void mckl_particle_resize_by_resample(
    mckl_particle particle, size_t n, MCKLResampleScheme scheme)
{
    switch (scheme) {
        case MCKLMultinomial:
            ::mckl::cast(particle).resize_by_resample(
                n, ::mckl::ResampleMultinomial());
            break;
        case MCKLResidual:
            ::mckl::cast(particle).resize_by_resample(
                n, ::mckl::ResampleResidual());
            break;
        case MCKLStratified:
            ::mckl::cast(particle).resize_by_resample(
                n, ::mckl::ResampleStratified());
            break;
        case MCKLSystematic:
            ::mckl::cast(particle).resize_by_resample(
                n, ::mckl::ResampleSystematic());
            break;
        case MCKLResidualStratified:
            ::mckl::cast(particle).resize_by_resample(
                n, ::mckl::ResampleResidualStratified());
            break;
        case MCKLResidualSystematic:
            ::mckl::cast(particle).resize_by_resample(
                n, ::mckl::ResampleResidualSystematic());
            break;
    }
}

void mckl_particle_resize_by_uniform(mckl_particle particle, size_t n)
{
    ::mckl::cast(particle).resize_by_uniform(n);
}

mckl_state_matrix mckl_particle_state(mckl_particle particle)
{
    return {&::mckl::cast(particle).state()};
}

mckl_weight mckl_particle_weight(mckl_particle particle)
{
    return {&::mckl::cast(particle).weight()};
}

mckl_rng mckl_particle_rng(mckl_particle particle, size_t id)
{
    return {&::mckl::cast(particle).rng(id),
        ::mckl::rng_type<::mckl::ParticleC::rng_type>()};
}

mckl_particle_index mckl_particle_get_index(mckl_particle particle, size_t id)
{
    return {::mckl::cast(particle).state().row_data(id), id};
}

} // extern "C"
