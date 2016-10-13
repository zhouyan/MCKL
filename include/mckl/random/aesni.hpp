//============================================================================
// MCKL/include/mckl/random/aesni.hpp
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

#ifndef MCKL_RANDOM_AESNI_HPP
#define MCKL_RANDOM_AESNI_HPP

#include <mckl/random/internal/common.hpp>
#include <mckl/random/counter.hpp>

#ifdef MCKL_MSVC
#include <immintrin.h>
#else
#include <wmmintrin.h>
#endif

#ifdef MCKL_GCC
#if MCKL_GCC_VERSION >= 60000
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wignored-attributes"
#endif
#endif

#define MCKL_DEFINE_RANDOM_AES_KEY_GEN_ASSIST(N, val)                         \
    template <>                                                               \
    inline __m128i AESKeyGenAssist<N>(const __m128i &xmm)                     \
    {                                                                         \
        return _mm_aeskeygenassist_si128(xmm, val);                           \
    }

/// \brief AES128Engine default rounds
/// \ingroup Config
#ifndef MCKL_AES128_ROUNDS
#define MCKL_AES128_ROUNDS 10
#endif

/// \brief AES192Engine default rounds
/// \ingroup Config
#ifndef MCKL_AES192_ROUNDS
#define MCKL_AES192_ROUNDS 12
#endif

/// \brief AES256Engine default rounds
/// \ingroup Config
#ifndef MCKL_AES256_ROUNDS
#define MCKL_AES256_ROUNDS 14
#endif

/// \brief ARSEngine default rounds
/// \ingroup Config
#ifndef MCKL_ARS_ROUNDS
#define MCKL_ARS_ROUNDS 5
#endif

