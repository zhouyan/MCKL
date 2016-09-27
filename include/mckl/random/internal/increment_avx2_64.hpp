//============================================================================
// MCKL/include/mckl/random/internal/increment_avx2_64.hpp
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

// AESNI
template <typename T>
class IncrementBlock<T, 2, 8, 64>
{
    static constexpr std::size_t K_ = 2;
    static constexpr std::size_t blocks_ = 8;

    public:
    static void eval(std::array<T, K_> &ctr,
        std::array<std::array<T, K_>, blocks_> &ctr_block)
    {
        __m256i c0 =
            _mm256_set_epi64x(static_cast<MCKL_INT64>(std::get<1>(ctr)),
                static_cast<MCKL_INT64>(std::get<0>(ctr)),
                static_cast<MCKL_INT64>(std::get<1>(ctr)),
                static_cast<MCKL_INT64>(std::get<0>(ctr)));
        c0 = _mm256_add_epi64(c0, _mm256_set_epi64x(0, 2, 0, 1));

        __m256i c1 = _mm256_add_epi64(c0, _mm256_set_epi64x(0, 2, 0, 2));
        __m256i c2 = _mm256_add_epi64(c0, _mm256_set_epi64x(0, 4, 0, 4));
        __m256i c3 = _mm256_add_epi64(c0, _mm256_set_epi64x(0, 6, 0, 6));

        __m256i *cptr = reinterpret_cast<__m256i *>(ctr_block.data());
        _mm256_store_si256(cptr++, c0);
        _mm256_store_si256(cptr++, c1);
        _mm256_store_si256(cptr++, c2);
        _mm256_store_si256(cptr++, c3);
    }
}; // class IncrementBlock

// Philox2x32
template <typename T>
class IncrementBlock<T, 1, 32, 64>
{
    static constexpr std::size_t K_ = 1;
    static constexpr std::size_t blocks_ = 32;

    public:
    static void eval(std::array<T, K_> &ctr,
        std::array<std::array<T, K_>, blocks_> &ctr_block)
    {
        __m256i c0 =
            _mm256_set1_epi64x(static_cast<MCKL_INT64>(std::get<0>(ctr)));
        c0 = _mm256_add_epi64(c0, _mm256_set_epi64x(4, 3, 2, 1));

        __m256i c1 = _mm256_add_epi64(c0, _mm256_set1_epi64x(0x04));
        __m256i c2 = _mm256_add_epi64(c0, _mm256_set1_epi64x(0x08));
        __m256i c3 = _mm256_add_epi64(c0, _mm256_set1_epi64x(0x0C));
        __m256i c4 = _mm256_add_epi64(c0, _mm256_set1_epi64x(0x10));
        __m256i c5 = _mm256_add_epi64(c0, _mm256_set1_epi64x(0x14));
        __m256i c6 = _mm256_add_epi64(c0, _mm256_set1_epi64x(0x18));
        __m256i c7 = _mm256_add_epi64(c0, _mm256_set1_epi64x(0x1C));

        __m256i *cptr = reinterpret_cast<__m256i *>(ctr_block.data());
        _mm256_store_si256(cptr++, c0);
        _mm256_store_si256(cptr++, c1);
        _mm256_store_si256(cptr++, c2);
        _mm256_store_si256(cptr++, c3);
        _mm256_store_si256(cptr++, c4);
        _mm256_store_si256(cptr++, c5);
        _mm256_store_si256(cptr++, c6);
        _mm256_store_si256(cptr++, c7);
    }
}; // class IncrementBlock

// Philox4x32
template <typename T>
class IncrementBlock<T, 2, 16, 64>
{
    static constexpr std::size_t K_ = 2;
    static constexpr std::size_t blocks_ = 16;

