//============================================================================
// MCKL/include/mckl/internal/avx2.hpp
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

#ifndef MCKL_INTERNAL_AVX2_HPP
#define MCKL_INTERNAL_AVX2_HPP

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

template <typename T>
MCKL_FLATTEN inline void load_si256(
    std::array<__m256i, 1> &s, const std::array<T, 32 / sizeof(T)> &state)
{
    const __m256i *sptr = reinterpret_cast<const __m256i *>(state.data());
    std::get<0>(s) = _mm256_load_si256(sptr++);
}

template <typename T>
MCKL_FLATTEN inline void load_si256(
    std::array<__m256i, 2> &s, const std::array<T, 64 / sizeof(T)> &state)
{
    const __m256i *sptr = reinterpret_cast<const __m256i *>(state.data());
    std::get<0>(s) = _mm256_load_si256(sptr++);
    std::get<1>(s) = _mm256_load_si256(sptr++);
}

template <typename T>
MCKL_FLATTEN inline void load_si256(
    std::array<__m256i, 4> &s, const std::array<T, 128 / sizeof(T)> &state)
{
    const __m256i *sptr = reinterpret_cast<const __m256i *>(state.data());
    std::get<0>(s) = _mm256_load_si256(sptr++);
    std::get<1>(s) = _mm256_load_si256(sptr++);
    std::get<2>(s) = _mm256_load_si256(sptr++);
    std::get<3>(s) = _mm256_load_si256(sptr++);
}

template <typename T>
MCKL_FLATTEN inline void load_si256(
    std::array<__m256i, 8> &s, const std::array<T, 256 / sizeof(T)> &state)
{
    const __m256i *sptr = reinterpret_cast<const __m256i *>(state.data());
    std::get<0>(s) = _mm256_load_si256(sptr++);
    std::get<1>(s) = _mm256_load_si256(sptr++);
    std::get<2>(s) = _mm256_load_si256(sptr++);
    std::get<3>(s) = _mm256_load_si256(sptr++);
    std::get<4>(s) = _mm256_load_si256(sptr++);
    std::get<5>(s) = _mm256_load_si256(sptr++);
    std::get<6>(s) = _mm256_load_si256(sptr++);
    std::get<7>(s) = _mm256_load_si256(sptr++);
}

template <typename T>
MCKL_FLATTEN inline void load_si256(
    std::array<__m256i, 16> &s, const std::array<T, 512 / sizeof(T)> &state)
{
    const __m256i *sptr = reinterpret_cast<const __m256i *>(state.data());
    std::get<0x0>(s) = _mm256_load_si256(sptr++);
    std::get<0x1>(s) = _mm256_load_si256(sptr++);
    std::get<0x2>(s) = _mm256_load_si256(sptr++);
    std::get<0x3>(s) = _mm256_load_si256(sptr++);
    std::get<0x4>(s) = _mm256_load_si256(sptr++);
    std::get<0x5>(s) = _mm256_load_si256(sptr++);
    std::get<0x6>(s) = _mm256_load_si256(sptr++);
    std::get<0x7>(s) = _mm256_load_si256(sptr++);
    std::get<0x8>(s) = _mm256_load_si256(sptr++);
    std::get<0x9>(s) = _mm256_load_si256(sptr++);
    std::get<0xA>(s) = _mm256_load_si256(sptr++);
    std::get<0xB>(s) = _mm256_load_si256(sptr++);
    std::get<0xC>(s) = _mm256_load_si256(sptr++);
    std::get<0xD>(s) = _mm256_load_si256(sptr++);
    std::get<0xE>(s) = _mm256_load_si256(sptr++);
    std::get<0xF>(s) = _mm256_load_si256(sptr++);
}

template <typename T>
MCKL_FLATTEN inline void store_si256(
    const std::array<__m256i, 1> &s, std::array<T, 32 / sizeof(T)> &state)
{
    __m256i *sptr = reinterpret_cast<__m256i *>(state.data());
    _mm256_store_si256(sptr++, std::get<0>(s));
}

template <typename T>
MCKL_FLATTEN inline void store_si256(
    const std::array<__m256i, 2> &s, std::array<T, 64 / sizeof(T)> &state)
{
    __m256i *sptr = reinterpret_cast<__m256i *>(state.data());
    _mm256_store_si256(sptr++, std::get<0>(s));
    _mm256_store_si256(sptr++, std::get<1>(s));
}

