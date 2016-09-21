//============================================================================
// MCKL/include/mckl/random/internal/rng_define_macro.hpp
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

#ifndef MCKL_RNG_DEFINE_MACRO_NA
#define MCKL_RNG_DEFINE_MACRO_NA(RNGType, Name, name)
#endif

MCKL_RNG_DEFINE_MACRO(::mckl::Philox2x32, Philox2x32, philox2x32)
MCKL_RNG_DEFINE_MACRO(::mckl::Philox4x32, Philox4x32, philox4x32)
MCKL_RNG_DEFINE_MACRO(::mckl::Philox2x64, Philox2x64, philox2x64)
MCKL_RNG_DEFINE_MACRO(::mckl::Philox4x64, Philox4x64, philox4x64)
MCKL_RNG_DEFINE_MACRO(::mckl::Philox2x32_64, Philox2x32_64, philox2x32_64)
MCKL_RNG_DEFINE_MACRO(::mckl::Philox4x32_64, Philox4x32_64, philox4x32_64)
MCKL_RNG_DEFINE_MACRO(::mckl::Philox2x64_64, Philox2x64_64, philox2x64_64)
MCKL_RNG_DEFINE_MACRO(::mckl::Philox4x64_64, Philox4x64_64, philox4x64_64)

MCKL_RNG_DEFINE_MACRO(::mckl::Threefry2x32, Threefry2x32, threefry2x32)
MCKL_RNG_DEFINE_MACRO(::mckl::Threefry4x32, Threefry4x32, threefry4x32)
MCKL_RNG_DEFINE_MACRO(::mckl::Threefry2x64, Threefry2x64, threefry2x64)
MCKL_RNG_DEFINE_MACRO(::mckl::Threefry4x64, Threefry4x64, threefry4x64)
MCKL_RNG_DEFINE_MACRO(::mckl::Threefry8x64, Threefry8x64, threefry8x64)
MCKL_RNG_DEFINE_MACRO(::mckl::Threefry16x64, Threefry16x64, threefry16x64)
MCKL_RNG_DEFINE_MACRO(::mckl::Threefish256, Threefish256, threefish256)
MCKL_RNG_DEFINE_MACRO(::mckl::Threefish512, Threefish512, threefish512)
MCKL_RNG_DEFINE_MACRO(::mckl::Threefish1024, Threefish1024, threefish1024)
MCKL_RNG_DEFINE_MACRO(
    ::mckl::Threefry2x32_64, Threefry2x32_64, threefry2x32_64)
MCKL_RNG_DEFINE_MACRO(
    ::mckl::Threefry4x32_64, Threefry4x32_64, threefry4x32_64)
MCKL_RNG_DEFINE_MACRO(
    ::mckl::Threefry2x64_64, Threefry2x64_64, threefry2x64_64)
MCKL_RNG_DEFINE_MACRO(
    ::mckl::Threefry4x64_64, Threefry4x64_64, threefry4x64_64)
MCKL_RNG_DEFINE_MACRO(
    ::mckl::Threefry8x64_64, Threefry8x64_64, threefry8x64_64)
MCKL_RNG_DEFINE_MACRO(
    ::mckl::Threefry16x64_64, Threefry16x64_64, threefry16x64_64)
MCKL_RNG_DEFINE_MACRO(
    ::mckl::Threefish256_64, Threefish256_64, threefish256_64)
MCKL_RNG_DEFINE_MACRO(
    ::mckl::Threefish512_64, Threefish512_64, threefish512_64)
MCKL_RNG_DEFINE_MACRO(
    ::mckl::Threefish1024_64, Threefish1024_64, threefish1024_64)

#if MCKL_HAS_AESNI
#define MCKL_RNG_DEFINE_MACRO_AESNI MCKL_RNG_DEFINE_MACRO
#else
#define MCKL_RNG_DEFINE_MACRO_AESNI MCKL_RNG_DEFINE_MACRO_NA
#endif

MCKL_RNG_DEFINE_MACRO_AESNI(::mckl::AES128x1, AES128x1, aes128x1)
MCKL_RNG_DEFINE_MACRO_AESNI(::mckl::AES128x2, AES128x2, aes128x2)
MCKL_RNG_DEFINE_MACRO_AESNI(::mckl::AES128x4, AES128x4, aes128x4)
MCKL_RNG_DEFINE_MACRO_AESNI(::mckl::AES128x8, AES128x8, aes128x8)
MCKL_RNG_DEFINE_MACRO_AESNI(::mckl::AES128x1_64, AES128x1_64, aes128x1_64)
MCKL_RNG_DEFINE_MACRO_AESNI(::mckl::AES128x2_64, AES128x2_64, aes128x2_64)
MCKL_RNG_DEFINE_MACRO_AESNI(::mckl::AES128x4_64, AES128x4_64, aes128x4_64)
MCKL_RNG_DEFINE_MACRO_AESNI(::mckl::AES128x8_64, AES128x8_64, aes128x8_64)

