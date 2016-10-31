//============================================================================
// MCKL/include/mckl/internal/fma.hpp
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

#ifndef MCKL_INTERNAL_FMA_HPP
#define MCKL_INTERNAL_FMA_HPP

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

MCKL_FLATTEN inline void fmadd_ps(
    std::array<__m256i, 4> &s, const __m256i &a, const __m256i &b)
{
    std::get<0>(s) = _mm256_castps_si256(
        _mm256_fmadd_ps(_mm256_castsi256_ps(std::get<0>(s)),
            _mm256_castsi256_ps(a), _mm256_castsi256_ps(b)));
    std::get<1>(s) = _mm256_castps_si256(
        _mm256_fmadd_ps(_mm256_castsi256_ps(std::get<1>(s)),
            _mm256_castsi256_ps(a), _mm256_castsi256_ps(b)));
    std::get<2>(s) = _mm256_castps_si256(
        _mm256_fmadd_ps(_mm256_castsi256_ps(std::get<2>(s)),
            _mm256_castsi256_ps(a), _mm256_castsi256_ps(b)));
    std::get<3>(s) = _mm256_castps_si256(
        _mm256_fmadd_ps(_mm256_castsi256_ps(std::get<3>(s)),
            _mm256_castsi256_ps(a), _mm256_castsi256_ps(b)));
}

MCKL_FLATTEN inline void fmadd_pd(
    std::array<__m256i, 4> &s, const __m256i &a, const __m256i &b)
{
    std::get<0>(s) = _mm256_castpd_si256(
        _mm256_fmadd_pd(_mm256_castsi256_pd(std::get<0>(s)),
            _mm256_castsi256_pd(a), _mm256_castsi256_pd(b)));
    std::get<1>(s) = _mm256_castpd_si256(
        _mm256_fmadd_pd(_mm256_castsi256_pd(std::get<1>(s)),
            _mm256_castsi256_pd(a), _mm256_castsi256_pd(b)));
    std::get<2>(s) = _mm256_castpd_si256(
        _mm256_fmadd_pd(_mm256_castsi256_pd(std::get<2>(s)),
            _mm256_castsi256_pd(a), _mm256_castsi256_pd(b)));
    std::get<3>(s) = _mm256_castpd_si256(
        _mm256_fmadd_pd(_mm256_castsi256_pd(std::get<3>(s)),
            _mm256_castsi256_pd(a), _mm256_castsi256_pd(b)));
}

MCKL_FLATTEN inline void fmadd_pd(std::array<__m256i, 4> &s, const __m256i &a,
    const std::array<__m256i, 4> &b)
{
    std::get<0>(s) = _mm256_castpd_si256(
        _mm256_fmadd_pd(_mm256_castsi256_pd(std::get<0>(s)),
            _mm256_castsi256_pd(a), _mm256_castsi256_pd(std::get<0>(b))));
    std::get<1>(s) = _mm256_castpd_si256(
        _mm256_fmadd_pd(_mm256_castsi256_pd(std::get<1>(s)),
            _mm256_castsi256_pd(a), _mm256_castsi256_pd(std::get<1>(b))));
    std::get<2>(s) = _mm256_castpd_si256(
        _mm256_fmadd_pd(_mm256_castsi256_pd(std::get<2>(s)),
            _mm256_castsi256_pd(a), _mm256_castsi256_pd(std::get<2>(b))));
    std::get<3>(s) = _mm256_castpd_si256(
        _mm256_fmadd_pd(_mm256_castsi256_pd(std::get<3>(s)),
            _mm256_castsi256_pd(a), _mm256_castsi256_pd(std::get<3>(b))));
}