template <typename T>
MCKL_FLATTEN inline void store_si256(
    const std::array<__m256i, 4> &s, std::array<T, 128 / sizeof(T)> &state)
{
    __m256i *sptr = reinterpret_cast<__m256i *>(state.data());
    _mm256_store_si256(sptr++, std::get<0>(s));
    _mm256_store_si256(sptr++, std::get<1>(s));
    _mm256_store_si256(sptr++, std::get<2>(s));
    _mm256_store_si256(sptr++, std::get<3>(s));
}

template <typename T>
MCKL_FLATTEN inline void store_si256(
    const std::array<__m256i, 8> &s, std::array<T, 256 / sizeof(T)> &state)
{
    __m256i *sptr = reinterpret_cast<__m256i *>(state.data());
    _mm256_store_si256(sptr++, std::get<0>(s));
    _mm256_store_si256(sptr++, std::get<1>(s));
    _mm256_store_si256(sptr++, std::get<2>(s));
    _mm256_store_si256(sptr++, std::get<3>(s));
    _mm256_store_si256(sptr++, std::get<4>(s));
    _mm256_store_si256(sptr++, std::get<5>(s));
    _mm256_store_si256(sptr++, std::get<6>(s));
    _mm256_store_si256(sptr++, std::get<7>(s));
}

template <typename T>
MCKL_FLATTEN inline void store_si256(
    const std::array<__m256i, 16> &s, std::array<T, 512 / sizeof(T)> &state)
{
    __m256i *sptr = reinterpret_cast<__m256i *>(state.data());
    _mm256_store_si256(sptr++, std::get<0x0>(s));
    _mm256_store_si256(sptr++, std::get<0x1>(s));
    _mm256_store_si256(sptr++, std::get<0x2>(s));
    _mm256_store_si256(sptr++, std::get<0x3>(s));
    _mm256_store_si256(sptr++, std::get<0x4>(s));
    _mm256_store_si256(sptr++, std::get<0x5>(s));
    _mm256_store_si256(sptr++, std::get<0x6>(s));
    _mm256_store_si256(sptr++, std::get<0x7>(s));
    _mm256_store_si256(sptr++, std::get<0x8>(s));
    _mm256_store_si256(sptr++, std::get<0x9>(s));
    _mm256_store_si256(sptr++, std::get<0xA>(s));
    _mm256_store_si256(sptr++, std::get<0xB>(s));
    _mm256_store_si256(sptr++, std::get<0xC>(s));
    _mm256_store_si256(sptr++, std::get<0xD>(s));
    _mm256_store_si256(sptr++, std::get<0xE>(s));
    _mm256_store_si256(sptr++, std::get<0xF>(s));
}

template <std::size_t i0, std::size_t i1, std::size_t i2, std::size_t i3,
    std::size_t i4, std::size_t i5, std::size_t i6, std::size_t i7,
    std::size_t N>
MCKL_FLATTEN inline void transpose8x32_si256(std::array<__m256i, N> &s)
{
    __m256i s0 = _mm256_unpacklo_epi32(std::get<i0>(s), std::get<i1>(s));
    __m256i s1 = _mm256_unpacklo_epi32(std::get<i2>(s), std::get<i3>(s));
    __m256i s2 = _mm256_unpacklo_epi32(std::get<i4>(s), std::get<i5>(s));
    __m256i s3 = _mm256_unpacklo_epi32(std::get<i6>(s), std::get<i7>(s));
    __m256i t0 = _mm256_unpackhi_epi32(std::get<i0>(s), std::get<i1>(s));
    __m256i t1 = _mm256_unpackhi_epi32(std::get<i2>(s), std::get<i3>(s));
    __m256i t2 = _mm256_unpackhi_epi32(std::get<i4>(s), std::get<i5>(s));
    __m256i t3 = _mm256_unpackhi_epi32(std::get<i6>(s), std::get<i7>(s));

    __m256i u0 = _mm256_unpacklo_epi64(s0, s1);
    __m256i u1 = _mm256_unpacklo_epi64(s2, s3);
    __m256i u2 = _mm256_unpacklo_epi64(t0, t1);
    __m256i u3 = _mm256_unpacklo_epi64(t2, t3);
    __m256i v0 = _mm256_unpackhi_epi64(s0, s1);
    __m256i v1 = _mm256_unpackhi_epi64(s2, s3);
    __m256i v2 = _mm256_unpackhi_epi64(t0, t1);
    __m256i v3 = _mm256_unpackhi_epi64(t2, t3);

    std::get<i0>(s) = _mm256_permute2x128_si256(u0, u1, 0x20);
    std::get<i1>(s) = _mm256_permute2x128_si256(v0, v1, 0x20);
    std::get<i2>(s) = _mm256_permute2x128_si256(u2, u3, 0x20);
    std::get<i3>(s) = _mm256_permute2x128_si256(v2, v3, 0x20);
    std::get<i4>(s) = _mm256_permute2x128_si256(u0, u1, 0x31);
    std::get<i5>(s) = _mm256_permute2x128_si256(v0, v1, 0x31);
    std::get<i6>(s) = _mm256_permute2x128_si256(u2, u3, 0x31);
    std::get<i7>(s) = _mm256_permute2x128_si256(v2, v3, 0x31);
}