    public:
    static void eval(std::array<T, K_> &ctr,
        std::array<std::array<T, K_>, blocks_> &ctr_block)
    {
        __m256i c0 =
            _mm256_set_epi64x(static_cast<MCKL_INT64>(std::get<1>(ctr)),
                static_cast<MCKL_INT64>(std::get<0>(ctr)),
                static_cast<MCKL_INT64>(std::get<1>(ctr)),
                static_cast<MCKL_INT64>(std::get<0>(ctr)));
        c0 = _mm256_add_epi64(c0, _mm256_set_epi64x(0, 2, 0, 1));

        __m256i c1 = _mm256_add_epi64(c0, _mm256_set_epi64x(0, 0x2, 0, 0x2));
        __m256i c2 = _mm256_add_epi64(c0, _mm256_set_epi64x(0, 0x4, 0, 0x4));
        __m256i c3 = _mm256_add_epi64(c0, _mm256_set_epi64x(0, 0x6, 0, 0x6));
        __m256i c4 = _mm256_add_epi64(c0, _mm256_set_epi64x(0, 0x8, 0, 0x8));
        __m256i c5 = _mm256_add_epi64(c0, _mm256_set_epi64x(0, 0xA, 0, 0xA));
        __m256i c6 = _mm256_add_epi64(c0, _mm256_set_epi64x(0, 0xC, 0, 0xC));
        __m256i c7 = _mm256_add_epi64(c0, _mm256_set_epi64x(0, 0xE, 0, 0xE));

        __m256i *cptr = reinterpret_cast<__m256i *>(ctr_block.data());
        _mm256_store_si256(cptr++, c0);
        _mm256_store_si256(cptr++, c1);
        _mm256_store_si256(cptr++, c2);
        _mm256_store_si256(cptr++, c3);
        _mm256_store_si256(cptr++, c4);
        _mm256_store_si256(cptr++, c5);
        _mm256_store_si256(cptr++, c6);
        _mm256_store_si256(cptr++, c7);
    }
}; // class IncrementBlock

// Threefry2x32
template <typename T>
class IncrementBlock<T, 1, 64, 64>
{
    static constexpr std::size_t K_ = 1;
    static constexpr std::size_t blocks_ = 64;

    public:
    static void eval(std::array<T, K_> &ctr,
        std::array<std::array<T, K_>, blocks_> &ctr_block)
    {
        __m256i c0 =
            _mm256_set1_epi64x(static_cast<MCKL_INT64>(std::get<0>(ctr)));
        c0 = _mm256_add_epi64(c0, _mm256_set_epi64x(4, 3, 2, 1));

        __m256i c1 = _mm256_add_epi64(c0, _mm256_set1_epi64x(0x04));
        __m256i c2 = _mm256_add_epi64(c0, _mm256_set1_epi64x(0x08));
        __m256i c3 = _mm256_add_epi64(c0, _mm256_set1_epi64x(0x0C));
        __m256i c4 = _mm256_add_epi64(c0, _mm256_set1_epi64x(0x10));
        __m256i c5 = _mm256_add_epi64(c0, _mm256_set1_epi64x(0x14));
        __m256i c6 = _mm256_add_epi64(c0, _mm256_set1_epi64x(0x18));
        __m256i c7 = _mm256_add_epi64(c0, _mm256_set1_epi64x(0x1C));
        __m256i c8 = _mm256_add_epi64(c0, _mm256_set1_epi64x(0x20));
        __m256i c9 = _mm256_add_epi64(c0, _mm256_set1_epi64x(0x24));
        __m256i cA = _mm256_add_epi64(c0, _mm256_set1_epi64x(0x28));
        __m256i cB = _mm256_add_epi64(c0, _mm256_set1_epi64x(0x2C));
        __m256i cC = _mm256_add_epi64(c0, _mm256_set1_epi64x(0x30));
        __m256i cD = _mm256_add_epi64(c0, _mm256_set1_epi64x(0x34));
        __m256i cE = _mm256_add_epi64(c0, _mm256_set1_epi64x(0x38));
        __m256i cF = _mm256_add_epi64(c0, _mm256_set1_epi64x(0x3C));

        __m256i *cptr = reinterpret_cast<__m256i *>(ctr_block.data());
        _mm256_store_si256(cptr++, c0);
        _mm256_store_si256(cptr++, c1);
        _mm256_store_si256(cptr++, c2);
        _mm256_store_si256(cptr++, c3);
        _mm256_store_si256(cptr++, c4);
        _mm256_store_si256(cptr++, c5);
        _mm256_store_si256(cptr++, c6);
        _mm256_store_si256(cptr++, c7);
        _mm256_store_si256(cptr++, c8);
        _mm256_store_si256(cptr++, c9);
        _mm256_store_si256(cptr++, cA);
        _mm256_store_si256(cptr++, cB);
        _mm256_store_si256(cptr++, cC);
        _mm256_store_si256(cptr++, cD);
        _mm256_store_si256(cptr++, cE);
        _mm256_store_si256(cptr++, cF);
    }
}; // class IncrementBlock

