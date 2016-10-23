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

#define MCKL_LOAD_SI128_1(s, sptr) std::get<0>(s) = _mm_load_si128(sptr + 0);

#define MCKL_LOAD_SI128_2(s, sptr)                                            \
    std::get<0>(s) = _mm_load_si128(sptr + 0);                                \
    std::get<1>(s) = _mm_load_si128(sptr + 1);

#define MCKL_LOAD_SI128_4(s, sptr)                                            \
    std::get<0>(s) = _mm_load_si128(sptr + 0);                                \
    std::get<1>(s) = _mm_load_si128(sptr + 1);                                \
    std::get<2>(s) = _mm_load_si128(sptr + 2);                                \
    std::get<3>(s) = _mm_load_si128(sptr + 3);

#define MCKL_LOAD_SI128_8(s, sptr)                                            \
    std::get<0>(s) = _mm_load_si128(sptr + 0);                                \
    std::get<1>(s) = _mm_load_si128(sptr + 1);                                \
    std::get<2>(s) = _mm_load_si128(sptr + 2);                                \
    std::get<3>(s) = _mm_load_si128(sptr + 3);                                \
    std::get<4>(s) = _mm_load_si128(sptr + 4);                                \
    std::get<5>(s) = _mm_load_si128(sptr + 5);                                \
    std::get<6>(s) = _mm_load_si128(sptr + 6);                                \
    std::get<7>(s) = _mm_load_si128(sptr + 7);

#define MCKL_LOAD_SI128_16(s, sptr)                                           \
    std::get<0x0>(s) = _mm_load_si128(sptr + 0x0);                            \
    std::get<0x1>(s) = _mm_load_si128(sptr + 0x1);                            \
    std::get<0x2>(s) = _mm_load_si128(sptr + 0x2);                            \
    std::get<0x3>(s) = _mm_load_si128(sptr + 0x3);                            \
    std::get<0x4>(s) = _mm_load_si128(sptr + 0x4);                            \
    std::get<0x5>(s) = _mm_load_si128(sptr + 0x5);                            \
    std::get<0x6>(s) = _mm_load_si128(sptr + 0x6);                            \
    std::get<0x7>(s) = _mm_load_si128(sptr + 0x7);                            \
    std::get<0x8>(s) = _mm_load_si128(sptr + 0x8);                            \
    std::get<0x9>(s) = _mm_load_si128(sptr + 0x9);                            \
    std::get<0xA>(s) = _mm_load_si128(sptr + 0xA);                            \
    std::get<0xB>(s) = _mm_load_si128(sptr + 0xB);                            \
    std::get<0xC>(s) = _mm_load_si128(sptr + 0xC);                            \
    std::get<0xD>(s) = _mm_load_si128(sptr + 0xD);                            \
    std::get<0xE>(s) = _mm_load_si128(sptr + 0xE);                            \
    std::get<0xF>(s) = _mm_load_si128(sptr + 0xF);

#define MCKL_STORE_SI128_1(s, sptr) _mm_store_si128(sptr + 0, std::get<0>(s));

#define MCKL_STORE_SI128_2(s, sptr)                                           \
    _mm_store_si128(sptr + 0, std::get<0>(s));                                \
    _mm_store_si128(sptr + 1, std::get<1>(s));

#define MCKL_STORE_SI128_4(s, sptr)                                           \
    _mm_store_si128(sptr + 0, std::get<0>(s));                                \
    _mm_store_si128(sptr + 1, std::get<1>(s));                                \
    _mm_store_si128(sptr + 2, std::get<2>(s));                                \
    _mm_store_si128(sptr + 3, std::get<3>(s));

#define MCKL_STORE_SI128_8(s, sptr)                                           \
    _mm_store_si128(sptr + 0, std::get<0>(s));                                \
    _mm_store_si128(sptr + 1, std::get<1>(s));                                \
    _mm_store_si128(sptr + 2, std::get<2>(s));                                \
    _mm_store_si128(sptr + 3, std::get<3>(s));                                \
    _mm_store_si128(sptr + 4, std::get<4>(s));                                \
    _mm_store_si128(sptr + 5, std::get<5>(s));                                \
    _mm_store_si128(sptr + 6, std::get<6>(s));                                \
    _mm_store_si128(sptr + 7, std::get<7>(s));