MCKL_FLATTEN inline void fmadd_ps(
    std::array<__m256i, 8> &s, const __m256i &a, const __m256i &b)
{
    std::get<0>(s) = _mm256_castps_si256(
        _mm256_fmadd_ps(_mm256_castsi256_ps(std::get<0>(s)),
            _mm256_castsi256_ps(a), _mm256_castsi256_ps(b)));
    std::get<1>(s) = _mm256_castps_si256(
        _mm256_fmadd_ps(_mm256_castsi256_ps(std::get<1>(s)),
            _mm256_castsi256_ps(a), _mm256_castsi256_ps(b)));
    std::get<2>(s) = _mm256_castps_si256(
        _mm256_fmadd_ps(_mm256_castsi256_ps(std::get<2>(s)),
            _mm256_castsi256_ps(a), _mm256_castsi256_ps(b)));
    std::get<3>(s) = _mm256_castps_si256(
        _mm256_fmadd_ps(_mm256_castsi256_ps(std::get<3>(s)),
            _mm256_castsi256_ps(a), _mm256_castsi256_ps(b)));
    std::get<4>(s) = _mm256_castps_si256(
        _mm256_fmadd_ps(_mm256_castsi256_ps(std::get<4>(s)),
            _mm256_castsi256_ps(a), _mm256_castsi256_ps(b)));
    std::get<5>(s) = _mm256_castps_si256(
        _mm256_fmadd_ps(_mm256_castsi256_ps(std::get<5>(s)),
            _mm256_castsi256_ps(a), _mm256_castsi256_ps(b)));
    std::get<6>(s) = _mm256_castps_si256(
        _mm256_fmadd_ps(_mm256_castsi256_ps(std::get<6>(s)),
            _mm256_castsi256_ps(a), _mm256_castsi256_ps(b)));
    std::get<7>(s) = _mm256_castps_si256(
        _mm256_fmadd_ps(_mm256_castsi256_ps(std::get<7>(s)),
            _mm256_castsi256_ps(a), _mm256_castsi256_ps(b)));
}

MCKL_FLATTEN inline void fmadd_pd(
    std::array<__m256i, 8> &s, const __m256i &a, const __m256i &b)
{
    std::get<0>(s) = _mm256_castpd_si256(
        _mm256_fmadd_pd(_mm256_castsi256_pd(std::get<0>(s)),
            _mm256_castsi256_pd(a), _mm256_castsi256_pd(b)));
    std::get<1>(s) = _mm256_castpd_si256(
        _mm256_fmadd_pd(_mm256_castsi256_pd(std::get<1>(s)),
            _mm256_castsi256_pd(a), _mm256_castsi256_pd(b)));
    std::get<2>(s) = _mm256_castpd_si256(
        _mm256_fmadd_pd(_mm256_castsi256_pd(std::get<2>(s)),
            _mm256_castsi256_pd(a), _mm256_castsi256_pd(b)));
    std::get<3>(s) = _mm256_castpd_si256(
        _mm256_fmadd_pd(_mm256_castsi256_pd(std::get<3>(s)),
            _mm256_castsi256_pd(a), _mm256_castsi256_pd(b)));
    std::get<4>(s) = _mm256_castpd_si256(
        _mm256_fmadd_pd(_mm256_castsi256_pd(std::get<4>(s)),
            _mm256_castsi256_pd(a), _mm256_castsi256_pd(b)));
    std::get<5>(s) = _mm256_castpd_si256(
        _mm256_fmadd_pd(_mm256_castsi256_pd(std::get<5>(s)),
            _mm256_castsi256_pd(a), _mm256_castsi256_pd(b)));
    std::get<6>(s) = _mm256_castpd_si256(
        _mm256_fmadd_pd(_mm256_castsi256_pd(std::get<6>(s)),
            _mm256_castsi256_pd(a), _mm256_castsi256_pd(b)));
    std::get<7>(s) = _mm256_castpd_si256(
        _mm256_fmadd_pd(_mm256_castsi256_pd(std::get<7>(s)),
            _mm256_castsi256_pd(a), _mm256_castsi256_pd(b)));
}

MCKL_FLATTEN inline void fmadd_pd(std::array<__m256i, 8> &s, const __m256i &a,
    const std::array<__m256i, 8> &b)
{
    std::get<0>(s) = _mm256_castpd_si256(
        _mm256_fmadd_pd(_mm256_castsi256_pd(std::get<0>(s)),
            _mm256_castsi256_pd(a), _mm256_castsi256_pd(std::get<0>(b))));
    std::get<1>(s) = _mm256_castpd_si256(
        _mm256_fmadd_pd(_mm256_castsi256_pd(std::get<1>(s)),
            _mm256_castsi256_pd(a), _mm256_castsi256_pd(std::get<1>(b))));
    std::get<2>(s) = _mm256_castpd_si256(
        _mm256_fmadd_pd(_mm256_castsi256_pd(std::get<2>(s)),
            _mm256_castsi256_pd(a), _mm256_castsi256_pd(std::get<2>(b))));
    std::get<3>(s) = _mm256_castpd_si256(
        _mm256_fmadd_pd(_mm256_castsi256_pd(std::get<3>(s)),
            _mm256_castsi256_pd(a), _mm256_castsi256_pd(std::get<3>(b))));
    std::get<4>(s) = _mm256_castpd_si256(
        _mm256_fmadd_pd(_mm256_castsi256_pd(std::get<4>(s)),
            _mm256_castsi256_pd(a), _mm256_castsi256_pd(std::get<4>(b))));
    std::get<5>(s) = _mm256_castpd_si256(
        _mm256_fmadd_pd(_mm256_castsi256_pd(std::get<5>(s)),
            _mm256_castsi256_pd(a), _mm256_castsi256_pd(std::get<5>(b))));
    std::get<6>(s) = _mm256_castpd_si256(
        _mm256_fmadd_pd(_mm256_castsi256_pd(std::get<6>(s)),
            _mm256_castsi256_pd(a), _mm256_castsi256_pd(std::get<6>(b))));
    std::get<7>(s) = _mm256_castpd_si256(
        _mm256_fmadd_pd(_mm256_castsi256_pd(std::get<7>(s)),
            _mm256_castsi256_pd(a), _mm256_castsi256_pd(std::get<7>(b))));
}

