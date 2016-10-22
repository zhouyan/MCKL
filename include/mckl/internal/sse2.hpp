//============================================================================
// MCKL/include/mckl/internal/sse2.hpp
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

#ifndef MCKL_INTERNAL_SSE2_HPP
#define MCKL_INTERNAL_SSE2_HPP

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

inline void load_si128(std::array<__m128i, 1> &s, const __m128i *sptr)
{
    std::get<0>(s) = _mm_load_si128(sptr++);
}

inline void load_si128(std::array<__m128i, 2> &s, const __m128i *sptr)
{
    std::get<0>(s) = _mm_load_si128(sptr++);
    std::get<1>(s) = _mm_load_si128(sptr++);
}

inline void load_si128(std::array<__m128i, 4> &s, const __m128i *sptr)
{
    std::get<0>(s) = _mm_load_si128(sptr++);
    std::get<1>(s) = _mm_load_si128(sptr++);
    std::get<2>(s) = _mm_load_si128(sptr++);
    std::get<3>(s) = _mm_load_si128(sptr++);
}

inline void load_si128(std::array<__m128i, 8> &s, const __m128i *sptr)
{
    std::get<0>(s) = _mm_load_si128(sptr++);
    std::get<1>(s) = _mm_load_si128(sptr++);
    std::get<2>(s) = _mm_load_si128(sptr++);
    std::get<3>(s) = _mm_load_si128(sptr++);
    std::get<4>(s) = _mm_load_si128(sptr++);
    std::get<5>(s) = _mm_load_si128(sptr++);
    std::get<6>(s) = _mm_load_si128(sptr++);
    std::get<7>(s) = _mm_load_si128(sptr++);
}

inline void load_si128(std::array<__m128i, 16> &s, const __m128i *sptr)
{
    std::get<0x0>(s) = _mm_load_si128(sptr++);
    std::get<0x1>(s) = _mm_load_si128(sptr++);
    std::get<0x2>(s) = _mm_load_si128(sptr++);
    std::get<0x3>(s) = _mm_load_si128(sptr++);
    std::get<0x4>(s) = _mm_load_si128(sptr++);
    std::get<0x5>(s) = _mm_load_si128(sptr++);
    std::get<0x6>(s) = _mm_load_si128(sptr++);
    std::get<0x7>(s) = _mm_load_si128(sptr++);
    std::get<0x8>(s) = _mm_load_si128(sptr++);
    std::get<0x9>(s) = _mm_load_si128(sptr++);
    std::get<0xA>(s) = _mm_load_si128(sptr++);
    std::get<0xB>(s) = _mm_load_si128(sptr++);
    std::get<0xC>(s) = _mm_load_si128(sptr++);
    std::get<0xD>(s) = _mm_load_si128(sptr++);
    std::get<0xE>(s) = _mm_load_si128(sptr++);
    std::get<0xF>(s) = _mm_load_si128(sptr++);
}

inline void store_si128(const std::array<__m128i, 1> &s, __m128i *sptr)
{
    _mm_store_si128(sptr++, std::get<0>(s));
}

inline void store_si128(const std::array<__m128i, 2> &s, __m128i *sptr)
{
    _mm_store_si128(sptr++, std::get<0>(s));
    _mm_store_si128(sptr++, std::get<1>(s));
}

inline void store_si128(const std::array<__m128i, 4> &s, __m128i *sptr)
{
    _mm_store_si128(sptr++, std::get<0>(s));
    _mm_store_si128(sptr++, std::get<1>(s));
    _mm_store_si128(sptr++, std::get<2>(s));
    _mm_store_si128(sptr++, std::get<3>(s));
}

inline void store_si128(const std::array<__m128i, 8> &s, __m128i *sptr)
{
    _mm_store_si128(sptr++, std::get<0>(s));
    _mm_store_si128(sptr++, std::get<1>(s));
    _mm_store_si128(sptr++, std::get<2>(s));
    _mm_store_si128(sptr++, std::get<3>(s));
    _mm_store_si128(sptr++, std::get<4>(s));
    _mm_store_si128(sptr++, std::get<5>(s));
    _mm_store_si128(sptr++, std::get<6>(s));
    _mm_store_si128(sptr++, std::get<7>(s));
}

