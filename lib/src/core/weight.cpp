//============================================================================
// MCKL/lib/src/core/weight.cpp
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
#include <mckl/random/rng.hpp>
#include "libmckl.hpp"

extern "C" {

mckl_weight mckl_weight_new(size_t n) { return {new ::mckl::Weight(n)}; }

void mckl_weight_delete(mckl_weight *weight_ptr)
{
    delete ::mckl::cast(weight_ptr);
    weight_ptr->ptr = nullptr;
}

void mckl_weight_assign(mckl_weight weight, mckl_weight other)
{
    ::mckl::cast(weight) = ::mckl::cast(other);
}

size_t mckl_weight_size(mckl_weight weight)
{
    return ::mckl::cast(weight).size();
}

void mckl_weight_resize(mckl_weight weight, size_t n)
{
    ::mckl::cast(weight).resize(n);
}

void mckl_weight_reserve(mckl_weight weight, size_t n)
{
    ::mckl::cast(weight).reserve(n);
}

void mckl_weight_shrink_to_fit(mckl_weight weight)
{
    ::mckl::cast(weight).shrink_to_fit();
}

double mckl_weight_ess(mckl_weight weight)
{
    return ::mckl::cast(weight).ess();
}

void mckl_weight_read_weight(mckl_weight weight, double *first, int stride)
{
    ::mckl::cast(weight).read_weight(first, stride);
}

const double *mckl_weight_data(mckl_weight weight)
{
    return ::mckl::cast(weight).data();
}

void mckl_weight_set_equal(mckl_weight weight)
{
    ::mckl::cast(weight).set_equal();
}

void mckl_weight_set(mckl_weight weight, const double *first, int stride)
{
    ::mckl::cast(weight).set(first, stride);
}

void mckl_weight_mul(mckl_weight weight, const double *first, int stride)
{
    ::mckl::cast(weight).mul(first, stride);
}

void mckl_weight_set_log(mckl_weight weight, const double *first, int stride)
{
    ::mckl::cast(weight).set_log(first, stride);
}

void mckl_weight_add_log(mckl_weight weight, const double *first, int stride)
{
    ::mckl::cast(weight).add_log(first, stride);
}

#ifdef MCKL_RNG_DEFINE_MACRO
#undef MCKL_RNG_DEFINE_MACRO
#endif

#ifdef MCKL_RNG_DEFINE_MACRO_NA
#undef MCKL_RNG_DEFINE_MACRO_NA
#endif

#define MCKL_RNG_DEFINE_MACRO(RNGType, Name, name)                            \
    inline size_t mckl_weight_draw_##name(mckl_weight weight, mckl_rng rng)   \
    {                                                                         \
        return ::mckl::cast(weight).draw(                                     \
            *reinterpret_cast<RNGType *>(rng.ptr));                           \
    }

#include <mckl/random/internal/rng_define_macro_alias.hpp>

#include <mckl/random/internal/rng_define_macro.hpp>

using mckl_weight_draw_type = size_t (*)(mckl_weight, mckl_rng);

#ifdef MCKL_RNG_DEFINE_MACRO
#undef MCKL_RNG_DEFINE_MACRO
#endif

#ifdef MCKL_RNG_DEFINE_MACRO_NA
#undef MCKL_RNG_DEFINE_MACRO_NA
#endif

#define MCKL_RNG_DEFINE_MACRO(RNGType, Name, name) mckl_weight_draw_##name,
#define MCKL_RNG_DEFINE_MACRO_NA(RNGType, Name, name) nullptr,

static mckl_weight_draw_type mckl_weight_draw_dispatch[] = {

#include <mckl/random/internal/rng_define_macro_alias.hpp>

#include <mckl/random/internal/rng_define_macro.hpp>

    nullptr}; // mckl_weight_draw_dispatch

size_t mckl_weight_draw(mckl_weight weight, mckl_rng rng)
{
    return mckl_weight_draw_dispatch[static_cast<std::size_t>(rng.type)](
        weight, rng);
}

} // extern "C"
