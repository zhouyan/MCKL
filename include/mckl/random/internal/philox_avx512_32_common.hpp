//============================================================================
// MCKL/include/mckl/random/internal/philox_avx512_32_common.hpp
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

#ifndef MCKL_RANDOM_INTERNAL_PHILOX_AVX512_32_COMMON_HPP
#define MCKL_RANDOM_INTERNAL_PHILOX_AVX512_32_COMMON_HPP

// TODO Move to AVX512
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
        const __m256i ymmm = _mm256_set_epi32(0, m3, 0, m2, 0, m1, 0, m0);    \
        const __m256i ymm8 = _mm256_mul_epu32(ymm0, ymmm);                    \
        const __m256i ymm9 = _mm256_mul_epu32(ymm1, ymmm);                    \
        const __m256i ymmA = _mm256_mul_epu32(ymm2, ymmm);                    \
        const __m256i ymmB = _mm256_mul_epu32(ymm3, ymmm);                    \
        const __m256i ymmC = _mm256_mul_epu32(ymm4, ymmm);                    \
        const __m256i ymmD = _mm256_mul_epu32(ymm5, ymmm);                    \
        const __m256i ymmE = _mm256_mul_epu32(ymm6, ymmm);                    \
        const __m256i ymmF = _mm256_mul_epu32(ymm7, ymmm);                    \
                                                                              \
        constexpr int ma = static_cast<int>(0xFFFFFFFF);                      \
        const __m256i ymma = _mm256_set_epi32(ma, 0, ma, 0, ma, 0, ma, 0);    \
        ymm0 = _mm256_and_si256(ymm0, ymma);                                  \
        ymm1 = _mm256_and_si256(ymm1, ymma);                                  \
        ymm2 = _mm256_and_si256(ymm2, ymma);                                  \
        ymm3 = _mm256_and_si256(ymm3, ymma);                                  \
        ymm4 = _mm256_and_si256(ymm4, ymma);                                  \
        ymm5 = _mm256_and_si256(ymm5, ymma);                                  \
        ymm6 = _mm256_and_si256(ymm6, ymma);                                  \
        ymm7 = _mm256_and_si256(ymm7, ymma);                                  \
                                                                              \
        constexpr int w0 =                                                    \
            static_cast<int>(Constants::weyl::value[0 % (K / 2)] * N);        \
        constexpr int w1 =                                                    \
            static_cast<int>(Constants::weyl::value[1 % (K / 2)] * N);        \
        constexpr int w2 =                                                    \
            static_cast<int>(Constants::weyl::value[2 % (K / 2)] * N);        \
        constexpr int w3 =                                                    \
            static_cast<int>(Constants::weyl::value[3 % (K / 2)] * N);        \
        const __m256i ymmw = _mm256_set_epi32(w3, 0, w2, 0, w1, 0, w0, 0);    \
        const __m256i ymmk = _mm256_add_epi32(ymmk0, ymmw);                   \
        ymm0 = _mm256_xor_si256(ymm0, ymmk);                                  \
        ymm1 = _mm256_xor_si256(ymm1, ymmk);                                  \
        ymm2 = _mm256_xor_si256(ymm2, ymmk);                                  \
        ymm3 = _mm256_xor_si256(ymm3, ymmk);                                  \
        ymm4 = _mm256_xor_si256(ymm4, ymmk);                                  \
        ymm5 = _mm256_xor_si256(ymm5, ymmk);                                  \
        ymm6 = _mm256_xor_si256(ymm6, ymmk);                                  \
        ymm7 = _mm256_xor_si256(ymm7, ymmk);                                  \
                                                                              \
        ymm0 = _mm256_xor_si256(ymm0, ymm8);                                  \
        ymm1 = _mm256_xor_si256(ymm1, ymm9);                                  \
        ymm2 = _mm256_xor_si256(ymm2, ymmA);                                  \
        ymm3 = _mm256_xor_si256(ymm3, ymmB);                                  \
        ymm4 = _mm256_xor_si256(ymm4, ymmC);                                  \
        ymm5 = _mm256_xor_si256(ymm5, ymmD);                                  \
        ymm6 = _mm256_xor_si256(ymm6, ymmE);                                  \
        ymm7 = _mm256_xor_si256(ymm7, ymmF);                                  \
                                                                              \
        ymm0 = _mm256_shuffle_epi32(ymm0, imm8);                              \
        ymm1 = _mm256_shuffle_epi32(ymm1, imm8);                              \
        ymm2 = _mm256_shuffle_epi32(ymm2, imm8);                              \
        ymm3 = _mm256_shuffle_epi32(ymm3, imm8);                              \
        ymm4 = _mm256_shuffle_epi32(ymm4, imm8);                              \
        ymm5 = _mm256_shuffle_epi32(ymm5, imm8);                              \
        ymm6 = _mm256_shuffle_epi32(ymm6, imm8);                              \
        ymm7 = _mm256_shuffle_epi32(ymm7, imm8);                              \
    }

#endif // MCKL_RANDOM_INTERNAL_PHILOX_AVX512_32_COMMON_HPP
