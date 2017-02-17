//============================================================================
// MCKL/include/mckl/random/internal/u01_avx2.hpp
//----------------------------------------------------------------------------
// MCKL: Monte Carlo Kernel Library
//----------------------------------------------------------------------------
// Copyright (c) 2013-2017, Yan Zhou
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

#ifndef MCKL_RANDOM_INTERNAL_U01_AVX2_HPP
#define MCKL_RANDOM_INTERNAL_U01_AVX2_HPP

#include <mckl/random/internal/common.hpp>
#include <mckl/random/internal/u01_generic.hpp>

#ifdef MCKL_GCC
#if MCKL_GCC_VERSION >= 60000
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wignored-attributes"
#endif
#endif

#define MCKL_RANDOM_INTERNAL_U01_AVX2_LOADU_SI256(u)                          \
    const __m256i *uptr = reinterpret_cast<const __m256i *>(u);               \
    __m256i ymm0 = _mm256_loadu_si256(uptr++);                                \
    __m256i ymm1 = _mm256_loadu_si256(uptr++);                                \
    __m256i ymm2 = _mm256_loadu_si256(uptr++);                                \
    __m256i ymm3 = _mm256_loadu_si256(uptr++);                                \
    __m256i ymm4 = _mm256_loadu_si256(uptr++);                                \
    __m256i ymm5 = _mm256_loadu_si256(uptr++);                                \
    __m256i ymm6 = _mm256_loadu_si256(uptr++);                                \
    __m256i ymm7 = _mm256_loadu_si256(uptr++);

#define MCKL_RANDOM_INTERNAL_U01_AVX2_LOADU_SI256_CVTEPU32_EPI64(u)           \
    const __m128i *uptr = reinterpret_cast<const __m128i *>(u);               \
    __m256i ymm0 = _mm256_cvtepu32_epi64(_mm_loadu_si128(uptr++));            \
    __m256i ymm1 = _mm256_cvtepu32_epi64(_mm_loadu_si128(uptr++));            \
    __m256i ymm2 = _mm256_cvtepu32_epi64(_mm_loadu_si128(uptr++));            \
    __m256i ymm3 = _mm256_cvtepu32_epi64(_mm_loadu_si128(uptr++));            \
    __m256i ymm4 = _mm256_cvtepu32_epi64(_mm_loadu_si128(uptr++));            \
    __m256i ymm5 = _mm256_cvtepu32_epi64(_mm_loadu_si128(uptr++));            \
    __m256i ymm6 = _mm256_cvtepu32_epi64(_mm_loadu_si128(uptr++));            \
    __m256i ymm7 = _mm256_cvtepu32_epi64(_mm_loadu_si128(uptr++));

#define MCKL_RANDOM_INTERNAL_U01_AVX2_STOREU_PS(r)                            \
    _mm256_storeu_ps(r + 0x00, _mm256_castsi256_ps(ymm0));                    \
    _mm256_storeu_ps(r + 0x08, _mm256_castsi256_ps(ymm1));                    \
    _mm256_storeu_ps(r + 0x10, _mm256_castsi256_ps(ymm2));                    \
    _mm256_storeu_ps(r + 0x18, _mm256_castsi256_ps(ymm3));                    \
    _mm256_storeu_ps(r + 0x20, _mm256_castsi256_ps(ymm4));                    \
    _mm256_storeu_ps(r + 0x28, _mm256_castsi256_ps(ymm5));                    \
    _mm256_storeu_ps(r + 0x30, _mm256_castsi256_ps(ymm6));                    \
    _mm256_storeu_ps(r + 0x38, _mm256_castsi256_ps(ymm7));

#define MCKL_RANDOM_INTERNAL_U01_AVX2_STOREU_PD(r)                            \
    _mm256_storeu_pd(r + 0x00, _mm256_castsi256_pd(ymm0));                    \
    _mm256_storeu_pd(r + 0x04, _mm256_castsi256_pd(ymm1));                    \
    _mm256_storeu_pd(r + 0x08, _mm256_castsi256_pd(ymm2));                    \
    _mm256_storeu_pd(r + 0x0C, _mm256_castsi256_pd(ymm3));                    \
    _mm256_storeu_pd(r + 0x10, _mm256_castsi256_pd(ymm4));                    \
    _mm256_storeu_pd(r + 0x14, _mm256_castsi256_pd(ymm5));                    \
    _mm256_storeu_pd(r + 0x18, _mm256_castsi256_pd(ymm6));                    \
    _mm256_storeu_pd(r + 0x1C, _mm256_castsi256_pd(ymm7));

#define MCKL_RANDOM_INTERNAL_U01_AVX2_AND1ADD_EPI32                           \
    const __m256i mask = _mm256_set1_epi32(1);                                \
    ymm0 = _mm256_add_epi32(ymm0, _mm256_and_si256(ymm0, mask));              \
    ymm1 = _mm256_add_epi32(ymm1, _mm256_and_si256(ymm1, mask));              \
    ymm2 = _mm256_add_epi32(ymm2, _mm256_and_si256(ymm2, mask));              \
    ymm3 = _mm256_add_epi32(ymm3, _mm256_and_si256(ymm3, mask));              \
    ymm4 = _mm256_add_epi32(ymm4, _mm256_and_si256(ymm4, mask));              \
    ymm5 = _mm256_add_epi32(ymm5, _mm256_and_si256(ymm5, mask));              \
    ymm6 = _mm256_add_epi32(ymm6, _mm256_and_si256(ymm6, mask));              \
    ymm7 = _mm256_add_epi32(ymm7, _mm256_and_si256(ymm7, mask));

#define MCKL_RANDOM_INTERNAL_U01_AVX2_AND1ADD_EPI64                           \
    const __m256i mask = _mm256_set1_epi64x(1);                               \
    ymm0 = _mm256_add_epi64(ymm0, _mm256_and_si256(ymm0, mask));              \
    ymm1 = _mm256_add_epi64(ymm1, _mm256_and_si256(ymm1, mask));              \
    ymm2 = _mm256_add_epi64(ymm2, _mm256_and_si256(ymm2, mask));              \
    ymm3 = _mm256_add_epi64(ymm3, _mm256_and_si256(ymm3, mask));              \
    ymm4 = _mm256_add_epi64(ymm4, _mm256_and_si256(ymm4, mask));              \
    ymm5 = _mm256_add_epi64(ymm5, _mm256_and_si256(ymm5, mask));              \
    ymm6 = _mm256_add_epi64(ymm6, _mm256_and_si256(ymm6, mask));              \
    ymm7 = _mm256_add_epi64(ymm7, _mm256_and_si256(ymm7, mask));

#define MCKL_RANDOM_INTERNAL_U01_AVX2_SLLI_EPI32(imm8)                        \
    ymm0 = _mm256_slli_epi32(ymm0, imm8);                                     \
    ymm1 = _mm256_slli_epi32(ymm1, imm8);                                     \
    ymm2 = _mm256_slli_epi32(ymm2, imm8);                                     \
    ymm3 = _mm256_slli_epi32(ymm3, imm8);                                     \
    ymm4 = _mm256_slli_epi32(ymm4, imm8);                                     \
    ymm5 = _mm256_slli_epi32(ymm5, imm8);                                     \
    ymm6 = _mm256_slli_epi32(ymm6, imm8);                                     \
    ymm7 = _mm256_slli_epi32(ymm7, imm8);

#define MCKL_RANDOM_INTERNAL_U01_AVX2_SRLI_EPI32(imm8)                        \
    ymm0 = _mm256_srli_epi32(ymm0, imm8);                                     \
    ymm1 = _mm256_srli_epi32(ymm1, imm8);                                     \
    ymm2 = _mm256_srli_epi32(ymm2, imm8);                                     \
    ymm3 = _mm256_srli_epi32(ymm3, imm8);                                     \
    ymm4 = _mm256_srli_epi32(ymm4, imm8);                                     \
    ymm5 = _mm256_srli_epi32(ymm5, imm8);                                     \
    ymm6 = _mm256_srli_epi32(ymm6, imm8);                                     \
    ymm7 = _mm256_srli_epi32(ymm7, imm8);

#define MCKL_RANDOM_INTERNAL_U01_AVX2_SLLI_EPI64(imm8)                        \
    ymm0 = _mm256_slli_epi64(ymm0, imm8);                                     \
    ymm1 = _mm256_slli_epi64(ymm1, imm8);                                     \
    ymm2 = _mm256_slli_epi64(ymm2, imm8);                                     \
    ymm3 = _mm256_slli_epi64(ymm3, imm8);                                     \
    ymm4 = _mm256_slli_epi64(ymm4, imm8);                                     \
    ymm5 = _mm256_slli_epi64(ymm5, imm8);                                     \
    ymm6 = _mm256_slli_epi64(ymm6, imm8);                                     \
    ymm7 = _mm256_slli_epi64(ymm7, imm8);

#define MCKL_RANDOM_INTERNAL_U01_AVX2_SRLI_EPI64(imm8)                        \
    ymm0 = _mm256_srli_epi64(ymm0, imm8);                                     \
    ymm1 = _mm256_srli_epi64(ymm1, imm8);                                     \
    ymm2 = _mm256_srli_epi64(ymm2, imm8);                                     \
    ymm3 = _mm256_srli_epi64(ymm3, imm8);                                     \
    ymm4 = _mm256_srli_epi64(ymm4, imm8);                                     \
    ymm5 = _mm256_srli_epi64(ymm5, imm8);                                     \
    ymm6 = _mm256_srli_epi64(ymm6, imm8);                                     \
    ymm7 = _mm256_srli_epi64(ymm7, imm8);