#define MCKL_STORE_SI128_16(s, sptr)                                          \
    _mm_store_si128(sptr + 0x0, std::get<0x0>(s));                            \
    _mm_store_si128(sptr + 0x1, std::get<0x1>(s));                            \
    _mm_store_si128(sptr + 0x2, std::get<0x2>(s));                            \
    _mm_store_si128(sptr + 0x3, std::get<0x3>(s));                            \
    _mm_store_si128(sptr + 0x4, std::get<0x4>(s));                            \
    _mm_store_si128(sptr + 0x5, std::get<0x5>(s));                            \
    _mm_store_si128(sptr + 0x6, std::get<0x6>(s));                            \
    _mm_store_si128(sptr + 0x7, std::get<0x7>(s));                            \
    _mm_store_si128(sptr + 0x8, std::get<0x8>(s));                            \
    _mm_store_si128(sptr + 0x9, std::get<0x9>(s));                            \
    _mm_store_si128(sptr + 0xA, std::get<0xA>(s));                            \
    _mm_store_si128(sptr + 0xB, std::get<0xB>(s));                            \
    _mm_store_si128(sptr + 0xC, std::get<0xC>(s));                            \
    _mm_store_si128(sptr + 0xD, std::get<0xD>(s));                            \
    _mm_store_si128(sptr + 0xE, std::get<0xE>(s));                            \
    _mm_store_si128(sptr + 0xF, std::get<0xF>(s));

#define MCKL_TRANSPOSE4X32_SI128(s, t, I0, I1, I2, I3)                        \
    std::get<0>(t) = _mm_unpacklo_epi32(std::get<I0>(s), std::get<I1>(s));    \
    std::get<1>(t) = _mm_unpacklo_epi32(std::get<I2>(s), std::get<I3>(s));    \
    std::get<2>(t) = _mm_unpackhi_epi32(std::get<I0>(s), std::get<I1>(s));    \
    std::get<3>(t) = _mm_unpackhi_epi32(std::get<I2>(s), std::get<I3>(s));    \
    std::get<I0>(s) = _mm_unpacklo_epi64(std::get<0>(t), std::get<1>(t));     \
    std::get<I1>(s) = _mm_unpackhi_epi64(std::get<0>(t), std::get<1>(t));     \
    std::get<I2>(s) = _mm_unpacklo_epi64(std::get<2>(t), std::get<3>(t));     \
    std::get<I3>(s) = _mm_unpackhi_epi64(std::get<2>(t), std::get<3>(t));

#define MCKL_TRANSPOSE4X32_LOAD_SI128_4(s, t, sptr)                           \
    std::get<0>(s) = _mm_load_si128(sptr + 0);                                \
    std::get<1>(s) = _mm_load_si128(sptr + 1);                                \
    std::get<2>(s) = _mm_load_si128(sptr + 2);                                \
    std::get<3>(s) = _mm_load_si128(sptr + 3);                                \
    MCKL_TRANSPOSE4X32_SI128(s, t, 0, 1, 2, 3);

#define MCKL_TRANSPOSE4X32_LOAD_SI128_8(s, t, sptr)                           \
    std::get<0>(s) = _mm_load_si128(sptr + 0);                                \
    std::get<4>(s) = _mm_load_si128(sptr + 1);                                \
    std::get<1>(s) = _mm_load_si128(sptr + 2);                                \
    std::get<5>(s) = _mm_load_si128(sptr + 3);                                \
    std::get<2>(s) = _mm_load_si128(sptr + 4);                                \
    std::get<6>(s) = _mm_load_si128(sptr + 5);                                \
    std::get<3>(s) = _mm_load_si128(sptr + 6);                                \
    std::get<7>(s) = _mm_load_si128(sptr + 7);                                \
    MCKL_TRANSPOSE4X32_SI128(s, t, 0, 1, 2, 3);                               \
    MCKL_TRANSPOSE4X32_SI128(s, t, 4, 5, 6, 7);