namespace mckl
{

/// \brief Default ARS constants
/// \ingroup AESNI
class ARSConstants
{
    public:
    static constexpr std::uint64_t weyl[2] = {
        0x9E3779B97F4A7C15, 0xBB67AE8584CAA73B};
}; // class ARSConstants

namespace internal
{

template <std::size_t>
inline __m128i AESKeyGenAssist(const __m128i &);

MCKL_DEFINE_RANDOM_AES_KEY_GEN_ASSIST(0x00, 0x8D)
MCKL_DEFINE_RANDOM_AES_KEY_GEN_ASSIST(0x01, 0x01)
MCKL_DEFINE_RANDOM_AES_KEY_GEN_ASSIST(0x02, 0x02)
MCKL_DEFINE_RANDOM_AES_KEY_GEN_ASSIST(0x03, 0x04)
MCKL_DEFINE_RANDOM_AES_KEY_GEN_ASSIST(0x04, 0x08)
MCKL_DEFINE_RANDOM_AES_KEY_GEN_ASSIST(0x05, 0x10)
MCKL_DEFINE_RANDOM_AES_KEY_GEN_ASSIST(0x06, 0x20)
MCKL_DEFINE_RANDOM_AES_KEY_GEN_ASSIST(0x07, 0x40)
MCKL_DEFINE_RANDOM_AES_KEY_GEN_ASSIST(0x08, 0x80)
MCKL_DEFINE_RANDOM_AES_KEY_GEN_ASSIST(0x09, 0x1B)
MCKL_DEFINE_RANDOM_AES_KEY_GEN_ASSIST(0x0A, 0x36)
MCKL_DEFINE_RANDOM_AES_KEY_GEN_ASSIST(0x0B, 0x6C)
MCKL_DEFINE_RANDOM_AES_KEY_GEN_ASSIST(0x0C, 0xD8)
MCKL_DEFINE_RANDOM_AES_KEY_GEN_ASSIST(0x0D, 0xAB)
MCKL_DEFINE_RANDOM_AES_KEY_GEN_ASSIST(0x0E, 0x4D)
MCKL_DEFINE_RANDOM_AES_KEY_GEN_ASSIST(0x0F, 0x9A)
MCKL_DEFINE_RANDOM_AES_KEY_GEN_ASSIST(0x10, 0x2F)
MCKL_DEFINE_RANDOM_AES_KEY_GEN_ASSIST(0x11, 0x5E)
MCKL_DEFINE_RANDOM_AES_KEY_GEN_ASSIST(0x12, 0xBC)
MCKL_DEFINE_RANDOM_AES_KEY_GEN_ASSIST(0x13, 0x63)
MCKL_DEFINE_RANDOM_AES_KEY_GEN_ASSIST(0x14, 0xC6)
MCKL_DEFINE_RANDOM_AES_KEY_GEN_ASSIST(0x15, 0x97)
MCKL_DEFINE_RANDOM_AES_KEY_GEN_ASSIST(0x16, 0x35)
MCKL_DEFINE_RANDOM_AES_KEY_GEN_ASSIST(0x17, 0x6A)
MCKL_DEFINE_RANDOM_AES_KEY_GEN_ASSIST(0x18, 0xD4)
MCKL_DEFINE_RANDOM_AES_KEY_GEN_ASSIST(0x19, 0xB3)
MCKL_DEFINE_RANDOM_AES_KEY_GEN_ASSIST(0x1A, 0x7D)
MCKL_DEFINE_RANDOM_AES_KEY_GEN_ASSIST(0x1B, 0xFA)
MCKL_DEFINE_RANDOM_AES_KEY_GEN_ASSIST(0x1C, 0xEF)
MCKL_DEFINE_RANDOM_AES_KEY_GEN_ASSIST(0x1D, 0xC5)
MCKL_DEFINE_RANDOM_AES_KEY_GEN_ASSIST(0x1E, 0x91)
MCKL_DEFINE_RANDOM_AES_KEY_GEN_ASSIST(0x1F, 0x39)
MCKL_DEFINE_RANDOM_AES_KEY_GEN_ASSIST(0x20, 0x72)
MCKL_DEFINE_RANDOM_AES_KEY_GEN_ASSIST(0x21, 0xE4)
MCKL_DEFINE_RANDOM_AES_KEY_GEN_ASSIST(0x22, 0xD3)
MCKL_DEFINE_RANDOM_AES_KEY_GEN_ASSIST(0x23, 0xBD)
MCKL_DEFINE_RANDOM_AES_KEY_GEN_ASSIST(0x24, 0x61)
MCKL_DEFINE_RANDOM_AES_KEY_GEN_ASSIST(0x25, 0xC2)
MCKL_DEFINE_RANDOM_AES_KEY_GEN_ASSIST(0x26, 0x9F)
MCKL_DEFINE_RANDOM_AES_KEY_GEN_ASSIST(0x27, 0x25)
MCKL_DEFINE_RANDOM_AES_KEY_GEN_ASSIST(0x28, 0x4A)
MCKL_DEFINE_RANDOM_AES_KEY_GEN_ASSIST(0x29, 0x94)
MCKL_DEFINE_RANDOM_AES_KEY_GEN_ASSIST(0x2A, 0x33)
MCKL_DEFINE_RANDOM_AES_KEY_GEN_ASSIST(0x2B, 0x66)
MCKL_DEFINE_RANDOM_AES_KEY_GEN_ASSIST(0x2C, 0xCC)
MCKL_DEFINE_RANDOM_AES_KEY_GEN_ASSIST(0x2D, 0x83)
MCKL_DEFINE_RANDOM_AES_KEY_GEN_ASSIST(0x2E, 0x1D)
MCKL_DEFINE_RANDOM_AES_KEY_GEN_ASSIST(0x2F, 0x3A)
MCKL_DEFINE_RANDOM_AES_KEY_GEN_ASSIST(0x30, 0x74)
MCKL_DEFINE_RANDOM_AES_KEY_GEN_ASSIST(0x31, 0xE8)
MCKL_DEFINE_RANDOM_AES_KEY_GEN_ASSIST(0x32, 0xCB)
MCKL_DEFINE_RANDOM_AES_KEY_GEN_ASSIST(0x33, 0x8D)
MCKL_DEFINE_RANDOM_AES_KEY_GEN_ASSIST(0x34, 0x01)
MCKL_DEFINE_RANDOM_AES_KEY_GEN_ASSIST(0x35, 0x02)
MCKL_DEFINE_RANDOM_AES_KEY_GEN_ASSIST(0x36, 0x04)
MCKL_DEFINE_RANDOM_AES_KEY_GEN_ASSIST(0x37, 0x08)
MCKL_DEFINE_RANDOM_AES_KEY_GEN_ASSIST(0x38, 0x10)
MCKL_DEFINE_RANDOM_AES_KEY_GEN_ASSIST(0x39, 0x20)
MCKL_DEFINE_RANDOM_AES_KEY_GEN_ASSIST(0x3A, 0x40)
MCKL_DEFINE_RANDOM_AES_KEY_GEN_ASSIST(0x3B, 0x80)
MCKL_DEFINE_RANDOM_AES_KEY_GEN_ASSIST(0x3C, 0x1B)
MCKL_DEFINE_RANDOM_AES_KEY_GEN_ASSIST(0x3D, 0x36)
MCKL_DEFINE_RANDOM_AES_KEY_GEN_ASSIST(0x3E, 0x6C)
MCKL_DEFINE_RANDOM_AES_KEY_GEN_ASSIST(0x3F, 0xD8)
MCKL_DEFINE_RANDOM_AES_KEY_GEN_ASSIST(0x40, 0xAB)
MCKL_DEFINE_RANDOM_AES_KEY_GEN_ASSIST(0x41, 0x4D)
MCKL_DEFINE_RANDOM_AES_KEY_GEN_ASSIST(0x42, 0x9A)
MCKL_DEFINE_RANDOM_AES_KEY_GEN_ASSIST(0x43, 0x2F)
MCKL_DEFINE_RANDOM_AES_KEY_GEN_ASSIST(0x44, 0x5E)
MCKL_DEFINE_RANDOM_AES_KEY_GEN_ASSIST(0x45, 0xBC)
MCKL_DEFINE_RANDOM_AES_KEY_GEN_ASSIST(0x46, 0x63)
MCKL_DEFINE_RANDOM_AES_KEY_GEN_ASSIST(0x47, 0xC6)
MCKL_DEFINE_RANDOM_AES_KEY_GEN_ASSIST(0x48, 0x97)
MCKL_DEFINE_RANDOM_AES_KEY_GEN_ASSIST(0x49, 0x35)
MCKL_DEFINE_RANDOM_AES_KEY_GEN_ASSIST(0x4A, 0x6A)
MCKL_DEFINE_RANDOM_AES_KEY_GEN_ASSIST(0x4B, 0xD4)
MCKL_DEFINE_RANDOM_AES_KEY_GEN_ASSIST(0x4C, 0xB3)
MCKL_DEFINE_RANDOM_AES_KEY_GEN_ASSIST(0x4D, 0x7D)
MCKL_DEFINE_RANDOM_AES_KEY_GEN_ASSIST(0x4E, 0xFA)
MCKL_DEFINE_RANDOM_AES_KEY_GEN_ASSIST(0x4F, 0xEF)
MCKL_DEFINE_RANDOM_AES_KEY_GEN_ASSIST(0x50, 0xC5)
MCKL_DEFINE_RANDOM_AES_KEY_GEN_ASSIST(0x51, 0x91)
MCKL_DEFINE_RANDOM_AES_KEY_GEN_ASSIST(0x52, 0x39)
MCKL_DEFINE_RANDOM_AES_KEY_GEN_ASSIST(0x53, 0x72)
MCKL_DEFINE_RANDOM_AES_KEY_GEN_ASSIST(0x54, 0xE4)
MCKL_DEFINE_RANDOM_AES_KEY_GEN_ASSIST(0x55, 0xD3)
MCKL_DEFINE_RANDOM_AES_KEY_GEN_ASSIST(0x56, 0xBD)
MCKL_DEFINE_RANDOM_AES_KEY_GEN_ASSIST(0x57, 0x61)
MCKL_DEFINE_RANDOM_AES_KEY_GEN_ASSIST(0x58, 0xC2)
MCKL_DEFINE_RANDOM_AES_KEY_GEN_ASSIST(0x59, 0x9F)
MCKL_DEFINE_RANDOM_AES_KEY_GEN_ASSIST(0x5A, 0x25)
MCKL_DEFINE_RANDOM_AES_KEY_GEN_ASSIST(0x5B, 0x4A)
MCKL_DEFINE_RANDOM_AES_KEY_GEN_ASSIST(0x5C, 0x94)
MCKL_DEFINE_RANDOM_AES_KEY_GEN_ASSIST(0x5D, 0x33)
MCKL_DEFINE_RANDOM_AES_KEY_GEN_ASSIST(0x5E, 0x66)
MCKL_DEFINE_RANDOM_AES_KEY_GEN_ASSIST(0x5F, 0xCC)
MCKL_DEFINE_RANDOM_AES_KEY_GEN_ASSIST(0x60, 0x83)
MCKL_DEFINE_RANDOM_AES_KEY_GEN_ASSIST(0x61, 0x1D)
MCKL_DEFINE_RANDOM_AES_KEY_GEN_ASSIST(0x62, 0x3A)
MCKL_DEFINE_RANDOM_AES_KEY_GEN_ASSIST(0x63, 0x74)
MCKL_DEFINE_RANDOM_AES_KEY_GEN_ASSIST(0x64, 0xE8)
MCKL_DEFINE_RANDOM_AES_KEY_GEN_ASSIST(0x65, 0xCB)
MCKL_DEFINE_RANDOM_AES_KEY_GEN_ASSIST(0x66, 0x8D)
MCKL_DEFINE_RANDOM_AES_KEY_GEN_ASSIST(0x67, 0x01)
MCKL_DEFINE_RANDOM_AES_KEY_GEN_ASSIST(0x68, 0x02)
MCKL_DEFINE_RANDOM_AES_KEY_GEN_ASSIST(0x69, 0x04)
MCKL_DEFINE_RANDOM_AES_KEY_GEN_ASSIST(0x6A, 0x08)
MCKL_DEFINE_RANDOM_AES_KEY_GEN_ASSIST(0x6B, 0x10)
MCKL_DEFINE_RANDOM_AES_KEY_GEN_ASSIST(0x6C, 0x20)
MCKL_DEFINE_RANDOM_AES_KEY_GEN_ASSIST(0x6D, 0x40)
MCKL_DEFINE_RANDOM_AES_KEY_GEN_ASSIST(0x6E, 0x80)
MCKL_DEFINE_RANDOM_AES_KEY_GEN_ASSIST(0x6F, 0x1B)
MCKL_DEFINE_RANDOM_AES_KEY_GEN_ASSIST(0x70, 0x36)
MCKL_DEFINE_RANDOM_AES_KEY_GEN_ASSIST(0x71, 0x6C)
MCKL_DEFINE_RANDOM_AES_KEY_GEN_ASSIST(0x72, 0xD8)
MCKL_DEFINE_RANDOM_AES_KEY_GEN_ASSIST(0x73, 0xAB)
MCKL_DEFINE_RANDOM_AES_KEY_GEN_ASSIST(0x74, 0x4D)
MCKL_DEFINE_RANDOM_AES_KEY_GEN_ASSIST(0x75, 0x9A)
MCKL_DEFINE_RANDOM_AES_KEY_GEN_ASSIST(0x76, 0x2F)
MCKL_DEFINE_RANDOM_AES_KEY_GEN_ASSIST(0x77, 0x5E)
MCKL_DEFINE_RANDOM_AES_KEY_GEN_ASSIST(0x78, 0xBC)
MCKL_DEFINE_RANDOM_AES_KEY_GEN_ASSIST(0x79, 0x63)
MCKL_DEFINE_RANDOM_AES_KEY_GEN_ASSIST(0x7A, 0xC6)
MCKL_DEFINE_RANDOM_AES_KEY_GEN_ASSIST(0x7B, 0x97)
MCKL_DEFINE_RANDOM_AES_KEY_GEN_ASSIST(0x7C, 0x35)
MCKL_DEFINE_RANDOM_AES_KEY_GEN_ASSIST(0x7D, 0x6A)
MCKL_DEFINE_RANDOM_AES_KEY_GEN_ASSIST(0x7E, 0xD4)
MCKL_DEFINE_RANDOM_AES_KEY_GEN_ASSIST(0x7F, 0xB3)
MCKL_DEFINE_RANDOM_AES_KEY_GEN_ASSIST(0x80, 0x7D)
MCKL_DEFINE_RANDOM_AES_KEY_GEN_ASSIST(0x81, 0xFA)
MCKL_DEFINE_RANDOM_AES_KEY_GEN_ASSIST(0x82, 0xEF)
MCKL_DEFINE_RANDOM_AES_KEY_GEN_ASSIST(0x83, 0xC5)
MCKL_DEFINE_RANDOM_AES_KEY_GEN_ASSIST(0x84, 0x91)
MCKL_DEFINE_RANDOM_AES_KEY_GEN_ASSIST(0x85, 0x39)
MCKL_DEFINE_RANDOM_AES_KEY_GEN_ASSIST(0x86, 0x72)
MCKL_DEFINE_RANDOM_AES_KEY_GEN_ASSIST(0x87, 0xE4)
MCKL_DEFINE_RANDOM_AES_KEY_GEN_ASSIST(0x88, 0xD3)
MCKL_DEFINE_RANDOM_AES_KEY_GEN_ASSIST(0x89, 0xBD)
MCKL_DEFINE_RANDOM_AES_KEY_GEN_ASSIST(0x8A, 0x61)
MCKL_DEFINE_RANDOM_AES_KEY_GEN_ASSIST(0x8B, 0xC2)
MCKL_DEFINE_RANDOM_AES_KEY_GEN_ASSIST(0x8C, 0x9F)
MCKL_DEFINE_RANDOM_AES_KEY_GEN_ASSIST(0x8D, 0x25)
MCKL_DEFINE_RANDOM_AES_KEY_GEN_ASSIST(0x8E, 0x4A)
MCKL_DEFINE_RANDOM_AES_KEY_GEN_ASSIST(0x8F, 0x94)
MCKL_DEFINE_RANDOM_AES_KEY_GEN_ASSIST(0x90, 0x33)
MCKL_DEFINE_RANDOM_AES_KEY_GEN_ASSIST(0x91, 0x66)
MCKL_DEFINE_RANDOM_AES_KEY_GEN_ASSIST(0x92, 0xCC)
MCKL_DEFINE_RANDOM_AES_KEY_GEN_ASSIST(0x93, 0x83)
MCKL_DEFINE_RANDOM_AES_KEY_GEN_ASSIST(0x94, 0x1D)
MCKL_DEFINE_RANDOM_AES_KEY_GEN_ASSIST(0x95, 0x3A)
MCKL_DEFINE_RANDOM_AES_KEY_GEN_ASSIST(0x96, 0x74)
MCKL_DEFINE_RANDOM_AES_KEY_GEN_ASSIST(0x97, 0xE8)
MCKL_DEFINE_RANDOM_AES_KEY_GEN_ASSIST(0x98, 0xCB)
MCKL_DEFINE_RANDOM_AES_KEY_GEN_ASSIST(0x99, 0x8D)
MCKL_DEFINE_RANDOM_AES_KEY_GEN_ASSIST(0x9A, 0x01)
MCKL_DEFINE_RANDOM_AES_KEY_GEN_ASSIST(0x9B, 0x02)
MCKL_DEFINE_RANDOM_AES_KEY_GEN_ASSIST(0x9C, 0x04)
MCKL_DEFINE_RANDOM_AES_KEY_GEN_ASSIST(0x9D, 0x08)
MCKL_DEFINE_RANDOM_AES_KEY_GEN_ASSIST(0x9E, 0x10)
MCKL_DEFINE_RANDOM_AES_KEY_GEN_ASSIST(0x9F, 0x20)
MCKL_DEFINE_RANDOM_AES_KEY_GEN_ASSIST(0xA0, 0x40)
MCKL_DEFINE_RANDOM_AES_KEY_GEN_ASSIST(0xA1, 0x80)
MCKL_DEFINE_RANDOM_AES_KEY_GEN_ASSIST(0xA2, 0x1B)
MCKL_DEFINE_RANDOM_AES_KEY_GEN_ASSIST(0xA3, 0x36)
MCKL_DEFINE_RANDOM_AES_KEY_GEN_ASSIST(0xA4, 0x6C)
MCKL_DEFINE_RANDOM_AES_KEY_GEN_ASSIST(0xA5, 0xD8)
MCKL_DEFINE_RANDOM_AES_KEY_GEN_ASSIST(0xA6, 0xAB)
MCKL_DEFINE_RANDOM_AES_KEY_GEN_ASSIST(0xA7, 0x4D)
MCKL_DEFINE_RANDOM_AES_KEY_GEN_ASSIST(0xA8, 0x9A)
MCKL_DEFINE_RANDOM_AES_KEY_GEN_ASSIST(0xA9, 0x2F)
MCKL_DEFINE_RANDOM_AES_KEY_GEN_ASSIST(0xAA, 0x5E)
MCKL_DEFINE_RANDOM_AES_KEY_GEN_ASSIST(0xAB, 0xBC)
MCKL_DEFINE_RANDOM_AES_KEY_GEN_ASSIST(0xAC, 0x63)
MCKL_DEFINE_RANDOM_AES_KEY_GEN_ASSIST(0xAD, 0xC6)
MCKL_DEFINE_RANDOM_AES_KEY_GEN_ASSIST(0xAE, 0x97)
MCKL_DEFINE_RANDOM_AES_KEY_GEN_ASSIST(0xAF, 0x35)
MCKL_DEFINE_RANDOM_AES_KEY_GEN_ASSIST(0xB0, 0x6A)
MCKL_DEFINE_RANDOM_AES_KEY_GEN_ASSIST(0xB1, 0xD4)
MCKL_DEFINE_RANDOM_AES_KEY_GEN_ASSIST(0xB2, 0xB3)
MCKL_DEFINE_RANDOM_AES_KEY_GEN_ASSIST(0xB3, 0x7D)
MCKL_DEFINE_RANDOM_AES_KEY_GEN_ASSIST(0xB4, 0xFA)
MCKL_DEFINE_RANDOM_AES_KEY_GEN_ASSIST(0xB5, 0xEF)
MCKL_DEFINE_RANDOM_AES_KEY_GEN_ASSIST(0xB6, 0xC5)
MCKL_DEFINE_RANDOM_AES_KEY_GEN_ASSIST(0xB7, 0x91)
MCKL_DEFINE_RANDOM_AES_KEY_GEN_ASSIST(0xB8, 0x39)
MCKL_DEFINE_RANDOM_AES_KEY_GEN_ASSIST(0xB9, 0x72)
MCKL_DEFINE_RANDOM_AES_KEY_GEN_ASSIST(0xBA, 0xE4)
MCKL_DEFINE_RANDOM_AES_KEY_GEN_ASSIST(0xBB, 0xD3)
MCKL_DEFINE_RANDOM_AES_KEY_GEN_ASSIST(0xBC, 0xBD)
MCKL_DEFINE_RANDOM_AES_KEY_GEN_ASSIST(0xBD, 0x61)
MCKL_DEFINE_RANDOM_AES_KEY_GEN_ASSIST(0xBE, 0xC2)
MCKL_DEFINE_RANDOM_AES_KEY_GEN_ASSIST(0xBF, 0x9F)
MCKL_DEFINE_RANDOM_AES_KEY_GEN_ASSIST(0xC0, 0x25)
MCKL_DEFINE_RANDOM_AES_KEY_GEN_ASSIST(0xC1, 0x4A)
MCKL_DEFINE_RANDOM_AES_KEY_GEN_ASSIST(0xC2, 0x94)
MCKL_DEFINE_RANDOM_AES_KEY_GEN_ASSIST(0xC3, 0x33)
MCKL_DEFINE_RANDOM_AES_KEY_GEN_ASSIST(0xC4, 0x66)
MCKL_DEFINE_RANDOM_AES_KEY_GEN_ASSIST(0xC5, 0xCC)
MCKL_DEFINE_RANDOM_AES_KEY_GEN_ASSIST(0xC6, 0x83)
MCKL_DEFINE_RANDOM_AES_KEY_GEN_ASSIST(0xC7, 0x1D)
MCKL_DEFINE_RANDOM_AES_KEY_GEN_ASSIST(0xC8, 0x3A)
MCKL_DEFINE_RANDOM_AES_KEY_GEN_ASSIST(0xC9, 0x74)
MCKL_DEFINE_RANDOM_AES_KEY_GEN_ASSIST(0xCA, 0xE8)
MCKL_DEFINE_RANDOM_AES_KEY_GEN_ASSIST(0xCB, 0xCB)
MCKL_DEFINE_RANDOM_AES_KEY_GEN_ASSIST(0xCC, 0x8D)
MCKL_DEFINE_RANDOM_AES_KEY_GEN_ASSIST(0xCD, 0x01)
MCKL_DEFINE_RANDOM_AES_KEY_GEN_ASSIST(0xCE, 0x02)
MCKL_DEFINE_RANDOM_AES_KEY_GEN_ASSIST(0xCF, 0x04)
MCKL_DEFINE_RANDOM_AES_KEY_GEN_ASSIST(0xD0, 0x08)
MCKL_DEFINE_RANDOM_AES_KEY_GEN_ASSIST(0xD1, 0x10)
MCKL_DEFINE_RANDOM_AES_KEY_GEN_ASSIST(0xD2, 0x20)
MCKL_DEFINE_RANDOM_AES_KEY_GEN_ASSIST(0xD3, 0x40)
MCKL_DEFINE_RANDOM_AES_KEY_GEN_ASSIST(0xD4, 0x80)
MCKL_DEFINE_RANDOM_AES_KEY_GEN_ASSIST(0xD5, 0x1B)
MCKL_DEFINE_RANDOM_AES_KEY_GEN_ASSIST(0xD6, 0x36)
MCKL_DEFINE_RANDOM_AES_KEY_GEN_ASSIST(0xD7, 0x6C)
MCKL_DEFINE_RANDOM_AES_KEY_GEN_ASSIST(0xD8, 0xD8)
MCKL_DEFINE_RANDOM_AES_KEY_GEN_ASSIST(0xD9, 0xAB)
MCKL_DEFINE_RANDOM_AES_KEY_GEN_ASSIST(0xDA, 0x4D)
MCKL_DEFINE_RANDOM_AES_KEY_GEN_ASSIST(0xDB, 0x9A)
MCKL_DEFINE_RANDOM_AES_KEY_GEN_ASSIST(0xDC, 0x2F)
MCKL_DEFINE_RANDOM_AES_KEY_GEN_ASSIST(0xDD, 0x5E)
MCKL_DEFINE_RANDOM_AES_KEY_GEN_ASSIST(0xDE, 0xBC)
MCKL_DEFINE_RANDOM_AES_KEY_GEN_ASSIST(0xDF, 0x63)
MCKL_DEFINE_RANDOM_AES_KEY_GEN_ASSIST(0xE0, 0xC6)
MCKL_DEFINE_RANDOM_AES_KEY_GEN_ASSIST(0xE1, 0x97)
MCKL_DEFINE_RANDOM_AES_KEY_GEN_ASSIST(0xE2, 0x35)
MCKL_DEFINE_RANDOM_AES_KEY_GEN_ASSIST(0xE3, 0x6A)
MCKL_DEFINE_RANDOM_AES_KEY_GEN_ASSIST(0xE4, 0xD4)
MCKL_DEFINE_RANDOM_AES_KEY_GEN_ASSIST(0xE5, 0xB3)
MCKL_DEFINE_RANDOM_AES_KEY_GEN_ASSIST(0xE6, 0x7D)
MCKL_DEFINE_RANDOM_AES_KEY_GEN_ASSIST(0xE7, 0xFA)
MCKL_DEFINE_RANDOM_AES_KEY_GEN_ASSIST(0xE8, 0xEF)
MCKL_DEFINE_RANDOM_AES_KEY_GEN_ASSIST(0xE9, 0xC5)
MCKL_DEFINE_RANDOM_AES_KEY_GEN_ASSIST(0xEA, 0x91)
MCKL_DEFINE_RANDOM_AES_KEY_GEN_ASSIST(0xEB, 0x39)
MCKL_DEFINE_RANDOM_AES_KEY_GEN_ASSIST(0xEC, 0x72)
MCKL_DEFINE_RANDOM_AES_KEY_GEN_ASSIST(0xED, 0xE4)
MCKL_DEFINE_RANDOM_AES_KEY_GEN_ASSIST(0xEE, 0xD3)
MCKL_DEFINE_RANDOM_AES_KEY_GEN_ASSIST(0xEF, 0xBD)
MCKL_DEFINE_RANDOM_AES_KEY_GEN_ASSIST(0xF0, 0x61)
MCKL_DEFINE_RANDOM_AES_KEY_GEN_ASSIST(0xF1, 0xC2)
MCKL_DEFINE_RANDOM_AES_KEY_GEN_ASSIST(0xF2, 0x9F)
MCKL_DEFINE_RANDOM_AES_KEY_GEN_ASSIST(0xF3, 0x25)
MCKL_DEFINE_RANDOM_AES_KEY_GEN_ASSIST(0xF4, 0x4A)
MCKL_DEFINE_RANDOM_AES_KEY_GEN_ASSIST(0xF5, 0x94)
MCKL_DEFINE_RANDOM_AES_KEY_GEN_ASSIST(0xF6, 0x33)
MCKL_DEFINE_RANDOM_AES_KEY_GEN_ASSIST(0xF7, 0x66)
MCKL_DEFINE_RANDOM_AES_KEY_GEN_ASSIST(0xF8, 0xCC)
MCKL_DEFINE_RANDOM_AES_KEY_GEN_ASSIST(0xF9, 0x83)
MCKL_DEFINE_RANDOM_AES_KEY_GEN_ASSIST(0xFA, 0x1D)
MCKL_DEFINE_RANDOM_AES_KEY_GEN_ASSIST(0xFB, 0x3A)
MCKL_DEFINE_RANDOM_AES_KEY_GEN_ASSIST(0xFC, 0x74)
MCKL_DEFINE_RANDOM_AES_KEY_GEN_ASSIST(0xFD, 0xE8)
MCKL_DEFINE_RANDOM_AES_KEY_GEN_ASSIST(0xFE, 0xCB)
MCKL_DEFINE_RANDOM_AES_KEY_GEN_ASSIST(0xFF, 0x8D)

class AES128KeySeqGenerator
{
    public:
    using key_type = std::array<std::uint64_t, 2>;

