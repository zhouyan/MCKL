//============================================================================
// MCKL/include/mckl/random/internal/philox_avx512_32_common.hpp
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

#ifndef MCKL_RANDOM_INTERNAL_PHILOX_AVX512_32_COMMON_HPP
#define MCKL_RANDOM_INTERNAL_PHILOX_AVX512_32_COMMON_HPP

#define MCKL_RANDOM_INTERNAL_PHILOX_AVX512_32_RBOX(K, N, imm8)                \
    {                                                                         \
        constexpr int m0 =                                                    \
            static_cast<int>(Constants::multiplier::value[0 % (K / 2)]);      \
        constexpr int m1 =                                                    \
            static_cast<int>(Constants::multiplier::value[1 % (K / 2)]);      \
        constexpr int m2 =                                                    \
            static_cast<int>(Constants::multiplier::value[2 % (K / 2)]);      \
        constexpr int m3 =                                                    \
            static_cast<int>(Constants::multiplier::value[3 % (K / 2)]);      \
        constexpr int m4 =                                                    \
            static_cast<int>(Constants::multiplier::value[4 % (K / 2)]);      \
        constexpr int m5 =                                                    \
            static_cast<int>(Constants::multiplier::value[5 % (K / 2)]);      \
        constexpr int m6 =                                                    \
            static_cast<int>(Constants::multiplier::value[6 % (K / 2)]);      \
        constexpr int m7 =                                                    \
            static_cast<int>(Constants::multiplier::value[7 % (K / 2)]);      \
        const __m512i ymmm = _mm512_set_epi32(                                \
            0, m7, 0, m6, 0, m5, 0, m4, 0, m3, 0, m2, 0, m1, 0, m0);          \
        const __m512i ymm8 = _mm512_mul_epu32(ymm0, ymmm);                    \
        const __m512i ymm9 = _mm512_mul_epu32(ymm1, ymmm);                    \
        const __m512i ymmA = _mm512_mul_epu32(ymm2, ymmm);                    \
        const __m512i ymmB = _mm512_mul_epu32(ymm3, ymmm);                    \
        const __m512i ymmC = _mm512_mul_epu32(ymm4, ymmm);                    \
        const __m512i ymmD = _mm512_mul_epu32(ymm5, ymmm);                    \
        const __m512i ymmE = _mm512_mul_epu32(ymm6, ymmm);                    \
        const __m512i ymmF = _mm512_mul_epu32(ymm7, ymmm);                    \
                                                                              \
        constexpr int ma = static_cast<int>(0xFFFFFFFF);                      \
        const __m512i ymma = _mm512_set_epi32(                                \
            ma, 0, ma, 0, ma, 0, ma, 0, ma, 0, ma, 0, ma, 0, ma, 0);          \
        ymm0 = _mm512_and_si512(ymm0, ymma);                                  \
        ymm1 = _mm512_and_si512(ymm1, ymma);                                  \
        ymm2 = _mm512_and_si512(ymm2, ymma);                                  \
        ymm3 = _mm512_and_si512(ymm3, ymma);                                  \
        ymm4 = _mm512_and_si512(ymm4, ymma);                                  \
        ymm5 = _mm512_and_si512(ymm5, ymma);                                  \
        ymm6 = _mm512_and_si512(ymm6, ymma);                                  \
        ymm7 = _mm512_and_si512(ymm7, ymma);                                  \
                                                                              \
        constexpr int w0 =                                                    \
            static_cast<int>(Constants::weyl::value[0 % (K / 2)] * N);        \
        constexpr int w1 =                                                    \
            static_cast<int>(Constants::weyl::value[1 % (K / 2)] * N);        \
        constexpr int w2 =                                                    \
            static_cast<int>(Constants::weyl::value[2 % (K / 2)] * N);        \
        constexpr int w3 =                                                    \
            static_cast<int>(Constants::weyl::value[3 % (K / 2)] * N);        \
        constexpr int w4 =                                                    \
            static_cast<int>(Constants::weyl::value[4 % (K / 2)] * N);        \
        constexpr int w5 =                                                    \
            static_cast<int>(Constants::weyl::value[5 % (K / 2)] * N);        \
        constexpr int w6 =                                                    \
            static_cast<int>(Constants::weyl::value[6 % (K / 2)] * N);        \
        constexpr int w7 =                                                    \
            static_cast<int>(Constants::weyl::value[7 % (K / 2)] * N);        \
        const __m512i ymmw = _mm512_set_epi32(                                \
            w7, 0, w6, 0, w5, 0, w4, 0, w3, 0, w2, 0, w1, 0, w0, 0);          \
        const __m512i ymmk = _mm512_add_epi32(ymmk0, ymmw);                   \
        ymm0 = _mm512_xor_si512(ymm0, ymmk);                                  \
        ymm1 = _mm512_xor_si512(ymm1, ymmk);                                  \
        ymm2 = _mm512_xor_si512(ymm2, ymmk);                                  \
        ymm3 = _mm512_xor_si512(ymm3, ymmk);                                  \
        ymm4 = _mm512_xor_si512(ymm4, ymmk);                                  \
        ymm5 = _mm512_xor_si512(ymm5, ymmk);                                  \
        ymm6 = _mm512_xor_si512(ymm6, ymmk);                                  \
        ymm7 = _mm512_xor_si512(ymm7, ymmk);                                  \
                                                                              \
        ymm0 = _mm512_xor_si512(ymm0, ymm8);                                  \
        ymm1 = _mm512_xor_si512(ymm1, ymm9);                                  \
        ymm2 = _mm512_xor_si512(ymm2, ymmA);                                  \
        ymm3 = _mm512_xor_si512(ymm3, ymmB);                                  \
        ymm4 = _mm512_xor_si512(ymm4, ymmC);                                  \
        ymm5 = _mm512_xor_si512(ymm5, ymmD);                                  \
        ymm6 = _mm512_xor_si512(ymm6, ymmE);                                  \
        ymm7 = _mm512_xor_si512(ymm7, ymmF);                                  \
                                                                              \
        ymm0 = _mm512_shuffle_epi32(ymm0, imm8);                              \
        ymm1 = _mm512_shuffle_epi32(ymm1, imm8);                              \
        ymm2 = _mm512_shuffle_epi32(ymm2, imm8);                              \
        ymm3 = _mm512_shuffle_epi32(ymm3, imm8);                              \
        ymm4 = _mm512_shuffle_epi32(ymm4, imm8);                              \
        ymm5 = _mm512_shuffle_epi32(ymm5, imm8);                              \
        ymm6 = _mm512_shuffle_epi32(ymm6, imm8);                              \
        ymm7 = _mm512_shuffle_epi32(ymm7, imm8);                              \
    }

#endif // MCKL_RANDOM_INTERNAL_PHILOX_AVX512_32_COMMON_HPP
