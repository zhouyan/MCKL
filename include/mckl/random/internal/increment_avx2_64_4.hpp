//============================================================================
// MCKL/include/mckl/random/internal/increment_avx2_64_4.hpp
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

template <typename T>
class IncrementBlock<T, 1, 16, 64>
{
    static constexpr std::size_t K_ = 1;
    static constexpr std::size_t blocks_ = 16;

    public:
    static void eval(std::array<T, K_> &ctr,
        std::array<std::array<T, K_>, blocks_> &ctr_block)
    {
        __m256i a0 =
            _mm256_set1_epi64x(static_cast<MCKL_INT64>(std::get<0>(ctr)));

        __m256i c0 =
            _mm256_add_epi64(a0, _mm256_set_epi64x(0x04, 0x03, 0x02, 0x01));
        __m256i c1 =
            _mm256_add_epi64(a0, _mm256_set_epi64x(0x08, 0x07, 0x06, 0x05));
        __m256i c2 =
            _mm256_add_epi64(a0, _mm256_set_epi64x(0x0C, 0x0B, 0x0A, 0x09));
        __m256i c3 =
            _mm256_add_epi64(a0, _mm256_set_epi64x(0x10, 0x0F, 0x0E, 0x0D));

        __m256i *cptr = reinterpret_cast<__m256i *>(ctr_block.data());
        _mm256_storeu_si256(cptr++, c0);
        _mm256_storeu_si256(cptr++, c1);
        _mm256_storeu_si256(cptr++, c2);
        _mm256_storeu_si256(cptr++, c3);
    }
}; // class IncrementBlock

template <typename T>
class IncrementBlock<T, 2, 8, 64>
{
    static constexpr std::size_t K_ = 2;
    static constexpr std::size_t blocks_ = 8;

    public:
    static void eval(const std::array<T, K_> &ctr,
        std::array<std::array<T, K_>, blocks_> &ctr_block)
    {
        __m256i a0 =
            _mm256_set_epi64x(static_cast<MCKL_INT64>(std::get<1>(ctr)),
                static_cast<MCKL_INT64>(std::get<0>(ctr)),
                static_cast<MCKL_INT64>(std::get<1>(ctr)),
                static_cast<MCKL_INT64>(std::get<0>(ctr)));

        __m256i c0 = _mm256_add_epi64(a0, _mm256_set_epi64x(0, 2, 0, 1));
        __m256i c1 = _mm256_add_epi64(a0, _mm256_set_epi64x(0, 4, 0, 3));
        __m256i c2 = _mm256_add_epi64(a0, _mm256_set_epi64x(0, 6, 0, 5));
        __m256i c3 = _mm256_add_epi64(a0, _mm256_set_epi64x(0, 8, 0, 7));

        __m256i *cptr = reinterpret_cast<__m256i *>(ctr_block.data());
        _mm256_storeu_si256(cptr++, c0);
        _mm256_storeu_si256(cptr++, c1);
        _mm256_storeu_si256(cptr++, c2);
        _mm256_storeu_si256(cptr++, c3);
    }
}; // class IncrementBlock

template <typename T>
class IncrementBlock<T, 4, 4, 64>
{
    static constexpr std::size_t K_ = 4;
    static constexpr std::size_t blocks_ = 4;

    public:
    static void eval(const std::array<T, K_> &ctr,
        std::array<std::array<T, K_>, blocks_> &ctr_block)
    {
        __m256i a0 =
            _mm256_set_epi64x(static_cast<MCKL_INT64>(std::get<3>(ctr)),
                static_cast<MCKL_INT64>(std::get<2>(ctr)),
                static_cast<MCKL_INT64>(std::get<1>(ctr)),
                static_cast<MCKL_INT64>(std::get<0>(ctr)));

        __m256i c0 = _mm256_add_epi64(a0, _mm256_set_epi64x(0, 0, 0, 1));
        __m256i c1 = _mm256_add_epi64(a0, _mm256_set_epi64x(0, 0, 0, 2));
        __m256i c2 = _mm256_add_epi64(a0, _mm256_set_epi64x(0, 0, 0, 3));
        __m256i c3 = _mm256_add_epi64(a0, _mm256_set_epi64x(0, 0, 0, 4));

        __m256i *cptr = reinterpret_cast<__m256i *>(ctr_block.data());
        _mm256_storeu_si256(cptr++, c0);
        _mm256_storeu_si256(cptr++, c1);
        _mm256_storeu_si256(cptr++, c2);
        _mm256_storeu_si256(cptr++, c3);
    }
}; // class IncrementBlock

template <typename T>
class IncrementBlock<T, 8, 2, 64>
{
    static constexpr std::size_t K_ = 8;
    static constexpr std::size_t blocks_ = 2;

    public:
    static void eval(const std::array<T, K_> &ctr,
        std::array<std::array<T, K_>, blocks_> &ctr_block)
    {
        __m256i a0 =
            _mm256_set_epi64x(static_cast<MCKL_INT64>(std::get<3>(ctr)),
                static_cast<MCKL_INT64>(std::get<2>(ctr)),
                static_cast<MCKL_INT64>(std::get<1>(ctr)),
                static_cast<MCKL_INT64>(std::get<0>(ctr)));
        __m256i a1 =
            _mm256_set_epi64x(static_cast<MCKL_INT64>(std::get<7>(ctr)),
                static_cast<MCKL_INT64>(std::get<6>(ctr)),
                static_cast<MCKL_INT64>(std::get<5>(ctr)),
                static_cast<MCKL_INT64>(std::get<4>(ctr)));

        __m256i c0 = _mm256_add_epi64(a0, _mm256_set_epi64x(0, 0, 0, 1));
        __m256i c2 = _mm256_add_epi64(a0, _mm256_set_epi64x(0, 0, 0, 2));

        __m256i *cptr = reinterpret_cast<__m256i *>(ctr_block.data());
        _mm256_storeu_si256(cptr++, c0);
        _mm256_storeu_si256(cptr++, a1);
        _mm256_storeu_si256(cptr++, c2);
        _mm256_storeu_si256(cptr++, a1);
    }
}; // class IncrementBlock
