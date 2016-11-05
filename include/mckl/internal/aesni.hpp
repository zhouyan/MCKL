//============================================================================
// MCKL/include/mckl/internal/aesni.hpp
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

#ifndef MCKL_INTERNAL_AESNI_HPP
#define MCKL_INTERNAL_AESNI_HPP

#include <mckl/internal/config.h>
#include <array>

#ifdef MCKL_GCC
#if MCKL_GCC_VERSION >= 60000
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wignored-attributes"
#endif
#endif

namespace mckl
{

namespace internal
{

MCKL_INLINE inline void aesenc_si128(
    std::array<__m128i, 4> &s, const __m128i &a)
{
    std::get<0>(s) = _mm_aesenc_si128(std::get<0>(s), a);
    std::get<1>(s) = _mm_aesenc_si128(std::get<1>(s), a);
    std::get<2>(s) = _mm_aesenc_si128(std::get<2>(s), a);
    std::get<3>(s) = _mm_aesenc_si128(std::get<3>(s), a);
}

MCKL_INLINE inline void aesdec_si128(
    std::array<__m128i, 4> &s, const __m128i &a)
{
    std::get<0>(s) = _mm_aesdec_si128(std::get<0>(s), a);
    std::get<1>(s) = _mm_aesdec_si128(std::get<1>(s), a);
    std::get<2>(s) = _mm_aesdec_si128(std::get<2>(s), a);
    std::get<3>(s) = _mm_aesdec_si128(std::get<3>(s), a);
}

MCKL_INLINE inline void aesenclast_si128(
    std::array<__m128i, 4> &s, const __m128i &a)
{
    std::get<0>(s) = _mm_aesenclast_si128(std::get<0>(s), a);
    std::get<1>(s) = _mm_aesenclast_si128(std::get<1>(s), a);
    std::get<2>(s) = _mm_aesenclast_si128(std::get<2>(s), a);
    std::get<3>(s) = _mm_aesenclast_si128(std::get<3>(s), a);
}

MCKL_INLINE inline void aesdeclast_si128(
    std::array<__m128i, 4> &s, const __m128i &a)
{
    std::get<0>(s) = _mm_aesdeclast_si128(std::get<0>(s), a);
    std::get<1>(s) = _mm_aesdeclast_si128(std::get<1>(s), a);
    std::get<2>(s) = _mm_aesdeclast_si128(std::get<2>(s), a);
    std::get<3>(s) = _mm_aesdeclast_si128(std::get<3>(s), a);
}

MCKL_INLINE inline void aesenc_si128(
    std::array<__m128i, 8> &s, const __m128i &a)
{
    std::get<0>(s) = _mm_aesenc_si128(std::get<0>(s), a);
    std::get<1>(s) = _mm_aesenc_si128(std::get<1>(s), a);
    std::get<2>(s) = _mm_aesenc_si128(std::get<2>(s), a);
    std::get<3>(s) = _mm_aesenc_si128(std::get<3>(s), a);
    std::get<4>(s) = _mm_aesenc_si128(std::get<4>(s), a);
    std::get<5>(s) = _mm_aesenc_si128(std::get<5>(s), a);
    std::get<6>(s) = _mm_aesenc_si128(std::get<6>(s), a);
    std::get<7>(s) = _mm_aesenc_si128(std::get<7>(s), a);
}

MCKL_INLINE inline void aesdec_si128(
    std::array<__m128i, 8> &s, const __m128i &a)
{
    std::get<0>(s) = _mm_aesdec_si128(std::get<0>(s), a);
    std::get<1>(s) = _mm_aesdec_si128(std::get<1>(s), a);
    std::get<2>(s) = _mm_aesdec_si128(std::get<2>(s), a);
    std::get<3>(s) = _mm_aesdec_si128(std::get<3>(s), a);
    std::get<4>(s) = _mm_aesdec_si128(std::get<4>(s), a);
    std::get<5>(s) = _mm_aesdec_si128(std::get<5>(s), a);
    std::get<6>(s) = _mm_aesdec_si128(std::get<6>(s), a);
    std::get<7>(s) = _mm_aesdec_si128(std::get<7>(s), a);
}

MCKL_INLINE inline void aesenclast_si128(
    std::array<__m128i, 8> &s, const __m128i &a)
{
    std::get<0>(s) = _mm_aesenclast_si128(std::get<0>(s), a);
    std::get<1>(s) = _mm_aesenclast_si128(std::get<1>(s), a);
    std::get<2>(s) = _mm_aesenclast_si128(std::get<2>(s), a);
    std::get<3>(s) = _mm_aesenclast_si128(std::get<3>(s), a);
    std::get<4>(s) = _mm_aesenclast_si128(std::get<4>(s), a);
    std::get<5>(s) = _mm_aesenclast_si128(std::get<5>(s), a);
    std::get<6>(s) = _mm_aesenclast_si128(std::get<6>(s), a);
    std::get<7>(s) = _mm_aesenclast_si128(std::get<7>(s), a);
}

MCKL_INLINE inline void aesdeclast_si128(
    std::array<__m128i, 8> &s, const __m128i &a)
{
    std::get<0>(s) = _mm_aesdeclast_si128(std::get<0>(s), a);
    std::get<1>(s) = _mm_aesdeclast_si128(std::get<1>(s), a);
    std::get<2>(s) = _mm_aesdeclast_si128(std::get<2>(s), a);
    std::get<3>(s) = _mm_aesdeclast_si128(std::get<3>(s), a);
    std::get<4>(s) = _mm_aesdeclast_si128(std::get<4>(s), a);
    std::get<5>(s) = _mm_aesdeclast_si128(std::get<5>(s), a);
    std::get<6>(s) = _mm_aesdeclast_si128(std::get<6>(s), a);
    std::get<7>(s) = _mm_aesdeclast_si128(std::get<7>(s), a);
}

MCKL_INLINE inline void aesenc_si128(
    std::array<__m128i, 16> &s, const __m128i &a)
{
    std::get<0x0>(s) = _mm_aesenc_si128(std::get<0x0>(s), a);
    std::get<0x1>(s) = _mm_aesenc_si128(std::get<0x1>(s), a);
    std::get<0x2>(s) = _mm_aesenc_si128(std::get<0x2>(s), a);
    std::get<0x3>(s) = _mm_aesenc_si128(std::get<0x3>(s), a);
    std::get<0x4>(s) = _mm_aesenc_si128(std::get<0x4>(s), a);
    std::get<0x5>(s) = _mm_aesenc_si128(std::get<0x5>(s), a);
    std::get<0x6>(s) = _mm_aesenc_si128(std::get<0x6>(s), a);
    std::get<0x7>(s) = _mm_aesenc_si128(std::get<0x7>(s), a);
    std::get<0x8>(s) = _mm_aesenc_si128(std::get<0x8>(s), a);
    std::get<0x9>(s) = _mm_aesenc_si128(std::get<0x9>(s), a);
    std::get<0xA>(s) = _mm_aesenc_si128(std::get<0xA>(s), a);
    std::get<0xB>(s) = _mm_aesenc_si128(std::get<0xB>(s), a);
    std::get<0xC>(s) = _mm_aesenc_si128(std::get<0xC>(s), a);
    std::get<0xD>(s) = _mm_aesenc_si128(std::get<0xD>(s), a);
    std::get<0xE>(s) = _mm_aesenc_si128(std::get<0xE>(s), a);
    std::get<0xF>(s) = _mm_aesenc_si128(std::get<0xF>(s), a);
}

MCKL_INLINE inline void aesdec_si128(
    std::array<__m128i, 16> &s, const __m128i &a)
{
    std::get<0x0>(s) = _mm_aesdec_si128(std::get<0x0>(s), a);
    std::get<0x1>(s) = _mm_aesdec_si128(std::get<0x1>(s), a);
    std::get<0x2>(s) = _mm_aesdec_si128(std::get<0x2>(s), a);
    std::get<0x3>(s) = _mm_aesdec_si128(std::get<0x3>(s), a);
    std::get<0x4>(s) = _mm_aesdec_si128(std::get<0x4>(s), a);
    std::get<0x5>(s) = _mm_aesdec_si128(std::get<0x5>(s), a);
    std::get<0x6>(s) = _mm_aesdec_si128(std::get<0x6>(s), a);
    std::get<0x7>(s) = _mm_aesdec_si128(std::get<0x7>(s), a);
    std::get<0x8>(s) = _mm_aesdec_si128(std::get<0x8>(s), a);
    std::get<0x9>(s) = _mm_aesdec_si128(std::get<0x9>(s), a);
    std::get<0xA>(s) = _mm_aesdec_si128(std::get<0xA>(s), a);
    std::get<0xB>(s) = _mm_aesdec_si128(std::get<0xB>(s), a);
    std::get<0xC>(s) = _mm_aesdec_si128(std::get<0xC>(s), a);
    std::get<0xD>(s) = _mm_aesdec_si128(std::get<0xD>(s), a);
    std::get<0xE>(s) = _mm_aesdec_si128(std::get<0xE>(s), a);
    std::get<0xF>(s) = _mm_aesdec_si128(std::get<0xF>(s), a);
}

MCKL_INLINE inline void aesenclast_si128(
    std::array<__m128i, 16> &s, const __m128i &a)
{
    std::get<0x0>(s) = _mm_aesenclast_si128(std::get<0x0>(s), a);
    std::get<0x1>(s) = _mm_aesenclast_si128(std::get<0x1>(s), a);
    std::get<0x2>(s) = _mm_aesenclast_si128(std::get<0x2>(s), a);
    std::get<0x3>(s) = _mm_aesenclast_si128(std::get<0x3>(s), a);
    std::get<0x4>(s) = _mm_aesenclast_si128(std::get<0x4>(s), a);
    std::get<0x5>(s) = _mm_aesenclast_si128(std::get<0x5>(s), a);
    std::get<0x6>(s) = _mm_aesenclast_si128(std::get<0x6>(s), a);
    std::get<0x7>(s) = _mm_aesenclast_si128(std::get<0x7>(s), a);
    std::get<0x8>(s) = _mm_aesenclast_si128(std::get<0x8>(s), a);
    std::get<0x9>(s) = _mm_aesenclast_si128(std::get<0x9>(s), a);
    std::get<0xA>(s) = _mm_aesenclast_si128(std::get<0xA>(s), a);
    std::get<0xB>(s) = _mm_aesenclast_si128(std::get<0xB>(s), a);
    std::get<0xC>(s) = _mm_aesenclast_si128(std::get<0xC>(s), a);
    std::get<0xD>(s) = _mm_aesenclast_si128(std::get<0xD>(s), a);
    std::get<0xE>(s) = _mm_aesenclast_si128(std::get<0xE>(s), a);
    std::get<0xF>(s) = _mm_aesenclast_si128(std::get<0xF>(s), a);
}

MCKL_INLINE inline void aesdeclast_si128(
    std::array<__m128i, 16> &s, const __m128i &a)
{
    std::get<0x0>(s) = _mm_aesdeclast_si128(std::get<0x0>(s), a);
    std::get<0x1>(s) = _mm_aesdeclast_si128(std::get<0x1>(s), a);
    std::get<0x2>(s) = _mm_aesdeclast_si128(std::get<0x2>(s), a);
    std::get<0x3>(s) = _mm_aesdeclast_si128(std::get<0x3>(s), a);
    std::get<0x4>(s) = _mm_aesdeclast_si128(std::get<0x4>(s), a);
    std::get<0x5>(s) = _mm_aesdeclast_si128(std::get<0x5>(s), a);
    std::get<0x6>(s) = _mm_aesdeclast_si128(std::get<0x6>(s), a);
    std::get<0x7>(s) = _mm_aesdeclast_si128(std::get<0x7>(s), a);
    std::get<0x8>(s) = _mm_aesdeclast_si128(std::get<0x8>(s), a);
    std::get<0x9>(s) = _mm_aesdeclast_si128(std::get<0x9>(s), a);
    std::get<0xA>(s) = _mm_aesdeclast_si128(std::get<0xA>(s), a);
    std::get<0xB>(s) = _mm_aesdeclast_si128(std::get<0xB>(s), a);
    std::get<0xC>(s) = _mm_aesdeclast_si128(std::get<0xC>(s), a);
    std::get<0xD>(s) = _mm_aesdeclast_si128(std::get<0xD>(s), a);
    std::get<0xE>(s) = _mm_aesdeclast_si128(std::get<0xE>(s), a);
    std::get<0xF>(s) = _mm_aesdeclast_si128(std::get<0xF>(s), a);
}

template <std::size_t>
inline __m128i aeskeygenassist_si128(const __m128i &);

template <>
inline __m128i aeskeygenassist_si128<0x00>(const __m128i &xmm)
{
    return _mm_aeskeygenassist_si128(xmm, 0x8D);
}

template <>
inline __m128i aeskeygenassist_si128<0x01>(const __m128i &xmm)
{
    return _mm_aeskeygenassist_si128(xmm, 0x01);
}

template <>
inline __m128i aeskeygenassist_si128<0x02>(const __m128i &xmm)
{
    return _mm_aeskeygenassist_si128(xmm, 0x02);
}

template <>
inline __m128i aeskeygenassist_si128<0x03>(const __m128i &xmm)
{
    return _mm_aeskeygenassist_si128(xmm, 0x04);
}

template <>
inline __m128i aeskeygenassist_si128<0x04>(const __m128i &xmm)
{
    return _mm_aeskeygenassist_si128(xmm, 0x08);
}

template <>
inline __m128i aeskeygenassist_si128<0x05>(const __m128i &xmm)
{
    return _mm_aeskeygenassist_si128(xmm, 0x10);
}

template <>
inline __m128i aeskeygenassist_si128<0x06>(const __m128i &xmm)
{
    return _mm_aeskeygenassist_si128(xmm, 0x20);
}

template <>
inline __m128i aeskeygenassist_si128<0x07>(const __m128i &xmm)
{
    return _mm_aeskeygenassist_si128(xmm, 0x40);
}

template <>
inline __m128i aeskeygenassist_si128<0x08>(const __m128i &xmm)
{
    return _mm_aeskeygenassist_si128(xmm, 0x80);
}

template <>
inline __m128i aeskeygenassist_si128<0x09>(const __m128i &xmm)
{
    return _mm_aeskeygenassist_si128(xmm, 0x1B);
}

template <>
inline __m128i aeskeygenassist_si128<0x0A>(const __m128i &xmm)
{
    return _mm_aeskeygenassist_si128(xmm, 0x36);
}

template <>
inline __m128i aeskeygenassist_si128<0x0B>(const __m128i &xmm)
{
    return _mm_aeskeygenassist_si128(xmm, 0x6C);
}

template <>
inline __m128i aeskeygenassist_si128<0x0C>(const __m128i &xmm)
{
    return _mm_aeskeygenassist_si128(xmm, 0xD8);
}

template <>
inline __m128i aeskeygenassist_si128<0x0D>(const __m128i &xmm)
{
    return _mm_aeskeygenassist_si128(xmm, 0xAB);
}

template <>
inline __m128i aeskeygenassist_si128<0x0E>(const __m128i &xmm)
{
    return _mm_aeskeygenassist_si128(xmm, 0x4D);
}

template <>
inline __m128i aeskeygenassist_si128<0x0F>(const __m128i &xmm)
{
    return _mm_aeskeygenassist_si128(xmm, 0x9A);
}

template <>
inline __m128i aeskeygenassist_si128<0x10>(const __m128i &xmm)
{
    return _mm_aeskeygenassist_si128(xmm, 0x2F);
}

template <>
inline __m128i aeskeygenassist_si128<0x11>(const __m128i &xmm)
{
    return _mm_aeskeygenassist_si128(xmm, 0x5E);
}

template <>
inline __m128i aeskeygenassist_si128<0x12>(const __m128i &xmm)
{
    return _mm_aeskeygenassist_si128(xmm, 0xBC);
}

template <>
inline __m128i aeskeygenassist_si128<0x13>(const __m128i &xmm)
{
    return _mm_aeskeygenassist_si128(xmm, 0x63);
}

template <>
inline __m128i aeskeygenassist_si128<0x14>(const __m128i &xmm)
{
    return _mm_aeskeygenassist_si128(xmm, 0xC6);
}

template <>
inline __m128i aeskeygenassist_si128<0x15>(const __m128i &xmm)
{
    return _mm_aeskeygenassist_si128(xmm, 0x97);
}

template <>
inline __m128i aeskeygenassist_si128<0x16>(const __m128i &xmm)
{
    return _mm_aeskeygenassist_si128(xmm, 0x35);
}

template <>
inline __m128i aeskeygenassist_si128<0x17>(const __m128i &xmm)
{
    return _mm_aeskeygenassist_si128(xmm, 0x6A);
}

template <>
inline __m128i aeskeygenassist_si128<0x18>(const __m128i &xmm)
{
    return _mm_aeskeygenassist_si128(xmm, 0xD4);
}

template <>
inline __m128i aeskeygenassist_si128<0x19>(const __m128i &xmm)
{
    return _mm_aeskeygenassist_si128(xmm, 0xB3);
}

template <>
inline __m128i aeskeygenassist_si128<0x1A>(const __m128i &xmm)
{
    return _mm_aeskeygenassist_si128(xmm, 0x7D);
}

template <>
inline __m128i aeskeygenassist_si128<0x1B>(const __m128i &xmm)
{
    return _mm_aeskeygenassist_si128(xmm, 0xFA);
}

template <>
inline __m128i aeskeygenassist_si128<0x1C>(const __m128i &xmm)
{
    return _mm_aeskeygenassist_si128(xmm, 0xEF);
}

template <>
inline __m128i aeskeygenassist_si128<0x1D>(const __m128i &xmm)
{
    return _mm_aeskeygenassist_si128(xmm, 0xC5);
}

template <>
inline __m128i aeskeygenassist_si128<0x1E>(const __m128i &xmm)
{
    return _mm_aeskeygenassist_si128(xmm, 0x91);
}

template <>
inline __m128i aeskeygenassist_si128<0x1F>(const __m128i &xmm)
{
    return _mm_aeskeygenassist_si128(xmm, 0x39);
}

template <>
inline __m128i aeskeygenassist_si128<0x20>(const __m128i &xmm)
{
    return _mm_aeskeygenassist_si128(xmm, 0x72);
}

template <>
inline __m128i aeskeygenassist_si128<0x21>(const __m128i &xmm)
{
    return _mm_aeskeygenassist_si128(xmm, 0xE4);
}

template <>
inline __m128i aeskeygenassist_si128<0x22>(const __m128i &xmm)
{
    return _mm_aeskeygenassist_si128(xmm, 0xD3);
}

template <>
inline __m128i aeskeygenassist_si128<0x23>(const __m128i &xmm)
{
    return _mm_aeskeygenassist_si128(xmm, 0xBD);
}

template <>
inline __m128i aeskeygenassist_si128<0x24>(const __m128i &xmm)
{
    return _mm_aeskeygenassist_si128(xmm, 0x61);
}

template <>
inline __m128i aeskeygenassist_si128<0x25>(const __m128i &xmm)
{
    return _mm_aeskeygenassist_si128(xmm, 0xC2);
}

template <>
inline __m128i aeskeygenassist_si128<0x26>(const __m128i &xmm)
{
    return _mm_aeskeygenassist_si128(xmm, 0x9F);
}

template <>
inline __m128i aeskeygenassist_si128<0x27>(const __m128i &xmm)
{
    return _mm_aeskeygenassist_si128(xmm, 0x25);
}

template <>
inline __m128i aeskeygenassist_si128<0x28>(const __m128i &xmm)
{
    return _mm_aeskeygenassist_si128(xmm, 0x4A);
}

template <>
inline __m128i aeskeygenassist_si128<0x29>(const __m128i &xmm)
{
    return _mm_aeskeygenassist_si128(xmm, 0x94);
}

template <>
inline __m128i aeskeygenassist_si128<0x2A>(const __m128i &xmm)
{
    return _mm_aeskeygenassist_si128(xmm, 0x33);
}

template <>
inline __m128i aeskeygenassist_si128<0x2B>(const __m128i &xmm)
{
    return _mm_aeskeygenassist_si128(xmm, 0x66);
}

template <>
inline __m128i aeskeygenassist_si128<0x2C>(const __m128i &xmm)
{
    return _mm_aeskeygenassist_si128(xmm, 0xCC);
}

template <>
inline __m128i aeskeygenassist_si128<0x2D>(const __m128i &xmm)
{
    return _mm_aeskeygenassist_si128(xmm, 0x83);
}

template <>
inline __m128i aeskeygenassist_si128<0x2E>(const __m128i &xmm)
{
    return _mm_aeskeygenassist_si128(xmm, 0x1D);
}

template <>
inline __m128i aeskeygenassist_si128<0x2F>(const __m128i &xmm)
{
    return _mm_aeskeygenassist_si128(xmm, 0x3A);
}

template <>
inline __m128i aeskeygenassist_si128<0x30>(const __m128i &xmm)
{
    return _mm_aeskeygenassist_si128(xmm, 0x74);
}

template <>
inline __m128i aeskeygenassist_si128<0x31>(const __m128i &xmm)
{
    return _mm_aeskeygenassist_si128(xmm, 0xE8);
}

template <>
inline __m128i aeskeygenassist_si128<0x32>(const __m128i &xmm)
{
    return _mm_aeskeygenassist_si128(xmm, 0xCB);
}

template <>
inline __m128i aeskeygenassist_si128<0x33>(const __m128i &xmm)
{
    return _mm_aeskeygenassist_si128(xmm, 0x8D);
}

template <>
inline __m128i aeskeygenassist_si128<0x34>(const __m128i &xmm)
{
    return _mm_aeskeygenassist_si128(xmm, 0x01);
}

template <>
inline __m128i aeskeygenassist_si128<0x35>(const __m128i &xmm)
{
    return _mm_aeskeygenassist_si128(xmm, 0x02);
}

template <>
inline __m128i aeskeygenassist_si128<0x36>(const __m128i &xmm)
{
    return _mm_aeskeygenassist_si128(xmm, 0x04);
}

template <>
inline __m128i aeskeygenassist_si128<0x37>(const __m128i &xmm)
{
    return _mm_aeskeygenassist_si128(xmm, 0x08);
}

template <>
inline __m128i aeskeygenassist_si128<0x38>(const __m128i &xmm)
{
    return _mm_aeskeygenassist_si128(xmm, 0x10);
}

template <>
inline __m128i aeskeygenassist_si128<0x39>(const __m128i &xmm)
{
    return _mm_aeskeygenassist_si128(xmm, 0x20);
}

template <>
inline __m128i aeskeygenassist_si128<0x3A>(const __m128i &xmm)
{
    return _mm_aeskeygenassist_si128(xmm, 0x40);
}

template <>
inline __m128i aeskeygenassist_si128<0x3B>(const __m128i &xmm)
{
    return _mm_aeskeygenassist_si128(xmm, 0x80);
}

template <>
inline __m128i aeskeygenassist_si128<0x3C>(const __m128i &xmm)
{
    return _mm_aeskeygenassist_si128(xmm, 0x1B);
}

template <>
inline __m128i aeskeygenassist_si128<0x3D>(const __m128i &xmm)
{
    return _mm_aeskeygenassist_si128(xmm, 0x36);
}

template <>
inline __m128i aeskeygenassist_si128<0x3E>(const __m128i &xmm)
{
    return _mm_aeskeygenassist_si128(xmm, 0x6C);
}

template <>
inline __m128i aeskeygenassist_si128<0x3F>(const __m128i &xmm)
{
    return _mm_aeskeygenassist_si128(xmm, 0xD8);
}

template <>
inline __m128i aeskeygenassist_si128<0x40>(const __m128i &xmm)
{
    return _mm_aeskeygenassist_si128(xmm, 0xAB);
}

template <>
inline __m128i aeskeygenassist_si128<0x41>(const __m128i &xmm)
{
    return _mm_aeskeygenassist_si128(xmm, 0x4D);
}

template <>
inline __m128i aeskeygenassist_si128<0x42>(const __m128i &xmm)
{
    return _mm_aeskeygenassist_si128(xmm, 0x9A);
}

template <>
inline __m128i aeskeygenassist_si128<0x43>(const __m128i &xmm)
{
    return _mm_aeskeygenassist_si128(xmm, 0x2F);
}

template <>
inline __m128i aeskeygenassist_si128<0x44>(const __m128i &xmm)
{
    return _mm_aeskeygenassist_si128(xmm, 0x5E);
}

template <>
inline __m128i aeskeygenassist_si128<0x45>(const __m128i &xmm)
{
    return _mm_aeskeygenassist_si128(xmm, 0xBC);
}

template <>
inline __m128i aeskeygenassist_si128<0x46>(const __m128i &xmm)
{
    return _mm_aeskeygenassist_si128(xmm, 0x63);
}

template <>
inline __m128i aeskeygenassist_si128<0x47>(const __m128i &xmm)
{
    return _mm_aeskeygenassist_si128(xmm, 0xC6);
}

template <>
inline __m128i aeskeygenassist_si128<0x48>(const __m128i &xmm)
{
    return _mm_aeskeygenassist_si128(xmm, 0x97);
}

template <>
inline __m128i aeskeygenassist_si128<0x49>(const __m128i &xmm)
{
    return _mm_aeskeygenassist_si128(xmm, 0x35);
}

template <>
inline __m128i aeskeygenassist_si128<0x4A>(const __m128i &xmm)
{
    return _mm_aeskeygenassist_si128(xmm, 0x6A);
}

template <>
inline __m128i aeskeygenassist_si128<0x4B>(const __m128i &xmm)
{
    return _mm_aeskeygenassist_si128(xmm, 0xD4);
}

template <>
inline __m128i aeskeygenassist_si128<0x4C>(const __m128i &xmm)
{
    return _mm_aeskeygenassist_si128(xmm, 0xB3);
}

template <>
inline __m128i aeskeygenassist_si128<0x4D>(const __m128i &xmm)
{
    return _mm_aeskeygenassist_si128(xmm, 0x7D);
}

template <>
inline __m128i aeskeygenassist_si128<0x4E>(const __m128i &xmm)
{
    return _mm_aeskeygenassist_si128(xmm, 0xFA);
}

template <>
inline __m128i aeskeygenassist_si128<0x4F>(const __m128i &xmm)
{
    return _mm_aeskeygenassist_si128(xmm, 0xEF);
}

template <>
inline __m128i aeskeygenassist_si128<0x50>(const __m128i &xmm)
{
    return _mm_aeskeygenassist_si128(xmm, 0xC5);
}

template <>
inline __m128i aeskeygenassist_si128<0x51>(const __m128i &xmm)
{
    return _mm_aeskeygenassist_si128(xmm, 0x91);
}

template <>
inline __m128i aeskeygenassist_si128<0x52>(const __m128i &xmm)
{
    return _mm_aeskeygenassist_si128(xmm, 0x39);
}

template <>
inline __m128i aeskeygenassist_si128<0x53>(const __m128i &xmm)
{
    return _mm_aeskeygenassist_si128(xmm, 0x72);
}

template <>
inline __m128i aeskeygenassist_si128<0x54>(const __m128i &xmm)
{
    return _mm_aeskeygenassist_si128(xmm, 0xE4);
}

template <>
inline __m128i aeskeygenassist_si128<0x55>(const __m128i &xmm)
{
    return _mm_aeskeygenassist_si128(xmm, 0xD3);
}

template <>
inline __m128i aeskeygenassist_si128<0x56>(const __m128i &xmm)
{
    return _mm_aeskeygenassist_si128(xmm, 0xBD);
}

template <>
inline __m128i aeskeygenassist_si128<0x57>(const __m128i &xmm)
{
    return _mm_aeskeygenassist_si128(xmm, 0x61);
}

template <>
inline __m128i aeskeygenassist_si128<0x58>(const __m128i &xmm)
{
    return _mm_aeskeygenassist_si128(xmm, 0xC2);
}

template <>
inline __m128i aeskeygenassist_si128<0x59>(const __m128i &xmm)
{
    return _mm_aeskeygenassist_si128(xmm, 0x9F);
}

template <>
inline __m128i aeskeygenassist_si128<0x5A>(const __m128i &xmm)
{
    return _mm_aeskeygenassist_si128(xmm, 0x25);
}

template <>
inline __m128i aeskeygenassist_si128<0x5B>(const __m128i &xmm)
{
    return _mm_aeskeygenassist_si128(xmm, 0x4A);
}

template <>
inline __m128i aeskeygenassist_si128<0x5C>(const __m128i &xmm)
{
    return _mm_aeskeygenassist_si128(xmm, 0x94);
}

template <>
inline __m128i aeskeygenassist_si128<0x5D>(const __m128i &xmm)
{
    return _mm_aeskeygenassist_si128(xmm, 0x33);
}

template <>
inline __m128i aeskeygenassist_si128<0x5E>(const __m128i &xmm)
{
    return _mm_aeskeygenassist_si128(xmm, 0x66);
}

template <>
inline __m128i aeskeygenassist_si128<0x5F>(const __m128i &xmm)
{
    return _mm_aeskeygenassist_si128(xmm, 0xCC);
}

template <>
inline __m128i aeskeygenassist_si128<0x60>(const __m128i &xmm)
{
    return _mm_aeskeygenassist_si128(xmm, 0x83);
}

template <>
inline __m128i aeskeygenassist_si128<0x61>(const __m128i &xmm)
{
    return _mm_aeskeygenassist_si128(xmm, 0x1D);
}

template <>
inline __m128i aeskeygenassist_si128<0x62>(const __m128i &xmm)
{
    return _mm_aeskeygenassist_si128(xmm, 0x3A);
}

template <>
inline __m128i aeskeygenassist_si128<0x63>(const __m128i &xmm)
{
    return _mm_aeskeygenassist_si128(xmm, 0x74);
}

template <>
inline __m128i aeskeygenassist_si128<0x64>(const __m128i &xmm)
{
    return _mm_aeskeygenassist_si128(xmm, 0xE8);
}

template <>
inline __m128i aeskeygenassist_si128<0x65>(const __m128i &xmm)
{
    return _mm_aeskeygenassist_si128(xmm, 0xCB);
}

template <>
inline __m128i aeskeygenassist_si128<0x66>(const __m128i &xmm)
{
    return _mm_aeskeygenassist_si128(xmm, 0x8D);
}

template <>
inline __m128i aeskeygenassist_si128<0x67>(const __m128i &xmm)
{
    return _mm_aeskeygenassist_si128(xmm, 0x01);
}

template <>
inline __m128i aeskeygenassist_si128<0x68>(const __m128i &xmm)
{
    return _mm_aeskeygenassist_si128(xmm, 0x02);
}

template <>
inline __m128i aeskeygenassist_si128<0x69>(const __m128i &xmm)
{
    return _mm_aeskeygenassist_si128(xmm, 0x04);
}

template <>
inline __m128i aeskeygenassist_si128<0x6A>(const __m128i &xmm)
{
    return _mm_aeskeygenassist_si128(xmm, 0x08);
}

template <>
inline __m128i aeskeygenassist_si128<0x6B>(const __m128i &xmm)
{
    return _mm_aeskeygenassist_si128(xmm, 0x10);
}

template <>
inline __m128i aeskeygenassist_si128<0x6C>(const __m128i &xmm)
{
    return _mm_aeskeygenassist_si128(xmm, 0x20);
}

template <>
inline __m128i aeskeygenassist_si128<0x6D>(const __m128i &xmm)
{
    return _mm_aeskeygenassist_si128(xmm, 0x40);
}

template <>
inline __m128i aeskeygenassist_si128<0x6E>(const __m128i &xmm)
{
    return _mm_aeskeygenassist_si128(xmm, 0x80);
}

template <>
inline __m128i aeskeygenassist_si128<0x6F>(const __m128i &xmm)
{
    return _mm_aeskeygenassist_si128(xmm, 0x1B);
}

template <>
inline __m128i aeskeygenassist_si128<0x70>(const __m128i &xmm)
{
    return _mm_aeskeygenassist_si128(xmm, 0x36);
}

template <>
inline __m128i aeskeygenassist_si128<0x71>(const __m128i &xmm)
{
    return _mm_aeskeygenassist_si128(xmm, 0x6C);
}

template <>
inline __m128i aeskeygenassist_si128<0x72>(const __m128i &xmm)
{
    return _mm_aeskeygenassist_si128(xmm, 0xD8);
}

template <>
inline __m128i aeskeygenassist_si128<0x73>(const __m128i &xmm)
{
    return _mm_aeskeygenassist_si128(xmm, 0xAB);
}

template <>
inline __m128i aeskeygenassist_si128<0x74>(const __m128i &xmm)
{
    return _mm_aeskeygenassist_si128(xmm, 0x4D);
}

template <>
inline __m128i aeskeygenassist_si128<0x75>(const __m128i &xmm)
{
    return _mm_aeskeygenassist_si128(xmm, 0x9A);
}

template <>
inline __m128i aeskeygenassist_si128<0x76>(const __m128i &xmm)
{
    return _mm_aeskeygenassist_si128(xmm, 0x2F);
}

template <>
inline __m128i aeskeygenassist_si128<0x77>(const __m128i &xmm)
{
    return _mm_aeskeygenassist_si128(xmm, 0x5E);
}

template <>
inline __m128i aeskeygenassist_si128<0x78>(const __m128i &xmm)
{
    return _mm_aeskeygenassist_si128(xmm, 0xBC);
}

template <>
inline __m128i aeskeygenassist_si128<0x79>(const __m128i &xmm)
{
    return _mm_aeskeygenassist_si128(xmm, 0x63);
}

template <>
inline __m128i aeskeygenassist_si128<0x7A>(const __m128i &xmm)
{
    return _mm_aeskeygenassist_si128(xmm, 0xC6);
}

template <>
inline __m128i aeskeygenassist_si128<0x7B>(const __m128i &xmm)
{
    return _mm_aeskeygenassist_si128(xmm, 0x97);
}

template <>
inline __m128i aeskeygenassist_si128<0x7C>(const __m128i &xmm)
{
    return _mm_aeskeygenassist_si128(xmm, 0x35);
}

template <>
inline __m128i aeskeygenassist_si128<0x7D>(const __m128i &xmm)
{
    return _mm_aeskeygenassist_si128(xmm, 0x6A);
}

template <>
inline __m128i aeskeygenassist_si128<0x7E>(const __m128i &xmm)
{
    return _mm_aeskeygenassist_si128(xmm, 0xD4);
}

template <>
inline __m128i aeskeygenassist_si128<0x7F>(const __m128i &xmm)
{
    return _mm_aeskeygenassist_si128(xmm, 0xB3);
}

template <>
inline __m128i aeskeygenassist_si128<0x80>(const __m128i &xmm)
{
    return _mm_aeskeygenassist_si128(xmm, 0x7D);
}

template <>
inline __m128i aeskeygenassist_si128<0x81>(const __m128i &xmm)
{
    return _mm_aeskeygenassist_si128(xmm, 0xFA);
}

template <>
inline __m128i aeskeygenassist_si128<0x82>(const __m128i &xmm)
{
    return _mm_aeskeygenassist_si128(xmm, 0xEF);
}

template <>
inline __m128i aeskeygenassist_si128<0x83>(const __m128i &xmm)
{
    return _mm_aeskeygenassist_si128(xmm, 0xC5);
}

template <>
inline __m128i aeskeygenassist_si128<0x84>(const __m128i &xmm)
{
    return _mm_aeskeygenassist_si128(xmm, 0x91);
}

template <>
inline __m128i aeskeygenassist_si128<0x85>(const __m128i &xmm)
{
    return _mm_aeskeygenassist_si128(xmm, 0x39);
}

template <>
inline __m128i aeskeygenassist_si128<0x86>(const __m128i &xmm)
{
    return _mm_aeskeygenassist_si128(xmm, 0x72);
}

template <>
inline __m128i aeskeygenassist_si128<0x87>(const __m128i &xmm)
{
    return _mm_aeskeygenassist_si128(xmm, 0xE4);
}

template <>
inline __m128i aeskeygenassist_si128<0x88>(const __m128i &xmm)
{
    return _mm_aeskeygenassist_si128(xmm, 0xD3);
}

template <>
inline __m128i aeskeygenassist_si128<0x89>(const __m128i &xmm)
{
    return _mm_aeskeygenassist_si128(xmm, 0xBD);
}

template <>
inline __m128i aeskeygenassist_si128<0x8A>(const __m128i &xmm)
{
    return _mm_aeskeygenassist_si128(xmm, 0x61);
}

template <>
inline __m128i aeskeygenassist_si128<0x8B>(const __m128i &xmm)
{
    return _mm_aeskeygenassist_si128(xmm, 0xC2);
}

template <>
inline __m128i aeskeygenassist_si128<0x8C>(const __m128i &xmm)
{
    return _mm_aeskeygenassist_si128(xmm, 0x9F);
}

template <>
inline __m128i aeskeygenassist_si128<0x8D>(const __m128i &xmm)
{
    return _mm_aeskeygenassist_si128(xmm, 0x25);
}

template <>
inline __m128i aeskeygenassist_si128<0x8E>(const __m128i &xmm)
{
    return _mm_aeskeygenassist_si128(xmm, 0x4A);
}

template <>
inline __m128i aeskeygenassist_si128<0x8F>(const __m128i &xmm)
{
    return _mm_aeskeygenassist_si128(xmm, 0x94);
}

template <>
inline __m128i aeskeygenassist_si128<0x90>(const __m128i &xmm)
{
    return _mm_aeskeygenassist_si128(xmm, 0x33);
}

template <>
inline __m128i aeskeygenassist_si128<0x91>(const __m128i &xmm)
{
    return _mm_aeskeygenassist_si128(xmm, 0x66);
}

template <>
inline __m128i aeskeygenassist_si128<0x92>(const __m128i &xmm)
{
    return _mm_aeskeygenassist_si128(xmm, 0xCC);
}

template <>
inline __m128i aeskeygenassist_si128<0x93>(const __m128i &xmm)
{
    return _mm_aeskeygenassist_si128(xmm, 0x83);
}

template <>
inline __m128i aeskeygenassist_si128<0x94>(const __m128i &xmm)
{
    return _mm_aeskeygenassist_si128(xmm, 0x1D);
}

template <>
inline __m128i aeskeygenassist_si128<0x95>(const __m128i &xmm)
{
    return _mm_aeskeygenassist_si128(xmm, 0x3A);
}

template <>
inline __m128i aeskeygenassist_si128<0x96>(const __m128i &xmm)
{
    return _mm_aeskeygenassist_si128(xmm, 0x74);
}

template <>
inline __m128i aeskeygenassist_si128<0x97>(const __m128i &xmm)
{
    return _mm_aeskeygenassist_si128(xmm, 0xE8);
}

template <>
inline __m128i aeskeygenassist_si128<0x98>(const __m128i &xmm)
{
    return _mm_aeskeygenassist_si128(xmm, 0xCB);
}

template <>
inline __m128i aeskeygenassist_si128<0x99>(const __m128i &xmm)
{
    return _mm_aeskeygenassist_si128(xmm, 0x8D);
}

template <>
inline __m128i aeskeygenassist_si128<0x9A>(const __m128i &xmm)
{
    return _mm_aeskeygenassist_si128(xmm, 0x01);
}

template <>
inline __m128i aeskeygenassist_si128<0x9B>(const __m128i &xmm)
{
    return _mm_aeskeygenassist_si128(xmm, 0x02);
}

template <>
inline __m128i aeskeygenassist_si128<0x9C>(const __m128i &xmm)
{
    return _mm_aeskeygenassist_si128(xmm, 0x04);
}

template <>
inline __m128i aeskeygenassist_si128<0x9D>(const __m128i &xmm)
{
    return _mm_aeskeygenassist_si128(xmm, 0x08);
}

template <>
inline __m128i aeskeygenassist_si128<0x9E>(const __m128i &xmm)
{
    return _mm_aeskeygenassist_si128(xmm, 0x10);
}

template <>
inline __m128i aeskeygenassist_si128<0x9F>(const __m128i &xmm)
{
    return _mm_aeskeygenassist_si128(xmm, 0x20);
}

template <>
inline __m128i aeskeygenassist_si128<0xA0>(const __m128i &xmm)
{
    return _mm_aeskeygenassist_si128(xmm, 0x40);
}

template <>
inline __m128i aeskeygenassist_si128<0xA1>(const __m128i &xmm)
{
    return _mm_aeskeygenassist_si128(xmm, 0x80);
}

template <>
inline __m128i aeskeygenassist_si128<0xA2>(const __m128i &xmm)
{
    return _mm_aeskeygenassist_si128(xmm, 0x1B);
}

template <>
inline __m128i aeskeygenassist_si128<0xA3>(const __m128i &xmm)
{
    return _mm_aeskeygenassist_si128(xmm, 0x36);
}

template <>
inline __m128i aeskeygenassist_si128<0xA4>(const __m128i &xmm)
{
    return _mm_aeskeygenassist_si128(xmm, 0x6C);
}

template <>
inline __m128i aeskeygenassist_si128<0xA5>(const __m128i &xmm)
{
    return _mm_aeskeygenassist_si128(xmm, 0xD8);
}

template <>
inline __m128i aeskeygenassist_si128<0xA6>(const __m128i &xmm)
{
    return _mm_aeskeygenassist_si128(xmm, 0xAB);
}

template <>
inline __m128i aeskeygenassist_si128<0xA7>(const __m128i &xmm)
{
    return _mm_aeskeygenassist_si128(xmm, 0x4D);
}

template <>
inline __m128i aeskeygenassist_si128<0xA8>(const __m128i &xmm)
{
    return _mm_aeskeygenassist_si128(xmm, 0x9A);
}

template <>
inline __m128i aeskeygenassist_si128<0xA9>(const __m128i &xmm)
{
    return _mm_aeskeygenassist_si128(xmm, 0x2F);
}

template <>
inline __m128i aeskeygenassist_si128<0xAA>(const __m128i &xmm)
{
    return _mm_aeskeygenassist_si128(xmm, 0x5E);
}

template <>
inline __m128i aeskeygenassist_si128<0xAB>(const __m128i &xmm)
{
    return _mm_aeskeygenassist_si128(xmm, 0xBC);
}

template <>
inline __m128i aeskeygenassist_si128<0xAC>(const __m128i &xmm)
{
    return _mm_aeskeygenassist_si128(xmm, 0x63);
}

template <>
inline __m128i aeskeygenassist_si128<0xAD>(const __m128i &xmm)
{
    return _mm_aeskeygenassist_si128(xmm, 0xC6);
}

template <>
inline __m128i aeskeygenassist_si128<0xAE>(const __m128i &xmm)
{
    return _mm_aeskeygenassist_si128(xmm, 0x97);
}

template <>
inline __m128i aeskeygenassist_si128<0xAF>(const __m128i &xmm)
{
    return _mm_aeskeygenassist_si128(xmm, 0x35);
}

template <>
inline __m128i aeskeygenassist_si128<0xB0>(const __m128i &xmm)
{
    return _mm_aeskeygenassist_si128(xmm, 0x6A);
}

template <>
inline __m128i aeskeygenassist_si128<0xB1>(const __m128i &xmm)
{
    return _mm_aeskeygenassist_si128(xmm, 0xD4);
}

template <>
inline __m128i aeskeygenassist_si128<0xB2>(const __m128i &xmm)
{
    return _mm_aeskeygenassist_si128(xmm, 0xB3);
}

template <>
inline __m128i aeskeygenassist_si128<0xB3>(const __m128i &xmm)
{
    return _mm_aeskeygenassist_si128(xmm, 0x7D);
}

template <>
inline __m128i aeskeygenassist_si128<0xB4>(const __m128i &xmm)
{
    return _mm_aeskeygenassist_si128(xmm, 0xFA);
}

template <>
inline __m128i aeskeygenassist_si128<0xB5>(const __m128i &xmm)
{
    return _mm_aeskeygenassist_si128(xmm, 0xEF);
}

template <>
inline __m128i aeskeygenassist_si128<0xB6>(const __m128i &xmm)
{
    return _mm_aeskeygenassist_si128(xmm, 0xC5);
}

template <>
inline __m128i aeskeygenassist_si128<0xB7>(const __m128i &xmm)
{
    return _mm_aeskeygenassist_si128(xmm, 0x91);
}

template <>
inline __m128i aeskeygenassist_si128<0xB8>(const __m128i &xmm)
{
    return _mm_aeskeygenassist_si128(xmm, 0x39);
}

template <>
inline __m128i aeskeygenassist_si128<0xB9>(const __m128i &xmm)
{
    return _mm_aeskeygenassist_si128(xmm, 0x72);
}

template <>
inline __m128i aeskeygenassist_si128<0xBA>(const __m128i &xmm)
{
    return _mm_aeskeygenassist_si128(xmm, 0xE4);
}

template <>
inline __m128i aeskeygenassist_si128<0xBB>(const __m128i &xmm)
{
    return _mm_aeskeygenassist_si128(xmm, 0xD3);
}

template <>
inline __m128i aeskeygenassist_si128<0xBC>(const __m128i &xmm)
{
    return _mm_aeskeygenassist_si128(xmm, 0xBD);
}

template <>
inline __m128i aeskeygenassist_si128<0xBD>(const __m128i &xmm)
{
    return _mm_aeskeygenassist_si128(xmm, 0x61);
}

template <>
inline __m128i aeskeygenassist_si128<0xBE>(const __m128i &xmm)
{
    return _mm_aeskeygenassist_si128(xmm, 0xC2);
}

template <>
inline __m128i aeskeygenassist_si128<0xBF>(const __m128i &xmm)
{
    return _mm_aeskeygenassist_si128(xmm, 0x9F);
}

template <>
inline __m128i aeskeygenassist_si128<0xC0>(const __m128i &xmm)
{
    return _mm_aeskeygenassist_si128(xmm, 0x25);
}

template <>
inline __m128i aeskeygenassist_si128<0xC1>(const __m128i &xmm)
{
    return _mm_aeskeygenassist_si128(xmm, 0x4A);
}

template <>
inline __m128i aeskeygenassist_si128<0xC2>(const __m128i &xmm)
{
    return _mm_aeskeygenassist_si128(xmm, 0x94);
}

template <>
inline __m128i aeskeygenassist_si128<0xC3>(const __m128i &xmm)
{
    return _mm_aeskeygenassist_si128(xmm, 0x33);
}

template <>
inline __m128i aeskeygenassist_si128<0xC4>(const __m128i &xmm)
{
    return _mm_aeskeygenassist_si128(xmm, 0x66);
}

template <>
inline __m128i aeskeygenassist_si128<0xC5>(const __m128i &xmm)
{
    return _mm_aeskeygenassist_si128(xmm, 0xCC);
}

template <>
inline __m128i aeskeygenassist_si128<0xC6>(const __m128i &xmm)
{
    return _mm_aeskeygenassist_si128(xmm, 0x83);
}

template <>
inline __m128i aeskeygenassist_si128<0xC7>(const __m128i &xmm)
{
    return _mm_aeskeygenassist_si128(xmm, 0x1D);
}

template <>
inline __m128i aeskeygenassist_si128<0xC8>(const __m128i &xmm)
{
    return _mm_aeskeygenassist_si128(xmm, 0x3A);
}

template <>
inline __m128i aeskeygenassist_si128<0xC9>(const __m128i &xmm)
{
    return _mm_aeskeygenassist_si128(xmm, 0x74);
}

template <>
inline __m128i aeskeygenassist_si128<0xCA>(const __m128i &xmm)
{
    return _mm_aeskeygenassist_si128(xmm, 0xE8);
}

template <>
inline __m128i aeskeygenassist_si128<0xCB>(const __m128i &xmm)
{
    return _mm_aeskeygenassist_si128(xmm, 0xCB);
}

template <>
inline __m128i aeskeygenassist_si128<0xCC>(const __m128i &xmm)
{
    return _mm_aeskeygenassist_si128(xmm, 0x8D);
}

template <>
inline __m128i aeskeygenassist_si128<0xCD>(const __m128i &xmm)
{
    return _mm_aeskeygenassist_si128(xmm, 0x01);
}

template <>
inline __m128i aeskeygenassist_si128<0xCE>(const __m128i &xmm)
{
    return _mm_aeskeygenassist_si128(xmm, 0x02);
}

template <>
inline __m128i aeskeygenassist_si128<0xCF>(const __m128i &xmm)
{
    return _mm_aeskeygenassist_si128(xmm, 0x04);
}

template <>
inline __m128i aeskeygenassist_si128<0xD0>(const __m128i &xmm)
{
    return _mm_aeskeygenassist_si128(xmm, 0x08);
}

template <>
inline __m128i aeskeygenassist_si128<0xD1>(const __m128i &xmm)
{
    return _mm_aeskeygenassist_si128(xmm, 0x10);
}

template <>
inline __m128i aeskeygenassist_si128<0xD2>(const __m128i &xmm)
{
    return _mm_aeskeygenassist_si128(xmm, 0x20);
}

template <>
inline __m128i aeskeygenassist_si128<0xD3>(const __m128i &xmm)
{
    return _mm_aeskeygenassist_si128(xmm, 0x40);
}

template <>
inline __m128i aeskeygenassist_si128<0xD4>(const __m128i &xmm)
{
    return _mm_aeskeygenassist_si128(xmm, 0x80);
}

template <>
inline __m128i aeskeygenassist_si128<0xD5>(const __m128i &xmm)
{
    return _mm_aeskeygenassist_si128(xmm, 0x1B);
}

template <>
inline __m128i aeskeygenassist_si128<0xD6>(const __m128i &xmm)
{
    return _mm_aeskeygenassist_si128(xmm, 0x36);
}

template <>
inline __m128i aeskeygenassist_si128<0xD7>(const __m128i &xmm)
{
    return _mm_aeskeygenassist_si128(xmm, 0x6C);
}

template <>
inline __m128i aeskeygenassist_si128<0xD8>(const __m128i &xmm)
{
    return _mm_aeskeygenassist_si128(xmm, 0xD8);
}

template <>
inline __m128i aeskeygenassist_si128<0xD9>(const __m128i &xmm)
{
    return _mm_aeskeygenassist_si128(xmm, 0xAB);
}

template <>
inline __m128i aeskeygenassist_si128<0xDA>(const __m128i &xmm)
{
    return _mm_aeskeygenassist_si128(xmm, 0x4D);
}

template <>
inline __m128i aeskeygenassist_si128<0xDB>(const __m128i &xmm)
{
    return _mm_aeskeygenassist_si128(xmm, 0x9A);
}

template <>
inline __m128i aeskeygenassist_si128<0xDC>(const __m128i &xmm)
{
    return _mm_aeskeygenassist_si128(xmm, 0x2F);
}

template <>
inline __m128i aeskeygenassist_si128<0xDD>(const __m128i &xmm)
{
    return _mm_aeskeygenassist_si128(xmm, 0x5E);
}

template <>
inline __m128i aeskeygenassist_si128<0xDE>(const __m128i &xmm)
{
    return _mm_aeskeygenassist_si128(xmm, 0xBC);
}

template <>
inline __m128i aeskeygenassist_si128<0xDF>(const __m128i &xmm)
{
    return _mm_aeskeygenassist_si128(xmm, 0x63);
}

template <>
inline __m128i aeskeygenassist_si128<0xE0>(const __m128i &xmm)
{
    return _mm_aeskeygenassist_si128(xmm, 0xC6);
}

template <>
inline __m128i aeskeygenassist_si128<0xE1>(const __m128i &xmm)
{
    return _mm_aeskeygenassist_si128(xmm, 0x97);
}

template <>
inline __m128i aeskeygenassist_si128<0xE2>(const __m128i &xmm)
{
    return _mm_aeskeygenassist_si128(xmm, 0x35);
}

template <>
inline __m128i aeskeygenassist_si128<0xE3>(const __m128i &xmm)
{
    return _mm_aeskeygenassist_si128(xmm, 0x6A);
}

template <>
inline __m128i aeskeygenassist_si128<0xE4>(const __m128i &xmm)
{
    return _mm_aeskeygenassist_si128(xmm, 0xD4);
}

template <>
inline __m128i aeskeygenassist_si128<0xE5>(const __m128i &xmm)
{
    return _mm_aeskeygenassist_si128(xmm, 0xB3);
}

template <>
inline __m128i aeskeygenassist_si128<0xE6>(const __m128i &xmm)
{
    return _mm_aeskeygenassist_si128(xmm, 0x7D);
}

template <>
inline __m128i aeskeygenassist_si128<0xE7>(const __m128i &xmm)
{
    return _mm_aeskeygenassist_si128(xmm, 0xFA);
}

template <>
inline __m128i aeskeygenassist_si128<0xE8>(const __m128i &xmm)
{
    return _mm_aeskeygenassist_si128(xmm, 0xEF);
}

template <>
inline __m128i aeskeygenassist_si128<0xE9>(const __m128i &xmm)
{
    return _mm_aeskeygenassist_si128(xmm, 0xC5);
}

template <>
inline __m128i aeskeygenassist_si128<0xEA>(const __m128i &xmm)
{
    return _mm_aeskeygenassist_si128(xmm, 0x91);
}

template <>
inline __m128i aeskeygenassist_si128<0xEB>(const __m128i &xmm)
{
    return _mm_aeskeygenassist_si128(xmm, 0x39);
}

template <>
inline __m128i aeskeygenassist_si128<0xEC>(const __m128i &xmm)
{
    return _mm_aeskeygenassist_si128(xmm, 0x72);
}

template <>
inline __m128i aeskeygenassist_si128<0xED>(const __m128i &xmm)
{
    return _mm_aeskeygenassist_si128(xmm, 0xE4);
}

template <>
inline __m128i aeskeygenassist_si128<0xEE>(const __m128i &xmm)
{
    return _mm_aeskeygenassist_si128(xmm, 0xD3);
}

template <>
inline __m128i aeskeygenassist_si128<0xEF>(const __m128i &xmm)
{
    return _mm_aeskeygenassist_si128(xmm, 0xBD);
}

template <>
inline __m128i aeskeygenassist_si128<0xF0>(const __m128i &xmm)
{
    return _mm_aeskeygenassist_si128(xmm, 0x61);
}

template <>
inline __m128i aeskeygenassist_si128<0xF1>(const __m128i &xmm)
{
    return _mm_aeskeygenassist_si128(xmm, 0xC2);
}

template <>
inline __m128i aeskeygenassist_si128<0xF2>(const __m128i &xmm)
{
    return _mm_aeskeygenassist_si128(xmm, 0x9F);
}

template <>
inline __m128i aeskeygenassist_si128<0xF3>(const __m128i &xmm)
{
    return _mm_aeskeygenassist_si128(xmm, 0x25);
}

template <>
inline __m128i aeskeygenassist_si128<0xF4>(const __m128i &xmm)
{
    return _mm_aeskeygenassist_si128(xmm, 0x4A);
}

template <>
inline __m128i aeskeygenassist_si128<0xF5>(const __m128i &xmm)
{
    return _mm_aeskeygenassist_si128(xmm, 0x94);
}

template <>
inline __m128i aeskeygenassist_si128<0xF6>(const __m128i &xmm)
{
    return _mm_aeskeygenassist_si128(xmm, 0x33);
}

template <>
inline __m128i aeskeygenassist_si128<0xF7>(const __m128i &xmm)
{
    return _mm_aeskeygenassist_si128(xmm, 0x66);
}

template <>
inline __m128i aeskeygenassist_si128<0xF8>(const __m128i &xmm)
{
    return _mm_aeskeygenassist_si128(xmm, 0xCC);
}

template <>
inline __m128i aeskeygenassist_si128<0xF9>(const __m128i &xmm)
{
    return _mm_aeskeygenassist_si128(xmm, 0x83);
}

template <>
inline __m128i aeskeygenassist_si128<0xFA>(const __m128i &xmm)
{
    return _mm_aeskeygenassist_si128(xmm, 0x1D);
}

template <>
inline __m128i aeskeygenassist_si128<0xFB>(const __m128i &xmm)
{
    return _mm_aeskeygenassist_si128(xmm, 0x3A);
}

template <>
inline __m128i aeskeygenassist_si128<0xFC>(const __m128i &xmm)
{
    return _mm_aeskeygenassist_si128(xmm, 0x74);
}

template <>
inline __m128i aeskeygenassist_si128<0xFD>(const __m128i &xmm)
{
    return _mm_aeskeygenassist_si128(xmm, 0xE8);
}

template <>
inline __m128i aeskeygenassist_si128<0xFE>(const __m128i &xmm)
{
    return _mm_aeskeygenassist_si128(xmm, 0xCB);
}

template <>
inline __m128i aeskeygenassist_si128<0xFF>(const __m128i &xmm)
{
    return _mm_aeskeygenassist_si128(xmm, 0x8D);
}

} // namespace mckl::internal

} // namespace mckl

#ifdef MCKL_GCC
#if MCKL_GCC_VERSION >= 60000
#pragma GCC diagnostic pop
#endif
#endif

#endif // MCKL_INTERNAL_AESNI_HPP
