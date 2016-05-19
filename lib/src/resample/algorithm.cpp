//============================================================================
// MCKL/lib/src/resample/algorithm.cpp
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
// SUBSTITUTE GOODS OR SERVICES{} LOSS OF USE, DATA, OR PROFITS{} OR BUSINESS
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
// CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
// POSSIBILITY OF SUCH DAMAGE.
//============================================================================

#include <mckl/mckl.h>
#include <mckl/resample/algorithm.hpp>
#include "libmcklrng.hpp"

extern "C" {

#define MCKL_DEFINE_LIB_RESAMPLE_DISPATCH(                                    \
    RNGType, Name, name, Scheme, scheme)                                      \
    inline void mckl_resample_##scheme##_##name(size_t n, size_t m,           \
        mckl_rng rng, const double *weight, size_t *replication)              \
    {                                                                         \
        ::mckl::Resample##Scheme resample;                                    \
        resample(n, m, *reinterpret_cast<RNGType *>(rng.ptr), weight,         \
            replication);                                                     \
    }

#ifdef MCKL_RNG_DEFINE_MACRO
#undef MCKL_RNG_DEFINE_MACRO
#endif

#ifdef MCKL_RNG_DEFINE_MACRO_NA
#undef MCKL_RNG_DEFINE_MACRO_NA
#endif

#define MCKL_RNG_DEFINE_MACRO(RNGType, Name, name)                            \
    MCKL_DEFINE_LIB_RESAMPLE_DISPATCH(                                        \
        RNGType, Name, name, Multinomial, multinomial)                        \
    MCKL_DEFINE_LIB_RESAMPLE_DISPATCH(                                        \
        RNGType, Name, name, Stratified, stratified)                          \
    MCKL_DEFINE_LIB_RESAMPLE_DISPATCH(                                        \
        RNGType, Name, name, Systematic, systematic)                          \
    MCKL_DEFINE_LIB_RESAMPLE_DISPATCH(                                        \
        RNGType, Name, name, Residual, residual)                              \
    MCKL_DEFINE_LIB_RESAMPLE_DISPATCH(                                        \
        RNGType, Name, name, ResidualStratified, residual_stratified)         \
    MCKL_DEFINE_LIB_RESAMPLE_DISPATCH(                                        \
        RNGType, Name, name, ResidualSystematic, residual_systematic)

#include <mckl/random/internal/rng_define_macro_alias.hpp>

#include <mckl/random/internal/rng_define_macro.hpp>

using mckl_resample_type = void (*)(
    size_t, size_t, mckl_rng, const double *, size_t *);

#ifdef MCKL_RNG_DEFINE_MACRO_NA
#undef MCKL_RNG_DEFINE_MACRO_NA
#endif

#define MCKL_RNG_DEFINE_MACRO_NA(RNGType, Name, name) nullptr,

#ifdef MCKL_RNG_DEFINE_MACRO
#undef MCKL_RNG_DEFINE_MACRO
#endif

#define MCKL_RNG_DEFINE_MACRO(RNGType, Name, name)                            \
    mckl_resample_multinomial_##name,

static mckl_resample_type mckl_resample_multinomial_dispatch[] = {

#include <mckl/random/internal/rng_define_macro_alias.hpp>

#include <mckl/random/internal/rng_define_macro.hpp>

    nullptr}; // mckl_resample_multinomial_dispatch

#ifdef MCKL_RNG_DEFINE_MACRO
#undef MCKL_RNG_DEFINE_MACRO
#endif

#define MCKL_RNG_DEFINE_MACRO(RNGType, Name, name)                            \
    mckl_resample_residual_##name,

static mckl_resample_type mckl_resample_residual_dispatch[] = {

#include <mckl/random/internal/rng_define_macro_alias.hpp>

#include <mckl/random/internal/rng_define_macro.hpp>

    nullptr}; // mckl_resample_residual_dispatch

#ifdef MCKL_RNG_DEFINE_MACRO
#undef MCKL_RNG_DEFINE_MACRO
#endif

#define MCKL_RNG_DEFINE_MACRO(RNGType, Name, name)                            \
    mckl_resample_stratified_##name,

static mckl_resample_type mckl_resample_stratified_dispatch[] = {

#include <mckl/random/internal/rng_define_macro_alias.hpp>

#include <mckl/random/internal/rng_define_macro.hpp>

    nullptr}; // mckl_resample_stratified_dispatch

#ifdef MCKL_RNG_DEFINE_MACRO
#undef MCKL_RNG_DEFINE_MACRO
#endif

#define MCKL_RNG_DEFINE_MACRO(RNGType, Name, name)                            \
    mckl_resample_systematic_##name,

static mckl_resample_type mckl_resample_systematic_dispatch[] = {

#include <mckl/random/internal/rng_define_macro_alias.hpp>

#include <mckl/random/internal/rng_define_macro.hpp>

    nullptr}; // mckl_resample_systematic_dispatch

#ifdef MCKL_RNG_DEFINE_MACRO
#undef MCKL_RNG_DEFINE_MACRO
#endif

#define MCKL_RNG_DEFINE_MACRO(RNGType, Name, name)                            \
    mckl_resample_residual_stratified_##name,

static mckl_resample_type mckl_resample_residual_stratified_dispatch[] = {

#include <mckl/random/internal/rng_define_macro_alias.hpp>

#include <mckl/random/internal/rng_define_macro.hpp>

    nullptr}; // mckl_resample_residual_stratified_dispatch

#ifdef MCKL_RNG_DEFINE_MACRO
#undef MCKL_RNG_DEFINE_MACRO
#endif

#define MCKL_RNG_DEFINE_MACRO(RNGType, Name, name)                            \
    mckl_resample_residual_systematic_##name,

static mckl_resample_type mckl_resample_residual_systematic_dispatch[] = {

#include <mckl/random/internal/rng_define_macro_alias.hpp>

#include <mckl/random/internal/rng_define_macro.hpp>

    nullptr}; // mckl_resample_residual_systematic_dispatch

#define MCKL_DEFINE_LIB_RESAMPLE(scheme)                                      \
    void mckl_resample_##scheme(size_t n, size_t m, mckl_rng rng,             \
        const double *weight, size_t *replication)                            \
    {                                                                         \
        mckl_resample_##scheme##_dispatch[static_cast<std::size_t>(           \
            rng.type)](n, m, rng, weight, replication);                       \
    }

MCKL_DEFINE_LIB_RESAMPLE(multinomial)
MCKL_DEFINE_LIB_RESAMPLE(stratified)
MCKL_DEFINE_LIB_RESAMPLE(systematic)
MCKL_DEFINE_LIB_RESAMPLE(residual)
MCKL_DEFINE_LIB_RESAMPLE(residual_stratified)
MCKL_DEFINE_LIB_RESAMPLE(residual_systematic)

} // extern "C"