// Threefry4x32, Threefry2x64
template <typename T>
class IncrementBlock<T, 2, 32, 64>
{
    static constexpr std::size_t K_ = 2;
    static constexpr std::size_t blocks_ = 32;

    public:
    static void eval(std::array<T, K_> &ctr,
        std::array<std::array<T, K_>, blocks_> &ctr_block)
    {
        __m256i c0 =
            _mm256_set_epi64x(static_cast<MCKL_INT64>(std::get<1>(ctr)),
                static_cast<MCKL_INT64>(std::get<0>(ctr)),
                static_cast<MCKL_INT64>(std::get<1>(ctr)),
                static_cast<MCKL_INT64>(std::get<0>(ctr)));
        c0 = _mm256_add_epi64(c0, _mm256_set_epi64x(0, 2, 0, 1));

        __m256i c1 = _mm256_add_epi64(c0, _mm256_set_epi64x(0, 0x02, 0, 0x02));
        __m256i c2 = _mm256_add_epi64(c0, _mm256_set_epi64x(0, 0x04, 0, 0x04));
        __m256i c3 = _mm256_add_epi64(c0, _mm256_set_epi64x(0, 0x06, 0, 0x06));
        __m256i c4 = _mm256_add_epi64(c0, _mm256_set_epi64x(0, 0x08, 0, 0x08));
        __m256i c5 = _mm256_add_epi64(c0, _mm256_set_epi64x(0, 0x0A, 0, 0x0A));
        __m256i c6 = _mm256_add_epi64(c0, _mm256_set_epi64x(0, 0x0C, 0, 0x0C));
        __m256i c7 = _mm256_add_epi64(c0, _mm256_set_epi64x(0, 0x0E, 0, 0x0E));
        __m256i c8 = _mm256_add_epi64(c0, _mm256_set_epi64x(0, 0x10, 0, 0x10));
        __m256i c9 = _mm256_add_epi64(c0, _mm256_set_epi64x(0, 0x12, 0, 0x12));
        __m256i cA = _mm256_add_epi64(c0, _mm256_set_epi64x(0, 0x14, 0, 0x14));
        __m256i cB = _mm256_add_epi64(c0, _mm256_set_epi64x(0, 0x16, 0, 0x16));
        __m256i cC = _mm256_add_epi64(c0, _mm256_set_epi64x(0, 0x18, 0, 0x18));
        __m256i cD = _mm256_add_epi64(c0, _mm256_set_epi64x(0, 0x1A, 0, 0x1A));
        __m256i cE = _mm256_add_epi64(c0, _mm256_set_epi64x(0, 0x1C, 0, 0x1C));
        __m256i cF = _mm256_add_epi64(c0, _mm256_set_epi64x(0, 0x1E, 0, 0x1E));

        __m256i *cptr = reinterpret_cast<__m256i *>(ctr_block.data());
        _mm256_store_si256(cptr++, c0);
        _mm256_store_si256(cptr++, c1);
        _mm256_store_si256(cptr++, c2);
        _mm256_store_si256(cptr++, c3);
        _mm256_store_si256(cptr++, c4);
        _mm256_store_si256(cptr++, c5);
        _mm256_store_si256(cptr++, c6);
        _mm256_store_si256(cptr++, c7);
        _mm256_store_si256(cptr++, c8);
        _mm256_store_si256(cptr++, c9);
        _mm256_store_si256(cptr++, cA);
        _mm256_store_si256(cptr++, cB);
        _mm256_store_si256(cptr++, cC);
        _mm256_store_si256(cptr++, cD);
        _mm256_store_si256(cptr++, cE);
        _mm256_store_si256(cptr++, cF);
    }
}; // class IncrementBlock

// Threefry4x64
template <typename T>
class IncrementBlock<T, 4, 16, 64>
{
    static constexpr std::size_t K_ = 4;
    static constexpr std::size_t blocks_ = 16;