#define MCKL_TRANSPOSE4X32_LOAD_SI128_16(s, t, sptr)                          \
    std::get<0x0>(s) = _mm_load_si128(sptr + 0x0);                            \
    std::get<0x4>(s) = _mm_load_si128(sptr + 0x1);                            \
    std::get<0x8>(s) = _mm_load_si128(sptr + 0x2);                            \
    std::get<0xC>(s) = _mm_load_si128(sptr + 0x3);                            \
    std::get<0x1>(s) = _mm_load_si128(sptr + 0x4);                            \
    std::get<0x5>(s) = _mm_load_si128(sptr + 0x5);                            \
    std::get<0x9>(s) = _mm_load_si128(sptr + 0x6);                            \
    std::get<0xD>(s) = _mm_load_si128(sptr + 0x7);                            \
    std::get<0x2>(s) = _mm_load_si128(sptr + 0x8);                            \
    std::get<0x6>(s) = _mm_load_si128(sptr + 0x9);                            \
    std::get<0xA>(s) = _mm_load_si128(sptr + 0xA);                            \
    std::get<0xE>(s) = _mm_load_si128(sptr + 0xB);                            \
    std::get<0x3>(s) = _mm_load_si128(sptr + 0xC);                            \
    std::get<0x7>(s) = _mm_load_si128(sptr + 0xD);                            \
    std::get<0xB>(s) = _mm_load_si128(sptr + 0xE);                            \
    std::get<0xF>(s) = _mm_load_si128(sptr + 0xF);                            \
    MCKL_TRANSPOSE4X32_SI128(s, t, 0x0, 0x1, 0x2, 0x3);                       \
    MCKL_TRANSPOSE4X32_SI128(s, t, 0x4, 0x5, 0x6, 0x7);                       \
    MCKL_TRANSPOSE4X32_SI128(s, t, 0x8, 0x9, 0xA, 0xB);                       \
    MCKL_TRANSPOSE4X32_SI128(s, t, 0xC, 0xD, 0xE, 0xF);

#define MCKL_TRANSPOSE4X32_STORE_SI128_4(s, t, sptr)                          \
    MCKL_TRANSPOSE4X32_SI128(s, t, 0, 1, 2, 3);                               \
    _mm_store_si128(sptr + 0, std::get<0>(s));                                \
    _mm_store_si128(sptr + 1, std::get<1>(s));                                \
    _mm_store_si128(sptr + 2, std::get<2>(s));                                \
    _mm_store_si128(sptr + 3, std::get<3>(s));

#define MCKL_TRANSPOSE4X32_STORE_SI128_8(s, t, sptr)                          \
    MCKL_TRANSPOSE4X32_SI128(s, t, 0, 1, 2, 3);                               \
    MCKL_TRANSPOSE4X32_SI128(s, t, 4, 5, 6, 7);                               \
    _mm_store_si128(sptr + 0, std::get<0>(s));                                \
    _mm_store_si128(sptr + 1, std::get<4>(s));                                \
    _mm_store_si128(sptr + 2, std::get<1>(s));                                \
    _mm_store_si128(sptr + 3, std::get<5>(s));                                \
    _mm_store_si128(sptr + 4, std::get<2>(s));                                \
    _mm_store_si128(sptr + 5, std::get<6>(s));                                \
    _mm_store_si128(sptr + 6, std::get<3>(s));                                \
    _mm_store_si128(sptr + 7, std::get<7>(s));

#define MCKL_TRANSPOSE4X32_STORE_SI128_16(s, t, sptr)                         \
    MCKL_TRANSPOSE4X32_SI128(s, t, 0x0, 0x1, 0x2, 0x3);                       \
    MCKL_TRANSPOSE4X32_SI128(s, t, 0x4, 0x5, 0x6, 0x7);                       \
    MCKL_TRANSPOSE4X32_SI128(s, t, 0x8, 0x9, 0xA, 0xB);                       \
    MCKL_TRANSPOSE4X32_SI128(s, t, 0xC, 0xD, 0xE, 0xF);                       \
    _mm_store_si128(sptr + 0x0, std::get<0x0>(s));                            \
    _mm_store_si128(sptr + 0x1, std::get<0x4>(s));                            \
    _mm_store_si128(sptr + 0x2, std::get<0x8>(s));                            \
    _mm_store_si128(sptr + 0x3, std::get<0xC>(s));                            \
    _mm_store_si128(sptr + 0x4, std::get<0x1>(s));                            \
    _mm_store_si128(sptr + 0x5, std::get<0x5>(s));                            \
    _mm_store_si128(sptr + 0x6, std::get<0x9>(s));                            \
    _mm_store_si128(sptr + 0x7, std::get<0xD>(s));                            \
    _mm_store_si128(sptr + 0x8, std::get<0x2>(s));                            \
    _mm_store_si128(sptr + 0x9, std::get<0x6>(s));                            \
    _mm_store_si128(sptr + 0xA, std::get<0xA>(s));                            \
    _mm_store_si128(sptr + 0xB, std::get<0xE>(s));                            \
    _mm_store_si128(sptr + 0xC, std::get<0x3>(s));                            \
    _mm_store_si128(sptr + 0xD, std::get<0x7>(s));                            \
    _mm_store_si128(sptr + 0xE, std::get<0xB>(s));                            \
    _mm_store_si128(sptr + 0xF, std::get<0xF>(s));