template <typename T>
MCKL_FLATTEN inline void transpose8x32_load_si256(
    std::array<__m256i, 8> &s, const std::array<T, 256 / sizeof(T)> &state)
{
    const __m256i *sptr = reinterpret_cast<const __m256i *>(state.data());
    std::get<0>(s) = _mm256_load_si256(sptr++);
    std::get<1>(s) = _mm256_load_si256(sptr++);
    std::get<2>(s) = _mm256_load_si256(sptr++);
    std::get<3>(s) = _mm256_load_si256(sptr++);
    std::get<4>(s) = _mm256_load_si256(sptr++);
    std::get<5>(s) = _mm256_load_si256(sptr++);
    std::get<6>(s) = _mm256_load_si256(sptr++);
    std::get<7>(s) = _mm256_load_si256(sptr++);
    transpose8x32_si256<0, 1, 2, 3, 4, 5, 6, 7>(s);
}

template <typename T>
MCKL_FLATTEN inline void transpose8x32_load_si256(
    std::array<__m256i, 16> &s, const std::array<T, 512 / sizeof(T)> &state)
{
    const __m256i *sptr = reinterpret_cast<const __m256i *>(state.data());
    std::get<0x0>(s) = _mm256_load_si256(sptr++);
    std::get<0x8>(s) = _mm256_load_si256(sptr++);
    std::get<0x1>(s) = _mm256_load_si256(sptr++);
    std::get<0x9>(s) = _mm256_load_si256(sptr++);
    std::get<0x2>(s) = _mm256_load_si256(sptr++);
    std::get<0xA>(s) = _mm256_load_si256(sptr++);
    std::get<0x3>(s) = _mm256_load_si256(sptr++);
    std::get<0xB>(s) = _mm256_load_si256(sptr++);
    std::get<0x4>(s) = _mm256_load_si256(sptr++);
    std::get<0xC>(s) = _mm256_load_si256(sptr++);
    std::get<0x5>(s) = _mm256_load_si256(sptr++);
    std::get<0xD>(s) = _mm256_load_si256(sptr++);
    std::get<0x6>(s) = _mm256_load_si256(sptr++);
    std::get<0xE>(s) = _mm256_load_si256(sptr++);
    std::get<0x7>(s) = _mm256_load_si256(sptr++);
    std::get<0xF>(s) = _mm256_load_si256(sptr++);
    transpose8x32_si256<0x0, 0x1, 0x2, 0x3, 0x4, 0x5, 0x6, 0x7>(s);
    transpose8x32_si256<0x8, 0x9, 0xA, 0xB, 0xC, 0xD, 0xE, 0xF>(s);
}

template <typename T>
MCKL_FLATTEN inline void transpose8x32_store_si256(
    std::array<__m256i, 8> &s, std::array<T, 256 / sizeof(T)> &state)
{
    transpose8x32_si256<0, 1, 2, 3, 4, 5, 6, 7>(s);
    __m256i *sptr = reinterpret_cast<__m256i *>(state.data());
    _mm256_store_si256(sptr++, std::get<0>(s));
    _mm256_store_si256(sptr++, std::get<1>(s));
    _mm256_store_si256(sptr++, std::get<2>(s));
    _mm256_store_si256(sptr++, std::get<3>(s));
    _mm256_store_si256(sptr++, std::get<4>(s));
    _mm256_store_si256(sptr++, std::get<5>(s));
    _mm256_store_si256(sptr++, std::get<6>(s));
    _mm256_store_si256(sptr++, std::get<7>(s));
}

