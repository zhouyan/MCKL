//============================================================================
// MCKL/include/mckl/random/threefry8x64_avx2.hpp
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
// s: 6 2 4 0
// t: 7 3 5 1
template <typename T, std::size_t Rounds>
class ThreefryGeneratorImpl<T, 8, Rounds, ThreefryConstants<T, 8>, 64>
    : public ThreefryGeneratorAVX2Impl<T, 8, Rounds, ThreefryConstants<T, 8>,
          ThreefryGeneratorImpl<T, 8, Rounds, ThreefryConstants<T, 8>>>
{
    friend ThreefryGeneratorAVX2Impl<T, 8, Rounds, ThreefryConstants<T, 8>,
        ThreefryGeneratorImpl<T, 8, Rounds, ThreefryConstants<T, 8>>>;

    template <std::size_t>
    static void permute(std::array<__m256i, 8> &s, std::array<__m256i, 8> &t)
    {
        // 0 1 3 2
        std::get<0>(s) = _mm256_permute4x64_epi64(std::get<0>(s), 0x1E);
        std::get<1>(s) = _mm256_permute4x64_epi64(std::get<1>(s), 0x1E);
        std::get<2>(s) = _mm256_permute4x64_epi64(std::get<2>(s), 0x1E);
        std::get<3>(s) = _mm256_permute4x64_epi64(std::get<3>(s), 0x1E);
        std::get<4>(s) = _mm256_permute4x64_epi64(std::get<4>(s), 0x1E);
        std::get<5>(s) = _mm256_permute4x64_epi64(std::get<5>(s), 0x1E);
        std::get<6>(s) = _mm256_permute4x64_epi64(std::get<6>(s), 0x1E);
        std::get<7>(s) = _mm256_permute4x64_epi64(std::get<7>(s), 0x1E);

        // 2 3 1 0
        std::get<0>(t) = _mm256_permute4x64_epi64(std::get<0>(t), 0xB4);
        std::get<1>(t) = _mm256_permute4x64_epi64(std::get<1>(t), 0xB4);
        std::get<2>(t) = _mm256_permute4x64_epi64(std::get<2>(t), 0xB4);
        std::get<3>(t) = _mm256_permute4x64_epi64(std::get<3>(t), 0xB4);
        std::get<4>(t) = _mm256_permute4x64_epi64(std::get<4>(t), 0xB4);
        std::get<5>(t) = _mm256_permute4x64_epi64(std::get<5>(t), 0xB4);
        std::get<6>(t) = _mm256_permute4x64_epi64(std::get<6>(t), 0xB4);
        std::get<7>(t) = _mm256_permute4x64_epi64(std::get<7>(t), 0xB4);
    }
}; // class ThreefryGeneratorImpl