inline void store_si128(const std::array<__m128i, 16> &s, __m128i *sptr)
{
    _mm_store_si128(sptr++, std::get<0x0>(s));
    _mm_store_si128(sptr++, std::get<0x1>(s));
    _mm_store_si128(sptr++, std::get<0x2>(s));
    _mm_store_si128(sptr++, std::get<0x3>(s));
    _mm_store_si128(sptr++, std::get<0x4>(s));
    _mm_store_si128(sptr++, std::get<0x5>(s));
    _mm_store_si128(sptr++, std::get<0x6>(s));
    _mm_store_si128(sptr++, std::get<0x7>(s));
    _mm_store_si128(sptr++, std::get<0x8>(s));
    _mm_store_si128(sptr++, std::get<0x9>(s));
    _mm_store_si128(sptr++, std::get<0xA>(s));
    _mm_store_si128(sptr++, std::get<0xB>(s));
    _mm_store_si128(sptr++, std::get<0xC>(s));
    _mm_store_si128(sptr++, std::get<0xD>(s));
    _mm_store_si128(sptr++, std::get<0xE>(s));
    _mm_store_si128(sptr++, std::get<0xF>(s));
}

template <std::size_t i0, std::size_t i1, std::size_t i2, std::size_t i3,
    std::size_t N>
inline void transpose4x32_si128(std::array<__m128i, N> &s)
{
    __m128i s0 = _mm_unpacklo_epi32(std::get<i0>(s), std::get<i1>(s));
    __m128i s1 = _mm_unpacklo_epi32(std::get<i2>(s), std::get<i3>(s));
    __m128i t0 = _mm_unpackhi_epi32(std::get<i0>(s), std::get<i1>(s));
    __m128i t1 = _mm_unpackhi_epi32(std::get<i2>(s), std::get<i3>(s));

    std::get<i0>(s) = _mm_unpacklo_epi64(s0, s1);
    std::get<i1>(s) = _mm_unpackhi_epi64(s0, s1);
    std::get<i2>(s) = _mm_unpacklo_epi64(t0, t1);
    std::get<i3>(s) = _mm_unpackhi_epi64(t0, t1);
}

inline void transpose4x32_load_si128(
    std::array<__m128i, 4> &s, const __m128i *sptr)
{
    std::get<0>(s) = _mm_load_si128(sptr++);
    std::get<1>(s) = _mm_load_si128(sptr++);
    std::get<2>(s) = _mm_load_si128(sptr++);
    std::get<3>(s) = _mm_load_si128(sptr++);
    transpose4x32_si128<0, 1, 2, 3>(s);
}

inline void transpose4x32_load_si128(
    std::array<__m128i, 8> &s, const __m128i *sptr)
{
    std::get<0>(s) = _mm_load_si128(sptr++);
    std::get<4>(s) = _mm_load_si128(sptr++);
    std::get<1>(s) = _mm_load_si128(sptr++);
    std::get<5>(s) = _mm_load_si128(sptr++);
    std::get<2>(s) = _mm_load_si128(sptr++);
    std::get<6>(s) = _mm_load_si128(sptr++);
    std::get<3>(s) = _mm_load_si128(sptr++);
    std::get<7>(s) = _mm_load_si128(sptr++);
    transpose4x32_si128<0, 1, 2, 3>(s);
    transpose4x32_si128<4, 5, 6, 7>(s);
}

inline void transpose4x32_load_si128(
    std::array<__m128i, 16> &s, const __m128i *sptr)
{
    std::get<0x0>(s) = _mm_load_si128(sptr++);
    std::get<0x4>(s) = _mm_load_si128(sptr++);
    std::get<0x8>(s) = _mm_load_si128(sptr++);
    std::get<0xC>(s) = _mm_load_si128(sptr++);
    std::get<0x1>(s) = _mm_load_si128(sptr++);
    std::get<0x5>(s) = _mm_load_si128(sptr++);
    std::get<0x9>(s) = _mm_load_si128(sptr++);
    std::get<0xD>(s) = _mm_load_si128(sptr++);
    std::get<0x2>(s) = _mm_load_si128(sptr++);
    std::get<0x6>(s) = _mm_load_si128(sptr++);
    std::get<0xA>(s) = _mm_load_si128(sptr++);
    std::get<0xE>(s) = _mm_load_si128(sptr++);
    std::get<0x3>(s) = _mm_load_si128(sptr++);
    std::get<0x7>(s) = _mm_load_si128(sptr++);
    std::get<0xB>(s) = _mm_load_si128(sptr++);
    std::get<0xF>(s) = _mm_load_si128(sptr++);
    transpose4x32_si128<0x0, 0x1, 0x2, 0x3>(s);
    transpose4x32_si128<0x4, 0x5, 0x6, 0x7>(s);
    transpose4x32_si128<0x8, 0x9, 0xA, 0xB>(s);
    transpose4x32_si128<0xC, 0xD, 0xE, 0xF>(s);
}

inline void transpose4x32_store_si128(std::array<__m128i, 4> &s, __m128i *sptr)
{
    transpose4x32_si128<0, 1, 2, 3>(s);
    _mm_store_si128(sptr++, std::get<0>(s));
    _mm_store_si128(sptr++, std::get<1>(s));
    _mm_store_si128(sptr++, std::get<2>(s));
    _mm_store_si128(sptr++, std::get<3>(s));
}