    template <std::size_t Rp1>
    static key_type key(const std::array<__m128i, Rp1> &rk)
    {
        union {
            key_type key;
            __m128i xmm;
        } buf;

        buf.xmm = std::get<0>(rk);

        return buf.key;
    }

    template <std::size_t Rp1>
    void operator()(const key_type &key, std::array<__m128i, Rp1> &rk)
    {
        xmm1_ = _mm_set_epi64x(static_cast<MCKL_INT64>(std::get<1>(key)),
            static_cast<MCKL_INT64>(std::get<0>(key)));
        std::get<0>(rk) = xmm1_;
        generate_seq<1>(rk, std::integral_constant<bool, 1 < Rp1>());
    }

    private:
    __m128i xmm1_;
    __m128i xmm2_;
    __m128i xmm3_;

    template <std::size_t, std::size_t Rp1>
    void generate_seq(std::array<__m128i, Rp1> &, std::false_type)
    {
    }

    template <std::size_t N, std::size_t Rp1>
    void generate_seq(std::array<__m128i, Rp1> &rk, std::true_type)
    {
        xmm2_ = AESKeyGenAssist<N % 256>(xmm1_);
        expand_key();
        std::get<N>(rk) = xmm1_;
        generate_seq<N + 1>(rk, std::integral_constant<bool, N + 1 < Rp1>());
    }