MCKL_FLATTEN inline void fmadd_ps(
    std::array<__m256i, 16> &s, const __m256i &a, const __m256i &b)
{
    std::get<0x0>(s) = _mm256_castps_si256(
        _mm256_fmadd_ps(_mm256_castsi256_ps(std::get<0x0>(s)),
            _mm256_castsi256_ps(a), _mm256_castsi256_ps(b)));
    std::get<0x1>(s) = _mm256_castps_si256(
        _mm256_fmadd_ps(_mm256_castsi256_ps(std::get<0x1>(s)),
            _mm256_castsi256_ps(a), _mm256_castsi256_ps(b)));
    std::get<0x2>(s) = _mm256_castps_si256(
        _mm256_fmadd_ps(_mm256_castsi256_ps(std::get<0x2>(s)),
            _mm256_castsi256_ps(a), _mm256_castsi256_ps(b)));
    std::get<0x3>(s) = _mm256_castps_si256(
        _mm256_fmadd_ps(_mm256_castsi256_ps(std::get<0x3>(s)),
            _mm256_castsi256_ps(a), _mm256_castsi256_ps(b)));
    std::get<0x4>(s) = _mm256_castps_si256(
        _mm256_fmadd_ps(_mm256_castsi256_ps(std::get<0x4>(s)),
            _mm256_castsi256_ps(a), _mm256_castsi256_ps(b)));
    std::get<0x5>(s) = _mm256_castps_si256(
        _mm256_fmadd_ps(_mm256_castsi256_ps(std::get<0x5>(s)),
            _mm256_castsi256_ps(a), _mm256_castsi256_ps(b)));
    std::get<0x6>(s) = _mm256_castps_si256(
        _mm256_fmadd_ps(_mm256_castsi256_ps(std::get<0x6>(s)),
            _mm256_castsi256_ps(a), _mm256_castsi256_ps(b)));
    std::get<0x7>(s) = _mm256_castps_si256(
        _mm256_fmadd_ps(_mm256_castsi256_ps(std::get<0x7>(s)),
            _mm256_castsi256_ps(a), _mm256_castsi256_ps(b)));
    std::get<0x8>(s) = _mm256_castps_si256(
        _mm256_fmadd_ps(_mm256_castsi256_ps(std::get<0x8>(s)),
            _mm256_castsi256_ps(a), _mm256_castsi256_ps(b)));
    std::get<0x9>(s) = _mm256_castps_si256(
        _mm256_fmadd_ps(_mm256_castsi256_ps(std::get<0x9>(s)),
            _mm256_castsi256_ps(a), _mm256_castsi256_ps(b)));
    std::get<0xA>(s) = _mm256_castps_si256(
        _mm256_fmadd_ps(_mm256_castsi256_ps(std::get<0xA>(s)),
            _mm256_castsi256_ps(a), _mm256_castsi256_ps(b)));
    std::get<0xB>(s) = _mm256_castps_si256(
        _mm256_fmadd_ps(_mm256_castsi256_ps(std::get<0xB>(s)),
            _mm256_castsi256_ps(a), _mm256_castsi256_ps(b)));
    std::get<0xC>(s) = _mm256_castps_si256(
        _mm256_fmadd_ps(_mm256_castsi256_ps(std::get<0xC>(s)),
            _mm256_castsi256_ps(a), _mm256_castsi256_ps(b)));
    std::get<0xD>(s) = _mm256_castps_si256(
        _mm256_fmadd_ps(_mm256_castsi256_ps(std::get<0xD>(s)),
            _mm256_castsi256_ps(a), _mm256_castsi256_ps(b)));
    std::get<0xE>(s) = _mm256_castps_si256(
        _mm256_fmadd_ps(_mm256_castsi256_ps(std::get<0xE>(s)),
            _mm256_castsi256_ps(a), _mm256_castsi256_ps(b)));
    std::get<0xF>(s) = _mm256_castps_si256(
        _mm256_fmadd_ps(_mm256_castsi256_ps(std::get<0xF>(s)),
            _mm256_castsi256_ps(a), _mm256_castsi256_ps(b)));
}