#define MCKL_TRANSPOSE2X64_SI128(s, t, I0, I1)                                \
    std::get<0>(t) = _mm_unpacklo_epi64(std::get<I0>(s), std::get<I1>(s));    \
    std::get<1>(t) = _mm_unpackhi_epi64(std::get<I0>(s), std::get<I1>(s));    \
    std::get<I0>(s) = std::get<0>(t);                                         \
    std::get<I1>(s) = std::get<1>(t);

#define MCKL_TRANSPOSE2X64_LOAD_SI128_2(s, t, sptr)                           \
    std::get<0>(s) = _mm_load_si128(sptr + 0);                                \
    std::get<1>(s) = _mm_load_si128(sptr + 1);                                \
    MCKL_TRANSPOSE2X64_SI128(s, t, 0, 1);

#define MCKL_TRANSPOSE2X64_LOAD_SI128_4(s, t, sptr)                           \
    std::get<0>(s) = _mm_load_si128(sptr + 0);                                \
    std::get<2>(s) = _mm_load_si128(sptr + 1);                                \
    std::get<1>(s) = _mm_load_si128(sptr + 2);                                \
    std::get<3>(s) = _mm_load_si128(sptr + 3);                                \
    MCKL_TRANSPOSE2X64_SI128(s, t, 0, 1);                                     \
    MCKL_TRANSPOSE2X64_SI128(s, t, 2, 3);

#define MCKL_TRANSPOSE2X64_LOAD_SI128_8(s, t, sptr)                           \
    std::get<0>(s) = _mm_load_si128(sptr + 0);                                \
    std::get<2>(s) = _mm_load_si128(sptr + 1);                                \
    std::get<4>(s) = _mm_load_si128(sptr + 2);                                \
    std::get<6>(s) = _mm_load_si128(sptr + 3);                                \
    std::get<1>(s) = _mm_load_si128(sptr + 4);                                \
    std::get<3>(s) = _mm_load_si128(sptr + 5);                                \
    std::get<5>(s) = _mm_load_si128(sptr + 6);                                \
    std::get<7>(s) = _mm_load_si128(sptr + 7);                                \
    MCKL_TRANSPOSE2X64_SI128(s, t, 0, 1);                                     \
    MCKL_TRANSPOSE2X64_SI128(s, t, 2, 3);                                     \
    MCKL_TRANSPOSE2X64_SI128(s, t, 4, 5);                                     \
    MCKL_TRANSPOSE2X64_SI128(s, t, 6, 7);

#define MCKL_TRANSPOSE2X64_LOAD_SI128_16(s, t, sptr)                          \
    std::get<0x0>(s) = _mm_load_si128(sptr + 0x0);                            \
    std::get<0x2>(s) = _mm_load_si128(sptr + 0x1);                            \
    std::get<0x4>(s) = _mm_load_si128(sptr + 0x2);                            \
    std::get<0x6>(s) = _mm_load_si128(sptr + 0x3);                            \
    std::get<0x8>(s) = _mm_load_si128(sptr + 0x4);                            \
    std::get<0xA>(s) = _mm_load_si128(sptr + 0x5);                            \
    std::get<0xC>(s) = _mm_load_si128(sptr + 0x6);                            \
    std::get<0xE>(s) = _mm_load_si128(sptr + 0x7);                            \
    std::get<0x1>(s) = _mm_load_si128(sptr + 0x8);                            \
    std::get<0x3>(s) = _mm_load_si128(sptr + 0x9);                            \
    std::get<0x5>(s) = _mm_load_si128(sptr + 0xA);                            \
    std::get<0x7>(s) = _mm_load_si128(sptr + 0xB);                            \
    std::get<0x9>(s) = _mm_load_si128(sptr + 0xC);                            \
    std::get<0xB>(s) = _mm_load_si128(sptr + 0xD);                            \
    std::get<0xD>(s) = _mm_load_si128(sptr + 0xE);                            \
    std::get<0xF>(s) = _mm_load_si128(sptr + 0xF);                            \
    MCKL_TRANSPOSE2X64_SI128(s, t, 0x0, 0x1);                                 \
    MCKL_TRANSPOSE2X64_SI128(s, t, 0x2, 0x3);                                 \
    MCKL_TRANSPOSE2X64_SI128(s, t, 0x4, 0x5);                                 \
    MCKL_TRANSPOSE2X64_SI128(s, t, 0x6, 0x7);                                 \
    MCKL_TRANSPOSE2X64_SI128(s, t, 0x8, 0x9);                                 \
    MCKL_TRANSPOSE2X64_SI128(s, t, 0xA, 0xB);                                 \
    MCKL_TRANSPOSE2X64_SI128(s, t, 0xC, 0xD);                                 \
    MCKL_TRANSPOSE2X64_SI128(s, t, 0xE, 0xF);