MCKL_RNG_DEFINE_MACRO_AESNI(::mckl::AES192x1, AES192x1, aes192x1)
MCKL_RNG_DEFINE_MACRO_AESNI(::mckl::AES192x2, AES192x2, aes192x2)
MCKL_RNG_DEFINE_MACRO_AESNI(::mckl::AES192x4, AES192x4, aes192x4)
MCKL_RNG_DEFINE_MACRO_AESNI(::mckl::AES192x8, AES192x8, aes192x8)
MCKL_RNG_DEFINE_MACRO_AESNI(::mckl::AES192x1_64, AES192x1_64, aes192x1_64)
MCKL_RNG_DEFINE_MACRO_AESNI(::mckl::AES192x2_64, AES192x2_64, aes192x2_64)
MCKL_RNG_DEFINE_MACRO_AESNI(::mckl::AES192x4_64, AES192x4_64, aes192x4_64)
MCKL_RNG_DEFINE_MACRO_AESNI(::mckl::AES192x8_64, AES192x8_64, aes192x8_64)

MCKL_RNG_DEFINE_MACRO_AESNI(::mckl::AES256x1, AES256x1, aes256x1)
MCKL_RNG_DEFINE_MACRO_AESNI(::mckl::AES256x2, AES256x2, aes256x2)
MCKL_RNG_DEFINE_MACRO_AESNI(::mckl::AES256x4, AES256x4, aes256x4)
MCKL_RNG_DEFINE_MACRO_AESNI(::mckl::AES256x8, AES256x8, aes256x8)
MCKL_RNG_DEFINE_MACRO_AESNI(::mckl::AES256x1_64, AES256x1_64, aes256x1_64)
MCKL_RNG_DEFINE_MACRO_AESNI(::mckl::AES256x2_64, AES256x2_64, aes256x2_64)
MCKL_RNG_DEFINE_MACRO_AESNI(::mckl::AES256x4_64, AES256x4_64, aes256x4_64)
MCKL_RNG_DEFINE_MACRO_AESNI(::mckl::AES256x8_64, AES256x8_64, aes256x8_64)

MCKL_RNG_DEFINE_MACRO_AESNI(::mckl::ARSx1, ARSx1, arsx1)
MCKL_RNG_DEFINE_MACRO_AESNI(::mckl::ARSx2, ARSx2, arsx2)
MCKL_RNG_DEFINE_MACRO_AESNI(::mckl::ARSx4, ARSx4, arsx4)
MCKL_RNG_DEFINE_MACRO_AESNI(::mckl::ARSx8, ARSx8, arsx8)
MCKL_RNG_DEFINE_MACRO_AESNI(::mckl::ARSx1_64, ARSx1_64, arsx1_64)
MCKL_RNG_DEFINE_MACRO_AESNI(::mckl::ARSx2_64, ARSx2_64, arsx2_64)
MCKL_RNG_DEFINE_MACRO_AESNI(::mckl::ARSx4_64, ARSx4_64, arsx4_64)
MCKL_RNG_DEFINE_MACRO_AESNI(::mckl::ARSx8_64, ARSx8_64, arsx8_64)

#undef MCKL_RNG_DEFINE_MACRO_AESNI

#if MCKL_HAS_RDRAND
#define MCKL_RNG_DEFINE_MACRO_RDRAND MCKL_RNG_DEFINE_MACRO
#else
#define MCKL_RNG_DEFINE_MACRO_RDRAND MCKL_RNG_DEFINE_MACRO_NA
#endif

MCKL_RNG_DEFINE_MACRO_RDRAND(::mckl::RDRAND16, RDRAND16, rdrand16)
MCKL_RNG_DEFINE_MACRO_RDRAND(::mckl::RDRAND32, RDRAND32, rdrand32)
MCKL_RNG_DEFINE_MACRO_RDRAND(::mckl::RDRAND64, RDRAND64, rdrand64)

#undef MCKL_RNG_DEFINE_MACRO_RDRAND