    void expand_key()
    {
        xmm2_ = _mm_shuffle_epi32(xmm2_, 0xFF); // pshufd xmm2, xmm2, 0xFF
        xmm3_ = _mm_slli_si128(xmm1_, 0x04);    // pslldq xmm3, 0x04
        xmm1_ = _mm_xor_si128(xmm1_, xmm3_);    // pxor   xmm1, xmm3
        xmm3_ = _mm_slli_si128(xmm3_, 0x04);    // pslldq xmm3, 0x04
        xmm1_ = _mm_xor_si128(xmm1_, xmm3_);    // pxor   xmm1, xmm3
        xmm3_ = _mm_slli_si128(xmm3_, 0x04);    // pslldq xmm3, 0x04
        xmm1_ = _mm_xor_si128(xmm1_, xmm3_);    // pxor   xmm1, xmm3
        xmm1_ = _mm_xor_si128(xmm1_, xmm2_);    // pxor   xmm1, xmm2
    }
}; // class AES128KeySeqGenerator

class AES192KeySeqGenerator
{
    public:
    using key_type = std::array<std::uint64_t, 3>;

    template <std::size_t Rp1>
    static key_type key(const std::array<__m128i, Rp1> &rk)
    {
        union {
            std::array<std::uint64_t, 4> key;
            std::array<__m128i, 2> xmm;
        } buf;

        key_type key;
        std::get<0>(buf.xmm) = std::get<0>(rk);
        std::get<1>(buf.xmm) = std::get<1>(rk);
        std::get<0>(key) = std::get<0>(buf.key);
        std::get<1>(key) = std::get<1>(buf.key);
        std::get<2>(key) = std::get<2>(buf.key);

        return key;
    }

