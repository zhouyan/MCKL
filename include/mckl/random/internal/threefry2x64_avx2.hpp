//============================================================================
// MCKL/include/mckl/random/threefry2x64_avx2.hpp
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

template <typename T, std::size_t Rounds>
class ThreefryGeneratorImpl<T, 2, Rounds, ThreefryConstants<T, 2>, 64>
    : public ThreefryGeneratorAVX2Impl<T, 2, Rounds, ThreefryConstants<T, 2>,
          ThreefryGeneratorImpl<T, 2, Rounds, ThreefryConstants<T, 2>>>
{
    friend ThreefryGeneratorAVX2Impl<T, 2, Rounds, ThreefryConstants<T, 2>,
        ThreefryGeneratorImpl<T, 2, Rounds, ThreefryConstants<T, 2>>>;

    template <std::size_t N>
    static void rotate(std::array<__m256i, 8> &t)
    {
        using constants = ThreefryConstants<T, 2>;

        static constexpr int L = constants::rotate[0][(N - 1) % 8];
        static constexpr int R = 64 - L;

        __m256i l0 = _mm256_slli_epi64(std::get<0>(t), L);
        __m256i l1 = _mm256_slli_epi64(std::get<1>(t), L);
        __m256i l2 = _mm256_slli_epi64(std::get<2>(t), L);
        __m256i l3 = _mm256_slli_epi64(std::get<3>(t), L);
        __m256i l4 = _mm256_slli_epi64(std::get<4>(t), L);
        __m256i l5 = _mm256_slli_epi64(std::get<5>(t), L);
        __m256i l6 = _mm256_slli_epi64(std::get<6>(t), L);
        __m256i l7 = _mm256_slli_epi64(std::get<7>(t), L);

        __m256i r0 = _mm256_srli_epi64(std::get<0>(t), R);
        __m256i r1 = _mm256_srli_epi64(std::get<1>(t), R);
        __m256i r2 = _mm256_srli_epi64(std::get<2>(t), R);
        __m256i r3 = _mm256_srli_epi64(std::get<3>(t), R);
        __m256i r4 = _mm256_srli_epi64(std::get<4>(t), R);
        __m256i r5 = _mm256_srli_epi64(std::get<5>(t), R);
        __m256i r6 = _mm256_srli_epi64(std::get<6>(t), R);
        __m256i r7 = _mm256_srli_epi64(std::get<7>(t), R);

        std::get<0>(t) = _mm256_or_si256(l0, r0);
        std::get<1>(t) = _mm256_or_si256(l1, r1);
        std::get<2>(t) = _mm256_or_si256(l2, r2);
        std::get<3>(t) = _mm256_or_si256(l3, r3);
        std::get<4>(t) = _mm256_or_si256(l4, r4);
        std::get<5>(t) = _mm256_or_si256(l5, r5);
        std::get<6>(t) = _mm256_or_si256(l6, r6);
        std::get<7>(t) = _mm256_or_si256(l7, r7);
    }

    template <std::size_t>
    static void permute(std::array<__m256i, 8> &, std::array<__m256i, 8> &)
    {
    }
}; // class ThreefryGeneratorImpl