inline void transpose4x32_store_si128(std::array<__m128i, 8> &s, __m128i *sptr)
{
    transpose4x32_si128<0, 1, 2, 3>(s);
    transpose4x32_si128<4, 5, 6, 7>(s);
    _mm_store_si128(sptr++, std::get<0>(s));
    _mm_store_si128(sptr++, std::get<4>(s));
    _mm_store_si128(sptr++, std::get<1>(s));
    _mm_store_si128(sptr++, std::get<5>(s));
    _mm_store_si128(sptr++, std::get<2>(s));
    _mm_store_si128(sptr++, std::get<6>(s));
    _mm_store_si128(sptr++, std::get<3>(s));
    _mm_store_si128(sptr++, std::get<7>(s));
}

inline void transpose4x32_store_si128(
    std::array<__m128i, 16> &s, __m128i *sptr)
{
    transpose4x32_si128<0x0, 0x1, 0x2, 0x3>(s);
    transpose4x32_si128<0x4, 0x5, 0x6, 0x7>(s);
    transpose4x32_si128<0x8, 0x9, 0xA, 0xB>(s);
    transpose4x32_si128<0xC, 0xD, 0xE, 0xF>(s);
    _mm_store_si128(sptr++, std::get<0x0>(s));
    _mm_store_si128(sptr++, std::get<0x4>(s));
    _mm_store_si128(sptr++, std::get<0x8>(s));
    _mm_store_si128(sptr++, std::get<0xC>(s));
    _mm_store_si128(sptr++, std::get<0x1>(s));
    _mm_store_si128(sptr++, std::get<0x5>(s));
    _mm_store_si128(sptr++, std::get<0x9>(s));
    _mm_store_si128(sptr++, std::get<0xD>(s));
    _mm_store_si128(sptr++, std::get<0x2>(s));
    _mm_store_si128(sptr++, std::get<0x6>(s));
    _mm_store_si128(sptr++, std::get<0xA>(s));
    _mm_store_si128(sptr++, std::get<0xE>(s));
    _mm_store_si128(sptr++, std::get<0x3>(s));
    _mm_store_si128(sptr++, std::get<0x7>(s));
    _mm_store_si128(sptr++, std::get<0xB>(s));
    _mm_store_si128(sptr++, std::get<0xF>(s));
}

template <std::size_t i0, std::size_t i1, std::size_t N>
inline void transpose2x64_si128(std::array<__m128i, N> &s)
{
    __m128i s0 = _mm_unpacklo_epi64(std::get<i0>(s), std::get<i1>(s));
    __m128i t0 = _mm_unpackhi_epi64(std::get<i0>(s), std::get<i1>(s));

    std::get<i0>(s) = s0;
    std::get<i1>(s) = t0;
}

inline void transpose2x64_load_si128(
    std::array<__m128i, 2> &s, const __m128i *sptr)
{
    std::get<0>(s) = _mm_load_si128(sptr++);
    std::get<1>(s) = _mm_load_si128(sptr++);
    transpose2x64_si128<0, 1>(s);
}

inline void transpose2x64_load_si128(
    std::array<__m128i, 4> &s, const __m128i *sptr)
{
    std::get<0>(s) = _mm_load_si128(sptr++);
    std::get<2>(s) = _mm_load_si128(sptr++);
    std::get<1>(s) = _mm_load_si128(sptr++);
    std::get<3>(s) = _mm_load_si128(sptr++);
    transpose2x64_si128<0, 1>(s);
    transpose2x64_si128<2, 3>(s);
}

inline void transpose2x64_load_si128(
    std::array<__m128i, 8> &s, const __m128i *sptr)
{
    std::get<0>(s) = _mm_load_si128(sptr++);
    std::get<2>(s) = _mm_load_si128(sptr++);
    std::get<4>(s) = _mm_load_si128(sptr++);
    std::get<6>(s) = _mm_load_si128(sptr++);
    std::get<1>(s) = _mm_load_si128(sptr++);
    std::get<3>(s) = _mm_load_si128(sptr++);
    std::get<5>(s) = _mm_load_si128(sptr++);
    std::get<7>(s) = _mm_load_si128(sptr++);
    transpose2x64_si128<0, 1>(s);
    transpose2x64_si128<2, 3>(s);
    transpose2x64_si128<4, 5>(s);
    transpose2x64_si128<6, 7>(s);
}

