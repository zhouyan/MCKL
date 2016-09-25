//============================================================================
// MCKL/include/mckl/random/threefry16x64_avx2.hpp
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
// s0 | s1 : 6 2 4 0 | E A C 8
// t0 | t1 : 7 3 5 1 | F B D 9
template <typename T, std::size_t Rounds>
class ThreefryGeneratorImpl<T, 16, Rounds, ThreefryConstants<T, 16>, 64>
    : public ThreefryGeneratorAVX2Impl<T, 16, Rounds, ThreefryConstants<T, 16>,
          ThreefryGeneratorImpl<T, 16, Rounds, ThreefryConstants<T, 16>>>
{
    friend ThreefryGeneratorAVX2Impl<T, 16, Rounds, ThreefryConstants<T, 16>,
        ThreefryGeneratorImpl<T, 16, Rounds, ThreefryConstants<T, 16>>>;

    template <std::size_t>
    static void permute(std::array<__m256i, 8> &s, std::array<__m256i, 8> &t)
    {
        // 1 2 3 0
        std::get<0>(s) = _mm256_permute4x64_epi64(std::get<0>(s), 0x6C);
        std::get<2>(s) = _mm256_permute4x64_epi64(std::get<2>(s), 0x6C);
        std::get<4>(s) = _mm256_permute4x64_epi64(std::get<4>(s), 0x6C);
        std::get<6>(s) = _mm256_permute4x64_epi64(std::get<6>(s), 0x6C);

        // 0 1 3 2
        std::get<1>(s) = _mm256_permute4x64_epi64(std::get<1>(s), 0x1E);
        std::get<3>(s) = _mm256_permute4x64_epi64(std::get<3>(s), 0x1E);
        std::get<5>(s) = _mm256_permute4x64_epi64(std::get<5>(s), 0x1E);
        std::get<7>(s) = _mm256_permute4x64_epi64(std::get<7>(s), 0x1E);

        // 3 1 2 0
        __m256i t0 = _mm256_permute4x64_epi64(std::get<1>(t), 0xD8);
        __m256i t2 = _mm256_permute4x64_epi64(std::get<3>(t), 0xD8);
        __m256i t4 = _mm256_permute4x64_epi64(std::get<5>(t), 0xD8);
        __m256i t6 = _mm256_permute4x64_epi64(std::get<7>(t), 0xD8);

        // 0 2 1 3
        std::get<1>(t) = _mm256_permute4x64_epi64(std::get<0>(t), 0x27);
        std::get<3>(t) = _mm256_permute4x64_epi64(std::get<2>(t), 0x27);
        std::get<5>(t) = _mm256_permute4x64_epi64(std::get<4>(t), 0x27);
        std::get<7>(t) = _mm256_permute4x64_epi64(std::get<6>(t), 0x27);

        std::get<0>(t) = t0;
        std::get<2>(t) = t2;
        std::get<4>(t) = t4;
        std::get<6>(t) = t6;
    }
}; // class ThreefryGeneratorImpl