    public:
    static void eval(std::array<T, K_> &ctr,
        std::array<std::array<T, K_>, blocks_> &ctr_block)
    {
        __m256i c0 =
            _mm256_set_epi64x(static_cast<MCKL_INT64>(std::get<3>(ctr)),
                static_cast<MCKL_INT64>(std::get<2>(ctr)),
                static_cast<MCKL_INT64>(std::get<1>(ctr)),
                static_cast<MCKL_INT64>(std::get<0>(ctr)));
        c0 = _mm256_add_epi64(c0, _mm256_set_epi64x(0, 0, 0, 1));

        __m256i c1 = _mm256_add_epi64(c0, _mm256_set_epi64x(0, 0, 0, 0x1));
        __m256i c2 = _mm256_add_epi64(c0, _mm256_set_epi64x(0, 0, 0, 0x2));
        __m256i c3 = _mm256_add_epi64(c0, _mm256_set_epi64x(0, 0, 0, 0x3));
        __m256i c4 = _mm256_add_epi64(c0, _mm256_set_epi64x(0, 0, 0, 0x4));
        __m256i c5 = _mm256_add_epi64(c0, _mm256_set_epi64x(0, 0, 0, 0x5));
        __m256i c6 = _mm256_add_epi64(c0, _mm256_set_epi64x(0, 0, 0, 0x6));
        __m256i c7 = _mm256_add_epi64(c0, _mm256_set_epi64x(0, 0, 0, 0x7));
        __m256i c8 = _mm256_add_epi64(c0, _mm256_set_epi64x(0, 0, 0, 0x8));
        __m256i c9 = _mm256_add_epi64(c0, _mm256_set_epi64x(0, 0, 0, 0x9));
        __m256i cA = _mm256_add_epi64(c0, _mm256_set_epi64x(0, 0, 0, 0xA));
        __m256i cB = _mm256_add_epi64(c0, _mm256_set_epi64x(0, 0, 0, 0xB));
        __m256i cC = _mm256_add_epi64(c0, _mm256_set_epi64x(0, 0, 0, 0xC));
        __m256i cD = _mm256_add_epi64(c0, _mm256_set_epi64x(0, 0, 0, 0xD));
        __m256i cE = _mm256_add_epi64(c0, _mm256_set_epi64x(0, 0, 0, 0xE));
        __m256i cF = _mm256_add_epi64(c0, _mm256_set_epi64x(0, 0, 0, 0xF));

        __m256i *cptr = reinterpret_cast<__m256i *>(ctr_block.data());
        _mm256_store_si256(cptr++, c0);
        _mm256_store_si256(cptr++, c1);
        _mm256_store_si256(cptr++, c2);
        _mm256_store_si256(cptr++, c3);
        _mm256_store_si256(cptr++, c4);
        _mm256_store_si256(cptr++, c5);
        _mm256_store_si256(cptr++, c6);
        _mm256_store_si256(cptr++, c7);
        _mm256_store_si256(cptr++, c8);
        _mm256_store_si256(cptr++, c9);
        _mm256_store_si256(cptr++, cA);
        _mm256_store_si256(cptr++, cB);
        _mm256_store_si256(cptr++, cC);
        _mm256_store_si256(cptr++, cD);
        _mm256_store_si256(cptr++, cE);
        _mm256_store_si256(cptr++, cF);
    }
}; // class IncrementBlock

// Threefry8x64
template <typename T>
class IncrementBlock<T, 8, 8, 64>
{
    static constexpr std::size_t K_ = 8;
    static constexpr std::size_t blocks_ = 8;

    public:
    static void eval(std::array<T, K_> &ctr,
        std::array<std::array<T, K_>, blocks_> &ctr_block)
    {
        __m256i c0 =
            _mm256_set_epi64x(static_cast<MCKL_INT64>(std::get<3>(ctr)),
                static_cast<MCKL_INT64>(std::get<2>(ctr)),
                static_cast<MCKL_INT64>(std::get<1>(ctr)),
                static_cast<MCKL_INT64>(std::get<0>(ctr)));
        __m256i c1 =
            _mm256_set_epi64x(static_cast<MCKL_INT64>(std::get<7>(ctr)),
                static_cast<MCKL_INT64>(std::get<6>(ctr)),
                static_cast<MCKL_INT64>(std::get<5>(ctr)),
                static_cast<MCKL_INT64>(std::get<4>(ctr)));
        c0 = _mm256_add_epi64(c0, _mm256_set_epi64x(0, 0, 0, 1));

        __m256i c2 = _mm256_add_epi64(c0, _mm256_set_epi64x(0, 0, 0, 1));
        __m256i c4 = _mm256_add_epi64(c0, _mm256_set_epi64x(0, 0, 0, 2));
        __m256i c6 = _mm256_add_epi64(c0, _mm256_set_epi64x(0, 0, 0, 3));
        __m256i c8 = _mm256_add_epi64(c0, _mm256_set_epi64x(0, 0, 0, 4));
        __m256i cA = _mm256_add_epi64(c0, _mm256_set_epi64x(0, 0, 0, 5));
        __m256i cC = _mm256_add_epi64(c0, _mm256_set_epi64x(0, 0, 0, 6));
        __m256i cE = _mm256_add_epi64(c0, _mm256_set_epi64x(0, 0, 0, 7));

        __m256i *cptr = reinterpret_cast<__m256i *>(ctr_block.data());
        _mm256_store_si256(cptr++, c0);
        _mm256_store_si256(cptr++, c1);
        _mm256_store_si256(cptr++, c2);
        _mm256_store_si256(cptr++, c1);
        _mm256_store_si256(cptr++, c4);
        _mm256_store_si256(cptr++, c1);
        _mm256_store_si256(cptr++, c6);
        _mm256_store_si256(cptr++, c1);
        _mm256_store_si256(cptr++, c8);
        _mm256_store_si256(cptr++, c1);
        _mm256_store_si256(cptr++, cA);
        _mm256_store_si256(cptr++, c1);
        _mm256_store_si256(cptr++, cC);
        _mm256_store_si256(cptr++, c1);
        _mm256_store_si256(cptr++, cE);
        _mm256_store_si256(cptr++, c1);
    }
}; // class IncrementBlock