#define MCKL_RANDOM_INTERNAL_U01_AVX2_CVTEPU32_PS_31                          \
    ymm0 = _mm256_castps_si256(_mm256_cvtepi32_ps(ymm0));                     \
    ymm1 = _mm256_castps_si256(_mm256_cvtepi32_ps(ymm1));                     \
    ymm2 = _mm256_castps_si256(_mm256_cvtepi32_ps(ymm2));                     \
    ymm3 = _mm256_castps_si256(_mm256_cvtepi32_ps(ymm3));                     \
    ymm4 = _mm256_castps_si256(_mm256_cvtepi32_ps(ymm4));                     \
    ymm5 = _mm256_castps_si256(_mm256_cvtepi32_ps(ymm5));                     \
    ymm6 = _mm256_castps_si256(_mm256_cvtepi32_ps(ymm6));                     \
    ymm7 = _mm256_castps_si256(_mm256_cvtepi32_ps(ymm7));

#if MCKL_USE_FMA

#define MCKL_RANDOM_INTERNAL_U01_AVX2_CVTEPU32_PS                             \
    const __m256 c = _mm256_set1_ps(Pow2<float, 23>::value);                  \
    const __m256 d = _mm256_set1_ps(Pow2<float, 16>::value);                  \
    const __m256i l = _mm256_set1_epi32(static_cast<int>(0xFFFF));            \
    __m256i zmm0 = _mm256_and_si256(ymm0, l);                                 \
    __m256i zmm1 = _mm256_and_si256(ymm1, l);                                 \
    __m256i zmm2 = _mm256_and_si256(ymm2, l);                                 \
    __m256i zmm3 = _mm256_and_si256(ymm3, l);                                 \
    __m256i zmm4 = _mm256_and_si256(ymm4, l);                                 \
    __m256i zmm5 = _mm256_and_si256(ymm5, l);                                 \
    __m256i zmm6 = _mm256_and_si256(ymm6, l);                                 \
    __m256i zmm7 = _mm256_and_si256(ymm7, l);                                 \
    MCKL_RANDOM_INTERNAL_U01_AVX2_SRLI_EPI32(16);                             \
    ymm0 = _mm256_add_epi32(ymm0, _mm256_castps_si256(c));                    \
    ymm1 = _mm256_add_epi32(ymm1, _mm256_castps_si256(c));                    \
    ymm2 = _mm256_add_epi32(ymm2, _mm256_castps_si256(c));                    \
    ymm3 = _mm256_add_epi32(ymm3, _mm256_castps_si256(c));                    \
    ymm4 = _mm256_add_epi32(ymm4, _mm256_castps_si256(c));                    \
    ymm5 = _mm256_add_epi32(ymm5, _mm256_castps_si256(c));                    \
    ymm6 = _mm256_add_epi32(ymm6, _mm256_castps_si256(c));                    \
    ymm7 = _mm256_add_epi32(ymm7, _mm256_castps_si256(c));                    \
    zmm0 = _mm256_add_epi32(zmm0, _mm256_castps_si256(c));                    \
    zmm1 = _mm256_add_epi32(zmm1, _mm256_castps_si256(c));                    \
    zmm2 = _mm256_add_epi32(zmm2, _mm256_castps_si256(c));                    \
    zmm3 = _mm256_add_epi32(zmm3, _mm256_castps_si256(c));                    \
    zmm4 = _mm256_add_epi32(zmm4, _mm256_castps_si256(c));                    \
    zmm5 = _mm256_add_epi32(zmm5, _mm256_castps_si256(c));                    \
    zmm6 = _mm256_add_epi32(zmm6, _mm256_castps_si256(c));                    \
    zmm7 = _mm256_add_epi32(zmm7, _mm256_castps_si256(c));                    \
    ymm0 = _mm256_castps_si256(_mm256_sub_ps(_mm256_castsi256_ps(ymm0), c));  \
    ymm1 = _mm256_castps_si256(_mm256_sub_ps(_mm256_castsi256_ps(ymm1), c));  \
    ymm2 = _mm256_castps_si256(_mm256_sub_ps(_mm256_castsi256_ps(ymm2), c));  \
    ymm3 = _mm256_castps_si256(_mm256_sub_ps(_mm256_castsi256_ps(ymm3), c));  \
    ymm4 = _mm256_castps_si256(_mm256_sub_ps(_mm256_castsi256_ps(ymm4), c));  \
    ymm5 = _mm256_castps_si256(_mm256_sub_ps(_mm256_castsi256_ps(ymm5), c));  \
    ymm6 = _mm256_castps_si256(_mm256_sub_ps(_mm256_castsi256_ps(ymm6), c));  \
    ymm7 = _mm256_castps_si256(_mm256_sub_ps(_mm256_castsi256_ps(ymm7), c));  \
    zmm0 = _mm256_castps_si256(_mm256_sub_ps(_mm256_castsi256_ps(zmm0), c));  \
    zmm1 = _mm256_castps_si256(_mm256_sub_ps(_mm256_castsi256_ps(zmm1), c));  \
    zmm2 = _mm256_castps_si256(_mm256_sub_ps(_mm256_castsi256_ps(zmm2), c));  \
    zmm3 = _mm256_castps_si256(_mm256_sub_ps(_mm256_castsi256_ps(zmm3), c));  \
    zmm4 = _mm256_castps_si256(_mm256_sub_ps(_mm256_castsi256_ps(zmm4), c));  \
    zmm5 = _mm256_castps_si256(_mm256_sub_ps(_mm256_castsi256_ps(zmm5), c));  \
    zmm6 = _mm256_castps_si256(_mm256_sub_ps(_mm256_castsi256_ps(zmm6), c));  \
    zmm7 = _mm256_castps_si256(_mm256_sub_ps(_mm256_castsi256_ps(zmm7), c));  \
    ymm0 = _mm256_castps_si256(_mm256_fmadd_ps(                               \
        _mm256_castsi256_ps(ymm0), d, _mm256_castsi256_ps(zmm0)));            \
    ymm1 = _mm256_castps_si256(_mm256_fmadd_ps(                               \
        _mm256_castsi256_ps(ymm1), d, _mm256_castsi256_ps(zmm1)));            \
    ymm2 = _mm256_castps_si256(_mm256_fmadd_ps(                               \
        _mm256_castsi256_ps(ymm2), d, _mm256_castsi256_ps(zmm2)));            \
    ymm3 = _mm256_castps_si256(_mm256_fmadd_ps(                               \
        _mm256_castsi256_ps(ymm3), d, _mm256_castsi256_ps(zmm3)));            \
    ymm4 = _mm256_castps_si256(_mm256_fmadd_ps(                               \
        _mm256_castsi256_ps(ymm4), d, _mm256_castsi256_ps(zmm4)));            \
    ymm5 = _mm256_castps_si256(_mm256_fmadd_ps(                               \
        _mm256_castsi256_ps(ymm5), d, _mm256_castsi256_ps(zmm5)));            \
    ymm6 = _mm256_castps_si256(_mm256_fmadd_ps(                               \
        _mm256_castsi256_ps(ymm6), d, _mm256_castsi256_ps(zmm6)));            \
    ymm7 = _mm256_castps_si256(_mm256_fmadd_ps(                               \
        _mm256_castsi256_ps(ymm7), d, _mm256_castsi256_ps(zmm7)));

#else // MCKL_USE_FMA