inline void transpose2x64_load_si128(
    std::array<__m128i, 16> &s, const __m128i *sptr)
{
    std::get<0x0>(s) = _mm_load_si128(sptr++);
    std::get<0x2>(s) = _mm_load_si128(sptr++);
    std::get<0x4>(s) = _mm_load_si128(sptr++);
    std::get<0x6>(s) = _mm_load_si128(sptr++);
    std::get<0x8>(s) = _mm_load_si128(sptr++);
    std::get<0xA>(s) = _mm_load_si128(sptr++);
    std::get<0xC>(s) = _mm_load_si128(sptr++);
    std::get<0xE>(s) = _mm_load_si128(sptr++);
    std::get<0x1>(s) = _mm_load_si128(sptr++);
    std::get<0x3>(s) = _mm_load_si128(sptr++);
    std::get<0x5>(s) = _mm_load_si128(sptr++);
    std::get<0x7>(s) = _mm_load_si128(sptr++);
    std::get<0x9>(s) = _mm_load_si128(sptr++);
    std::get<0xB>(s) = _mm_load_si128(sptr++);
    std::get<0xD>(s) = _mm_load_si128(sptr++);
    std::get<0xF>(s) = _mm_load_si128(sptr++);
    transpose2x64_si128<0x0, 0x1>(s);
    transpose2x64_si128<0x2, 0x3>(s);
    transpose2x64_si128<0x4, 0x5>(s);
    transpose2x64_si128<0x6, 0x7>(s);
    transpose2x64_si128<0x8, 0x9>(s);
    transpose2x64_si128<0xA, 0xB>(s);
    transpose2x64_si128<0xC, 0xD>(s);
    transpose2x64_si128<0xE, 0xF>(s);
}

inline void transpose2x64_store_si128(std::array<__m128i, 2> &s, __m128i *sptr)
{
    transpose2x64_si128<0, 1>(s);
    _mm_store_si128(sptr++, std::get<0>(s));
    _mm_store_si128(sptr++, std::get<1>(s));
}

inline void transpose2x64_store_si128(std::array<__m128i, 4> &s, __m128i *sptr)
{
    transpose2x64_si128<0, 1>(s);
    transpose2x64_si128<2, 3>(s);
    _mm_store_si128(sptr++, std::get<0>(s));
    _mm_store_si128(sptr++, std::get<2>(s));
    _mm_store_si128(sptr++, std::get<1>(s));
    _mm_store_si128(sptr++, std::get<3>(s));
}

inline void transpose2x64_store_si128(std::array<__m128i, 8> &s, __m128i *sptr)
{
    transpose2x64_si128<0, 1>(s);
    transpose2x64_si128<2, 3>(s);
    transpose2x64_si128<4, 5>(s);
    transpose2x64_si128<6, 7>(s);
    _mm_store_si128(sptr++, std::get<0>(s));
    _mm_store_si128(sptr++, std::get<2>(s));
    _mm_store_si128(sptr++, std::get<4>(s));
    _mm_store_si128(sptr++, std::get<6>(s));
    _mm_store_si128(sptr++, std::get<1>(s));
    _mm_store_si128(sptr++, std::get<3>(s));
    _mm_store_si128(sptr++, std::get<5>(s));
    _mm_store_si128(sptr++, std::get<7>(s));
}

inline void transpose2x64_store_si128(
    std::array<__m128i, 16> &s, __m128i *sptr)
{
    transpose2x64_si128<0x0, 0x1>(s);
    transpose2x64_si128<0x2, 0x3>(s);
    transpose2x64_si128<0x4, 0x5>(s);
    transpose2x64_si128<0x6, 0x7>(s);
    transpose2x64_si128<0x8, 0x9>(s);
    transpose2x64_si128<0xA, 0xB>(s);
    transpose2x64_si128<0xC, 0xD>(s);
    transpose2x64_si128<0xE, 0xF>(s);
    _mm_store_si128(sptr++, std::get<0x0>(s));
    _mm_store_si128(sptr++, std::get<0x2>(s));
    _mm_store_si128(sptr++, std::get<0x4>(s));
    _mm_store_si128(sptr++, std::get<0x6>(s));
    _mm_store_si128(sptr++, std::get<0x8>(s));
    _mm_store_si128(sptr++, std::get<0xA>(s));
    _mm_store_si128(sptr++, std::get<0xC>(s));
    _mm_store_si128(sptr++, std::get<0xE>(s));
    _mm_store_si128(sptr++, std::get<0x1>(s));
    _mm_store_si128(sptr++, std::get<0x3>(s));
    _mm_store_si128(sptr++, std::get<0x5>(s));
    _mm_store_si128(sptr++, std::get<0x7>(s));
    _mm_store_si128(sptr++, std::get<0x9>(s));
    _mm_store_si128(sptr++, std::get<0xB>(s));
    _mm_store_si128(sptr++, std::get<0xD>(s));
    _mm_store_si128(sptr++, std::get<0xF>(s));
}

} // namespace mckl::internal

} // namespace mckl

#ifdef MCKL_GCC
#if MCKL_GCC_VERSION >= 60000
#pragma GCC diagnostic pop
#endif
#endif

#endif // MCKL_INTERNAL_SSE2_HPP