#define MCKL_TRANSPOSE2X64_STORE_SI128_2(s, t, sptr)                          \
    MCKL_TRANSPOSE2X64_SI128(s, t, 0, 1);                                     \
    _mm_store_si128(sptr + 0, std::get<0>(s));                                \
    _mm_store_si128(sptr + 1, std::get<1>(s));

#define MCKL_TRANSPOSE2X64_STORE_SI128_4(s, t, sptr)                          \
    MCKL_TRANSPOSE2X64_SI128(s, t, 0, 1);                                     \
    MCKL_TRANSPOSE2X64_SI128(s, t, 2, 3);                                     \
    _mm_store_si128(sptr + 0, std::get<0>(s));                                \
    _mm_store_si128(sptr + 1, std::get<2>(s));                                \
    _mm_store_si128(sptr + 2, std::get<1>(s));                                \
    _mm_store_si128(sptr + 3, std::get<3>(s));

#define MCKL_TRANSPOSE2X64_STORE_SI128_8(s, t, sptr)                          \
    MCKL_TRANSPOSE2X64_SI128(s, t, 0, 1);                                     \
    MCKL_TRANSPOSE2X64_SI128(s, t, 2, 3);                                     \
    MCKL_TRANSPOSE2X64_SI128(s, t, 4, 5);                                     \
    MCKL_TRANSPOSE2X64_SI128(s, t, 6, 7);                                     \
    _mm_store_si128(sptr + 0, std::get<0>(s));                                \
    _mm_store_si128(sptr + 1, std::get<2>(s));                                \
    _mm_store_si128(sptr + 2, std::get<4>(s));                                \
    _mm_store_si128(sptr + 3, std::get<6>(s));                                \
    _mm_store_si128(sptr + 4, std::get<1>(s));                                \
    _mm_store_si128(sptr + 5, std::get<3>(s));                                \
    _mm_store_si128(sptr + 6, std::get<5>(s));                                \
    _mm_store_si128(sptr + 7, std::get<7>(s));

#define MCKL_TRANSPOSE2X64_STORE_SI128_16(s, t, sptr)                         \
    MCKL_TRANSPOSE2X64_SI128(s, t, 0x0, 0x1);                                 \
    MCKL_TRANSPOSE2X64_SI128(s, t, 0x2, 0x3);                                 \
    MCKL_TRANSPOSE2X64_SI128(s, t, 0x4, 0x5);                                 \
    MCKL_TRANSPOSE2X64_SI128(s, t, 0x6, 0x7);                                 \
    MCKL_TRANSPOSE2X64_SI128(s, t, 0x8, 0x9);                                 \
    MCKL_TRANSPOSE2X64_SI128(s, t, 0xA, 0xB);                                 \
    MCKL_TRANSPOSE2X64_SI128(s, t, 0xC, 0xD);                                 \
    MCKL_TRANSPOSE2X64_SI128(s, t, 0xE, 0xF);                                 \
    _mm_store_si128(sptr + 0x0, std::get<0x0>(s));                            \
    _mm_store_si128(sptr + 0x1, std::get<0x2>(s));                            \
    _mm_store_si128(sptr + 0x2, std::get<0x4>(s));                            \
    _mm_store_si128(sptr + 0x3, std::get<0x6>(s));                            \
    _mm_store_si128(sptr + 0x4, std::get<0x8>(s));                            \
    _mm_store_si128(sptr + 0x5, std::get<0xA>(s));                            \
    _mm_store_si128(sptr + 0x6, std::get<0xC>(s));                            \
    _mm_store_si128(sptr + 0x7, std::get<0xE>(s));                            \
    _mm_store_si128(sptr + 0x8, std::get<0x1>(s));                            \
    _mm_store_si128(sptr + 0x9, std::get<0x3>(s));                            \
    _mm_store_si128(sptr + 0xA, std::get<0x5>(s));                            \
    _mm_store_si128(sptr + 0xB, std::get<0x7>(s));                            \
    _mm_store_si128(sptr + 0xC, std::get<0x9>(s));                            \
    _mm_store_si128(sptr + 0xD, std::get<0xB>(s));                            \
    _mm_store_si128(sptr + 0xE, std::get<0xD>(s));                            \
    _mm_store_si128(sptr + 0xF, std::get<0xF>(s));

#endif // MCKL_INTERNAL_SSE2_HPP
