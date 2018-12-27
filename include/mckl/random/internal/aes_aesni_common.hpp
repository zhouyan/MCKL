//============================================================================
// MCKL/include/mckl/random/internal/aes_aesni_common.hpp
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

#ifndef MCKL_RANDOM_INTERNAL_AES_AESNI_COMMON_HPP
#define MCKL_RANDOM_INTERNAL_AES_AESNI_COMMON_HPP

#define MCKL_RANDOM_INTERNAL_AES_AESNI_ENCFIRST(xmmk)                         \
    xmm0 = _mm_xor_si128(xmm0, xmmk);                                         \
    xmm1 = _mm_xor_si128(xmm1, xmmk);                                         \
    xmm2 = _mm_xor_si128(xmm2, xmmk);                                         \
    xmm3 = _mm_xor_si128(xmm3, xmmk);                                         \
    xmm4 = _mm_xor_si128(xmm4, xmmk);                                         \
    xmm5 = _mm_xor_si128(xmm5, xmmk);                                         \
    xmm6 = _mm_xor_si128(xmm6, xmmk);                                         \
    xmm7 = _mm_xor_si128(xmm7, xmmk);

#define MCKL_RANDOM_INTERNAL_AES_AESNI_ENC(xmmk)                              \
    xmm0 = _mm_aesenc_si128(xmm0, xmmk);                                      \
    xmm1 = _mm_aesenc_si128(xmm1, xmmk);                                      \
    xmm2 = _mm_aesenc_si128(xmm2, xmmk);                                      \
    xmm3 = _mm_aesenc_si128(xmm3, xmmk);                                      \
    xmm4 = _mm_aesenc_si128(xmm4, xmmk);                                      \
    xmm5 = _mm_aesenc_si128(xmm5, xmmk);                                      \
    xmm6 = _mm_aesenc_si128(xmm6, xmmk);                                      \
    xmm7 = _mm_aesenc_si128(xmm7, xmmk);

#define MCKL_RANDOM_INTERNAL_AES_AESNI_ENCLAST(xmmk)                          \
    xmm0 = _mm_aesenclast_si128(xmm0, xmmk);                                  \
    xmm1 = _mm_aesenclast_si128(xmm1, xmmk);                                  \
    xmm2 = _mm_aesenclast_si128(xmm2, xmmk);                                  \
    xmm3 = _mm_aesenclast_si128(xmm3, xmmk);                                  \
    xmm4 = _mm_aesenclast_si128(xmm4, xmmk);                                  \
    xmm5 = _mm_aesenclast_si128(xmm5, xmmk);                                  \
    xmm6 = _mm_aesenclast_si128(xmm6, xmmk);                                  \
    xmm7 = _mm_aesenclast_si128(xmm7, xmmk);

#define MCKL_RANDOM_INTERNAL_AES_AESNI_ENCFIRST_MEM(k)                        \
    {                                                                         \
        const __m128i xmmk = k;                                               \
        xmm0 = _mm_xor_si128(xmm0, xmmk);                                     \
        xmm1 = _mm_xor_si128(xmm1, xmmk);                                     \
        xmm2 = _mm_xor_si128(xmm2, xmmk);                                     \
        xmm3 = _mm_xor_si128(xmm3, xmmk);                                     \
        xmm4 = _mm_xor_si128(xmm4, xmmk);                                     \
        xmm5 = _mm_xor_si128(xmm5, xmmk);                                     \
        xmm6 = _mm_xor_si128(xmm6, xmmk);                                     \
        xmm7 = _mm_xor_si128(xmm7, xmmk);                                     \
    }