    template <std::size_t Rp1>
    void operator()(const key_type &key, std::array<__m128i, Rp1> &rk)
    {
        xmm1_ = _mm_set_epi64x(static_cast<MCKL_INT64>(std::get<1>(key)),
            static_cast<MCKL_INT64>(std::get<0>(key)));
        xmm7_ = _mm_set_epi64x(0, static_cast<MCKL_INT64>(std::get<2>(key)));
        std::get<0>(rk) = xmm1_;
        std::get<1>(rk) = xmm7_;

        xmm3_ = _mm_setzero_si128();
        xmm6_ = _mm_setzero_si128();
        xmm4_ = _mm_shuffle_epi32(xmm7_, 0x4F); // pshufd xmm4, xmm7, 0x4F

        std::array<char, Rp1 * 16 + 16> rk_tmp;
        generate_seq<1, Rp1>(
            rk_tmp.data(), std::integral_constant<bool, 24 < Rp1 * 16>());
        copy_key(
            rk, rk_tmp.data(), std::integral_constant<bool, 24 < Rp1 * 16>());
    }

    private:
    __m128i xmm1_;
    __m128i xmm2_;
    __m128i xmm3_;
    __m128i xmm4_;
    __m128i xmm5_;
    __m128i xmm6_;
    __m128i xmm7_;

    template <std::size_t, std::size_t>
    void generate_seq(char *, std::false_type)
    {
    }

    template <std::size_t N, std::size_t Rp1>
    void generate_seq(char *rk_ptr, std::true_type)
    {
        generate_key<N>(rk_ptr);
        complete_key<N>(
            rk_ptr, std::integral_constant<bool, N * 24 + 16 < Rp1 * 16>());
        generate_seq<N + 1, Rp1>(
            rk_ptr, std::integral_constant<bool, N * 24 + 24 < Rp1 * 16>());
    }

    template <std::size_t N>
    void generate_key(char *rk_ptr)
    {
        // In entry, N * 24 < Rp1 * 16
        // Required Storage: N * 24 + 16;

        xmm2_ = AESKeyGenAssist<N % 256>(xmm4_);
        generate_key_expansion();
        _mm_storeu_si128(reinterpret_cast<__m128i *>(rk_ptr + N * 24), xmm1_);
    }