#define MCKL_RANDOM_INTERNAL_U01_AVX2_CVTEPU32_PS                             \
    const __m256 c = _mm256_set1_ps(Pow2<float, 23>::value);                  \
    const __m256 d = _mm256_set1_ps(Pow2<float, 16>::value);                  \
    const __m256i l = _mm256_set1_epi32(static_cast<MCKL_INT64>(0xFFFF));     \
    __m256i zmm0 = _mm256_and_si256(ymm0, l);                                 \
    __m256i zmm1 = _mm256_and_si256(ymm1, l);                                 \
    __m256i zmm2 = _mm256_and_si256(ymm2, l);                                 \
    __m256i zmm3 = _mm256_and_si256(ymm3, l);                                 \
    __m256i zmm4 = _mm256_and_si256(ymm4, l);                                 \
    __m256i zmm5 = _mm256_and_si256(ymm5, l);                                 \
    __m256i zmm6 = _mm256_and_si256(ymm6, l);                                 \
    __m256i zmm7 = _mm256_and_si256(ymm7, l);                                 \
    MCKL_RANDOM_INTERNAL_U01_AVX2_SRLI_EPI32(16);                             \
    ymm0 = _mm256_add_epi32(ymm0, _mm256_castps_si256(c));                    \
    ymm1 = _mm256_add_epi32(ymm1, _mm256_castps_si256(c));                    \
    ymm2 = _mm256_add_epi32(ymm2, _mm256_castps_si256(c));                    \
    ymm3 = _mm256_add_epi32(ymm3, _mm256_castps_si256(c));                    \
    ymm4 = _mm256_add_epi32(ymm4, _mm256_castps_si256(c));                    \
    ymm5 = _mm256_add_epi32(ymm5, _mm256_castps_si256(c));                    \
    ymm6 = _mm256_add_epi32(ymm6, _mm256_castps_si256(c));                    \
    ymm7 = _mm256_add_epi32(ymm7, _mm256_castps_si256(c));                    \
    zmm0 = _mm256_add_epi32(zmm0, _mm256_castps_si256(c));                    \
    zmm1 = _mm256_add_epi32(zmm1, _mm256_castps_si256(c));                    \
    zmm2 = _mm256_add_epi32(zmm2, _mm256_castps_si256(c));                    \
    zmm3 = _mm256_add_epi32(zmm3, _mm256_castps_si256(c));                    \
    zmm4 = _mm256_add_epi32(zmm4, _mm256_castps_si256(c));                    \
    zmm5 = _mm256_add_epi32(zmm5, _mm256_castps_si256(c));                    \
    zmm6 = _mm256_add_epi32(zmm6, _mm256_castps_si256(c));                    \
    zmm7 = _mm256_add_epi32(zmm7, _mm256_castps_si256(c));                    \
    ymm0 = _mm256_castps_si256(_mm256_sub_ps(_mm256_castsi256_ps(ymm0), c));  \
    ymm1 = _mm256_castps_si256(_mm256_sub_ps(_mm256_castsi256_ps(ymm1), c));  \
    ymm2 = _mm256_castps_si256(_mm256_sub_ps(_mm256_castsi256_ps(ymm2), c));  \
    ymm3 = _mm256_castps_si256(_mm256_sub_ps(_mm256_castsi256_ps(ymm3), c));  \
    ymm4 = _mm256_castps_si256(_mm256_sub_ps(_mm256_castsi256_ps(ymm4), c));  \
    ymm5 = _mm256_castps_si256(_mm256_sub_ps(_mm256_castsi256_ps(ymm5), c));  \
    ymm6 = _mm256_castps_si256(_mm256_sub_ps(_mm256_castsi256_ps(ymm6), c));  \
    ymm7 = _mm256_castps_si256(_mm256_sub_ps(_mm256_castsi256_ps(ymm7), c));  \
    zmm0 = _mm256_castps_si256(_mm256_sub_ps(_mm256_castsi256_ps(zmm0), c));  \
    zmm1 = _mm256_castps_si256(_mm256_sub_ps(_mm256_castsi256_ps(zmm1), c));  \
    zmm2 = _mm256_castps_si256(_mm256_sub_ps(_mm256_castsi256_ps(zmm2), c));  \
    zmm3 = _mm256_castps_si256(_mm256_sub_ps(_mm256_castsi256_ps(zmm3), c));  \
    zmm4 = _mm256_castps_si256(_mm256_sub_ps(_mm256_castsi256_ps(zmm4), c));  \
    zmm5 = _mm256_castps_si256(_mm256_sub_ps(_mm256_castsi256_ps(zmm5), c));  \
    zmm6 = _mm256_castps_si256(_mm256_sub_ps(_mm256_castsi256_ps(zmm6), c));  \
    zmm7 = _mm256_castps_si256(_mm256_sub_ps(_mm256_castsi256_ps(zmm7), c));  \
    ymm0 = _mm256_castps_si256(_mm256_mul_ps(_mm256_castsi256_ps(ymm0), d));  \
    ymm1 = _mm256_castps_si256(_mm256_mul_ps(_mm256_castsi256_ps(ymm1), d));  \
    ymm2 = _mm256_castps_si256(_mm256_mul_ps(_mm256_castsi256_ps(ymm2), d));  \
    ymm3 = _mm256_castps_si256(_mm256_mul_ps(_mm256_castsi256_ps(ymm3), d));  \
    ymm4 = _mm256_castps_si256(_mm256_mul_ps(_mm256_castsi256_ps(ymm4), d));  \
    ymm5 = _mm256_castps_si256(_mm256_mul_ps(_mm256_castsi256_ps(ymm5), d));  \
    ymm6 = _mm256_castps_si256(_mm256_mul_ps(_mm256_castsi256_ps(ymm6), d));  \
    ymm7 = _mm256_castps_si256(_mm256_mul_ps(_mm256_castsi256_ps(ymm7), d));  \
    ymm0 = _mm256_castps_si256(                                               \
        _mm256_add_ps(_mm256_castsi256_ps(ymm0), _mm256_castsi256_ps(zmm0))); \
    ymm1 = _mm256_castps_si256(                                               \
        _mm256_add_ps(_mm256_castsi256_ps(ymm1), _mm256_castsi256_ps(zmm1))); \
    ymm2 = _mm256_castps_si256(                                               \
        _mm256_add_ps(_mm256_castsi256_ps(ymm2), _mm256_castsi256_ps(zmm2))); \
    ymm3 = _mm256_castps_si256(                                               \
        _mm256_add_ps(_mm256_castsi256_ps(ymm3), _mm256_castsi256_ps(zmm3))); \
    ymm4 = _mm256_castps_si256(                                               \
        _mm256_add_ps(_mm256_castsi256_ps(ymm4), _mm256_castsi256_ps(zmm4))); \
    ymm5 = _mm256_castps_si256(                                               \
        _mm256_add_ps(_mm256_castsi256_ps(ymm5), _mm256_castsi256_ps(zmm5))); \
    ymm6 = _mm256_castps_si256(                                               \
        _mm256_add_ps(_mm256_castsi256_ps(ymm6), _mm256_castsi256_ps(zmm6))); \
    ymm7 = _mm256_castps_si256(                                               \
        _mm256_add_ps(_mm256_castsi256_ps(ymm7), _mm256_castsi256_ps(zmm7)));

#endif // // MCKL_USE_FMA

#define MCKL_RANDOM_INTERNAL_U01_AVX2_CVTEPU64_PD_52                          \
    const __m256d c = _mm256_set1_pd(Pow2<double, 52>::value);                \
    ymm0 = _mm256_add_epi64(ymm0, _mm256_castpd_si256(c));                    \
    ymm1 = _mm256_add_epi64(ymm1, _mm256_castpd_si256(c));                    \
    ymm2 = _mm256_add_epi64(ymm2, _mm256_castpd_si256(c));                    \
    ymm3 = _mm256_add_epi64(ymm3, _mm256_castpd_si256(c));                    \
    ymm4 = _mm256_add_epi64(ymm4, _mm256_castpd_si256(c));                    \
    ymm5 = _mm256_add_epi64(ymm5, _mm256_castpd_si256(c));                    \
    ymm6 = _mm256_add_epi64(ymm6, _mm256_castpd_si256(c));                    \
    ymm7 = _mm256_add_epi64(ymm7, _mm256_castpd_si256(c));                    \
    ymm0 = _mm256_castpd_si256(_mm256_sub_pd(_mm256_castsi256_pd(ymm0), c));  \
    ymm1 = _mm256_castpd_si256(_mm256_sub_pd(_mm256_castsi256_pd(ymm1), c));  \
    ymm2 = _mm256_castpd_si256(_mm256_sub_pd(_mm256_castsi256_pd(ymm2), c));  \
    ymm3 = _mm256_castpd_si256(_mm256_sub_pd(_mm256_castsi256_pd(ymm3), c));  \
    ymm4 = _mm256_castpd_si256(_mm256_sub_pd(_mm256_castsi256_pd(ymm4), c));  \
    ymm5 = _mm256_castpd_si256(_mm256_sub_pd(_mm256_castsi256_pd(ymm5), c));  \
    ymm6 = _mm256_castpd_si256(_mm256_sub_pd(_mm256_castsi256_pd(ymm6), c));  \
    ymm7 = _mm256_castpd_si256(_mm256_sub_pd(_mm256_castsi256_pd(ymm7), c));

#if MCKL_USE_FMA

