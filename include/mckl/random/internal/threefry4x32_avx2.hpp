//============================================================================
// MCKL/include/mckl/random/threefry4x32_avx2.hpp
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

// Packing scheme
// s: 2''' 2' 0''' 0' 2'' 2 0'' 0
// t: 3''' 3' 1''' 1' 3'' 3 1'' 1
template <typename T, std::size_t Rounds>
class ThreefryGeneratorImpl<T, 4, Rounds, ThreefryConstants<T, 4>, 32>
    : public ThreefryGeneratorAVX2Impl<T, 4, Rounds, ThreefryConstants<T, 4>,
          ThreefryGeneratorImpl<T, 4, Rounds, ThreefryConstants<T, 4>>>
{
    friend ThreefryGeneratorAVX2Impl<T, 4, Rounds, ThreefryConstants<T, 4>,
        ThreefryGeneratorImpl<T, 4, Rounds, ThreefryConstants<T, 4>>>;

    template <std::size_t N>
    static void rotate(std::array<__m256i, 8> &t)
    {
        using constants = ThreefryConstants<T, 4>;

        static constexpr int L0 = constants::rotate[0][(N - 1) % 8];
        static constexpr int L1 = constants::rotate[1][(N - 1) % 8];
        static constexpr int R0 = 32 - L0;
        static constexpr int R1 = 32 - L1;

        __m256i x0 = _mm256_unpacklo_epi64(std::get<0>(t), std::get<1>(t));
        __m256i x1 = _mm256_unpacklo_epi64(std::get<2>(t), std::get<3>(t));
        __m256i x2 = _mm256_unpacklo_epi64(std::get<4>(t), std::get<5>(t));
        __m256i x3 = _mm256_unpacklo_epi64(std::get<6>(t), std::get<7>(t));

        __m256i x4 = _mm256_unpackhi_epi64(std::get<0>(t), std::get<1>(t));
        __m256i x5 = _mm256_unpackhi_epi64(std::get<2>(t), std::get<3>(t));
        __m256i x6 = _mm256_unpackhi_epi64(std::get<4>(t), std::get<5>(t));
        __m256i x7 = _mm256_unpackhi_epi64(std::get<6>(t), std::get<7>(t));

        __m256i l0 = _mm256_slli_epi32(x0, L0);
        __m256i l1 = _mm256_slli_epi32(x1, L0);
        __m256i l2 = _mm256_slli_epi32(x2, L0);
        __m256i l3 = _mm256_slli_epi32(x3, L0);
        __m256i l4 = _mm256_slli_epi32(x4, L1);
        __m256i l5 = _mm256_slli_epi32(x5, L1);
        __m256i l6 = _mm256_slli_epi32(x6, L1);
        __m256i l7 = _mm256_slli_epi32(x7, L1);

        __m256i r0 = _mm256_srli_epi32(x0, R0);
        __m256i r1 = _mm256_srli_epi32(x1, R0);
        __m256i r2 = _mm256_srli_epi32(x2, R0);
        __m256i r3 = _mm256_srli_epi32(x3, R0);
        __m256i r4 = _mm256_srli_epi32(x4, R1);
        __m256i r5 = _mm256_srli_epi32(x5, R1);
        __m256i r6 = _mm256_srli_epi32(x6, R1);
        __m256i r7 = _mm256_srli_epi32(x7, R1);

        x0 = _mm256_or_si256(l0, r0);
        x1 = _mm256_or_si256(l1, r1);
        x2 = _mm256_or_si256(l2, r2);
        x3 = _mm256_or_si256(l3, r3);
        x4 = _mm256_or_si256(l4, r4);
        x5 = _mm256_or_si256(l5, r5);
        x6 = _mm256_or_si256(l6, r6);
        x7 = _mm256_or_si256(l7, r7);

        std::get<0>(t) = _mm256_unpacklo_epi64(x0, x4);
        std::get<2>(t) = _mm256_unpacklo_epi64(x1, x5);
        std::get<4>(t) = _mm256_unpacklo_epi64(x2, x6);
        std::get<6>(t) = _mm256_unpacklo_epi64(x3, x7);

        std::get<1>(t) = _mm256_unpackhi_epi64(x0, x4);
        std::get<3>(t) = _mm256_unpackhi_epi64(x1, x5);
        std::get<5>(t) = _mm256_unpackhi_epi64(x2, x6);
        std::get<7>(t) = _mm256_unpackhi_epi64(x3, x7);
    }

    template <std::size_t>
    static void permute(std::array<__m256i, 8> &, std::array<__m256i, 8> &t)
    {
        // 1 0 3 2
        std::get<0>(t) = _mm256_shuffle_epi32(std::get<0>(t), 0x4E);
        std::get<1>(t) = _mm256_shuffle_epi32(std::get<1>(t), 0x4E);
        std::get<2>(t) = _mm256_shuffle_epi32(std::get<2>(t), 0x4E);
        std::get<3>(t) = _mm256_shuffle_epi32(std::get<3>(t), 0x4E);
        std::get<4>(t) = _mm256_shuffle_epi32(std::get<4>(t), 0x4E);
        std::get<5>(t) = _mm256_shuffle_epi32(std::get<5>(t), 0x4E);
        std::get<6>(t) = _mm256_shuffle_epi32(std::get<6>(t), 0x4E);
        std::get<7>(t) = _mm256_shuffle_epi32(std::get<7>(t), 0x4E);
    }
}; // class ThreefryGeneratorImpl