    template <std::size_t>
    void complete_key(char *, std::false_type)
    {
    }

    template <std::size_t N>
    void complete_key(char *rk_ptr, std::true_type)
    {
        // In entry, N * 24 + 16 < Rp1 * 16
        // Required storage: N * 24 + 32

        complete_key_expansion();
        _mm_storeu_si128(
            reinterpret_cast<__m128i *>(rk_ptr + N * 24 + 16), xmm7_);
    }

    void generate_key_expansion()
    {
        xmm2_ = _mm_shuffle_epi32(xmm2_, 0xFF); // pshufd xmm2, xmm2, 0xFF
        xmm3_ = _mm_castps_si128(               // shufps xmm3, xmm1, 0x10
            _mm_shuffle_ps(
                _mm_castsi128_ps(xmm3_), _mm_castsi128_ps(xmm1_), 0x10));
        xmm1_ = _mm_xor_si128(xmm1_, xmm3_);     // pxor   xmm1, xmm3
        xmm3_ = _mm_castps_si128(_mm_shuffle_ps( // shufps xmm3, xmm1, 0x10
            _mm_castsi128_ps(xmm3_), _mm_castsi128_ps(xmm1_), 0x8C));
        xmm1_ = _mm_xor_si128(xmm1_, xmm3_); // pxor   xmm1, xmm3
        xmm1_ = _mm_xor_si128(xmm1_, xmm2_); // pxor   xmm1, xmm2
    }

    void complete_key_expansion()
    {
        xmm5_ = xmm4_;                           // movdqa xmm5, xmm4
        xmm5_ = _mm_slli_si128(xmm5_, 0x04);     // pslldq xmm5, 0x04
        xmm6_ = _mm_castps_si128(_mm_shuffle_ps( // shufps xmm6, xmm1, 0x10
            _mm_castsi128_ps(xmm6_), _mm_castsi128_ps(xmm1_), 0xF0));
        xmm6_ = _mm_xor_si128(xmm6_, xmm5_);    // pxor   xmm6, xmm5
        xmm4_ = _mm_xor_si128(xmm4_, xmm6_);    // pxor   xmm4, xmm6
        xmm7_ = _mm_shuffle_epi32(xmm4_, 0x0E); // pshufd xmm7, xmm4, 0x0E
    }

    template <std::size_t Rp1>
    void copy_key(std::array<__m128i, Rp1> &, const char *, std::false_type)
    {
    }

    template <std::size_t Rp1>
    void copy_key(
        std::array<__m128i, Rp1> &rk, const char *rk_ptr, std::true_type)
    {
        char *dst = reinterpret_cast<char *>(rk.data());
        std::memcpy(dst + 24, rk_ptr + 24, Rp1 * 16 - 24);
    }
}; // class AES192KeySeqGenerator

class AES256KeySeqGenerator
{
    public:
    using key_type = std::array<std::uint64_t, 4>;

    template <std::size_t Rp1>
    static key_type key(const std::array<__m128i, Rp1> &rk)
    {
        union {
            key_type key;
            std::array<__m128i, 2> xmm;
        } buf;

        std::get<0>(buf.xmm) = std::get<0>(rk);
        std::get<1>(buf.xmm) = std::get<2>(rk);

        return buf.key;
    }

    template <std::size_t Rp1>
    void operator()(const key_type &key, std::array<__m128i, Rp1> &rk)
    {
        xmm1_ = _mm_set_epi64x(static_cast<MCKL_INT64>(std::get<1>(key)),
            static_cast<MCKL_INT64>(std::get<0>(key)));
        xmm3_ = _mm_set_epi64x(static_cast<MCKL_INT64>(std::get<3>(key)),
            static_cast<MCKL_INT64>(std::get<2>(key)));
        std::get<0>(rk) = xmm1_;
        std::get<1>(rk) = xmm3_;
        generate_seq<2>(rk, std::integral_constant<bool, 2 < Rp1>());
    }

    private:
    __m128i xmm1_;
    __m128i xmm2_;
    __m128i xmm3_;
    __m128i xmm4_;

    template <std::size_t, std::size_t Rp1>
    void generate_seq(std::array<__m128i, Rp1> &, std::false_type)
    {
    }

    template <std::size_t N, std::size_t Rp1>
    void generate_seq(std::array<__m128i, Rp1> &rk, std::true_type)
    {
        generate_key<N>(rk, std::integral_constant<bool, N % 2 == 0>());
        generate_seq<N + 1>(rk, std::integral_constant<bool, N + 1 < Rp1>());
    }

    template <std::size_t N, std::size_t Rp1>
    void generate_key(std::array<__m128i, Rp1> &rk, std::true_type)
    {
        xmm2_ = AESKeyGenAssist<(N / 2) % 256>(xmm3_);
        expand_key(std::true_type());
        std::get<N>(rk) = xmm1_;
    }

    template <std::size_t N, std::size_t Rp1>
    void generate_key(std::array<__m128i, Rp1> &rk, std::false_type)
    {
        xmm4_ = _mm_aeskeygenassist_si128(xmm1_, 0);
        expand_key(std::false_type());
        std::get<N>(rk) = xmm3_;
    }

    void expand_key(std::true_type)
    {
        xmm2_ = _mm_shuffle_epi32(xmm2_, 0xFF); // pshufd xmm2, xmm2, 0xFF
        xmm4_ = _mm_slli_si128(xmm1_, 0x04);    // pslldq xmm4, 0x04
        xmm1_ = _mm_xor_si128(xmm1_, xmm4_);    // pxor   xmm1, xmm4
        xmm4_ = _mm_slli_si128(xmm4_, 0x04);    // pslldq xmm4, 0x04
        xmm1_ = _mm_xor_si128(xmm1_, xmm4_);    // pxor   xmm1, xmm4
        xmm4_ = _mm_slli_si128(xmm4_, 0x04);    // pslldq xmm4, 0x04
        xmm1_ = _mm_xor_si128(xmm1_, xmm4_);    // pxor   xmm1, xmm4
        xmm1_ = _mm_xor_si128(xmm1_, xmm2_);    // pxor   xmm1, xmm2
    }

    void expand_key(std::false_type)
    {
        xmm2_ = _mm_shuffle_epi32(xmm4_, 0xAA); // pshufd xmm2, xmm4, 0xAA
        xmm4_ = _mm_slli_si128(xmm3_, 0x04);    // pslldq xmm4, 0x04
        xmm3_ = _mm_xor_si128(xmm3_, xmm4_);    // pxor   xmm3, xmm4
        xmm4_ = _mm_slli_si128(xmm4_, 0x04);    // pslldq xmm4, 0x04
        xmm3_ = _mm_xor_si128(xmm3_, xmm4_);    // pxor   xmm3, xmm4
        xmm4_ = _mm_slli_si128(xmm4_, 0x04);    // pslldq xmm4, 0x04
        xmm3_ = _mm_xor_si128(xmm3_, xmm4_);    // pxor   xmm3, xmm4
        xmm3_ = _mm_xor_si128(xmm3_, xmm2_);    // pxor   xmm1, xmm2
    }
}; // class AES256KeySeqGenerator

template <typename Constants>
class ARSKeySeqGenerator
{
    public:
    using key_type = std::array<std::uint64_t, 4>;