#define MCKL_RANDOM_INTERNAL_U01_AVX2_CVTEPU64_PD                             \
    const __m256d c = _mm256_set1_pd(Pow2<double, 52>::value);                \
    const __m256d d = _mm256_set1_pd(Pow2<double, 32>::value);                \
    const __m256i l =                                                         \
        _mm256_set1_epi64x(static_cast<MCKL_INT64>(0xFFFFFFFF));              \
    __m256i zmm0 = _mm256_and_si256(ymm0, l);                                 \
    __m256i zmm1 = _mm256_and_si256(ymm1, l);                                 \
    __m256i zmm2 = _mm256_and_si256(ymm2, l);                                 \
    __m256i zmm3 = _mm256_and_si256(ymm3, l);                                 \
    __m256i zmm4 = _mm256_and_si256(ymm4, l);                                 \
    __m256i zmm5 = _mm256_and_si256(ymm5, l);                                 \
    __m256i zmm6 = _mm256_and_si256(ymm6, l);                                 \
    __m256i zmm7 = _mm256_and_si256(ymm7, l);                                 \
    MCKL_RANDOM_INTERNAL_U01_AVX2_SRLI_EPI64(32);                             \
    ymm0 = _mm256_add_epi64(ymm0, _mm256_castpd_si256(c));                    \
    ymm1 = _mm256_add_epi64(ymm1, _mm256_castpd_si256(c));                    \
    ymm2 = _mm256_add_epi64(ymm2, _mm256_castpd_si256(c));                    \
    ymm3 = _mm256_add_epi64(ymm3, _mm256_castpd_si256(c));                    \
    ymm4 = _mm256_add_epi64(ymm4, _mm256_castpd_si256(c));                    \
    ymm5 = _mm256_add_epi64(ymm5, _mm256_castpd_si256(c));                    \
    ymm6 = _mm256_add_epi64(ymm6, _mm256_castpd_si256(c));                    \
    ymm7 = _mm256_add_epi64(ymm7, _mm256_castpd_si256(c));                    \
    zmm0 = _mm256_add_epi64(zmm0, _mm256_castpd_si256(c));                    \
    zmm1 = _mm256_add_epi64(zmm1, _mm256_castpd_si256(c));                    \
    zmm2 = _mm256_add_epi64(zmm2, _mm256_castpd_si256(c));                    \
    zmm3 = _mm256_add_epi64(zmm3, _mm256_castpd_si256(c));                    \
    zmm4 = _mm256_add_epi64(zmm4, _mm256_castpd_si256(c));                    \
    zmm5 = _mm256_add_epi64(zmm5, _mm256_castpd_si256(c));                    \
    zmm6 = _mm256_add_epi64(zmm6, _mm256_castpd_si256(c));                    \
    zmm7 = _mm256_add_epi64(zmm7, _mm256_castpd_si256(c));                    \
    ymm0 = _mm256_castpd_si256(_mm256_sub_pd(_mm256_castsi256_pd(ymm0), c));  \
    ymm1 = _mm256_castpd_si256(_mm256_sub_pd(_mm256_castsi256_pd(ymm1), c));  \
    ymm2 = _mm256_castpd_si256(_mm256_sub_pd(_mm256_castsi256_pd(ymm2), c));  \
    ymm3 = _mm256_castpd_si256(_mm256_sub_pd(_mm256_castsi256_pd(ymm3), c));  \
    ymm4 = _mm256_castpd_si256(_mm256_sub_pd(_mm256_castsi256_pd(ymm4), c));  \
    ymm5 = _mm256_castpd_si256(_mm256_sub_pd(_mm256_castsi256_pd(ymm5), c));  \
    ymm6 = _mm256_castpd_si256(_mm256_sub_pd(_mm256_castsi256_pd(ymm6), c));  \
    ymm7 = _mm256_castpd_si256(_mm256_sub_pd(_mm256_castsi256_pd(ymm7), c));  \
    zmm0 = _mm256_castpd_si256(_mm256_sub_pd(_mm256_castsi256_pd(zmm0), c));  \
    zmm1 = _mm256_castpd_si256(_mm256_sub_pd(_mm256_castsi256_pd(zmm1), c));  \
    zmm2 = _mm256_castpd_si256(_mm256_sub_pd(_mm256_castsi256_pd(zmm2), c));  \
    zmm3 = _mm256_castpd_si256(_mm256_sub_pd(_mm256_castsi256_pd(zmm3), c));  \
    zmm4 = _mm256_castpd_si256(_mm256_sub_pd(_mm256_castsi256_pd(zmm4), c));  \
    zmm5 = _mm256_castpd_si256(_mm256_sub_pd(_mm256_castsi256_pd(zmm5), c));  \
    zmm6 = _mm256_castpd_si256(_mm256_sub_pd(_mm256_castsi256_pd(zmm6), c));  \
    zmm7 = _mm256_castpd_si256(_mm256_sub_pd(_mm256_castsi256_pd(zmm7), c));  \
    ymm0 = _mm256_castpd_si256(_mm256_fmadd_pd(                               \
        _mm256_castsi256_pd(ymm0), d, _mm256_castsi256_pd(zmm0)));            \
    ymm1 = _mm256_castpd_si256(_mm256_fmadd_pd(                               \
        _mm256_castsi256_pd(ymm1), d, _mm256_castsi256_pd(zmm1)));            \
    ymm2 = _mm256_castpd_si256(_mm256_fmadd_pd(                               \
        _mm256_castsi256_pd(ymm2), d, _mm256_castsi256_pd(zmm2)));            \
    ymm3 = _mm256_castpd_si256(_mm256_fmadd_pd(                               \
        _mm256_castsi256_pd(ymm3), d, _mm256_castsi256_pd(zmm3)));            \
    ymm4 = _mm256_castpd_si256(_mm256_fmadd_pd(                               \
        _mm256_castsi256_pd(ymm4), d, _mm256_castsi256_pd(zmm4)));            \
    ymm5 = _mm256_castpd_si256(_mm256_fmadd_pd(                               \
        _mm256_castsi256_pd(ymm5), d, _mm256_castsi256_pd(zmm5)));            \
    ymm6 = _mm256_castpd_si256(_mm256_fmadd_pd(                               \
        _mm256_castsi256_pd(ymm6), d, _mm256_castsi256_pd(zmm6)));            \
    ymm7 = _mm256_castpd_si256(_mm256_fmadd_pd(                               \
        _mm256_castsi256_pd(ymm7), d, _mm256_castsi256_pd(zmm7)));

#else // MCKL_USE_FMA

#define MCKL_RANDOM_INTERNAL_U01_AVX2_CVTEPU64_PD                             \
    const __m256d c = _mm256_set1_pd(Pow2<double, 52>::value);                \
    const __m256d d = _mm256_set1_pd(Pow2<double, 32>::value);                \
    const __m256i l =                                                         \
        _mm256_set1_epi64x(static_cast<MCKL_INT64>(0xFFFFFFFF));              \
    __m256i zmm0 = _mm256_and_si256(ymm0, l);                                 \
    __m256i zmm1 = _mm256_and_si256(ymm1, l);                                 \
    __m256i zmm2 = _mm256_and_si256(ymm2, l);                                 \
    __m256i zmm3 = _mm256_and_si256(ymm3, l);                                 \
    __m256i zmm4 = _mm256_and_si256(ymm4, l);                                 \
    __m256i zmm5 = _mm256_and_si256(ymm5, l);                                 \
    __m256i zmm6 = _mm256_and_si256(ymm6, l);                                 \
    __m256i zmm7 = _mm256_and_si256(ymm7, l);                                 \
    MCKL_RANDOM_INTERNAL_U01_AVX2_SRLI_EPI64(32);                             \
    ymm0 = _mm256_add_epi64(ymm0, _mm256_castpd_si256(c));                    \
    ymm1 = _mm256_add_epi64(ymm1, _mm256_castpd_si256(c));                    \
    ymm2 = _mm256_add_epi64(ymm2, _mm256_castpd_si256(c));                    \
    ymm3 = _mm256_add_epi64(ymm3, _mm256_castpd_si256(c));                    \
    ymm4 = _mm256_add_epi64(ymm4, _mm256_castpd_si256(c));                    \
    ymm5 = _mm256_add_epi64(ymm5, _mm256_castpd_si256(c));                    \
    ymm6 = _mm256_add_epi64(ymm6, _mm256_castpd_si256(c));                    \
    ymm7 = _mm256_add_epi64(ymm7, _mm256_castpd_si256(c));                    \
    zmm0 = _mm256_add_epi64(zmm0, _mm256_castpd_si256(c));                    \
    zmm1 = _mm256_add_epi64(zmm1, _mm256_castpd_si256(c));                    \
    zmm2 = _mm256_add_epi64(zmm2, _mm256_castpd_si256(c));                    \
    zmm3 = _mm256_add_epi64(zmm3, _mm256_castpd_si256(c));                    \
    zmm4 = _mm256_add_epi64(zmm4, _mm256_castpd_si256(c));                    \
    zmm5 = _mm256_add_epi64(zmm5, _mm256_castpd_si256(c));                    \
    zmm6 = _mm256_add_epi64(zmm6, _mm256_castpd_si256(c));                    \
    zmm7 = _mm256_add_epi64(zmm7, _mm256_castpd_si256(c));                    \
    ymm0 = _mm256_castpd_si256(_mm256_sub_pd(_mm256_castsi256_pd(ymm0), c));  \
    ymm1 = _mm256_castpd_si256(_mm256_sub_pd(_mm256_castsi256_pd(ymm1), c));  \
    ymm2 = _mm256_castpd_si256(_mm256_sub_pd(_mm256_castsi256_pd(ymm2), c));  \
    ymm3 = _mm256_castpd_si256(_mm256_sub_pd(_mm256_castsi256_pd(ymm3), c));  \
    ymm4 = _mm256_castpd_si256(_mm256_sub_pd(_mm256_castsi256_pd(ymm4), c));  \
    ymm5 = _mm256_castpd_si256(_mm256_sub_pd(_mm256_castsi256_pd(ymm5), c));  \
    ymm6 = _mm256_castpd_si256(_mm256_sub_pd(_mm256_castsi256_pd(ymm6), c));  \
    ymm7 = _mm256_castpd_si256(_mm256_sub_pd(_mm256_castsi256_pd(ymm7), c));  \
    zmm0 = _mm256_castpd_si256(_mm256_sub_pd(_mm256_castsi256_pd(zmm0), c));  \
    zmm1 = _mm256_castpd_si256(_mm256_sub_pd(_mm256_castsi256_pd(zmm1), c));  \
    zmm2 = _mm256_castpd_si256(_mm256_sub_pd(_mm256_castsi256_pd(zmm2), c));  \
    zmm3 = _mm256_castpd_si256(_mm256_sub_pd(_mm256_castsi256_pd(zmm3), c));  \
    zmm4 = _mm256_castpd_si256(_mm256_sub_pd(_mm256_castsi256_pd(zmm4), c));  \
    zmm5 = _mm256_castpd_si256(_mm256_sub_pd(_mm256_castsi256_pd(zmm5), c));  \
    zmm6 = _mm256_castpd_si256(_mm256_sub_pd(_mm256_castsi256_pd(zmm6), c));  \
    zmm7 = _mm256_castpd_si256(_mm256_sub_pd(_mm256_castsi256_pd(zmm7), c));  \
    ymm0 = _mm256_castpd_si256(_mm256_mul_pd(_mm256_castsi256_pd(ymm0), d));  \
    ymm1 = _mm256_castpd_si256(_mm256_mul_pd(_mm256_castsi256_pd(ymm1), d));  \
    ymm2 = _mm256_castpd_si256(_mm256_mul_pd(_mm256_castsi256_pd(ymm2), d));  \
    ymm3 = _mm256_castpd_si256(_mm256_mul_pd(_mm256_castsi256_pd(ymm3), d));  \
    ymm4 = _mm256_castpd_si256(_mm256_mul_pd(_mm256_castsi256_pd(ymm4), d));  \
    ymm5 = _mm256_castpd_si256(_mm256_mul_pd(_mm256_castsi256_pd(ymm5), d));  \
    ymm6 = _mm256_castpd_si256(_mm256_mul_pd(_mm256_castsi256_pd(ymm6), d));  \
    ymm7 = _mm256_castpd_si256(_mm256_mul_pd(_mm256_castsi256_pd(ymm7), d));  \
    ymm0 = _mm256_castpd_si256(                                               \
        _mm256_add_pd(_mm256_castsi256_pd(ymm0), _mm256_castsi256_pd(zmm0))); \
    ymm1 = _mm256_castpd_si256(                                               \
        _mm256_add_pd(_mm256_castsi256_pd(ymm1), _mm256_castsi256_pd(zmm1))); \
    ymm2 = _mm256_castpd_si256(                                               \
        _mm256_add_pd(_mm256_castsi256_pd(ymm2), _mm256_castsi256_pd(zmm2))); \
    ymm3 = _mm256_castpd_si256(                                               \
        _mm256_add_pd(_mm256_castsi256_pd(ymm3), _mm256_castsi256_pd(zmm3))); \
    ymm4 = _mm256_castpd_si256(                                               \
        _mm256_add_pd(_mm256_castsi256_pd(ymm4), _mm256_castsi256_pd(zmm4))); \
    ymm5 = _mm256_castpd_si256(                                               \
        _mm256_add_pd(_mm256_castsi256_pd(ymm5), _mm256_castsi256_pd(zmm5))); \
    ymm6 = _mm256_castpd_si256(                                               \
        _mm256_add_pd(_mm256_castsi256_pd(ymm6), _mm256_castsi256_pd(zmm6))); \
    ymm7 = _mm256_castpd_si256(                                               \
        _mm256_add_pd(_mm256_castsi256_pd(ymm7), _mm256_castsi256_pd(zmm7)));