template <typename T>
MCKL_FLATTEN inline void transpose8x32_store_si256(
    std::array<__m256i, 16> &s, std::array<T, 512 / sizeof(T)> &state)
{
    transpose8x32_si256<0x0, 0x1, 0x2, 0x3, 0x4, 0x5, 0x6, 0x7>(s);
    transpose8x32_si256<0x8, 0x9, 0xA, 0xB, 0xC, 0xD, 0xE, 0xF>(s);
    __m256i *sptr = reinterpret_cast<__m256i *>(state.data());
    _mm256_store_si256(sptr++, std::get<0x0>(s));
    _mm256_store_si256(sptr++, std::get<0x8>(s));
    _mm256_store_si256(sptr++, std::get<0x1>(s));
    _mm256_store_si256(sptr++, std::get<0x9>(s));
    _mm256_store_si256(sptr++, std::get<0x2>(s));
    _mm256_store_si256(sptr++, std::get<0xA>(s));
    _mm256_store_si256(sptr++, std::get<0x3>(s));
    _mm256_store_si256(sptr++, std::get<0xB>(s));
    _mm256_store_si256(sptr++, std::get<0x4>(s));
    _mm256_store_si256(sptr++, std::get<0xC>(s));
    _mm256_store_si256(sptr++, std::get<0x5>(s));
    _mm256_store_si256(sptr++, std::get<0xD>(s));
    _mm256_store_si256(sptr++, std::get<0x6>(s));
    _mm256_store_si256(sptr++, std::get<0xE>(s));
    _mm256_store_si256(sptr++, std::get<0x7>(s));
    _mm256_store_si256(sptr++, std::get<0xF>(s));
}

template <std::size_t i0, std::size_t i1, std::size_t i2, std::size_t i3,
    std::size_t N>
MCKL_FLATTEN inline void transpose4x64_si256(std::array<__m256i, N> &s)
{
    __m256i s0 = _mm256_unpacklo_epi64(std::get<i0>(s), std::get<i1>(s));
    __m256i s1 = _mm256_unpacklo_epi64(std::get<i2>(s), std::get<i3>(s));
    __m256i t0 = _mm256_unpackhi_epi64(std::get<i0>(s), std::get<i1>(s));
    __m256i t1 = _mm256_unpackhi_epi64(std::get<i2>(s), std::get<i3>(s));

    std::get<i0>(s) = _mm256_permute2x128_si256(s0, s1, 0x20);
    std::get<i1>(s) = _mm256_permute2x128_si256(t0, t1, 0x20);
    std::get<i2>(s) = _mm256_permute2x128_si256(s0, s1, 0x31);
    std::get<i3>(s) = _mm256_permute2x128_si256(t0, t1, 0x31);
}

template <typename T>
MCKL_FLATTEN inline void transpose4x64_load_si256(
    std::array<__m256i, 4> &s, const std::array<T, 128 / sizeof(T)> &state)
{
    const __m256i *sptr = reinterpret_cast<const __m256i *>(state.data());
    std::get<0>(s) = _mm256_load_si256(sptr++);
    std::get<1>(s) = _mm256_load_si256(sptr++);
    std::get<2>(s) = _mm256_load_si256(sptr++);
    std::get<3>(s) = _mm256_load_si256(sptr++);
    transpose4x64_si256<0, 1, 2, 3>(s);
}

template <typename T>
MCKL_FLATTEN inline void transpose4x64_load_si256(
    std::array<__m256i, 8> &s, const std::array<T, 256 / sizeof(T)> &state)
{
    const __m256i *sptr = reinterpret_cast<const __m256i *>(state.data());
    std::get<0>(s) = _mm256_load_si256(sptr++);
    std::get<4>(s) = _mm256_load_si256(sptr++);
    std::get<1>(s) = _mm256_load_si256(sptr++);
    std::get<5>(s) = _mm256_load_si256(sptr++);
    std::get<2>(s) = _mm256_load_si256(sptr++);
    std::get<6>(s) = _mm256_load_si256(sptr++);
    std::get<3>(s) = _mm256_load_si256(sptr++);
    std::get<7>(s) = _mm256_load_si256(sptr++);
    transpose4x64_si256<0, 1, 2, 3>(s);
    transpose4x64_si256<4, 5, 6, 7>(s);
}