    template <std::size_t Rp1>
    static key_type key(const std::array<__m128i, Rp1> &rk)
    {
        union {
            key_type key;
            __m128i xmm;
        } buf;

        buf.xmm = std::get<0>(rk);

        return buf.key;
    }

    template <std::size_t Rp1>
    void operator()(const key_type &key, std::array<__m128i, Rp1> &rk)
    {
        key_ = _mm_set_epi64x(static_cast<MCKL_INT64>(std::get<1>(key)),
            static_cast<MCKL_INT64>(std::get<0>(key)));
        generate<0>(rk, std::integral_constant<bool, 0 < Rp1>());
    }

    private:
    __m128i key_;

    template <std::size_t, std::size_t Rp1>
    void generate(std::array<__m128i, Rp1> &, std::false_type) const
    {
    }

    template <std::size_t N, std::size_t Rp1>
    void generate(std::array<__m128i, Rp1> &rk, std::true_type) const
    {
        static constexpr MCKL_INT64 w0 =
            static_cast<MCKL_INT64>(Constants::weyl[0] * N);
        static constexpr MCKL_INT64 w1 =
            static_cast<MCKL_INT64>(Constants::weyl[1] * N);

        __m128i w = _mm_set_epi64x(w1, w0);
        std::get<N>(rk) = _mm_add_epi64(key_, w);
        generate<N + 1>(rk, std::integral_constant<bool, N + 1 < Rp1>());
    }
}; // class ARSKeySeqGenerator

template <std::size_t Rounds, typename KeySeqGenerator>
class AESKeySeqImpl
{
    public:
    using key_type = typename KeySeqGenerator::key_type;

    static constexpr std::size_t rounds() { return Rounds; }

    key_type key() const { return KeySeqGenerator::key(rk_); }

    void reset(const key_type &key)
    {
        KeySeqGenerator generator;
        generator(key, rk_);
    }

    const std::array<__m128i, rounds() + 1> get() const { return rk_; }

    friend bool operator==(const AESKeySeqImpl<Rounds, KeySeqGenerator> &seq1,
        const AESKeySeqImpl<Rounds, KeySeqGenerator> &seq2)
    {
        std::array<std::uint64_t, 2 * (rounds() + 1)> ks1;
        std::array<std::uint64_t, 2 * (rounds() + 1)> ks2;
        std::memcpy(ks1.data(), seq1.rk_.data(), 16 * (rounds() + 1));
        std::memcpy(ks2.data(), seq2.rk_.data(), 16 * (rounds() + 1));

        return ks1 == ks2;
    }

    friend bool operator!=(const AESKeySeqImpl<Rounds, KeySeqGenerator> &seq1,
        const AESKeySeqImpl<Rounds, KeySeqGenerator> &seq2)
    {
        return !(seq1 == seq2);
    }

    template <typename CharT, typename Traits>
    friend std::basic_ostream<CharT, Traits> &operator<<(
        std::basic_ostream<CharT, Traits> &os,
        const AESKeySeqImpl<Rounds, KeySeqGenerator> &seq)
    {
        if (!os)
            return os;

        std::array<std::uint64_t, 2 * (rounds() + 1)> ks;
        std::memcpy(ks.data(), seq.rk_.data(), 16 * (rounds() + 1));
        ostream(os, ks);

        return os;
    }

    template <typename CharT, typename Traits>
    friend std::basic_istream<CharT, Traits> &operator>>(
        std::basic_istream<CharT, Traits> &is,
        AESKeySeqImpl<Rounds, KeySeqGenerator> &seq)
    {
        if (!is)
            return is;

        std::array<std::uint64_t, 2 * (rounds() + 1)> ks;
        istream(is, ks);
        if (is)
            std::memcpy(seq.rk_.data(), ks.data(), 16 * (rounds() + 1));

        return is;
    }

    private:
    std::array<__m128i, rounds() + 1> rk_;
}; // class AESKeySeqImpl

template <std::size_t Rounds, typename Constants>
class ARSKeySeqImpl
{
    public:
    using key_type = typename ARSKeySeqGenerator<Constants>::key_type;

    static constexpr std::size_t rounds() { return Rounds; }

    key_type key() const { return key_; }

    void reset(const key_type &key) { key_ = key; }

    std::array<__m128i, rounds() + 1> get() const
    {
        ARSKeySeqGenerator<Constants> generator;
        std::array<__m128i, rounds() + 1> rk;
        generator(key_, rk);

        return rk;
    }

    friend bool operator==(const ARSKeySeqImpl<Rounds, Constants> &seq1,
        const ARSKeySeqImpl<Rounds, Constants> &seq2)
    {
        return seq1.key_ == seq2.key_;
    }

    friend bool operator!=(const ARSKeySeqImpl<Rounds, Constants> &seq1,
        const ARSKeySeqImpl<Rounds, Constants> &seq2)
    {
        return !(seq1 == seq2);
    }

    template <typename CharT, typename Traits>
    friend std::basic_ostream<CharT, Traits> &operator<<(
        std::basic_ostream<CharT, Traits> &os,
        const ARSKeySeqImpl<Rounds, Constants> &seq)
    {
        if (!os)
            return os;

        ostream(os, seq.key_);

        return os;
    }

    template <typename CharT, typename Traits>
    friend std::basic_istream<CharT, Traits> &operator>>(
        std::basic_istream<CharT, Traits> &is,
        ARSKeySeqImpl<Rounds, Constants> &seq)
    {
        if (!is)
            return is;

        key_type k = {{0}};
        istream(is, k);
        if (is)
            seq.key_ = k;

        return is;
    }

    private:
    key_type key_;
}; // class ARSKeySeqImpl

} // namespace mckl::internal

/// \brief AES128Engine key sequence generator
/// \ingroup AESNI
template <std::size_t Rounds = MCKL_AES128_ROUNDS>
using AES128KeySeq =
    internal::AESKeySeqImpl<Rounds, internal::AES128KeySeqGenerator>;

/// \brief AES192Engine key sequence generator
/// \ingroup AESNI
template <std::size_t Rounds = MCKL_AES192_ROUNDS>
using AES192KeySeq =
    internal::AESKeySeqImpl<Rounds, internal::AES192KeySeqGenerator>;

/// \brief AES256Engine key sequence generator
/// \ingroup AESNI
template <std::size_t Rounds = MCKL_AES256_ROUNDS>
using AES256KeySeq =
    internal::AESKeySeqImpl<Rounds, internal::AES256KeySeqGenerator>;

/// \brief Default ARSEngine key sequence generator
/// \ingroup AESNI
///
/// \tparam Rounds Number of rounds of AES encryption
/// \tparam Constants A trait class that defines algorithm constants, see
/// ARSConstants
///
/// \details
/// This generator implement the ARS algorithm in
/// [Random123](http://www.deshawresearch.com/resources_random123.html),
/// developed John K. Salmon, Mark A. Moraes, Ron O. Dror, and David E. Shaw.
template <std::size_t Rounds = MCKL_ARS_ROUNDS,
    typename Constants = ARSConstants>
using ARSKeySeq = internal::ARSKeySeqImpl<Rounds, Constants>;

namespace internal
{

#include <mckl/random/internal/aesni_generic.hpp>
#if MCKL_USE_AVX2
#include <mckl/random/internal/aesni_avx2.hpp>
#endif

} // namespace mckl::internal

/// \brief RNG generator using AES-NI instructions
/// \ingroup AESNI
template <typename KeySeqType>
class AESNIGenerator
{
    static_assert(KeySeqType::rounds() != 0,
        "**AESNIGenerate** used with KeySeqType::rounds() equal to zero");