#endif // MCKL_USE_FMA

#define MCKL_RANDOM_INTERNAL_U01_AVX2_ADD_PS(a)                               \
    ymm0 = _mm256_castps_si256(_mm256_add_ps(_mm256_castsi256_ps(ymm0), a));  \
    ymm1 = _mm256_castps_si256(_mm256_add_ps(_mm256_castsi256_ps(ymm1), a));  \
    ymm2 = _mm256_castps_si256(_mm256_add_ps(_mm256_castsi256_ps(ymm2), a));  \
    ymm3 = _mm256_castps_si256(_mm256_add_ps(_mm256_castsi256_ps(ymm3), a));  \
    ymm4 = _mm256_castps_si256(_mm256_add_ps(_mm256_castsi256_ps(ymm4), a));  \
    ymm5 = _mm256_castps_si256(_mm256_add_ps(_mm256_castsi256_ps(ymm5), a));  \
    ymm6 = _mm256_castps_si256(_mm256_add_ps(_mm256_castsi256_ps(ymm6), a));  \
    ymm7 = _mm256_castps_si256(_mm256_add_ps(_mm256_castsi256_ps(ymm7), a));

#define MCKL_RANDOM_INTERNAL_U01_AVX2_ADD_PD(a)                               \
    ymm0 = _mm256_castpd_si256(_mm256_add_pd(_mm256_castsi256_pd(ymm0), a));  \
    ymm1 = _mm256_castpd_si256(_mm256_add_pd(_mm256_castsi256_pd(ymm1), a));  \
    ymm2 = _mm256_castpd_si256(_mm256_add_pd(_mm256_castsi256_pd(ymm2), a));  \
    ymm3 = _mm256_castpd_si256(_mm256_add_pd(_mm256_castsi256_pd(ymm3), a));  \
    ymm4 = _mm256_castpd_si256(_mm256_add_pd(_mm256_castsi256_pd(ymm4), a));  \
    ymm5 = _mm256_castpd_si256(_mm256_add_pd(_mm256_castsi256_pd(ymm5), a));  \
    ymm6 = _mm256_castpd_si256(_mm256_add_pd(_mm256_castsi256_pd(ymm6), a));  \
    ymm7 = _mm256_castpd_si256(_mm256_add_pd(_mm256_castsi256_pd(ymm7), a));

#define MCKL_RANDOM_INTERNAL_U01_AVX2_MUL_PS(a)                               \
    ymm0 = _mm256_castps_si256(_mm256_mul_ps(_mm256_castsi256_ps(ymm0), a));  \
    ymm1 = _mm256_castps_si256(_mm256_mul_ps(_mm256_castsi256_ps(ymm1), a));  \
    ymm2 = _mm256_castps_si256(_mm256_mul_ps(_mm256_castsi256_ps(ymm2), a));  \
    ymm3 = _mm256_castps_si256(_mm256_mul_ps(_mm256_castsi256_ps(ymm3), a));  \
    ymm4 = _mm256_castps_si256(_mm256_mul_ps(_mm256_castsi256_ps(ymm4), a));  \
    ymm5 = _mm256_castps_si256(_mm256_mul_ps(_mm256_castsi256_ps(ymm5), a));  \
    ymm6 = _mm256_castps_si256(_mm256_mul_ps(_mm256_castsi256_ps(ymm6), a));  \
    ymm7 = _mm256_castps_si256(_mm256_mul_ps(_mm256_castsi256_ps(ymm7), a));

#define MCKL_RANDOM_INTERNAL_U01_AVX2_MUL_PD(a)                               \
    ymm0 = _mm256_castpd_si256(_mm256_mul_pd(_mm256_castsi256_pd(ymm0), a));  \
    ymm1 = _mm256_castpd_si256(_mm256_mul_pd(_mm256_castsi256_pd(ymm1), a));  \
    ymm2 = _mm256_castpd_si256(_mm256_mul_pd(_mm256_castsi256_pd(ymm2), a));  \
    ymm3 = _mm256_castpd_si256(_mm256_mul_pd(_mm256_castsi256_pd(ymm3), a));  \
    ymm4 = _mm256_castpd_si256(_mm256_mul_pd(_mm256_castsi256_pd(ymm4), a));  \
    ymm5 = _mm256_castpd_si256(_mm256_mul_pd(_mm256_castsi256_pd(ymm5), a));  \
    ymm6 = _mm256_castpd_si256(_mm256_mul_pd(_mm256_castsi256_pd(ymm6), a));  \
    ymm7 = _mm256_castpd_si256(_mm256_mul_pd(_mm256_castsi256_pd(ymm7), a));

#if MCKL_USE_FMA

#define MCKL_RANDOM_INTERNAL_U01_AVX2_FMADD_PS(a, b)                          \
    ymm0 = _mm256_castps_si256(                                               \
        _mm256_fmadd_ps(_mm256_castsi256_ps(ymm0), a, b));                    \
    ymm1 = _mm256_castps_si256(                                               \
        _mm256_fmadd_ps(_mm256_castsi256_ps(ymm1), a, b));                    \
    ymm2 = _mm256_castps_si256(                                               \
        _mm256_fmadd_ps(_mm256_castsi256_ps(ymm2), a, b));                    \
    ymm3 = _mm256_castps_si256(                                               \
        _mm256_fmadd_ps(_mm256_castsi256_ps(ymm3), a, b));                    \
    ymm4 = _mm256_castps_si256(                                               \
        _mm256_fmadd_ps(_mm256_castsi256_ps(ymm4), a, b));                    \
    ymm5 = _mm256_castps_si256(                                               \
        _mm256_fmadd_ps(_mm256_castsi256_ps(ymm5), a, b));                    \
    ymm6 = _mm256_castps_si256(                                               \
        _mm256_fmadd_ps(_mm256_castsi256_ps(ymm6), a, b));                    \
    ymm7 = _mm256_castps_si256(                                               \
        _mm256_fmadd_ps(_mm256_castsi256_ps(ymm7), a, b));

#define MCKL_RANDOM_INTERNAL_U01_AVX2_FMADD_PD(a, b)                          \
    ymm0 = _mm256_castpd_si256(                                               \
        _mm256_fmadd_pd(_mm256_castsi256_pd(ymm0), a, b));                    \
    ymm1 = _mm256_castpd_si256(                                               \
        _mm256_fmadd_pd(_mm256_castsi256_pd(ymm1), a, b));                    \
    ymm2 = _mm256_castpd_si256(                                               \
        _mm256_fmadd_pd(_mm256_castsi256_pd(ymm2), a, b));                    \
    ymm3 = _mm256_castpd_si256(                                               \
        _mm256_fmadd_pd(_mm256_castsi256_pd(ymm3), a, b));                    \
    ymm4 = _mm256_castpd_si256(                                               \
        _mm256_fmadd_pd(_mm256_castsi256_pd(ymm4), a, b));                    \
    ymm5 = _mm256_castpd_si256(                                               \
        _mm256_fmadd_pd(_mm256_castsi256_pd(ymm5), a, b));                    \
    ymm6 = _mm256_castpd_si256(                                               \
        _mm256_fmadd_pd(_mm256_castsi256_pd(ymm6), a, b));                    \
    ymm7 = _mm256_castpd_si256(                                               \
        _mm256_fmadd_pd(_mm256_castsi256_pd(ymm7), a, b));

