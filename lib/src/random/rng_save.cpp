//============================================================================
// MCKL/lib/src/random/rng_save.cpp
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

#include "libmcklrng.hpp"

extern "C" {

#ifdef MCKL_RNG_DEFINE_MACRO
#undef MCKL_RNG_DEFINE_MACRO
#endif

#ifdef MCKL_RNG_DEFINE_MACRO_NA
#undef MCKL_RNG_DEFINE_MACRO_NA
#endif

#define MCKL_RNG_DEFINE_MACRO(RNGType, Name, name)                            \
    inline size_t mckl_rng_save_##name(mckl_rng rng, void *mem)               \
    {                                                                         \
        std::size_t size = sizeof(RNGType);                                   \
        if (mem != nullptr)                                                   \
            std::memcpy(mem, rng.ptr, size);                                  \
                                                                              \
        return size;                                                          \
    }

#include <mckl/random/internal/rng_define_macro_alias.hpp>

#include <mckl/random/internal/rng_define_macro.hpp>

using mckl_rng_save_type = size_t (*)(mckl_rng, void *);

static mckl_rng_save_type mckl_rng_save_dispatch[] = {

#ifdef MCKL_RNG_DEFINE_MACRO
#undef MCKL_RNG_DEFINE_MACRO
#endif

#ifdef MCKL_RNG_DEFINE_MACRO_NA
#undef MCKL_RNG_DEFINE_MACRO_NA
#endif

#define MCKL_RNG_DEFINE_MACRO(RNGType, Name, name) mckl_rng_save_##name,
#define MCKL_RNG_DEFINE_MACRO_NA(RNGType, Name, name) nullptr,

#include <mckl/random/internal/rng_define_macro_alias.hpp>

#include <mckl/random/internal/rng_define_macro.hpp>

    nullptr}; // mckl_rng_save_dispatch

size_t mckl_rng_save(mckl_rng rng, void *mem)
{
    return mckl_rng_save_dispatch[static_cast<std::size_t>(rng.type)](
        rng, mem);
}

} // extern "C"