    public:
    using ctr_type = Counter<std::uint64_t, 2>;
    using key_type = typename KeySeqType::key_type;

    static constexpr std::size_t size() { return sizeof(__m128i); }

    key_type key() const { return key_seq_.key(); }

    void reset(const key_type &key) { key_seq_.reset(key); }

    void enc(const void *plain, void *cipher) const
    {
        __m128i state =
            _mm_loadu_si128(reinterpret_cast<const __m128i *>(plain));
        std::array<__m128i, rounds_ + 1> rk(key_seq_.get());
        internal::AESNIGeneratorImpl<KeySeqType>::eval(state, rk);
        _mm_storeu_si128(reinterpret_cast<__m128i *>(cipher), state);
    }

    template <typename ResultType>
    void operator()(ctr_type &ctr, ResultType *buffer) const
    {
        std::array<__m128i, rounds_ + 1> rk(key_seq_.get());
        generate(ctr, buffer, rk);
    }

    template <typename ResultType>
    void operator()(ctr_type &ctr, std::size_t n, ResultType *buffer) const
    {
        static constexpr std::size_t stride = size() / sizeof(ResultType);
        static constexpr std::size_t blocks =
            internal::AESNIGeneratorImpl<KeySeqType>::blocks();

        alignas(32) union {
            std::array<__m128i, blocks> state;
            std::array<ctr_type, blocks> ctr_block;
        } buf;

        std::array<__m128i, rounds_ + 1> rk(key_seq_.get());

        const std::size_t m = n / blocks;
        const std::size_t l = n % blocks;
        for (std::size_t i = 0; i != m; ++i, buffer += stride * blocks) {
            MCKL_FLATTEN_CALL increment(ctr, buf.ctr_block);
            internal::AESNIGeneratorImpl<KeySeqType>::eval(buf.state, rk);
            std::memcpy(buffer, buf.state.data(), size() * blocks);
        }
        for (std::size_t i = 0; i != l; ++i, buffer += stride)
            generate(ctr, buffer, rk);
    }

    friend bool operator==(const AESNIGenerator<KeySeqType> &gen1,
        const AESNIGenerator<KeySeqType> &gen2)
    {
        return gen1.key_seq_ == gen2.key_seq_;
    }

    friend bool operator!=(const AESNIGenerator<KeySeqType> &gen1,
        const AESNIGenerator<KeySeqType> &gen2)
    {
        return !(gen1 == gen2);
    }

    template <typename CharT, typename Traits>
    friend std::basic_ostream<CharT, Traits> &operator<<(
        std::basic_ostream<CharT, Traits> &os,
        const AESNIGenerator<KeySeqType> &gen)
    {
        if (!os)
            return os;

        os << gen.key_seq_;

        return os;
    }

    template <typename CharT, typename Traits>
    friend std::basic_istream<CharT, Traits> &operator>>(
        std::basic_istream<CharT, Traits> &is, AESNIGenerator<KeySeqType> &gen)
    {
        if (!is)
            return is;

        AESNIGenerator<KeySeqType> gen_tmp;
        is >> std::ws >> gen_tmp.key_seq_;

        if (is)
            gen = std::move(gen_tmp);

        return is;
    }

    private:
    KeySeqType key_seq_;

    static constexpr std::size_t rounds_ = KeySeqType::rounds();

    template <typename ResultType>
    void generate(ctr_type &ctr, ResultType *buffer,
        const std::array<__m128i, rounds_ + 1> &rk) const
    {
        alignas(32) union {
            __m128i state;
            ctr_type ctr;
            std::array<ResultType, size() / sizeof(ResultType)> result;
        } buf;

        MCKL_FLATTEN_CALL increment(ctr);
        buf.ctr = ctr;
        internal::AESNIGeneratorImpl<KeySeqType>::eval(buf.state, rk);
        std::copy(buf.result.begin(), buf.result.end(), buffer);
    }
}; // class AESNIGenerator

/// \brief RNG engine using AES-NI instructions
/// \ingroup AESNI
template <typename ResultType, typename KeySeqType>
using AESNIEngine = CounterEngine<ResultType, AESNIGenerator<KeySeqType>>;

/// \brief AES-128 RNG engine
/// \ingroup AESNI
template <typename ResultType, std::size_t Rounds = MCKL_AES128_ROUNDS>
using AES128Engine = AESNIEngine<ResultType, AES128KeySeq<Rounds>>;

/// \brief AES-192 RNG engine
/// \ingroup AESNI
template <typename ResultType, std::size_t Rounds = MCKL_AES192_ROUNDS>
using AES192Engine = AESNIEngine<ResultType, AES192KeySeq<Rounds>>;

/// \brief AES-256 RNG engine
/// \ingroup AESNI
template <typename ResultType, std::size_t Rounds = MCKL_AES256_ROUNDS>
using AES256Engine = AESNIEngine<ResultType, AES256KeySeq<Rounds>>;

/// \brief ARS RNG engine
/// \ingroup AESNI
template <typename ResultType, std::size_t Rounds = MCKL_ARS_ROUNDS,
    typename Constants = ARSConstants>
using ARSEngine = AESNIEngine<ResultType, ARSKeySeq<Rounds, Constants>>;

/// \brief AES-128 RNG engine with 32-bit integers output
/// \ingroup AESNI
using AES128 = AES128Engine<std::uint32_t>;

/// \brief AES-192 RNG engine with 32-bit integers output
/// \ingroup AESNI
using AES192 = AES192Engine<std::uint32_t>;

/// \brief AES-256 RNG engine with 32-bit integers output
/// \ingroup AESNI
using AES256 = AES256Engine<std::uint32_t>;

/// \brief ARS RNG engine with 32-bit integers output
/// \ingroup AESNI
using ARS = ARSEngine<std::uint32_t>;

/// \brief AES-128 RNG engine with 64-bit integers output
/// \ingroup AESNI
using AES128_64 = AES128Engine<std::uint64_t>;

/// \brief AES-192 RNG engine with 64-bit integers output
/// \ingroup AESNI
using AES192_64 = AES192Engine<std::uint64_t>;

/// \brief AES-256 RNG engine with 64-bit integers output
/// \ingroup AESNI
using AES256_64 = AES256Engine<std::uint64_t>;

/// \brief ARS RNG engine with 64-bit integers output
/// \ingroup AESNI
using ARS_64 = ARSEngine<std::uint64_t>;

} // namespace mckl

#ifdef MCKL_GCC
#if MCKL_GCC_VERSION >= 60000
#pragma GCC diagnostic pop
#endif
#endif

#endif // MCKL_RANDOM_AESNI_HPP