#define MCKL_RANDOM_INTERNAL_AES_AESNI_ENC_MEM(k)                             \
    {                                                                         \
        const __m128i xmmk = k;                                               \
        xmm0 = _mm_aesenc_si128(xmm0, xmmk);                                  \
        xmm1 = _mm_aesenc_si128(xmm1, xmmk);                                  \
        xmm2 = _mm_aesenc_si128(xmm2, xmmk);                                  \
        xmm3 = _mm_aesenc_si128(xmm3, xmmk);                                  \
        xmm4 = _mm_aesenc_si128(xmm4, xmmk);                                  \
        xmm5 = _mm_aesenc_si128(xmm5, xmmk);                                  \
        xmm6 = _mm_aesenc_si128(xmm6, xmmk);                                  \
        xmm7 = _mm_aesenc_si128(xmm7, xmmk);                                  \
    }

#define MCKL_RANDOM_INTERNAL_AES_AESNI_ENCLAST_MEM(k)                         \
    {                                                                         \
        const __m128i xmmk = k;                                               \
        xmm0 = _mm_aesenclast_si128(xmm0, xmmk);                              \
        xmm1 = _mm_aesenclast_si128(xmm1, xmmk);                              \
        xmm2 = _mm_aesenclast_si128(xmm2, xmmk);                              \
        xmm3 = _mm_aesenclast_si128(xmm3, xmmk);                              \
        xmm4 = _mm_aesenclast_si128(xmm4, xmmk);                              \
        xmm5 = _mm_aesenclast_si128(xmm5, xmmk);                              \
        xmm6 = _mm_aesenclast_si128(xmm6, xmmk);                              \
        xmm7 = _mm_aesenclast_si128(xmm7, xmmk);                              \
    }

#define MCKL_RANDOM_INTERNAL_AES_AESNI_EVAL                                   \
    template <typename ResultType>                                            \
    static void eval(std::array<std::uint64_t, 2> &ctr, std::size_t n,        \
        ResultType *r, const KeySeqType &ks)                                  \
    {                                                                         \
        constexpr std::size_t R = sizeof(__m128i) / sizeof(ResultType);       \
                                                                              \
        const std::size_t n0 =                                                \
            static_cast<std::size_t>(std::min(static_cast<std::uint64_t>(n),  \
                std::numeric_limits<std::uint64_t>::max() - ctr.front()));    \
                                                                              \
        eval_kernel(ctr, n0, r, ks);                                          \
        n -= n0;                                                              \
        r += n0 * R;                                                          \
                                                                              \
        if (n != 0) {                                                         \
            eval(ctr, r, ks);                                                 \
            n -= 1;                                                           \
            r += R;                                                           \
        }                                                                     \
                                                                              \
        eval_kernel(ctr, n, r, ks);                                           \
    }

#define MCKL_RANDOM_INTERNAL_AES_AESNI_STORE(n, N, rptr)                      \
    if (n >= N) {                                                             \
        n -= N;                                                               \
        _mm_storeu_si128(rptr++, xmm0);                                       \
        _mm_storeu_si128(rptr++, xmm1);                                       \
        _mm_storeu_si128(rptr++, xmm2);                                       \
        _mm_storeu_si128(rptr++, xmm3);                                       \
        _mm_storeu_si128(rptr++, xmm4);                                       \
        _mm_storeu_si128(rptr++, xmm5);                                       \
        _mm_storeu_si128(rptr++, xmm6);                                       \
        _mm_storeu_si128(rptr++, xmm7);                                       \
    } else {                                                                  \
        std::array<__m128i, S> s;                                             \
        std::get<0>(s) = xmm0;                                                \
        std::get<1>(s) = xmm1;                                                \
        std::get<2>(s) = xmm2;                                                \
        std::get<3>(s) = xmm3;                                                \
        std::get<4>(s) = xmm4;                                                \
        std::get<5>(s) = xmm5;                                                \
        std::get<6>(s) = xmm6;                                                \
        std::get<7>(s) = xmm7;                                                \
        std::memcpy(rptr, s.data(), n * sizeof(__m128i));                     \
        break;                                                                \
    }

#endif // MCKL_RANDOM_INTERNAL_AES_AESNI_COMMON_HPP
