//============================================================================
// MCKL/include/mckl/rng/engine.hpp
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

#ifndef MCKL_RNG_ENGINE_HPP
#define MCKL_RNG_ENGINE_HPP

#include <mckl/internal/config.h>

/// \brief Default `RNG` type
/// \ingroup Config
#ifndef MCKL_RNG_TYPE
#if MCKL_HAS_AES_NI
#define MCKL_RNG_TYPE ::mckl::ARS
#else
#define MCKL_RNG_TYPE ::mckl::Threefry
#endif
#endif

/// \brief Default `RNG_64` type
#ifndef MCKL_RNG_64_TYPE
#if MCKL_HAS_AES_NI
#define MCKL_RNG_64_TYPE ARS_64
#else
#define MCKL_RNG_64_TYPE Threefry_64
#endif
#endif

/// \brief Default `RNGMini` type
/// \ingroup Config
#ifndef MCKL_RNG_MINI_TYPE
#define MCKL_RNG_MINI_TYPE ::mckl::Philox2x32
#endif

/// \brief Default `RNG_64` type
#ifndef MCKL_RNG_MINI_64_TYPE
#define MCKL_RNG_MINI_64_TYPE ::mckl::Philox2x32_64
#endif

#include <mckl/rng/philox.hpp>
#include <mckl/rng/threefry.hpp>

#if MCKL_HAS_AES_NI
#include <mckl/rng/aes_ni.hpp>
#endif

#if MCKL_HAS_MKL
#include <mckl/rng/mkl.hpp>
#endif

#if MCKL_HAS_RDRAND
#include <mckl/rng/rdrand.hpp>
#endif

namespace mckl
{

/// \brief The default 32-bits RNG
/// \ingroup RNG
using RNG = MCKL_RNG_TYPE;

/// \brief The default 64-bits RNG
/// \ingroup RNG
using RNG_64 = MCKL_RNG_64_TYPE;

/// \brief The 32-bits RNG with smallest state
/// \ingroup RNG
using RNGMini = MCKL_RNG_MINI_TYPE;

/// \brief The 64-bits RNG with smallest state
/// \ingroup RNG
using RNGMini_64 = MCKL_RNG_MINI_64_TYPE;

} // namespace mckl

#endif // MCKL_RNG_ENGINE_HPP