MCKL_FLATTEN inline void fmadd_pd(
    std::array<__m256i, 16> &s, const __m256i &a, const __m256i &b)
{
    std::get<0x0>(s) = _mm256_castpd_si256(
        _mm256_fmadd_pd(_mm256_castsi256_pd(std::get<0x0>(s)),
            _mm256_castsi256_pd(a), _mm256_castsi256_pd(b)));
    std::get<0x1>(s) = _mm256_castpd_si256(
        _mm256_fmadd_pd(_mm256_castsi256_pd(std::get<0x1>(s)),
            _mm256_castsi256_pd(a), _mm256_castsi256_pd(b)));
    std::get<0x2>(s) = _mm256_castpd_si256(
        _mm256_fmadd_pd(_mm256_castsi256_pd(std::get<0x2>(s)),
            _mm256_castsi256_pd(a), _mm256_castsi256_pd(b)));
    std::get<0x3>(s) = _mm256_castpd_si256(
        _mm256_fmadd_pd(_mm256_castsi256_pd(std::get<0x3>(s)),
            _mm256_castsi256_pd(a), _mm256_castsi256_pd(b)));
    std::get<0x4>(s) = _mm256_castpd_si256(
        _mm256_fmadd_pd(_mm256_castsi256_pd(std::get<0x4>(s)),
            _mm256_castsi256_pd(a), _mm256_castsi256_pd(b)));
    std::get<0x5>(s) = _mm256_castpd_si256(
        _mm256_fmadd_pd(_mm256_castsi256_pd(std::get<0x5>(s)),
            _mm256_castsi256_pd(a), _mm256_castsi256_pd(b)));
    std::get<0x6>(s) = _mm256_castpd_si256(
        _mm256_fmadd_pd(_mm256_castsi256_pd(std::get<0x6>(s)),
            _mm256_castsi256_pd(a), _mm256_castsi256_pd(b)));
    std::get<0x7>(s) = _mm256_castpd_si256(
        _mm256_fmadd_pd(_mm256_castsi256_pd(std::get<0x7>(s)),
            _mm256_castsi256_pd(a), _mm256_castsi256_pd(b)));
    std::get<0x8>(s) = _mm256_castpd_si256(
        _mm256_fmadd_pd(_mm256_castsi256_pd(std::get<0x8>(s)),
            _mm256_castsi256_pd(a), _mm256_castsi256_pd(b)));
    std::get<0x9>(s) = _mm256_castpd_si256(
        _mm256_fmadd_pd(_mm256_castsi256_pd(std::get<0x9>(s)),
            _mm256_castsi256_pd(a), _mm256_castsi256_pd(b)));
    std::get<0xA>(s) = _mm256_castpd_si256(
        _mm256_fmadd_pd(_mm256_castsi256_pd(std::get<0xA>(s)),
            _mm256_castsi256_pd(a), _mm256_castsi256_pd(b)));
    std::get<0xB>(s) = _mm256_castpd_si256(
        _mm256_fmadd_pd(_mm256_castsi256_pd(std::get<0xB>(s)),
            _mm256_castsi256_pd(a), _mm256_castsi256_pd(b)));
    std::get<0xC>(s) = _mm256_castpd_si256(
        _mm256_fmadd_pd(_mm256_castsi256_pd(std::get<0xC>(s)),
            _mm256_castsi256_pd(a), _mm256_castsi256_pd(b)));
    std::get<0xD>(s) = _mm256_castpd_si256(
        _mm256_fmadd_pd(_mm256_castsi256_pd(std::get<0xD>(s)),
            _mm256_castsi256_pd(a), _mm256_castsi256_pd(b)));
    std::get<0xE>(s) = _mm256_castpd_si256(
        _mm256_fmadd_pd(_mm256_castsi256_pd(std::get<0xE>(s)),
            _mm256_castsi256_pd(a), _mm256_castsi256_pd(b)));
    std::get<0xF>(s) = _mm256_castpd_si256(
        _mm256_fmadd_pd(_mm256_castsi256_pd(std::get<0xF>(s)),
            _mm256_castsi256_pd(a), _mm256_castsi256_pd(b)));
}