#else // MCKL_USE_FMA

#define MCKL_RANDOM_INTERNAL_U01_AVX2_FMADD_PS(a, b)                          \
    MCKL_RANDOM_INTERNAL_U01_AVX2_MUL_PS(a)                                   \
    MCKL_RANDOM_INTERNAL_U01_AVX2_ADD_PS(b)

#define MCKL_RANDOM_INTERNAL_U01_AVX2_FMADD_PD(a, b)                          \
    MCKL_RANDOM_INTERNAL_U01_AVX2_MUL_PD(a)                                   \
    MCKL_RANDOM_INTERNAL_U01_AVX2_ADD_PD(b)

#endif // MCKL_USE_FMA

namespace mckl
{

namespace internal
{

template <typename UIntType, typename RealType, typename Lower, typename Upper,
    int = std::numeric_limits<UIntType>::digits>
class U01AVX2Impl : public U01GenericImpl<UIntType, RealType, Lower, Upper>
{
}; // class U01AVX2Impl

template <typename UIntType, typename RealType, typename Lower, typename Upper>
class U01AVX2ImplBase
{
    public:
    MCKL_INLINE static RealType eval(UIntType u)
    {
        return U01GenericImpl<UIntType, RealType, Lower, Upper>::eval(u);
    }

    MCKL_INLINE static void eval(std::size_t n, const UIntType *u, RealType *r)
    {
        constexpr std::size_t S = 8;
        constexpr std::size_t N = sizeof(__m256i) * S / sizeof(RealType);

        while (n >= N) {
            U01AVX2Impl<UIntType, RealType, Lower, Upper>::eval(u, r);
            n -= N;
            u += N;
            r += N;
        }

        for (std::size_t i = 0; i != n; ++i)
            r[i] = eval(u[i]);
    }
}; // class U01AVX2ImplBase

template <typename UIntType, typename RealType, int Q,
    int = std::numeric_limits<UIntType>::digits>
class U01CanonicalAVX2Impl
    : public U01CanonicalGenericImpl<UIntType, RealType, Q>
{
}; // class U01CanonicalAVX2Impl

template <typename UIntType, typename RealType, int Q>
class U01CanonicalAVX2ImplBase
{
    public:
    MCKL_INLINE static RealType eval(const UIntType *u)
    {
        return U01CanonicalGenericImpl<UIntType, RealType, Q>::eval(u);
    }

