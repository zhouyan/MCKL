//============================================================================
// MCKL/include/mckl/random/internal/philox_sse2_32_common.hpp
//----------------------------------------------------------------------------
// MCKL: Monte Carlo Kernel Library
//----------------------------------------------------------------------------
// Copyright (c) 2013-2018, Yan Zhou
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

#ifndef MCKL_RANDOM_INTERNAL_PHILOX_SSE2_32_COMMON_HPP
#define MCKL_RANDOM_INTERNAL_PHILOX_SSE2_32_COMMON_HPP

#define MCKL_RANDOM_INTERNAL_PHILOX_SSE2_32_RBOX(K, N, imm8)                  \
    {                                                                         \
        constexpr int m0 =                                                    \
            static_cast<int>(Constants::multiplier::value[0 % (K / 2)]);      \
        constexpr int m1 =                                                    \
            static_cast<int>(Constants::multiplier::value[1 % (K / 2)]);      \
        const __m128i xmmm = _mm_set_epi32(0, m1, 0, m0);                     \
        const __m128i xmm8 = _mm_mul_epu32(xmm0, xmmm);                       \
        const __m128i xmm9 = _mm_mul_epu32(xmm1, xmmm);                       \
        const __m128i xmmA = _mm_mul_epu32(xmm2, xmmm);                       \
        const __m128i xmmB = _mm_mul_epu32(xmm3, xmmm);                       \
        const __m128i xmmC = _mm_mul_epu32(xmm4, xmmm);                       \
        const __m128i xmmD = _mm_mul_epu32(xmm5, xmmm);                       \
        const __m128i xmmE = _mm_mul_epu32(xmm6, xmmm);                       \
        const __m128i xmmF = _mm_mul_epu32(xmm7, xmmm);                       \
                                                                              \
        constexpr int ma = static_cast<int>(0xFFFFFFFF);                      \
        const __m128i xmma = _mm_set_epi32(ma, 0, ma, 0);                     \
        xmm0 = _mm_and_si128(xmm0, xmma);                                     \
        xmm1 = _mm_and_si128(xmm1, xmma);                                     \
        xmm2 = _mm_and_si128(xmm2, xmma);                                     \
        xmm3 = _mm_and_si128(xmm3, xmma);                                     \
        xmm4 = _mm_and_si128(xmm4, xmma);                                     \
        xmm5 = _mm_and_si128(xmm5, xmma);                                     \
        xmm6 = _mm_and_si128(xmm6, xmma);                                     \
        xmm7 = _mm_and_si128(xmm7, xmma);                                     \
                                                                              \
        constexpr int w0 =                                                    \
            static_cast<int>(Constants::weyl::value[0 % (K / 2)] * N);        \
        constexpr int w1 =                                                    \
            static_cast<int>(Constants::weyl::value[1 % (K / 2)] * N);        \
        const __m128i xmmw = _mm_set_epi32(w1, 0, w0, 0);                     \
        const __m128i xmmk = _mm_add_epi32(xmmk0, xmmw);                      \
        xmm0 = _mm_xor_si128(xmm0, xmmk);                                     \
        xmm1 = _mm_xor_si128(xmm1, xmmk);                                     \
        xmm2 = _mm_xor_si128(xmm2, xmmk);                                     \
        xmm3 = _mm_xor_si128(xmm3, xmmk);                                     \
        xmm4 = _mm_xor_si128(xmm4, xmmk);                                     \
        xmm5 = _mm_xor_si128(xmm5, xmmk);                                     \
        xmm6 = _mm_xor_si128(xmm6, xmmk);                                     \
        xmm7 = _mm_xor_si128(xmm7, xmmk);                                     \
                                                                              \
        xmm0 = _mm_xor_si128(xmm0, xmm8);                                     \
        xmm1 = _mm_xor_si128(xmm1, xmm9);                                     \
        xmm2 = _mm_xor_si128(xmm2, xmmA);                                     \
        xmm3 = _mm_xor_si128(xmm3, xmmB);                                     \
        xmm4 = _mm_xor_si128(xmm4, xmmC);                                     \
        xmm5 = _mm_xor_si128(xmm5, xmmD);                                     \
        xmm6 = _mm_xor_si128(xmm6, xmmE);                                     \
        xmm7 = _mm_xor_si128(xmm7, xmmF);                                     \
                                                                              \
        xmm0 = _mm_shuffle_epi32(xmm0, imm8);                                 \
        xmm1 = _mm_shuffle_epi32(xmm1, imm8);                                 \
        xmm2 = _mm_shuffle_epi32(xmm2, imm8);                                 \
        xmm3 = _mm_shuffle_epi32(xmm3, imm8);                                 \
        xmm4 = _mm_shuffle_epi32(xmm4, imm8);                                 \
        xmm5 = _mm_shuffle_epi32(xmm5, imm8);                                 \
        xmm6 = _mm_shuffle_epi32(xmm6, imm8);                                 \
        xmm7 = _mm_shuffle_epi32(xmm7, imm8);                                 \
    }

#endif // MCKL_RANDOM_INTERNAL_PHILOX_SSE2_32_COMMON_HPP
