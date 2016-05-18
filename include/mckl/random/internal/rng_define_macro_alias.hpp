//============================================================================
// MCKL/include/random/internal/rng_define_macro_alias.hpp
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

#ifndef MCKL_RNG_DEFINE_MACRO_NA
#define MCKL_RNG_DEFINE_MACRO_NA(RNGType, Name, name)
#endif

MCKL_RNG_DEFINE_MACRO(::mckl::RNG, RNG, rng)
MCKL_RNG_DEFINE_MACRO(::mckl::RNG_64, RNG_64, rng_64)

MCKL_RNG_DEFINE_MACRO(::mckl::RNGMini, RNGMini, rngmini)
MCKL_RNG_DEFINE_MACRO(::mckl::RNGMini_64, RNGMini_64, rngmini_64)

MCKL_RNG_DEFINE_MACRO(::mckl::Philox, Philox, philox)
MCKL_RNG_DEFINE_MACRO(::mckl::Philox_64, Philox_64, philox_64)

MCKL_RNG_DEFINE_MACRO(::mckl::Threefry, Threefry, threefry)
MCKL_RNG_DEFINE_MACRO(::mckl::Threefry_64, Threefry_64, threefry_64)

#if MCKL_HAS_AESNI
#define MCKL_RNG_DEFINE_MACRO_AESNI MCKL_RNG_DEFINE_MACRO
#else
#define MCKL_RNG_DEFINE_MACRO_AESNI MCKL_RNG_DEFINE_MACRO_NA
#endif

MCKL_RNG_DEFINE_MACRO_AESNI(::mckl::AES128, AES128, aes128)
MCKL_RNG_DEFINE_MACRO_AESNI(::mckl::AES128_64, AES128_64, aes128_64)

MCKL_RNG_DEFINE_MACRO_AESNI(::mckl::AES192, AES192, aes192)
MCKL_RNG_DEFINE_MACRO_AESNI(::mckl::AES192_64, AES192_64, aes192_64)

MCKL_RNG_DEFINE_MACRO_AESNI(::mckl::AES256, AES256, aes256)
MCKL_RNG_DEFINE_MACRO_AESNI(::mckl::AES256_64, AES256_64, aes256_64)

MCKL_RNG_DEFINE_MACRO_AESNI(::mckl::ARS, ARS, ars)
MCKL_RNG_DEFINE_MACRO_AESNI(::mckl::ARS_64, ARS_64, ars_64)

#undef MCKL_RNG_DEFINE_MACRO_AESNI