template <typename T>
MCKL_FLATTEN inline void transpose4x64_load_si256(
    std::array<__m256i, 16> &s, const std::array<T, 512 / sizeof(T)> &state)
{
    const __m256i *sptr = reinterpret_cast<const __m256i *>(state.data());
    std::get<0x0>(s) = _mm256_load_si256(sptr++);
    std::get<0x4>(s) = _mm256_load_si256(sptr++);
    std::get<0x8>(s) = _mm256_load_si256(sptr++);
    std::get<0xC>(s) = _mm256_load_si256(sptr++);
    std::get<0x1>(s) = _mm256_load_si256(sptr++);
    std::get<0x5>(s) = _mm256_load_si256(sptr++);
    std::get<0x9>(s) = _mm256_load_si256(sptr++);
    std::get<0xD>(s) = _mm256_load_si256(sptr++);
    std::get<0x2>(s) = _mm256_load_si256(sptr++);
    std::get<0x6>(s) = _mm256_load_si256(sptr++);
    std::get<0xA>(s) = _mm256_load_si256(sptr++);
    std::get<0xE>(s) = _mm256_load_si256(sptr++);
    std::get<0x3>(s) = _mm256_load_si256(sptr++);
    std::get<0x7>(s) = _mm256_load_si256(sptr++);
    std::get<0xB>(s) = _mm256_load_si256(sptr++);
    std::get<0xF>(s) = _mm256_load_si256(sptr++);
    transpose4x64_si256<0x0, 0x1, 0x2, 0x3>(s);
    transpose4x64_si256<0x4, 0x5, 0x6, 0x7>(s);
    transpose4x64_si256<0x8, 0x9, 0xA, 0xB>(s);
    transpose4x64_si256<0xC, 0xD, 0xE, 0xF>(s);
}

template <typename T>
MCKL_FLATTEN inline void transpose4x64_store_si256(
    std::array<__m256i, 4> &s, std::array<T, 128 / sizeof(T)> &state)
{
    transpose4x64_si256<0, 1, 2, 3>(s);
    __m256i *sptr = reinterpret_cast<__m256i *>(state.data());
    _mm256_store_si256(sptr++, std::get<0>(s));
    _mm256_store_si256(sptr++, std::get<1>(s));
    _mm256_store_si256(sptr++, std::get<2>(s));
    _mm256_store_si256(sptr++, std::get<3>(s));
}

template <typename T>
MCKL_FLATTEN inline void transpose4x64_store_si256(
    std::array<__m256i, 8> &s, std::array<T, 256 / sizeof(T)> &state)
{
    transpose4x64_si256<0, 1, 2, 3>(s);
    transpose4x64_si256<4, 5, 6, 7>(s);
    __m256i *sptr = reinterpret_cast<__m256i *>(state.data());
    _mm256_store_si256(sptr++, std::get<0>(s));
    _mm256_store_si256(sptr++, std::get<4>(s));
    _mm256_store_si256(sptr++, std::get<1>(s));
    _mm256_store_si256(sptr++, std::get<5>(s));
    _mm256_store_si256(sptr++, std::get<2>(s));
    _mm256_store_si256(sptr++, std::get<6>(s));
    _mm256_store_si256(sptr++, std::get<3>(s));
    _mm256_store_si256(sptr++, std::get<7>(s));
}

template <typename T>
MCKL_FLATTEN inline void transpose4x64_store_si256(
    std::array<__m256i, 16> &s, std::array<T, 512 / sizeof(T)> &state)
{
    transpose4x64_si256<0x0, 0x1, 0x2, 0x3>(s);
    transpose4x64_si256<0x4, 0x5, 0x6, 0x7>(s);
    transpose4x64_si256<0x8, 0x9, 0xA, 0xB>(s);
    transpose4x64_si256<0xC, 0xD, 0xE, 0xF>(s);
    __m256i *sptr = reinterpret_cast<__m256i *>(state.data());
    _mm256_store_si256(sptr++, std::get<0x0>(s));
    _mm256_store_si256(sptr++, std::get<0x4>(s));
    _mm256_store_si256(sptr++, std::get<0x8>(s));
    _mm256_store_si256(sptr++, std::get<0xC>(s));
    _mm256_store_si256(sptr++, std::get<0x1>(s));
    _mm256_store_si256(sptr++, std::get<0x5>(s));
    _mm256_store_si256(sptr++, std::get<0x9>(s));
    _mm256_store_si256(sptr++, std::get<0xD>(s));
    _mm256_store_si256(sptr++, std::get<0x2>(s));
    _mm256_store_si256(sptr++, std::get<0x6>(s));
    _mm256_store_si256(sptr++, std::get<0xA>(s));
    _mm256_store_si256(sptr++, std::get<0xE>(s));
    _mm256_store_si256(sptr++, std::get<0x3>(s));
    _mm256_store_si256(sptr++, std::get<0x7>(s));
    _mm256_store_si256(sptr++, std::get<0xB>(s));
    _mm256_store_si256(sptr++, std::get<0xF>(s));
}

} // namespace mckl::internal

} // namespace mckl

#ifdef MCKL_GCC
#if MCKL_GCC_VERSION >= 60000
#pragma GCC diagnostic pop
#endif
#endif

#endif // MCKL_INTERNAL_AVX2_HPP