// Threefry16x64
template <typename T>
class IncrementBlock<T, 16, 4, 64>
{
    static constexpr std::size_t K_ = 16;
    static constexpr std::size_t blocks_ = 4;

    public:
    static void eval(std::array<T, K_> &ctr,
        std::array<std::array<T, K_>, blocks_> &ctr_block)
    {
        __m256i c0 =
            _mm256_set_epi64x(static_cast<MCKL_INT64>(std::get<0x3>(ctr)),
                static_cast<MCKL_INT64>(std::get<0x2>(ctr)),
                static_cast<MCKL_INT64>(std::get<0x1>(ctr)),
                static_cast<MCKL_INT64>(std::get<0x0>(ctr)));
        __m256i c1 =
            _mm256_set_epi64x(static_cast<MCKL_INT64>(std::get<0x7>(ctr)),
                static_cast<MCKL_INT64>(std::get<0x6>(ctr)),
                static_cast<MCKL_INT64>(std::get<0x5>(ctr)),
                static_cast<MCKL_INT64>(std::get<0x4>(ctr)));
        __m256i c2 =
            _mm256_set_epi64x(static_cast<MCKL_INT64>(std::get<0xB>(ctr)),
                static_cast<MCKL_INT64>(std::get<0xA>(ctr)),
                static_cast<MCKL_INT64>(std::get<0x9>(ctr)),
                static_cast<MCKL_INT64>(std::get<0x8>(ctr)));
        __m256i c3 =
            _mm256_set_epi64x(static_cast<MCKL_INT64>(std::get<0xF>(ctr)),
                static_cast<MCKL_INT64>(std::get<0xE>(ctr)),
                static_cast<MCKL_INT64>(std::get<0xD>(ctr)),
                static_cast<MCKL_INT64>(std::get<0xC>(ctr)));
        c0 = _mm256_add_epi64(c0, _mm256_set_epi64x(0, 0, 0, 1));

        __m256i c4 = _mm256_add_epi64(c0, _mm256_set_epi64x(0, 0, 0, 1));
        __m256i c8 = _mm256_add_epi64(c0, _mm256_set_epi64x(0, 0, 0, 2));
        __m256i cC = _mm256_add_epi64(c0, _mm256_set_epi64x(0, 0, 0, 3));

        __m256i *cptr = reinterpret_cast<__m256i *>(ctr_block.data());
        _mm256_store_si256(cptr++, c0);
        _mm256_store_si256(cptr++, c1);
        _mm256_store_si256(cptr++, c2);
        _mm256_store_si256(cptr++, c3);
        _mm256_store_si256(cptr++, c4);
        _mm256_store_si256(cptr++, c1);
        _mm256_store_si256(cptr++, c2);
        _mm256_store_si256(cptr++, c3);
        _mm256_store_si256(cptr++, c8);
        _mm256_store_si256(cptr++, c1);
        _mm256_store_si256(cptr++, c2);
        _mm256_store_si256(cptr++, c3);
        _mm256_store_si256(cptr++, cC);
        _mm256_store_si256(cptr++, c1);
        _mm256_store_si256(cptr++, c2);
        _mm256_store_si256(cptr++, c3);
    }
}; // class IncrementBlock