MCKL_FLATTEN inline void fmadd_pd(std::array<__m256i, 16> &s, const __m256i &a,
    const std::array<__m256i, 16> &b)
{
    std::get<0x0>(s) = _mm256_castpd_si256(
        _mm256_fmadd_pd(_mm256_castsi256_pd(std::get<0x0>(s)),
            _mm256_castsi256_pd(a), _mm256_castsi256_pd(std::get<0x0>(b))));
    std::get<0x1>(s) = _mm256_castpd_si256(
        _mm256_fmadd_pd(_mm256_castsi256_pd(std::get<0x1>(s)),
            _mm256_castsi256_pd(a), _mm256_castsi256_pd(std::get<0x1>(b))));
    std::get<0x2>(s) = _mm256_castpd_si256(
        _mm256_fmadd_pd(_mm256_castsi256_pd(std::get<0x2>(s)),
            _mm256_castsi256_pd(a), _mm256_castsi256_pd(std::get<0x2>(b))));
    std::get<0x3>(s) = _mm256_castpd_si256(
        _mm256_fmadd_pd(_mm256_castsi256_pd(std::get<0x3>(s)),
            _mm256_castsi256_pd(a), _mm256_castsi256_pd(std::get<0x3>(b))));
    std::get<0x4>(s) = _mm256_castpd_si256(
        _mm256_fmadd_pd(_mm256_castsi256_pd(std::get<0x4>(s)),
            _mm256_castsi256_pd(a), _mm256_castsi256_pd(std::get<0x4>(b))));
    std::get<0x5>(s) = _mm256_castpd_si256(
        _mm256_fmadd_pd(_mm256_castsi256_pd(std::get<0x5>(s)),
            _mm256_castsi256_pd(a), _mm256_castsi256_pd(std::get<0x5>(b))));
    std::get<0x6>(s) = _mm256_castpd_si256(
        _mm256_fmadd_pd(_mm256_castsi256_pd(std::get<0x6>(s)),
            _mm256_castsi256_pd(a), _mm256_castsi256_pd(std::get<0x6>(b))));
    std::get<0x7>(s) = _mm256_castpd_si256(
        _mm256_fmadd_pd(_mm256_castsi256_pd(std::get<0x7>(s)),
            _mm256_castsi256_pd(a), _mm256_castsi256_pd(std::get<0x7>(b))));
    std::get<0x8>(s) = _mm256_castpd_si256(
        _mm256_fmadd_pd(_mm256_castsi256_pd(std::get<0x8>(s)),
            _mm256_castsi256_pd(a), _mm256_castsi256_pd(std::get<0x8>(b))));
    std::get<0x9>(s) = _mm256_castpd_si256(
        _mm256_fmadd_pd(_mm256_castsi256_pd(std::get<0x9>(s)),
            _mm256_castsi256_pd(a), _mm256_castsi256_pd(std::get<0x9>(b))));
    std::get<0xA>(s) = _mm256_castpd_si256(
        _mm256_fmadd_pd(_mm256_castsi256_pd(std::get<0xA>(s)),
            _mm256_castsi256_pd(a), _mm256_castsi256_pd(std::get<0xA>(b))));
    std::get<0xB>(s) = _mm256_castpd_si256(
        _mm256_fmadd_pd(_mm256_castsi256_pd(std::get<0xB>(s)),
            _mm256_castsi256_pd(a), _mm256_castsi256_pd(std::get<0xB>(b))));
    std::get<0xC>(s) = _mm256_castpd_si256(
        _mm256_fmadd_pd(_mm256_castsi256_pd(std::get<0xC>(s)),
            _mm256_castsi256_pd(a), _mm256_castsi256_pd(std::get<0xC>(b))));
    std::get<0xD>(s) = _mm256_castpd_si256(
        _mm256_fmadd_pd(_mm256_castsi256_pd(std::get<0xD>(s)),
            _mm256_castsi256_pd(a), _mm256_castsi256_pd(std::get<0xD>(b))));
    std::get<0xE>(s) = _mm256_castpd_si256(
        _mm256_fmadd_pd(_mm256_castsi256_pd(std::get<0xE>(s)),
            _mm256_castsi256_pd(a), _mm256_castsi256_pd(std::get<0xE>(b))));
    std::get<0xF>(s) = _mm256_castpd_si256(
        _mm256_fmadd_pd(_mm256_castsi256_pd(std::get<0xF>(s)),
            _mm256_castsi256_pd(a), _mm256_castsi256_pd(std::get<0xF>(b))));
}

} // namespace mckl::internal

} // namespace mckl

#ifdef MCKL_GCC
#if MCKL_GCC_VERSION >= 60000
#pragma GCC diagnostic pop
#endif
#endif

#endif // MCKL_INTERNAL_FMA_HPP
