//============================================================================
// MCKL/lib/src/random/seed.cpp
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
#include <mckl/random/rng.hpp>
#include <mckl/random/seed.hpp>

namespace mckl
{

using SeedC = SeedGenerator<NullType, unsigned>;

} // namespace mckl

extern "C" {

#ifdef MCKL_RNG_DEFINE_MACRO
#undef MCKL_RNG_DEFINE_MACRO
#endif

#ifdef MCKL_RNG_DEFINE_MACRO_NA
#undef MCKL_RNG_DEFINE_MACRO_NA
#endif

#define MCKL_RNG_DEFINE_MACRO(RNGType, Name, name)                            \
    inline void mckl_seed_##name(mckl_rng rng)                                \
    {                                                                         \
        ::mckl::SeedC::instance()(*reinterpret_cast<RNGType *>(rng.ptr));     \
    }

#include <mckl/random/internal/rng_define_macro_alias.hpp>

#include <mckl/random/internal/rng_define_macro.hpp>

using mckl_seed_type = void (*)(mckl_rng);

static mckl_seed_type mckl_seed_dispatch[] = {

#ifdef MCKL_RNG_DEFINE_MACRO
#undef MCKL_RNG_DEFINE_MACRO
#endif

#ifdef MCKL_RNG_DEFINE_MACRO_NA
#undef MCKL_RNG_DEFINE_MACRO_NA
#endif

#define MCKL_RNG_DEFINE_MACRO(RNGType, Name, name) mckl_seed_##name,
#define MCKL_RNG_DEFINE_MACRO_NA(RNGType, Name, name) nullptr,

#include <mckl/random/internal/rng_define_macro_alias.hpp>

#include <mckl/random/internal/rng_define_macro.hpp>

    nullptr}; // mckl_seed_dispatch

void mckl_seed(mckl_rng rng)
{
    mckl_seed_dispatch[static_cast<std::size_t>(rng.type)](rng);
}

unsigned mckl_seed_get(void) { return ::mckl::SeedC::instance().get(); }

void mckl_seed_set(unsigned seed) { ::mckl::SeedC::instance().set(seed); }

void mckl_seed_modulo(unsigned div, unsigned rem)
{
    ::mckl::SeedC::instance().modulo(div, rem);
}

size_t mckl_seed_save(void *mem)
{
    std::size_t size = sizeof(::mckl::SeedC);
    if (mem != nullptr)
        std::memcpy(mem, &::mckl::SeedC::instance(), size);

    return size;
}

void mckl_seed_load(const void *mem)
{
    std::memcpy(&::mckl::SeedC::instance(), mem, sizeof(::mckl::SeedC));
}

void mckl_seed_save_f(const char *filename)
{
    std::ofstream os(filename);
    os << ::mckl::SeedC::instance() << std::endl;
    os.close();
}

void mckl_seed_load_f(const char *filename)
{
    std::ifstream is(filename);
    is >> ::mckl::SeedC::instance();
    is.close();
}

} // extern "C"