    MCKL_INLINE static void eval(std::size_t n, const UIntType *u, RealType *r)
    {
        constexpr std::size_t S = 8;
        constexpr std::size_t N = sizeof(__m256i) * S / sizeof(RealType);

        while (n >= N) {
            U01CanonicalAVX2Impl<UIntType, RealType, Q>::eval(u, r);
            n -= N;
            u += N * Q;
            r += N;
        }

        for (std::size_t i = 0; i != n; ++i, u += Q)
            r[i] = eval(u);
    }
}; // class U01CanonicalCanonicalAVX2ImplBase

template <typename UIntType>
class U01AVX2Impl<UIntType, float, Closed, Closed, 32>
    : public U01AVX2ImplBase<UIntType, float, Closed, Closed>
{
    public:
    using U01AVX2ImplBase<UIntType, float, Closed, Closed>::eval;

    private:
    friend U01AVX2ImplBase<UIntType, float, Closed, Closed>;

    MCKL_INLINE static void eval(const UIntType *u, float *r)
    {
        const __m256 d25 = _mm256_set1_ps(Pow2<float, -25>::value);

        MCKL_RANDOM_INTERNAL_U01_AVX2_LOADU_SI256(u)
        MCKL_RANDOM_INTERNAL_U01_AVX2_SLLI_EPI32(1)
        MCKL_RANDOM_INTERNAL_U01_AVX2_SRLI_EPI32(7)
        MCKL_RANDOM_INTERNAL_U01_AVX2_AND1ADD_EPI32
        MCKL_RANDOM_INTERNAL_U01_AVX2_CVTEPU32_PS_31
        MCKL_RANDOM_INTERNAL_U01_AVX2_MUL_PS(d25)
        MCKL_RANDOM_INTERNAL_U01_AVX2_STOREU_PS(r)
    }
}; // class U01AVX2Impl

template <typename UIntType>
class U01AVX2Impl<UIntType, float, Closed, Open, 32>
    : public U01AVX2ImplBase<UIntType, float, Closed, Open>
{
    public:
    using U01AVX2ImplBase<UIntType, float, Closed, Open>::eval;

    private:
    friend U01AVX2ImplBase<UIntType, float, Closed, Open>;

    MCKL_INLINE static void eval(const UIntType *u, float *r)
    {
        const __m256 d24 = _mm256_set1_ps(Pow2<float, -24>::value);

        MCKL_RANDOM_INTERNAL_U01_AVX2_LOADU_SI256(u)
        MCKL_RANDOM_INTERNAL_U01_AVX2_SRLI_EPI32(8)
        MCKL_RANDOM_INTERNAL_U01_AVX2_CVTEPU32_PS_31
        MCKL_RANDOM_INTERNAL_U01_AVX2_MUL_PS(d24)
        MCKL_RANDOM_INTERNAL_U01_AVX2_STOREU_PS(r)
    }
}; // class U01AVX2Impl

template <typename UIntType>
class U01AVX2Impl<UIntType, float, Open, Closed, 32>
    : public U01AVX2ImplBase<UIntType, float, Open, Closed>
{
    public:
    using U01AVX2ImplBase<UIntType, float, Open, Closed>::eval;

    private:
    friend U01AVX2ImplBase<UIntType, float, Open, Closed>;

    MCKL_INLINE static void eval(const UIntType *u, float *r)
    {
        const __m256 d24 = _mm256_set1_ps(Pow2<float, -24>::value);

        MCKL_RANDOM_INTERNAL_U01_AVX2_LOADU_SI256(u)
        MCKL_RANDOM_INTERNAL_U01_AVX2_SRLI_EPI32(8)
        MCKL_RANDOM_INTERNAL_U01_AVX2_CVTEPU32_PS_31
        MCKL_RANDOM_INTERNAL_U01_AVX2_FMADD_PS(d24, d24)
        MCKL_RANDOM_INTERNAL_U01_AVX2_STOREU_PS(r)
    }
}; // class U01AVX2Impl

template <typename UIntType>
class U01AVX2Impl<UIntType, float, Open, Open, 32>
    : public U01AVX2ImplBase<UIntType, float, Open, Open>
{
    public:
    using U01AVX2ImplBase<UIntType, float, Open, Open>::eval;

    private:
    friend U01AVX2ImplBase<UIntType, float, Open, Open>;

    MCKL_INLINE static void eval(const UIntType *u, float *r)
    {
        const __m256 d23 = _mm256_set1_ps(Pow2<float, -23>::value);
        const __m256 d24 = _mm256_set1_ps(Pow2<float, -24>::value);

        MCKL_RANDOM_INTERNAL_U01_AVX2_LOADU_SI256(u)
        MCKL_RANDOM_INTERNAL_U01_AVX2_SRLI_EPI32(9)
        MCKL_RANDOM_INTERNAL_U01_AVX2_CVTEPU32_PS_31
        MCKL_RANDOM_INTERNAL_U01_AVX2_FMADD_PS(d23, d24)
        MCKL_RANDOM_INTERNAL_U01_AVX2_STOREU_PS(r)
    }
}; // class U01AVX2Impl

template <typename UIntType>
class U01AVX2Impl<UIntType, double, Closed, Closed, 32>
    : public U01AVX2ImplBase<UIntType, double, Closed, Closed>
{
    public:
    using U01AVX2ImplBase<UIntType, double, Closed, Closed>::eval;

    private:
    friend U01AVX2ImplBase<UIntType, double, Closed, Closed>;

    MCKL_INLINE static void eval(const UIntType *u, double *r)
    {
        const __m256d d32 = _mm256_set1_pd(Pow2<double, -32>::value);

        MCKL_RANDOM_INTERNAL_U01_AVX2_LOADU_SI256_CVTEPU32_EPI64(u)
        MCKL_RANDOM_INTERNAL_U01_AVX2_AND1ADD_EPI64
        MCKL_RANDOM_INTERNAL_U01_AVX2_CVTEPU64_PD_52
        MCKL_RANDOM_INTERNAL_U01_AVX2_MUL_PD(d32)
        MCKL_RANDOM_INTERNAL_U01_AVX2_STOREU_PD(r)
    }
}; // class U01AVX2Impl

template <typename UIntType>
class U01AVX2Impl<UIntType, double, Closed, Open, 32>
    : public U01AVX2ImplBase<UIntType, double, Closed, Open>
{
    public:
    using U01AVX2ImplBase<UIntType, double, Closed, Open>::eval;

    private:
    friend U01AVX2ImplBase<UIntType, double, Closed, Open>;

    MCKL_INLINE static void eval(const UIntType *u, double *r)
    {
        const __m256d d32 = _mm256_set1_pd(Pow2<double, -32>::value);

        MCKL_RANDOM_INTERNAL_U01_AVX2_LOADU_SI256_CVTEPU32_EPI64(u)
        MCKL_RANDOM_INTERNAL_U01_AVX2_CVTEPU64_PD_52
        MCKL_RANDOM_INTERNAL_U01_AVX2_MUL_PD(d32)
        MCKL_RANDOM_INTERNAL_U01_AVX2_STOREU_PD(r)
    }
}; // class U01AVX2Impl

template <typename UIntType>
class U01AVX2Impl<UIntType, double, Open, Closed, 32>
    : public U01AVX2ImplBase<UIntType, double, Open, Closed>
{
    public:
    using U01AVX2ImplBase<UIntType, double, Open, Closed>::eval;

    private:
    friend U01AVX2ImplBase<UIntType, double, Open, Closed>;

    MCKL_INLINE static void eval(const UIntType *u, double *r)
    {
        const __m256d d32 = _mm256_set1_pd(Pow2<double, -32>::value);

        MCKL_RANDOM_INTERNAL_U01_AVX2_LOADU_SI256_CVTEPU32_EPI64(u)
        MCKL_RANDOM_INTERNAL_U01_AVX2_CVTEPU64_PD_52
        MCKL_RANDOM_INTERNAL_U01_AVX2_FMADD_PD(d32, d32)
        MCKL_RANDOM_INTERNAL_U01_AVX2_STOREU_PD(r)
    }
}; // class U01AVX2Impl

template <typename UIntType>
class U01AVX2Impl<UIntType, double, Open, Open, 32>
    : public U01AVX2ImplBase<UIntType, double, Open, Open>
{
    public:
    using U01AVX2ImplBase<UIntType, double, Open, Open>::eval;

    private:
    friend U01AVX2ImplBase<UIntType, double, Open, Open>;

    MCKL_INLINE static void eval(const UIntType *u, double *r)
    {
        const __m256d d32 = _mm256_set1_pd(Pow2<double, -32>::value);
        const __m256d d33 = _mm256_set1_pd(Pow2<double, -33>::value);

        MCKL_RANDOM_INTERNAL_U01_AVX2_LOADU_SI256_CVTEPU32_EPI64(u)
        MCKL_RANDOM_INTERNAL_U01_AVX2_CVTEPU64_PD_52
        MCKL_RANDOM_INTERNAL_U01_AVX2_FMADD_PD(d32, d33)
        MCKL_RANDOM_INTERNAL_U01_AVX2_STOREU_PD(r)
    }
}; // class U01AVX2Impl

template <typename UIntType>
class U01AVX2Impl<UIntType, double, Closed, Closed, 64>
    : public U01AVX2ImplBase<UIntType, double, Closed, Closed>
{
    public:
    using U01AVX2ImplBase<UIntType, double, Closed, Closed>::eval;

    private:
    friend U01AVX2ImplBase<UIntType, double, Closed, Closed>;

    MCKL_INLINE static void eval(const UIntType *u, double *r)
    {
        const __m256d d54 = _mm256_set1_pd(Pow2<double, -54>::value);

        MCKL_RANDOM_INTERNAL_U01_AVX2_LOADU_SI256(u)
        MCKL_RANDOM_INTERNAL_U01_AVX2_SLLI_EPI64(1)
        MCKL_RANDOM_INTERNAL_U01_AVX2_SRLI_EPI64(10)
        MCKL_RANDOM_INTERNAL_U01_AVX2_AND1ADD_EPI64
        MCKL_RANDOM_INTERNAL_U01_AVX2_CVTEPU64_PD
        MCKL_RANDOM_INTERNAL_U01_AVX2_MUL_PD(d54)
        MCKL_RANDOM_INTERNAL_U01_AVX2_STOREU_PD(r)
    }
}; // class U01AVX2Impl

template <typename UIntType>
class U01AVX2Impl<UIntType, double, Closed, Open, 64>
    : public U01AVX2ImplBase<UIntType, double, Closed, Open>
{
    public:
    using U01AVX2ImplBase<UIntType, double, Closed, Open>::eval;

    private:
    friend U01AVX2ImplBase<UIntType, double, Closed, Open>;

    MCKL_INLINE static void eval(const UIntType *u, double *r)
    {
        const __m256d d53 = _mm256_set1_pd(Pow2<double, -53>::value);

        MCKL_RANDOM_INTERNAL_U01_AVX2_LOADU_SI256(u)
        MCKL_RANDOM_INTERNAL_U01_AVX2_SRLI_EPI64(11)
        MCKL_RANDOM_INTERNAL_U01_AVX2_CVTEPU64_PD
        MCKL_RANDOM_INTERNAL_U01_AVX2_MUL_PD(d53)
        MCKL_RANDOM_INTERNAL_U01_AVX2_STOREU_PD(r)
    }
}; // class U01AVX2Impl

template <typename UIntType>
class U01AVX2Impl<UIntType, double, Open, Closed, 64>
    : public U01AVX2ImplBase<UIntType, double, Open, Closed>
{
    public:
    using U01AVX2ImplBase<UIntType, double, Open, Closed>::eval;

    private:
    friend U01AVX2ImplBase<UIntType, double, Open, Closed>;

    MCKL_INLINE static void eval(const UIntType *u, double *r)
    {
        const __m256d d53 = _mm256_set1_pd(Pow2<double, -53>::value);

        MCKL_RANDOM_INTERNAL_U01_AVX2_LOADU_SI256(u)
        MCKL_RANDOM_INTERNAL_U01_AVX2_SRLI_EPI64(11)
        MCKL_RANDOM_INTERNAL_U01_AVX2_CVTEPU64_PD
        MCKL_RANDOM_INTERNAL_U01_AVX2_FMADD_PD(d53, d53)
        MCKL_RANDOM_INTERNAL_U01_AVX2_STOREU_PD(r)
    }
}; // class U01AVX2Impl

template <typename UIntType>
class U01AVX2Impl<UIntType, double, Open, Open, 64>
    : public U01AVX2ImplBase<UIntType, double, Open, Open>
{
    public:
    using U01AVX2ImplBase<UIntType, double, Open, Open>::eval;

    private:
    friend U01AVX2ImplBase<UIntType, double, Open, Open>;

    MCKL_INLINE static void eval(const UIntType *u, double *r)
    {
        const __m256d d52 = _mm256_set1_pd(Pow2<double, -52>::value);
        const __m256d d53 = _mm256_set1_pd(Pow2<double, -53>::value);

        MCKL_RANDOM_INTERNAL_U01_AVX2_LOADU_SI256(u)
        MCKL_RANDOM_INTERNAL_U01_AVX2_SRLI_EPI64(12)
        MCKL_RANDOM_INTERNAL_U01_AVX2_CVTEPU64_PD_52
        MCKL_RANDOM_INTERNAL_U01_AVX2_FMADD_PD(d52, d53)
        MCKL_RANDOM_INTERNAL_U01_AVX2_STOREU_PD(r)
    }
}; // class U01AVX2Impl

template <typename UIntType>
class U01CanonicalAVX2Impl<UIntType, float, 1, 32>
    : public U01CanonicalAVX2ImplBase<UIntType, float, 1>
{
    public:
    using U01CanonicalAVX2ImplBase<UIntType, float, 1>::eval;

    private:
    friend U01CanonicalAVX2ImplBase<UIntType, float, 1>;

    MCKL_INLINE static void eval(const UIntType *u, float *r)
    {
        const __m256 d32 = _mm256_set1_ps(Pow2<float, -32>::value);

        MCKL_RANDOM_INTERNAL_U01_AVX2_LOADU_SI256(u);
        MCKL_RANDOM_INTERNAL_U01_AVX2_CVTEPU32_PS
        MCKL_RANDOM_INTERNAL_U01_AVX2_MUL_PS(d32)
        MCKL_RANDOM_INTERNAL_U01_AVX2_STOREU_PS(r)
    }
}; // class U01CanonicalAVX2Impl

template <typename UIntType>
class U01CanonicalAVX2Impl<UIntType, double, 2, 32>
    : public U01CanonicalAVX2ImplBase<UIntType, double, 2>
{
    public:
    using U01CanonicalAVX2ImplBase<UIntType, double, 2>::eval;

    private:
    friend U01CanonicalAVX2ImplBase<UIntType, double, 2>;

    MCKL_INLINE static void eval(const UIntType *u, double *r)
    {
        const __m256d d32 = _mm256_set1_pd(Pow2<double, -32>::value);
        const __m256d d64 = _mm256_set1_pd(Pow2<double, -64>::value);
        const __m256d c = _mm256_set1_pd(Pow2<double, 52>::value);
        const __m256i l =
            _mm256_set1_epi64x(static_cast<MCKL_INT64>(0xFFFFFFFF));

        MCKL_RANDOM_INTERNAL_U01_AVX2_LOADU_SI256(u)

        __m256i zmm0 = _mm256_and_si256(ymm0, l);
        __m256i zmm1 = _mm256_and_si256(ymm1, l);
        __m256i zmm2 = _mm256_and_si256(ymm2, l);
        __m256i zmm3 = _mm256_and_si256(ymm3, l);
        __m256i zmm4 = _mm256_and_si256(ymm4, l);
        __m256i zmm5 = _mm256_and_si256(ymm5, l);
        __m256i zmm6 = _mm256_and_si256(ymm6, l);
        __m256i zmm7 = _mm256_and_si256(ymm7, l);

        MCKL_RANDOM_INTERNAL_U01_AVX2_SRLI_EPI64(32);

        ymm0 = _mm256_add_epi64(ymm0, _mm256_castpd_si256(c));
        ymm1 = _mm256_add_epi64(ymm1, _mm256_castpd_si256(c));
        ymm2 = _mm256_add_epi64(ymm2, _mm256_castpd_si256(c));
        ymm3 = _mm256_add_epi64(ymm3, _mm256_castpd_si256(c));
        ymm4 = _mm256_add_epi64(ymm4, _mm256_castpd_si256(c));
        ymm5 = _mm256_add_epi64(ymm5, _mm256_castpd_si256(c));
        ymm6 = _mm256_add_epi64(ymm6, _mm256_castpd_si256(c));
        ymm7 = _mm256_add_epi64(ymm7, _mm256_castpd_si256(c));

        zmm0 = _mm256_add_epi64(zmm0, _mm256_castpd_si256(c));
        zmm1 = _mm256_add_epi64(zmm1, _mm256_castpd_si256(c));
        zmm2 = _mm256_add_epi64(zmm2, _mm256_castpd_si256(c));
        zmm3 = _mm256_add_epi64(zmm3, _mm256_castpd_si256(c));
        zmm4 = _mm256_add_epi64(zmm4, _mm256_castpd_si256(c));
        zmm5 = _mm256_add_epi64(zmm5, _mm256_castpd_si256(c));
        zmm6 = _mm256_add_epi64(zmm6, _mm256_castpd_si256(c));
        zmm7 = _mm256_add_epi64(zmm7, _mm256_castpd_si256(c));

        ymm0 =
            _mm256_castpd_si256(_mm256_sub_pd(_mm256_castsi256_pd(ymm0), c));
        ymm1 =
            _mm256_castpd_si256(_mm256_sub_pd(_mm256_castsi256_pd(ymm1), c));
        ymm2 =
            _mm256_castpd_si256(_mm256_sub_pd(_mm256_castsi256_pd(ymm2), c));
        ymm3 =
            _mm256_castpd_si256(_mm256_sub_pd(_mm256_castsi256_pd(ymm3), c));
        ymm4 =
            _mm256_castpd_si256(_mm256_sub_pd(_mm256_castsi256_pd(ymm4), c));
        ymm5 =
            _mm256_castpd_si256(_mm256_sub_pd(_mm256_castsi256_pd(ymm5), c));
        ymm6 =
            _mm256_castpd_si256(_mm256_sub_pd(_mm256_castsi256_pd(ymm6), c));
        ymm7 =
            _mm256_castpd_si256(_mm256_sub_pd(_mm256_castsi256_pd(ymm7), c));

        zmm0 =
            _mm256_castpd_si256(_mm256_sub_pd(_mm256_castsi256_pd(zmm0), c));
        zmm1 =
            _mm256_castpd_si256(_mm256_sub_pd(_mm256_castsi256_pd(zmm1), c));
        zmm2 =
            _mm256_castpd_si256(_mm256_sub_pd(_mm256_castsi256_pd(zmm2), c));
        zmm3 =
            _mm256_castpd_si256(_mm256_sub_pd(_mm256_castsi256_pd(zmm3), c));
        zmm4 =
            _mm256_castpd_si256(_mm256_sub_pd(_mm256_castsi256_pd(zmm4), c));
        zmm5 =
            _mm256_castpd_si256(_mm256_sub_pd(_mm256_castsi256_pd(zmm5), c));
        zmm6 =
            _mm256_castpd_si256(_mm256_sub_pd(_mm256_castsi256_pd(zmm6), c));
        zmm7 =
            _mm256_castpd_si256(_mm256_sub_pd(_mm256_castsi256_pd(zmm7), c));

        MCKL_RANDOM_INTERNAL_U01_AVX2_MUL_PD(d32);

#if MCKL_USE_FMA

        ymm0 = _mm256_castpd_si256(_mm256_fmadd_pd(
            _mm256_castsi256_pd(zmm0), d64, _mm256_castsi256_pd(ymm0)));
        ymm1 = _mm256_castpd_si256(_mm256_fmadd_pd(
            _mm256_castsi256_pd(zmm1), d64, _mm256_castsi256_pd(ymm1)));
        ymm2 = _mm256_castpd_si256(_mm256_fmadd_pd(
            _mm256_castsi256_pd(zmm2), d64, _mm256_castsi256_pd(ymm2)));
        ymm3 = _mm256_castpd_si256(_mm256_fmadd_pd(
            _mm256_castsi256_pd(zmm3), d64, _mm256_castsi256_pd(ymm3)));
        ymm4 = _mm256_castpd_si256(_mm256_fmadd_pd(
            _mm256_castsi256_pd(zmm4), d64, _mm256_castsi256_pd(ymm4)));
        ymm5 = _mm256_castpd_si256(_mm256_fmadd_pd(
            _mm256_castsi256_pd(zmm5), d64, _mm256_castsi256_pd(ymm5)));
        ymm6 = _mm256_castpd_si256(_mm256_fmadd_pd(
            _mm256_castsi256_pd(zmm6), d64, _mm256_castsi256_pd(ymm6)));
        ymm7 = _mm256_castpd_si256(_mm256_fmadd_pd(
            _mm256_castsi256_pd(zmm7), d64, _mm256_castsi256_pd(ymm7)));

#else // MCKL_USE_FMA

        zmm0 =
            _mm256_castpd_si256(_mm256_mul_pd(_mm256_castsi256_pd(zmm0), d64));
        zmm1 =
            _mm256_castpd_si256(_mm256_mul_pd(_mm256_castsi256_pd(zmm1), d64));
        zmm2 =
            _mm256_castpd_si256(_mm256_mul_pd(_mm256_castsi256_pd(zmm2), d64));
        zmm3 =
            _mm256_castpd_si256(_mm256_mul_pd(_mm256_castsi256_pd(zmm3), d64));
        zmm4 =
            _mm256_castpd_si256(_mm256_mul_pd(_mm256_castsi256_pd(zmm4), d64));
        zmm5 =
            _mm256_castpd_si256(_mm256_mul_pd(_mm256_castsi256_pd(zmm5), d64));
        zmm6 =
            _mm256_castpd_si256(_mm256_mul_pd(_mm256_castsi256_pd(zmm6), d64));
        zmm7 =
            _mm256_castpd_si256(_mm256_mul_pd(_mm256_castsi256_pd(zmm7), d64));

        ymm0 = _mm256_castpd_si256(_mm256_add_pd(
            _mm256_castsi256_pd(ymm0), _mm256_castsi256_pd(zmm0)));
        ymm1 = _mm256_castpd_si256(_mm256_add_pd(
            _mm256_castsi256_pd(ymm1), _mm256_castsi256_pd(zmm1)));
        ymm2 = _mm256_castpd_si256(_mm256_add_pd(
            _mm256_castsi256_pd(ymm2), _mm256_castsi256_pd(zmm2)));
        ymm3 = _mm256_castpd_si256(_mm256_add_pd(
            _mm256_castsi256_pd(ymm3), _mm256_castsi256_pd(zmm3)));
        ymm4 = _mm256_castpd_si256(_mm256_add_pd(
            _mm256_castsi256_pd(ymm4), _mm256_castsi256_pd(zmm4)));
        ymm5 = _mm256_castpd_si256(_mm256_add_pd(
            _mm256_castsi256_pd(ymm5), _mm256_castsi256_pd(zmm5)));
        ymm6 = _mm256_castpd_si256(_mm256_add_pd(
            _mm256_castsi256_pd(ymm6), _mm256_castsi256_pd(zmm6)));
        ymm7 = _mm256_castpd_si256(_mm256_add_pd(
            _mm256_castsi256_pd(ymm7), _mm256_castsi256_pd(zmm7)));

#endif // MCKL_USE_FMA

        MCKL_RANDOM_INTERNAL_U01_AVX2_STOREU_PD(r)
    }
}; // class U01CanonicalAVX2Impl

template <typename UIntType>
class U01CanonicalAVX2Impl<UIntType, double, 1, 64>
    : public U01CanonicalAVX2ImplBase<UIntType, double, 1>
{
    public:
    using U01CanonicalAVX2ImplBase<UIntType, double, 1>::eval;

    private:
    friend U01CanonicalAVX2ImplBase<UIntType, double, 1>;

    MCKL_INLINE static void eval(const UIntType *u, double *r)
    {
        const __m256d d64 = _mm256_set1_pd(Pow2<double, -64>::value);

        MCKL_RANDOM_INTERNAL_U01_AVX2_LOADU_SI256(u)
        MCKL_RANDOM_INTERNAL_U01_AVX2_CVTEPU64_PD
        MCKL_RANDOM_INTERNAL_U01_AVX2_MUL_PD(d64)
        MCKL_RANDOM_INTERNAL_U01_AVX2_STOREU_PD(r)
    }
}; // class U01CanonicalAVX2Impl

} // namespace mckl::internal

} // namespace mckl

#ifdef MCKL_GCC
#if MCKL_GCC_VERSION >= 60000
#pragma GCC diagnostic pop
#endif
#endif

#endif // MCKL_RANDOM_INTERNAL_U01_AVX2_HPP
