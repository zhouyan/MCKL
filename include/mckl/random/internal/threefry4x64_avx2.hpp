//============================================================================
// MCKL/include/mckl/random/threefry4x64_avx2.hpp
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
// s: 2' 2 0' 0
// t: 3' 3 1' 1
template <typename T, std::size_t Rounds>
class ThreefryGeneratorImpl<T, 4, Rounds, ThreefryConstants<T, 4>, 64>
    : public ThreefryGeneratorAVX2Impl<T, 4, Rounds, ThreefryConstants<T, 4>,
          ThreefryGeneratorImpl<T, 4, Rounds, ThreefryConstants<T, 4>>>
{
    friend ThreefryGeneratorAVX2Impl<T, 4, Rounds, ThreefryConstants<T, 4>,
        ThreefryGeneratorImpl<T, 4, Rounds, ThreefryConstants<T, 4>>>;

    template <std::size_t>
    static void permute(std::array<__m256i, 8> &, std::array<__m256i, 8> &t)
    {
        // 1 0 3 2
        std::get<0>(t) = _mm256_permute4x64_epi64(std::get<0>(t), 0x4E);
        std::get<1>(t) = _mm256_permute4x64_epi64(std::get<1>(t), 0x4E);
        std::get<2>(t) = _mm256_permute4x64_epi64(std::get<2>(t), 0x4E);
        std::get<3>(t) = _mm256_permute4x64_epi64(std::get<3>(t), 0x4E);
        std::get<4>(t) = _mm256_permute4x64_epi64(std::get<4>(t), 0x4E);
        std::get<5>(t) = _mm256_permute4x64_epi64(std::get<5>(t), 0x4E);
        std::get<6>(t) = _mm256_permute4x64_epi64(std::get<6>(t), 0x4E);
        std::get<7>(t) = _mm256_permute4x64_epi64(std::get<7>(t), 0x4E);
    }
}; // class ThreefryGeneratorImpl
